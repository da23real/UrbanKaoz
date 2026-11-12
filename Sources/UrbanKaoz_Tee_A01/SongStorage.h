#pragma once
// Alternating snapshots: an interrupted write never destroys the previous bank.
struct SongDiskHeader {uint32_t magic,version,generation,count,nextId,start,end,crc;uint8_t loop,view,reservedLegacy,reserved;};
uint32_t songDiskGeneration=0;
EXTMEM NoteLock817 songLocksStageR10[LOCK_CAPACITY817];uint16_t songLocksCountR10=0;
FLASHMEM uint32_t songHash(uint32_t h,const void*data,size_t n){const uint8_t*p=(const uint8_t*)data;while(n--){h^=*p++;h*=16777619u;}return h;}
FLASHMEM uint32_t songDiskHash(SongDiskHeader h,const SongClip*c,const ZLEvent*z){h.crc=0;uint32_t crc=songHash(2166136261u,&h,sizeof(h));crc=songHash(crc,c,sizeof(songClips));return songHash(crc,z,h.count*sizeof(ZLEvent));}
FLASHMEM bool songReadBank(uint8_t bank,SongDiskHeader&h,SongClip*c,ZLEvent*z){
  char path[64];snprintf(path,sizeof(path),"%s/song%u.UKS",songFolder,bank);File f=SD.open(path,FILE_READ);
  if(!f)return false;bool ok=f.read((uint8_t*)&h,sizeof(h))==sizeof(h)&&h.magic==0x554B5A4Cu&&(h.version==1||h.version==3)&&h.count<=ZL_CAPACITY&&h.start>=1&&h.end>=h.start&&h.end<=62500000&&h.loop<2&&h.view<3&&h.nextId>0;
  if(ok)ok=f.size()>=sizeof(h)+sizeof(songClips)+h.count*sizeof(ZLEvent)&&f.read((uint8_t*)c,sizeof(songClips))==sizeof(songClips)&&f.read((uint8_t*)z,h.count*sizeof(ZLEvent))==h.count*sizeof(ZLEvent);
  songLocksCountR10=0;if(ok&&h.version==3)ok=f.read((uint8_t*)&songLocksCountR10,2)==2&&songLocksCountR10<=LOCK_CAPACITY817&&f.read((uint8_t*)songLocksStageR10,songLocksCountR10*sizeof(NoteLock817))==songLocksCountR10*sizeof(NoteLock817);
  if(ok)ok=f.position()==f.size();f.close();
  if(!ok)return false;
  uint32_t hash=songDiskHash(h,c,z);if(h.version==3){hash=songHash(hash,&songLocksCountR10,2);hash=songHash(hash,songLocksStageR10,songLocksCountR10*sizeof(NoteLock817));}
  if(ok)ok=h.crc==hash;
  for(uint16_t i=0;i<songLocksCountR10&&ok;++i){auto&l=songLocksStageR10[i];ok=l.pattern==254&&lockValid817(l);bool found=false;for(uint32_t j=0;j<h.count&&ok;++j)if(z[j].id==(uint32_t)l.tick&&!z[j].kind&&z[j].track==l.track&&z[j].a==l.note){found=true;break;}ok&=found;}
  for(uint32_t i=0;ok&&i<h.count;++i)ok=zlValid(z[i])&&z[i].id>0&&z[i].id<h.nextId&&(!i||z[i].tick>=z[i-1].tick);
  for(uint16_t i=0;i<SONG_CLIP_CAPACITY&&ok;++i)if(c[i].used)ok=songClipValid(c[i],1024);
  return ok;
}
FLASHMEM bool songSaveWorkspace(){
  if(!songWorkspaceReady||!storageWriteAvailable||playing){songError("STOP_AND_CHECK_PSRAM_SD");return false;}
  SongDiskHeader old;SongClip oldClips[SONG_CLIP_CAPACITY];
  for(uint8_t b=0;b<2;++b)if(songReadBank(b,old,oldClips,zlStaging)&&old.generation>songDiskGeneration)songDiskGeneration=old.generation;
  if(songDiskGeneration==UINT32_MAX){songError("SONG_GENERATION_LIMIT");return false;}
  SongDiskHeader h={};h.magic=0x554B5A4Cu;h.version=3;h.generation=songDiskGeneration+1;h.count=zlCount;h.nextId=zlNextId;h.start=songStartBar;h.end=songEndBar;h.loop=songLoopEnabled;h.view=songView;h.reservedLegacy=0;h.crc=songDiskHash(h,songClips,zlEvents);
  songLocksCountR10=0;for(uint16_t i=0;i<lockCount817;++i)if(noteLocks817[i].pattern==254)songLocksStageR10[songLocksCountR10++]=noteLocks817[i];
  h.crc=songHash(h.crc,&songLocksCountR10,2);h.crc=songHash(h.crc,songLocksStageR10,songLocksCountR10*sizeof(NoteLock817));
  SD.mkdir(songFolder);char path[64];snprintf(path,sizeof(path),"%s/song%u.UKS",songFolder,(unsigned)(h.generation%2));SD.remove(path);File f=SD.open(path,FILE_WRITE);
  bool ok=f&&f.write((uint8_t*)&h,sizeof(h))==sizeof(h)&&f.write((uint8_t*)songClips,sizeof(songClips))==sizeof(songClips)&&f.write((uint8_t*)zlEvents,zlCount*sizeof(ZLEvent))==zlCount*sizeof(ZLEvent)&&f.write((uint8_t*)&songLocksCountR10,2)==2&&f.write((uint8_t*)songLocksStageR10,songLocksCountR10*sizeof(NoteLock817))==songLocksCountR10*sizeof(NoteLock817);if(f){f.flush();if(f.getWriteError())ok=false;f.close();}
  SongDiskHeader verify;SongClip clips[SONG_CLIP_CAPACITY];if(ok)ok=songReadBank(h.generation%2,verify,clips,zlStaging);
  if(ok){songDiskGeneration=h.generation;songSend("SAVED,%s",path);DISPLAY_SERIAL.println("MSG|SONG SAVED");}else songError("SONG_SAVE_FAILED_PREVIOUS_BANK_KEPT");return ok;
}
FLASHMEM bool songLoadWorkspace(){
  if(!songWorkspaceReady||!sdAvailable||playing){songError("STOP_AND_CHECK_PSRAM_SD");return false;}
  SongDiskHeader best={},h;SongClip clips[SONG_CLIP_CAPACITY];int bank=-1;
  for(uint8_t b=0;b<2;++b)if(songReadBank(b,h,clips,zlStaging)&&(bank<0||h.generation>best.generation)){best=h;bank=b;}
  if(bank<0){songError("NO_VALID_SONG_SNAPSHOT");return false;}
  if(!songReadBank(bank,h,clips,zlStaging)){songError("SONG_READ_FAILED");return false;}
  uint16_t other=0;for(uint16_t i=0;i<lockCount817;++i)if(noteLocks817[i].pattern!=254)++other;if(other+songLocksCountR10>LOCK_CAPACITY817){songError("PLOCK_CAPACITY");return false;}
  kbSongRemoveR10(0);for(uint16_t i=0;i<songLocksCountR10;++i)noteLocks817[lockCount817++]=songLocksStageR10[i];
  memcpy(songClips,clips,sizeof(songClips));memcpy(zlEvents,zlStaging,h.count*sizeof(ZLEvent));zlCount=h.count;zlNextId=h.nextId;songStartBar=h.start;songEndBar=h.end;songLoopEnabled=h.loop;songView=h.view;songDiskGeneration=h.generation;++songRevision;songSnapshot();return true;
}
