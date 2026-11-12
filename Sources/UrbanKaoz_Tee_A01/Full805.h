#pragma once
#include "DelayDisk907.h"
uint16_t fullPatternLinks911[16][16];
uint8_t fullLinkColor55[16][16];
uint8_t fullVolume818[16];uint16_t fullSolo818=0;
struct FullExtra805 {
  uint32_t magic, size, zlCount, nextId, start, end;
  uint8_t loop, view, reservedLegacy, preview;
  uint16_t muted, reserved;
};
uint16_t fullGrooveInitialized26=0;
uint16_t fullGap810=2,fullBudget810=500;
struct FullFooter805 { uint32_t magic, generation, hash; };
EXTMEM SongClip fullClips805[SONG_CLIP_CAPACITY];
EXTMEM ModUnit805 fullMods805[64];
EXTMEM Delay805 fullDelays805[16][16];
EXTMEM VirtualLink805 fullLinks805[32];
EXTMEM ModMeta806 fullMeta806[64];
#include "ModDisk931.h"
FLASHMEM uint32_t full805Hash(File &f,uint32_t count) {
  uint8_t buffer[4096];uint32_t hash=2166136261u;
  if(!f.seek(0))return 0;
  while(count){uint32_t n=min((uint32_t)sizeof(buffer),count);if(f.read(buffer,n)!=(int)n)return 0;
    for(uint32_t i=0;i<n;++i){hash^=buffer[i];hash*=16777619u;}count-=n;serviceNoteOffs();display821.service();}
  return hash;
}
FLASHMEM bool full805Append(File &f) {
  FullExtra805 h={0xA55554Bu,sizeof(FullExtra805),zlCount,zlNextId,songStartBar,songEndBar,
    (uint8_t)songLoopEnabled,songView,(uint8_t)(0x80|stepLayout25|(headTrail25<<2)),(uint8_t)trackNavigationPreview};
  for(uint8_t t=0;t<16;++t){if(trackMuted[t])h.muted|=1u<<t;if(rollMod827[t])h.reserved|=1u<<t;}
  bool ok=f.write((uint8_t*)&h,sizeof(h))==sizeof(h)&&
    f.write((uint8_t*)songClips,sizeof(songClips))==sizeof(songClips)&&
    f.write((uint8_t*)zlEvents,zlCount*sizeof(ZLEvent))==zlCount*sizeof(ZLEvent)&&
    modWriteUnits931(f)&&
    f.write((uint8_t*)delays805,sizeof(delays805))==sizeof(delays805)&&
    f.write((uint8_t*)virtualLinks805,sizeof(virtualLinks805))==sizeof(virtualLinks805)&&
    modWriteMeta931(f);
  uint32_t patchSize=patch808DiskSize();
  if(ok)ok=f.write((uint8_t*)&patchSize,4)==4&&patch808Write(f);
  if(ok)ok=modWriteExtra931(f)&&f.write((uint8_t*)&ccGap810,2)==2&&f.write((uint8_t*)&ccBudget810,2)==2;
  if(ok)ok=f.write((uint8_t*)delayExtra817,sizeof(delayExtra817))==sizeof(delayExtra817)&&([&](){uint16_t count=lockCount817|0x8000u;return f.write((uint8_t*)&count,2)==2;})()&&modWriteLocks931(f);
  if(ok)ok=f.write(trackVolume818,16)==16&&f.write((uint8_t*)&trackSolo818,2)==2;
  if(ok)ok=delayDiskWrite907(f);
  if(ok)ok=f.write((uint8_t*)patternLinks911,sizeof(patternLinks911))==sizeof(patternLinks911);
  if(ok)ok=rollWrite1000(f)&&mixerWrite938(f)&&programWrite939(f)&&ccWrite941(f)&&kbWriteR10(f);
  if(ok)ok=midiInWrite17(f)&&blockWrite21(f)&&f.write((uint8_t*)&grooveInitialized26,2)==2;
  if(ok)ok=f.write((uint8_t*)fxConfig32,sizeof(fxConfig32))==sizeof(fxConfig32);
  if(!orderReady51)orderReset51();if(ok)ok=autoWrite46(f)&&indWrite50(f)&&f.write((uint8_t*)patternOrder51,256)==256;
  if(ok)ok=f.write((uint8_t*)patternLinkColor55,256)==256;
  FullFooter805 footer={0x8054546u,full805Generation,0};
  if(ok)ok=f.write((uint8_t*)&footer,8)==8;
  f.flush();uint32_t end=f.position();
  if(ok){footer.hash=full805Hash(f,end);ok=footer.hash&&f.seek(end)&&f.write((uint8_t*)&footer.hash,4)==4;}
  f.flush();return ok&&!f.getWriteError();
}
FLASHMEM bool full807Error(const char*reason){Serial.print("FULL,INVALID,");Serial.println(reason);return false;}
FLASHMEM bool full805ReadExtra(File &f,FullExtra805 &h,bool verifyHash=true) {
  ModStageScope931 stageScope;
  delayClear907(delayStage907);delayProjectFile925=false;
  bool read42=f.read((uint8_t*)&h,sizeof(h))==sizeof(h);bool version55=h.magic==0xA55554Bu;bool orderVersion51=version55||h.magic==0xA51554Bu;bool indVersion50=orderVersion51||h.magic==0xA50554Bu;bool auto46=indVersion50||h.magic==0xA46554Bu;bool wide42=auto46||h.magic==0xA42554Bu;if(wide42)h.magic=0xA27554Bu;
  bool ok=read42&&(h.magic==0x805554Bu||h.magic==0x806554Bu||h.magic==0x808554Bu||(h.magic==0x810554Bu||h.magic==0x811554Bu||h.magic==0x817554Bu||h.magic==0x818554Bu||h.magic==0x819554Bu||h.magic==0x907554Bu||(h.magic==0x911554Bu||(h.magic==0x927554Bu||(h.magic==0x928554Bu||(h.magic==0x930554Bu||(h.magic==0x938554Bu||(h.magic==0x939554Bu||(h.magic==0x941554Bu||(h.magic==0xA10554Bu||(h.magic==0xA17554Bu||(h.magic==0xA26554Bu||h.magic==0xA27554Bu))))))))))))&&h.size==sizeof(h)&&
    h.zlCount<=ZL_CAPACITY&&h.nextId>0&&h.start>=1&&h.end>=h.start&&h.end<=62500000&&h.loop<2&&h.view<3&&(h.reservedLegacy<2||((h.reservedLegacy&0xF0)==0x80&&((h.reservedLegacy>>2)&3)<3))&&h.preview<2;
  if(!ok||external_psram_size<16)return full807Error("EXTRA_HEADER_OR_PSRAM");
  uint32_t expected=sizeof(songClips)+h.zlCount*sizeof(ZLEvent)+512*(wide42?sizeof(ModUnit805):sizeof(ModLegacy42))+sizeof(delays805)+sizeof(virtualLinks805)+sizeof(FullFooter805);
  if(h.magic>=0x806554Bu)expected+=512*sizeof(ModLegacyMeta931);
  if(h.magic>=0x808554Bu?f.size()-f.position()<expected+4:f.size()-f.position()!=expected)return full807Error("EXTRA_SIZE");
  ok=f.read((uint8_t*)fullClips805,sizeof(fullClips805))==sizeof(fullClips805)&&
    f.read((uint8_t*)zlStaging,h.zlCount*sizeof(ZLEvent))==h.zlCount*sizeof(ZLEvent)&&
    modReadUnits931(f,wide42)&&
    f.read((uint8_t*)fullDelays805,sizeof(fullDelays805))==sizeof(fullDelays805)&&
    f.read((uint8_t*)fullLinks805,sizeof(fullLinks805))==sizeof(fullLinks805);
  memset(fullMeta806,0,sizeof(fullMeta806));
  if(ok&&h.magic>=0x806554Bu)ok=modReadMeta931(f);
  if(ok&&h.magic==0x805554Bu)for(uint16_t old=0;old<512;++old)if(fullMap931[old]>=0)mod806Associate(fullMeta806[fullMap931[old]],old/32,0,true);
  for(uint16_t i=0;i<SONG_CLIP_CAPACITY&&ok;++i)if(fullClips805[i].used)ok=songClipValid(fullClips805[i],1024);
  for(uint32_t i=0;i<h.zlCount&&ok;++i)ok=zlValid(zlStaging[i])&&zlStaging[i].id>0&&zlStaging[i].id<h.nextId&&(!i||zlStaging[i].tick>=zlStaging[i-1].tick);
  for(uint8_t t=0;t<16&&ok;++t)for(uint8_t p=0;p<16&&ok;++p){
    const auto&d=fullDelays805[t][p];ok=d.enabled<2&&d.ticks>=1&&d.ticks<=3072&&d.repeats<=16&&d.velocity<=127;
  }
  for(uint16_t key=0;key<64&&ok;++key){auto&m=fullMods805[key];ok=m.enabled<2&&((h.magic==0x810554Bu||h.magic==0x811554Bu||h.magic>=0x817554Bu)||(m.hold<=999&&m.attack<=999&&m.decay<=999&&m.release<=999&&m.rate<=3072))&&m.sustain<=127&&m.curve<3&&m.shape<((h.magic==0x810554Bu||h.magic==0x811554Bu||h.magic>=0x817554Bu)?5:4)&&m.rate>=1;
      for(auto&r:m.outputs){
        if(r.kind>=17&&r.kind<=24){r={};continue;} // Removed destinations in legacy files.
        if((r.block21>2&&!modTrackMidi43(r)&&!ioVirtual53(r))||(r.kind==MOD_CV46&&r.track>3)||r.port>8||r.channel>15||r.reference<(r.kind==MOD_MIDI_BEND806?-8192:0)||r.reference>(r.kind==MOD_MIDI_BEND806?8191:r.kind==MOD_CV46?4095:127)||r.enabled>1||(r.kind>MOD_TRACK21&&r.kind!=MOD_CV46)||r.track>15||(r.number>127&&!modIsTarget811(r))||(r.kind==MOD_PARAM811?(r.number%8>=modParamCount811(modTarget810(r))):(r.depth< -modLimit810(r.kind)||r.depth>modLimit810(r.kind))))ok=false;
      }
  }
  if(!ok)return full807Error("CLIP_EVENT_MOD_OR_DELAY_RANGE");
  for(const auto&r:fullLinks805)if(r.enabled>1||r.bus>63||r.kind>MOD_RLV||r.track>15||r.depth<-8192||r.depth>8191)ok=false;
  if(ok&&h.magic==0x805554Bu)for(auto &mod:fullMods805)
    for(auto &r:mod.outputs)if(r.kind==MOD_CC)r.track=midiChannelForTrack(r.track);
  if(!ok)return full807Error("VIRTUAL_LINK_RANGE");
  if(ok&&h.magic>=0x808554Bu){
    uint32_t extraBytes=(h.magic==0x810554Bu||h.magic==0x811554Bu||h.magic>=0x817554Bu)?512*sizeof(ModExtra810)+4:0;
    if(h.magic>=0x817554Bu)extraBytes+=sizeof(delayExtra817)+2+LOCK_CAPACITY817*16; // minimum legacy size; record flag validates actual payload
    if(h.magic>=0x818554Bu)extraBytes+=18;
    uint32_t n=0;ok=f.read((uint8_t*)&n,4)==4&&f.size()-f.position()>=sizeof(FullFooter805)+extraBytes&&(h.magic>=0x907554Bu?n<=f.size()-f.position()-sizeof(FullFooter805)-extraBytes:n==f.size()-f.position()-sizeof(FullFooter805)-extraBytes);
    uint32_t end=f.position()+n;
    if(ok)ok=patch808Read(f,false)&&f.position()==end;
    if(!ok)return full807Error("PATCH_DATA");
  }
  if(ok&&(h.magic==0x810554Bu||h.magic==0x811554Bu||h.magic>=0x817554Bu)){
    ok=modReadExtra931(f)&&f.read((uint8_t*)&fullGap810,2)==2&&f.read((uint8_t*)&fullBudget810,2)==2;
    for(const auto&e:fullExtra810)if(e.mode>6||e.phase>360||e.attackCurve< -100||e.attackCurve>100||e.decayCurve< -100||e.decayCurve>100||e.releaseCurve< -100||e.releaseCurve>100)ok=false;
    if(fullGap810>100||fullBudget810<50||fullBudget810>900||!modGraphValid810(fullMods805))ok=false;
    graphDirty810=true;if(!ok)return full807Error("REALTIME_DATA");
  }
  fullLockCount817=0;memset(fullLocks817,0,sizeof(fullLocks817));
  for(auto&track:fullDelayExtra817)for(auto&e:track)e={0,16,1};
  if(ok&&h.magic>=0x817554Bu){
    ok=f.read((uint8_t*)fullDelayExtra817,sizeof(fullDelayExtra817))==sizeof(fullDelayExtra817)&&f.read((uint8_t*)&fullLockCount817,2)==2;
    if(ok){bool extended=(fullLockCount817&0x8000u)!=0;fullLockCount817&=0x7FFFu;ok=fullLockCount817<=LOCK_CAPACITY817;memset(fullLocks817,0,sizeof(fullLocks817));for(uint16_t i=0;i<LOCK_CAPACITY817&&ok;++i){size_t n=extended?sizeof(NoteLock817):16;ok=f.read((uint8_t*)&fullLocks817[i],n)==n;}}
    for(auto&track:fullDelayExtra817)for(auto&e:track)if(e.wait>3072||e.output>16||e.rolls>1)ok=false;
    for(uint16_t i=0;i<fullLockCount817&&ok;++i){auto&l=fullLocks817[i];if(modLock931(l.kind)){if(l.target>=512||fullMap931[l.target]<0){lockDiagnostic45("DISK_TARGET",i,l);ok=false;break;}l.target=fullMap931[l.target];}ok=lockValid817(l);if(!ok)lockDiagnostic45("DISK_VALUE",i,l);}
    if(!ok)return full807Error("PLOCK_DELAY_DATA");
  }
  memset(fullVolume818,90,16);fullSolo818=0;
  if(ok&&h.magic>=0x818554Bu){ok=f.read(fullVolume818,16)==16&&f.read((uint8_t*)&fullSolo818,2)==2;for(auto v:fullVolume818)if(v>127)ok=false;if(!ok)return full807Error("VOLUME_DATA");}
  if(ok&&h.magic>=0x907554Bu)ok=delayDiskRead907(f);
  else if(ok){for(uint8_t t=0;t<16&&ok;++t)for(uint8_t p=0;p<16&&ok;++p)if(fullDelays805[t][p].enabled)ok=delayStageAdd907(t*16+p,{(int8_t)fullDelays805[t][p].velocity,0,0,2});}
  if(!ok)return full807Error("DELAY907_DATA");
  memset(fullPatternLinks911,0,sizeof(fullPatternLinks911));
  if(ok&&h.magic>=0x911554Bu)ok=f.read((uint8_t*)fullPatternLinks911,sizeof(fullPatternLinks911))==sizeof(fullPatternLinks911);
  if(ok)ok=rollRead1000(f,(h.magic==0x927554Bu||(h.magic==0x928554Bu||(h.magic==0x930554Bu||(h.magic==0x938554Bu||(h.magic==0x939554Bu||(h.magic==0x941554Bu||(h.magic==0xA10554Bu||(h.magic==0xA17554Bu||(h.magic==0xA26554Bu||h.magic==0xA27554Bu))))))))));
  if(ok)ok=mixerRead938(f,(h.magic==0x938554Bu||(h.magic==0x939554Bu||(h.magic==0x941554Bu||(h.magic==0xA10554Bu||(h.magic==0xA17554Bu||(h.magic==0xA26554Bu||h.magic==0xA27554Bu)))))));
  if(ok)ok=programRead939(f,(h.magic==0x939554Bu||(h.magic==0x941554Bu||(h.magic==0xA10554Bu||(h.magic==0xA17554Bu||(h.magic==0xA26554Bu||h.magic==0xA27554Bu))))));
  if(ok)ok=ccRead941(f,(h.magic==0x941554Bu||(h.magic==0xA10554Bu||(h.magic==0xA17554Bu||(h.magic==0xA26554Bu||h.magic==0xA27554Bu)))));
  if(ok)ok=kbReadR10(f,(h.magic==0xA10554Bu||(h.magic==0xA17554Bu||(h.magic==0xA26554Bu||h.magic==0xA27554Bu))));
  if(ok)ok=midiInRead17(f,(h.magic==0xA17554Bu||(h.magic==0xA26554Bu||h.magic==0xA27554Bu)));
  if(ok)ok=blockRead21(f,false);
  if(h.magic!=0xA27554Bu)for(uint16_t k=0;k<64;++k)if(fullKinds931[k])fullMods805[k].sustain=80;
  fullGrooveInitialized26=0;if(ok&&(h.magic==0xA26554Bu||h.magic==0xA27554Bu))ok=f.read((uint8_t*)&fullGrooveInitialized26,2)==2;
  if(ok)ok=f.read((uint8_t*)fxStage32,sizeof(fxStage32))==sizeof(fxStage32);for(auto&c:fxStage32)if(!harmony32::valid(c))ok=false;
  if(ok)ok=autoRead46(f,auto46)&&indRead50(f,indVersion50);for(uint8_t t=0;t<16;++t)for(uint8_t p=0;p<16;++p)fullOrder51[t][p]=p;if(ok&&orderVersion51){ok=f.read((uint8_t*)fullOrder51,256)==256;for(auto&row:fullOrder51){uint16_t used=0;for(auto p:row){if(p>=16||(used&(1u<<p))){ok=false;break;}used|=1u<<p;}}}
  for(uint8_t t=0;t<16;++t)for(uint8_t p=0;p<16;++p)fullLinkColor55[t][p]=1+(fullPatternLinks911[t][p]?fullPatternLinks911[t][p]-1:0)%(NEO_PALETTE_COUNT-1);
  if(ok&&version55){ok=f.read((uint8_t*)fullLinkColor55,256)==256;for(auto&row:fullLinkColor55)for(auto c:row)if(c>=NEO_PALETTE_COUNT)ok=false;}
  FullFooter805 footer={};if(ok)ok=f.read((uint8_t*)&footer,sizeof(footer))==sizeof(footer)&&footer.magic==0x8054546u&&f.position()==f.size();
  if(ok&&verifyHash)ok=footer.hash==full805Hash(f,f.size()-4);
  if(!ok)return full807Error("FOOTER_OR_HASH");return true;
}
FLASHMEM bool full805Read(File &f,bool verified) {
  FullExtra805 h={};if(!full805ReadExtra(f,h,!verified))return false;memcpy(patternOrder51,fullOrder51,256);orderReady51=true;
  if(h.magic>=0x808554Bu){
    patchResolve10(patchStage808);
    patch808ClearClipboard();
    patch808Free(patchDb808);patchDb808=patchStage808;memset(&patchStage808,0,sizeof(patchStage808));
    memcpy(modRat808,fullRat808,sizeof(modRat808));midiAdvance808=fullMidiAdvance808;installPrefs933();
  }else patch808Init();
  delayClear907(delayNodes907);delayNodes907=delayStage907;delayStage907=nullptr;delayImportPending925=true;
  midiInRelease17();memcpy(midiInChannel17,midiInStage17,16);
  grooveInitialized26=fullGrooveInitialized26;memcpy(fxConfig32,fxStage32,sizeof(fxConfig32));fxRebuild32();
  autoInstall46();blockInstall21();kbInstallR10();rollInstall1000();mixerInstall938();programInstall939();memcpy(ccEnv941,ccStage941,sizeof(ccEnv941));memset(ccSeen941,0,sizeof(ccSeen941));memset(ioSeen38,0,sizeof(ioSeen38));
  memcpy(patternLinks911,fullPatternLinks911,sizeof(patternLinks911));memcpy(patternLinkColor55,fullLinkColor55,256);
  memcpy(songClips,fullClips805,sizeof(songClips));memcpy(zlEvents,zlStaging,h.zlCount*sizeof(ZLEvent));
  memcpy(modUnits805,fullMods805,sizeof(modUnits805));memcpy(modKinds931,fullKinds931,sizeof(modKinds931));memcpy(delays805,fullDelays805,sizeof(delays805));memcpy(virtualLinks805,fullLinks805,sizeof(virtualLinks805));
  memcpy(modMeta806,fullMeta806,sizeof(modMeta806));
  memcpy(trackVolume818,fullVolume818,16);(pidiStateDirty931=true, trackSolo818=fullSolo818);
  stepLayout25=(h.reservedLegacy&0x80)?h.reservedLegacy&3:0;headTrail25=(h.reservedLegacy&0x80)?(h.reservedLegacy>>2)&3:0;
  for(unsigned t=0;t<16;++t)rollMod827[t]=(h.reserved&(1u<<t))!=0;
  freeInvalidate11();lockStop817();lockCount817=fullLockCount817;memcpy(noteLocks817,fullLocks817,sizeof(noteLocks817));memcpy(delayExtra817,fullDelayExtra817,sizeof(delayExtra817));listMigrate44();
  fullMigrateNeeded810=(h.magic!=0x810554Bu&&h.magic!=0x811554Bu&&h.magic!=0x817554Bu&&h.magic!=0x818554Bu&&h.magic!=0x819554Bu&&h.magic!=0x907554Bu&&h.magic!=0x911554Bu&&h.magic!=0x927554Bu&&h.magic!=0x928554Bu&&h.magic!=0x930554Bu&&h.magic!=0x938554Bu&&h.magic!=0x939554Bu&&h.magic!=0x941554Bu&&h.magic!=0xA10554Bu&&h.magic!=0xA17554Bu&&h.magic!=0xA26554Bu&&h.magic!=0xA27554Bu);
  if(!fullMigrateNeeded810){memcpy(modExtra810,fullExtra810,sizeof(modExtra810));ccGap810=fullGap810;ccBudget810=fullBudget810;}
  else {memset(modExtra810,0,sizeof(modExtra810));ccGap810=2;ccBudget810=500;}
  midiReset810();for(auto&v:modPatternEventAt810)v=-1;
  memset(patternModRefsValid806,0,sizeof(patternModRefsValid806));
  zlCount=h.zlCount;zlNextId=h.nextId;songStartBar=h.start;songEndBar=h.end;songLoopEnabled=h.loop;songView=h.view;trackNavigationPreview=h.preview;
  for(uint8_t t=0;t<16;++t)(pidiStateDirty931=true, trackMuted[t]=(h.muted&(1u<<t))!=0);

  indInstall50();full805Restored=true;songWorkspaceReady=true;mod805Invalidate();mod807ClearHistory();++songRevision;return true;
}
FLASHMEM bool fullRange39(File &f,uint32_t count,int low,int high,bool signedBytes=false){
 uint8_t buffer[512];
 while(count){uint32_t n=min(count,(uint32_t)sizeof(buffer));if(f.read(buffer,n)!=(int)n)return false;
  for(uint32_t i=0;i<n;++i){int value=signedBytes?(int)(int8_t)buffer[i]:(int)buffer[i];if(value<low||value>high)return false;}count-=n;}
 return true;
}
FLASHMEM bool grooveValidate51(File&f,uint16_t version){for(unsigned i=0;i<GROOVE_COUNT*16;++i){int8_t row[5];if(f.read((uint8_t*)row,5)!=5)return false;for(unsigned j=0;j<5;++j){if(version>=28&&(j==2||j==4))continue;if(row[j]<-99||row[j]>99)return false;}if(version>=28&&((uint8_t)row[2]|((uint16_t)(uint8_t)row[4]<<8))>1000)return false;}return true;}
FLASHMEM bool full805ValidateFile08(File f) {
  if(!f||!f.seek(0))return full807Error("OPEN");
  FullProjectHeader h={};if(f.read((uint8_t*)&h,sizeof(h))==sizeof(h)&&h.magic==FULL_PROJECT_MAGIC&&h.version!=FULL_PROJECT_VERSION&&h.version!=33&&h.version!=34){DISPLAY_SERIAL.println("MSG|PROJECT VERSION 33/34/35 REQUIRED");return full807Error("PROJECT_VERSION_33_34_35_REQUIRED");}f.seek(0);bool ok=f.read((uint8_t*)&h,sizeof(h))==sizeof(h)&&h.magic==FULL_PROJECT_MAGIC&&(h.version==FULL_PROJECT_VERSION||h.version==33||h.version==34)&&h.selectedTrack<16&&h.globalViewMode<=NOTE_VIEW_CHROMATIC&&h.headFollow<2;
  for(uint8_t t=0;t<16&&ok;++t)ok=h.activePattern[t]<16&&h.polyphony[t]<=TRACK_PLAY_MULTI&&h.noteView[t]>=-1&&h.noteView[t]<=NOTE_VIEW_CHROMATIC;
  if(!ok){return full807Error("PROJECT_HEADER");}
  for(uint16_t i=0;i<256&&ok;++i){uint32_t size=0;ok=f.read((uint8_t*)&size,4)==4&&size<=100000&&size<=f.size()-f.position();
    if(ok&&size){uint32_t end=f.position()+size,magic=0;uint16_t bars=0;ok=size>=6&&f.read((uint8_t*)&magic,4)==4&&(magic==0x554B4236UL||magic==0x554B4235UL)&&f.read((uint8_t*)&bars,2)==2&&bars>=1&&bars<=MAX_BARS;
      uint32_t expected=(magic==0x554B4231UL?6:7)+sizeof(stepActive[0])+sizeof(stepVel[0])+sizeof(stepMidiNote[0])+sizeof(stepGateTicks[0])+sizeof(stepMicroTick[0])+sizeof(stepLockVel[0])+sizeof(stepLockPitch[0])+sizeof(stepLockGate[0])+sizeof(stepLockTime[0])+ROLL_ROW_BYTES906+sizeof(stepRollLockMask[0]);
      if(magic>=0x554B4233UL)expected+=sizeof(stepExtraNoteCount[0])+sizeof(stepExtraMidiNote[0]);if(magic>=0x554B4234UL)expected+=sizeof(stepLogicalMicro[0])+sizeof(fineNote[0]);
      if(magic>=0x554B4235UL)expected+=sizeof(stepExtra14[0]);if(magic>=0x554B4236UL)expected+=sizeof(defaultMov41[0]);
      ok=ok&&size==expected&&f.seek(end);
      if(!ok){Serial.printf("FULL,PATTERN_DETAIL,INDEX=%u,SIZE=%lu,EXPECTED=%lu,MAGIC=%08lX,BARS=%u\n",i,(unsigned long)size,(unsigned long)expected,(unsigned long)magic,bars);}
    }else if(!ok)Serial.printf("FULL,PATTERN_DETAIL,INDEX=%u,SIZE=%lu,OFFSET=%lu,FILE=%lu\n",i,(unsigned long)size,(unsigned long)f.position(),(unsigned long)f.size());
  }
  if(!ok){return full807Error("PATTERN_RECORD");}
  uint8_t assignments40[256]={},references40[256]={};
  if(ok&&h.version>=2)ok=grooveValidate51(f,h.version)&&fullRange39(f,sizeof(grooveShape16),0,100);
  if(ok&&h.version>=2){ok=f.read(assignments40,256)==256;for(auto a:assignments40)if(a>(h.version>=21?GROOVE_COUNT+2:h.version>=3?GROOVE_USE_LIST:GROOVE_COUNT))ok=false;}
  if(ok&&h.version>=3){for(uint32_t i=0;i<sizeof(grooveLists)/sizeof(GrooveListEntry)&&ok;++i){GrooveListEntry e;ok=f.read((uint8_t*)&e,sizeof(e))==sizeof(e)&&e.sheet<=GROOVE_COUNT&&e.repeats<=99;if(i%14==0)references40[i/14]=e.sheet;}}
  if(!ok){return full807Error("GROOVE_DATA");}
  if(ok&&h.version>=4){FullExtra805 extra;ok=full805ReadExtra(f,extra);}
  else if(ok)ok=f.position()==f.size();
  if(ok&&h.version>=21)for(int i=0;i<256;++i)if(assignments40[i]==GROOVE_COUNT+2&&(!references40[i]||references40[i]>blockStageCount40(i/16)))ok=false;
  return ok;
}
uint32_t autosaveLast807=0,autosaveHash807=0;
FLASHMEM uint32_t projectHash807(){
  uint32_t hash=2166136261u;
  auto add=[&](const void*p,size_t n){auto b=(const uint8_t*)p;while(n--){hash^=*b++;hash*=16777619u;}};
  FullProjectHeader header;fillFullProjectHeader(header);header.selectedTrack=0;
  add(&header,sizeof(header));
  #define HASH807(live) add(live,sizeof(live));
  HASH807(stepActive) HASH807(stepVel) HASH807(stepMidiNote) HASH807(stepGateTicks)
  HASH807(stepMicroTick) HASH807(stepLockVel) HASH807(stepLockPitch) HASH807(stepLockGate)
  HASH807(stepLockTime) for(uint8_t t906=0;t906<16;++t906)for(uint8_t p906=0;p906<64;++p906)if(rollPages906[t906][p906]){add(&t906,1);add(&p906,1);add(rollPages906[t906][p906],sizeof(RollPage906));} HASH807(stepRollLockMask) HASH807(stepExtraNoteCount)
  HASH807(defaultMov41) HASH807(stepExtraMidiNote) HASH807(stepExtra14) HASH807(stepLogicalMicro) HASH807(fineNote)
  #undef HASH807
  add(trackBars,sizeof(trackBars));add(patternBank,sizeof(patternBank));
  for(uint8_t t=0;t<16;++t)for(uint8_t p=0;p<16;++p)if(patternDirty805[t][p]&&p!=activePattern[t]&&patterns906[t][p]){add(&t,1);add(&p,1);add(patterns906[t][p]->data,patterns906[t][p]->length);}
  add(patternOrder51,sizeof(patternOrder51));add(grooveOffset,sizeof(grooveOffset));add(grooveShape16,sizeof(grooveShape16));add(patternGroove,sizeof(patternGroove));add(grooveLists,sizeof(grooveLists));
  add(kbTracksR10,sizeof(kbTracksR10));add(modKinds931,sizeof(modKinds931));add(modUnits805,sizeof(modUnits805));add(modMeta806,sizeof(modMeta806));add(delays805,sizeof(delays805));add(delayExtra817,sizeof(delayExtra817));add(&lockCount817,2);add(noteLocks817,lockCount817*sizeof(NoteLock817));add(virtualLinks805,sizeof(virtualLinks805));for(auto*n=delayNodes907;n;n=n->next){add(&n->key,2);add(&n->value,sizeof(n->value));}
  add(songClips,sizeof(songClips));add(zlEvents,zlCount*sizeof(ZLEvent));add(&songStartBar,sizeof(songStartBar));add(&songEndBar,sizeof(songEndBar));
  add(&songLoopEnabled,sizeof(songLoopEnabled));add(trackMuted,sizeof(trackMuted));add(trackVolume818,16);add(&trackSolo818,2);add(mixerMap938,sizeof(mixerMap938));add(mixerOff938,sizeof(mixerOff938));
  add(modExtra810,sizeof(modExtra810));add(&ccGap810,2);add(&ccBudget810,2);
  uint32_t patches=patch808Hash();add(&patches,4);return hash;
}
FLASHMEM void autosave807Baseline(){autosavePending805=false;}
FLASHMEM void autosave805Request() { /* Stop is not a save request anymore. */ }
FLASHMEM void full805RecoverFiles() {
  if(!storageWriteAvailable)return;
  for(uint8_t t=0;t<16;++t)for(uint8_t p=0;p<16;++p){char path[40],backup[48];internalPatternPath(t,p,path);snprintf(backup,sizeof(backup),"%s.BAK",path);if(!SD.exists(path)&&SD.exists(backup))SD.sdfs.rename(backup,path);}
  for(uint8_t slot=1;slot<=10;++slot){char path[80],backup[88];snprintf(path,sizeof(path),"/URBANKAOZ/PRJ/autosave/auto%02u.UKP",slot);snprintf(backup,sizeof(backup),"%s.BAK",path);if(!SD.exists(path)&&SD.exists(backup))SD.sdfs.rename(backup,path);}
}
FLASHMEM uint32_t autosave805Latest() {
  uint32_t generation=0;
  for(uint8_t slot=1;slot<=10;++slot){char path[72];snprintf(path,sizeof(path),"/URBANKAOZ/PRJ/autosave/auto%02u.UKP",slot);File f=SD.open(path,FILE_READ);if(!f)continue;
    
  
  FullFooter805 footer={};if(f.size()>=sizeof(footer)&&f.seek(f.size()-sizeof(footer))&&f.read((uint8_t*)&footer,sizeof(footer))==sizeof(footer)&&footer.magic==0x8054546u)generation=max(generation,footer.generation);f.close();}
  return generation;
}
FLASHMEM void autosave805Service() { /* VA01.11: explicit saves only. */ }

