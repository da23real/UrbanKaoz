#pragma once
// One owner per physical pad; release retains its original pitch/channel/context.
struct RecordPad820 {
  uint64_t tick, clock;
  uint8_t track, pattern, note, velocity, channel;
  uint8_t outputMask;
  bool held, sounding, capture, song;
  uint8_t kbCount=0;NoteLock817 kbLocks[4];
};
DMAMEM RecordPad820 recordPads820[64];
uint64_t recordClock820=0;
uint8_t recordBase820=36;
bool recordTest820=false;

bool recordContext820(){return !remixMode&&!settingsOpen&&debugTestMode==DEBUG_TEST_NONE&&((recordArmed823()&&!fileBrowserMode)||(keyboardContext823()&&!trackChord815&&!delayMixer930));}
FLASHMEM bool recordStore820(const RecordPad820 &p,uint32_t duration){
  if(!p.capture)return false;
  if(p.song){if(lockCount817+p.kbCount>LOCK_CAPACITY817){DISPLAY_SERIAL.println("MSG|KB P.LOCK CAPACITY");return false;}uint32_t id=zlNextId;if(!zlInsert({p.tick,0,max(1UL,duration),p.track,0,p.note,p.velocity}))return false;return kbStoreR10(p.track,254,(int32_t)id,p.note,p.kbLocks,p.kbCount);}
  if(activePattern[p.track]!=p.pattern){DISPLAY_SERIAL.println(F("MSG|REC PATTERN CHANGED"));return false;}
  uint16_t step=p.tick/96;int16_t offset=p.tick%96;
  // Both persisted micro fields are limited to +/-48. Their sum retains every tick,
  // including the final half-step of a loop, without rounding or changing cells.
  int16_t micro=min((int16_t)48,offset),time=offset-micro;
  if(step>=trackStepCount(p.track))return false;
  uint16_t gate=min((uint32_t)GATE_TICK_MAX,max(1UL,duration));
  if(!stepActive[p.track][step]){
    grooveFirstNote26(p.track,step);clearTrackStep(p.track,step);stepActive[p.track][step]=true;
    stepMidiNote[p.track][step]=p.note;stepVel[p.track][step]=p.velocity;
    stepGateTicks[p.track][step]=gate;stepLogicalMicro[p.track][step]=micro;stepMicroTick[p.track][step]=time;
    stepLockPitch[p.track][step]=stepLockVel[p.track][step]=stepLockGate[p.track][step]=stepLockTime[p.track][step]=true;
  }else{
    int16_t i=addFineNote(p.track,step,time,p.note,micro);if(i<0)return false;
    auto &e=fineNote[p.track][i];e.vel=p.velocity;e.gate=gate;e.pxMask=PX_PITCH|PX_VEL|PX_GATE|PX_TIME;
    fineLastTriggeredCycle[p.track][i]=UINT32_MAX;
  }
  kbStoreR10(p.track,p.pattern,(int32_t)step*96+micro,p.note,p.kbLocks,p.kbCount);
  mod805Invalidate();dismissSdNoticeAfterNoteCreation();neoDisplayDirty=displayValueDirty=displayBpmDirty=true;return true;
}
FLASHMEM void recordFinish820(uint8_t i){
  auto &p=recordPads820[i];
  if(p.sounding&&!recordTest820){MidiScope902 route(p.outputMask,p.track);manualModStop06(p.track,i);noteOffNow(p.channel,p.note);}
  p.sounding=false;
  if(p.capture){recordStore820(p,(uint32_t)min((uint64_t)UINT32_MAX,max((uint64_t)1,recordClock820-p.clock)));p.capture=false;}
}
FLASHMEM void recordClose820(){for(uint8_t i=0;i<64;++i)if(recordPads820[i].held)recordFinish820(i);}
FLASHMEM void recordToggle820(){
  if(remixMode){remixRecordToggle913();return;}
  recordClose820();
  if(songEngineActive)songRecord=!songRecord;else recordingArmed=!recordingArmed;
  bool armed=songEngineActive?songRecord:recordingArmed;
  recordDelete51=false;selectionIndicatorsPending931=true;
  // Record never changes the current view or the selection.
  neoDisplayDirty=displayValueDirty=displayBpmDirty=true;
  DISPLAY_SERIAL.println(armed?F("MSG|REC OVERDUB ON"):F("MSG|REC OFF"));
  Serial.printf("REC820,ARMED=%u,SONG=%u,BASE=%u\n",armed,songEngineActive,recordBase820);
  if(songEngineActive)songState();
}
FLASHMEM bool recordPad820(uint8_t n,uint8_t pad,uint8_t edge){
  uint8_t i=n*16+pad;auto &p=recordPads820[i];
  if(edge==SEESAW_KEYPAD_EDGE_FALLING&&p.held){recordFinish820(i);p.held=false;uint8_t z=kbZoneForR10(p.track,n,pad);if(kbSettings13(p.track).zone[z].type!=KB_NOT_R10){bool held=false;for(uint8_t j=0;j<64;++j)if(recordPads820[j].held&&recordPads820[j].track==p.track&&kbZoneForR10(p.track,j/16,j%16)==z)held=true;if(!held)kbSettings13(p.track).zone[z].valid=0;}keyboardSwallow823[n]&=~(1u<<pad);neoPadDown[n][pad]=false;neoDisplayDirty=true;return true;}
  if(keyboardSwallow823[n]&(1u<<pad)){if(edge==SEESAW_KEYPAD_EDGE_FALLING){keyboardSwallow823[n]&=~(1u<<pad);neoPadDown[n][pad]=false;neoPadEdited[n][pad]=false;}return true;}
  // KB center/range use all pads, including the former octave corners.
  if(!recordContext820())return false;
  if(edge!=SEESAW_KEYPAD_EDGE_RISING||p.held)return true;
  uint8_t track=keyboardTrack823();
  p={songEngineActive?songTick:(uint64_t)localTickForTrack(track),recordClock820,track,activePattern[track],
    (uint8_t)(recordBase820+i),defaultVel[track],midiChannelForTrack(track),outMask902(track),true,false,false,songEngineActive};
  neoPadDown[n][pad]=true;neoDisplayDirty=true;
  bool sound=kbPressR10(n,pad,p.note,p.kbLocks,p.kbCount);
  if(recordCapture823()&&recordDelete51)return true;
  if(!recordCapture823()&&!songMode&&!patternMode&&!remixMode&&!grooveEditMode&&kbSelectR10(track,kbSettings13(track).zone[kbZoneForR10(track,n,pad)].type==KB_NOT_R10,p.note,p.kbLocks,p.kbCount)){if(sound&&!recordTest820){MidiScope902 route(p.outputMask,p.track);kbLiveR10(track,p.note,p.channel,p.kbLocks,p.kbCount);p.sounding=noteOnNow(p.channel,p.note,p.velocity);}return true;}
  if(!sound)return true;
  p.sounding=true;p.capture=playing&&recordCapture823();
  if(!recordTest820){MidiScope902 route(p.outputMask,p.track);kbLiveR10(track,p.note,p.channel,p.kbLocks,p.kbCount);if(noteOnNow(p.channel,p.note,p.velocity)){uint32_t gate=max(1000UL,(uint32_t)(defaultGateTicks[track]*tickDurationMs()*1000.0f));manualModStart06(track,i,gate);manualDelayPrepared06(track,p.note,p.velocity,gate);}}
  return true;
}
FLASHMEM bool recordOctave820(uint8_t){return false;}
bool recordErasePitch820(uint8_t track,uint8_t note){
  if(!recordCapture823()||!recordDelete51||!recordContext820())return false;
  for(const auto &p:recordPads820)if(p.held&&p.track==track&&p.note==note)return true;
  return false;
}
void recordErase820(){
  if(!playing||!recordCapture823()||!recordDelete51||!recordContext820())return;
  if(songEngineActive){
    for(uint32_t i=0;i<zlCount;){auto &e=zlEvents[i];
      if(e.kind==0&&e.tick==songTick&&recordErasePitch820(e.track,e.a)){
        uint32_t id=e.id;kbSongRemoveR10(id);memmove(&zlEvents[i],&zlEvents[i+1],(--zlCount-i)*sizeof(ZLEvent));++songRevision;
        songSend("ZREMOVE,%lu",(unsigned long)id);neoDisplayDirty=true;
      }else ++i;
    }return;
  }
  uint8_t t=selectedTrack;uint32_t tick=localTickForTrack(t);
  for(uint16_t s=0;s<trackStepCount(t);++s)for(int v=(int)noteCountAtStep(t,s)-1;v>=0;--v){
    int32_t at=(int32_t)s*96+audibleMicroTick(t,s,v);uint32_t length=trackSequenceTicks(t);
    at=(at%(int32_t)length+length)%length;
    if((uint32_t)at==tick&&recordErasePitch820(t,noteAtVoice(t,s,v))){removeVoiceAtStep(t,s,v);mod805Invalidate();neoDisplayDirty=true;}
  }
}
FLASHMEM bool recordRender820(){
  if(!recordContext820())return false;
  for(uint8_t n=0;n<NEO_COUNT;++n)if(neoAvailable[n]||pc831.active){
    for(uint8_t p=0;p<16;++p){uint8_t track=keyboardTrack823(),zone=kbZoneForR10(track,n,p);auto &key=kbSettings13(track);auto &settings=key.zone[zone];int pads=key.split==0?64:key.split==1?32:16,pos=kbPositionR10(track,n,p);uint8_t note=clampInt(kbCenterFor12(track,zone)+(key.split&&!key.panels?(pads/2-pos):(pos-pads/2))*settings.range,0,127);
      bool lit=keyboardLit823(midiChannelForTrack(keyboardTrack823()),note);
      uint32_t c=recordPads820[n*16+p].held?colorFor(n,90,90,90):note%12==0?colorFor(n,0,lit?100:65,lit?55:35):colorFor(n,0,lit?35:10,lit?80:24);
      uint8_t z=kbZoneForR10(keyboardTrack823(),n,p);auto&rgb=NEO_PALETTE[kbColorsR10[z]];uint8_t dim=kbSettings13(keyboardTrack823()).zone[z].mute?4:(lit?1:2);
      c=recordPads820[n*16+p].held?colorFor(n,90,90,90):colorFor(n,rgb.r/dim,rgb.g/dim,rgb.b/dim);
      c=colorScale11(c,colorBrightness11[7][z]);

      if(settings.type==KB_NOT_R10){uint8_t level=note%12==0?85:25+(note%12)*3;if(settings.mute)level/=4;c=colorScale11(colorFor(n,rgb.r,rgb.g,rgb.b),level);if(lit)c=colorFor(n,100,100,100);c=colorScale11(c,colorBrightness11[7][z]);}
      uint8_t t08=keyboardTrack823(),head=playheadBrightness24(t08,n,p);
      if(head)c=colorScale11(colorForTrackRole(n,t08,NEO_COLOR_PLAYHEAD),head);
      if(recordPads820[n*16+p].held)c=colorForTrackRole(n,t08,NEO_COLOR_SELECTED_NOTE);
      neoPixel821(n,p,c);
    }diagShowNeo814(n);serviceRealtimeBetweenNeoPanels();
  }return true;
}
void recordAdvance820(){++recordClock820;if(recordContext820()&&recordClock820%96u==0)neoDisplayDirty=true;}

FLASHMEM void recordSuspend823(){for(uint8_t i=0;i<64;++i)if(recordPads820[i].capture)recordFinish820(i);}
