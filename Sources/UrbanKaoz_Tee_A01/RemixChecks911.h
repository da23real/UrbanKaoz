#pragma once
FLASHMEM void remixStatus911(){Serial.printf("REMIX913,WAIT=%u,QUAN=%u,RECORD=%u,DELETE=%u,LAYOUT=%u,PREVIEW=%u,CLEAR=%u,YES=%u,SHIFT=%d\n",remixWait913,remixQuan913,remixRecord913,remixDelete913,remixLayout913,remixLayoutPreview913,remixClearAsk913,remixClearYes913,remixQuantShift913);
Serial.printf("REMIX911,MODE=%u,READY=%u,PLAY=%u,QV=%u,SOURCE=%u,HEAR=%u,LENGTH=%u,FINE=%u,LAT=%d,CHAIN=%u,HOLD=%u,LOOP=%u,SEL=%d,MASK=%u,SRCNOTES=%lu,DRAFT=%lu,AUDITION=%u,WRITE=%u\n",remixMode,remixReady911,playing,quickView903,remixSourceBar911+1,remixHearBar911,remixLength911,remixFine911,remixLatency911,remixChain911,remixHold911,remixLoop911,remixSelection911.role,remixSelection911.steps,(unsigned long)remixSource911.notes.size,(unsigned long)remixDraft911.notes.size,remixAudition911.active,remixWriteView911);
}
FLASHMEM void remixEditStatus912(){
 uint32_t count=0;const rmx911::Note*first=nullptr;auto&m=remixEditModel911();
 for(uint32_t i=0;i<m.notes.size;++i)if(remixSelection911.contains(m.notes[i],m,remixSelection911.role==0)){++count;if(!first)first=&m.notes[i];}
 Serial.printf("RMXEDIT912,ROLE=%d,COUNT=%lu,NOTE=%d,VEL=%d,GATE=%d,TARGET=%u,SOURCE=%u\n",remixSelection911.role,(unsigned long)count,first?first->pitch:-1,first?first->velocity:-1,first?first->gate:-1,remixTargetMask911,remixSourceMask911);
}
FLASHMEM void remixChecks911(){
 if(playing||pendingSelectionEdit.active||remixEditBefore911||fileBrowserMode!=FILE_BROWSER_NONE){Serial.println("CHECK911,BUSY");return;}
 uint16_t pass=0,fail=0;auto check=[&](bool yes,const char*name){if(yes)++pass;else ++fail;Serial.printf("CHECK911,%s,%s\n",yes?"PASS":"FAIL",name);};
 rmx911::Model m;for(uint8_t t=0;t<16;++t){m.tracks[t].roll[1]=24;m.tracks[t].delay[1]=96;m.tracks[t].delay[4]=t;}
 m.mask=1;m.notes.add({m.nextId++,1531,96,0,0,60,100,0});m.set(m.notes[0],rmx911::Roll,5,101);m.set(m.notes[0],rmx911::CC,74,99);m.set(m.notes[0],rmx911::Delay,6,2);
 auto&a=checkFixture805;memset(&a,0,sizeof(a));a.bars=1;int32_t identity=0;
 check(remixImageNote911(a,m,m.notes[0],1531,&identity),"negative_latency_representation");check(a.active[15]&&15*96+a.logical[15]+a.micro[15]==1531,"wrap_to_last_step");check(a.roll[15].level==101&&(a.rollMask[15]&32),"roll_lock_preserved");
 auto*packed=packPattern906(a);check(packed&&unpackPattern906(packed,patternWork906)&&!memcmp(&a,&patternWork906,sizeof(a)),"pattern_codec_roundtrip");if(packed)extmem_free(packed);
 uint16_t oldBars=remixBars911;remixBars911=1;uint32_t bytes=patternBytes906,rollBytes=rollBytes906;uint16_t locks=lockCount817;
 check(remixInstall911(m,false,15,3,1,0,true,true),"write_pre03_remix01_post00_preflight");check(bytes==patternBytes906&&rollBytes==rollBytes906&&locks==lockCount817,"write_preflight_has_no_commit");
 check(!remixInstall911(m,false,15,63,2,0,true,true),"reject_over_64_bars");
 int32_t oldLimit=allocationLimit906;allocationLimit906=0;check(!remixInstall911(m,false,15,0,1,0,true,true),"allocation_failure_no_commit");allocationLimit906=oldLimit;
 check(bytes==patternBytes906&&rollBytes==rollBytes906&&locks==lockCount817,"failed_write_preserves_patterns_locks");
 memset(&a,0,sizeof(a));a.bars=1;bool represented=true;
 for(uint16_t i=0;i<129;++i){rmx911::Note n{i+1,0,(uint16_t)(i+1),0,0,60,100,0};represented&=remixImageNote911(a,m,n,0);}check(represented,"base_plus_128_fine_notes");rmx911::Note extra{1000,0,200,0,0,60,100,0};check(!remixImageNote911(a,m,extra,0),"polyphony_overflow_explicit");
 PLinkHeader911 h;h.notes=m.notes.size;h.properties=m.properties.size;h.nextId=m.nextId;h.mask=m.mask;h.hash=plinkHash911(h,m);uint32_t hash=h.hash;m.notes[0].velocity=99;check(plinkHash911(h,m)!=hash,"plink_hash_detects_note_change");m.notes[0].velocity=100;m.properties[0].value++;check(plinkHash911(h,m)!=hash,"plink_hash_detects_lock_change");
 rmx911::Selection selected;check(selected.choose(0,3)&&!selected.choose(1,4)&&selected.role==0&&selected.steps==8,"exclusive_source_target_selection");
 remixBars911=oldBars;Serial.printf("CHECK911,RESULT,%u,%u,NO_SD_WRITE\n",pass,fail);
}

