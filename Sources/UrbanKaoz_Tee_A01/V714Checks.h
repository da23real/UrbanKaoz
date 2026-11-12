  // Included inside the isolated test fixture, never during normal playback.
  uint32_t failures = 0;
  const uint8_t pattern = activePattern[t];
  GrooveListEntry savedList[GROOVE_LIST_SIZE];
  memcpy(savedList, grooveLists[t][pattern], sizeof(savedList));
  uint8_t savedAssignment = patternGroove[t][pattern];
  uint64_t savedTick = transportTickAbsolute, savedOrigin = grooveListOrigin[t];
  uint16_t savedLoopStart = loopStartStep, savedLoopEnd = loopEndStep;
  uint32_t savedPhase = grooveTrackPhase[t];
  loopStartStep = 1; loopEndStep = STEPS_PER_BAR; grooveTrackPhase[t] = 0;
  GateEditMode savedGateMode = gateEditMode;
  int8_t savedMulti = trackMultiEditMode[t];
  TrackRollSettings savedRoll = trackRoll[t];
  uint8_t savedSheet[GROOVE_STEPS][5];
  memcpy(savedSheet, grooveOffset[GROOVE_COUNT - 1], sizeof(savedSheet));
  uint8_t savedPitch0 = stepMidiNote[t][0], savedPitch1 = stepMidiNote[t][1];

  dispatchStableButtonPress(3);
  if (!grooveEditMode || remixMode) failures |= 1;
  dispatchStableButtonPress(10);
  if (!patternMode || !grooveEditMode) failures |= 2;
  dispatchStableButtonPress(3);
  if (!remixMode || grooveEditMode || !patternMode) failures |= 4;
  dispatchStableButtonPress(3); dispatchStableButtonPress(10);
  if (patternMode || grooveEditMode || remixMode) failures |= 8;

  GrooveListEntry example[GROOVE_LIST_SIZE] = {{1,3},{2,1},{1,3},{3,1}};
  const uint8_t expected[] = {1,1,1,2,1,1,1,3,1};
  for (uint8_t bar = 0; bar < sizeof(expected); ++bar)
    if (listSheetAtBar(example, bar) != expected[bar]) failures |= 16;
  example[1].sheet = 0;
  if (listSheetAtBar(example, 3) != 1 || example[2].sheet != 1 || example[3].sheet != 3) failures |= 32;

  selectButtonActive = true; selectionMode = true; explicitNoteSelection = true;
  setNoteObjectSelected(t, 0, 0, true);
  // No changes during this selection: remove, do not exile.
  toggleNotePadSelection(t, 0, 0);
  if (exiledNoteObject(t, 0, 0) || selectedNoteObject(t, 0, 0)) failures |= 4096;
  toggleNotePadSelection(t, 0, 0);
  if (exiledNoteObject(t, 0, 0) || !selectedNoteObject(t, 0, 0)) failures |= 8192;
  gateEditMode = GATE_EDIT_1T; trackMultiEditMode[t] = 0;
  for (uint8_t i = 0; i < 9; ++i) stageSelectedParamDelta(EDIT_GATE, 1);
  if (audibleGateTicks(t, 0) != 33 || audibleGateTicks(t, 1) != 24 || selectionEditDeltaVisible) failures |= 64;
  grooveTestCapture = true; grooveTestNotes = 0;
  triggerStep(t, 0);
  uint32_t expectedGate = (uint32_t)(33 * tickDurationMs() * 1000.0f);
  if (grooveTestNotes != 1 || grooveTestGateUs != expectedGate) failures |= 128;
  gateEditMode = GATE_EDIT_24T;
  if (audibleGateTicks(t, 0) != 33) failures |= 16384;
  stageSelectedParamDelta(EDIT_GATE, 1);
  if (audibleGateTicks(t, 0) != 57) failures |= 32768;
  toggleNotePadSelection(t, 0, 0);
  if (!exiledNoteObject(t, 0, 0) || audibleGateTicks(t, 0) != 24) failures |= 65536;
  toggleNotePadSelection(t, 0, 0);
  if (audibleGateTicks(t, 0) != 57) failures |= 131072;
  // A selected note longer than one step must reach the actual MIDI trigger.
  for(uint8_t turn=0;turn<10;++turn)stageSelectedParamDelta(EDIT_GATE, 1);
  grooveTestNotes=0;triggerStep(t,0);
  if(audibleGateTicks(t,0)!=297 || grooveTestNotes!=1 ||
     grooveTestGateUs!=(uint32_t)(297*tickDurationMs()*1000.0f))failures|=2097152;
  gateEditMode = GATE_EDIT_1T;
  // The SAME policy must sample the original 24, not its own zeroed draft.
  discardPendingSelectionEdit(); trackMultiEditMode[t] = 1;
  stageSelectedParamDelta(EDIT_GATE, 1);
  if (audibleGateTicks(t, 0) != 25) failures |= 256;
  discardPendingSelectionEdit(); clearStepSelection();
  selectButtonActive = false; selectionMode = false; selectedStep = -1;

  // New selection after cancellation must not inherit a previous edit.
  selectButtonActive = true; selectionMode = true; explicitNoteSelection = true;
  setNoteObjectSelected(t, 0, 0, true); toggleNotePadSelection(t, 0, 0);
  if (exiledNoteObject(t, 0, 0)) failures |= 262144;
  clearStepSelection(); selectButtonActive = false; selectionMode = false; selectedStep = -1;

  uint8_t savedEdited = editedGroove;
  editedGroove = GROOVE_COUNT; grooveEditMode = true;
  grooveStepMask = (1u << 6) | (1u << 14); grooveFeedbackField = -1;
  grooveOffset[GROOVE_COUNT-1][6][3] = -4;
  grooveOffset[GROOVE_COUNT-1][14][3] = -8;
  editGrooveFromKy(5, 1); editGrooveFromKy(5, 1);
  if (grooveFeedbackDelta != 2) failures |= 524288;
  editGrooveFromKy(5, -1); editGrooveFromKy(5, -1);
  if (grooveFeedbackDelta != 0 || grooveOffset[GROOVE_COUNT-1][6][3] != -4 ||
      grooveOffset[GROOVE_COUNT-1][14][3] != -8) failures |= 1048576;
  grooveEditMode = false; grooveStepMask = 0; grooveFeedbackField = -1; editedGroove = savedEdited;

  memset(grooveLists[t][pattern], 0, sizeof(savedList));
  grooveLists[t][pattern][0] = {GROOVE_COUNT, 3};
  patternGroove[t][pattern] = GROOVE_USE_LIST;
  grooveListOrigin[t] = 0;
  memset(grooveOffset[GROOVE_COUNT - 1], 0, sizeof(savedSheet));
  grooveOffset[GROOVE_COUNT - 1][0][3] = -2;
  stepMidiNote[t][0] = 60; stepMidiNote[t][1] = 62;
  trackRoll[t].start = 0; trackRoll[t].level = 0;
  grooveTestNotes = 0;
  transportTickAbsolute = 1534;
  processGrooveListTrack(t, 1534);
  if (grooveTestNotes != 1 || grooveTestPitch != 60) failures |= 512;
  transportTickAbsolute = 1535;
  processGrooveListTrack(t, 1535);
  if (grooveTestNotes != 1) failures |= 1024;
  // A delayed head from the previous measure still uses that measure's sheet.
  grooveOffset[GROOVE_COUNT - 1][0][3] = 2;
  transportTickAbsolute = 1538; grooveTestNotes = 0;
  processGrooveListTrack(t, 2);
  if (grooveTestNotes != 1 || grooveTestPitch != 60) failures |= 2048;
  grooveTestCapture = false;

  memcpy(grooveLists[t][pattern], savedList, sizeof(savedList));
  memcpy(grooveOffset[GROOVE_COUNT - 1], savedSheet, sizeof(savedSheet));
  patternGroove[t][pattern] = savedAssignment;
  grooveListOrigin[t] = savedOrigin; transportTickAbsolute = savedTick;
  loopStartStep = savedLoopStart; loopEndStep = savedLoopEnd; grooveTrackPhase[t] = savedPhase;
  gateEditMode = savedGateMode; trackMultiEditMode[t] = savedMulti;
  trackRoll[t] = savedRoll;
  stepMidiNote[t][0] = savedPitch0; stepMidiNote[t][1] = savedPitch1;
  clearStepSelection(); selectedStep = -1;
  sendSelectionIndicatorsToPicran(); sendGroovePacket();
  Serial.print(failures ? F("V715CHECK,FAIL,MASK=") : F("V715CHECK,PASS,MASK="));
  Serial.println(failures);
