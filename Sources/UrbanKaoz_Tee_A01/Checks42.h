#pragma once
// Explicit synthetic test command, only after B29PREP. Never captures a user session.
FLASHMEM void checks42(){
 if(!benchPrepared29||playing||!sdAvailable||!storageWriteAvailable||sdBusy905()){Serial.println("CHECK42,BUSY_PREPARE_SYNTHETIC_STOP_SD");return;}
 const char*path="/URBANKAOZ/PLink/TEST42.PLK";
 if(SD.exists(path)){Serial.println("CHECK42,REFUSE_EXISTING_FIXTURE");return;}
 unsigned pass=0,fail=0;auto ck=[&](bool ok,const char*n){++(ok?pass:fail);Serial.printf("CHECK42,%s,%s\n",ok?"PASS":"FAIL",n);};
 benchPrepare29(1,4,4,1,120,0);playing=false;
 remixDraft911.notes.clear();remixDraft911.properties.clear();remixDraft911.mask=1;remixDraft911.nextId=2;
 for(auto&t:remixDraft911.tracks){t=rmx911::Track{};t.bars=1;t.delay[1]=1;for(uint8_t i=0;i<6;++i)t.roll[i]=clampRollFieldValue(i,0);}
 ck(remixDraft911.notes.add({1,0,96,0,0,72,100,0}),"synthetic_draft");remixBars911=1;remixTargetMask911=1;remixMode=true;
 remixBrowserOpen911(true);strcpy(fileBrowserName,"TEST42");executeBrowserSave();
 rmx911::Model read;PLinkHeader911 header;ck(plinkRead911(path,read,header)&&read.notes.size==1&&read.notes[0].pitch==72,"real_Remix_Save_readback");
 if(!fail){
  remixDraft911.notes[0].pitch=60;strcpy(remixLoadPath911,path);remixLoading911=true;remixLoadSource42=false;remixReady911=false;uint8_t before=noteAtVoice(0,0,0);remixLoadChoice911();
  ck(remixDraft911.notes.size==1&&remixDraft911.notes[0].pitch==72&&noteAtVoice(0,0,0)==before,"Load_to_Remix_source_unchanged");
  remixLoading911=true;remixLoadSource42=true;remixLoadBar42=1;remixConfirm911=false;remixLoadChoice911();ck(remixConfirm911&&noteAtVoice(0,16,0)==before,"source_confirmation_before_replace");remixLoadChoice911();
  ck(trackBars[0]==4&&noteAtVoice(0,0,0)==before&&noteAtVoice(0,16,0)==72&&noteAtVoice(0,32,0)==before,"source_replace_one_measure_only");
 }
 remixBrowser911=true;openLoadBrowser();handleScreenButtonPress(SCREEN_BUTTON_BITS[0]);ck(!remixBrowser911&&fileBrowserMode==FILE_BROWSER_LOAD&&!strcmp(fileBrowserPath,"/URBANKAOZ/PRJ"),"Full_Load_leaves_Remix_browser");closeFileBrowser();
 remixBrowser911=true;openSaveMenu();handleScreenButtonPress(SCREEN_BUTTON_BITS[0]);ck(!remixBrowser911&&fileBrowserMode==FILE_BROWSER_SAVE,"Full_Save_not_Load");closeFileBrowser();
 ck(SD.remove(path),"remove_own_synthetic_fixture");
 const char*longPath42="/URBANKAOZ/PLink/TEST42.UKL";
 if(!SD.exists(longPath42)){
  Item809 fixture42,read42;fixture42.kind=ITEM_LFO809;fixture42.curve.enabled=1;fixture42.curve.rate=LFO_MAX42;fixture42.curve.curve=2;fixture42.curve.sustain=80;fixture42.rat=1;strcpy(fixture42.name07,"LONG");
  File f42=SD.open(longPath42,FILE_WRITE_BEGIN);bool ok42=f42&&itemWrite809(f42,fixture42);f42.close();f42=SD.open(longPath42,FILE_READ);ok42=ok42&&f42&&itemRead809(f42,read42,ITEM_LFO809)&&read42.curve.rate==LFO_MAX42;f42.close();itemFree809(read42);ck(ok42,"long_LFO_preset_SD_roundtrip");ck(SD.remove(longPath42),"remove_own_LFO_fixture");
 }else ck(false,"existing_LFO_fixture_untouched");
 remixMode=false;remixLeave911();benchPrepare29(1,4,4,1,120,0);
 Serial.printf("CHECK42,SUMMARY,PASS=%u,FAIL=%u\n",pass,fail);
}
