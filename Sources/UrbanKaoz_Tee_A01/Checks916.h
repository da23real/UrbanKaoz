#pragma once
FLASHMEM void checks916(){
 if(playing||remixMode||remixDraft911.notes.size||remixHistory911.count||menuNavigationOnly()){Serial.println("CHECK916,BUSY");return;}
 unsigned pass=0,fail=0;auto check=[&](bool ok,const char*name){Serial.printf("CHECK916,%s,%s\n",ok?"PASS":"FAIL",name);ok?++pass:++fail;};
 uint8_t colors[4];memcpy(colors,modeColors902,4);
 // Reproduce the overlay that previously swallowed J1 before Color could return.
 remixMode=true;remixReady911=true;showParaScreen();settingsPage=5;settingsMenuLevel=SETTINGS_COLOR_ITEMS;
 p23NavigateMenu(0,0,true);check(settingsMenuLevel==SETTINGS_ITEMS&&settingsPage==5,"J1_Color_over_Remix");
 settingsMenuLevel=SETTINGS_COLOR_ITEMS;openColorSettingByBe(8);modeColors902[0]=(colors[0]+1)%NEO_PALETTE_COUNT;
 p23NavigateMenu(0,0,true);check(settingsMenuLevel==SETTINGS_COLOR_ITEMS&&!memcmp(colors,modeColors902,4),"J1_Mixer_cancel_over_Remix");
 p23NavigateMenu(0,0,true);p23NavigateMenu(0,0,true);check(settingsMenuLevel==SETTINGS_CATEGORIES,"J1_categories_over_Remix");
 showGameScreen();
 uint8_t oldLen=remixLen916,oldHit=remixHit916,oldQuantum=remixQuantum916,oldChannels=remixChannels916,oldLayout=remixLayout913,oldQuan=remixQuan913;
 uint16_t oldLength=remixLength911,oldFixed=remixFixed916;int oldLat=remixLatency911;bool oldHold=remixHold911,oldFine=remixFine911,oldChain=remixChain911;
 remixLen916=rmx916::Fixed;remixFixed916=30;remixQuantum916=1;remixKy911(6,1);check(remixFixed916==192,"Fix_croche_exact");
 remixKeys911=0;remixKey911(6,true);remixKey911(6,false);check(remixQuantum916==2,"Fix_toggle_noire");
 remixFixed916=30;remixKy911(6,1);check(remixFixed916==384,"Fix_noire_exact");
 remixKy911(5,-1);check(remixLen916==rmx916::Pad&&remixLength911==96,"Len_PAD_96");
 remixKy911(6,1);check(remixFixed916==384,"Fix_ignored_outside_FIX");
 remixKy911(4,1);check(remixLatency911==oldLat+1,"K5_LAT");
 remixKy911(8,1);check(remixQuan913==min(4,(int)oldQuan+1),"K9_QUAN");
 remixKy911(13,1);check(remixChannels916==(oldChannels+1)%4&&remixLayout913==oldLayout,"ChVw_independent_Trix");
 uint16_t hear=remixHearBar911;remixHearBar911=0;remixAudition911.active=true;remixAudition911.tracks=1;
 remixHit916=rmx916::PadR;check(remixTrack911(0,0)&&!remixTrack911(1,0),"PadR_only_audition_channels");
 remixHit916=rmx916::PadO;check(!remixTrack911(0,0),"PadO_keeps_original_reader");
 remixAudition911.active=false;remixHearBar911=hear;
 uint64_t tick=transportTickAbsolute;uint16_t current=currentStep,measure=measureTick;uint16_t oldSource=remixSourceBar911;
 auto oldClipboardType=clipboardSlots[3].type;auto*oldClipboard=clipboardSlots[3].remix;bool oldClipboardValid=clipboardSlots[3].valid;
 remixHit916=rmx916::Chain;remixChain911=true;remixLayout913=0;remixSourceBar911=0;
 remixPad(0,3,SEESAW_KEYPAD_EDGE_RISING);remixPad(0,3,SEESAW_KEYPAD_EDGE_FALLING);
 check(transportTickAbsolute==288&&remixChainRole916==0&&!remixAudition911.active,"CHAIN_SRC_position_only");
 remixPad(1,7,SEESAW_KEYPAD_EDGE_RISING);remixPad(1,7,SEESAW_KEYPAD_EDGE_FALLING);
 check(transportTickAbsolute==672&&remixChainRole916==1&&!remixAudition911.active,"CHAIN_RMX_position_only");
 check(!remixDraft911.notes.size&&clipboardSlots[3].type==oldClipboardType&&clipboardSlots[3].remix==oldClipboard&&clipboardSlots[3].valid==oldClipboardValid,"CHAIN_no_copy_no_paste");
 transportTickAbsolute=tick;currentStep=current;measureTick=measure;remixSourceBar911=oldSource;remixChainRole916=-1;evaluateCurrentTickOnNextClock=false;
 remixLen916=oldLen;remixHit916=oldHit;remixQuantum916=oldQuantum;remixChannels916=oldChannels;remixLayout913=oldLayout;remixQuan913=oldQuan;
 remixLength911=oldLength;remixFixed916=oldFixed;remixLatency911=oldLat;remixHold911=oldHold;remixFine911=oldFine;remixChain911=oldChain;
 remixMode=remixReady911=false;showGameScreen();remixDisplay911(true);neoDisplayDirty=true;
 Serial.printf("CHECK916,RESULT,PASS=%u,FAIL=%u\n",pass,fail);
}
