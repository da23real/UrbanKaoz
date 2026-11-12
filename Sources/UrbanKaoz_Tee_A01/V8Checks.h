#pragma once
FLASHMEM void runV8Check() {
  if(playing||songEngineActive||songMode||patternMode||remixMode||grooveEditMode||menuNavigationOnly()||
     selectButtonActive||deleteButtonActive||pendingSelectionEdit.active||selectedStep>=0||anyLiveClipboardPreview()) {
    Serial.println("V8CHECK,BUSY");return;
  }
  unsigned failures=0;
  bool oldAwait=screenUiAwaitRelease;screenUiAwaitRelease=false;
  // Real physical routing, not the legacy direct clipboard entry point.
  bool held=clipboardButtonStable[11];clipboardButtonStable[11]=true;
  handleScreenButtonPress(SCREEN_BUTTON_BITS[4]);
  clipboardButtonStable[11]=held;screenUiAwaitRelease=false;
  if(storageWriteAvailable) {
    if(!saveMenuActive||saveMenuClipboardMode)failures|=1;
    moveSaveMenu(1);confirmSaveMenu();
    if(!saveMenuActive||!saveMenuClipboardMode)failures|=2;
    moveSaveMenu(3);if(saveMenuSelection!=3)failures|=4;
    moveSaveMenu(1);confirmSaveMenu();
    if(!saveMenuActive||saveMenuClipboardMode)failures|=8;
    saveMenuSelection=0;confirmSaveMenu();
    if(fileBrowserMode!=FILE_BROWSER_SAVE||pendingSaveCandidate!=SAVE_CANDIDATE_FULL)failures|=16;
    copyField(fileBrowserName,sizeof(fileBrowserName),"V8CHECK");
    char path[64]="";bool saved=saveFullProject(path,sizeof(path));
    FullProjectHeader expected,actual;fillFullProjectHeader(expected);
    File f=saved?SD.open(path,FILE_READ):File();
    bool readback=f&&f.read((uint8_t*)&actual,sizeof(actual))==sizeof(actual)&&
      !memcmp(&expected,&actual,sizeof(actual));
    if(f)f.close();if(!saved||!readback)failures|=32;
    // Keep the uniquely named project as a recovery point; never overwrite default.
    Serial.print("V8SAVE,");Serial.print(saved&&readback?"PASS,":"FAIL,");Serial.println(path);
    closeFileBrowser();
  }else Serial.println("V8SAVE,SKIPPED,SD_NOT_WRITABLE");
  saveMenuActive=false;saveMenuClipboardMode=false;showGameScreen();screenUiAwaitRelease=false;
  if(!songWorkspaceReady){screenUiAwaitRelease=oldAwait;Serial.print("V8CHECK,SAVE_MASK=");Serial.println(failures);Serial.println("V8CHECK,SONG_SKIPPED,PSRAM_REQUIRED");return;}
  handleTransportButtonEdge(7,true);handleTransportButtonEdge(7,false);
  if(!songMode||remixMode||grooveEditMode)failures|=64;
  dispatchStableButtonPress(3);if(songMode||!songEngineActive||remixMode||grooveEditMode)failures|=128;
  handleTransportButtonEdge(7,true);handleTransportButtonEdge(7,false);
  dispatchStableButtonPress(5);if(!songDeleteArmed)failures|=256;
  dispatchStableButtonPress(5);if(songDeleteArmed)failures|=512;
  handleTransportButtonEdge(7,true);handleTransportButtonEdge(7,false);
  if(songMode||remixMode)failures|=1024;
  songEngineActive=false;patternMode=false;showGameScreen();screenUiAwaitRelease=oldAwait;
  Serial.print(failures?"V8CHECK,FAIL,MASK=":"V8CHECK,PASS,MASK=");Serial.println(failures);
}
