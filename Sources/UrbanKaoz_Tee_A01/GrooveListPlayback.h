#pragma once
// Evaluate the sheet at the logical occurrence, not at the shifted Note On.
// This also covers anticipation from the next bar and delay from the previous.
void playListRoll(uint8_t track, uint16_t step, uint8_t voice, int64_t target, int64_t now) {
  TrackRollSettings roll = audibleRollSettings(track, step, voice);
  if (!roll.start || (!roll.level && !rollExtra1000(track,activePattern[track],(int32_t)step*96+logicalMicroTick(track,step,voice),noteAtVoice(track,step,voice)).v[1])) return;
  const int64_t origin38=target;
  int64_t tick = target - roll.start;
  target-=rollStop09(roll,rollExtra1000(track,activePattern[track],(int32_t)step*96+logicalMicroTick(track,step,voice),noteAtVoice(track,step,voice)));
  int32_t spacing = clampInt(roll.speed, 1, TICKS_PER_STEP);
  int32_t acceleration = (int32_t)roll.accel * 256;
  for(uint16_t repeat = 0; repeat < MAX_PREROLL_NOTES && tick < target; ++repeat) {
    if(rollDirection60(track,activePattern[track],lockTick817(track,step,voice),noteAtVoice(track,step,voice))!=1)blockObserveDue29(tick,now);
    if(rollDirection60(track,activePattern[track],lockTick817(track,step,voice),noteAtVoice(track,step,voice))!=0)blockObserveDue29(2*origin38-tick,now);
    if ((rollDirection60(track,activePattern[track],lockTick817(track,step,voice),noteAtVoice(track,step,voice))!=1&&tick==now)||(rollDirection60(track,activePattern[track],lockTick817(track,step,voice),noteAtVoice(track,step,voice))!=0&&2*origin38-tick==now)) triggerRollNote(track, step, voice, rollVelocity(roll, repeat),repeat,(int32_t)(now-origin38));
    if (spacing >= target - tick) break;
    tick += spacing;
    rollAdvance60(spacing,acceleration,roll.expo,repeat);
  }
}

void processGrooveListTrack(uint8_t track, uint32_t local) {
  Scope830 measured(P830_LIST);
  int64_t now = (int64_t)transportTickAbsolute;
  uint32_t length = trackSequenceTicks(track);
  grooveEventTrack = track;
  for (int8_t offset = -19; offset <= 20; ++offset) {
      int64_t head = now - local % TICKS_PER_STEP + (int32_t)offset * TICKS_PER_STEP;
      // TIM composition +/-96, Groove +/-99, Roll <=1536 ticks, either side of the note.
      if (head < 0 || head < now - 1731 || head > now + 1731) continue;
      modLogicalTick805 = head;
      uint32_t master = bridgeMasterTickAt((uint64_t)head);
      uint32_t logical = (master + length - grooveTrackPhase[track] % length) % length;
      if (logical % TICKS_PER_STEP) continue;
      uint16_t step = logical / TICKS_PER_STEP;
      if (step >= trackStepCount(track) || !stepActive[track][step]) continue;
      grooveEventSheet = listSheetAtTick(track, activePattern[track], (uint64_t)head);
      uint8_t emitted[EXTRA_NOTES_PER_STEP + 1], emittedCount = 0;
      for (uint8_t voice = 0; voice < noteCountAtStep(track, step); ++voice) {
        if (pendingDeleteNote(track, step, voice) || previewSuppressesVoice(track, step, voice)) continue;
        if (voice && voice < baseVoiceCountAtStep(track, step) && trackPlayMode[linkOwner12(track,LINK_POLY12)] == TRACK_PLAY_MONO) continue;
        uint8_t pitch = audibleNoteAtVoice(track, step, voice);
        if (voice < baseVoiceCountAtStep(track, step)) {
          bool duplicate = false;
          for (uint8_t i = 0; i < emittedCount; ++i) duplicate |= emitted[i] == pitch;
          if (duplicate) continue;
          emitted[emittedCount++] = pitch;
        }
        int64_t target = head + audibleMicroTick(track, step, voice);
        // Prepared incoming/outgoing events own occurrences across a switch.
        if (grooveBridgeReady[track] && !grooveBridgeSwitched[track] && head >= (int64_t)grooveBridgeAt[track]) continue;
        if (grooveBridgeSwitched[track] && head < (int64_t)grooveBridgeAt[track]) continue;
        playListRoll(track, step, voice, target, now);
        if (target != now) continue;
        uint32_t gate = (uint32_t)clampInt((int)(audibleGateTicks(track, step, voice) * tickDurationMs() * 1000.0f), 1000, 20000000);
        triggerTrackNoteWithFixedGate(track, pitch, audibleVel(track, step, voice), gate, 0);
      }
  }
  modLogicalTick805 = -1;
  grooveEventSheet = -1;
  grooveEventTrack = 255;
}
