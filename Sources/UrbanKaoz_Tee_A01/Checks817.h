#pragma once
EXTMEM NoteLock817 checkLockBackup817[LOCK_CAPACITY817];
EXTMEM PatternImage805 checkPattern817;
FLASHMEM void checks817(){
 if(playing||lockLeaseCount817||!trackContext815()||selectionHasActiveNotes()||selectedStep>=0){Serial.printf("CHECK817,BUSY,PLAY=%u,LEASES=%u,CONTEXT=%u,SELECT=%u,STEP=%d\n",playing,lockLeaseCount817,trackContext815(),selectionHasActiveNotes(),selectedStep);return;}
 uint16_t pass=0,fail=0;auto check=[&](bool ok,const char*name){Serial.printf("CHECK817,%s,%s\n",ok?"OK":"FAIL",name);if(ok)++pass;else ++fail;};
 lockTest817=true;lockTestSends817=0;uint32_t epoch=modEpoch805;
 lockLeases817[0]={micros()+1000000,0,1,20,7,0,74,LOCK_CC817,0,60,true,false};
 lockLeases817[1]={micros()+1000000,0,2,90,7,0,74,LOCK_CC817,0,62,true,false};lockLeaseCount817=2;
 check(lockValue817(LOCK_CC817,0,74,3)==90,"overlap_last_note");
 lockDrop817(0);check(lockTestSends817==0&&lockValue817(LOCK_CC817,0,74,3)==90,"older_end_preserves_newer");
 lockDrop817(1);check(lockTestSends817==1&&lockTestLast817==7,"restore_previous_cc");
 lockLeases817[0]={micros()+1000000,0,3,22,8,0,74,LOCK_CC817,0,60,true,false};
 lockLeases817[1]={micros()+1000000,0,4,99,8,0,74,LOCK_CC817,0,62,true,false};lockLeaseCount817=2;
 lockDrop817(1);check(lockTestLast817==22,"newer_end_restores_older");
 check(lockMidiOffer817(0,74,41)==22,"automation_cannot_overwrite_lock");lockDrop817(0);check(lockTestLast817==41,"automation_base_restored");
 lockLeases817[0]={micros()+1000000,1000000,5,55,10,0,74,LOCK_CC817,0,60,true,false};lockLeaseCount817=1;
 lockOff817(0,60);check(lockLeaseCount817==1&&lockLeases817[0].released,"release_survives_note_off");
 lockLeases817[0].until=micros()-1;lockService817();check(!lockLeaseCount817&&lockTestLast817==10,"release_expiry_restores");
 lockLeases817[0]={micros()+1000000,0,6,55,10,0,74,LOCK_CC817,0,60,true,false};lockLeaseCount817=1;
 lockOff817(0,60);check(!lockLeaseCount817,"gate_end_restores");
 lockLeases817[0]={micros()+1000000,0,7,999,0,0,0,LOCK_MOD817,0,60,true,false};lockLeaseCount817=1;
 check(lockValue817(LOCK_MOD817,0,0,100)==999,"internal_overlay");lockStop817();check(lockValue817(LOCK_MOD817,0,0,100)==100,"internal_base_unchanged");
 NoteLock817 good={96,3072,0,1,0,0,60,LOCK_DELAY817};check(lockValid817(good),"delay_lock_valid");good.value=0;check(!lockValid817(good),"delay_length_zero_rejected");
 check(clampInt(7+16,1,64)==23&&clampInt(29-16,1,64)==13,"measure_additive_not_snap");
 uint8_t savedTrack=selectedTrack,p=activePattern[15],savedVoice=selectedVoice;
 uint16_t savedCount=lockCount817;memcpy(checkLockBackup817,noteLocks817,sizeof(noteLocks817));
 NoteLock817 savedUndo[32];memcpy(savedUndo,lockUndo817[0],sizeof(savedUndo));uint8_t savedUndoCount=lockUndoCount817[0];
 pattern805CaptureLive(15,checkPattern817);
 auto savedDelay=delays805[15][p];auto savedExtra=delayExtra817[15][p];
 uint8_t savedOwner=modOwnerTrack805,savedPattern=modOwnerPattern805;
 uint8_t savedModes[2]={delayTimeMode817[0],delayTimeMode817[1]};
 bool savedExplicit=explicitNoteSelection,savedSong=songEngineActive,savedRoll=rollTrigger817;
 uint64_t savedTick=transportTickAbsolute;
 lockCount817=0;selectedTrack=15;selectedStep=0;selectedVoice=0;
 memset(stepActive[15],0,sizeof(stepActive[15]));memset(fineNote[15],0,sizeof(fineNote[15]));
 stepActive[15][0]=true;stepExtraNoteCount[15][0]=0;stepLogicalMicro[15][0]=0;stepMidiNote[15][0]=60;stepLockPitch[15][0]=true;
 stepSelected[15][0]=true;selectedBaseMask[15][0]=1;explicitNoteSelection=true;
 check(lockEdit817(LOCK_DELAY817,15*16+p,1,100,96,1,3072)&&lockCount817==1&&noteLocks817[0].value==196,"edit_selected_note_creates_lock");
 check(delays805[15][p].ticks==savedDelay.ticks,"lock_edit_does_not_change_base");
 lockBegin817(15,p,0,60,15,60,100000);
 check(lockValue817(LOCK_DELAY817,15*16+p,1,100)==196,"real_note_on_applies_lock");
 lockStop817();check(lockValue817(LOCK_DELAY817,15*16+p,1,100)==100,"stop_restores_internal_parameter");
 lockPitch817(15,0,0,61);check(noteLocks817[0].note==61,"pitch_edit_tracks_lock");noteLocks817[0].note=60;
 lockUndoCapture817(0,15,0,0);lockRemoveNote817(15,0,0);check(lockCount817==0,"delete_removes_lock");lockUndoInstall817(0);check(lockCount817==1&&noteLocks817[0].value==196,"undo_restores_lock");
 lockMove817(15,0,60,19);check(noteLocks817[0].tick==19,"logical_move_tracks_lock");
 lockCount817=0;selectedStep=-1;explicitNoteSelection=false;stepSelected[15][0]=false;selectedBaseMask[15][0]=0;
 modOwnerTrack805=15;modOwnerPattern805=p;modUi805=MOD_DELAY806;delays805[15][p]={101,3,127,1};delayExtra817[15][p]={7,4,0};
 delayTimeMode817[0]=0;delayTimeMode817[1]=1;delayEdit817(1,3);
 check(delays805[15][p].ticks==389,"three_KY_crans_preserved");
 // Delay Roy mapping and local scheduling are covered by the V908 host harness.
 pattern805ApplyLive(15,checkPattern817);
 delays805[15][p]=savedDelay;delayExtra817[15][p]=savedExtra;
 lockCount817=savedCount;memcpy(noteLocks817,checkLockBackup817,sizeof(noteLocks817));
 memcpy(lockUndo817[0],savedUndo,sizeof(savedUndo));lockUndoCount817[0]=savedUndoCount;
 selectedTrack=savedTrack;selectedVoice=savedVoice;selectedStep=-1;explicitNoteSelection=savedExplicit;
 modOwnerTrack805=savedOwner;modOwnerPattern805=savedPattern;modUi805=0;delayTimeMode817[0]=savedModes[0];delayTimeMode817[1]=savedModes[1];
 mod805Invalidate();followTrack817();showGameScreen();
 uint8_t oldType=modType805;uint16_t oldKey=modSelected806;
 mod805Open(1);modSelected806=0;modUi805=MOD_DEST806;
 selectEditingTrack((savedTrack+1)%16);followTrack817();
 check(modUi805==MOD_LIST806&&modType805==1&&modOwnerTrack805==selectedTrack&&modSelected806==UINT16_MAX,"track_change_from_mod_output_returns_home");
 modUi805=0;selectEditingTrack(savedTrack);followTrack817();modType805=oldType;modSelected806=oldKey;modOwnerTrack805=savedOwner;modOwnerPattern805=savedPattern;
 lockSettingsOpen817();patch808Screen(6);check(patchUi808==10,"sequence_plocks_button_opens_setting");patchUi808=0;showGameScreen();
 lockTest817=false;modEpoch805=epoch+1;
 Serial.printf("CHECK817,%s,PASS=%u,FAIL=%u\n",fail?"FAIL":"PASS",pass,fail);
}
FLASHMEM void checksFiles817(){
 if(playing||!storageWriteAvailable||lockLeaseCount817){Serial.printf("CHECK817FILES,BUSY,PLAY=%u,SDWRITE=%u,LEASES=%u\n",playing,storageWriteAvailable,lockLeaseCount817);return;}
 const char*path="/URBANKAOZ/PRJ/.817TEST.UKX";
 const char*fx="/URBANKAOZ/PRJ/.817FX.UFX";
 if(SD.exists(path)||SD.exists(fx)){Serial.println("CHECK817FILES,FILES_EXIST");return;}
 uint16_t count=lockCount817;memcpy(checkLockBackup817,noteLocks817,sizeof(noteLocks817));auto extra=delayExtra817[15][15];
 lockCount817=2;noteLocks817[0]={96,74,15,74,15,15,60,LOCK_CC817};noteLocks817[1]={96,307,255,0,15,15,60,LOCK_DELAY817};delayExtra817[15][15]={19,4,0};
 File f=SD.open(path,FILE_WRITE);bool ok=f&&full805Append(f);f.close();
 f=SD.open(path,FILE_READ);FullExtra805 h={};if(ok)ok=f&&full805ReadExtra(f,h)&&h.magic==0x817554Bu&&fullLockCount817==2&&!memcmp(noteLocks817,fullLocks817,2*sizeof(NoteLock817))&&!memcmp(delayExtra817,fullDelayExtra817,sizeof(delayExtra817));f.close();
 Serial.printf("CHECK817FILES,%s,PROJECT_LOCKS_DELAY_ROUNDTRIP\n",ok?"OK":"FAIL");
 bool corrupt=false;if(ok){f=SD.open(path,FILE_WRITE_BEGIN);uint8_t bad=0;corrupt=f&&f.seek(1)&&f.write(&bad,1)==1;f.close();f=SD.open(path,FILE_READ);if(corrupt)corrupt=f&&!full805ReadExtra(f,h);f.close();}
 Serial.printf("CHECK817FILES,%s,CORRUPTION_REJECTED\n",corrupt?"OK":"FAIL");
 Item809 original,loaded;original.kind=ITEM_FX809;original.delay={197,5,101,1};original.delayExtra={31,4,0};
 f=SD.open(fx,FILE_WRITE);bool fxOk=f&&itemWrite809(f,original);f.close();f=SD.open(fx,FILE_READ);if(fxOk)fxOk=f&&itemRead809(f,loaded,ITEM_FX809)&&loaded.delay.ticks==197&&loaded.delay.velocity==101&&loaded.delayExtra.wait==31&&loaded.delayExtra.output==4&&!loaded.delayExtra.rolls;f.close();itemFree809(loaded);
 Serial.printf("CHECK817FILES,%s,FX_ROUNDTRIP\n",fxOk?"OK":"FAIL");
 SD.remove(path);SD.remove(fx);lockCount817=count;memcpy(noteLocks817,checkLockBackup817,sizeof(noteLocks817));delayExtra817[15][15]=extra;
 Serial.printf("CHECK817FILES,%s\n",ok&&corrupt&&fxOk?"PASS":"FAIL");
}
