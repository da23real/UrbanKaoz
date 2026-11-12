#pragma once
FLASHMEM bool remixFinishTake913(){
 if(!remixTakeBefore913)return true;
 bool ok=remixHistory911.push(*remixTakeBefore913,remixDraft911);
 if(!ok){remixDraft911.swap(*remixTakeBefore913);remixRecord913=false;remixError911("REC UNDO MEMORY FULL");}
 remixTakeBefore913->~Model();extmem_free(remixTakeBefore913);remixTakeBefore913=nullptr;
 remixUndoRole911=1;remixScheduleDirty911=true;return ok;
}
FLASHMEM void remixRecordToggle913(){
 if(remixWriteView911||remixBrowser911)return;
 if(remixHit916==rmx916::Chain){remixError911("CHAIN: RECORD OFF");return;}
 if(!remixFinishSelection911(true)||!remixFinishTake913())return;
 remixRecord913=!remixRecord913;
 if(remixDelete913){remixDelete913=deleteButtonActive=false;sendSelectionIndicatorsToPicran();}
 remixClearAsk913=false;remixDisplay911(true);remixError911(remixRecord913?"REMIX REC OVERDUB":"REMIX REC OFF");
}
FLASHMEM void remixRecordNote913(const rmx911::Model&m,uint32_t index,uint8_t rollVelocity,uint8_t rollPitch){
 if(!remixRecord913||!playing||selectButtonActive||remixDelete913||remixWriteView911||remixBrowser911||&m!=&remixSource911)return;
 if(remixRecordCycle913!=transportTickAbsolute/1536){if(!remixFinishTake913())return;remixRecorded913.clear();remixRecordCycle913=transportTickAbsolute/1536;}
 uint64_t now=transportTickAbsolute;if(remixPhase917(now)!=rmx913::Target)return;
 if(!remixTakeBefore913){remixTakeBefore913=rmx911::History::copy(remixDraft911);if(!remixTakeBefore913){remixRecord913=false;remixError911("REC MEMORY FULL");return;}}
 if(!remixRecorded913.reserve(remixRecorded913.size+1)){remixRecord913=false;remixFinishTake913();remixError911("REC MEMORY FULL");return;}
 if(!rmx913::capture(remixDraft911,m,m.notes[index],now%1536,remixQuantShift913,rollVelocity,delayBaseSlot924(m.notes[index].track,0,0,5))){remixRecord913=false;remixFinishTake913();remixError911("REC MEMORY FULL");return;}
 auto&note=remixDraft911.notes[remixDraft911.notes.size-1];
 if(rollPitch<128)note.pitch=rollPitch;
 // Timing heard from the source already includes groove; avoid applying it twice.
 note.time=clampInt(-remixGroove911(remixDraft911,note,3),-96,96);
 remixRecorded913.add(note.id);remixRecordCycle913=transportTickAbsolute/1536;
 remixScheduleDirty911=true;neoDisplayDirty=displayValueDirty=true;
}
FLASHMEM bool remixDeleteStep913(uint8_t step){
 if(!remixFinishTake913())return false;rmx911::Model stage;
 if(!stage.clone(remixDraft911)){remixError911("DELETE MEMORY FULL");return false;}
 if(!rmx913::eraseStep(stage,step))return true;
 remixUndoRole911=1;return remixCommitStage911(stage,remixDraft911,remixHistory911);
}
FLASHMEM bool remixClearDraft913(){
 if(!remixFinishSelection911(true)||!remixFinishTake913())return false;
 rmx911::Model empty;memcpy(empty.tracks,remixDraft911.tracks,sizeof(empty.tracks));
 if(!remixCommitStage911(empty,remixDraft911,remixHistory911))return false;
 remixAudition911.active=false;remixRecorded913.clear();remixUndoRole911=1;return true;
}
FLASHMEM void remixLayoutEp913(){
 if(modUi805||patchUi808||settingsOpen||itemKind809)return;
 char rows[4][21];for(uint8_t y=0;y<4;++y){uint8_t raw=(3-y)*4;snprintf(rows[y],21," %02u  %02u  %02u  %02u",rmx913::step(raw,remixChannelsPreview917?remixChannels916:remixLayout913)+1,rmx913::step(raw+1,remixChannelsPreview917?remixChannels916:remixLayout913)+1,rmx913::step(raw+2,remixChannelsPreview917?remixChannels916:remixLayout913)+1,rmx913::step(raw+3,remixChannelsPreview917?remixChannels916:remixLayout913)+1);}
 mod806LiveFrame(rows[0],rows[1],rows[2],rows[3]);
}
FLASHMEM void remixCancelPreview913(){if(!remixLayoutPreview913)return;remixLayoutPreview913=false;if(!modUi805&&!patchUi808&&!settingsOpen&&!itemKind809)showGameScreen();}
FLASHMEM bool remixWaitActive913(){return playing&&remixMode&&remixPhase917(transportTickAbsolute)==rmx913::Wait;}
FLASHMEM void remixSessionService913(){
 if(remixWaitActive913())for(auto&note:activeNotes)if(note.active&&!note.remixLive913){MidiScope902 route(note.outputMask902,note.track);noteOffNow(note.channel,note.note);note.active=false;}

 if(remixLayoutPreview913&&(int32_t)(millis()-remixLayoutUntil913)>=0)remixCancelPreview913();
 uint64_t cycle=transportTickAbsolute/1536;
 if(remixChainRole916>=0&&cycle!=remixChainBar917)remixChainRole916=-1;
 if(cycle!=remixRecordCycle913){remixFinishTake913();remixRecorded913.clear();remixRecordCycle913=cycle;}
 if(!playing)remixFinishTake913();
 static uint32_t lastTick=UINT32_MAX;uint32_t tick=globalLoopedTick()/96;
 if(tick!=lastTick){lastTick=tick;neoDisplayDirty=true;}
}