FLASHMEM void openNewProject807(){
  loadTypeMenuActive=false;fileBrowserMode=FILE_BROWSER_LOAD;
  newProjectConfirm807=patternLoadConfirmActive=true;
  sendMenuScreen("NEW PROJECT?","Unsaved data lost","Cancel    Create","");
}
FLASHMEM void newProject807(){
 indReset50();
  (pidiStateDirty931=true, playing=internalClockRunning=false);pendingClockTicks=0;allNotesOff();
  songEngineActive=songMode=songRecord=false;recordingArmed=false;autoRecord50=false;
  patternMode=(pidiStateDirty931=true, remixMode=grooveEditMode=false);
  memset(patternLinks911,0,sizeof(patternLinks911));remixLeave911();
  initPattern();trackReset818();resetSessionPatternsEmpty();internalPatternSessionFresh=true;
  resetPatterns906();memset(patternDirty805,0,sizeof(patternDirty805));memset(patternModRefsValid806,0,sizeof(patternModRefsValid806));
  memset(grooveLists,0,sizeof(grooveLists));memset(grooveOffset,0,sizeof(grooveOffset));grooveDefaults16();memset(patternGroove,0,sizeof(patternGroove));
  memset(songClips,0,sizeof(songClips));memset(songCache,0,sizeof(songCache));
  songWorkspaceInit();zlNextId=1;songStartBar=1;songEndBar=4;songTick=0;songLoopEnabled=false;
  stepLayout25=0;headTrail25=0;grooveInitialized26=0;grooveNoticeDuration26=0;
  full805Restored=false;trackClear12();linkReset12();mod805Init();patch808Init();mod807ClearHistory();
  selection805Reset();deletedNoteHistoryCount=deletedNoteHistoryCursor=0;
  newProjectConfirm807=false;deleteButtonActive=false;
  bpm=settingsValue[6][2];setTransportPositionToMeasureStart();updateInternalClockPeriod();
  showGameScreen();sendSelectionIndicatorsToPicran();DISPLAY_SERIAL.println("CUSTOM,OFF");
  autosavePending805=false;autosaveLast807=millis(); // Keep previous hash: a new empty project is a change.
  ++songRevision;songSnapshot();DISPLAY_SERIAL.println("MSG|NEW PROJECT");
}


FLASHMEM bool full805Validate(const char*path){File f=SD.open(path,FILE_READ);return full805ValidateFile08(f);}

