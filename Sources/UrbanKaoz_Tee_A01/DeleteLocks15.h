#pragma once
PatchParam808* modTrackParam21(const ModRoute805&);
PatchParam808* modBasePatch43(const ModRoute805&,uint8_t&,uint8_t&);
FLASHMEM void deleteEncoderLocks15(uint8_t id){
 NumericValue917 d;deleteButtonActive=false;bool described=numericDescribe917(id,d);deleteButtonActive=true;
 if(!described)return;
 uint8_t kind;uint16_t target,parameter;
 if(d.kind==N917_TARGET){auto&r=modUnits805[d.object].outputs[d.param];if(modIsTarget811(r)){target=modTarget810(r);parameter=r.number%8;kind=r.kind==MOD_DEPTH810?LOCK_DEPTH817:modKind931(target)&&parameter?LOCK_EXTRA817:LOCK_MOD817;if(kind==LOCK_EXTRA817)parameter=4;}else if(r.kind==MOD_TRACK21){auto*q=modTrackParam21(r);if(!q)return;kind=q->kind?(q->bits==14?LOCK_NRPN14817:LOCK_NRPN7817):LOCK_CC817;target=r.track;parameter=q->number;}else if(r.kind==MOD_CC||r.kind==MOD_PATCH810){uint8_t t,b;auto*q=modBasePatch43(r,t,b);if(!q)return;kind=LOCK_CC817;target=t;parameter=q->number;}else return;}
 else if(d.kind==N917_LFOENV27){kind=d.param<3?LOCK_MOD817:LOCK_EXTRA817;target=d.object;parameter=d.param<3?d.param+2:2;}
 else if(d.kind==N917_BLOCK){kind=LOCK_BLOCK21;target=d.t;parameter=d.field==0?12:d.field-4;}
 else if(d.kind==N917_MOD){kind=d.lock;target=d.object;parameter=d.param;}
 else if(d.kind==N917_DELAY){kind=LOCK_DELAY817;target=delayKey924(d.t,d.p,d.slot);parameter=d.field;}
 else if(d.kind==N917_PATCH){auto*q=patchEditableAt44(patch808Find(patchDb808,quickItem903),d.param);if(!q)return;kind=q->kind?(q->bits==14?LOCK_NRPN14817:LOCK_NRPN7817):LOCK_CC817;target=d.t;parameter=q->number;}
 else if(d.kind==N917_ROLLX){kind=LOCK_ROLL1000;target=d.t;parameter=d.field;}
 else return;
 auto matches=[&](const NoteLock817&l){
  if(l.kind!=kind||l.parameter!=parameter||l.pattern!=activePattern[l.track]||l.target!=freeTarget11(kind,target,l.track))return false;
  if(l.note>=128){for(uint16_t i=0;i<selectedEmptyCount;++i){auto&c=selectedEmptyCells[i];if(c.track==l.track&&c.tick==l.tick&&(c.pitch|128)==l.note)return true;}return false;}
  uint8_t t=l.track;for(uint16_t s=0;s<trackStepCount(t);++s)for(uint8_t v=0;v<noteCountAtStep(t,s);++v)if(selectedNoteObject(t,s,v)&&lockTick817(t,s,v)==l.tick&&noteAtVoice(t,s,v)==l.note)return true;
  return false;
 };
 bool any=false;for(uint16_t i=0;i<lockCount817&&!any;++i)any=matches(noteLocks817[i]);for(uint16_t i=0;i<freeDraftCount25&&!any;++i)any=matches(freeDraft25[i].lock)&&!freeDraft25[i].remove;if(!any)return;
 discardNoteRedo();if(deletedNoteHistoryCount>=NOTE_UNDO_LEVELS){lockUndoShift817();for(uint8_t i=1;i<NOTE_UNDO_LEVELS;++i)deletedNoteHistory[i-1]=deletedNoteHistory[i];deletedNoteHistoryCount=NOTE_UNDO_LEVELS-1;}
 uint8_t h=deletedNoteHistoryCount++;deletedNoteHistoryCursor=deletedNoteHistoryCount;auto&r=deletedNoteHistory[h];r={};r.valid=true;r.track=selectedTrack;r.pattern=activePattern[selectedTrack];r.originalVoice=255;lockUndoCount817[h]=0;
 for(uint16_t i=0;i<freeDraftCount25;++i){auto&d=freeDraft25[i];if(matches(d.lock)&&!d.remove&&lockFind817(d.lock.track,d.lock.pattern,d.lock.tick,d.lock.note,d.lock.kind,d.lock.target,d.lock.parameter)<0)lockUndo817[h][lockUndoCount817[h]++]=d.lock;}
 for(uint16_t i=0;i<lockCount817;){if(matches(noteLocks817[i])){int draft=lockDraftIndex26(noteLocks817[i]);if(draft<0||!freeDraft25[draft].remove)lockUndo817[h][lockUndoCount817[h]++]=draft>=0?freeDraft25[draft].lock:noteLocks817[i];noteLocks817[i]=noteLocks817[--lockCount817];}else ++i;}
 for(uint16_t i=0;i<freeDraftCount25;)if(matches(freeDraft25[i].lock))freeDraft25[i]=freeDraft25[--freeDraftCount25];else ++i;
 freeInvalidate11();++modEpoch805;neoDisplayDirty=displayValueDirty=true;
}