// Isolated populated Remix fixture. No pattern, transport, SD or project write.
FLASHMEM void remixChecks912(){
 if(playing||remixMode||remixHistory911.count||remixSource911.notes.size||remixDraft911.notes.size||menuNavigationOnly()){Serial.println("CHECK912,BUSY");return;}
 unsigned pass=0,fail=0;auto check=[&](bool ok,const char*label){if(ok)++pass;else ++fail;Serial.printf("CHECK912,%s,%s\n",ok?"PASS":"FAIL",label);};
 auto oldTrack=selectedTrack;auto oldQv=quickView903;auto oldSource=remixSourceMask911;auto oldTarget=remixTargetMask911;auto oldLength=remixLength911;auto oldLat=remixLatency911;
 rmx911::Model savedSource,savedDraft;savedSource.swap(remixSource911);savedDraft.swap(remixDraft911);
 (pidiStateDirty931=true, remixMode=remixReady911=true);remixSourceMask911=3;remixTargetMask911=3;remixLength911=96;remixLatency911=-14;
 remixSource911.mask=3;remixSource911.notes.add({1,0,96,0,0,60,90,0});remixSource911.notes.add({2,0,96,0,1,64,80,0});remixSource911.nextId=3;
 remixDraft911.mask=3;remixDraft911.notes.add({1,1522,96,0,0,60,90,0});remixDraft911.notes.add({2,96,120,0,1,64,80,0});remixDraft911.nextId=3;
 
 // Exercise the same Select / pad / QV / encoder route as physical controls.
 dispatchStableButtonPress(EDIT_BUTTON_BITS[EDIT_BUTTON_SELECT]);onPadEdge(1,0,SEESAW_KEYPAD_EDGE_RISING);onPadEdge(1,0,SEESAW_KEYPAD_EDGE_FALLING);quickChoose903(1);
 check(remixSelection911.role==1&&remixSelection911.steps==1,"target_selection_routes_to_remix");
 check(remixSelection911.contains(remixDraft911.notes[0],remixDraft911,false),"negative_lat_note_selected_on_nearest_step");
 applyParamDeltaFromKy(3,7);check(remixDraft911.notes[0].velocity==97,"target_velocity_actual_encoder");
 applyParamDeltaFromKy(2,-2);check(remixDraft911.notes[0].pitch==62,"target_note_actual_encoder");
 applyParamDeltaFromKy(4,3);check(remixDraft911.notes[0].gate==99,"target_gate_actual_encoder");
 p23BackOrRecover();check(remixDraft911.notes[0].velocity==90&&remixDraft911.notes[0].pitch==60&&remixDraft911.notes[0].gate==96&&!selectButtonActive,"J1_rolls_back_target");
 dispatchStableButtonPress(EDIT_BUTTON_BITS[EDIT_BUTTON_SELECT]);onPadEdge(1,0,SEESAW_KEYPAD_EDGE_RISING);onPadEdge(1,0,SEESAW_KEYPAD_EDGE_FALLING);applyParamDeltaFromKy(3,7);dispatchStableButtonPress(EDIT_BUTTON_BITS[EDIT_BUTTON_SELECT]);
 check(remixDraft911.notes[0].velocity==97&&!selectButtonActive,"Select_commits_target");dispatchStableButtonPress(EDIT_BUTTON_BITS[EDIT_BUTTON_UNDO]);check(remixDraft911.notes[0].velocity==90,"target_undo");dispatchStableButtonPress(EDIT_BUTTON_BITS[EDIT_BUTTON_REDO]);check(remixDraft911.notes[0].velocity==97,"target_redo");
 onPadEdge(3,0,SEESAW_KEYPAD_EDGE_RISING);onPadEdge(3,0,SEESAW_KEYPAD_EDGE_FALLING);check(remixTargetMask911==2&&remixDraft911.notes.size==2,"mute_preserves_draft");
 dispatchStableButtonPress(EDIT_BUTTON_BITS[EDIT_BUTTON_SELECT]);onPadEdge(1,0,SEESAW_KEYPAD_EDGE_RISING);onPadEdge(1,0,SEESAW_KEYPAD_EDGE_FALLING);applyParamDeltaFromKy(3,1);check(remixDraft911.notes[0].velocity==98,"muted_target_remains_editable");p23BackOrRecover();
 rmx911::Model output;check(rmx911::export912(output,remixDraft911,remixTargetMask911)&&output.mask==2&&output.notes.size==1&&output.notes[0].track==1,"Write_and_PLink_filter_muted_tracks");
 dispatchStableButtonPress(EDIT_BUTTON_BITS[EDIT_BUTTON_SELECT]);onPadEdge(0,0,SEESAW_KEYPAD_EDGE_RISING);onPadEdge(0,0,SEESAW_KEYPAD_EDGE_FALLING);applyParamDeltaFromKy(3,8);check(remixSource911.notes.size==2&&remixSource911.notes[0].velocity==90&&!remixEditBefore911,"source_read_only_for_edit_and_delete");
 onPadEdge(1,0,SEESAW_KEYPAD_EDGE_RISING);onPadEdge(1,0,SEESAW_KEYPAD_EDGE_FALLING);check(remixSelection911.role==0,"source_target_exclusive");dispatchStableButtonPress(EDIT_BUTTON_BITS[EDIT_BUTTON_SELECT]);
 onPadEdge(1,2,SEESAW_KEYPAD_EDGE_RISING);onPadEdge(1,2,SEESAW_KEYPAD_EDGE_FALLING);bool identities=false;for(uint32_t i=0;i<remixDraft911.notes.size;++i)if(remixDraft911.notes[i].tick==178)identities|=remixDraft911.notes[i].track==0;
 check(identities&&remixTargetMask911==2,"paste_keeps_track_identity_despite_target_mute");
 bool cycle=true;for(unsigned t=0;t<4;t++)for(unsigned bar=0;bar<8;bar++)cycle&=rmx911::targetBar912(bar*1536,4,4)==(bar%4==3);check(cycle,"HEAR4_only_common_fourth_bar");
 remixFinishSelection911(false);
releaseRemixClipboard915(3);
clipboardSlots[3].valid=false;
clipboardSlots[3].type=CLIP_TYPE_NONE;
clipboardFineCount[3]=0;
remixHistory911.clear();
remixSource911.swap(savedSource);
remixDraft911.swap(savedDraft);
(pidiStateDirty931=true, remixMode=remixReady911=false);
remixAudition911.active=false;
remixAnchor911=-1;
remixCopySelection911.clear();
remixSelection911.clear();
remixUndoRole911=1;
remixSourceMask911=oldSource;
remixTargetMask911=oldTarget;
remixLength911=oldLength;
remixLatency911=oldLat;
selectedTrack=oldTrack;
quickView903=oldQv;
remixScheduleDirty911=true;
remixDue911.clear();
remixDisplay911(true);
showGameScreen();

 Serial.printf("CHECK912,RESULT,%u,%u,NO_SD_WRITE\n",pass,fail);
}
