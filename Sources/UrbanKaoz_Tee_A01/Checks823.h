#pragma once
FLASHMEM void keyboardCheck823(){
  if(playing||keyboardEnabled823||recordingArmed||songRecord||menuNavigationOnly()||!songWorkspaceReady||
     deleteButtonActive||hasSelectedSteps()||selectedStep>=0||pendingSelectionEdit.active||trackChord815||selectButtonActive||selectionMode){Serial.println(F("CHECK823,BUSY"));return;}
  for(uint8_t n=0;n<4;++n)for(uint8_t p=0;p<16;++p)if(neoPadDown[n][p]||trackPads815[n][p]){Serial.println(F("CHECK823,PAD_HELD"));return;}
  const uint8_t t=15;pattern805CaptureLive(t,recordFixture820);
  uint8_t oldTrack=selectedTrack,oldSongTrack=songSelectedTrack,oldBase=recordBase820,oldVoice=selectedVoice;
  bool oldSong=songMode,oldEngine=songEngineActive,oldPattern=patternMode,oldRemix=remixMode,oldGroove=grooveEditMode,oldExplicit=explicitNoteSelection;
  bool oldDelete=hc165ProbeStable[5],oldSdNotice=sdNeoNoticeActive;
  uint64_t oldClock=recordClock820,oldTick=songTick;
  uint32_t oldCount=zlCount,oldNext=zlNextId,oldRevision=songRevision;
  uint8_t oldWrite=songOutWrite;bool oldOverflow=songOutOverflow;
  memcpy(zlStaging,zlEvents,zlCount*sizeof(ZLEvent));
  uint16_t pass=0,fail=0;auto check=[&](bool ok,const __FlashStringHelper *name){if(ok)++pass;else ++fail;Serial.print(F("CHECK823,"));Serial.print(ok?F("PASS,"):F("FAIL,"));Serial.println(name);};
  recordTest820=true;selectedTrack=songSelectedTrack=t;recordBase820=36;
  songMode=songEngineActive=patternMode=remixMode=grooveEditMode=hc165ProbeStable[5]=false;
  auto down=[](uint8_t p){onPadEdge(0,p,SEESAW_KEYPAD_EDGE_RISING);};
  auto up=[](uint8_t p){onPadEdge(0,p,SEESAW_KEYPAD_EDGE_FALLING);};
  handleTransportButtonEdge(0,true);check(recordingArmed&&!keyboardEnabled823&&!recordContext820(),F("record_preserves_grid"));
  handleTransportButtonEdge(0,true);
  dispatchStableButtonPress(11);check(keyboardEnabled823&&!recordingArmed&&recordContext820(),F("b11_independent"));
  playing=true;down(0);check(recordPads820[0].sounding&&!recordPads820[0].capture,F("warmup_during_play"));up(0);playing=false;
  bool modes=true;patternMode=true;modes&=recordContext820();patternMode=false;remixMode=true;modes&=!recordContext820();remixMode=false;grooveEditMode=true;modes&=recordContext820();grooveEditMode=false;songMode=true;modes&=recordContext820();songMode=false;
  check(modes,F("keyboard_all_musical_modes"));
  int oldOctave=recordBase820;onPadEdge(3,15,SEESAW_KEYPAD_EDGE_RISING);onPadEdge(3,15,SEESAW_KEYPAD_EDGE_FALLING);check(recordBase820==oldOctave+12,F("octave_without_record"));down(12);up(12);
  // Pure draft editing: no commit and no P.lock database mutation.
  for(uint16_t s=0;s<STEP_COUNT;++s)clearTrackStep(t,s);
  RecordPad820 e={37,0,t,activePattern[t],60,100,0,false,false,true,false};recordStore820(e,96);
  e.tick=59;e.note=67;recordStore820(e,72);e.tick=96;e.note=72;recordStore820(e,40);
  explicitNoteSelection=true;setNoteObjectSelected(t,0,0,true);setNoteObjectSelected(t,0,1,true);selectButtonActive=selectionMode=true;
  handleTransportButtonEdge(0,true);check(recordingArmed&&selectButtonActive&&selectedNoteObject(t,0,0),F("record_preserves_selection"));
  playing=true;down(4);
  check(!recordPads820[4].capture&&!recordPads820[4].sounding&&pendingSelectionEdit.active,F("select_routes_to_edit"));
  check(visualNoteAtVoice(t,0,0)==40&&visualNoteAtVoice(t,0,1)==40&&visualNoteAtVoice(t,1,0)==72,F("all_selected_same_pitch_only"));up(4);
  discardPendingSelectionEdit();clearStepSelection();selectedStep=1;selectedVoice=0;
  down(7);check(visualNoteAtVoice(t,1,0)==43&&noteCountAtStep(t,1)==1,F("single_selected_pitch"));up(7);
  discardPendingSelectionEdit();selectedStep=-1;selectButtonActive=selectionMode=false;
  // Song target keeps fixture insertion away from the user's live pattern state.
  songMode=songEngineActive=songRecord=true;recordingArmed=false;zlCount=0;songTick=137;
  down(1);check(recordPads820[1].capture,F("capture_resumes_after_select"));recordClock820+=20;
  selectButtonActive=selectionMode=true;keyboardService823();
  check(zlCount==1&&zlEvents[0].gate==20&&!recordPads820[1].capture,F("select_closes_inflight_take"));
  recordClock820+=20;selectButtonActive=selectionMode=false;keyboardService823();up(1);
  check(zlCount==1,F("held_note_no_restart_after_select"));
  down(2);recordClock820+=9;dispatchStableButtonPress(11);
  check(!keyboardEnabled823&&songRecord&&zlCount==2,F("b11_off_closes_take_keeps_record"));up(2);
  check(!recordPads820[2].held&&!keyboardSwallow823[0],F("paired_release_after_b11"));
  dispatchStableButtonPress(11);down(2);check(recordPads820[2].capture,F("next_press_after_b11_valid"));up(2);
  selectButtonActive=selectionMode=true;hc165ProbeStable[5]=true;check(!recordErasePitch820(t,38),F("select_suspends_record_delete"));hc165ProbeStable[5]=false;selectButtonActive=selectionMode=false;
  playing=false;songMode=songEngineActive=songRecord=false;recordingArmed=false;
  dispatchStableButtonPress(52);
  check(trackChord815&&!recordContext820(),F("b52_overrides_keyboard"));
  trackTest818=true;trackPreviewCapture818=255;onPadEdge(0,4,SEESAW_KEYPAD_EDGE_RISING);onPadEdge(0,4,SEESAW_KEYPAD_EDGE_FALLING);
  check(trackPreviewCapture818==0,F("b52_preview_action_preserved"));trackTest818=false;
  dispatchStableButtonPress(52);
  check(!trackChord815&&recordContext820(),F("b52_returns_keyboard"));
  MidiScope902 activityRoute33(outMask902(t),t);keyboardLightsClear823();uint8_t channel=midiChannelForTrack(t);keyboardMidi823(channel,60,true);
  check(keyboardLit823(channel,60)&&!keyboardLit823((channel+1)%16,60),F("light_channel_isolation"));
  check(keyboardTrackPulse823(t),F("pre_activity_pulse"));
  keyboardMidi823(channel,60,false);check(!keyboardLit823(channel,60),F("light_noteoff"));
  keyboardMidi823(channel,61,true);keyboardDrum823(channel,61,1000000);
  check(keyboardLit823(channel,61),F("drum_gate_visible"));
  for(auto &d:drumLights823)if(d.active)d.until=micros()-1;keyboardService823();check(!keyboardLit823(channel,61),F("drum_gate_expires"));
  for(uint8_t i=0;i<16;++i)trackPulseUntil823[i]=millis()-1;keyboardService823();check(trackPulseMask823==0,F("pre_pulse_expires"));
  keyboardMidi823(channel,62,true);keyboardLightsClear823();check(!keyboardLit823(channel,62),F("stop_clears_lights"));
  // A pad held in the grid when B11 changes view is consumed, never repurposed.
  keyboardEnabled823=false;neoPadDown[0][3]=true;dispatchStableButtonPress(11);up(3);
  check(!keyboardSwallow823[0]&&!recordPads820[3].held&&!neoPadDown[0][3],F("grid_held_release_consumed"));
  keyboardEnabled823=false;recordTest820=false;recordingArmed=songRecord=false;
  discardPendingSelectionEdit();clearStepSelection();selectedStep=-1;selectedVoice=oldVoice;explicitNoteSelection=oldExplicit;
  pattern805ApplyLive(t,recordFixture820);memcpy(zlEvents,zlStaging,oldCount*sizeof(ZLEvent));
  zlCount=oldCount;zlNextId=oldNext;songRevision=oldRevision;recordClock820=oldClock;songTick=oldTick;
  selectedTrack=oldTrack;songSelectedTrack=oldSongTrack;recordBase820=oldBase;
  songMode=oldSong;songEngineActive=oldEngine;patternMode=oldPattern;remixMode=oldRemix;grooveEditMode=oldGroove;
  hc165ProbeStable[5]=oldDelete;sdNeoNoticeActive=oldSdNotice;songOutWrite=oldWrite;songOutOverflow=oldOverflow;
  neoDisplayDirty=displayValueDirty=true;
  Serial.printf("CHECK823,%s,PASS=%u,FAIL=%u\n",fail?"FAIL":"PASS",pass,fail);
}
