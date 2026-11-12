#pragma once
struct StabilityRecord904 {uint32_t magic;uint8_t track,quick,transport,check;};
FLASHMEM bool validStability904(const StabilityRecord904&s){return s.magic==0x9045354u&&s.track<2&&s.quick<2&&s.transport<2&&s.check==(s.track^(s.quick<<1)^(s.transport<<2)^0x94);}
FLASHMEM void saveStability904(){pidiStateDirty931=true;StabilityRecord904 s{0x9045354u,(uint8_t)stableTrack904,(uint8_t)stableQuick904,(uint8_t)stableTransport904,(uint8_t)(stableTrack904^(stableQuick904<<1)^(stableTransport904<<2)^0x94)};EEPROM.put(3060,s);}
FLASHMEM void loadStability904(){uint32_t v=0;EEPROM.get(3080,v);if((v&~1u)==0x9060000)numericEnabled906=v&1;StabilityRecord904 s{};EEPROM.get(3060,s);if(validStability904(s)){stableTrack904=s.track;stableQuick904=s.quick;stableTransport904=s.transport;}}
uint64_t readButtons904(){uint64_t raw=read74HC165();return normalize903(raw);}
FLASHMEM void statusStability904(){Serial.printf("STABLE904,TRACK=%u,QUICK=%u,TRANSPORT=%u,NOTES=%s,DELAY=%s,SCAN=%u\n",stableTrack904,stableQuick904,stableTransport904,cutNotes903?"CUT":"KEEP",cutDelay903?"CUT":"KEEP",56);}
FLASHMEM void renderStability904(){
 char top[21],bottom[21],value[21];const char* labels[8]={};const char*title="SEQ > STABLE";const char*description="Choose a function";
 if(stabilityPage904==1){labels[0]="TRACK";}
 else if(stabilityPage904==2||stabilityPage904==3||stabilityPage904==5){
  bool on=stabilityPage904==2?stableTrack904:stabilityPage904==5?numericEnabled906:stableQuick904;
  labels[0]="OFF";labels[1]="ON";title=stabilityPage904==2?"STABLE > TRACK":stabilityPage904==5?"NUMERIC VALUE":"QUICK ACCESS";
  snprintf(value,sizeof(value),"%s",on?"ON":"OFF");
  description=stabilityPage904==2?"Track parameters keep playing":stabilityPage904==5?"Numeric entry of current value":"Quick value access on E1-E4";
 }else{title="TRANSPORT > SPECIAL";labels[0]="OFF";labels[1]="ON";labels[4]="NOTE";labels[5]="DELY";
  snprintf(value,sizeof(value),"%s N:%s D:%s",stableTransport904?"ON":"OFF",cutNotes903?"CUT":"KEEP",cutDelay903?"CUT":"KEEP");description="Pause / previous / next measure";}
 makeEightButtonLines(top,bottom,labels);
 if(stabilityPage904==1)sendMenuScreen(title,description,top,bottom);
 else {char heading[48];snprintf(heading,sizeof(heading),"%s %s",title,stabilityPage904==2?(stableTrack904?"ON":"OFF"):stabilityPage904==3?(stableQuick904?"ON":"OFF"):stabilityPage904==5?(numericEnabled906?"ON":"OFF"):"");sendMenuScreen(heading,stabilityPage904==4?value:description,top,bottom);}
 settingsPageRetryPending=false;
}
FLASHMEM void openStability904(uint8_t page){stabilityPage904=page;settingsMenuLevel=SETTINGS_STABILITY904;renderStability904();}
FLASHMEM void buttonStability904(uint8_t button){
 if(stabilityPage904==1){if(button==0)openStability904(2);return;}
 if(button<2){bool on=button==1;
  if(stabilityPage904==2)stableTrack904=on;
  else if(stabilityPage904==5){numericEnabled906=on;numericEntryActive=false;numericEntryLen=0;uint32_t v=on?0x9060001:0x9060000;EEPROM.put(3080,v);displayValueDirty=true;}
  else if(stabilityPage904==3){stableQuick904=on;quickView903=0;quickWorking903=false;memset(modPreviousLcd806,0,sizeof(modPreviousLcd806));displayValueDirty=true;}
  else {stableTransport904=on;softTransport903=false;keepDelayPaused903=false;suppressCancel903=false;pidiCancel827();pidiStateValid827=false;}
  saveStability904();
 }else if(stabilityPage904==4&&(button==4||button==5)){toggleTransport903(button==4?7:6);}
 renderStability904();statusStability904();
}
FLASHMEM void backStability904(){
 if(stabilityPage904==2){openStability904(1);return;}
 if(stabilityPage904==3||stabilityPage904==5){settingsMenuLevel=SETTINGS_CATEGORIES;settingsCategoryPage=1;}
 else if(stabilityPage904==4)settingsMenuLevel=SETTINGS_TRANSPORT_ITEMS;
 else {settingsMenuLevel=SETTINGS_ITEMS;settingsPage=5;}
 stabilityPage904=0;sendSettingsPage();
}
FLASHMEM void checks904(){
 if(playing||settingsOpen||modUi805||patchUi808||trackMenuActive){Serial.println("CHECK904,BUSY_RETURN_HOME_STOPPED");return;}
 unsigned pass=0,fail=0;auto check=[&](bool ok,const char*name){ok?++pass:++fail;Serial.printf("CHECK904,%s,%s\n",ok?"PASS":"FAIL",name);};
 bool t=stableTrack904,q=stableQuick904,r=stableTransport904;uint8_t view=quickView903;
 stableQuick904=false;check(!quickButton903(12)&&!quickButton903(13),"OFF_preserves_save_load");check(quickButton903(48)&&quickView903==view,"OFF_ignores_new_quick_button");check(!quickDisplay903(),"OFF_preserves_legacy_display");
 stableQuick904=true;check(!quickButton903(12)&&!quickButton903(13),"ON_preserves_save_load");
 stableTransport904=false;{TransportScope903 a(1);check(!softTransport903,"OFF_pause_legacy");}
 stableTransport904=true;{TransportScope903 a(1);check(softTransport903,"ON_pause_special");{TransportScope903 b(2);check(!softTransport903,"ON_stop_always_hard");}{TransportScope903 b(0);check(!softTransport903,"ON_return_always_hard");}}
 check(menuPageItem(0,7,9)==7&&menuPageItem(1,0,9)==8&&menuPageItem(1,1,9)==-1,"SEQ_arrows_and_second_page");
 StabilityRecord904 s{0x9045354u,0,0,0,0x94};check(validStability904(s),"OFF_record_valid");s.quick=2;check(!validStability904(s),"reject_invalid_record");
 stableTrack904=t;stableQuick904=q;stableTransport904=r;quickView903=view;
 Serial.printf("CHECK904,SUMMARY,PASS=%u,FAIL=%u\n",pass,fail);
}
