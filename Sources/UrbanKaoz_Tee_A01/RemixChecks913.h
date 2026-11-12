#pragma once
FLASHMEM void remixChecks913(){
 if(playing||remixMode||remixHistory911.count||remixSource911.notes.size||remixDraft911.notes.size||menuNavigationOnly()){Serial.println("CHECK913,BUSY");return;}
 unsigned passed=0,failed=0;auto check=[&](bool ok,const char*label){if(ok)++passed;else ++failed;Serial.printf("CHECK913,%s,%s\n",ok?"PASS":"FAIL",label);};
 auto oldTick=transportTickAbsolute;auto oldStart=loopStartStep,oldEnd=loopEndStep;uint16_t oldBars[16];memcpy(oldBars,trackBars,sizeof(oldBars));auto oldTrack=selectedTrack;auto oldQv=quickView903;
 uint16_t oldHear=remixHearBar911;uint8_t oldWait=remixWait913,oldQuan=remixQuan913,oldLayout=remixLayout913;
 rmx911::Model source,draft;source.swap(remixSource911);draft.swap(remixDraft911);
 for(uint8_t t=0;t<16;++t)trackBars[t]=4;loopStartStep=1;loopEndStep=64;
 (pidiStateDirty931=true, remixMode=remixReady911=true);quickView903=0;remixHearBar911=4;remixWait913=3;remixLayout913=0;
 remixSource911.mask=1;remixSource911.nextId=3;remixSource911.notes.add({1,0,96,0,0,60,90,0});remixSource911.notes.add({2,48,96,0,0,62,80,0});
 remixDraft911.mask=1;remixDraft911.nextId=2;remixDraft911.notes.add({1,768,96,0,0,72,100,0});
 // No clock is started and no MIDI is emitted: borrow the transport position only.
 (pidiStateDirty931=true, playing=true);recordToggle820();check(remixRecord913,"Record_button_arms_Remix");
 transportTickAbsolute=2*1536+1500;check(remixTrack911(0,0),"WAIT_bypasses_source_pattern");
 remixRecordNote913(remixSource911,0,0);check(remixDraft911.notes.size==1,"WAIT_does_not_record_pad_notes");
 // One press one tick late: later notes retain their offset, not independent quantization.
 remixQuantShift913=rmx913::quantShift(1,96);transportTickAbsolute=3*1536+1;remixSessionService913();remixRecordNote913(remixSource911,0,0);
 transportTickAbsolute=3*1536+49;remixRecordNote913(remixSource911,1,0);
 check(remixDraft911.notes.size==3&&remixDraft911.notes[1].tick==0&&remixDraft911.notes[2].tick==48,"recorded_fragment_preserves_48_tick_spacing");
 check(remixDraft911.notes[0].pitch==72,"overdub_preserves_previous_notes");
 check(remixRecorded913.size==2,"new_attacks_suppressed_for_current_pass");
 transportTickAbsolute=4*1536;remixSessionService913();check(!remixTakeBefore913&&remixRecorded913.size==0,"bar_boundary_commits_take");
 remixRecordNote913(remixSource911,0,0);check(remixDraft911.notes.size==3,"source_or_WAIT_bar_not_recorded");
 (pidiStateDirty931=true, playing=false);recordToggle820();check(!remixRecord913,"Record_button_disarms_Remix");
 dispatchStableButtonPress(EDIT_BUTTON_BITS[EDIT_BUTTON_UNDO]);check(remixDraft911.notes.size==1,"undo_recorded_pass");dispatchStableButtonPress(EDIT_BUTTON_BITS[EDIT_BUTTON_REDO]);check(remixDraft911.notes.size==3,"redo_recorded_pass");
 dispatchStableButtonPress(EDIT_BUTTON_BITS[EDIT_BUTTON_DELETE]);check(remixDelete913&&deleteButtonActive,"Delete_toggles_mode");
 onPadEdge(1,0,SEESAW_KEYPAD_EDGE_RISING);onPadEdge(1,0,SEESAW_KEYPAD_EDGE_FALLING);check(remixDraft911.notes.size==2,"Delete_pad_removes_only_its_step");
 onPadEdge(0,0,SEESAW_KEYPAD_EDGE_RISING);onPadEdge(0,0,SEESAW_KEYPAD_EDGE_FALLING);check(remixSource911.notes.size==2,"Delete_keeps_source_intact");
 p23BackOrRecover();check(!remixDelete913&&!deleteButtonActive,"J1_exits_Delete");dispatchStableButtonPress(EDIT_BUTTON_BITS[EDIT_BUTTON_UNDO]);check(remixDraft911.notes.size==3,"undo_Delete_pad");
 remixKey911(10,true);remixKey911(10,false);check(remixClearAsk913&&!remixClearYes913,"Clear_confirmation_defaults_NO");remixKey911(10,true);remixKey911(10,false);check(remixDraft911.notes.size==3&&!remixClearAsk913,"Clear_NO_preserves_draft");
 remixKey911(10,true);remixKey911(10,false);remixKy911(10,1);remixKey911(10,true);remixKey911(10,false);check(remixDraft911.notes.size==0,"Clear_YES_clears_draft");dispatchStableButtonPress(EDIT_BUTTON_BITS[EDIT_BUTTON_UNDO]);check(remixDraft911.notes.size==3,"undo_Clear");
 remixKy911(2,10);check(remixWait913==3,"WAIT_clamped_to_HEAR_minus_one");remixKy911(1,-3);check(remixHearBar911==1&&remixWait913==0,"lower_HEAR_clamps_WAIT");
 remixKy911(9,2);check(remixQuan913==2,"QUAN_1_16");remixKy911(12,1);check(remixLayout913==1&&remixLayoutPreview913,"K13_opens_layout_preview");
 remixLayoutUntil913=millis();remixSessionService913();check(!remixLayoutPreview913,"preview_returns_after_deadline");
 remixFinishTake913();remixRecord913=remixDelete913=deleteButtonActive=false;remixClearAsk913=false;remixKeys911=0;remixHistory911.clear();remixSource911.swap(source);remixDraft911.swap(draft);remixDue911.clear();remixRecorded913.clear();remixAudition911.active=false;remixQuantShift913=0;remixRecordCycle913=UINT64_MAX;
 (pidiStateDirty931=true, remixMode=remixReady911=false);remixLayout913=oldLayout;remixHearBar911=oldHear;remixWait913=oldWait;remixQuan913=oldQuan;quickView903=oldQv;selectedTrack=oldTrack;transportTickAbsolute=oldTick;loopStartStep=oldStart;loopEndStep=oldEnd;memcpy(trackBars,oldBars,sizeof(oldBars));remixScheduleDirty911=true;remixDisplay911(true);showGameScreen();
 Serial.printf("CHECK913,RESULT,%u,%u,NO_SD_WRITE\n",passed,failed);
}
