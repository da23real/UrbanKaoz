#pragma once
#include "Snkoz935.h"
struct Prefs933 {uint32_t magic;uint16_t mute,ccMask;int16_t cc[16];uint16_t crc;};
static_assert(3600+sizeof(Prefs933)<4284,"EEPROM933 capacity");
FLASHMEM void savePrefs933(){Prefs933 p{};p.magic=0x933CC01;p.mute=delayMute933;p.ccMask=trackCcMask933;memcpy(p.cc,trackCc933,sizeof(p.cc));p.crc=pidi827::crc((uint8_t*)&p,offsetof(Prefs933,crc));EEPROM.put(3600,p);}
FLASHMEM void loadPrefs933(){Prefs933 p{};EEPROM.get(3600,p);if(p.magic!=0x933CC01||p.crc!=pidi827::crc((uint8_t*)&p,offsetof(Prefs933,crc)))return;for(auto x:p.cc)if(x< -96||x>96)return;delayMute933=p.mute;trackCcMask933=p.ccMask;memcpy(trackCc933,p.cc,sizeof(p.cc));}
FLASHMEM void delayEntry933(){delayIncrementShown925=0;memset(delayToggle907,0,sizeof(delayToggle907));delayToggle907[1]=1;}
FLASHMEM void trackRender933(){if(trackPage933>=5){fxRender32();return;}if(trackPage933==4){programMenu939();return;}if(trackPage933==3){mixerRender938();return;}char a[21],b[21];snprintf(a,21,"TRACK %02u %s",selectedTrack+1,trackPage933==1?"CC":"DELAY MUTE");if(trackPage933==1){snprintf(b,21,"%+d ticks %s",trackDraft933,trackCcMask933&(1u<<selectedTrack)?"Track":"Global");sendMenuScreen(a,b,"validate to hear","OA OK     Global");}else{snprintf(b,21,"Cut on mute: %s",delayMute933&(1u<<selectedTrack)?"On":"Off");sendMenuScreen(a,b,"Off: delays continue","MUTE      Back");}}
FLASHMEM void trackOpen933(uint8_t page){trackPage933=page;trackDraft933=trackCcMask933&(1u<<selectedTrack)?trackCc933[selectedTrack]:midiAdvance808;trackRender933();}
FLASHMEM void trackRoy933(uint8_t roy,int delta,bool click){if(trackPage933>=5){fxTurn32(roy,delta,click);return;}if(trackPage933>=5){fxRender32();return;}if(trackPage933==4){if(!roy&&delta){if(!patternMode&&lockWantsEdit817()){lockEdit817(LOCK_PROGRAM26,selectedTrack,0,programValue939(selectedTrack,activePattern[selectedTrack]),delta,0,128);}else programEdit939(delta,false);}programMenu939();return;}if(trackPage933==3){mixerTurn938(roy,delta);return;}if(roy)return;if(trackPage933==1){trackDraft933=WRAP23(trackDraft933,delta,-96,96);if(click){trackCc933[selectedTrack]=trackDraft933;trackCcMask933|=1u<<selectedTrack;++modEpoch805;savePrefs933();}}else if(click||delta){delayMute933^=1u<<selectedTrack;pidiStateDirty931=true;savePrefs933();}trackRender933();}
FLASHMEM void trackButton933(uint8_t i){if(trackPage933>=5){fxButton32(i);return;}if(trackPage933>=5){fxRender32();return;}if(trackPage933==4){if(!patternMode&&lockWantsEdit817()){int v=lockDisplay817(LOCK_PROGRAM26,selectedTrack,0,programValue939(selectedTrack,activePattern[selectedTrack]));lockEdit817(LOCK_PROGRAM26,selectedTrack,0,programValue939(selectedTrack,activePattern[selectedTrack]),-v,0,128);programMenu939();return;}programEdit939(0,false,true);programMenu939();return;}if(trackPage933==3){sendTrackMenu();return;}if(i>=4){if(trackPage933==1){trackCcMask933&=~(1u<<selectedTrack);++modEpoch805;savePrefs933();trackOpen933(1);}else sendTrackMenu();}else trackRoy933(0,0,true);}
FLASHMEM void synkozEvent933(uint8_t type){if(type==1||type==2)ioVirtualReset53();if(!synkozEnabled933||!Serial)return;uint32_t seq=++synkozSequence933;if(synkozCount933==64){++synkozDrops933;return;}uint8_t i=(synkozHead933+synkozCount933)%64;synkozQueue933[i]={seq,micros(),synkozTick933,type};++synkozCount933;}
FLASHMEM void synkozStop933(){if(snkozNote935)snkozMidi935(false);if(snkozTest935&&synkozTaking933){playing=false;internalClockRunning=false;}neoDisplayDirty=true;if(synkozTaking933)synkozEvent933(3);if(synkozPreroll933)internalClockRunning=false;synkozTaking933=synkozPreroll933=false;}
FLASHMEM void synkozService933(){patchMetadataService37();if(!synkozEnabled933)return;if(!pc831.active){synkozStop933();synkozEnabled933=false;DISPLAY_SERIAL.println("SYNKOZ,0");return;}if(synkozTaking933&&!synkozPreroll933&&!playing)synkozStop933();for(uint8_t n=0;n<4&&synkozCount933&&Serial.availableForWrite()>=100;++n){auto&e=synkozQueue933[synkozHead933];uint8_t kind=e.type&7;int track=e.channel>=16?e.channel+1:(e.type>>3)-1;if(kind==7)Serial.printf("SK1,P,%lu,%lu,%lu,%u,%u,%lu,%d\n",(unsigned long)e.seq,(unsigned long)e.us,(unsigned long)e.tick,e.note,e.channel&15,(unsigned long)synkozDrops933,track);else if(kind==6)Serial.printf("SK1,C,%lu,%lu,%lu,%u,%u,%u,%lu,%d\n",(unsigned long)e.seq,(unsigned long)e.us,(unsigned long)e.tick,e.note,e.velocity,e.channel&15,(unsigned long)synkozDrops933,track);else if(kind>=4)Serial.printf("SK1,N,%lu,%lu,%lu,%u,%u,%u,%u,%lu,%d\n",(unsigned long)e.seq,(unsigned long)e.us,(unsigned long)e.tick,kind==4,e.note,e.velocity,e.channel&15,(unsigned long)synkozDrops933,track);else Serial.printf("SK1,E,%lu,%lu,%lu,%u,%lu\n",(unsigned long)e.seq,(unsigned long)e.us,(unsigned long)e.tick,e.type,(unsigned long)synkozDrops933);synkozHead933=(synkozHead933+1)%64;--synkozCount933;}}
FLASHMEM bool synkozClock933(){
 if(synkozEnabled933&&playing&&!synkozTaking933){snkozTest935=false;captureTaking15=false;synkozTaking933=true;synkozPreroll933=false;synkozTick933=transportTickAbsolute;synkozEvent933(1);synkozEvent933(2);}
 if(!synkozTaking933)return false;
 ++synkozTick933;
 if(synkozTick933%(PPQN/24)==0)synkozEvent933(0);
 if(snkozNote935&&synkozTick933%PPQN==PPQN/4){snkozMidi935(false);if(snkozSeriesA01)snkozDarkA01();}
 if(synkozPreroll933){
  if(synkozTick933<4UL*PPQN){if(synkozTick933%PPQN==0)snkozVisual935();return true;}
  synkozPreroll933=false;evaluateCurrentTickOnNextClock=true;playing=true;pidiStateDirty931=true;neoDisplayDirty=true;
  if(!snkozTest935)patch808Play();synkozEvent933(2);
 }
 if(snkozTest935){if(synkozTick933%PPQN==0){snkozVisual935();snkozMidi935(true);}return true;}
 return false;
}
FLASHMEM bool synkozCommand933(const char*op,unsigned count,const char*arg){
 if(!strcmp(op,"SKSERIESCFG2")&&count==3&&!synkozTaking933&&!snkozNote935){long packed;if(!pcNumber831(arg,packed)||packed<17*128||packed>=17*17*128)return false;uint8_t choice=(packed/128)%17;snkozTrackA01=choice?choice-1:selectedTrack;snkozPitchA01=packed%128;snkozChannelA01=packed/(17*128)-1;snkozMaskA01=outMask902(snkozTrackA01);snkozConfiguredA01=true;Serial.printf("SK1,SERIESCFG2,%ld,%u,%u,%u,%u\n",packed,snkozTrackA01+1,snkozChannelA01+1,snkozPitchA01,snkozMaskA01);return true;}
 if((!strcmp(op,"SKTESTCFG")||!strcmp(op,"SKSERIESCFG"))&&count==3&&!synkozTaking933&&!snkozNote935){long packed;if(!pcNumber831(arg,packed)||packed<0||packed>=17*128)return false;uint8_t choice=packed/128;snkozTrackA01=choice?choice-1:selectedTrack;snkozPitchA01=packed%128;snkozChannelA01=midiChannelForTrack(snkozTrackA01);snkozMaskA01=outMask902(snkozTrackA01);snkozConfiguredA01=true;Serial.printf("SK1,%s,%ld,%u,%u,%u,%u\n",!strcmp(op,"SKSERIESCFG")?"SERIESCFG":"TESTCFG",packed,snkozTrackA01+1,snkozChannelA01+1,snkozPitchA01,snkozMaskA01);return true;}
 if(!strcmp(op,"SKTEST")&&count==3&&!synkozTaking933&&(arg[0]=='0'||arg[0]=='1'||arg[0]=='2')&&!arg[1]){snkozSeriesA01=arg[0]=='2';snkozTest935=arg[0]!='0';return true;}
 if(!strcmp(op,"SKTIME")&&count==3){long id;if(!pcNumber831(arg,id))return false;if(Serial.availableForWrite()>=70)Serial.printf("SK1,T,%ld,%lu\n",id,(unsigned long)micros());return true;}
 if(!strcmp(op,"SKMODE")&&count==3&&(arg[0]=='0'||arg[0]=='1')&&!arg[1]){if(arg[0]=='0')synkozStop933();synkozEnabled933=arg[0]=='1';synkozHead933=synkozCount933=0;ukReset60=true;DISPLAY_SERIAL.printf("SYNKOZ,%u\n",synkozEnabled933);Serial.printf("SK1,MODE,%u\n",synkozEnabled933);if(synkozEnabled933)for(uint8_t t=0;t<16;++t)Serial.printf("SKPRG,%u,%u,%d,%u\n",t,activePattern[t],(int)programValue939(t,activePattern[t])-1,midiChannelForTrack(t));return true;}
 if(!strcmp(op,"SKSTART")&&count==2&&synkozEnabled933){if(snkozTest935&&!snkozConfiguredA01){snkozTrackA01=selectedTrack;snkozPitchA01=defaultMidiNote[selectedTrack];snkozChannelA01=midiChannelForTrack(selectedTrack);snkozMaskA01=outMask902(selectedTrack);}handleTransportButtonEdge(2,true);if(!snkozTest935){if(songEngineActive){if(!songPrepare())return false;}else transportReturnToSequenceStart();}synkozResume934=false;synkozSong934=songEngineActive;synkozAnchor934=songEngineActive?songTick:transportTickAbsolute;synkozHead933=synkozCount933=0;ukReset60=true;synkozTick933=synkozSequence933=synkozDrops933=0;captureTaking15=false;synkozTaking933=synkozPreroll933=true;patchMetadataStart37();playing=false;internalClockRunning=true;noInterrupts();pendingClockTicks=0;interrupts();synkozEvent933(1);snkozVisual935();return true;}
 if(!strcmp(op,"SKPAUSE")&&count==2&&synkozEnabled933){
  synkozMaxResume934=synkozTaking933&&!synkozPreroll933&&synkozTick933>=4UL*PPQN?synkozTick933-4UL*PPQN:0;
  playing=false;internalClockRunning=false;pidiStateDirty931=true;
  noInterrupts();pendingClockTicks=0;interrupts();allNotesOff();synkozStop933();synkozResume934=true;return true;
 }
 if(!strcmp(op,"SKRESUME")&&count==3&&synkozEnabled933&&synkozResume934){
  long kept;if(!pcNumber831(arg,kept)||kept<0||(uint32_t)kept>synkozMaxResume934||kept%(4UL*PPQN)||songEngineActive!=synkozSong934)return false;
  uint64_t next=synkozAnchor934+(uint32_t)kept;
  if(snkozTest935){} else if(songEngineActive)songSeek(songLoopPosition(next,songLoopEnabled,songStartBar,songEndBar,4UL*PPQN));
  else{transportTickAbsolute=next;measureTick=localTickForTrack(selectedTrack);currentStep=measureTick/TICKS_PER_STEP;resetGrooveBridges();invalidateStepTriggers();}
  synkozAnchor934=songEngineActive?songTick:transportTickAbsolute;synkozResume934=false;
  synkozHead933=synkozCount933=0;ukReset60=true;synkozTick933=synkozSequence933=synkozDrops933=0;
  playing=false;captureTaking15=false;synkozTaking933=synkozPreroll933=true;patchMetadataStart37();internalClockRunning=true;evaluateCurrentTickOnNextClock=true;
  noInterrupts();pendingClockTicks=0;interrupts();synkozEvent933(1);snkozVisual935();return true;
 }
 if(!strcmp(op,"SKSTOP")&&count==2){handleTransportButtonEdge(2,true);return true;}
 return false;
}

FLASHMEM void installPrefs933(){delayMute933=fullDelayMute933;trackCcMask933=fullCcMask933;memcpy(trackCc933,fullCc933,sizeof(trackCc933));pidiStateDirty931=true;}

// Notes and CC accepted into the Tee-to-Pidi MIDI queue, once per event, without ports.
FLASHMEM void synkozNote934(uint8_t status,uint8_t note,uint8_t velocity){
 if(!synkozEnabled933||!synkozTaking933||synkozPreroll933||!Serial)return;
 uint8_t kind=status&0xf0;if(kind!=0x90&&kind!=0x80&&kind!=0xB0&&kind!=0xC0)return;
 uint32_t seq=++synkozSequence933;if(synkozCount933==64){++synkozDrops933;return;}
 uint8_t i=(synkozHead933+synkozCount933)%64;
 synkozQueue933[i]={seq,micros(),synkozTick933,(uint8_t)((kind==0xC0?7:kind==0xB0?6:kind==0x90&&velocity?4:5)|((midiTrack940<16?midiTrack940+1:0)<<3)),note,velocity,(uint8_t)(status&15)};++synkozCount933;
}
static_assert(sizeof(SynkozEvent933)==16,"Reuse event padding; no per-tick allocation");
