#pragma once
EXTMEM PatternImage805 recordFixture820;
FLASHMEM void recordCheck820(){
  if(playing||recordingArmed||songRecord||menuNavigationOnly()||!songWorkspaceReady||deleteButtonActive||hasSelectedSteps()){
    Serial.println(F("CHECK820,BUSY"));return;
  }
  for(const auto &p:recordPads820)if(p.held){Serial.println(F("CHECK820,BUSY"));return;}
  bool oldKeyboard=keyboardEnabled823;keyboardEnabled823=true;
  const uint8_t t=15;pattern805CaptureLive(t,recordFixture820);
  uint8_t oldTrack=selectedTrack,oldSongTrack=songSelectedTrack,oldBase=recordBase820;
  bool oldEngine=songEngineActive,oldMode=songMode,oldPattern=patternMode,oldDelete=hc165ProbeStable[5];
  uint64_t oldClock=recordClock820,oldTick=songTick;
  uint32_t oldCount=zlCount,oldId=zlNextId,oldRevision=songRevision;
  memcpy(zlStaging,zlEvents,zlCount*sizeof(ZLEvent));
  uint8_t oldWrite=songOutWrite;bool oldOverflow=songOutOverflow;
  uint16_t pass=0,fail=0;
  auto check=[&](bool ok,const __FlashStringHelper *name){if(ok)++pass;else ++fail;Serial.print(F("CHECK820,"));Serial.print(ok?F("PASS,"):F("FAIL,"));Serial.println(name);};
  selectedTrack=songSelectedTrack=t;songEngineActive=songMode=patternMode=false;recordingArmed=recordTest820=true;
  hc165ProbeStable[5]=false;trackBars[t]=4;
  for(uint16_t s=0;s<STEP_COUNT;++s)clearTrackStep(t,s);
  RecordPad820 e={37,0,t,activePattern[t],60,100,0,1,false,false,true,false};
  check(recordStore820(e,113),F("step_insert"));
  check(stepLogicalMicro[t][0]==37&&stepGateTicks[t][0]==113&&noteCountAtStep(t,0)==1,F("timing_duration_no_grid"));
  e.tick=59;check(recordStore820(e,71)&&noteCountAtStep(t,0)==2,F("same_pitch_overdub"));
  int fine=-1;for(int i=0;i<FINE_EVENTS_PER_TRACK;++i)if(fineNote[t][i].active)fine=i;
  check(fine>=0&&fineNote[t][fine].logicalMicro+fineNote[t][fine].micro==59&&fineNote[t][fine].gate==71,F("independent_duration"));
  e.tick=trackSequenceTicks(t)-1;check(recordStore820(e,27),F("last_loop_tick_insert"));
  uint16_t last=e.tick/96;
  check(stepLogicalMicro[t][last]+stepMicroTick[t][last]==95,F("last_loop_tick_exact"));
  // Delete sees only crossings while the physical modifier and pitch are held.
  uint64_t savedTransport=transportTickAbsolute;
  uint32_t crossing=localTickForTrack(t);uint16_t cell=crossing/96;
  clearTrackStep(t,cell);e.tick=crossing;e.note=36;recordStore820(e,40);
  e.note=48;recordStore820(e,40);
  recordBase820=36;hc165ProbeStable[5]=true;playing=true;
  recordPad820(0,0,SEESAW_KEYPAD_EDGE_RISING);recordErase820();
  check(findVoiceAtStep(t,cell,36)<0&&findVoiceAtStep(t,cell,48)>=0,F("step_delete_preserves_other_octave"));
  recordPad820(0,0,SEESAW_KEYPAD_EDGE_FALLING);hc165ProbeStable[5]=false;playing=false;
  transportTickAbsolute=savedTransport;
  // Arming while stopped auditions but never stores; octave changes do not retune a held note.
  recordPad820(0,0,SEESAW_KEYPAD_EDGE_RISING);
  check(recordPads820[0].held&&!recordPads820[0].capture,F("armed_before_play"));
  uint8_t pitch=recordPads820[0].note;recordOctave820(15);
  check(recordPads820[0].note==pitch,F("held_octave_identity"));
  recordPad820(0,0,SEESAW_KEYPAD_EDGE_FALLING);
  check(!recordPads820[0].held,F("paired_release"));
  songEngineActive=songMode=songRecord=true;recordingArmed=false;zlCount=0;songTick=137;playing=true;
  recordPad820(0,1,SEESAW_KEYPAD_EDGE_RISING);recordClock820+=123;recordPad820(0,1,SEESAW_KEYPAD_EDGE_FALLING);
  check(zlCount==1&&zlEvents[0].tick==137&&zlEvents[0].gate==123,F("song_exact_note_gate"));
  recordPad820(0,1,SEESAW_KEYPAD_EDGE_RISING);recordClock820+=11;recordPad820(0,1,SEESAW_KEYPAD_EDGE_FALLING);
  check(zlCount==2,F("song_overdub"));
  hc165ProbeStable[5]=true;recordPad820(0,1,SEESAW_KEYPAD_EDGE_RISING);
  check(!recordPads820[1].sounding&&!recordPads820[1].capture,F("delete_silent"));
  check(recordErasePitch820(t,recordBase820+1)&&!recordErasePitch820(t,recordBase820+13),F("delete_exact_octave"));
  recordErase820();check(zlCount==0,F("delete_crossing_zl"));
  hc165ProbeStable[5]=false;check(!recordErasePitch820(t,recordBase820+1),F("delete_release_stops"));
  recordPad820(0,1,SEESAW_KEYPAD_EDGE_FALLING);
  songTick=151;recordPad820(0,2,SEESAW_KEYPAD_EDGE_RISING);recordClock820+=17;recordClose820();
  check(zlCount==1&&zlEvents[0].gate==17&&!recordPads820[2].sounding&&!recordPads820[2].capture,F("stop_finalizes"));
  recordPad820(0,2,SEESAW_KEYPAD_EDGE_FALLING);check(zlCount==1,F("stop_release_no_duplicate"));
  playing=false;recordingArmed=songRecord=recordTest820=false;
  pattern805ApplyLive(t,recordFixture820);memcpy(zlEvents,zlStaging,oldCount*sizeof(ZLEvent));
  zlCount=oldCount;zlNextId=oldId;songRevision=oldRevision;songTick=oldTick;recordClock820=oldClock;
  selectedTrack=oldTrack;songSelectedTrack=oldSongTrack;recordBase820=oldBase;songEngineActive=oldEngine;songMode=oldMode;patternMode=oldPattern;hc165ProbeStable[5]=oldDelete;
  keyboardEnabled823=oldKeyboard; songOutWrite=oldWrite;songOutOverflow=oldOverflow;
  neoDisplayDirty=displayValueDirty=true;
  Serial.printf("CHECK820,%s,PASS=%u,FAIL=%u\n",fail?"FAIL":"PASS",pass,fail);
}
