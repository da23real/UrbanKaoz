#pragma once
// View changes are distinct from Select/Delete operations inside a view.
FLASHMEM void recordExitA01(){
 if(!recordingArmed&&!songRecord&&!remixRecord913)return;
 recordClose820();if(remixRecord913)remixFinishTake913();
 recordingArmed=songRecord=remixRecord913=false;
 neoDisplayDirty=displayValueDirty=displayBpmDirty=true;
 DISPLAY_SERIAL.println("MSG|REC OFF");
}
FLASHMEM void leavePadModesA01(bool preserveSelection){
 compactRecenter08=true;
 recordExitA01();
 if(!preserveSelection){leaveStepSelection(false);clearPatternSelection();selectButtonActive=selectionMode=shiftButtonActive=columnSelectionMode=false;deleteButtonActive=false;selection805Changed();}
 keyboardEnabled823=false;patternMode=grooveEditMode=false;trackChord815=false;delayMixer930=modMixer939=false;
 if(remixMode){remixLeave911();remixMode=false;pidiStateDirty931=true;}
 if(songMode){if(preserveSelection)songMode=false;else songLeaveSurface();}
 neoDisplayDirty=displayValueDirty=true;sendSelectionIndicatorsToPicran();
}
