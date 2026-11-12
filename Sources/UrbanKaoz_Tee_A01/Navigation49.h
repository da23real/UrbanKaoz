#pragma once
FLASHMEM void patternDown51(){
 if(!orderReady51)orderReset51();bool changed=false;
 for(uint8_t t=0;t<16;++t)for(int row=14;row>=0;--row){uint8_t a=patternOrder51[t][row],b=patternOrder51[t][row+1];if(patternSelected[t][a]&&!patternSelected[t][b]){patternOrder51[t][row]=b;patternOrder51[t][row+1]=a;changed=true;}}
 if(changed){storageDirty931=true;neoDisplayDirty=displayValueDirty=true;if(lastPatternValid){uint8_t row=patternRow51(lastPatternTrack,lastPatternIndex);if(row>=patternRowOffset+4)patternRowOffset=min((uint8_t)12,(uint8_t)(row-3));}}
}

// Physical low bits of the extended nine-register chain, independent of legacy IDs.
static bool navigationInitialized49=false;
static uint16_t navigationStable49=0xffff,navigationLast49=0xffff;
static uint32_t navigationChanged49[16]={};
FLASHMEM void navigationPress49(uint8_t physical){
 if(physical!=4&&physical!=5&&physical!=6&&physical!=7&&physical!=12&&physical!=13&&physical!=14&&physical!=15)return;
 presentationTouch43();inputActivity811=millis();
 if(debugTestMode!=DEBUG_TEST_NONE){Serial.printf("NAV49,TEST,B%u\n",physical);return;}
 if(assignOpen932||buttonSettingsLiveTest()||controller940||sdBusy905())return;
 NeoInputScope932 input;
 if(patternMode&&(physical==12||physical==15)){patternRowOffset=clampInt((int)patternRowOffset+(physical==12?1:-1),0,PATTERN_COUNT-4);neoDisplayDirty=true;return;}
 if(physical==15)zoomAroundSelection(1,VIEW_SRC_J2_ZOOM);
 else if(physical==12)zoomAroundSelection(-1,VIEW_SRC_J2_ZOOM);
 else if(physical==14)p23MoveViewPage(-1);
 else if(physical==13)p23MoveViewPage(1);
 else if(physical==7){
   if(remixMode&&remixSelection911.steps)remixFinishSelection911(false);
   else cancelStepSelectionWithoutCommit();
   neoDisplayDirty=displayValueDirty=true;
 }else if(physical==6){
   // Commit edits, retaining note masks, selectedStep and the open EP page.
   if(remixMode&&remixSelection911.steps)return; // Other uses remain unspecified.
   lockHoldCancel26();commitPendingSelectionEdit();
   if(pendingSelectionEdit.active||lockDraftAny26())return;
   moveFinish41(true);selectionIndicatorsPending931=true;
   neoDisplayDirty=displayValueDirty=true;
 }
 if(physical==4||physical==5){
  int delta=physical==4?-1:1;
  if(patternMode){patternRowOffset=clampInt((int)patternRowOffset+delta,0,PATTERN_COUNT-4);neoDisplayDirty=true;}
  else if(noteViewForTrack(selectedTrack)==NOTE_VIEW_COMPACT)moveCompactPitchRows(delta);
  else {chromaticBottomNote=clampInt((int)chromaticBottomNote-delta,0,124);neoDisplayDirty=true;}
 }

}
FLASHMEM void serviceNavigation49(uint16_t raw,uint32_t now){
 if(!navigationInitialized49){navigationInitialized49=true;navigationStable49=navigationLast49=raw;for(auto&v:navigationChanged49)v=now;return;}
 for(uint8_t bit=0;bit<16;++bit){uint16_t mask=1u<<bit;
   if((raw^navigationLast49)&mask){navigationLast49=(navigationLast49&~mask)|(raw&mask);navigationChanged49[bit]=now;}
   bool pressed=!(raw&mask);uint16_t debounce=pressed?hc165PressDebounceMs:hc165ReleaseDebounceMs;
   if(((raw^navigationStable49)&mask)&&(uint32_t)(now-navigationChanged49[bit])>=debounce){
     navigationStable49=(navigationStable49&~mask)|(raw&mask);
     Serial.printf("NAV49,%s,B%u\n",pressed?"PRESS":"RELEASE",bit);
     if(pressed)navigationPress49(bit);
   }
 }
}
