#pragma once
// Contexts: Step, Pattern, Groove steps, Groove pattern list, Remix, Song.
FLASHMEM uint8_t selection805Context() {
  if (songMode) return 5;
  if (remixMode) return 4;
  if (grooveEditMode) return patternMode ? 3 : 2;
  return patternMode ? 1 : 0;
}
FLASHMEM void selection805SongState() {
  songSend("SELMODE,%u,%u", shiftButtonActive, columnSelectionMode);
}
FLASHMEM void selection805Reset() {
  rectangleArmed = false;
  selectionAllCycle = 0; selectionAllContext = 255;
  memset(columnScopes805, 0, sizeof(columnScopes805));
  grooveListSelection805 = 0; grooveListAnchor805 = -1;
  songSend("SELCLEAR");
}
FLASHMEM void selection805Changed() {
 
  neoDisplayDirty = displayValueDirty = true;
  modDisplayDirty810=patchDisplayDirty810=true;memset(modPreviousLcd806,0,sizeof(modPreviousLcd806));
  updateRoySelectionIndicators(); sendSelectionIndicatorsToPicran();
  if (grooveEditMode) sendGroovePacket();
}
FLASHMEM void selection805ToggleShift() {
  if(remixMode)return;
  bool enable = !shiftButtonActive;
  shiftButtonActive = enable; selectButtonActive = !enable && selectionMode;
  columnSelectionMode = false; if(enable)selectionMode = true;
  rectangleArmed = false;
  selection805Changed(); selection805SongState();
  DISPLAY_SERIAL.println(enable ? F("MSG|SHIFT RECTANGLE ON") : F("MSG|SHIFT OFF"));
}
FLASHMEM void selection805ToggleColumn() {
  if(remixMode)return;
  if (debugTestMode != DEBUG_TEST_NONE || menuNavigationOnly() || deleteButtonActive) return;
  bool enable = !columnSelectionMode;
  columnSelectionMode = enable; shiftButtonActive = false; selectButtonActive = !enable && selectionMode;
  if(enable)selectionMode = true; rectangleArmed = false;
  memset(columnScopes805, 0, sizeof(columnScopes805));
  selection805Changed(); selection805SongState();
  DISPLAY_SERIAL.println(enable ? F("MSG|COL ON") : F("MSG|COL OFF"));
}
FLASHMEM bool selection805VisiblePitch(uint8_t pitch) {
  if (monoMeasureRowsView(selectedTrack)) return true;
  for (uint8_t p = 0; p < 16; p += 4) if (padChromaticNote(p) == pitch) return true;
  return false;
}
FLASHMEM bool selection805VisibleNote(uint16_t step, uint8_t voice) {
  int32_t tick = (int32_t)step * TICKS_PER_STEP + logicalMicroTick(selectedTrack, step, voice);
  if (!selection805VisiblePitch(visualNoteAtVoice(selectedTrack, step, voice))) return false;
  for (uint8_t n = 0; n < NEO_COUNT; ++n) for (uint8_t p = 0; p < 16; ++p) {
    uint32_t begin, end; padTimelineBounds(n, p, begin, end);
    if (tick >= (int32_t)begin && tick < (int32_t)end) return true;
  }
  return false;
}
FLASHMEM void selection805NoteRegion(uint32_t begin, uint32_t end, uint8_t lo, uint8_t hi,
                            bool visiblePitchOnly, int8_t force = -1) {
  bool all = true, any = false;
  for (uint8_t pass = 0; pass < 2; ++pass) {
    for (uint16_t s = 0; s < trackStepCount(selectedTrack); ++s)
      for (uint8_t v = 0; v < noteCountAtStep(selectedTrack, s); ++v) {
        uint8_t pitch = visualNoteAtVoice(selectedTrack, s, v);
        int32_t tick = (int32_t)s * TICKS_PER_STEP + logicalMicroTick(selectedTrack, s, v);
        if (tick < (int32_t)begin || tick >= (int32_t)end || pitch < lo || pitch > hi ||
            (visiblePitchOnly && !selection805VisiblePitch(pitch))) continue;
        if (!pass) { any = true; all &= selectedNoteObject(selectedTrack, s, v); }
        else setNoteObjectSelected(selectedTrack, s, v, force < 0 ? !all : force != 0);
      }
    if (!any) break;
  }
  explicitNoteSelection = true; selectedStep = selectedFineIndex = -1;
}
// A note column is a phase within a bar, across every bar and pitch of this track.
FLASHMEM void selectionColumn20(uint32_t begin,uint32_t end,bool selected){
  const int32_t bar=STEPS_PER_BAR*TICKS_PER_STEP;
  uint32_t start=begin%bar,width=end-begin;
  for(uint16_t step=0;step<trackStepCount(selectedTrack);++step)
    for(uint8_t voice=0;voice<noteCountAtStep(selectedTrack,step);++voice){
      int32_t phase=((int32_t)step*TICKS_PER_STEP+logicalMicroTick(selectedTrack,step,voice))%bar;
      if(phase<0)phase+=bar;
      if(width>=(uint32_t)bar || ((phase+bar-start)%bar)<width)setNoteObjectSelected(selectedTrack,step,voice,selected);
    }
  explicitNoteSelection=true;selectedStep=selectedFineIndex=-1;
}
FLASHMEM void selection805PatternRegion(uint8_t t0, uint8_t t1, uint8_t p0, uint8_t p1, int8_t force = -1) {
  bool all = true;
  for (uint8_t t = t0; t <= t1; ++t) for (uint8_t p = p0; p <= p1; ++p)
    if (songPatternPresent(t, patternAtRow51(t,p))) all &= patternSelected[t][patternAtRow51(t,p)];
  for (uint8_t t = t0; t <= t1; ++t) for (uint8_t p = p0; p <= p1; ++p)
    if (songPatternPresent(t, patternAtRow51(t,p))) patternSelected[t][patternAtRow51(t,p)] = force < 0 ? !all : force != 0;
}



