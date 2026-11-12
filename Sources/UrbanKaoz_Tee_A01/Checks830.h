#pragma once
FLASHMEM void status830(){
  static const char*names[P830_COUNT]={"TRANSPORT","TICK","TRACK","AGENDA","LIST","SONG","BRIDGE","SOURCE","CURVE","FRAME","COLLECT","FLUSH","OFFER","MIDI_TX","PIDI_QUEUE","DEPS30","INDEX30","ADDR30","ROLL30","RVL30"};
  const uint32_t cyclesPerUs=F_CPU_ACTUAL/1000000;
  Serial.printf("LAT830,BEGIN,PROFILE=%u,OPT=%u,LATE_LIMIT_US=%lu,PSRAM_MB=%u\n",profile830,optimize830,LATE_LIMIT830,external_psram_size);
  for(uint8_t i=0;i<P830_COUNT;++i){const auto&s=stats830[i];
    Serial.printf("LAT830,%s,N=%lu,INCL_US=%llu,SELF_US=%llu,MAX_US=%lu,TRACK=%u\n",names[i],s.count,s.inclusive/cyclesPerUs,s.exclusive/cyclesPerUs,s.maximum/cyclesPerUs,s.track);
  }
  Serial.printf("LAT830,COUNTS,SOURCE_HIT=%lu,SOURCE_MISS=%lu,SOURCE_VOICES=%lu,FRAMES=%lu,FLUSH_VISITS=%lu,AGENDA_OK=%lu,FALLBACK=%lu\n",sourceHits830,sourceMisses830,sourceVoices830,frameBuilds830,flushVisited830,agendaEligible830,agendaFallback830);
  Serial.printf("LAT830,LATE,TICKS=%lu,NOTES=%lu,TICK_AGE_MAX_US=%lu,NOTE_AGE_MAX_US=%lu,PENDING=%lu\n",lateTicks830,lateNotes830,maxTickAge830,maxNoteAge830,(uint32_t)pendingClockTicks);
  Serial.println("LAT830,END");
}

