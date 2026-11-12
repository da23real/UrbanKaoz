#pragma once
// Dedicated view; does not change EP, QV, note selection or transport.
FLASHMEM void toggleDelayMixer930(){
 if(debugTestMode!=DEBUG_TEST_NONE||itemKind809||itemChoosing809||numericEntryActive)return;
 bool nextDelay=!delayMixer930;leavePadModesA01(false);delayMixer930=nextDelay;
 if(delayMixer930){trackChord815=false;recordClose820();}
 neoDisplayDirty=displayValueDirty=true;lastNeoRefreshMs=millis()-NEO_MIN_REFRESH_MS;
 DISPLAY_SERIAL.println(delayMixer930?"MSG|DELAY MIXER":"MSG|DELAY MIXER OFF");
}
FLASHMEM bool delayMixerPad930(uint8_t n,uint8_t pad,uint8_t edge){
 uint16_t bit=1u<<pad;
 if(delayPadHeld930[n]&bit){if(edge==SEESAW_KEYPAD_EDGE_FALLING){delayPadHeld930[n]&=~bit;neoPadDown[n][pad]=neoPadEdited[n][pad]=false;}return true;}
 if(!delayMixer930||debugTestMode!=DEBUG_TEST_NONE||edge!=SEESAW_KEYPAD_EDGE_RISING)return false;
 delayPadHeld930[n]|=bit;delayToggle925(n*4+pad%4,pad/4);
 neoDisplayDirty=displayValueDirty=delayMenuDirty817=modDisplayDirty810=true;
 if(modOwnerTrack805==n*4+pad%4&&(modUi805==MOD_FX806||(modUi805==MOD_DELAY806&&delaySlot924==pad/4)))mod805Menu();
 return true;
}
FLASHMEM bool delayMixerRender930(){
 if(!delayMixer930||debugTestMode!=DEBUG_TEST_NONE)return false;
 for(uint8_t n=0;n<NEO_COUNT;++n){if(!(neoAvailable[n]||pc831.active))continue;
  for(uint8_t pad=0;pad<16;++pad){uint8_t t=n*4+pad%4,s=pad/4;uint16_t key=t*16+s*256;
   bool enabled=delayMask925[t]&(1u<<s),worked=delayWorkedMask930[t]&(1u<<s);
   bool editing=(modUi805==MOD_DELAY806&&modOwnerTrack805==t&&delaySlot924==s)||(quickView903==3&&quickItem903==key);
   uint32_t color=editing?colorFor(n,0,enabled?85:20,enabled?85:20):mixerColor932(n,worked?(enabled?3:2):(enabled?1:0),true);
   neoPixel821(n,pad,color);
  }diagShowNeo814(n);serviceRealtimeBetweenNeoPanels();
 }return true;
}
FLASHMEM bool mixerTrackNotes930(uint8_t t){
 uint16_t bit=1u<<t;if(mixerValid930&bit)return mixerNotes930&bit;
 bool notes=false;uint16_t count=trackStepCount(t);
 for(uint16_t s=0;s<count;++s)if(stepActive[t][s]){notes=true;break;}
 if(!notes)for(const auto&e:fineNote[t])if(e.active&&e.step<count){notes=true;break;}
 if(notes)mixerNotes930|=bit;else mixerNotes930&=~bit;mixerValid930|=bit;return notes;
}
FLASHMEM uint32_t mixerMuteColor930(uint8_t n,uint8_t t){
 bool notes=mixerTrackNotes930(t),muted=trackMuted[t];
 return mixerColor932(n,notes?(muted?2:3):(muted?0:1),false);
}
FLASHMEM void keyboardOctave930(int direction){
 recordBase820=clampInt((int)recordBase820+direction*12,0,60);
 neoDisplayDirty=true;char msg[32];snprintf(msg,sizeof(msg),"MSG|REC NOTES %u-%u",recordBase820,recordBase820+63);DISPLAY_SERIAL.println(msg);
}
FLASHMEM bool keyboardOctavePad930(uint8_t n,uint8_t pad,uint8_t edge){
 uint16_t bit=1u<<pad;
 if(octavePadHeld930[n]&bit){if(edge==SEESAW_KEYPAD_EDGE_FALLING){octavePadHeld930[n]&=~bit;neoPadDown[n][pad]=false;}return true;}
 if(!recordContext820()||!((n==0&&pad==12)||(n==3&&pad==15)))return false;
 if(edge==SEESAW_KEYPAD_EDGE_RISING){octavePadHeld930[n]|=bit;keyboardOctave930(n==0?-1:1);}return true;
}
