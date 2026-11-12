#pragma once
FLASHMEM void toggleTrackView905(){
 if(debugTestMode!=DEBUG_TEST_NONE)return;
 bool nextMixer=modMixer939||!trackChord815;leavePadModesA01(false);if(patternMode){patternMode=false;grooveEditMode=false;clearPatternSelection();sendSelectionIndicatorsToPicran();}
 delayMixer930=false;mixerValid930=0;if(modMixer939){modMixer939=false;trackChord815=true;}else trackChord815=nextMixer;
 neoDisplayDirty=displayValueDirty=true;lastNeoRefreshMs=millis()-NEO_MIN_REFRESH_MS;
 DISPLAY_SERIAL.print("VOL818,");DISPLAY_SERIAL.print(trackVolume818[selectedTrack]);DISPLAY_SERIAL.print(',');DISPLAY_SERIAL.println(trackChord815);
}
// Deliberately bounded to a two-digit suffix. 99 requires a different name.
FLASHMEM bool incrementName905(char*name,size_t capacity){
 size_t n=strlen(name),base=n;unsigned value=0;
 if(n>=2&&isdigit(name[n-1])&&isdigit(name[n-2])){base=n-2;value=(name[n-2]-'0')*10+name[n-1]-'0';}
 if(value>=99||base+3>capacity)return false;
 snprintf(name+base,capacity-base,"%02u",value+1);return true;
}
FLASHMEM void renderSaveCollision905(){
 sendMenuScreen("FILE ALREADY EXISTS",fileBrowserName,"Overwrite Cancel    ","Increment           ");
}
FLASHMEM bool prepareSave905(){
 saveWork905[0]=0;
 if(sdBusy905()||!storageWriteAvailable||fileBrowserMode!=FILE_BROWSER_SAVE||!storageNameValid(fileBrowserName)||!fileBrowserName[0]){DISPLAY_SERIAL.println("MSG|SAVE UNAVAILABLE");return false;}
 if(snprintf(saveTarget905,sizeof(saveTarget905),"%s/%s.%s",fileBrowserPath,fileBrowserName,fileBrowserExtension)>=(int)sizeof(saveTarget905)){DISPLAY_SERIAL.println("MSG|PATH TOO LONG");return false;}
 if(SD.exists(saveTarget905)&&!saveOverwrite905){saveCollision905=true;renderSaveCollision905();return false;}
 // A new temporary file protects an existing target until writing has succeeded.
 for(unsigned i=0;i<100;++i){
  if(snprintf(saveWork905,sizeof(saveWork905),"%s/UK905%02u.TMP",fileBrowserPath,i)>=(int)sizeof(saveWork905))break;
  if(!SD.exists(saveWork905)){saveOverwrite905=false;return true;}
 }
 saveWork905[0]=0;saveOverwrite905=false;DISPLAY_SERIAL.println("MSG|NO TEMP FILE AVAILABLE");return false;
}
FLASHMEM bool finishSave905(bool ok,char*path,size_t size){
 if(!saveWork905[0])return false;
 char backup[80]={};bool displaced=false;
 if(ok&&SD.exists(saveTarget905)){
  for(unsigned i=0;i<100;++i){snprintf(backup,sizeof(backup),"%s/UK905%02u.BAK",fileBrowserPath,i);if(!SD.exists(backup))break;}
  ok=!SD.exists(backup)&&SD.sdfs.rename(saveTarget905,backup);displaced=ok;
 }
 if(ok)ok=SD.sdfs.rename(saveWork905,saveTarget905);
 if(!ok){SD.remove(saveWork905);if(displaced)SD.sdfs.rename(backup,saveTarget905);}
 else {if(displaced)SD.remove(backup);copyField(path,size,saveTarget905);rememberFileName816(path);}
 saveWork905[0]=0;saveOverwrite905=false;return ok;
}
FLASHMEM void saveCollisionButton905(uint8_t index){
 if(index==2||index==3){saveCollision905=false;renderFileBrowser();return;}
 if(index==0||index==1){saveCollision905=false;saveOverwrite905=true;executeBrowserSave();return;}
 if(index!=4&&index!=5)return;
 char candidate[sizeof(fileBrowserName)];strcpy(candidate,fileBrowserName);
 char path[80];
 do{
  if(!incrementName905(candidate,sizeof(candidate))){DISPLAY_SERIAL.println("MSG|RENAME: SUFFIX 99 OR NAME TOO LONG");return;}
  snprintf(path,sizeof(path),"%s/%s.%s",fileBrowserPath,candidate,fileBrowserExtension);
 }while(SD.exists(path));
 strcpy(fileBrowserName,candidate);saveCollision905=false;executeBrowserSave();
}
