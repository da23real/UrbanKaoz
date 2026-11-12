#pragma once
static bool freeDirty11=true;
static uint16_t freeTracks11=0;
static uint32_t freeNext11[16]={},freeLast11[16]={};
static uint64_t freeAbsolute11[16]={};
static uint8_t freePattern11[16]={};
static uint16_t freeCount11=65535;
FLASHMEM void freeInvalidate11(){freeDirty11=true;lockEventInvalidate30();}
FLASHMEM uint16_t freeTarget11(uint8_t kind,uint16_t target,uint8_t t){return (lockMidiKind817(kind)||kind==LOCK_ROLL1000||kind==LOCK_BLOCK21)?t:kind==LOCK_DELAY817?(target/256)*256+t*16+activePattern[t]:target;}
FLASHMEM int freeIndex11(const EmptySelectionCell&c,uint8_t kind,uint16_t target,uint16_t parameter){return lockFind817(c.track,activePattern[c.track],c.tick,c.pitch|128,kind,freeTarget11(kind,target,c.track),parameter);}
struct FreeDraft25 {NoteLock817 lock;bool remove;};
static EXTMEM FreeDraft25 freeDraft25[LOCK_CAPACITY817];static uint16_t freeDraftCount25=0;
static EXTMEM int16_t lockPlaybackMap39[LOCK_CAPACITY817];
static uint16_t lockPlaybackSize39=0,lockPlaybackBaseCount39=UINT16_MAX;
static uint32_t lockPlaybackEpoch39=UINT32_MAX;
FLASHMEM uint16_t lockPlaybackCount39(){
 if(!freeDraftCount25)return lockCount817;
 if(lockPlaybackEpoch39==registryEpoch31()&&lockPlaybackBaseCount39==lockCount817)return lockPlaybackSize39;
 lockPlaybackSize39=0;
 for(uint16_t i=0;i<lockCount817;++i)if(lockDraftIndex26(noteLocks817[i])<0)lockPlaybackMap39[lockPlaybackSize39++]=i;
 for(uint16_t i=0;i<freeDraftCount25;++i)if(!freeDraft25[i].remove&&lockPlaybackSize39<LOCK_CAPACITY817)lockPlaybackMap39[lockPlaybackSize39++]=-1-(int)i;
 lockPlaybackEpoch39=registryEpoch31();lockPlaybackBaseCount39=lockCount817;return lockPlaybackSize39;
}
FLASHMEM const NoteLock817& lockPlaybackEntry39(int i){if(!freeDraftCount25)return noteLocks817[i];int at=lockPlaybackMap39[i];return at<0?freeDraft25[-1-at].lock:noteLocks817[at];}
FLASHMEM bool lockPlaybackDraft39(int i){return freeDraftCount25&&lockPlaybackMap39[i]<0;}
FLASHMEM bool lockDraftAny26(){return freeDraftCount25!=0;}
FLASHMEM bool lockDraftRoom26(uint16_t need){return freeDraftCount25+need<=LOCK_CAPACITY817&&lockCount817+need<=LOCK_CAPACITY817;}
FLASHMEM int lockDraftIndex26(const NoteLock817& key){for(uint16_t i=0;i<freeDraftCount25;++i){const auto&l=freeDraft25[i].lock;if(l.track==key.track&&l.pattern==key.pattern&&l.tick==key.tick&&l.note==key.note&&l.kind==key.kind&&l.target==key.target&&l.parameter==key.parameter)return i;}return -1;}
FLASHMEM int lockDraftValue26(const NoteLock817& key,int base){int d=lockDraftIndex26(key);if(d>=0)return freeDraft25[d].remove?base:freeDraft25[d].lock.value;int i=lockFind817(key.track,key.pattern,key.tick,key.note,key.kind,key.target,key.parameter);return i>=0?noteLocks817[i].value:base;}
FLASHMEM bool lockDraftPut26(const NoteLock817& key,bool remove){int d=lockDraftIndex26(key);if(d<0){if(freeDraftCount25>=LOCK_CAPACITY817){DISPLAY_SERIAL.println("MSG|P.LOCK CAPACITY");return false;}
 if(!remove&&lockFind817(key.track,key.pattern,key.tick,key.note,key.kind,key.target,key.parameter)<0){uint16_t count=0,total=lockCount817;
  for(uint16_t j=0;j<lockCount817;++j){auto&l=noteLocks817[j];int at=lockDraftIndex26(l);if(at>=0&&freeDraft25[at].remove)continue;if(l.track==key.track&&l.pattern==key.pattern&&l.tick==key.tick&&l.note==key.note)++count;}
  for(uint16_t j=0;j<freeDraftCount25;++j){auto&x=freeDraft25[j];auto&l=x.lock;if(!x.remove&&lockFind817(l.track,l.pattern,l.tick,l.note,l.kind,l.target,l.parameter)<0){++total;if(l.track==key.track&&l.pattern==key.pattern&&l.tick==key.tick&&l.note==key.note)++count;}}
  if(count>=32||total>=LOCK_CAPACITY817){DISPLAY_SERIAL.println("MSG|P.LOCK CAPACITY");return false;}
 }d=freeDraftCount25++;}freeDraft25[d]={key,remove};freeInvalidate11();neoDisplayDirty=displayValueDirty=true;return true;}
