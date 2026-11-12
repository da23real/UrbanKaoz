#pragma once
bool writeClipEntry15(File&,uint8_t,uint8_t);
void clearClipSlot15(uint8_t);
// Separate pattern manifests must not be mistaken for empty note clipboards.
// 0 empty, 1 one note, 2 sequence/selection, 3 pattern collection.
FLASHMEM uint8_t clipboardSaveKind(uint8_t slot) {
  if(slot>=CLIPBOARD_COUNT) return 0;
  if(patternClipboard[slot].valid && patternClipboard[slot].count) return 3;
  const ClipboardSlot &c=clipboardSlots[slot]; if(!c.valid||c.type==CLIP_TYPE_REMIX) return 0;
  if(c.type!=CLIP_TYPE_NOTES && c.type!=CLIP_TYPE_SELECTION) return 2;
  if(clipboardFineCount[slot]) return clipboardFineCount[slot]==1?1:2;
  uint16_t count=0;
  for(uint16_t s=0;s<STEP_COUNT;++s) if(c.active[s]) ++count;
  return count==1?1:2;
}
static const uint32_t CLIP_PATTERN_MAGIC=0x554B4331UL;
FLASHMEM bool transferClipboardBytes(File &in,File &out,uint32_t count) {
  uint8_t data[128];
  while(count) {
    size_t n=min((uint32_t)sizeof(data),count);
    if(in.read(data,n)!=n || out.write(data,n)!=n) return false;
    count-=n;
  }
  return true;
}
FLASHMEM bool isPatternClipboardBundle(const char *path) {
  File f=SD.open(path,FILE_READ);uint32_t magic=0;
  bool ok=f && f.read((uint8_t*)&magic,4)==4 && magic==CLIP_PATTERN_MAGIC;
  if(f) f.close();return ok;
}
FLASHMEM bool savePatternClipboardBundle(uint8_t slot,char *path,size_t pathSize) {
  if(clipKinds18[slot]!=3 || !makeUniqueSavePath("PAT","CLIP","PAT",path,pathSize)) return false;
  const PatternClipboardManifest &m=patternClipboard[slot];
  File out=SD.open(path,FILE_WRITE);if(!out)return false;
  bool ok=out.write((const uint8_t*)&CLIP_PATTERN_MAGIC,4)==4 && out.write(&m.count,1)==1;
  for(uint8_t i=0;i<m.count&&ok;++i) {
    ok=writeClipEntry15(out,slot,i);
  }
  out.flush(); if(out.getWriteError())ok=false;out.close();
  if(!ok)SD.remove(path);return ok;
}
// Validate the complete bundle before creating staging files or touching data.
FLASHMEM bool validatePatternClipboardBundle(File &f,PatternClipboardManifest &m) {
  uint32_t magic=0; memset(&m,0,sizeof(m));
  if(!f.seek(0)||f.read((uint8_t*)&magic,4)!=4||magic!=CLIP_PATTERN_MAGIC||
     f.read(&m.count,1)!=1||m.count<1||m.count>PATTERN_CLIP_MAX)return false;
  for(uint8_t i=0;i<m.count;++i) {
    uint8_t meta[3];uint32_t size=0;
    if(f.read(meta,3)!=3||meta[0]>=TRACK_COUNT||meta[1]>=PATTERN_COUNT||meta[2]>1||
       f.read((uint8_t*)&size,4)!=4||size>65536||size>(uint32_t)(f.size()-f.position()))return false;
    for(uint8_t j=0;j<i;++j)if(m.sourceTrack[j]==meta[0]&&m.sourcePattern[j]==meta[1])return false;
    m.sourceTrack[i]=meta[0];m.sourcePattern[i]=meta[1];m.sourceHasNotes[i]=meta[2];
    if(size) {
      uint32_t begin=f.position(),bm=0;uint16_t bars=0;
      if(size<7||f.read((uint8_t*)&bm,4)!=4||f.read((uint8_t*)&bars,2)!=2||
         bm!=0x554B4236UL||bars<1||bars>MAX_BARS)return false;
      uint32_t expected=6+(bm!=0x554B4231UL)+sizeof(stepActive[0])+sizeof(stepVel[0])+
        sizeof(stepMidiNote[0])+sizeof(stepGateTicks[0])+sizeof(stepMicroTick[0])+
        sizeof(stepLockVel[0])+sizeof(stepLockPitch[0])+sizeof(stepLockGate[0])+
        sizeof(stepLockTime[0])+ROLL_ROW_BYTES906+sizeof(stepRollLockMask[0]);
      if(bm>=0x554B4233UL)expected+=sizeof(stepExtraNoteCount[0])+sizeof(stepExtraMidiNote[0]);
      if(bm>=0x554B4234UL)expected+=sizeof(stepLogicalMicro[0])+sizeof(fineNote[0]);
      if(bm>=0x554B4236UL)expected+=sizeof(stepExtra14[0]);
      if(size!=expected||!f.seek(begin+size))return false;
    }else if(meta[2])return false;
    uint8_t assignment=0;GrooveListEntry list[GROOVE_LIST_SIZE];
    if(f.read(&assignment,1)!=1||assignment>GROOVE_USE_LIST||
       f.read((uint8_t*)list,sizeof(list))!=sizeof(list))return false;
    for(const auto &e:list)if(e.sheet>GROOVE_COUNT||e.repeats>99)return false;
  }
  m.valid=f.position()==f.size();return m.valid;
}
FLASHMEM void patternImportPath(uint8_t i,bool groove,char *path) {
  snprintf(path,48,"/URBANKAOZ/PAT/.IMPORT%02u.%s",i,groove?"GRL":"BIN");
}
// destinationSlot >=0 restores a Clipboard, -1 restores original track/pattern
// positions. Does not call pastePatternClipboard (the separate deferred bug).
FLASHMEM bool loadPatternClipboardBundle(const char *path,int8_t destinationSlot) {
  if(destinationSlot>=CLIPBOARD_COUNT||playing||!storageWriteAvailable)return false;
  if(destinationSlot>=0)patternModRefsValid806[destinationSlot]=false;
  File in=SD.open(path,FILE_READ);PatternClipboardManifest m;
  if(!in||!validatePatternClipboardBundle(in,m)){if(in)in.close();return false;}
  bool ok=in.seek(5);uint32_t sizes[PATTERN_CLIP_MAX]={};
  for(uint8_t i=0;i<m.count&&ok;++i) {
    uint8_t meta[3];ok=in.read(meta,3)==3&&in.read((uint8_t*)&sizes[i],4)==4;
    for(uint8_t g=0;g<2&&ok;++g) {
      char temp[48];patternImportPath(i,g,temp);
      // Fixed staging paths are internal, never user-named files.
      SD.remove(temp);File out=SD.open(temp,FILE_WRITE);
      ok=out&&transferClipboardBytes(in,out,g?1+sizeof(grooveLists[0][0]):sizes[i]);
      if(out){out.flush();if(out.getWriteError())ok=false;out.close();}
    }
  }
  in.close();
  if(ok) {
    if(destinationSlot>=0){clearClipSlot15(destinationSlot);releaseRemixClipboard915(destinationSlot);patternClipboard[destinationSlot].valid=false;clipboardSlots[destinationSlot].valid=false;}
    for(uint8_t i=0;i<m.count&&ok;++i) {
      char temp[48],dest[48];patternImportPath(i,false,temp);
      if(destinationSlot>=0)patternClipboardPath(destinationSlot,i,dest);
      else internalPatternPath(m.sourceTrack[i],m.sourcePattern[i],dest);
      if(sizes[i])ok=copySdBinary(temp,dest);else SD.remove(dest);
      patternImportPath(i,true,temp);
      if(destinationSlot>=0){snprintf(dest,sizeof(dest),"/URBANKAOZ/PAT/.C%u_%02u.GRL",destinationSlot+1,i);if(ok)ok=copySdBinary(temp,dest);}
      else if(ok) {
        uint8_t t=m.sourceTrack[i],p=m.sourcePattern[i];File g=SD.open(temp,FILE_READ);
        ok=g&&g.read(&patternGroove[t][p],1)==1&&g.read((uint8_t*)grooveLists[t][p],sizeof(grooveLists[t][p]))==sizeof(grooveLists[t][p]);
        if(g)g.close();patternBank[t][p].initialized=sizes[i]>0;
        patternBank[t][p].hasNotes=m.sourceHasNotes[i];patternBank[t][p].bars=1;
        if(sizes[i]){File f=SD.open(dest,FILE_READ);if(f){f.seek(4);f.read((uint8_t*)&patternBank[t][p].bars,2);f.close();}}
        if(ok&&activePattern[t]==p){if(sizes[i])ok=loadTrackPattern(t,p);else clearTrackPatternData(t);}
      }
    }
  }
  for(uint8_t i=0;i<m.count;++i)for(uint8_t g=0;g<2;++g){char temp[48];patternImportPath(i,g,temp);SD.remove(temp);}
  if(ok&&destinationSlot>=0){patternClipboard[destinationSlot]=m;clipboardFineCount[destinationSlot]=0;}
  if(ok&&destinationSlot<0){invalidateStepTriggers();neoDisplayDirty=displayValueDirty=displayBpmDirty=true;}
  return ok;
}
