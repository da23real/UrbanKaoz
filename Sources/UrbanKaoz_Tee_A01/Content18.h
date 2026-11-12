#pragma once
// Versioned standalone note/sequence/pattern files. No tempo is serialized.
// Each entry owns a complete image and its attached OR independent PLX events.
struct ContentEntry18 {
 AutoClip46*automation=nullptr;
 PackedPattern906*image=nullptr;NoteLock817*locks=nullptr;uint16_t count=0,length=16;
 uint8_t track=0,pattern=0,groove=0,note=36,vel=100;uint16_t gate=96;int16_t micro=0;
 TrackRollSettings roll{};RollExtra1000 rollExtra{};GrooveListEntry list[GROOVE_LIST_SIZE]{};
};
struct Content18 {uint8_t kind=0,count=0,flags=0;ContentEntry18 entries[16];};
static ContentEntry18 noteClips18[4];
static ContentEntry18 previewContent18;
static PatternImage805*previewImages18[5]={};
FLASHMEM uint8_t contentClipKind18(uint8_t slot){if(slot>=4||!noteClips18[slot].image||!clipboardSlots[slot].valid)return 0;return clipboardSlots[slot].type==CLIP_TYPE_NOTES?1:clipboardSlots[slot].type==CLIP_TYPE_EFFECT?6:2;}
FLASHMEM void contentFreeEntry18(ContentEntry18&e){autoClipFree46(e.automation);if(e.image)extmem_free(e.image);if(e.locks)extmem_free(e.locks);e={};}
FLASHMEM void contentFree18(Content18&c){for(auto&e:c.entries)contentFreeEntry18(e);c.count=0;}
FLASHMEM void contentClearClip18(uint8_t slot){if(slot<4){contentFreeEntry18(noteClips18[slot]);if(previewImages18[slot]){extmem_free(previewImages18[slot]);previewImages18[slot]=nullptr;}}}
FLASHMEM bool contentImageValid18(const PatternImage805&i){
 if(i.bars<1||i.bars>64||i.mov[0]<-98303||i.mov[0]>98303)return false;
 for(uint16_t s=0;s<1024;++s){if(i.extraCount[s]>3||i.note[s]>127||i.vel[s]>127)return false;if(i.active[s]&&!i.gate[s])return false;for(uint8_t v=0;v<i.extraCount[s];++v)if(i.extraNote[s][v]>127||i.extra14[s][v].vel>127||!i.extra14[s][v].gate)return false;}
 for(auto&e:i.fine)if(e.active&&(e.step>=1024||e.note>127||e.vel>127||!e.gate))return false;
 return true;
}
FLASHMEM bool contentWrite18(File&f,const Content18&c){
 uint32_t hash=0xffffffffu;bool ok=true;auto put=[&](const void*p,size_t n){if(ok){ok=f.write((const uint8_t*)p,n)==n;hash=crc905(hash,(const uint8_t*)p,n);}};
 uint32_t magic=0x5200504c;put(&magic,4);put(&c.kind,1);put(&c.count,1);put(&c.flags,1);
 for(uint8_t j=0;j<c.count&&ok;++j){auto&e=c.entries[j];if(!e.image){ok=false;break;}
  put(&e.track,1);put(&e.pattern,1);put(&e.length,2);put(&e.groove,1);put(&e.note,1);put(&e.vel,1);put(&e.gate,2);put(&e.micro,2);put(&e.roll,sizeof(e.roll));put(&e.rollExtra,sizeof(e.rollExtra));put(e.list,sizeof(e.list));
  put(&e.image->length,4);put(e.image->data,e.image->length);put(&e.count,2);put(e.locks,e.count*sizeof(NoteLock817));uint16_t sizes[3]={e.automation?e.automation->laneCount:(uint16_t)0,e.automation?e.automation->count:(uint16_t)0,e.automation?e.automation->muteCount:(uint16_t)0};put(sizes,sizeof(sizes));if(e.automation){put(e.automation->lanes,sizes[0]*sizeof(AutoLane46));put(e.automation->points,sizes[1]*sizeof(AutoPoint46));put(e.automation->mutes,sizes[2]*sizeof(AutoMute46));}
  uint8_t flags=e.automation&&e.automation->indCaptured?e.automation->indFlags:0;uint16_t ni=e.automation?e.automation->indCount:0;put(&flags,1);put(&ni,2);if(ni)put(e.automation->ind,ni*sizeof(IndEntry50));
 }hash=~hash;return ok&&f.write((uint8_t*)&hash,4)==4;
}
FLASHMEM bool contentRead18(File&f,Content18&c){
 uint32_t hash=0xffffffffu;bool ok=true;auto get=[&](void*p,size_t n){if(ok){ok=f.read((uint8_t*)p,n)==n;if(ok)hash=crc905(hash,(uint8_t*)p,n);}};
 uint32_t magic=0;get(&magic,4);get(&c.kind,1);get(&c.count,1);get(&c.flags,1);if(!ok||(magic!=0x4100504c&&magic!=0x4400504c&&magic!=0x4600504c&&magic!=0x5000504c&&magic!=0x5200504c)||c.kind<1||c.kind>4||!c.count||c.count>16||(c.kind!=3&&c.count!=1)||c.flags>1||(c.flags&&c.kind!=3))return false;
 uint32_t totalLocks=0;
 for(uint8_t j=0;j<c.count&&ok;++j){auto&e=c.entries[j];get(&e.track,1);get(&e.pattern,1);get(&e.length,2);get(&e.groove,1);get(&e.note,1);get(&e.vel,1);get(&e.gate,2);get(&e.micro,2);get(&e.roll,sizeof(e.roll));e.rollExtra={};get(&e.rollExtra,magic==0x4100504c?14:sizeof(e.rollExtra));get(e.list,sizeof(e.list));
  if(!ok||e.track>=16||e.pattern>=16||!e.length||e.length>1024||e.groove>GROOVE_USE_LIST||e.note>127||e.vel>127||!e.gate)return false;
  for(auto&v:e.list)if(v.sheet>GROOVE_COUNT||v.repeats>99)return false;
  for(uint8_t k=0;k<j;++k)if(c.entries[k].track==e.track&&c.entries[k].pattern==e.pattern)return false;
  uint32_t n=0;get(&n,4);if(!ok||!n||n>sizeof(PatternImage805)*2)return false;e.image=(PackedPattern906*)extmem_malloc(n+4);if(!e.image)return false;e.image->length=n;get(e.image->data,n);
  if(!ok||!unpackPattern906(e.image,patternWork906)||!contentImageValid18(patternWork906))return false;
  get(&e.count,2);totalLocks+=e.count;if(!ok||totalLocks>LOCK_CAPACITY817)return false;
  if(e.count){e.locks=(NoteLock817*)extmem_malloc(e.count*sizeof(NoteLock817));if(!e.locks)return false;get(e.locks,e.count*sizeof(NoteLock817));}
  for(uint16_t k=0;k<e.count;++k){auto&l=e.locks[k];if(!lockValid817(l)||l.track!=e.track||l.pattern!=e.pattern||l.tick< -96||l.tick>=(int32_t)e.length*96)return false;}
  if(magic==0x4600504c||magic==0x5000504c||magic==0x5200504c){uint16_t sizes[3];get(sizes,sizeof(sizes));if(!ok)return false;if(sizes[0]||sizes[1]||sizes[2]){e.automation=autoClipAlloc46(sizes[0],sizes[1],sizes[2]);if(!e.automation)return false;get(e.automation->lanes,sizes[0]*sizeof(AutoLane46));get(e.automation->points,sizes[1]*sizeof(AutoPoint46));get(e.automation->mutes,sizes[2]*sizeof(AutoMute46));if(!ok||!autoClipValid46(e.automation))return false;}}
  if(magic==0x5000504c||magic==0x5200504c){uint8_t flags;uint16_t ni;get(&flags,1);get(&ni,2);if(!ok||ni>IndStore50::CAPACITY)return false;if(flags||ni){if(!e.automation)e.automation=autoClipAlloc46(0,0,0);if(!e.automation)return false;auto*c=e.automation;c->indFlags=flags;c->indCaptured=true;c->indCount=ni;if(ni){c->ind=(IndEntry50*)extmem_malloc(ni*sizeof(IndEntry50));if(!c->ind)return false;get(c->ind,ni*sizeof(IndEntry50));}if(!autoClipValid46(c))return false;}}
 }uint32_t expected=0;return ok&&f.read((uint8_t*)&expected,4)==4&&expected==~hash&&!f.available();
}
FLASHMEM void contentDefaults18(ContentEntry18&e,uint8_t t,uint8_t p){e.track=t;e.pattern=p;e.note=defaultMidiNote[t];e.vel=defaultVel[t];e.gate=defaultGateTicks[t];e.micro=defaultMicroTick[t];e.roll=trackRoll[t];e.rollExtra=rollExtra1000(t);e.groove=patternGroove[t][p];memcpy(e.list,grooveLists[t][p],sizeof(e.list));if(e.groove==GROOVE_COUNT+2||listId40(t,p)||ind50.active(t*16+p,IND_OOV50)){e.groove=GROOVE_USE_LIST;memcpy(e.list,listRead40(t,p),sizeof(e.list));}}
FLASHMEM void contentResolve18(PatternImage805&i,const ContentEntry18&e){
 for(uint16_t s=0;s<1024;++s)if(i.active[s]){if(!i.lockPitch[s])i.note[s]=e.note;if(!i.lockVel[s])i.vel[s]=e.vel;if(!i.lockGate[s])i.gate[s]=e.gate;if(!i.lockTime[s])i.micro[s]=e.micro;i.lockPitch[s]=i.lockVel[s]=i.lockGate[s]=i.lockTime[s]=true;for(uint8_t field=0;field<6;++field)if(!(i.rollMask[s]&(1u<<field)))setRollFieldValue(i.roll[s],field,rollFieldValue(e.roll,field));i.rollMask[s]=63;}
 for(auto&n:i.fine)if(n.active){if(!(n.pxMask&PX_PITCH))n.note=e.note;if(!(n.pxMask&PX_VEL))n.vel=e.vel;if(!(n.pxMask&PX_GATE))n.gate=e.gate;if(!(n.pxMask&PX_TIME))n.micro=e.micro;for(uint8_t field=0;field<6;++field)if(!(n.pxMask&(1u<<(PX_ROLL_SHIFT+field))))setRollFieldValue(n.roll,field,rollFieldValue(e.roll,field));n.pxMask=0x3ff;}
}
FLASHMEM bool contentLocks18(ContentEntry18&e,uint8_t t,uint8_t p,int32_t offset,int32_t end,bool selection,bool one,uint8_t pitch,int32_t tick){
 auto include=[&](const NoteLock817&l){if(l.track!=t||l.pattern!=p)return false;if(one)return l.note==pitch&&l.tick==tick;if(l.tick<offset-48||l.tick>=end)return false;if(!selection)return true;if(l.note>=128){uint16_t s=max(0L,(long)l.tick)/96;return s<1024&&stepSelected[t][s];}int center=max(0L,(long)l.tick)/96;for(int s=max(0,center-1);s<=min(1023,center+1);++s)if(stepSelected[t][s])for(uint8_t v=0;v<noteCountAtStep(t,s);++v)if(noteAtVoice(t,s,v)==l.note&&lockTick817(t,s,v)==l.tick)return true;return false;};
 for(uint16_t k=0;k<lockCount817;++k)if(include(noteLocks817[k]))++e.count;
 if(e.count){e.locks=(NoteLock817*)extmem_malloc(e.count*sizeof(NoteLock817));if(!e.locks)return false;uint16_t n=0;for(uint16_t k=0;k<lockCount817;++k)if(include(noteLocks817[k])){auto l=noteLocks817[k];l.tick-=offset;e.locks[n++]=l;}}
 return true;
}
FLASHMEM bool contentBuild18(Content18&c,uint8_t kind,uint8_t t,bool selection){
 c.kind=kind;c.count=kind==3?16:1;c.flags=kind==3;
 for(uint8_t j=0;j<c.count;++j){uint8_t track=kind==3?j:t;auto&e=c.entries[j];auto&i=patternWork906;contentDefaults18(e,track,activePattern[track]);pattern805CaptureLive(track,i);e.length=trackStepCount(track);
  if(kind==1){if(selectedStep<0)return false;FineNoteEvent note{};if(!captureNoteRecord(track,selectedStep,selectedVoice,note))return false;int32_t tick=lockTick817(track,selectedStep,selectedVoice);uint8_t pitch=noteAtVoice(track,selectedStep,selectedVoice);int32_t offset=(int32_t)note.step*96;
   memset(&i,0,sizeof(i));i.bars=1;note.step=0;i.fine[0]=note;e.length=1;e.note=note.note;e.vel=note.vel;e.gate=note.gate;e.micro=note.micro;
   if(!contentLocks18(e,track,e.pattern,offset,offset+96,false,true,pitch,tick))return false;
  }else if(selection){
   uint16_t first=1024,last=0;for(uint16_t s=0;s<1024;++s)if(stepSelected[track][s]){first=min(first,s);last=max(last,s);}if(first==1024)return false;
   // Shift every per-step field together; extra voices keep independent values.
   for(uint16_t s=0;s<1024;++s){uint16_t src=s+first;bool keep=src<=last&&stepSelected[track][src];
    i.active[s]=keep&&i.active[src];i.extraCount[s]=keep?i.extraCount[src]:0;
    if(keep){i.vel[s]=i.vel[src];i.note[s]=i.note[src];i.gate[s]=i.gate[src];i.micro[s]=i.micro[src];i.logical[s]=i.logical[src];i.lockVel[s]=i.lockVel[src];i.lockPitch[s]=i.lockPitch[src];i.lockGate[s]=i.lockGate[src];i.lockTime[s]=i.lockTime[src];i.roll[s]=i.roll[src];i.rollMask[s]=i.rollMask[src];memcpy(i.extraNote[s],i.extraNote[src],sizeof(i.extraNote[s]));memcpy(i.extra14[s],i.extra14[src],sizeof(i.extra14[s]));}else i.rollMask[s]=0;
   }
   for(auto&n:i.fine)if(n.active){if(n.step<first||n.step>last||!stepSelected[track][n.step])n.active=false;else n.step-=first;}
   e.length=last-first+1;i.bars=(e.length+15)/16;if(!contentLocks18(e,track,e.pattern,first*96,(last+1)*96,true,false,0,0))return false;
  }else if(!contentLocks18(e,track,e.pattern,0,e.length*96,false,false,0,0))return false;
  if(kind!=1){uint32_t first=0;if(selection){while(first<1024&&!stepSelected[track][first])++first;}if(!autoClipCapture46(e.automation,track,e.pattern,first*96,(first+e.length)*96))return false;}
  if(kind==3)contentResolve18(i,e);e.image=packPattern906(i);if(!e.image)return false;
 }return true;
}
FLASHMEM bool contentSaveFile18(Content18&c,char*path,size_t size){
 const char*ext=c.kind==1?"NPX":c.kind==3?"PAT":"SEQ";
 if(!makeUniqueSavePath(ext,ext,ext,path,size))return false;File f=SD.open(path,FILE_WRITE_BEGIN);bool ok=f&&f.truncate(0)&&contentWrite18(f,c);f.close();
 auto*verify=(Content18*)extmem_malloc(sizeof(Content18));if(!verify)return false;new(verify)Content18{};if(ok){f=SD.open(path,FILE_READ);ok=f&&contentRead18(f,*verify);f.close();}contentFree18(*verify);extmem_free(verify);return ok;
}
FLASHMEM bool contentSave18(uint8_t kind,uint8_t t,bool selection,char*path,size_t size){auto*c=(Content18*)extmem_malloc(sizeof(Content18));if(!c)return false;new(c)Content18{};bool ok=contentBuild18(*c,kind,t,selection)&&contentSaveFile18(*c,path,size);contentFree18(*c);extmem_free(c);return ok;}