// Explicit, RAM-only fixture. Refuse a loaded/playing session. All fields touched
// are restored; no project reset, persistence API, EEPROM or physical MIDI notes.
EXTMEM FineNoteEvent savedFine830[FINE_EVENTS_PER_TRACK];
EXTMEM uint32_t savedTriggers830[STEP_COUNT],savedFineTriggers830[FINE_EVENTS_PER_TRACK];
EXTMEM uint32_t savedLocal830[TRACK_COUNT];
EXTMEM DeferredRollAudio savedDeferred830[DEFERRED_ROLL_COUNT];
FLASHMEM void checks830(){
  if(playing||internalClockRunning||pendingClockTicks||sdAvailable||songEngineActive||zlCount||lockCount817||lockLeaseCount817||modDirty806||selectButtonActive||deleteButtonActive||recordingArmed||songRecord||clipboardPreviewActive||anyLiveClipboardPreview()||midiCounters810.pending){Serial.println("CHECK830,REQUIRES_STOP_EMPTY_NO_SD");return;}
  for(const auto&n:activeNotes)if(n.active){Serial.println("CHECK830,NOTES_SOUNDING");return;}
  for(uint16_t k=0;k<64;++k)if(mod806Unit(k).enabled){Serial.println("CHECK830,MOD_DATA_PRESENT");return;}
  for(uint8_t t=0;t<16;++t){
    if(pendingPattern[t]>=0||grooveBridgeReady[t]){Serial.println("CHECK830,TRANSITION_PRESENT");return;}
    for(uint16_t s=0;s<STEP_COUNT;++s)if(stepActive[t][s]){Serial.println("CHECK830,NOTES_PRESENT");return;}
    for(const auto&n:fineNote[t])if(n.active){Serial.println("CHECK830,FINE_NOTES_PRESENT");return;}
  }
  for(const auto&c:songClips)if(c.used){Serial.println("CHECK830,CLIPS_PRESENT");return;}
  uint16_t pass=0,fail=0;
  auto check=[&](bool ok,const char*n){if(ok)++pass;else ++fail;Serial.printf("CHECK830,%s,%s\n",ok?"PASS":"FAIL",n);};
  bool oldProfile=profile830,oldOptimize=optimize830,oldEnv=playbackEnv828,oldLfo=playbackLfo828;
  bool oldCapture=grooveTestCapture,oldMuted[16];memcpy(oldMuted,trackMuted,sizeof(oldMuted));
  auto oldUnit=mod806Unit(0);auto oldMeta=modMeta806[0];auto oldRoll=trackRoll[0];
  auto oldExtra=modExtra810[0];uint16_t oldRat=modRat808[0];
  uint16_t oldBars=trackBars[0],oldStart=loopStartStep,oldEnd=loopEndStep;
  uint32_t oldPhase=grooveTrackPhase[0];uint64_t oldTick=transportTickAbsolute;
  uint8_t oldGroove=patternGroove[0][activePattern[0]];bool oldRemix=remixMode;
  uint32_t oldTestNotes=grooveTestNotes;auto oldTestPitch=grooveTestPitch;auto oldTestGate=grooveTestGateUs;
  auto oldMidi=midiPending810[0][74];
  uint16_t oldStep=currentStep,oldClockWrite=clockWrite830;uint32_t oldMeasure=measureTick,oldStepCounter=playbackStepCounter;
  uint32_t oldView=viewTickOffset;auto oldViewSource=pendingViewSource;
  bool oldNeoDirty=neoDisplayDirty,oldBpmDirty=displayBpmDirty,oldEvaluate=evaluateCurrentTickOnNextClock;
  uint64_t oldRecordClock=recordClock820;
  memcpy(savedTriggers830,lastTriggeredCycle[0],sizeof(savedTriggers830));memcpy(savedFineTriggers830,fineLastTriggeredCycle[0],sizeof(savedFineTriggers830));
  memcpy(savedLocal830,lastTrackLocalTick,sizeof(savedLocal830));memcpy(savedDeferred830,deferredRoll,sizeof(savedDeferred830));
  memcpy(savedFine830,fineNote[0],sizeof(savedFine830));
  profile830=false;playing=true;grooveTestCapture=true;remixMode=false;
  memset(trackMuted,1,sizeof(trackMuted));trackMuted[0]=false;
  trackBars[0]=64;loopStartStep=1;loopEndStep=STEP_COUNT;grooveTrackPhase[0]=0;
  patternGroove[0][activePattern[0]]=0;trackRoll[0]={};
  for(uint16_t s=0;s<STEP_COUNT;s+=4)stepActive[0][s]=true;
  for(uint8_t i=0;i<FINE_EVENTS_PER_TRACK;++i){auto&e=fineNote[0][i];e={};e.active=true;e.step=(i/4)*4;e.note=60+i%4;}
  auto&m=mod806Unit(0);m={};m.enabled=1;m.inputs=1;m.decay=32;m.release=16;m.rate=384;
  m.outputs[0]={MOD_CC,0,74,1,0};modMeta806[0]={};modExtra810[0]={};modRat808[0]=1;
  playbackEnv828=playbackLfo828=true;mod805Invalidate();
  optimize830=true;agendaBegin822();check(!timingMod830(0),"unassociated_env_keeps_agenda");agendaEnd822();
  mod806Associate(modMeta806[0],0,activePattern[0],true);mod805Invalidate();
  agendaBegin822();check(!timingMod830(0),"CC_env_keeps_agenda");agendaEnd822();
  m.outputs[0].kind=MOD_TIM;m.outputs[0].track=1;mod805Invalidate();agendaBegin822();
  check(!timingMod830(0)&&timingMod830(1),"timing_only_disables_destination_even_zero_depth");agendaEnd822();
  m.outputs[0].kind=MOD_CC;m.outputs[0].track=0;
  auto oldVirtual=virtualLinks805[0];
  m.outputs[0]={MOD_VIRTUAL,0,5,1,0};virtualLinks805[0]={5,MOD_RSP,2,1,0};mod805Invalidate();agendaBegin822();
  check(timingMod830(2)&&!timingMod830(0),"virtual_timing_route_conservative_at_zero_depth");agendaEnd822();
  virtualLinks805[0]=oldVirtual;m.outputs[0]={MOD_CC,0,74,1,0};
  // Same source results, including out-of-order lookups and loop boundaries.
  bool same=true;static const int64_t times[]={0,8,384,17,98304,98312,383,98400};
  for(auto at:times){optimize830=false;mod805Invalidate();auto a=mod805Source(0,at);
    optimize830=true;mod805Invalidate();auto b=mod805Source(0,at);
    same&=a.begin==b.begin&&a.next==b.next&&a.gate==b.gate&&a.validUntil==b.validUntil;
  }check(same,"source_results_identical");
  // Four cases, 16 adjacent ticks per slice; include agenda construction.
  for(uint8_t scenario=0;scenario<4;++scenario){
    playbackEnv828=scenario!=0;modMeta806[0]={};
    if(scenario>=2)mod806Associate(modMeta806[0],0,activePattern[0],true);
    m.outputs[0].depth=scenario==3?127:0;
    uint32_t notes[2]={};
    for(uint8_t mode=0;mode<2;++mode){optimize830=mode;mod805Invalidate();resetStats830();grooveTestNotes=0;
      // Reset trigger guards so both runs own the same occurrences.
      memset(lastTriggeredCycle[0],255,sizeof(lastTriggeredCycle[0]));memset(fineLastTriggeredCycle[0],255,sizeof(fineLastTriggeredCycle[0]));
      uint32_t began=micros();agendaBegin822();
      for(uint8_t tick=0;tick<16;++tick){transportTickAbsolute=tick;processEventsAtCurrentTick();}
      agendaEnd822();uint32_t us=micros()-began;notes[mode]=grooveTestNotes;
      Serial.printf("BENCH830,CASE=%u,OPT=%u,TICKS=16,US=%lu,NOTES=%lu,SOURCE_MISS=%lu,FRAMES=%lu,AGENDA_OK=%lu,FALLBACK=%lu\n",scenario,mode,us,notes[mode],sourceMisses830,frameBuilds830,agendaEligible830,agendaFallback830);
    }check(notes[0]==notes[1],"same_note_count_baseline_optimized");
  }
  // Profile both paths separately from the wall-time comparisons above. These
  // figures include measurement overhead and exclude physical MIDI emission.
  playbackEnv828=true;mod806Associate(modMeta806[0],0,activePattern[0],true);m.outputs[0].depth=0;
  for(uint8_t mode=0;mode<2;++mode){
    optimize830=mode;profile830=true;mod805Invalidate();resetStats830();agendaBegin822();
    for(uint8_t tick=1;tick<=16;++tick){transportTickAbsolute=tick;Scope830 measured(P830_TICK);processEventsAtCurrentTick();}
    agendaEnd822();const uint32_t cpu=F_CPU_ACTUAL/1000000;
    Serial.printf("BENCH830,PROFILE,OPT=%u,TICK_US=%llu,AGENDA_US=%llu,SOURCE_US=%llu,CURVE_US=%llu,FRAME_SELF_US=%llu,TRACK_SELF_US=%llu\n",mode,stats830[P830_TICK].inclusive/cpu,stats830[P830_AGENDA].inclusive/cpu,stats830[P830_SOURCE].inclusive/cpu,stats830[P830_CURVE].inclusive/cpu,stats830[P830_FRAME].exclusive/cpu,stats830[P830_TRACK].exclusive/cpu);
  }
  profile830=false;
  // Actual flush behavior: active -> zero -> disabled, old pending automation cancelled.
  optimize830=true;playbackEnv828=true;m.outputs[0].depth=127;
  mod805Invalidate();mod808FlushMidi(0,4);check(modMidiWas806[0][74],"active_midi_registered");
  m.outputs[0].depth=0;mod805Invalidate();mod808FlushMidi(1,4);
  check(!modMidiWas806[0][74]&&!midiPending810[0][74].pending,"depth_zero_cancels_pending");
  m.outputs[0].depth=127;mod805Invalidate();mod808FlushMidi(0,4);
  playbackEnv828=false;mod805Invalidate();mod808FlushMidi(1,4);
  check(!modMidiWas806[0][74]&&!midiPending810[0][74].pending,"bypass_cancels_pending");
  for(uint8_t mode=0;mode<2;++mode){optimize830=mode;resetStats830();uint32_t began=micros();
    for(uint8_t i=0;i<32;++i)mod808FlushMidi(i,4);
    Serial.printf("BENCH830,EMPTY_FLUSH,OPT=%u,N=32,US=%lu,VISITS=%lu\n",mode,(uint32_t)(micros()-began),flushVisited830);
  }
  clockContext830=true;tickDue830=micros()-LATE_LIMIT830-100;check(rejectLateNote830(),"expired_note_rejected");
  tickDue830=micros();check(!rejectLateNote830(),"current_note_allowed");
  clockContext830=false;check(!rejectLateNote830(),"live_note_outside_transport_allowed");
  uint32_t oldOverruns=clockOverruns821;
  for(uint16_t i=0;i<4100;++i)clockEnqueue830(micros()-100000);
  check(pendingClockTicks==4100&&clockOverruns821-oldOverruns==4,"timestamp_overflow_preserves_musical_tick_count");
  clockContext830=true;bool taken=clockTake830();check(taken&&tickExpired830(),"overwritten_timestamp_is_expired");
  pendingClockTicks=0;clockOverruns821=oldOverruns;clockContext830=false;
  // Exercise the real transport queue across the timestamp-ring wrap. A stale
  // note at tick 384 must not reach the capture hook, yet all eight ticks advance.
  optimize830=true;playbackEnv828=playbackLfo828=false;mod805Invalidate();grooveTestNotes=0;
  transportTickAbsolute=383;evaluateCurrentTickOnNextClock=false;clockWrite830=4094;
  for(uint8_t i=0;i<8;++i){clockDates830[clockWrite830]=micros()-100000;clockWrite830=(clockWrite830+1)&4095;}
  pendingClockTicks=8;uint8_t calls=0;while(pendingClockTicks&&calls++<16)updateTransport();
  check(!pendingClockTicks&&transportTickAbsolute==391&&grooveTestNotes==0,"stale_queue_advances_without_note_burst");
  transportTickAbsolute=383;memset(lastTriggeredCycle[0],255,sizeof(lastTriggeredCycle[0]));memset(fineLastTriggeredCycle[0],255,sizeof(fineLastTriggeredCycle[0]));
  clockDates830[clockWrite830]=micros();clockWrite830=(clockWrite830+1)&4095;pendingClockTicks=1;updateTransport();
  check(transportTickAbsolute==384&&grooveTestNotes==5,"fresh_note_after_catchup_has_all_voices");
  // Direct trigger guard must precede P.locks, scheduled gates and Pidi requests.
  uint32_t beforeNotes=grooveTestNotes,beforeDelays=pidiDelays827,beforeFrames=pidiFrames827;
  clockContext830=true;tickDue830=micros()-100000;triggerTrackNoteWithFixedGate(0,60,100,10000);
  check(grooveTestNotes==beforeNotes&&pidiDelays827==beforeDelays&&pidiFrames827==beforeFrames,"expired_trigger_has_no_note_or_delay_side_effect");
  // Song must also advance and suppress stale notes, then resume the same event.
  auto oldClip=songClips[0];uint64_t oldSongTick=songTick;bool oldSongLoop=songLoopEnabled;
  songClips[0]={0,0,16,0,activePattern[0],true};songLoopEnabled=false;songEngineActive=true;songTick=383;
  songClockTick();check(songTick==384&&grooveTestNotes==beforeNotes,"song_stale_tick_advances_silently");
  tickDue830=micros();songTick=383;songClockTick();check(songTick==384&&grooveTestNotes==beforeNotes+5,"song_current_tick_resumes");
  songClips[0]=oldClip;songTick=oldSongTick;songLoopEnabled=oldSongLoop;songEngineActive=false;clockContext830=false;
  playing=false;profileTrack830=255;
  for(uint16_t s=0;s<STEP_COUNT;s+=4)stepActive[0][s]=false;
  memcpy(fineNote[0],savedFine830,sizeof(savedFine830));memcpy(trackMuted,oldMuted,sizeof(oldMuted));
  mod806Unit(0)=oldUnit;modMeta806[0]=oldMeta;modExtra810[0]=oldExtra;modRat808[0]=oldRat;
  trackBars[0]=oldBars;loopStartStep=oldStart;loopEndStep=oldEnd;grooveTrackPhase[0]=oldPhase;transportTickAbsolute=oldTick;
  trackRoll[0]=oldRoll;patternGroove[0][activePattern[0]]=oldGroove;remixMode=oldRemix;
  grooveTestCapture=oldCapture;grooveTestNotes=oldTestNotes;grooveTestPitch=oldTestPitch;grooveTestGateUs=oldTestGate;
  midiPending810[0][74]=oldMidi;
  currentStep=oldStep;clockWrite830=oldClockWrite;measureTick=oldMeasure;playbackStepCounter=oldStepCounter;
  viewTickOffset=oldView;pendingViewSource=oldViewSource;neoDisplayDirty=oldNeoDirty;displayBpmDirty=oldBpmDirty;evaluateCurrentTickOnNextClock=oldEvaluate;recordClock820=oldRecordClock;
  memcpy(lastTriggeredCycle[0],savedTriggers830,sizeof(savedTriggers830));memcpy(fineLastTriggeredCycle[0],savedFineTriggers830,sizeof(savedFineTriggers830));
  memcpy(lastTrackLocalTick,savedLocal830,sizeof(savedLocal830));memcpy(deferredRoll,savedDeferred830,sizeof(savedDeferred830));
  playbackEnv828=oldEnv;playbackLfo828=oldLfo;profile830=oldProfile;optimize830=oldOptimize;
  mod805Invalidate();mod808FlushMidi(0,4);resetStats830();
  Serial.printf("CHECK830,END,PASS=%u,FAIL=%u,RESTORED=1\n",pass,fail);
}