FLASHMEM bool selection805GrooveClick(uint8_t ky) {
  if(ky>=GROOVE_LIST_SIZE)return false;grooveListFocus805=ky;
  if(shiftButtonActive){if(grooveListAnchor805<0){grooveListAnchor805=ky;DISPLAY_SERIAL.println(F("MSG|LIST SECOND ENTRY"));}
    else {uint16_t mask=0;for(uint8_t i=min((uint8_t)grooveListAnchor805,ky);i<=max((uint8_t)grooveListAnchor805,ky);++i)mask|=1u<<i;
      if((grooveListSelection805&mask)==mask)grooveListSelection805&=~mask;else grooveListSelection805|=mask;grooveListAnchor805=-1;}
    sendGrooveListPacket();return true;
  }
  if(selectButtonActive){grooveListSelection805^=1u<<ky;sendGrooveListPacket();return true;}return false;
}
FLASHMEM void selection805All() {
  if (remixMode) return;
  uint8_t context = selection805Context();
  if (selectionAllContext != context) { selectionAllCycle = 0; selectionAllContext = context; }
  uint8_t scope = selectionAllCycle;
  selectionAllCycle = (selectionAllCycle + 1) % 4;
  selectButtonActive = selectionMode = true; shiftButtonActive = columnSelectionMode = false; rectangleArmed = false;
  if (context == 5) { songState(); songSend("SELALL,%u", scope); return; }
  // Cycling selection must not validate the current edit draft.
  if (context == 0) {
    clearStepSelection(); explicitNoteSelection = true;
    uint8_t pitch = activePitchTrack == selectedTrack ? activePitchRow : defaultMidiNote[selectedTrack];
    for (uint16_t s = 0; s < trackStepCount(selectedTrack); ++s)
      for (uint8_t v = 0; v < noteCountAtStep(selectedTrack, s); ++v) {
        if ((scope & 1) && !selection805VisibleNote(s, v)) continue;
        if (scope >= 2 && visualNoteAtVoice(selectedTrack, s, v) != pitch) continue;
        setNoteObjectSelected(selectedTrack, s, v, true);
      }
    selectedStep = selectedFineIndex = -1;
  } else if (context == 3) {
    uint8_t t=selectedTrack,p=activePattern[t];bool found=false;
    for(uint8_t a=0;a<16&&!found;++a)for(uint8_t b=0;b<16&&!found;++b)if(patternSelected[a][b]){t=a;p=b;found=true;}
    grooveListSelection805=0;const auto*list40=listEditRead52(listTrack40,listPattern40);uint8_t sheet=list40[grooveListFocus805].sheet;
    for(uint8_t i=0;i<GROOVE_LIST_SIZE;++i){uint8_t value=list40[i].sheet;if(!value)break;
      if(scope<2||value==sheet)grooveListSelection805|=1u<<i;}
    sendGrooveListPacket();
  } else if (context == 1) {
    clearPatternSelection();
    uint8_t t0 = scope >= 2 ? lastPatternTrack : 0;
    uint8_t t1 = scope >= 2 ? lastPatternTrack : TRACK_COUNT - 1;
    uint8_t p0 = scope & 1 ? patternRowOffset : 0;
    uint8_t p1 = scope & 1 ? min((int)PATTERN_COUNT - 1, (int)patternRowOffset + 3) : PATTERN_COUNT - 1;
    selection805PatternRegion(t0, t1, p0, p1, 1);
  } else if (context == 2) {
    grooveStepMask = 0;
    uint8_t field = grooveFeedbackField >= 0 ? grooveFeedbackField : 0;
    for (uint8_t s = 0; s < GROOVE_STEPS; ++s)
      if (scope < 2 || (field == 2 ? grooveGate51(editedGroove, s) != 100 : grooveOffset[editedGroove - 1][s][field] != 0)) grooveStepMask |= 1u << s;
  }
  selectionMode = true;
  selection805Changed();
  DISPLAY_SERIAL.print(F("MSG|SELECT ALL ")); DISPLAY_SERIAL.println(scope + 1);
}
FLASHMEM bool selection805Pad(uint8_t n, uint8_t p, uint8_t edge) {
  if (debugTestMode != DEBUG_TEST_NONE || menuNavigationOnly() || deleteButtonActive || songMode) return false;
  if (!shiftButtonActive && !columnSelectionMode) return false;
  uint8_t context = selection805Context();
  if (context == 4) return false;
  neoPadDown[n][p] = edge == SEESAW_KEYPAD_EDGE_RISING;
  neoPadEdited[n][p] = neoPadDown[n][p];
  if (!neoPadDown[n][p]) return true;
  uint32_t begin = 0, end = 0;
  uint8_t pitch = padChromaticNote(p), track = selectedTrack;
  if (context == 1 || context == 3) { track = n * 4 + p % 4; begin = patternRowOffset + p / 4; end = begin + 1; }
  else if (context == 2) { if (p >= 4) return true; begin = n * 4 + p; end = begin + 1; }
 
  else padTimelineBounds(n, p, begin, end);
  uint32_t lo = begin, hi = end;
  uint8_t t0 = track, t1 = track, p0 = pitch, p1 = pitch;
  int8_t force = -1; bool visibleOnly = false;
  if (columnSelectionMode) {
    // Note columns repeat by bar; other contexts retain their own coordinates.
    uint32_t keyBegin = (context == 1 || context == 3) ? 0 : context==0 ? begin%(STEPS_PER_BAR*TICKS_PER_STEP) : begin;
    ColumnScope805 *scope = nullptr, *empty = nullptr;
    for (auto &s : columnScopes805) {
      if (!s.stage && !empty) empty = &s;
      if (s.stage && s.context == context && s.track == track && s.begin == keyBegin && (context!=0 || s.end==end-begin)) { scope = &s; break; }
    }
    if (!scope) scope = empty;
    if (!scope) { DISPLAY_SERIAL.println(F("MSG|32 COLUMNS MAX")); return true; }
    uint8_t stage = scope->stage + 1;
    *scope = {keyBegin, context==0?end-begin:end, track, context, (uint8_t)(stage == (context==0?2:3) ? 0 : stage)};
    force = stage == (context==0?2:3) ? 0 : 1;
    if (context == 1 || context == 3) { lo = stage == 1 ? patternRowOffset : 0; hi = stage == 1 ? patternRowOffset + 4 : PATTERN_COUNT; }
    else { p0 = 0; p1 = 127; visibleOnly = stage == 1; }
  } else {
    if (!rectangleArmed || rectangleContext != context || (context == 4 && rectangleTrack != track)) {
      rectangleArmed = true; rectangleContext = context; rectangleBegin = begin; rectangleEnd = end;
      rectanglePitch = pitch; rectangleTrack = track; rectangleNeo = n; rectanglePad = p;
      DISPLAY_SERIAL.println(F("MSG|RECTANGLE SECOND PAD")); selection805Changed(); return true;
    }
    lo = min(begin, rectangleBegin); hi = max(end, rectangleEnd);
    t0 = min(track, rectangleTrack); t1 = max(track, rectangleTrack);
    p0 = min(pitch, rectanglePitch); p1 = max(pitch, rectanglePitch);
    rectangleArmed = false;
  }
  if (context == 1 || context == 3) {
    selection805PatternRegion(t0, t1, lo, min((uint32_t)PATTERN_COUNT, hi) - 1, force);
    lastPatternTrack = selectedTrack = track; lastPatternIndex = patternAtRow51(track,begin); lastPatternValid = true;
  } else if (context == 2) {
    uint16_t mask = 0; for (uint32_t s = lo; s < hi && s < GROOVE_STEPS; ++s) mask |= 1u << s;
    if (force == 0 || (force < 0 && (grooveStepMask & mask) == mask)) grooveStepMask &= ~mask;
    else grooveStepMask |= mask;
  } else if(context==0 && columnSelectionMode){selectionColumn20(lo,hi,force!=0);
  } else { if(monoMeasureRowsView(selectedTrack)){p0=0;p1=127;} selection805NoteRegion(lo, hi, p0, p1, visibleOnly, force); }
  selection805Changed(); return true;
}
