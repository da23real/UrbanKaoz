#pragma once
bool keyboardEnabled823=false;
uint16_t keyboardSwallow823[4]={};
// Follow emitted MIDI, including held keyboard notes. Drum mode has no Note Off,
// so its visual gate gets a separate bounded timer without changing MIDI behavior.
DMAMEM uint8_t keyboardLights823[16][16];
struct DrumLight823 { uint32_t until; uint8_t channel,note; bool active; };
DMAMEM DrumLight823 drumLights823[64];
uint32_t trackPulseUntil823[16]={};
uint16_t trackPulseMask823=0;
bool keyboardContext823(){return !remixMode&&keyboardEnabled823&&debugTestMode==DEBUG_TEST_NONE&&!itemKind809&&!itemChoosing809&&!numericEntryActive;}
bool recordArmed823(){if(remixMode)return false;return songEngineActive?songRecord:recordingArmed;}
bool recordSuspended823(){return selectButtonActive||shiftButtonActive||columnSelectionMode||selectionMode;}
bool recordCapture823(){return recordArmed823()&&!settingsOpen;}
uint8_t keyboardTrack823(){return songEngineActive?songSelectedTrack:selectedTrack;}
bool keyboardLit823(uint8_t channel,uint8_t note){return channel<16&&note<128&&(keyboardLights823[channel][note/8]&(1u<<(note%8)));}
void keyboardLight823(uint8_t channel,uint8_t note,bool on){
  if(channel>=16||note>=128)return;
  bool before=keyboardLit823(channel,note);uint8_t &bits=keyboardLights823[channel][note/8];
  if(on)bits|=1u<<(note%8);else bits&=~(1u<<(note%8));
  if(before!=on&&keyboardEnabled823)neoDisplayDirty=true;
}
void keyboardMidi823(uint8_t channel,uint8_t note,bool on){
  channel&=15;
  for(auto &d:drumLights823)if(d.active&&d.channel==channel&&d.note==note)d.active=false;
  keyboardLight823(channel,note,on);
  if(on){uint32_t until=millis()+90;
    if(midiTrack940<16){trackPulseUntil823[midiTrack940]=until;trackPulseMask823|=1u<<midiTrack940;}
    if(trackChord815)neoDisplayDirty=true;
  }
}
void keyboardDrum823(uint8_t channel,uint8_t note,uint32_t gateUs){
  int slot=-1;for(uint8_t i=0;i<64;++i)if(!drumLights823[i].active){slot=i;break;}
  if(slot<0){slot=0;for(uint8_t i=1;i<64;++i)if((int32_t)(drumLights823[i].until-drumLights823[slot].until)<0)slot=i;
    keyboardLight823(drumLights823[slot].channel,drumLights823[slot].note,false);}
  drumLights823[slot]={micros()+min(gateUs,0x7FFFFFFFUL),channel,note,true};
}
void keyboardLightsClear823(){
  memset(keyboardLights823,0,sizeof(keyboardLights823));memset(drumLights823,0,sizeof(drumLights823));
  trackPulseMask823=0;neoDisplayDirty=true;
}
bool keyboardTrackPulse823(uint8_t track){return track<16&&(trackPulseMask823&(1u<<track));}
FLASHMEM void keyboardToggle823(){
  bool nextKeyboard=!keyboardEnabled823;bool armed54=recordingArmed,song54=songRecord;recordingArmed=songRecord=false;leavePadModesA01(nextKeyboard);recordingArmed=armed54;songRecord=song54;selectionIndicatorsPending931=true;
  for(uint8_t n=0;n<4;++n)for(uint8_t p=0;p<16;++p)
    if(neoPadDown[n][p]&&!trackPads815[n][p])keyboardSwallow823[n]|=1u<<p;
  keyboardEnabled823=nextKeyboard;
  kbZoneViewR11=nextKeyboard;
  if(nextKeyboard){quickView903=0;patchBank808=false;memset(modPreviousLcd806,0,sizeof(modPreviousLcd806));}
  if(keyboardEnabled823)trackChord815=false;
  neoDisplayDirty=displayValueDirty=true;
  if(nextKeyboard)kbDisplayR10();
}
void keyboardService823(){
  // Closing an in-flight take on Select preserves it; held pads never restart a take.
  if(!recordCapture823())recordSuspend823();
  uint32_t now=micros();
  for(auto &d:drumLights823)if(d.active&&(int32_t)(now-d.until)>=0){d.active=false;keyboardLight823(d.channel,d.note,false);}
  now=millis();
  for(uint8_t t=0;t<16;++t)if((trackPulseMask823&(1u<<t))&&(int32_t)(now-trackPulseUntil823[t])>=0){trackPulseMask823&=~(1u<<t);if(trackChord815)neoDisplayDirty=true;}
}
