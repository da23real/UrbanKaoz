#pragma once
FLASHMEM void status915(){Serial.printf("STAT915,SRC_PENDING=%u,SEEK=%u,UNDO=%u,REDO=%u,SRC_BUILDS=%lu,DST_BUILDS=%lu,CLIP4=%u,PULS=%d,COLORS=%u/%u/%u/%u,PATCHUI=%u,SETTINGS=%u,LEVEL=%u\n",remixPendingSource915+1,remixSeek915,remixHistory911.cursor,remixHistory911.count-remixHistory911.cursor,(unsigned long)remixSourceBuilds915,(unsigned long)remixTargetBuilds915,clipboardSlots[3].type,settingsValue[1][2],modeColors902[0],modeColors902[1],modeColors902[2],modeColors902[3],patchUi808,settingsOpen,settingsMenuLevel);
}
FLASHMEM void checks915(){
 if(playing||remixMode||remixHistory911.count||remixDraft911.notes.size||menuNavigationOnly()||clipboardSlots[3].valid){Serial.println("CHECK915,BUSY");return;}
 unsigned pass=0,fail=0;auto check=[&](bool ok,const char*name){Serial.printf("CHECK915,%s,%s\n",ok?"PASS":"FAIL",name);ok?++pass:++fail;};
 uint8_t colors[4];memcpy(colors,modeColors902,4);
 showParaScreen();settingsPage=5;settingsMenuLevel=SETTINGS_COLOR_ITEMS;openColorSettingByBe(8);
 check(settingsMenuLevel==SETTINGS_MIXER_EDIT915,"single_MIXER_page");
 for(uint8_t i=0;i<4;++i){royTurn831(3-i,1,0,1);check(modeColors902[i]==min((int)colors[i]+1,NEO_PALETTE_COUNT-1),"four_Roy_independent_colors");}
 p23NavigateMenu(0,0,true);check(!memcmp(colors,modeColors902,4)&&settingsMenuLevel==SETTINGS_COLOR_ITEMS,"J1_rolls_back_all_colors");
 settingsPage=4;settingsMenuLevel=SETTINGS_ITEMS;midi808Open();p23NavigateMenu(0,0,true);check(!patchUi808&&settingsOpen&&settingsPage==4&&settingsMenuLevel==SETTINGS_ITEMS,"MIDI_timing_J1_parent");
 midiConfig810();p23NavigateMenu(0,0,true);check(!patchUi808&&settingsOpen&&settingsPage==4&&settingsMenuLevel==SETTINGS_ITEMS,"MIDI_flow_J1_parent");
 p23NavigateMenu(0,0,true);check(settingsMenuLevel==SETTINGS_CATEGORIES,"MIDI_branch_J1_categories");showGameScreen();
 uint64_t tick=transportTickAbsolute;bool muted=trackMuted[0];uint16_t bars=trackBars[0],length=remixLength911;auto source=remixSourceBar911,pending=remixPendingSource915,mask=remixSourceMask911;int lat=remixLatency911;
 trackMuted[0]=true;trackBars[0]=2;remixMode=remixReady911=true;quickView903=0;remixLength911=96;remixSourceMask911=1;remixSourceBar911=remixPendingSource915=0;remixLatency911=0;
 remixSource911.notes.add({1,0,96,0,0,60,100,0});remixSource911.nextId=2;remixSource911.mask=1;remixSource911.set(remixSource911.notes[0],rmx911::CC,74,93);
 remixSchedule911(remixSource911,remixAuditionDue911,true);remixService911();auto builds=remixSourceBuilds915,dstBuilds=remixTargetBuilds915;
 remixStartAudition911(0);remixStartAudition911(0);check(remixSourceBuilds915==builds,"source_cache_reused_by_pads");
 check(clipboardSlots[3].valid&&clipboardSlots[3].type==CLIP_TYPE_REMIX&&clipboardSlots[3].remix&&clipboardSlots[3].remix->notes.size==1,"shared_slot4_owns_fragment");
 remixAudition911.active=false;remixKy911(12,1);remixCancelPreview913();remixService911();check(remixTargetBuilds915==dstBuilds,"grid_does_not_rebuild_target");
 check(remixPaste911(2)&&remixDraft911.notes.size==1&&remixDraft911.notes[0].tick==192&&remixDraft911.value(remixDraft911.notes[0],rmx911::CC,74)==93,"clipboard_paste_preserves_patch_lock");
 check(remixHistory911.move(remixDraft911,-1)&&remixDraft911.notes.size==0,"delta_undo_paste");check(remixHistory911.move(remixDraft911,1)&&remixDraft911.notes.size==1,"delta_redo_paste");
 remixKy911(0,1);check(remixSourceBar911==0&&remixPendingSource915==1&&remixSourceBuilds915==builds,"SRC_rotation_only_stages");
 remixKey911(0,true);remixKey911(0,false);check(remixSourceBar911==1&&remixSourceBuilds915==builds+1,"SRC_click_rebuilds_once");
 remixRecord913=true;remixKy911(4,1);check(remixSeek915&&!remixRecord913,"seek_disarms_record");remixRecordToggle913();check(!remixRecord913,"seek_excludes_record");
 remixSeekPad915(3);check(transportTickAbsolute==1536+288,"seek_moves_transport_to_source_pad");
 remixSeek915=false;remixMode=false;releaseRemixClipboard915(3);clipboardSlots[3].valid=false;clipboardSlots[3].type=CLIP_TYPE_NONE;
 // Copy through the ordinary route must replace the same shared slot.
 captureTrackIntoClipboard(3);check(clipboardSlots[3].type!=CLIP_TYPE_REMIX&&!clipboardSlots[3].remix,"ordinary_copy_can_overwrite_slot4");
 remixHistory911.clear();rmx911::Model emptySource,emptyDraft;remixSource911.swap(emptySource);remixDraft911.swap(emptyDraft);remixDue911.clear();remixAuditionDue911.clear();remixRecorded913.clear();remixCopySelection911.clear();remixSelection911.clear();remixAnchor911=-1;remixReady911=remixDraftInitialized915=false;remixLayout913=0;remixAudition911.active=false;remixRecordCycle913=UINT64_MAX;
 trackMuted[0]=muted;trackBars[0]=bars;transportTickAbsolute=tick;remixLength911=length;remixSourceBar911=source;remixPendingSource915=pending;remixSourceMask911=mask;remixLatency911=lat;remixScheduleDirty911=true;clipboardSlots[3].valid=false;clipboardSlots[3].type=CLIP_TYPE_NONE;clipboardFineCount[3]=0;showGameScreen();
 Serial.printf("CHECK915,RESULT,%u,%u,NO_SD_WRITE\n",pass,fail);
}
