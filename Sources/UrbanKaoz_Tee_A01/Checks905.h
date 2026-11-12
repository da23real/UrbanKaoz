#pragma once
FLASHMEM void checks905(){
 if(playing||menuStopLatched||modUi805||patchUi808||trackMenuActive||sdBusy905()||selectionHasActiveNotes()||selectedStep>=0||selectButtonActive||shiftButtonActive){Serial.println("CHECK905,BUSY_RETURN_HOME_PAUSED");return;}
 unsigned pass=0,fail=0;auto check=[&](bool ok,const char*name){++(ok?pass:fail);Serial.printf("CHECK905,%s,%s\n",ok?"PASS":"FAIL",name);};
 char name[17]="Test";check(incrementName905(name,sizeof(name))&&!strcmp(name,"Test01"),"name_without_suffix");
 check(incrementName905(name,sizeof(name))&&!strcmp(name,"Test02"),"name_01_to_02");strcpy(name,"Test50");
 check(incrementName905(name,sizeof(name))&&!strcmp(name,"Test51"),"name_50_to_51");strcpy(name,"Test99");
 check(!incrementName905(name,sizeof(name))&&!strcmp(name,"Test99"),"99_requires_rename");strcpy(name,"abcdefghijklmnop");check(!incrementName905(name,sizeof(name)),"no_truncation");
 uint8_t binary[]={0,1,127,128,255};char encoded[11];uint8_t decoded[5];hex905(binary,5,encoded);
 check(decode905(encoded,decoded,5)==5&&!memcmp(binary,decoded,5),"binary_roundtrip");check(decode905("0Z",decoded,5)<0&&decode905("0",decoded,5)<0,"invalid_hex_rejected");
 check(~crc905(0xffffffffu,(const uint8_t*)"123456789",9)==0xcbf43926u,"CRC32_reference");
 char path[64];check(path905("2F555242414E4B414F5A",path,sizeof(path))&&!strcmp(path,"/URBANKAOZ"),"absolute_SD_path");
 check(!path905("2F2E2E2F78",path,sizeof(path))&&!path905("2F780079",path,sizeof(path)),"traversal_and_NUL_rejected");
 bool oldQuick=stableQuick904,oldTransport=stableTransport904,oldChord=trackChord815;
 TransportSettings903 oldPersisted;EEPROM.get(3040,oldPersisted);
 bool oldNotes=cutNotes903,oldDelay=cutDelay903,oldSong=songMode,oldPattern=patternMode,oldRemix=remixMode;
 uint8_t oldTimeMode=delayTimeMode817[0];uint8_t view=quickView903;quickView903=0;stableQuick904=false;trackChord815=false;
 dispatchStableButtonPress(52);check(trackChord815,"B52_quick_off_opens");dispatchStableButtonPress(52);check(!trackChord815,"B52_closes");
 stableQuick904=true;quickView903=3;dispatchStableButtonPress(52);check(trackChord815&&quickView903==3,"B52_with_quick_delay");
 char down[]="P1,B,0,1",up[]="P1,B,0,0";handlePicoLine(1,down);handlePicoLine(1,up);check(trackChord815,"K1_no_grid_toggle");
 stableTransport904=false;handleTransportButtonEdge(6,true);handleTransportButtonEdge(7,true);
 check(songMode==oldSong&&patternMode==oldPattern&&remixMode==oldRemix&&cutNotes903==oldNotes&&cutDelay903==oldDelay,"B30_B31_OFF_no_legacy_actions");
 stableTransport904=true;handleTransportButtonEdge(6,true);check(cutDelay903!=oldDelay&&cutNotes903==oldNotes,"B31_delay_only");handleTransportButtonEdge(7,true);check(cutNotes903!=oldNotes,"B30_notes");
 auto old=delays805[modOwnerTrack805][modOwnerPattern805];auto savedDelay=modSavedDelay806;auto dirty=modDirty806;auto epoch=modEpoch805;
 delays805[modOwnerTrack805][modOwnerPattern805].repeats=1;delayEdit817(2,-1);check(delays805[modOwnerTrack805][modOwnerPattern805].repeats==0,"delay_editor_reaches_zero");delayEdit817(2,-1);check(delays805[modOwnerTrack805][modOwnerPattern805].repeats==0,"delay_lower_bound_zero");
 delays805[modOwnerTrack805][modOwnerPattern805]=old;modSavedDelay806=savedDelay;modDirty806=dirty;modEpoch805=epoch;
 stableQuick904=oldQuick;stableTransport904=oldTransport;trackChord815=oldChord;cutNotes903=oldNotes;cutDelay903=oldDelay;quickView903=view;
 EEPROM.put(3040,oldPersisted);delayTimeMode817[0]=oldTimeMode;pidiStateValid827=false;neoDisplayDirty=displayValueDirty=true;
 Serial.printf("CHECK905,SUMMARY,PASS=%u,FAIL=%u\n",pass,fail);
}
