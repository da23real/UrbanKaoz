#pragma once
// Event lookup preserves registry order, including multiple PLX on one note.
// VA01.31 keeps the immutable registry index across slices and lease changes.
// All other epoch/count invalidations remain conservative; flags expose VA01.30.
EXTMEM int16_t lockEventHeads30[1024],lockEventLinks30[LOCK_CAPACITY817];
static uint32_t lockEventEpoch30=UINT32_MAX,lockEventSlice30=UINT32_MAX;
static uint16_t lockEventCount30=UINT16_MAX,lockEventFree30=0;
FLASHMEM void lockEventInvalidate30(){lockEventCount30=UINT16_MAX;++modEpoch805;}
FLASHMEM unsigned lockEventHash30(uint8_t t,uint8_t p,int32_t tick,uint8_t note){return ((uint32_t)tick*2654435761u+t*1619u+p*31337u+note*6971u)&1023;}
FLASHMEM void lockEventBuild30(){
 if(lockEventEpoch30==registryEpoch31()&&((optim31&1)||lockEventSlice30==agendaSlice830)&&lockEventCount30==lockPlaybackCount39())return;
 Scope830 eventMeasure30(P830_INDEX30);++indexBuilds31;memset(lockEventHeads30,255,sizeof(lockEventHeads30));lockEventFree30=0;
 for(int i=(int)lockPlaybackCount39()-1;i>=0;--i){auto&l=lockPlaybackEntry39(i);unsigned h=lockEventHash30(l.track,l.pattern,l.tick,l.note);lockEventLinks30[i]=lockEventHeads30[h];lockEventHeads30[h]=i;if(l.note>=128&&l.track<16)lockEventFree30|=1u<<l.track;}
 lockEventEpoch30=registryEpoch31();lockEventSlice30=agendaSlice830;lockEventCount30=lockPlaybackCount39();
}
FLASHMEM bool lockEventSame30(const NoteLock817&a,uint8_t t,uint8_t p,int32_t tick,uint8_t note){return a.track==t&&a.pattern==p&&a.tick==tick&&a.note==note;}
FLASHMEM int lockEventFirst30(uint8_t t,uint8_t p,int32_t tick,uint8_t note){
 if(!lockPlaybackCount39())return -1;
 if(!blockTargeted30)return 0; // Existing callers retain their exact predicates.
 lockEventBuild30();int i=lockEventHeads30[lockEventHash30(t,p,tick,note)];
 while(i>=0&&!lockEventSame30(lockPlaybackEntry39(i),t,p,tick,note))i=lockEventLinks30[i];return i;
}
FLASHMEM int lockEventNext30(int current){
 if(!blockTargeted30)return current+1<lockPlaybackCount39()?current+1:-1;
 auto&key=lockPlaybackEntry39(current);int i=lockEventLinks30[current];
 while(i>=0&&!lockEventSame30(lockPlaybackEntry39(i),key.track,key.pattern,key.tick,key.note))i=lockEventLinks30[i];return i;
}
FLASHMEM bool lockEventPresent30(uint8_t t,uint8_t p,int32_t tick,uint8_t note){
 for(int i=lockEventFirst30(t,p,tick,note);i>=0;i=lockEventNext30(i))if(lockEventSame30(lockPlaybackEntry39(i),t,p,tick,note))return true;return false;
}
FLASHMEM bool lockFreeTrack30(uint8_t t){if(!blockTargeted30)return true;lockEventBuild30();return (lockEventFree30&(1u<<t))!=0;}

