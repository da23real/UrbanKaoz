#pragma once
void songSeek(uint64_t);
static uint32_t browseBar40=0;static int8_t browseSide40=-1;static bool browsePlaying40=false,browseSong40=false;
FLASHMEM uint64_t songDisplayTick40(){return browseSide40>=0&&browseSong40?(uint64_t)browseBar40*1536:songTick;}
FLASHMEM void transportCancel40(){browseSide40=-1;transportMeasureRepeatAtMs[0]=transportMeasureRepeatAtMs[1]=0;}
FLASHMEM void pendingApply40(){
 for(uint8_t t=0;t<16;++t)if(pendingPattern[t]>=0){uint8_t p=pendingPattern[t];clearGrooveBridge(t);pendingPattern[t]=-1;switchTrackPattern(t,p,false);if(activePattern[t]!=p){pendingPattern[t]=p;DISPLAY_SERIAL.println("MSG|PATTERN CHANGE FAILED");}}
}
FLASHMEM void transportStep40(uint8_t side){
 uint32_t lo=browseSong40?songStartBar-1:(loopStartStep-1)/16;
 uint32_t hi=browseSong40?songEndBar-1:(loopEndStep-1)/16;
 if(side)browseBar40=min(hi,browseBar40+1);else browseBar40=browseBar40>lo?browseBar40-1:lo;
 if(browseSong40){songState();}
 else DISPLAY_SERIAL.printf("MSG|TRANSPORT BAR %lu\n",(unsigned long)browseBar40+1);
}
FLASHMEM void transportBrowse40(uint8_t index,bool pressed){
 uint8_t side=index-4;
 if(pressed){
  if(browseSide40<0){browsePlaying40=playing;browseSong40=songEngineActive;pendingApply40();browseBar40=(browseSong40?songTick:globalLoopedTick())/1536;}
  browseSide40=side;playing=internalClockRunning=false;noInterrupts();pendingClockTicks=0;interrupts();allNotesOff();
  transportStep40(side);transportMeasureRepeatAtMs[1-side]=0;transportMeasureRepeatAtMs[side]=millis()+transportTicksToMs(384);pidiStateDirty931=true;
 }else if(browseSide40==side){
  transportMeasureRepeatAtMs[side]=0;browseSide40=-1;
  if(browseSong40)songSeek((uint64_t)browseBar40*1536);else setTransportPositionToBar(browseBar40);
  playing=internalClockRunning=browsePlaying40;pidiStateDirty931=true;
 }
}
FLASHMEM void transportRepeat40(){if(controller940||browseSide40<0)return;uint8_t side=browseSide40;
 if(!transportMeasureRepeatAtMs[side]){browseSide40=-1;return;}
 uint32_t now=millis();if((int32_t)(now-transportMeasureRepeatAtMs[side])<0)return;
 transportStep40(side);transportMeasureRepeatAtMs[side]=now+transportTicksToMs(384);
}
