#pragma once
// Only the sheet/list being edited is retained, not the project or playback state.
static uint8_t grooveDraftListCount60=0,grooveDraftPattern60=0;static bool grooveDraftAssignPresent60=false;static GrooveEdit50 grooveDraft60;static bool grooveDraftActive60=false;
FLASHMEM void grooveBegin60(){
 if(grooveDraftActive60||!grooveEditMode)return;
 auto&h=grooveDraft60;h={};h.generation=patternGeneration906;h.list=patternMode;h.id=h.list?listEditId52(listTrack40,listPattern40):editedGroove;
 if(!h.id&&!h.list)return;h.track=listTrack40;h.pattern=listPattern40;h.ind=h.list&&indEditing59();
 if(h.list){grooveDraftListCount60=listCount44(h.track);grooveDraftPattern60=patternGroove[h.track][h.pattern];grooveDraftAssignPresent60=ind50.find(h.track*16+h.pattern,IND_OOV50,LOCK_BLOCK21,0,12)>=0;memcpy(h.entries,listEditRead52(h.track,h.pattern),sizeof(h.entries));if(h.ind)for(unsigned f=0;f<28;++f)if(ind50.find(h.track*16+h.pattern,IND_OOV50,IND_LIST50,0,f)>=0)h.indPresent|=1u<<f;}
 else{memcpy(h.values,grooveOffset[h.id-1],80);memcpy(h.shape,grooveShape16[h.id-1],2);}grooveDraftActive60=true;
}
FLASHMEM void grooveFinish60(bool commit){
 if(!grooveDraftActive60)return;auto&h=grooveDraft60;grooveDraftActive60=false;
 if(!commit&&h.generation==patternGeneration906){
  if(h.list){if(h.ind){for(unsigned i=0;i<14;++i)for(unsigned f=0;f<2;++f){unsigned param=i*2+f;if(h.indPresent&(1u<<param))ind50.set(h.track*16+h.pattern,IND_OOV50,IND_LIST50,0,param,f?h.entries[i].repeats:h.entries[i].sheet);else ind50.remove(h.track*16+h.pattern,IND_OOV50,IND_LIST50,0,param);}}else if(h.id&&h.id<=listCount44(h.track))memcpy(listBank44(h.track)[h.id-1],h.entries,sizeof(h.entries));}
  if(h.list&&!h.id){listCount44(h.track)=grooveDraftListCount60;if(h.ind){if(!grooveDraftAssignPresent60)ind50.remove(h.track*16+h.pattern,IND_OOV50,LOCK_BLOCK21,0,12);else ind50.set(h.track*16+h.pattern,IND_OOV50,LOCK_BLOCK21,0,12,0);}else{blocks21[h.track][0].groove=0;patternGroove[h.track][h.pattern]=grooveDraftPattern60;}}
  else if(!h.list){memcpy(grooveOffset[h.id-1],h.values,80);memcpy(grooveShape16[h.id-1],h.shape,2);}indChanged50();grooveRefresh50();
 }
}
static bool deleteHeld60=false,deleteLong60=false;static uint32_t deleteAt60=0;
FLASHMEM void deleteEdge60(bool down){
 if(down){deleteHeld60=true;deleteLong60=false;deleteAt60=millis();}
 else if(deleteHeld60){deleteHeld60=false;if(!deleteLong60)handleEditButtonPress(EDIT_BUTTON_DELETE);}
}
FLASHMEM bool assignDirty60(){return assignButton932<64&&(assignConflict932>=0||assignFunction932!=buttonMap932[assignButton932]);}
FLASHMEM void uiService60(){
 ukService60();
 if(deleteHeld60&&!deleteLong60&&(uint32_t)(millis()-deleteAt60)>=2000){deleteLong60=true;if(selectionHasActiveNotes()&&!songMode&&!remixMode&&!grooveEditMode&&!settingsOpen){if(deleteButtonActive)finishDeleteMode(false);deleteStepSelection();selection805Changed();}}
 uint16_t masks[4]={};
 if(grooveDraftActive60&&grooveEditMode){auto&h=grooveDraft60;if(!patternMode&&!h.list&&h.id==editedGroove){for(uint8_t f=0;f<4;++f){bool changed=false;for(uint8_t i=0;i<16;++i)changed|=grooveOffset[h.id-1][i][f]!=h.values[i][f]||(f==2&&grooveOffset[h.id-1][i][4]!=h.values[i][4]);if(changed)masks[2]|=15u<<(f*4);}for(unsigned f=0;f<2;++f)if(h.shape[f]!=grooveShape16[h.id-1][f])masks[3]|=15u<<(f*4);}else if(patternMode&&h.list){auto*now=listEditRead52(h.track,h.pattern);for(unsigned i=0;i<14;++i)if(now[i].sheet!=h.entries[i].sheet||now[i].repeats!=h.entries[i].repeats)masks[i/4]|=15u<<((i%4)*4);}}
 if(pendingSelectionEdit.active&&!grooveEditMode){if(!quickView903){const uint8_t map[4]={2,3,4,5};for(unsigned i=0;i<4;++i)if(pendingSelectionEdit.field[i].touched)masks[map[i]/4]|=15u<<((map[i]%4)*4);const uint8_t rm[6]={10,11,12,13,14,8};for(unsigned i=0;i<6;++i)if(pendingSelectionEdit.roll[i].touched)masks[rm[i]/4]|=15u<<((rm[i]%4)*4);}else if(quickView903==2){for(unsigned k=0;k<16;++k){uint8_t f=rollKey26(k);if(f<10&&!(f&1)&&pendingSelectionEdit.roll[f/2].touched)masks[k/4]|=15u<<((k%4)*4);if((f==5&&pendingSelectionEdit.field[EDIT_TIME].touched)||(f==9&&pendingSelectionEdit.field[EDIT_VEL].touched))masks[k/4]|=15u<<((k%4)*4);}}}
 static uint16_t sent[4]={65535,65535,65535,65535};if(memcmp(sent,masks,sizeof(sent))){memcpy(sent,masks,sizeof(sent));DISPLAY_SERIAL.printf("EDIT60,%u,%u,%u,%u\n",masks[0],masks[1],masks[2],masks[3]);}
}