FLASHMEM void contentRemapLock18(NoteLock817&l,uint8_t t,uint8_t p,int32_t offset){l.track=t;l.pattern=p;l.tick+=offset;if(lockMidiKind817(l.kind)||l.kind==LOCK_ROLL1000||l.kind==LOCK_BLOCK21)l.target=t;else if(l.kind==LOCK_DELAY817)l.target=(l.target/256)*256+t*16+p;}
FLASHMEM Content18* contentOpen18(const char*path){auto*c=(Content18*)extmem_malloc(sizeof(Content18));if(!c)return nullptr;new(c)Content18{};File f=SD.open(path,FILE_READ);bool ok=f&&contentRead18(f,*c);f.close();if(!ok){contentFree18(*c);extmem_free(c);return nullptr;}return c;}
FLASHMEM bool contentLoadPattern18(const char*path){
 auto*c=contentOpen18(path);if(!c)return false;bool ok=c->kind==3;uint16_t removed=0,added=0;
 if(c->flags)for(uint8_t j=0;j<c->count;++j){auto&e=c->entries[j];e.pattern=activePattern[e.track];for(uint16_t k=0;k<e.count;++k)contentRemapLock18(e.locks[k],e.track,e.pattern,0);}
 for(uint8_t j=0;j<c->count&&ok;++j){auto&e=c->entries[j];added+=e.count;for(uint16_t k=0;k<lockCount817;++k)if(noteLocks817[k].track==e.track&&noteLocks817[k].pattern==e.pattern)++removed;}
 ok=ok&&lockCount817-removed+added<=LOCK_CAPACITY817;unsigned autoL=0,autoP=0,autoM=0,autoFree=0;for(auto&l:autoLanes46)autoFree+=!l.used;for(uint8_t j=0;j<c->count;++j)if(auto*a=c->entries[j].automation){autoL+=a->laneCount;autoP+=a->count;autoM+=a->muteCount;}ok=ok&&autoL<=autoFree&&autoP+autoCount46<=AUTO_POINTS46&&autoM+autoMuteCount46<=AUTO_MUTES46;
 // Reserve required roll pages before any note/PLX replacement.
 for(uint8_t j=0;j<c->count&&ok;++j){auto&e=c->entries[j];if(e.pattern!=activePattern[e.track])continue;ok=unpackPattern906(e.image,patternWork906);for(uint16_t s=0;s<1024&&ok;++s)if(patternWork906.rollMask[s])ok=rollReserve906(e.track,s/16);}
 RollNode1000*rolls[16]={};
 for(uint8_t j=0;j<c->count&&ok;++j){auto&e=c->entries[j];if(e.pattern==activePattern[e.track]){rolls[j]=(RollNode1000*)extmem_malloc(sizeof(RollNode1000));if(!rolls[j])ok=false;}}
 if(ok){stopClipboardPreview(true,true);playing=internalClockRunning=false;allNotesOff();
 for(uint8_t j=0;j<c->count;++j){auto&e=c->entries[j];uint8_t t=e.track,p=e.pattern;unpackPattern906(e.image,patternWork906);if(p==activePattern[t]){pattern805ApplyLive(t,patternWork906);defaultMidiNote[t]=e.note;defaultVel[t]=e.vel;defaultGateTicks[t]=e.gate;defaultMicroTick[t]=e.micro;trackRoll[t]=e.roll;trackLoopStartStep[t]=1;trackLoopEndStep[t]=e.length;auto**at=&rollNodes1000;while(*at&&(*at)->track!=t)at=&(*at)->next;if(*at){auto*old=*at;*at=old->next;extmem_free(old);}*rolls[j]={rollNodes1000,t,e.rollExtra};rollNodes1000=rolls[j];rolls[j]=nullptr;}
 installPattern906(t,p,e.image);e.image=nullptr;patternDirty805[t][p]=true;patternBank[t][p]={true,patternWork906.bars,true};patternGroove[t][p]=e.groove;memcpy(grooveLists[t][p],e.list,sizeof(e.list));lockClearPattern817(t,p);for(uint16_t k=0;k<e.count;++k)noteLocks817[lockCount817++]=e.locks[k];autoClipInstall46(e.automation,t,p,0,true);}}
 for(auto*n:rolls)if(n)extmem_free(n);
 contentFree18(*c);extmem_free(c);if(ok){clearStepSelection();selectedStep=-1;invalidateStepTriggers();freeInvalidate11();neoDisplayDirty=displayValueDirty=true;}return ok;
}
FLASHMEM bool contentLoadClip18(const char*path,uint8_t slot){
 if(slot>=4)return false;auto*c=contentOpen18(path);if(!c)return false;
 releaseRemixClipboard915(slot);contentClearClip18(slot);clearClipSlot15(slot);patternClipboard[slot]={};clipboardFineCount[slot]=0;clipboardSlots[slot].valid=false;
 if(c->kind==3){auto&m=patternClipboard[slot];m.valid=true;m.count=c->count;patternModRefsValid806[slot]=false;for(uint8_t j=0;j<c->count;++j){auto&e=c->entries[j];auto&d=clips15[slot][j];d.automation=e.automation;e.automation=nullptr;d.image=e.image;e.image=nullptr;d.locks=e.locks;e.locks=nullptr;d.count=e.count;d.groove=e.groove;d.note=e.note;d.vel=e.vel;d.gate=e.gate;d.micro=e.micro;d.roll=e.roll;d.rollExtra=e.rollExtra;memcpy(d.list,e.list,sizeof(d.list));m.sourceTrack[j]=e.track;m.sourcePattern[j]=e.pattern;m.sourceHasNotes[j]=true;}}
 else {auto&e=c->entries[0];noteClips18[slot]=e;e.automation=nullptr;e.image=nullptr;e.locks=nullptr;auto&clip=clipboardSlots[slot];resetLoadedClipboard(clip);clip.valid=true;clip.type=c->kind==1?CLIP_TYPE_NOTES:c->kind==4?CLIP_TYPE_EFFECT:CLIP_TYPE_SEQUENCE;clip.sourceTrack=e.track;clip.lengthSteps=e.length;clip.defaultNote=e.note;clip.defaultVel=e.vel;clip.defaultGateTicks=e.gate;clip.defaultMicroTick=e.micro;clipboardSelectedTypes[slot]=clip.type;
  if(c->kind==4&&unpackPattern906(noteClips18[slot].image,patternWork906)){memcpy(clipboardFine[slot],patternWork906.fine,sizeof(patternWork906.fine));for(auto&n:patternWork906.fine)if(n.active)++clipboardFineCount[slot];}
 }
 contentFree18(*c);extmem_free(c);return true;
}
FLASHMEM bool contentPaste18(uint8_t slot){
 if(slot>=4||!noteClips18[slot].image||clipboardSlots[slot].type==CLIP_TYPE_EFFECT)return false;
 auto&e=noteClips18[slot];auto&i=patternWork906;uint8_t t=selectedTrack,p=activePattern[t];
 uint32_t anchor=lastGridTickValid?lastGridTick:(uint32_t)(selectedStep>=0?selectedStep:currentStep)*96;int16_t selected=firstSelectedStepOnTrack(t);if(selected>=0)anchor=selected*96;
 if(!unpackPattern906(e.image,i)){DISPLAY_SERIAL.println("MSG|INVALID CLIP CONTENT");return true;}
 auto locate=[&](const FineNoteEvent&n,uint16_t&s,int16_t&logical){if(anchor%96==0){s=anchor/96+n.step;logical=n.logicalMicro;}else tickToStepAndMicro(t,anchor+n.step*96+n.logicalMicro,s,logical);};
 auto each=[&](auto emit){
  for(uint16_t s=0;s<e.length;++s)if(i.active[s]){FineNoteEvent n{};n.active=true;n.step=s;n.note=i.lockPitch[s]?i.note[s]:e.note;n.vel=i.lockVel[s]?i.vel[s]:e.vel;n.gate=i.lockGate[s]?i.gate[s]:e.gate;n.micro=i.lockTime[s]?i.micro[s]:e.micro;n.logicalMicro=i.logical[s];n.pxMask=0x3ff;n.roll=e.roll;
   for(uint8_t f=0;f<6;++f)if(i.rollMask[s]&(1u<<f))setRollFieldValue(n.roll,f,rollFieldValue(i.roll[s],f));
   emit(n);for(uint8_t v=0;v<i.extraCount[s];++v){n.note=i.extraNote[s][v];n.vel=i.extra14[s][v].vel;n.gate=i.extra14[s][v].gate;n.micro=i.extra14[s][v].micro;emit(n);}}
  for(auto&n:i.fine)if(n.active)emit(n);
 };
 const bool erase=clipboardSlots[slot].type==CLIP_TYPE_SEQUENCE&&clipboardPasteMode[slot]==CLIP_PASTE_ERASE;
 const uint32_t end=anchor+(uint32_t)e.length*96,limit=(uint32_t)trackStepCount(t)*96;
 struct PasteRoom18{uint8_t voices[1024];int8_t logical[1024];};
 auto*room=(PasteRoom18*)extmem_malloc(sizeof(PasteRoom18));if(!room){DISPLAY_SERIAL.println("MSG|CLIP MEMORY");return true;}
 uint16_t fineUsed=0,removed=0;bool ok=end<=limit;
 for(uint16_t s=0;s<1024;++s){bool clear=erase&&s*96>=anchor&&s*96<end;room->voices[s]=!clear&&stepActive[t][s]?1+stepExtraNoteCount[t][s]:0;room->logical[s]=stepLogicalMicro[t][s];}
 for(auto&n:fineNote[t])if(n.active&&!(erase&&n.step*96>=anchor&&n.step*96<end))++fineUsed;
 for(uint16_t k=0;k<lockCount817;++k){auto&l=noteLocks817[k];if(erase&&l.track==t&&l.pattern==p&&l.tick>=(int32_t)anchor&&l.tick<(int32_t)end)++removed;}
 ok=ok&&lockCount817-removed+e.count<=LOCK_CAPACITY817&&autoClipRoom46(e.automation);
 each([&](const FineNoteEvent&n){int32_t tick=(int32_t)anchor+n.step*96+n.logicalMicro;if(tick< -48||tick>=(int32_t)limit){ok=false;return;}uint16_t s;int16_t logical;locate(n,s,logical);if(s>=1024){ok=false;return;}
  if(!room->voices[s]){room->voices[s]=1;room->logical[s]=logical;}
  else if(room->logical[s]==logical&&room->voices[s]<4)++room->voices[s];else ++fineUsed;
  if(n.pxMask>>(PX_ROLL_SHIFT))ok=ok&&rollReserve906(t,s/16);
 });extmem_free(room);ok=ok&&fineUsed<=FINE_EVENTS_PER_TRACK;
 if(!ok){DISPLAY_SERIAL.println("MSG|CLIP EXCEEDS CAPACITY");return true;}
 if(erase){for(uint16_t s=0;s<1024;++s)if(s*96>=anchor&&s*96<end)clearTrackStep(t,s);for(auto&n:fineNote[t])if(n.active&&n.step*96>=anchor&&n.step*96<end)n.active=false;for(uint16_t k=0;k<lockCount817;){auto&l=noteLocks817[k];if(l.track==t&&l.pattern==p&&l.tick>=(int32_t)anchor&&l.tick<(int32_t)end)noteLocks817[k]=noteLocks817[--lockCount817];else ++k;}}
 each([&](const FineNoteEvent&n){uint16_t s;int16_t logical;locate(n,s,logical);
  if(!stepActive[t][s]){stepActive[t][s]=true;stepMidiNote[t][s]=n.note;stepVel[t][s]=n.vel;stepGateTicks[t][s]=n.gate;stepMicroTick[t][s]=n.micro;stepLogicalMicro[t][s]=logical;stepLockPitch[t][s]=stepLockVel[t][s]=stepLockGate[t][s]=stepLockTime[t][s]=true;stepExtraNoteCount[t][s]=0;rollSet906(t,s,n.roll,(n.pxMask>>PX_ROLL_SHIFT)&63);}
  else if(stepLogicalMicro[t][s]==logical&&stepExtraNoteCount[t][s]<3){uint8_t v=stepExtraNoteCount[t][s]++;stepExtraMidiNote[t][s][v]=n.note;stepExtra14[t][s][v]={n.gate,n.micro,n.vel};}
  else for(auto&dst:fineNote[t])if(!dst.active){dst=n;dst.step=s;dst.logicalMicro=logical;dst.active=true;break;}
 });
 for(uint16_t k=0;k<e.count;++k){auto l=e.locks[k];contentRemapLock18(l,t,p,anchor);int at=lockFind817(t,p,l.tick,l.note,l.kind,l.target,l.parameter);if(at>=0)noteLocks817[at]=l;else noteLocks817[lockCount817++]=l;}
 autoClipInstall46(e.automation,t,p,anchor,false);patternDirty805[t][p]=true;freeInvalidate11();invalidateStepTriggers();neoDisplayDirty=displayValueDirty=true;DISPLAY_SERIAL.println("MSG|CONTENT + PLX PASTED");return true;
}
FLASHMEM bool contentEffect18(uint8_t slot){
 if(slot>=4||clipboardSlots[slot].type!=CLIP_TYPE_EFFECT||!noteClips18[slot].image)return false;
 auto&e=noteClips18[slot];if(!unpackPattern906(e.image,patternWork906))return true;const FineNoteEvent*record=nullptr;for(auto&n:patternWork906.fine)if(n.active){record=&n;break;}if(!record)return true;
 NoteLock817 locks[32];uint8_t count=0;for(uint16_t k=0;k<e.count;++k){auto&l=e.locks[k];if(l.note!=record->note||l.tick!=record->step*96+record->logicalMicro)continue;if(count==32){DISPLAY_SERIAL.println("MSG|PLX EVENT CAPACITY");return true;}locks[count++]=l;}
 uint8_t t=selectedTrack;uint16_t targets=0;
 auto each=[&](auto apply){if(selectionHasActiveNotes()){for(uint16_t s=0;s<trackStepCount(t);++s)for(uint8_t v=0;v<noteCountAtStep(t,s);++v)if(selectedNoteObject(t,s,v))apply(s,v);}else if(selectedStep>=0)apply((uint16_t)selectedStep,selectedVoice);};
 bool ok=true;each([&](uint16_t s,uint8_t v){++targets;uint8_t present=0;for(uint16_t k=0;k<lockCount817;++k){auto&l=noteLocks817[k];if(l.track==t&&l.pattern==activePattern[t]&&l.tick==lockTick817(t,s,v)&&l.note==noteAtVoice(t,s,v))++present;}if(present+count>32)ok=false;});
 if(!ok||lockCount817+(uint32_t)targets*count>LOCK_CAPACITY817){DISPLAY_SERIAL.println("MSG|PLX CAPACITY");return true;}
 each([&](uint16_t s,uint8_t v){kbStoreR10(t,activePattern[t],lockTick817(t,s,v),noteAtVoice(t,s,v),locks,count);applyPxRecordToVoice(t,s,v,*record);});
 freeInvalidate11();invalidateStepTriggers();neoDisplayDirty=displayValueDirty=true;DISPLAY_SERIAL.println(targets?"MSG|PLX PASTED":"MSG|SELECT NOTES");return true;
}
// Copy only the already copied note records and the matching PLX. This is
// clipboard content creation, never a scan for the Save screen's type labels.
FLASHMEM void contentCaptureClip18(uint8_t slot,uint8_t t,uint16_t first){
 contentClearClip18(slot);auto&e=noteClips18[slot];auto&i=patternWork906;memset(&i,0,sizeof(i));i.bars=max((uint16_t)1,(uint16_t)((clipboardSlots[slot].lengthSteps+15)/16));contentDefaults18(e,t,activePattern[t]);e.length=clipboardSlots[slot].lengthSteps;
 memcpy(i.fine,clipboardFine[slot],sizeof(i.fine));
 auto include=[&](const NoteLock817&l){if(clipboardCopyScope[slot]==CLIP_COPY_NOTE_ONLY||l.track!=t||l.pattern!=e.pattern)return false;for(uint8_t j=0;j<clipboardFineCount[slot];++j){auto&n=i.fine[j];if(l.note==n.note&&l.tick==(int32_t)(first+n.step)*96+n.logicalMicro)return true;}return false;};
 for(uint16_t k=0;k<lockCount817;++k)if(include(noteLocks817[k]))++e.count;
 if(e.count){e.locks=(NoteLock817*)extmem_malloc(e.count*sizeof(NoteLock817));if(!e.locks){contentClearClip18(slot);DISPLAY_SERIAL.println("MSG|CLIP PLX MEMORY");return;}uint16_t n=0;for(uint16_t k=0;k<lockCount817;++k)if(include(noteLocks817[k])){auto l=noteLocks817[k];l.tick-=first*96;e.locks[n++]=l;}}
 if(clipboardSlots[slot].type==CLIP_TYPE_SEQUENCE&&!autoClipCapture46(e.automation,t,e.pattern,first*96,(first+e.length)*96)){contentClearClip18(slot);DISPLAY_SERIAL.println("MSG|CLIP AUTO MEMORY");return;}e.image=packPattern906(i);if(!e.image){contentClearClip18(slot);DISPLAY_SERIAL.println("MSG|CLIP MEMORY");}
}
FLASHMEM bool contentSaveClip18(uint8_t slot,char*path,size_t size){
 if(slot>=4)return false;
 if(clipKinds18[slot]==4){auto*p=trackClips12[slot];if(!p||!makeUniqueSavePath("TRACK","TRACK","UKT",path,size))return false;File f=SD.open(path,FILE_WRITE_BEGIN);bool ok=f&&f.truncate(0)&&trackWrite12(f,*p);f.close();if(ok){f=SD.open(path,FILE_READ);auto*q=trackRead12(f);ok=q;trackFree12(q);f.close();}return ok;}
 if(clipKinds18[slot]==5){auto*p=clipboardSlots[slot].remix;if(!p||!makeUniqueSavePath("PLink","REMIX","PLK",path,size))return false;PLinkHeader911 h;h.notes=p->notes.size;h.properties=p->properties.size;h.nextId=p->nextId;h.mask=p->mask;h.bars=1;h.linked=false;h.hash=plinkHash911(h,*p);File f=SD.open(path,FILE_WRITE_BEGIN);bool ok=f&&f.truncate(0)&&f.write((uint8_t*)&h,sizeof(h))==sizeof(h)&&f.write((uint8_t*)p->tracks,sizeof(p->tracks))==sizeof(p->tracks)&&f.write((uint8_t*)p->notes.data,p->notes.size*sizeof(rmx911::Note))==p->notes.size*sizeof(rmx911::Note)&&f.write((uint8_t*)p->properties.data,p->properties.size*sizeof(rmx911::Property))==p->properties.size*sizeof(rmx911::Property);f.close();rmx911::Model verify;PLinkHeader911 vh;return ok&&plinkRead911(path,verify,vh);}
 auto*c=(Content18*)extmem_malloc(sizeof(Content18));if(!c)return false;new(c)Content18{};c->kind=clipKinds18[slot]==6?4:clipKinds18[slot];bool ok=c->kind>=1&&c->kind<=4;
 if(ok&&c->kind==3){c->count=patternClipboard[slot].count;ok=patternClipboard[slot].valid&&c->count;for(uint8_t j=0;j<c->count&&ok;++j){ok=hydrateClip15(slot,j);auto&src=clips15[slot][j];auto&e=c->entries[j];e.automation=src.automation;e.locks=src.locks;e.count=src.count;e.track=patternClipboard[slot].sourceTrack[j];e.pattern=patternClipboard[slot].sourcePattern[j];e.groove=src.groove;e.note=src.note;e.vel=src.vel;e.gate=src.gate;e.micro=src.micro;e.roll=src.roll;e.rollExtra=src.rollExtra;memcpy(e.list,src.list,sizeof(e.list));if(ok)ok=unpackPattern906(src.image,patternWork906);if(ok){contentResolve18(patternWork906,e);e.image=packPattern906(patternWork906);ok=e.image;}e.length=patternWork906.bars*16;}}
 else if(ok){c->count=1;c->entries[0]=noteClips18[slot];ok=c->entries[0].image;}
 if(ok)ok=contentSaveFile18(*c,path,size);
 if(c->kind==3)for(auto&e:c->entries)if(e.image)extmem_free(e.image);
 // Lock arrays and individual clipboard images are borrowed.
 extmem_free(c);return ok;
}
FLASHMEM void contentPreviewClear18(){for(auto*&p:previewImages18)if(p){extmem_free(p);p=nullptr;}}
FLASHMEM bool contentPreview18(const char*path){
 auto*c=contentOpen18(path);if(!c)return false;if(c->kind>2){contentFree18(*c);extmem_free(c);return false;}
 stopClipboardPreview(true,true);contentFreeEntry18(previewContent18);previewContent18=c->entries[0];c->entries[0].automation=nullptr;c->entries[0].image=nullptr;c->entries[0].locks=nullptr;contentFree18(*c);extmem_free(c);
 resetLoadedClipboard(loadPreviewBuffer);loadPreviewBuffer.valid=true;loadPreviewBuffer.type=CLIP_TYPE_SEQUENCE;loadPreviewBuffer.lengthSteps=previewContent18.length;loadPreviewFineCount=0;beginClipboardPreview(255,selectedTrack);return true;
}
static uint16_t patternPreviewSteps36[4][16]={};
static int8_t patternPreviewIndex36[4]={-1,-1,-1,-1};
FLASHMEM void startPatternPreview36(uint8_t slot){
 uint32_t steps=0;for(uint8_t j=0;j<patternClipboard[slot].count;++j){if(!hydrateClip15(slot,j)||!unpackPattern906(clips15[slot][j].image,patternWork906)){DISPLAY_SERIAL.println("MSG|PATTERN PREVIEW ERROR");return;}patternPreviewSteps36[slot][j]=patternWork906.bars*16;steps+=patternPreviewSteps36[slot][j];}
 if(!steps)return;patternPreviewIndex36[slot]=-1;if(previewImages18[slot]){extmem_free(previewImages18[slot]);previewImages18[slot]=nullptr;}
 beginLiveClipboardPreview(slot,selectedTrack);clipboardLivePreview[slot].endTick=steps*96;
 DISPLAY_SERIAL.println("MSG|PATTERN PREVIEW");
}
FLASHMEM bool contentPreviewTick18(uint8_t slot,uint8_t t,uint32_t tick,uint32_t end){
 uint8_t index=slot==255?4:slot;if(index>4)return false;ContentEntry18 entry36;auto*chosen=index==4?&previewContent18:&noteClips18[index];
 if(index<4&&patternClipboard[index].valid){
 uint8_t j=0;uint32_t offset=0;for(;j<patternClipboard[index].count;++j){auto&c=clips15[index][j];if(!hydrateClip15(index,j))return true;
 // Packed header contains the bar count; unpack only when changing entry.
 uint32_t span=patternPreviewSteps36[index][j]*96UL;
 if(tick<offset+span){entry36.image=c.image;entry36.locks=c.locks;entry36.count=c.count;entry36.length=patternPreviewSteps36[index][j];entry36.note=c.note;entry36.vel=c.vel;entry36.gate=c.gate;entry36.micro=c.micro;tick-=offset;end=span;chosen=&entry36;break;}offset+=span;}
 if(j>=patternClipboard[index].count)return true;
 if(patternPreviewIndex36[index]!=j){if(previewImages18[index])extmem_free(previewImages18[index]);previewImages18[index]=nullptr;patternPreviewIndex36[index]=j;}
 }
 auto&e=*chosen;if(!e.image||!end)return false;
 auto*&image=previewImages18[index];if(!image){image=(PatternImage805*)extmem_malloc(sizeof(PatternImage805));if(!image||!unpackPattern906(e.image,*image)){if(image)extmem_free(image);image=nullptr;DISPLAY_SERIAL.println("MSG|PREVIEW MEMORY");return true;}}
 MidiScope902 route(outMask902(t),t);uint8_t ch=midiChannelForTrack(t);
 auto locks=[&](int32_t at,uint8_t pitch,uint32_t duration){for(uint16_t k=0;k<e.count;++k){auto l=e.locks[k];if(l.tick!=at||l.note!=pitch)continue;contentRemapLock18(l,t,activePattern[t],0);if(l.kind==LOCK_DELAY817&&pitch<128){delayNoteLock925(l.target/256,l.parameter,l.value);continue;}if(l.kind==LOCK_ROLL1000&&pitch<128)continue;
  uint16_t target=lockMidiKind817(l.kind)?patch808Channel(t,patch808Context(t,activePattern[t])):l.target;int previous=0;if(lockMidiKind817(l.kind)){int top=lockTop817(l.kind,target,l.parameter);if(top>=0)previous=lockLeases817[top].previous;else if(l.kind==LOCK_CC817&&midiPending810[firstOut12(t)*16+target][l.parameter].known)previous=midiPending810[firstOut12(t)*16+target][l.parameter].last;}
  for(auto&lease:lockLeases817)if(!lease.used){lease={micros()+duration,0,++lockSerial817,l.value,previous,target,l.parameter,l.kind,ch,(uint8_t)(pitch&127),true,false,outMask902(t)};++lockLeaseCount817;if(lockMidiKind817(l.kind))lockSend817(l.kind,target,l.parameter,l.value);break;}
 }++modEpoch805;};
 auto play=[&](int32_t at,int16_t micro,uint8_t note,uint8_t vel,uint16_t gate){int32_t due=(at+micro+image->mov[0])%(int32_t)end;if(due<0)due+=end;if((uint32_t)due!=tick)return;uint32_t us=min(20000000UL,max(1000UL,(uint32_t)(gate*tickDurationMs()*1000.0f)));delayNoteLocksReset925();locks(at,note,us);triggerPreviewNoteWithFixedGate(ch,note,vel,us,slot,t);};
 auto&i=*image;for(uint16_t s=0;s<e.length;++s)if(i.active[s]){int32_t at=s*96+i.logical[s];play(at,i.lockTime[s]?i.micro[s]:e.micro,i.lockPitch[s]?i.note[s]:e.note,i.lockVel[s]?i.vel[s]:e.vel,i.lockGate[s]?i.gate[s]:e.gate);for(uint8_t v=0;v<i.extraCount[s];++v)play(at,i.extra14[s][v].micro,i.extraNote[s][v],i.extra14[s][v].vel,i.extra14[s][v].gate);}
 for(auto&n:i.fine)if(n.active)play(n.step*96+n.logicalMicro,n.micro,n.note,n.vel,n.gate);
 for(uint16_t k=0;k<e.count;++k){auto&l=e.locks[k];if(l.note<128||l.tick!=(int32_t)tick)continue;bool first=true;for(uint16_t j=0;j<k;++j)if(e.locks[j].tick==l.tick&&e.locks[j].note==l.note)first=false;if(first)locks(l.tick,l.note,(uint32_t)(l.duration*tickDurationMs()*1000.0f));}
 return true;
}
