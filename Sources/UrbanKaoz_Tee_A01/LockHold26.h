#pragma once
struct HoldLock26 {bool down=false,fired=false;uint32_t at=0;uint8_t t=0,p=0,view=0;NumericValue917 descriptor;};
static HoldLock26 lockHolds26[20];
FLASHMEM void lockHoldCancel26(){for(auto&h:lockHolds26)h.down=false;}
FLASHMEM bool lockHoldDescribe26(uint8_t id,NumericValue917&d){
 if(grooveEditMode&&patternMode&&id<14&&ind50.active(listTrack40*16+listPattern40,IND_OOV50)){d={};d.kind=N917_MOD;d.lock=IND_LIST50;d.object=0;d.param=id*2+((grooveListRepeatEditing>>id)&1);d.t=listTrack40;d.p=listPattern40;return true;}
 if((!patternMode&&!indEditing59()&&!lockWantsEdit817())||remixMode)return false;
 if(id==16&&trackMenuActive&&trackPage933==4){d={};d.kind=N917_MOD;d.lock=LOCK_PROGRAM26;d.object=selectedTrack;d.t=selectedTrack;d.p=activePattern[selectedTrack];return true;}
 if(quickView903==5&&quickItem903<64&&modKind931(quickItem903)&&id==7){d={};d.kind=N917_MOD;d.lock=LOCK_MOD817;d.object=quickItem903;d.param=6;return true;}
 if(!numericDescribe917(id,d))return false;
 return d.kind==N917_LFOENV27||d.kind==N917_BLOCK||d.kind==N917_MOD||d.kind==N917_DELAY||d.kind==N917_PATCH||d.kind==N917_ROLLX||d.kind==N917_NOTE||d.kind==N917_ROLL;
}
FLASHMEM void lockHoldTurn26(uint8_t id){if(id<20)lockHolds26[id].down=false;}
FLASHMEM bool lockHoldEdge26(uint8_t id,bool down){
 if(id>=20||lockWantsEdit817())return false;auto&h=lockHolds26[id];
 if(!down){bool consumed=h.fired;h.down=h.fired=false;return consumed;}
 NumericValue917 d;if(!lockHoldDescribe26(id,d))return false;
 h={true,false,millis(),selectedTrack,activePattern[selectedTrack],quickView903,d};return false;
}
FLASHMEM void lockHoldReset26(const NumericValue917&d){
 if(d.kind==N917_MOD&&d.lock==IND_LIST50){ind50.remove(d.t*16+d.p,IND_OOV50,IND_LIST50,0,d.param);indChanged50();return;}
 if((patternMode||(!lockWantsEdit817()&&indEditing59()))&&(d.kind==N917_NOTE||d.kind==N917_ROLL)){uint8_t kind=d.kind==N917_NOTE?IND_NOTE50:IND_ROLL50;uint8_t field=d.kind==N917_NOTE?(d.field==EDIT_PITCH?0:d.field==EDIT_VEL?1:d.field==EDIT_GATE?2:3):d.field;bool old=deleteButtonActive;deleteButtonActive=true;indEdit50(kind,d.t,field,0,0,0,0);deleteButtonActive=old;return;}
 if(d.kind==N917_NOTE){int base=d.field==EDIT_PITCH?defaultMidiNote[d.t]:d.field==EDIT_VEL?defaultVel[d.t]:d.field==EDIT_GATE?defaultGateTicks[d.t]:defaultMicroTick[d.t];stageSelectedParamValue((EditMode)d.field,base,false);return;}
 if(d.kind==N917_ROLL){if(stageSelectedRollDelta(d.field,0)){auto&f=pendingSelectionEdit.roll[d.field];f.sameValue=true;f.target=rollFieldValue(trackRoll[d.t],d.field);f.delta=0;markSelectedHeldPadEdited();}return;}
 uint8_t kind;uint16_t target,param;
 if(d.kind==N917_LFOENV27){kind=d.param<3?LOCK_MOD817:LOCK_EXTRA817;target=d.object;param=d.param<3?d.param+2:2;}
 else if(d.kind==N917_BLOCK){kind=LOCK_BLOCK21;target=d.t;param=d.field==0?12:d.field-4;}
 else if(d.kind==N917_MOD){kind=d.lock;target=d.object;param=d.param;}
 else if(d.kind==N917_DELAY){kind=LOCK_DELAY817;target=delayKey924(d.t,d.p,d.slot);param=d.field;}
 else if(d.kind==N917_ROLLX){kind=LOCK_ROLL1000;target=d.t;param=d.field;}
 else if(d.kind==N917_PATCH){auto*q=patch808ParamAt(patch808Find(patchDb808,quickItem903),d.param);if(!q)return;kind=q->kind?(q->bits==14?LOCK_NRPN14817:LOCK_NRPN7817):LOCK_CC817;target=d.t;param=q->number;}
 else return;
 bool old=deleteButtonActive;deleteButtonActive=true;lockEdit817(kind,target,param,0,0,0,0);deleteButtonActive=old;
}
FLASHMEM void lockHoldService26(){
 for(uint8_t id=0;id<20;++id){auto&h=lockHolds26[id];if(!h.down||h.fired)continue;
  NumericValue917 now;if(h.t!=selectedTrack||h.p!=activePattern[selectedTrack]||h.view!=quickView903||!lockHoldDescribe26(id,now)||now.kind!=h.descriptor.kind||now.field!=h.descriptor.field||now.object!=h.descriptor.object||now.param!=h.descriptor.param||now.p!=h.descriptor.p){h.down=false;continue;}
  if(millis()-h.at<2000)continue;h.fired=true;lockHoldReset26(h.descriptor);if(id<16)kyRotatedWhileDown[id]=true;
  DISPLAY_SERIAL.println("MSG|P.LOCK DEFAULT");displayValueDirty=neoDisplayDirty=true;if(trackPage933==4)programMenu939();
 }
}
