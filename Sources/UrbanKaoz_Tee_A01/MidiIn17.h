#pragma once
// Route ownership is retained from attack to release, including after UI edits.
struct MidiInVoice17 {uint8_t track,channel,note,outChannel,mask;uint16_t held;};
EXTMEM MidiInVoice17 midiInVoices17[256]={};
uint32_t midiInReceived17=0,midiInSent17=0,midiInRejected17=0;
FLASHMEM void midiInInit17(){memset(midiInVoices17,0,sizeof(midiInVoices17));}
FLASHMEM bool midiInTrackHeld17(uint8_t track){for(const auto&v:midiInVoices17)if(v.held&&v.track==track)return true;return false;}
FLASHMEM void midiInStop17(MidiInVoice17&v){
 if(!v.held)return;
 uint8_t track=v.track;v.held=0;
 // Multiple input channels/tracks may own the same output address.
 uint8_t mask=v.mask;
 for(const auto&other:midiInVoices17)if(other.held&&other.note==v.note&&other.outChannel==v.outChannel)mask&=~other.mask;
 if(mask){MidiScope902 route(mask,track);noteOffNow(v.outChannel,v.note);}
 if(!midiInTrackHeld17(track))manualModStop06(track,254);
}
FLASHMEM void midiInRelease17(uint8_t track,uint8_t channel){
 for(auto&v:midiInVoices17)if(v.held&&(track==255||v.track==track)&&(channel==255||v.channel==channel))midiInStop17(v);
 // USB disconnection / channel All Notes Off also releases channel-based mods.
 // A local track reassignment must not release the source for other listeners.
 if(track==255)for(uint8_t c=0;c<16;++c)if(channel==255||channel==c)mod806UsbNoteOff(c+1,modMidiNote806[c],0);
}
FLASHMEM void midiInOff17(uint8_t channel,uint8_t note,uint8_t velocity){MidiInputScope53 scope53(1);
 if(channel<1||channel>16||note>127)return;
 mod806UsbNoteOff(channel,note,velocity);
 for(auto&v:midiInVoices17)if(v.held&&v.channel==channel-1&&v.note==note){if(v.held>1)--v.held;else midiInStop17(v);}
}
FLASHMEM void midiInOn17(uint8_t channel,uint8_t note,uint8_t velocity){MidiInputScope53 scope53(1);
 if(channel<1||channel>16||note>127||velocity>127)return;
 if(!velocity){midiInOff17(channel,note,0);return;}
 if(controller940||fullBusy910)return;
 ++midiInReceived17;mod806UsbNoteOn(channel,note,velocity);
 for(uint8_t t=0;t<16;++t){
  if(midiInChannel17[t]!=channel||trackSilent818(t))continue;
  uint8_t mask=outMask902(t),ch=midiChannelForTrack(t);if(!mask)continue;
  MidiInVoice17*slot=nullptr;MidiInVoice17*freeSlot=nullptr;
  for(auto&v:midiInVoices17){if(!v.held){if(!freeSlot)freeSlot=&v;}else if(v.track==t&&v.channel==channel-1&&v.note==note){slot=&v;break;}}
  uint16_t depth=1;
  if(slot){if(slot->held==UINT16_MAX){++midiInRejected17;continue;}depth=slot->held+1;midiInStop17(*slot);}
  else slot=freeSlot;
  if(!slot){++midiInRejected17;continue;}
  MidiScope902 route(mask,t);
  uint32_t gate=max(1000UL,(uint32_t)(defaultGateTicks[t]*tickDurationMs()*1000.0f));
  manualModStart06(t,254,gate);
  int bend=trackPitchBend[t];mod806ChannelBend(ch,songEngineActive?songTick:transportTickAbsolute,bend);sendPitchBendNow(ch,bend);
  if(!noteOnNow(ch,note,velocity)){manualModStop06(t,254);++midiInRejected17;continue;}
  *slot={t,(uint8_t)(channel-1),note,ch,mask,depth};++midiInSent17;
  // Like the live keyboard: fixed initial gate for echoes; dry note follows USB release.
  // No roll scheduler and no remix capture/replacement are entered here.
  manualDelay1000(t,note,velocity,gate);
 }
}
FLASHMEM void midiInService17(){for(auto&v:midiInVoices17)if(v.held&&(controller940||trackSilent818(v.track)))midiInStop17(v);}
FLASHMEM void midiInRender17(){
 char title[21],value[40];snprintf(title,sizeof(title),"TRACK %02u MIDI",selectedTrack+1);
 const char*labels[8]={"IN",nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr};
 if(midiInMenu17==2){snprintf(title,sizeof(title),"TRACK %02u MIDI IN",selectedTrack+1);uint8_t ch=midiInChannel17[selectedTrack];
  if(ch)snprintf(value,sizeof(value),"USB CH%02u  OA:channel",ch);else strcpy(value,"USB OFF   OA:channel");
  labels[0]="USB";labels[1]=nullptr;
 }else strcpy(value,"USB input");
 char top[21],bottom[21];makeEightButtonLines(top,bottom,labels);sendMenuScreen(title,value,top,bottom);
}
FLASHMEM void midiInButton17(uint8_t index){
 if(midiInMenu17==1){if(index==0){midiInMenu17=2;midiInRender17();}}
}
FLASHMEM void midiInRoy17(uint8_t roy,int32_t delta){
 if(midiInMenu17!=2||roy!=0||!delta)return;
 uint8_t ch=clampInt((int)midiInChannel17[selectedTrack]+delta,0,16);
 if(ch==midiInChannel17[selectedTrack])return;
 midiInRelease17(selectedTrack);midiInChannel17[selectedTrack]=ch;storageDirty931=true;midiInRender17();
}
FLASHMEM bool midiInWrite17(File&f){return f.write(midiInChannel17,16)==16;}
FLASHMEM bool midiInRead17(File&f,bool present){
 memset(midiInStage17,0,16);if(!present)return true;
 if(f.read(midiInStage17,16)!=16)return false;
 for(uint8_t ch:midiInStage17)if(ch>16)return false;return true;
}
FLASHMEM void midiInStatus17(){
 unsigned held=0;for(const auto&v:midiInVoices17)held+=v.held!=0;
 Serial.printf("MIDIIN17,USB=%u,T=%u,IN=%u,MOUT=%u,CH=%u,HELD=%u,RECEIVED=%lu,SENT=%lu,REJECTED=%lu,MENU=%u\n",songUsbConnected,selectedTrack+1,midiInChannel17[selectedTrack],outMask902(selectedTrack),midiChannelForTrack(selectedTrack)+1,held,(unsigned long)midiInReceived17,(unsigned long)midiInSent17,(unsigned long)midiInRejected17,midiInMenu17);
}
