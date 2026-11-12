#pragma once
struct AutoUndo46 {AutoLane46 lane{};AutoPoint46*points=nullptr;uint16_t count=0;};
static AutoUndo46 autoUndoEntries46[2];static uint8_t autoUndoCount46=0;
FLASHMEM void autoUndoClear46(){for(auto&u:autoUndoEntries46){extmem_free(u.points);u={};}autoUndoCount46=0;}
FLASHMEM bool autoRemove46(uint16_t lane){
 if(lane>=AUTO_LANES46||!autoLanes46[lane].used)return false;uint16_t count=0;for(uint16_t i=0;i<autoCount46;++i)if(autoPoints46[i].lane==lane)++count;
 auto*points=count?(AutoPoint46*)extmem_malloc(count*sizeof(AutoPoint46)):nullptr;if(count&&!points){DISPLAY_SERIAL.println("MSG|AUTO UNDO MEMORY");return false;}
 uint16_t at=0;for(uint16_t i=0;i<autoCount46;++i)if(autoPoints46[i].lane==lane)points[at++]=autoPoints46[i];
 if(autoUndoCount46==2){extmem_free(autoUndoEntries46[0].points);autoUndoEntries46[0]=autoUndoEntries46[1];autoUndoCount46=1;}
 autoUndoEntries46[autoUndoCount46++]={autoLanes46[lane],points,count};autoLanes46[lane].used=0;
 for(uint16_t i=0;i<autoCount46;)if(autoPoints46[i].lane==lane)autoPoints46[i]=autoPoints46[--autoCount46];else ++i;autoReindex46();storageDirty931=true;return true;
}
FLASHMEM bool autoUndo46(){
 if(patMenu46!=3)return false;if(!autoUndoCount46){DISPLAY_SERIAL.println("MSG|NO AUTO UNDO");return true;}
 auto&u=autoUndoEntries46[autoUndoCount46-1];uint16_t lane=0;while(lane<AUTO_LANES46&&autoLanes46[lane].used)++lane;
 if(lane==AUTO_LANES46||autoCount46+u.count>AUTO_POINTS46){DISPLAY_SERIAL.println("MSG|AUTO UNDO CAPACITY");return true;}
 autoLanes46[lane]=u.lane;for(uint16_t i=0;i<u.count;++i){auto p=u.points[i];p.lane=lane;autoPoints46[autoCount46++]=p;}extmem_free(u.points);u={};--autoUndoCount46;autoSelected46=lane;autoReindex46();storageDirty931=true;patRender46();return true;
}
FLASHMEM void autoNoteRemove46(uint8_t t,int32_t tick,uint8_t note){for(uint16_t i=0;i<autoMuteCount46;){auto&m=autoMutes46[i];if(m.track==t&&m.pattern==activePattern[t]&&m.identity==tick&&m.note==note)m=autoMutes46[--autoMuteCount46];else ++i;}}
FLASHMEM void autoNoteMove46(uint8_t t,int32_t tick,uint8_t note,int32_t destination){for(uint16_t i=0;i<autoMuteCount46;++i){auto&m=autoMutes46[i];if(m.track==t&&m.pattern==activePattern[t]&&m.identity==tick&&m.note==note){autoShiftScope49(m,destination-tick,trackSequenceTicks(t));m.identity=destination;}}}
FLASHMEM void autoNotePitch46(uint8_t t,int32_t tick,uint8_t note,uint8_t pitch){for(uint16_t i=0;i<autoMuteCount46;++i){auto&m=autoMutes46[i];if(m.track==t&&m.pattern==activePattern[t]&&m.identity==tick&&m.note==note)m.note=pitch;}}
static EXTMEM AutoMute46 autoNoteHistory46[2][AUTO_MUTES46];static uint16_t autoNoteHistoryCount46[2]={};static uint8_t autoNoteHistoryMap46[2]={0,1};
FLASHMEM void autoNoteUndoCapture46(uint8_t index,uint8_t t,int32_t tick,uint8_t note){uint8_t h=autoNoteHistoryMap46[index];autoNoteHistoryCount46[h]=0;for(uint16_t i=0;i<autoMuteCount46;++i){auto&m=autoMutes46[i];if(m.track==t&&m.pattern==activePattern[t]&&m.identity==tick&&m.note==note)autoNoteHistory46[h][autoNoteHistoryCount46[h]++]=m;}}
FLASHMEM void autoNoteUndoShift46(){uint8_t h=autoNoteHistoryMap46[0];autoNoteHistoryMap46[0]=autoNoteHistoryMap46[1];autoNoteHistoryMap46[1]=h;autoNoteHistoryCount46[h]=0;}
FLASHMEM bool autoNoteUndoRoom46(uint8_t index){return autoMuteCount46+autoNoteHistoryCount46[autoNoteHistoryMap46[index]]<=AUTO_MUTES46;}
FLASHMEM void autoNoteUndoInstall46(uint8_t index){uint8_t h=autoNoteHistoryMap46[index];for(uint16_t i=0;i<autoNoteHistoryCount46[h];++i)autoMutes46[autoMuteCount46++]=autoNoteHistory46[h][i];}
struct AutoModHistory46 {AutoLane46 lanes[AUTO_LANES46];AutoPoint46 points[AUTO_POINTS46];AutoMute46 mutes[AUTO_MUTES46];uint16_t count=0,muteCount=0,laneCount=0;};
static EXTMEM AutoModHistory46 autoModHistory46[2];
FLASHMEM void autoModHistoryReset46(uint8_t index){auto&h=autoModHistory46[index];h.count=h.muteCount=h.laneCount=0;}
FLASHMEM bool autoTargets46(const AutoLane46&l,uint16_t key){return l.used&&((l.type==2&&l.target==key)||(l.type==1&&modLock931(l.kind)&&l.target==key)||(!l.type&&modIsTarget811(l.route)&&modTarget810(l.route)==key));}
FLASHMEM void autoModCapture46(uint16_t key,uint8_t index){
 auto&h=autoModHistory46[index];autoModHistoryReset46(index);uint16_t map[AUTO_LANES46];for(auto&v:map)v=AUTO_NONE46;
 for(uint16_t i=0;i<AUTO_LANES46;++i)if(autoTargets46(autoLanes46[i],key)){map[i]=h.laneCount;h.lanes[h.laneCount++]=autoLanes46[i];}
 for(uint16_t i=0;i<autoCount46;++i)if(map[autoPoints46[i].lane]!=AUTO_NONE46){auto p=autoPoints46[i];p.lane=map[p.lane];h.points[h.count++]=p;}
 for(uint16_t i=0;i<autoMuteCount46;++i)if(autoMutes46[i].key==key)h.mutes[h.muteCount++]=autoMutes46[i];
}
FLASHMEM bool autoModRoom46(uint8_t index){auto&h=autoModHistory46[index];uint16_t free=0;for(auto&l:autoLanes46)free+=!l.used;return h.count+autoCount46<=AUTO_POINTS46&&h.muteCount+autoMuteCount46<=AUTO_MUTES46&&h.laneCount<=free;}
FLASHMEM void autoModRestore46(uint8_t index){auto&h=autoModHistory46[index];uint16_t map[AUTO_LANES46],next=0;for(uint16_t i=0;i<h.laneCount;++i){while(next<AUTO_LANES46&&autoLanes46[next].used)++next;map[i]=next;autoLanes46[next++]=h.lanes[i];}for(uint16_t i=0;i<h.count;++i){auto p=h.points[i];p.lane=map[p.lane];autoPoints46[autoCount46++]=p;}for(uint16_t i=0;i<h.muteCount;++i)autoMutes46[autoMuteCount46++]=h.mutes[i];autoModHistoryReset46(index);autoReindex46();}

FLASHMEM void autoHistoryClear46(){for(auto&v:autoNoteHistoryCount46)v=0;for(uint8_t i=0;i<2;++i)autoModHistoryReset46(i);autoUndoClear46();}

FLASHMEM void writeDraftClear46(){ if(writeMask41&64)for(uint16_t i=0;i<freeDraftCount25;){auto&l=freeDraft25[i].lock;bool remove=false;if((writeTracks41&(1u<<l.track))&&l.pattern==activePattern[l.track]){if(!writeSelection46)remove=true;else for(uint16_t n=0;n<writeNoteCount46;++n){auto&v=writeNotes46[n];if(v.track==l.track&&v.identity==l.tick&&v.note==l.note){remove=true;break;}}}if(remove)freeDraft25[i]=freeDraft25[--freeDraftCount25];else ++i;}}
