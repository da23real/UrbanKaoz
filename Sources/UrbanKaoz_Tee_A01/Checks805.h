#pragma once
EXTMEM PatternImage805 checkSaved805[2],checkCache805[2],checkFixture805;
FLASHMEM uint32_t checkSelectionCount805(uint8_t t){uint32_t n=0;for(uint16_t s=0;s<trackStepCount(t);++s)for(uint8_t v=0;v<noteCountAtStep(t,s);++v)if(selectedNoteObject(t,s,v))++n;return n;}
FLASHMEM void runChecks805() {
  if(playing||menuNavigationOnly()||songMode||songEngineActive||patternMode||grooveEditMode||remixMode||pendingSelectionEdit.active||selectedStep>=0||hasSelectedSteps()||selectedPatternCount()||external_psram_size<16){Serial.println("CHECK805,BUSY");return;}
  uint32_t count=0,failed=0;
  auto check=[&](bool ok,const char*name){++count;if(!ok){++failed;Serial.print("CHECK805,FAIL,");Serial.print(name);Serial.print(",SELECTED=");Serial.println(checkSelectionCount805(0));}};
  uint8_t oldTrack=selectedTrack,oldPitch=chromaticBottomNote,oldActive[2]={activePattern[0],activePattern[1]};
  uint8_t oldDefault[2]={defaultVel[0],defaultMidiNote[0]};uint16_t oldGate=defaultGateTicks[0];int16_t oldMicro=defaultMicroTick[0];
  auto oldView=trackNoteViewMode[linkOwner12(0,LINK_VIEW12)];auto oldPlay=trackPlayMode[linkOwner12(0,LINK_POLY12)];int8_t oldZoom=zoomLevel;uint32_t oldViewTick=viewTickOffset;
  uint64_t oldTick=transportTickAbsolute;uint32_t oldPhase[2]={grooveTrackPhase[0],grooveTrackPhase[1]};
  uint16_t oldLoops[4]={trackLoopStartStep[0],trackLoopEndStep[0],trackLoopStartStep[1],trackLoopEndStep[1]};
  bool oldMuted[2]={trackMuted[0],trackMuted[1]},oldDirty[2]={patternDirty805[0][0],patternDirty805[1][0]};
  uint8_t oldGroove[2]={patternGroove[0][0],patternGroove[1][0]},oldEdited=editedGroove;
  int8_t oldOffsets[16][5];memcpy(oldOffsets,grooveOffset[0],sizeof(oldOffsets));
  uint16_t oldAssigned[16];memcpy(oldAssigned,grooveAssigned805,sizeof(oldAssigned));
  ModMeta806 oldMeta=modMeta806[32];ModUnit805 oldMod=modUnits805[32];uint8_t oldLastPitch=activePitchRow,oldLastPitchTrack=activePitchTrack;
  for(uint8_t t=0;t<2;++t){pattern805CaptureLive(t,checkSaved805[t]);if(oldDirty[t])unpackPattern906(patterns906[t][0],checkCache805[t]);}
  memset(&checkFixture805,0,sizeof(checkFixture805));checkFixture805.bars=4;
  checkFixture805.active[0]=checkFixture805.active[16]=true;checkFixture805.note[0]=checkFixture805.note[16]=60;
  checkFixture805.lockPitch[0]=checkFixture805.lockPitch[16]=true;checkFixture805.extraCount[0]=1;checkFixture805.extraNote[0][0]=64;
  checkFixture805.fine[0].active=true;checkFixture805.fine[0].step=2;checkFixture805.fine[0].note=60;checkFixture805.fine[0].pxMask=PX_PITCH;
  for(uint8_t t=0;t<2;++t){pattern805ApplyLive(t,checkFixture805);activePattern[t]=0;trackLoopStartStep[t]=1;trackLoopEndStep[t]=64;trackMuted[t]=false;grooveTrackPhase[t]=0;patternGroove[t][0]=0;patternDirty805[t][0]=false;}
  selectedTrack=0;trackNoteViewMode[linkOwner12(0,LINK_VIEW12)]=NOTE_VIEW_CHROMATIC;trackPlayMode[linkOwner12(0,LINK_POLY12)]=TRACK_PLAY_MULTI;chromaticBottomNote=60;viewTickOffset=0;zoomLevel=0;
  defaultVel[0]=100;defaultGateTicks[0]=384;defaultMicroTick[0]=0;defaultMidiNote[0]=60;activePitchRow=60;activePitchTrack=0;transportTickAbsolute=0;
  selection805Reset();clearStepSelection();selection805All();check(checkSelectionCount805(0)==4,"SelectAll all voices");
  selection805All();check(checkSelectionCount805(0)==2,"SelectAll visible notes");selection805All();check(checkSelectionCount805(0)==3,"SelectAll last pitch");selection805All();check(checkSelectionCount805(0)==2,"SelectAll visible last pitch");
  clearStepSelection();columnSelectionMode=true;shiftButtonActive=false;
  for(uint8_t stage=0;stage<2;++stage){selection805Pad(0,12,SEESAW_KEYPAD_EDGE_RISING);selection805Pad(0,12,SEESAW_KEYPAD_EDGE_FALLING);check(checkSelectionCount805(0)==(stage==0?3:0),"COL all bars and pitches/off");}
  columnSelectionMode=false;shiftButtonActive=true;rectangleArmed=false;
  selection805Pad(0,12,SEESAW_KEYPAD_EDGE_RISING);selection805Pad(0,12,SEESAW_KEYPAD_EDGE_FALLING);chromaticBottomNote=62;
  selection805Pad(0,6,SEESAW_KEYPAD_EDGE_RISING);selection805Pad(0,6,SEESAW_KEYPAD_EDGE_FALLING);check(checkSelectionCount805(0)==3,"Shift time/pitch rectangle");
  clearStepSelection();shiftButtonActive=false;selectionMode=false;
  patternMode=true;patternSelected[0][0]=true;pattern805Edit(3,7);check(defaultVel[0]==100&&stepLockVel[0][0]&&effectiveVel(0,0)==107&&effectiveVel(0,2)==107&&!stepActive[0][55],"Pattern locks preserve defaults and empty cells");
  pattern805Edit(2,-2);check(stepMidiNote[0][0]==62&&stepExtraMidiNote[0][0][0]==66,"Pattern transposition preserves chord interval");
  clearPatternSelection();patternMode=false;clearStepSelection();setNoteObjectSelected(0,0,0,true);explicitNoteSelection=true;selectionMode=true;
  stageSelectedParamDelta(EDIT_VEL,3);commitPendingSelectionEdit();int validated=effectiveVel(0,0);
  clearStepSelection();setNoteObjectSelected(0,2,0,true);explicitNoteSelection=true;stageSelectedParamDelta(EDIT_VEL,9);leaveStepSelection(false);
  check(effectiveVel(0,0)==validated&&effectiveVel(0,2)==107&&!pendingSelectionEdit.active,"J1 preserves validated note, discards remaining draft");
  grooveEditMode=true;editedGroove=1;grooveStepMask=1;memset(grooveAssigned805,0,sizeof(grooveAssigned805));patternGroove[0][0]=patternGroove[1][0]=0;
  editGrooveFromKy(2,1);check(patternGroove[0][0]==1&&patternGroove[1][0]==0,"Groove assigns only active owner");patternGroove[0][0]=0;editGrooveFromKy(2,1);check(patternGroove[0][0]==0,"Explicit Groove OFF remains OFF");grooveEditMode=false;grooveStepMask=0;
  check(fabsf(envelope805(5,100,0,10,0,127,20,0)-0.5f)<0.001f,"HADSR Attack");
  check(fabsf(envelope805(15,5,0,10,0,127,20,0)-0.25f)<0.001f,"Release during Attack preserves instantaneous level");
  check(envelope805(4,100,5,10,10,64,10,0)==0,"Hold before Attack");
  memset(&modMeta806[32],0,sizeof(ModMeta806));mod806Associate(modMeta806[32],1,0,true);
  auto&m=modUnits805[32];memset(&m,0,sizeof(m));m.enabled=1;m.inputs=1;m.sustain=127;m.rate=384;m.outputs[0]={MOD_GAT,1,0,1,96};m.outputs[1]={MOD_TIM,1,0,1,-48};
  mod805Invalidate();check(mod805Offset(1,MOD_GAT,96)==0,"Stopped transport suspends modulation");
  // V8.11: this old diagnostic used to evaluate while stopped. Exercise the
  // playing path explicitly, without starting the hardware clock or any notes.
  playing=true;mod805Invalidate();check(mod805Offset(1,MOD_GAT,96)==96&&mod805Offset(1,MOD_TIM,96)==-48,"Future GAT/TIM sourced from another track");
  trackMuted[1]=true;mod805Invalidate();check(mod805Offset(1,MOD_GAT,96)==0,"Muted owner stops modulation");trackMuted[1]=false;trackMuted[0]=true;mod805Invalidate();check(mod805Offset(1,MOD_GAT,96)==0,"Muted trigger source stops envelope");trackMuted[0]=false;mod805Invalidate();
  uint32_t began=micros();volatile int sink=0;for(uint16_t i=0;i<1000;++i)sink+=mod805Offset(1,MOD_TIM,96);Serial.print("CHECK805,MOD_1000_US,");Serial.println(micros()-began);(void)sink;
  playing=false;
  check(NOTE_UNDO_LEVELS==32&&EDIT_BUTTON_BITS[EDIT_BUTTON_UNDO]==8&&EDIT_BUTTON_BITS[EDIT_BUTTON_REDO]==2,"Undo 32 / B8 / B2");
  for(uint8_t t=0;t<2;++t){pattern805ApplyLive(t,checkSaved805[t]);activePattern[t]=oldActive[t];trackLoopStartStep[t]=oldLoops[t*2];trackLoopEndStep[t]=oldLoops[t*2+1];trackMuted[t]=oldMuted[t];grooveTrackPhase[t]=oldPhase[t];patternGroove[t][0]=oldGroove[t];patternDirty805[t][0]=oldDirty[t];if(oldDirty[t])storePattern906(t,0,checkCache805[t]);else releasePattern906(t,0);}
  memcpy(grooveOffset[0],oldOffsets,sizeof(oldOffsets));memcpy(grooveAssigned805,oldAssigned,sizeof(oldAssigned));editedGroove=oldEdited;modUnits805[32]=oldMod;modMeta806[32]=oldMeta;
  selectedTrack=oldTrack;chromaticBottomNote=oldPitch;trackNoteViewMode[linkOwner12(0,LINK_VIEW12)]=oldView;trackPlayMode[linkOwner12(0,LINK_POLY12)]=oldPlay;zoomLevel=oldZoom;viewTickOffset=oldViewTick;transportTickAbsolute=oldTick;
  defaultVel[0]=oldDefault[0];defaultMidiNote[0]=oldDefault[1];defaultGateTicks[0]=oldGate;defaultMicroTick[0]=oldMicro;activePitchRow=oldLastPitch;activePitchTrack=oldLastPitchTrack;
  clearStepSelection();clearPatternSelection();selectionMode=selectButtonActive=shiftButtonActive=columnSelectionMode=false;selection805Reset();mod805Invalidate();neoDisplayDirty=displayValueDirty=true;
  Serial.print(failed?"CHECK805,FAIL,":"CHECK805,PASS,");Serial.print(count);Serial.print(",FAILURES=");Serial.println(failed);
}
FLASHMEM void runFullCheck805(){
  if(playing||!storageWriteAvailable||external_psram_size<16){Serial.println("FULLCHECK805,BUSY");return;}
  strcpy(full805ForcedPath,"/URBANKAOZ/PRJ/.FULLCHECK805.UKP");SD.remove(full805ForcedPath);char saved[128];bool ok=saveFullProject(saved,sizeof(saved));
  if(ok)ok=full805Validate(saved)&&!memcmp(fullClips805,songClips,sizeof(songClips))&&!memcmp(zlStaging,zlEvents,zlCount*sizeof(ZLEvent))&&!memcmp(fullMods805,modUnits805,sizeof(modUnits805))&&!memcmp(fullDelays805,delays805,sizeof(delays805))&&!memcmp(fullLinks805,virtualLinks805,sizeof(virtualLinks805))&&!memcmp(fullMeta806,modMeta806,sizeof(modMeta806));
  if(ok){File f=SD.open(saved,FILE_WRITE);uint8_t byte=0;if(!f||!f.seek(32)||f.read(&byte,1)!=1)ok=false;else{byte^=0x40;ok=f.seek(32)&&f.write(&byte,1)==1;}f.close();if(ok)ok=!full805Validate(saved);}
  full805ForcedPath[0]=0;SD.remove("/URBANKAOZ/PRJ/.FULLCHECK805.UKP");Serial.println(ok?"FULLCHECK805,PASS,ROUNDTRIP_AND_CORRUPTION":"FULLCHECK805,FAIL");
}