FLASHMEM void ukService60(){
 static bool connected=false;static uint64_t pads=0,knownPads=0;static uint16_t modes=0,knownModes=0;
 if(!synkozEnabled933||!pc831.active){connected=false;return;}if(!connected||ukReset60){connected=true;ukReset60=false;knownPads=knownModes=0;}
 uint16_t current=(stepContext25()?1:0)|(patternMode&&!grooveEditMode?2:0)|(grooveEditMode&&!patternMode?4:0)|(grooveEditMode&&patternMode?8:0)|(quickView903==2?16:0)|((quickView903==3||modUi805==MOD_DELAY806)?32:0)|((quickView903==4||(modUi805&&modUi805!=MOD_DELAY806&&modSelected806<64&&!modKind931(modSelected806)))?64:0)|((quickView903==5||(modUi805&&modUi805!=MOD_DELAY806&&modSelected806<64&&modKind931(modSelected806)))?128:0)|((quickView903==6||patchUi808||patchBank808)?256:0)|(songMode?512:0)|(remixMode?1024:0)|(keyboardEnabled823?2048:0)|(recordArmed823()?4096:0)|(playing?8192:0)|(indEditing59()?16384:0)|(assignOpen932?32768:0);
 for(uint8_t i=0;i<16&&synkozCount933<60;++i){uint16_t b=1u<<i;if((knownModes&b)&&!((modes^current)&b))continue;auto&e=synkozQueue933[(synkozHead933+synkozCount933++)%64];e={++synkozSequence933,micros(),synkozTick933,7,(uint8_t)(i+((current&b)?0:64)),0,30};knownModes|=b;modes=(modes&~b)|(current&b);}
 for(uint8_t i=0;i<64&&synkozCount933<60;++i){uint64_t b=1ULL<<i;if((knownPads&b)&&!((pads^ukPads60)&b))continue;bool on=ukPads60&b;auto&e=synkozQueue933[(synkozHead933+synkozCount933++)%64];e={++synkozSequence933,micros(),synkozTick933,(uint8_t)(on?4:5),i,(uint8_t)(on?127:0),30};knownPads|=b;pads=(pads&~b)|(ukPads60&b);}
}
