#pragma once
struct PatternClip15 {AutoClip46*automation=nullptr;PackedPattern906*image=nullptr;NoteLock817*locks=nullptr;uint16_t count=0;uint8_t groove=0;GrooveListEntry list[GROOVE_LIST_SIZE]{};uint8_t note=36,vel=100;uint16_t gate=96;int16_t micro=0;TrackRollSettings roll{};RollExtra1000 rollExtra{};};
static PatternClip15 clips15[CLIPBOARD_COUNT][PATTERN_CLIP_MAX];
FLASHMEM void clearClip15(PatternClip15&c){autoClipFree46(c.automation);if(c.image)extmem_free(c.image);if(c.locks)extmem_free(c.locks);c={};}
FLASHMEM void clearClipSlot15(uint8_t slot){if(slot<CLIPBOARD_COUNT)for(auto&c:clips15[slot])clearClip15(c);}
FLASHMEM bool hydrateClip15(uint8_t slot,uint8_t index){
 auto&c=clips15[slot][index];if(c.image)return true;
 char path[40];patternClipboardPath(slot,index,path);File f=SD.open(path,FILE_READ);auto&im=patternWork906;
 if(f){uint32_t magic=0;uint16_t bars=0;uint8_t notes=0;bool ok=f.read((uint8_t*)&magic,4)==4&&f.read((uint8_t*)&bars,2)==2&&f.read(&notes,1)==1&&magic==0x554B4236&&readPatternPayload906(f,magic,bars,im);f.close();if(!ok)return false;}
 else {if(patternClipboard[slot].sourceHasNotes[index])return false;memset(&im,0,sizeof(im));im.bars=1;}
 snprintf(path,sizeof(path),"/URBANKAOZ/PAT/.C%u_%02u.GRL",slot+1,index);f=SD.open(path,FILE_READ);
 bool ok=f&&f.read(&c.groove,1)==1&&c.groove<=GROOVE_USE_LIST&&f.read((uint8_t*)c.list,sizeof(c.list))==sizeof(c.list);if(f)f.close();if(!ok)return false;
 for(auto&e:c.list)if(e.sheet>GROOVE_COUNT||e.repeats>99)return false;
 c.image=packPattern906(im);return c.image;
}
bool recoveryWritePattern08(File&,const PatternImage805&);
FLASHMEM bool writeClipEntry15(File&out,uint8_t slot,uint8_t index){
 if(!hydrateClip15(slot,index)||!unpackPattern906(clips15[slot][index].image,patternWork906))return false;
 auto&m=patternClipboard[slot];auto&c=clips15[slot][index];uint8_t meta[3]={m.sourceTrack[index],m.sourcePattern[index],(uint8_t)m.sourceHasNotes[index]};
 return out.write(meta,3)==3&&recoveryWritePattern08(out,patternWork906)&&out.write(&c.groove,1)==1&&out.write((uint8_t*)c.list,sizeof(c.list))==sizeof(c.list);
}
FLASHMEM bool capturePattern15(uint8_t slot){
 if(slot>=CLIPBOARD_COUNT)return false;if(!selectedPatternCount()){sendClipboardMessage(slot,CLIP_TYPE_NONE,"SELECT PATTERN");return false;}auto&m=patternClipboard[slot];m={};for(auto&c:clips15[slot])clearClip15(c);
 patternModRefsValid806[slot]=false;
 for(uint8_t t=0;t<16;t++)for(uint8_t p=0;p<16;p++)if(patternSelected[t][p]){
  if(m.count>=PATTERN_CLIP_MAX){DISPLAY_SERIAL.println("MSG|MAX 16 PATTERNS");return false;}
  auto&image=patternWork906;bool ok=true;
  if(p==activePattern[t])pattern805CaptureLive(t,image);
  else if(patternDirty805[t][p])ok=unpackPattern906(patterns906[t][p],image);
  else if(patternBank[t][p].initialized)ok=pattern805Read(t,p,image);
  else {memset(&image,0,sizeof(image));image.bars=1;}
  if(!ok)return false;auto&c=clips15[slot][m.count];c.image=packPattern906(image);if(!c.image||!autoClipCapture46(c.automation,t,p,0,image.bars*1536UL))return false;
  for(uint16_t k=0;k<lockCount817;k++)if(noteLocks817[k].track==t&&noteLocks817[k].pattern==p)++c.count;
  if(c.count){c.locks=(NoteLock817*)extmem_malloc(c.count*sizeof(NoteLock817));if(!c.locks)return false;uint16_t n=0;for(uint16_t k=0;k<lockCount817;k++)if(noteLocks817[k].track==t&&noteLocks817[k].pattern==p)c.locks[n++]=noteLocks817[k];}
  c.note=defaultMidiNote[t];c.vel=defaultVel[t];c.gate=defaultGateTicks[t];c.micro=defaultMicroTick[t];c.roll=trackRoll[t];c.rollExtra=rollExtra1000(t);
  c.groove=patternGroove[t][p];memcpy(c.list,grooveLists[t][p],sizeof(c.list));if(c.groove==GROOVE_COUNT+2){c.groove=GROOVE_USE_LIST;memcpy(c.list,listRead40(t,p),sizeof(c.list));}
  if(!patch808CaptureClipboard(slot,m.count,t,p))return false;
  m.sourceTrack[m.count]=t;m.sourcePattern[m.count]=p;m.sourceHasNotes[m.count]=patternHasNotes(t,p);++m.count;
 }
 m.valid=m.count>0;patternModRefsValid806[slot]=m.valid;if(m.valid){releaseRemixClipboard915(slot);clipboardSlots[slot].valid=false;sendClipboardMessage(slot,CLIP_TYPE_SEQUENCE,"PATTERNS COPIED");}return m.valid;
}
FLASHMEM bool installPattern15(uint8_t slot,uint8_t index,uint8_t t,uint8_t p){
 if(slot>=CLIPBOARD_COUNT||index>=patternClipboard[slot].count||t>=16||p>=16)return false;auto&c=clips15[slot][index];
 if(!hydrateClip15(slot,index)||!autoClipRoom46(c.automation))return false;
 uint16_t old=0;for(uint16_t k=0;k<lockCount817;k++)if(noteLocks817[k].track==t&&noteLocks817[k].pattern==p)++old;
 if(lockCount817-old+c.count>LOCK_CAPACITY817||!unpackPattern906(c.image,patternWork906))return false;
 auto*copy=packPattern906(patternWork906);if(!copy)return false;
 if(patternModRefsValid806[slot]&&!patch808PasteClipboard(slot,index,t,p)){extmem_free(copy);return false;}
 if(p==activePattern[t]&&!pattern805ApplyLive(t,patternWork906)){extmem_free(copy);return false;}
 installPattern906(t,p,copy);patternDirty805[t][p]=true;auto&dest=patternBank[t][p];dest.initialized=true;dest.hasNotes=patternClipboard[slot].sourceHasNotes[index];dest.bars=patternWork906.bars;
 patternGroove[t][p]=c.groove;memcpy(grooveLists[t][p],c.list,sizeof(c.list));lockClearPattern817(t,p);
 for(uint16_t k=0;k<c.count;k++){auto l=c.locks[k];l.track=t;l.pattern=p;if((l.kind==LOCK_CC817||l.kind==LOCK_NRPN7817||l.kind==LOCK_NRPN14817)||l.kind==LOCK_ROLL1000||l.kind==LOCK_BLOCK21)l.target=t;else if(l.kind==LOCK_DELAY817)l.target=(l.target/256)*256+t*16+p;noteLocks817[lockCount817++]=l;}
 if(!patternModRefsValid806[slot])mod806DetachPattern(t,p);
 autoClipInstall46(c.automation,t,p,0,true);freeInvalidate11();mod805Invalidate();return true;
}
