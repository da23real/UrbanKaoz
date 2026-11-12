#pragma once
// Only deleted objects are retained, in the existing two history slots.
struct DeletedObject23 {FineNoteEvent event;uint8_t track,pattern;};
static DeletedObject23* deletedObjects23[2]={};
static uint16_t deletedCounts23[2]={};
FLASHMEM void deleteFree23(uint8_t i){extmem_free(deletedObjects23[i]);deletedObjects23[i]=nullptr;deletedCounts23[i]=0;}
FLASHMEM void deleteShift23(){deleteFree23(0);deletedObjects23[0]=deletedObjects23[1];deletedCounts23[0]=deletedCounts23[1];deletedObjects23[1]=nullptr;deletedCounts23[1]=0;}
FLASHMEM bool deleteLockSelected23(const NoteLock817&l){
 if(l.pattern!=activePattern[l.track])return false;
 if(l.note>=128){for(uint16_t i=0;i<selectedEmptyCount;++i){auto&c=selectedEmptyCells[i];if(c.track==l.track&&c.tick==l.tick&&(c.pitch|128)==l.note)return true;}return false;}
 uint16_t s;int16_t micro;tickToStepAndMicro(l.track,l.tick,s,micro);int8_t v=findVoiceAtExactTick(l.track,s,l.note,micro);return v>=0&&selectedNoteObject(l.track,s,v);
}
FLASHMEM bool deleteSelection23(){
 commitPendingSelectionEdit();uint16_t count=0,locks=0;
 for(uint8_t t=0;t<16;++t)for(uint16_t s=0;s<trackStepCount(t);++s)for(uint8_t v=0;v<noteCountAtStep(t,s);++v)if(selectedNoteObject(t,s,v))++count;
 for(uint16_t i=0;i<lockCount817;++i)if(deleteLockSelected23(noteLocks817[i]))++locks;
 if(!count&&!locks)return true;
 auto* objects=(DeletedObject23*)extmem_malloc(max((size_t)1,count*sizeof(DeletedObject23)));if(!objects){DISPLAY_SERIAL.println("MSG|DELETE MEMORY FULL");return true;}
 uint16_t n=0;for(uint8_t t=0;t<16;++t)for(uint16_t s=0;s<trackStepCount(t);++s)for(uint8_t v=0;v<noteCountAtStep(t,s);++v)if(selectedNoteObject(t,s,v)){auto&o=objects[n++];o.track=t;o.pattern=activePattern[t];captureNoteRecord(t,s,v,o.event);}
 discardNoteRedo();if(deletedNoteHistoryCount>=NOTE_UNDO_LEVELS){lockUndoShift817();deletedNoteHistory[0]=deletedNoteHistory[1];deletedNoteHistoryCount=1;}
 uint8_t h=deletedNoteHistoryCount++;deleteFree23(h);deletedObjects23[h]=objects;deletedCounts23[h]=count;lockUndoCount817[h]=0;
 for(uint16_t i=0;i<lockCount817;)if(deleteLockSelected23(noteLocks817[i])){lockUndo817[h][lockUndoCount817[h]++]=noteLocks817[i];noteLocks817[i]=noteLocks817[--lockCount817];}else ++i;
 for(uint8_t t=0;t<16;++t)for(uint16_t s=0;s<trackStepCount(t);++s)for(int v=(int)noteCountAtStep(t,s)-1;v>=0;--v)if(selectedNoteObject(t,s,v))removeVoiceAtStep(t,s,v);
 deletedNoteHistory[h]={};deletedNoteHistory[h].valid=true;deletedNoteHistory[h].originalVoice=254;deletedNoteHistoryCursor=deletedNoteHistoryCount;
 clearStepSelection();selectedStep=selectedFineIndex=-1;freeInvalidate11();invalidateStepTriggers();selection805Changed();DISPLAY_SERIAL.println("MSG|SELECTION DELETED");return true;
}
FLASHMEM bool deleteHistory23(bool redo){
 if((redo&&deletedNoteHistoryCursor>=deletedNoteHistoryCount)||(!redo&&!deletedNoteHistoryCursor))return false;
 uint8_t h=redo?deletedNoteHistoryCursor:deletedNoteHistoryCursor-1;auto&r=deletedNoteHistory[h];if(r.originalVoice!=254)return false;
 for(uint16_t i=0;i<deletedCounts23[h];++i){auto&o=deletedObjects23[h][i];if(activePattern[o.track]!=o.pattern){DISPLAY_SERIAL.println("MSG|UNDO OTHER PATTERN");return true;}}
 for(uint16_t i=0;i<lockUndoCount817[h];++i){auto&l=lockUndo817[h][i];if(activePattern[l.track]!=l.pattern){DISPLAY_SERIAL.println("MSG|UNDO OTHER PATTERN");return true;}}
 if(!redo){
  if(lockCount817+lockUndoCount817[h]>LOCK_CAPACITY817){DISPLAY_SERIAL.println("MSG|UNDO P.LOCK CAPACITY");return true;}
  for(uint16_t i=0;i<deletedCounts23[h];++i){auto&o=deletedObjects23[h][i];if(findVoiceAtExactTick(o.track,o.event.step,o.event.note,o.event.logicalMicro)>=0){DISPLAY_SERIAL.println("MSG|UNDO NOTE COLLISION");return true;}}
  uint16_t placed=0;for(;placed<deletedCounts23[h];++placed){auto&o=deletedObjects23[h][placed];if(!movePlace939(o.track,o.event))break;}
  if(placed!=deletedCounts23[h]){while(placed){auto&o=deletedObjects23[h][--placed];int8_t v=findVoiceAtExactTick(o.track,o.event.step,o.event.note,o.event.logicalMicro);if(v>=0)removeVoiceAtStep(o.track,o.event.step,v);}DISPLAY_SERIAL.println("MSG|UNDO NOTE CAPACITY");return true;}
  for(uint16_t i=0;i<lockUndoCount817[h];++i)noteLocks817[lockCount817++]=lockUndo817[h][i];--deletedNoteHistoryCursor;r.restored=true;
 }else{
  for(uint16_t i=0;i<deletedCounts23[h];++i){auto&o=deletedObjects23[h][i];int8_t v=findVoiceAtExactTick(o.track,o.event.step,o.event.note,o.event.logicalMicro);FineNoteEvent current{};if(v<0){DISPLAY_SERIAL.println("MSG|REDO CONTENT CHANGED");return true;}captureNoteRecord(o.track,o.event.step,v,current);if(!sameNoteRecord(current,o.event)){DISPLAY_SERIAL.println("MSG|REDO CONTENT CHANGED");return true;}}
  for(uint16_t i=0;i<deletedCounts23[h];++i){auto&o=deletedObjects23[h][i];int8_t v=findVoiceAtExactTick(o.track,o.event.step,o.event.note,o.event.logicalMicro);if(v>=0)removeVoiceAtStep(o.track,o.event.step,v);}
  for(uint16_t i=0;i<lockUndoCount817[h];++i){auto&l=lockUndo817[h][i];int at=lockFind817(l.track,l.pattern,l.tick,l.note,l.kind,l.target,l.parameter);if(at>=0)noteLocks817[at]=noteLocks817[--lockCount817];}++deletedNoteHistoryCursor;r.restored=false;
 }
 freeInvalidate11();invalidateStepTriggers();selection805Changed();DISPLAY_SERIAL.println(redo?"MSG|REDO SELECTION":"MSG|UNDO SELECTION");return true;
}