FLASHMEM int freeDraftIndex25(const EmptySelectionCell&c,uint8_t kind,uint16_t target,uint16_t param){for(uint16_t i=0;i<freeDraftCount25;++i){const auto&l=freeDraft25[i].lock;if(l.track==c.track&&l.pattern==activePattern[c.track]&&l.tick==c.tick&&l.note==(c.pitch|128)&&l.kind==kind&&l.target==freeTarget11(kind,target,c.track)&&l.parameter==param)return i;}return -1;}
FLASHMEM bool freeDraftFinish25(bool commit){
 if(commit){int required=lockCount817;for(uint16_t j=0;j<freeDraftCount25;++j){auto&d=freeDraft25[j];auto&l=d.lock;bool present=lockFind817(l.track,l.pattern,l.tick,l.note,l.kind,l.target,l.parameter)>=0;required+=d.remove?-(int)present:!present;}if(required>LOCK_CAPACITY817){DISPLAY_SERIAL.println("MSG|P.LOCK CAPACITY");return false;}}
 if(commit)for(uint16_t j=0;j<freeDraftCount25;++j)if(freeDraft25[j].remove){auto&l=freeDraft25[j].lock;int i=lockFind817(l.track,l.pattern,l.tick,l.note,l.kind,l.target,l.parameter);if(i>=0)noteLocks817[i]=noteLocks817[--lockCount817];}
 if(commit)for(uint16_t j=0;j<freeDraftCount25;++j){auto&d=freeDraft25[j];auto&l=d.lock;int i=lockFind817(l.track,l.pattern,l.tick,l.note,l.kind,l.target,l.parameter);if(d.remove){if(i>=0)noteLocks817[i]=noteLocks817[--lockCount817];}else {if(i<0)i=lockCount817++;noteLocks817[i]=l;}}
 lockClearDraft39();freeDraftCount25=0;freeInvalidate11();++modEpoch805;neoDisplayDirty=displayValueDirty=true;return true;
}
FLASHMEM int freeGateValue11(){
 if(!selectedEmptyCount)return defaultGateTicks[selectedTrack];const auto&c=selectedEmptyCells[0];
 for(uint16_t i=0;i<freeDraftCount25;++i){auto&l=freeDraft25[i].lock;if(l.track==c.track&&l.pattern==activePattern[c.track]&&l.tick==c.tick&&l.note==(c.pitch|128))return l.duration;}
 for(uint16_t i=0;i<lockCount817;++i){auto&l=noteLocks817[i];if(l.track==c.track&&l.pattern==activePattern[c.track]&&l.tick==c.tick&&l.note==(c.pitch|128))return l.duration;}
 return max(1UL,c.gate);
}
FLASHMEM bool freeGateEdit11(int delta,bool absolute){
 if(patternMode||!selectedEmptyCount||selectionHasActiveNotes())return false;
 for(uint16_t k=0;k<selectedEmptyCount;++k){auto&c=selectedEmptyCells[k];uint32_t gate=c.gate;
  for(uint16_t i=0;i<lockCount817;++i){auto&l=noteLocks817[i];if(l.track==c.track&&l.pattern==activePattern[c.track]&&l.tick==c.tick&&l.note==(c.pitch|128)){gate=l.duration;break;}}
  for(uint16_t j=0;j<freeDraftCount25;++j){auto&l=freeDraft25[j].lock;if(l.track==c.track&&l.pattern==activePattern[c.track]&&l.tick==c.tick&&l.note==(c.pitch|128)){gate=l.duration;break;}}
  c.gate=absolute?clampInt(delta,1,65535):WRAP23(gate,delta,1,65535);
  for(uint16_t i=0;i<lockCount817;++i){auto&l=noteLocks817[i];if(l.track==c.track&&l.pattern==activePattern[c.track]&&l.tick==c.tick&&l.note==(c.pitch|128)){int d=freeDraftIndex25(c,l.kind,l.target,l.parameter);if(d<0&&freeDraftCount25<LOCK_CAPACITY817){d=freeDraftCount25++;freeDraft25[d]={l,false};}if(d>=0)freeDraft25[d].lock.duration=c.gate;}}
  for(uint16_t j=0;j<freeDraftCount25;++j){auto&l=freeDraft25[j].lock;if(l.track==c.track&&l.pattern==activePattern[c.track]&&l.tick==c.tick&&l.note==(c.pitch|128))l.duration=c.gate;}
 }freeDirty11=true;displayValueDirty=neoDisplayDirty=true;return true;
}
FLASHMEM bool freeEdit11(uint8_t kind,uint16_t target,uint16_t parameter,int base,int delta,int lo,int hi){
 if(patternMode||!explicitNoteSelection||!selectedEmptyCount)return false;
 uint16_t need=0;for(uint16_t k=0;k<selectedEmptyCount;++k)if(freeDraftIndex25(selectedEmptyCells[k],kind,target,parameter)<0)++need;
 if(freeDraftCount25+need>LOCK_CAPACITY817||lockCount817+freeDraftCount25+need>LOCK_CAPACITY817){DISPLAY_SERIAL.println("MSG|P.LOCK CAPACITY");return true;}
 for(uint16_t k=0;k<selectedEmptyCount;++k){auto&c=selectedEmptyCells[k];int base59=indBase50(c.track,activePattern[c.track],kind,freeTarget11(kind,target,c.track),parameter,base);int d=freeDraftIndex25(c,kind,target,parameter),i=freeIndex11(c,kind,target,parameter);int old=d>=0?(freeDraft25[d].remove?base59:freeDraft25[d].lock.value):i>=0?noteLocks817[i].value:base59;
  int value=(kind==LOCK_DELAY817&&(parameter<=2||parameter==4))?clampInt(old+delta,lo,hi):WRAP23(old,delta,lo,hi);
  bool existed=d>=0;if(d<0){uint16_t count=0;for(uint16_t j=0;j<lockCount817;++j){auto&l=noteLocks817[j];if(l.track==c.track&&l.pattern==activePattern[c.track]&&l.tick==c.tick&&l.note==(c.pitch|128))++count;}for(uint16_t j=0;j<freeDraftCount25;++j){auto&l=freeDraft25[j].lock;if(l.track==c.track&&l.pattern==activePattern[c.track]&&l.tick==c.tick&&l.note==(c.pitch|128)&&lockFind817(l.track,l.pattern,l.tick,l.note,l.kind,l.target,l.parameter)<0)++count;}if(i<0&&count>=32){DISPLAY_SERIAL.println("MSG|32 P.LOCKS PER EVENT");continue;}d=freeDraftCount25++;}
  uint32_t duration=existed?freeDraft25[d].lock.duration:i>=0?noteLocks817[i].duration:max(1UL,c.gate);
  freeDraft25[d]={{c.tick,value,freeTarget11(kind,target,c.track),parameter,c.track,activePattern[c.track],(uint8_t)(c.pitch|128),kind,duration},deleteButtonActive||value==base59};
 }freeInvalidate11();displayValueDirty=neoDisplayDirty=true;return true;
}
FLASHMEM int freeDisplay11(uint8_t kind,uint16_t target,uint16_t parameter,int base){
 if(!selectedEmptyCount)return base;auto&c=selectedEmptyCells[0];int d=freeDraftIndex25(c,kind,target,parameter);if(d>=0)return freeDraft25[d].remove?base:freeDraft25[d].lock.value;int i=freeIndex11(c,kind,target,parameter);return i<0?base:noteLocks817[i].value;
}
FLASHMEM bool lockDifferent11(uint8_t kind,uint16_t target,uint16_t parameter,int base){if(!lockWantsEdit817())return false;for(uint16_t i=0;i<freeDraftCount25;++i){auto&l=freeDraft25[i].lock;if(l.kind==kind&&l.target==target&&l.parameter==parameter)return true;}return false;}
FLASHMEM void lockBlinkSend11(uint8_t screen,uint16_t mask){if(screen<4){DISPLAY_SERIAL.printf("BLK11,%u,%u\n",screen,mask);}}
FLASHMEM void freePlayTrack11(uint8_t t,uint32_t tick,uint8_t pattern){
 if(pattern==255)pattern=activePattern[t];uint64_t absolute=songEngineActive?songTick:transportTickAbsolute;
 if(freeCount11!=lockPlaybackCount39()){freeCount11=lockPlaybackCount39();freeDirty11=true;}
 if(freeDirty11){freeTracks11=0;for(uint16_t i=0;i<lockPlaybackCount39();++i)if(lockPlaybackEntry39(i).note>=128)freeTracks11|=1u<<lockPlaybackEntry39(i).track;for(uint8_t j=0;j<16;++j){freeNext11[j]=0;freeLast11[j]=UINT32_MAX;freeAbsolute11[j]=UINT64_MAX;}freeDirty11=false;}
 if(!(freeTracks11&(1u<<t)))return;
 bool seek=tick<freeLast11[t]||freeAbsolute11[t]==UINT64_MAX||freePattern11[t]!=pattern;freePattern11[t]=pattern;freeLast11[t]=tick;
 if(!seek&&tick<freeNext11[t])return;if(freeAbsolute11[t]==absolute)return;
 freeAbsolute11[t]=absolute;uint32_t next=UINT32_MAX;bool pitches[128]={};
 for(uint16_t i=0;i<lockPlaybackCount39();++i){auto&l=lockPlaybackEntry39(i);if(l.track!=t||l.pattern!=pattern||l.note<128)continue;
  if(l.tick>(int32_t)tick)next=min(next,(uint32_t)l.tick);
  if(l.tick!=(int32_t)tick||pitches[l.note&127])continue;pitches[l.note&127]=true;
  MidiScope902 route(outMask902(t),t);auto&ctx=patch808Context(t,pattern);uint8_t channel=patch808Channel(t,ctx);
  lockBegin817(t,pattern,l.tick,l.note,channel,l.note,(uint32_t)min(1800000000.0f,l.duration*tickDurationMs()*1000.0f));
 }freeNext11[t]=next;
}
FLASHMEM bool freeTail11(uint8_t t,uint8_t pitch,uint32_t start,uint32_t end){
 for(uint16_t s=0;s<trackStepCount(t);++s)for(uint8_t v=0;v<noteCountAtStep(t,s);++v){if(noteAtVoice(t,s,v)!=pitch)continue;int32_t head=lockTick817(t,s,v);if(head<(int32_t)start&&head+(int32_t)effectiveGateTicks(t,s,v)>(int32_t)start)return true;}return false;
}
FLASHMEM void freeDelaySnapshot11(uint8_t t){
 if(!playbackLocks828)return;lockService817();
 for(uint8_t s=0;s<4;++s)for(uint8_t f=0;f<18;++f){if(f>=12&&f<15)continue;
  uint16_t target=s*256+t*16+activePattern[t];int at=lockTop817(LOCK_DELAY817,target,f);int value=at<0?autoParameter46(LOCK_DELAY817,target,f,indBase50(t,indRuntimePattern50(t),LOCK_DELAY817,target,f,INT32_MIN)):lockLeases817[at].value;if(value==INT32_MIN)continue;
  bool present=freeDelayHas11(s*32+f);
  if(!present)delayNoteLock925(s,f,value);
 }
}
FLASHMEM int32_t freeTick11(uint8_t t,uint8_t pitch,uint32_t start,uint32_t end){int32_t tick=INT32_MAX;for(uint16_t i=0;i<lockCount817;++i){auto&l=noteLocks817[i];if(l.note>=128&&l.track==t&&l.pattern==activePattern[t]&&(l.note&127)==pitch&&l.tick>=(int32_t)start&&l.tick<(int32_t)end)tick=min(tick,l.tick);}return tick==INT32_MAX?start:tick;}
FLASHMEM bool freeDeleteSelected11(){bool removed=false;for(uint16_t k=0;k<selectedEmptyCount;++k){auto c=selectedEmptyCells[k];if(!freeTail11(c.track,c.pitch,c.tick,c.tick+1))removed|=freeDeletePad11(c.track,c.pitch,c.tick,c.tick+1);}return removed;}
FLASHMEM bool freeAtPad11(uint8_t t,uint8_t pitch,uint32_t start,uint32_t end){if(!lockFreeTrack30(t))return false;for(uint16_t i=0;i<lockCount817;++i){auto&l=noteLocks817[i];if(l.note>=128&&l.track==t&&l.pattern==activePattern[t]&&(l.note&127)==pitch&&l.tick>=(int32_t)start&&l.tick<(int32_t)end)return true;}return false;}
FLASHMEM bool freeDeletePad11(uint8_t t,uint8_t pitch,uint32_t start,uint32_t end){
 uint16_t count=0;for(uint16_t i=0;i<lockCount817;++i){auto&l=noteLocks817[i];if(l.note>=128&&l.track==t&&l.pattern==activePattern[t]&&(l.note&127)==pitch&&l.tick>=(int32_t)start&&l.tick<(int32_t)end)++count;}
 if(!count)return false;if(count>32){DISPLAY_SERIAL.println("MSG|ZOOM TO DELETE P.LOCKS");return false;}
 discardNoteRedo();if(deletedNoteHistoryCount>=NOTE_UNDO_LEVELS){lockUndoShift817();for(uint8_t i=1;i<NOTE_UNDO_LEVELS;++i)deletedNoteHistory[i-1]=deletedNoteHistory[i];deletedNoteHistoryCount=NOTE_UNDO_LEVELS-1;}
 uint8_t history=deletedNoteHistoryCount++;deletedNoteHistoryCursor=deletedNoteHistoryCount;auto&record=deletedNoteHistory[history];record={};record.valid=true;record.track=t;record.pattern=activePattern[t];record.originalVoice=255;lockUndoCount817[history]=0;
 bool removed=false;for(uint16_t i=0;i<lockCount817;){auto&l=noteLocks817[i];if(l.note>=128&&l.track==t&&l.pattern==activePattern[t]&&(l.note&127)==pitch&&l.tick>=(int32_t)start&&l.tick<(int32_t)end){lockUndo817[history][lockUndoCount817[history]++]=l;l=noteLocks817[--lockCount817];removed=true;}else ++i;}
 if(removed){freeDirty11=true;++modEpoch805;neoDisplayDirty=displayValueDirty=true;}return removed;
}
