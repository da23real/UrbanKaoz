#pragma once
// One pass over base and fine voices per frame; never use duration for hit-testing.
static uint16_t selectedGateBody11[4]={},plxGateBody41[4]={};
bool noteHasPlx35(uint8_t,uint16_t,uint8_t);
FLASHMEM void buildGateBody09(uint16_t (&rows)[4]){
 memset(selectedGateBody11,0,sizeof(selectedGateBody11));memset(plxGateBody41,0,sizeof(plxGateBody41));
 uint8_t t=selectedTrack;int32_t span=windowTickSpan(),pixel=max(1,span/16);bool bars=monoMeasureRowsView(t);
 auto mark=[&](uint16_t step,uint8_t voice){
  if(pendingDeleteNote(t,step,voice))return;
  int32_t head=(int32_t)step*96+logicalMicroTick(t,step,voice);
  int32_t end=head+pendingEditedValue(t,step,voice,EDIT_GATE,effectiveGateTicks(t,step,voice));
  uint8_t pitch=visualNoteAtVoice(t,step,voice);
  for(uint8_t row=0;row<4;++row){if(!bars&&pitch!=padChromaticNote(row*4))continue;
   int32_t start=(int32_t)viewTickOffset+(bars?row*span:0);
   if(end<=start||head>=start+span)continue;
   int first=max(0,(head-start)/pixel),last=min(15,(end-start-1)/pixel);
   for(int col=first;col<=last;++col){rows[row]|=1u<<col;if(noteHasPlx35(t,step,voice))plxGateBody41[row]|=1u<<col;if(selectedNoteObject(t,step,voice)||(selectedTrack==t&&selectedStep==step&&selectedVoice==voice))selectedGateBody11[row]|=1u<<col;}
  }
 };
 for(uint16_t step=0;step<trackStepCount(t);++step)for(uint8_t voice=0;voice<baseVoiceCountAtStep(t,step);++voice)mark(step,voice);
 for(uint16_t i=0;i<FINE_EVENTS_PER_TRACK;++i)if(fineNote[t][i].active&&fineNote[t][i].step<trackStepCount(t))mark(fineNote[t][i].step,voiceForFineIndex(t,fineNote[t][i].step,i));
}
