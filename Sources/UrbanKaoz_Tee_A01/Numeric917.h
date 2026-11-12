#pragma once
// Read on opening; publish only on Enter. No session or per-tick value table.
enum NumericKind917:uint8_t {N917_NONE,N917_NOTE,N917_CHANNEL,N917_VOLUME,N917_BEND,N917_ROLL,N917_RMX,N917_RMX_NOTE,N917_DELAY,N917_MOD,N917_PATCH,N917_BPM,N917_ZOOM,N917_VIEW,N917_LENGTH,N917_LOOP,N917_GROOVE,N917_ROLLX,N917_FREEGATE,N917_TARGET,N917_LFOENV27,N917_BLOCK,N917_IO38,N917_DTARGET55};
struct NumericValue917 {uint8_t kind=0,field=0,ky=0,t=0,p=0,lock=0,slot=0;uint16_t object=0,param=0;int value=0,lo=0,hi=127,base=0;};
static uint32_t numericToken917=0,numericFingerprint917=0;

FLASHMEM bool numericMod917(uint8_t ky,uint16_t key,int depth,NumericValue917&d){
 if(key>=64||!mod806Unit(key).enabled)return false;
 if(ioKey38(key)&&ky!=8&&ky!=9){auto&m=modUnits805[key];if(ky>4)return false;d.kind=N917_IO38;d.object=key;d.param=ky;d.value=ky==0?m.hold:ky==1?m.attack:ky==2?m.decay:ky==3?m.release:m.sustain;d.lo=ky==3?1:0;d.hi=ky==0?12:ky==1?15:ky==4?127:65535;return true;}
 if(ioKey38(key)){if(ky==8||ky==9)ky+=4;}
 else if(!modKind931(key)){if(ky==9||ky==11)return false;if(ky==8)ky=12;else if(ky==10)ky=13;else if(ky==12||ky==13)ky-=4;}
 if(modKind931(key)){
  if(ky>=12){auto&m=mod806Unit(key);d.kind=N917_LFOENV27;d.object=key;d.param=ky-12;d.lo=ky==15?-100:0;d.hi=ky==15?100:65535;d.value=ky==12?m.decay:ky==13?m.attack:ky==14?m.release:modExtra810[key].releaseCurve;d.base=d.value;d.value=lockDisplay817(ky==15?LOCK_EXTRA817:LOCK_MOD817,key,ky==15?2:ky-10,d.value);return true;}
  if(ky==9)return false;if(ky==8)ky=12;else if(ky==10)ky=13;else if(ky==11)ky=14;
 }

 if(ky==13&&depth>=0){auto&r=mod806Unit(key).outputs[depth];int value;const char*label="";if(!modTargetBase06(r,0,value,label))return false;d.kind=N917_TARGET;d.object=key;d.param=depth;d.value=value;d.lo=0;d.hi=r.kind==MOD_CV46?4095:127;if(modIsTarget811(r)){uint16_t target=modTarget810(r);uint8_t f=r.number%8;if(r.kind==MOD_DEPTH810){d.hi=modLimit810(mod806Unit(target).outputs[f].kind);d.lo=-d.hi;if(d.hi==8192)--d.hi;}else{d.hi=modKind931(target)?(f?360:LFO_MAX42):(f==3?127:65535);d.lo=modKind931(target)&&!f?1:0;}}return true;}
 auto&m=mod806Unit(key);auto&e=modExtra810[key];d.kind=N917_MOD;d.object=key;d.lock=LOCK_MOD817;d.hi=65535;
 if(ky==14){d.lock=LOCK_RAT817;d.value=modRat808[key];d.lo=1;d.hi=3072;}
 else if(ky==12&&depth>=0&&m.outputs[depth].enabled){d.lock=LOCK_DEPTH817;d.param=depth;d.value=m.outputs[depth].depth;d.hi=modLimit810(m.outputs[depth].kind);d.lo=-d.hi;if(d.hi==8192)--d.hi;}
 else if(!(modKind931(key))){
  if(ky==0){d.value=holdBase50(key,m);d.lo=-384;}
  else if(ky==2){d.param=1;d.value=m.attack;}
  else if(ky==4){d.param=2;d.value=m.decay;}
  else if(ky==6){d.param=3;d.value=m.sustain;d.hi=127;}
  else if(ky==8){d.param=4;d.value=m.release;}
  else if(ky==3||ky==5||ky==9){d.lock=LOCK_EXTRA817;d.param=ky==3?0:ky==5?1:2;d.value=d.param==0?e.attackCurve:d.param==1?e.decayCurve:e.releaseCurve;d.lo=-100;d.hi=100;}
  else return false;
 }else if(ky==0){d.value=m.rate;d.lo=1;d.hi=lfoMs932(key)?LFO_MAX42:65535;}
 else if(ky==6){d.param=5;d.value=m.hold;d.lo=1;d.hi=99;}
 else if(ky==4){d.lock=LOCK_EXTRA817;d.param=4;d.value=e.phase;d.hi=360;}
 else return false;
 d.base=d.value;d.value=lockDisplay817(d.lock,key,d.param,d.value);return true;
}
FLASHMEM bool numericDescribe917(uint8_t id,NumericValue917&d){
 NeoInputScope932 neoScope(id<16);
 d=NumericValue917{};d.ky=id;d.t=selectedTrack;d.p=activePattern[d.t];
 if((quickView903==3||modUi805==MOD_DELAY806)&&(id==6||id==7||id==9||id==11))return false;
 if(sdBusy905()||debugTestMode!=DEBUG_TEST_NONE||numericEntryActive||deleteButtonActive||fileBrowserMode!=FILE_BROWSER_NONE)return false;
 if(id<16&&grooveEditMode&&!patternMode&&!menuNavigationOnly()){
  d.kind=N917_GROOVE;d.field=id;if(id==0){d.value=editedGroove;d.lo=1;d.hi=GROOVE_COUNT;return true;}
  if(id==12||id==13){d.value=grooveShape16[editedGroove-1][id-12];d.lo=0;d.hi=100;return true;}if(id<8||id>11||!grooveStepMask)return false;id-=6;d.field=id;bool first=true,mixed=false;for(uint8_t i=0;i<16;++i)if(grooveStepMask&(1u<<i)){int v=id==4?grooveGate51(editedGroove,i):grooveOffset[editedGroove-1][i][id-2];if(first){d.value=v;first=false;}else mixed|=v!=d.value;}d.base=mixed;d.lo=id==4?(mixed?-1000:0):-99;d.hi=id==4?1000:99;if(mixed)d.value=0;return true;
}
 // Roy controls use their displayed transport context; menus never edit behind it.
 if(id>=16){
  uint8_t r=id-16;if(r>=4)return false;
  if(modUi805==MOD_DELAY806){if(delayTargetsOpen55)return false;d.kind=N917_DELAY;d.t=modOwnerTrack805;d.p=modOwnerPattern805;d.field=delayRoyField21(r);d.slot=delaySlot924;if(r==3){d.object=delayTargetSelected55[d.t][d.slot];auto v=delayGet907(d.t*16+d.slot*256);if(d.object==4){if(v.scdo==-128)return false;d.field=delayXp56?11:7;}else{auto&a=v.targets55[d.object];if(a.kind==255)return false;d.kind=N917_DTARGET55;d.field=delayXp56;d.param=a.kind;d.value=d.field?a.exp:a.kind==7?v.note:a.step;d.base=d.value;d.lo=d.field?0:a.kind==7?-127:-32767;d.hi=d.field?100:a.kind==7?127:32767;return true;}}}
  else {
   if(menuNavigationOnly()||modUi805||patchUi808||songMode||grooveEditMode)return false;
   if(r==0){d.kind=roy1EditsZoom?N917_ZOOM:N917_BPM;d.value=roy1EditsZoom?zoomLevel:(int)bpm;d.lo=roy1EditsZoom?-2:(int)BPM_MIN;d.hi=roy1EditsZoom?5:(int)BPM_MAX;}
   else if(r==1){d.kind=beatEditLength?N917_LENGTH:N917_VIEW;d.value=beatEditLength?beatDraftBars:viewTickOffset/max(1UL,windowTickSpan())+1;d.lo=1;uint32_t span=windowTickSpan(),visible=monoMeasureRowsView(selectedTrack)?span*4:span;d.hi=beatEditLength?MAX_BARS:1+(activeSequenceTicks()>visible?(activeSequenceTicks()-visible)/max(1UL,span):0);}
   else {d.kind=N917_LOOP;d.field=r-2;d.value=d.field?loopEndStep:loopStartStep;d.lo=d.field?loopStartStep:1;d.hi=d.field?longestTrackStepCount():loopEndStep;}
   return true;
  }
 }else if(!quickView903&&!patternMode&&!grooveEditMode&&!remixMode&&id==6){return false;
 }else if(!quickView903&&!patternMode&&!grooveEditMode&&!remixMode&&id==7){d.kind=N917_BEND;d.value=trackPitchBend[d.t];d.lo=-8192;d.hi=8191;return true;
 }else if(quickView903==1){
 d.t=quickTrack903;d.p=activePattern[d.t];
 if(id==0){d.kind=N917_BLOCK;d.field=0;d.base=blocks21[d.t][0].groove;d.value=lockDisplay817(LOCK_BLOCK21,d.t,12,d.base);d.lo=0;d.hi=listCount44(d.t);return true;}
 if(id==7||id==6){d.kind=N917_NOTE;d.field=id==7?EDIT_PITCH:EDIT_TIME;EditMode old=editMode;editMode=(EditMode)d.field;d.value=getCurrentEditedValue();editMode=old;d.lo=id==7?0:MICRO_TICK_MIN;d.hi=id==7?127:MICRO_TICK_MAX;return true;}
 if(id>=4){uint8_t b=(id-4)/4,f=(id-4)%4;auto&r=blocks21[linkOwner12(d.t,LINK_MOUT12)][b];d.kind=N917_BLOCK;d.field=id;d.base=f==0?r.port:f==1?r.channel:f==2?r.tim:r.transpose;d.value=lockDisplay817(LOCK_BLOCK21,d.t,id-4,d.base);d.lo=f==2?-96:f==3?-127:0;d.hi=f==0?8:f==1?15:f==2?96:127;if(f==1){++d.value;++d.lo;++d.hi;}return true;}
 if(id==3){d.kind=N917_BEND;d.value=trackPitchBend[d.t];d.lo=-8192;d.hi=8191;return true;}
 d.kind=N917_NOTE;d.field=id==0?EDIT_PITCH:id==1?EDIT_VEL:EDIT_GATE;EditMode old=editMode;editMode=(EditMode)d.field;d.value=getCurrentEditedValue();editMode=old;d.lo=id==0?0:id==1?1:GATE_TICK_MIN;d.hi=id==2?GATE_TICK_MAX:127;return true;
 }else if(remixMode&&quickView903<3){
  if(menuNavigationOnly()&&!remixWriteView911)return false;
  if(remixWriteView911){d.kind=N917_RMX;d.field=id+32;d.lo=0;d.hi=63;
   if(id==1&&!remixWriteCurrent911){d.value=remixWriteRow911+1;d.lo=(remixLoading911?0:remixWriteMinimum911)+1;d.hi=16;}
   else if(id==2&&!remixLoading911)d.value=remixPre911;
   else if(id==3&&!remixLoading911){d.value=remixCopies911;d.lo=1;d.hi=64;}
   else if(id==4&&!remixLoading911)d.value=remixPost911;else return false;return d.lo<=d.hi;
  }
  if(!quickView903){d.kind=N917_RMX;d.field=id;
   if(id==0&&!selectButtonActive){d.value=remixPendingSource915+1;d.lo=1;d.hi=remixMaxBars911();}
   else if(id==1){d.value=remixHearBar911;d.hi=MAX_BARS;}
   else if(id==2){d.value=remixWait913;d.hi=max(0,(int)remixHearBar911-1);}
   else if(id==4){d.value=remixLatency911;d.lo=-1536;d.hi=1536;}
   else if(id==6&&remixLen916==rmx916::Fixed){d.value=remixFixed916;d.lo=1;d.hi=1536;}
   else if(id==12||id==13){d.value=(id==12?remixLayout913:remixChannels916)+1;d.lo=1;d.hi=4;}
   else return false;return true;
  }
  if(remixSelection911.role!=1||!selectButtonActive||!remixSelection911.steps)return false;
  int field=-1;if(quickView903==1){if(id>=2&&id<=5)field=id-2;else if(id==8)field=9;else if(id>=10&&id<=14)field=id-6;}
  else if(quickView903==2&&!(id%2)&&id<12)field=4+id/2;
  else if(quickView903==3&&!(id%2))field=10+delayFields907[id/2];
  if(field<0)return false;d.kind=N917_RMX_NOTE;d.field=field;
  const rmx911::Note*n=nullptr;for(uint32_t i=0;i<remixDraft911.notes.size;++i)if(remixSelection911.contains(remixDraft911.notes[i],remixDraft911,false)){n=&remixDraft911.notes[i];break;}if(!n)return false;
  if(field<4){d.value=field==0?n->pitch:field==1?n->velocity:field==2?n->gate:n->time;d.lo=field==1?1:field==2?GATE_TICK_MIN:field==3?-96:0;d.hi=field==2?GATE_TICK_MAX:field==3?96:127;}
  else {d.value=remixDraft911.value(*n,field<10?rmx911::Roll:rmx911::Delay,field<10?field-4:field-10);d.lo=field<10?clampRollFieldValue(field-4,-65535):delayMin907(field-10);d.hi=field<10?clampRollFieldValue(field-4,65535):delayMax907(field-10);}
  return true;
 }else if(quickView903==3){
  if(id==14)return false;d.kind=N917_DELAY;d.field=delayQvFields925[id];d.t=quickView903==3?quickTrack903:modOwnerTrack805;d.p=quickView903==3?quickPattern903:modOwnerPattern805;d.slot=delaySlot924;if(quickView903==3){if(quickItem903>=2048)return false;d.t=(quickItem903%256)/16;d.p=quickItem903%16;d.slot=quickItem903/256;}
 }else if(quickView903==4||quickView903==5){
  int depth=-1;if(quickItem903>=64)return false;for(uint8_t n=0;n<8;++n){uint8_t i=(quickDepth903+n)%8;if(quickDepthVisible59(quickItem903,i)){depth=i;break;}}
  return numericMod917(id,quickItem903,depth,d);
 }else if(quickView903==6){
  auto*q=patchEditableAt44(patch808Find(patchDb808,quickItem903),quickPatchPage903*16+id);if(!q)return false;
  d.kind=N917_PATCH;d.t=quickTrack903;d.p=quickPattern903;d.param=quickPatchPage903*16+id;d.hi=q->bits==14?16383:127;uint16_t value=0;patchRawValue50(quickPatchContext903(),*q,value);d.value=lockDisplay817(q->kind?(q->bits==14?LOCK_NRPN14817:LOCK_NRPN7817):LOCK_CC817,d.t,q->number,value);return true;
 }else if(menuNavigationOnly()||patchBank808||patternMode)return false;
 else if(grooveEditMode){return false;
 }else {
  if(quickView903==2&&stableQuick904){id=rollKey26(id);if(id==5||id==9){d.kind=N917_NOTE;d.field=id==5?EDIT_TIME:EDIT_VEL;EditMode old38=editMode;editMode=(EditMode)d.field;d.value=getCurrentEditedValue();editMode=old38;d.lo=id==5?MICRO_TICK_MIN:1;d.hi=id==5?MICRO_TICK_MAX:127;return true;}if(id==7||id==12||id==14||id==15){d.kind=N917_ROLLX;d.field=id==7?6:id==12?0:id==14?5:3;}else {if(id%2||id>=12)return false;d.kind=N917_ROLL;d.field=id/2;}}
  else if(id==0&&trackContext815()){d.kind=N917_VOLUME;d.value=trackVolume818[d.t];d.hi=127;return true;}
  else if(id==1){d.kind=N917_CHANNEL;d.value=midiChannelForTrack(d.t)+1;d.lo=1;d.hi=16;return true;}
  else if(id>=2&&id<=5){
   if(id==4&&selectedEmptyCount&&!selectionHasActiveNotes()){d.kind=N917_FREEGATE;d.value=freeGateValue11();d.lo=1;d.hi=65535;return true;}
   if(hasSelectedSteps()&&!selectionHasActiveNotes())return false;d.kind=N917_NOTE;d.field=id==2?EDIT_PITCH:id==3?EDIT_VEL:id==4?EDIT_GATE:EDIT_TIME;
   EditMode previous=editMode;editMode=(EditMode)d.field;d.value=getCurrentEditedValue();if(selectionHasActiveNotes()){for(uint16_t s=0;s<trackStepCount(d.t);++s){int8_t v=selectedRangeVoice(d.t,s);if(v<0)continue;int base=id==2?noteAtVoice(d.t,s,v):id==3?effectiveVel(d.t,s,v):id==4?effectiveGateTicks(d.t,s,v):effectiveMicroTick(d.t,s,v);d.value=pendingEditedValue(d.t,s,v,editMode,base);break;}}editMode=previous;
   d.lo=id==3?1:id==4?GATE_TICK_MIN:id==5?MICRO_TICK_MIN:0;d.hi=id==4?GATE_TICK_MAX:id==5?MICRO_TICK_MAX:127;return true;
  }else if(id==6){d.kind=N917_BEND;d.value=trackPitchBend[d.t];d.lo=-8192;d.hi=8191;return true;}
  else if(id==15){d.kind=N917_ROLLX;d.field=0;}
  else if(id==8||(id>=10&&id<=14)){d.kind=N917_ROLL;d.field=id==8?5:id-10;}
  else return false;
 }
 if(d.kind==N917_DELAY&&id<16&&(id==6||id==7||id==9||id==11))return false;
 if(d.kind==N917_DELAY&&remixMode&&remixSelection911.steps){if(remixSelection911.role!=1)return false;
for(uint32_t i=0;
i<remixDraft911.notes.size;
++i){const auto&n=remixDraft911.notes[i];
if(!remixSelection911.contains(n,remixDraft911,false))continue;
int at=remixDraft911.find(n.id,rmx911::Delay,100+d.slot*32+d.field);
d.value=at>=0?remixDraft911.properties[at].value-4096:delayBaseSlot924(n.track,0,d.slot,d.field);
d.lo=delayMin907(d.field);
d.hi=delayMax907(d.field);
return true;
}return false;
}
 if(d.kind==N917_DELAY){d.value=lockDisplay817(LOCK_DELAY817,delayKey924(d.t,d.p,d.slot),d.field,delayBaseSlot924(d.t,d.p,d.slot,d.field));d.lo=delayMin907(d.field);d.hi=delayMax907(d.field);if(d.field==4||d.field==17){++d.value;d.lo=1;d.hi=d.field==17?3:16;}if(d.field==3&&delayValue907(d.t,d.p,8,true)==1)d.lo=0;return true;}
 if(d.kind==N917_ROLL&&d.field==5&&lockDisplay817(LOCK_ROLL1000,d.t,1,rollExtra1000(d.t).v[1])){d.kind=N917_ROLLX;d.field=2;}
 if(d.kind==N917_ROLLX){auto x=rollExtra1000(d.t);d.value=lockDisplay817(LOCK_ROLL1000,d.t,d.field,x.v[d.field]);d.lo=rollMin1000(d.field);d.hi=d.field==6?max(0,(int)displayedRollForTrack(d.t).start-1):rollMax1000(d.field);d.value=min(d.value,d.hi);return true;}
 if(d.kind==N917_ROLL){if(hasSelectedSteps()&&!selectionHasActiveNotes())return false;d.value=rollFieldValue(displayedRollForTrack(d.t),d.field);d.lo=clampRollFieldValue(d.field,-65535);d.hi=clampRollFieldValue(d.field,65535);return true;}return false;
}
FLASHMEM uint32_t numericHash917(const NumericValue917&d){
 uint32_t hash=2166136261u;auto add=[&](uint32_t v){hash=(hash^v)*16777619u;};
 add(d.slot);add(d.kind);add(d.field);add(d.ky);add(d.t);add(d.p);add(d.object);add(d.param);add(d.lock);add(d.value);add(d.base);add(d.lo);add(d.hi);
 add(remixMode);
add(grooveEditMode);
add(patternMode);
add(quickView903);
add(quickItem903);
add(modUi805);
add(modSelected806);
add(selectedTrack);
add(selectedStep);
add(selectedVoice);
add(selectButtonActive);
add(remixSelection911.steps);
add(remixSelection911.exiled);
add(remixDraft911.nextId);
add(editedGroove);
add(grooveStepMask);
add(pitchEncoderSelectMode);
add(timeEncoderMoveMode);
add(beatEditLength);
add(roy1EditsZoom);
add(remixLen916);
add(remixHit916);
add(remixWriteView911);
add(remixWriteCurrent911);
add(remixLoading911);

 for(uint8_t t=0;t<16;++t){add(activePattern[t]);for(uint16_t s=0;s<trackStepCount(t);++s)if(stepSelected[t][s]){add(t*1024+s);add(selectedBaseMask[t][s]);}}
 return hash;
}
FLASHMEM bool numericApply917(const NumericValue917&d,int value){
 if(value<d.lo||value>d.hi)return false;
 if(d.kind==N917_DTARGET55){auto v=delayGet907(d.t*16+d.slot*256);auto&a=v.targets55[d.object];if(a.kind!=d.param)return false;if(d.field)a.exp=value;else if(a.kind==7)v.note=value;else a.step=value;bool ok=delaySet907(d.t*16+d.slot*256,v);if(ok){storageDirty931=true;displayValueDirty=modDisplayDirty810=true;}return ok;}
 if(d.kind==N917_BLOCK){blockKy21(d.field,value-d.value);return true;}
 if(d.kind==N917_ROLLX){rollEdit1000(d.field,value-d.value);return true;}
 if(d.kind==N917_RMX_NOTE){remixEditDelta911(d.field,value,true);return true;}
 if(d.kind==N917_RMX){uint8_t q=remixQuantum916;remixQuantum916=0;remixKy911(d.field>=32?d.field-32:d.field,value-d.value);remixQuantum916=q;return true;}
 if(d.kind==N917_TARGET){int result;const char*label="";bool ok=modTargetBase06(mod806Unit(d.object).outputs[d.param],value-d.value,result,label);if(ok){mod805Invalidate();modDisplayDirty810=displayValueDirty=true;}return ok;}
 if(d.kind==N917_FREEGATE)return freeGateEdit11(value,true);
 if(d.kind==N917_NOTE){editMode=(EditMode)d.field;if(selectionHasActiveNotes()||currentEditIsPLock())stageSelectedParamValue(editMode,value,false);else if(!indEdit50(IND_NOTE50,d.t,d.field==EDIT_PITCH?0:d.field==EDIT_VEL?1:d.field==EDIT_GATE?2:3,0,0,value,value))setDirectDefaultValue(d.t,editMode,value);markSelectedHeldPadEdited();}
 else if(d.kind==N917_CHANNEL)changeMidiChannelFromKy(value-d.value);
 else if(d.kind==N917_VOLUME)trackVolumeTurn818(value-d.value);
 else if(d.kind==N917_BEND){if(!indEdit50(IND_NOTE50,d.t,4,0,0,value,value))trackPitchBend[d.t]=value;}
 else if(d.kind==N917_ROLL){if(stageSelectedRollDelta(d.field,0)){auto&f=pendingSelectionEdit.roll[d.field];f.sameValue=true;f.target=value;f.delta=0;markSelectedHeldPadEdited();}else if(!indEdit50(IND_ROLL50,d.t,d.field,0,0,value,value))setRollFieldValue(trackRoll[d.t],d.field,value);scheduleConfiguredRollApply();}
 else if(d.kind==N917_DELAY){
  if(remixMode&&remixSelection911.steps){uint8_t saved=delaySlot924;delaySlot924=d.slot;remixEditDelta911(10+d.field,value,true);delaySlot924=saved;return true;}
  if(d.field==4||d.field==17)--value;if(d.field<2)delayIncrementShown925&=~(1u<<d.field);int base=delayBaseSlot924(d.t,d.p,d.slot,d.field);
  if(!lockEdit817(LOCK_DELAY817,delayKey924(d.t,d.p,d.slot),d.field,base,0,value,value)){
   if(!delayAssign924(d.t,d.p,d.slot,d.field,value))return false;
  }++modEpoch805;delayMenuDirty817=modDisplayDirty810=true;
 }else if(d.kind==N917_LFOENV27){auto&m=mod806Unit(d.object);if(lockEdit817(d.param<3?LOCK_MOD817:LOCK_EXTRA817,d.object,d.param<3?d.param+2:2,d.base,0,value,value)){modDisplayDirty810=true;return true;}if(d.param==0)m.decay=value;else if(d.param==1)m.attack=value;else if(d.param==2)m.release=value;else modExtra810[d.object].releaseCurve=value;modPresetDirty07[d.object]=storageDirty931=true;mod805Invalidate();modDisplayDirty810=true;
 }else if(d.kind==N917_IO38){auto&m=modUnits805[d.object];if(d.param==0)m.hold=value;if(d.param==1)m.attack=value;if(d.param==2)m.decay=value;if(d.param==3)m.release=value;if(d.param==4)m.sustain=value;ioSeen38[d.object]=false;storageDirty931=true;mod805Invalidate();
 }else if(d.kind==N917_MOD){
  if(!lockEdit817(d.lock,d.object,d.param,d.base,0,value,value)){
   auto&m=mod806Unit(d.object);auto&e=modExtra810[d.object];
   if(d.lock==LOCK_RAT817)modRat808[d.object]=value;
   else if(d.lock==LOCK_DEPTH817)m.outputs[d.param].depth=value;
   else if(d.lock==LOCK_EXTRA817){if(d.param==0)e.attackCurve=value;else if(d.param==1)e.decayCurve=value;else if(d.param==2)e.releaseCurve=value;else e.phase=value;}
   else if(modKind931(d.object)){if(d.param==5)m.hold=value;else m.rate=value;}
   else if(d.param==0)holdSet50(d.object,value);else if(d.param==1)m.attack=value;else if(d.param==2)m.decay=value;else if(d.param==3)m.sustain=value;else m.release=value;
  }mod805Invalidate();modDisplayDirty810=true;
 }else if(d.kind==N917_PATCH){
  auto*q=patchEditableAt44(patch808Find(patchDb808,quickItem903),d.param);if(!q)return false;
  auto&c=patchDb808.context[linkOwner12(d.t,LINK_PATCH12)][quickPatchBlock21*17];
  uint16_t base59=65535;patchRawValue50(c,*q,base59);if(lockWantsEdit817()&&indBase50(d.t,activePattern[d.t],q->kind?(q->bits==14?LOCK_NRPN14817:LOCK_NRPN7817):LOCK_CC817,d.t,q->number,base59)==65535){lockBlinkSend11((d.param%16)/4,15u<<((d.param%4)*4));DISPLAY_SERIAL.println("MSG|SET PATCH BASE FIRST");return false;}
  
  if(!lockEdit817(q->kind?(q->bits==14?LOCK_NRPN14817:LOCK_NRPN7817):LOCK_CC817,d.t,q->number,d.value,0,value,value)){
   if(!patchSetDefault59(c,*q,value))return false;patchChanged15(d.t,c,*q,value);
  }++modEpoch805;
 }else if(d.kind==N917_BPM){bpm=value;updateInternalClockPeriod();}
 else if(d.kind==N917_ZOOM){while(zoomLevel!=value)zoomAroundSelection(value>zoomLevel?1:-1,VIEW_SRC_ROY1_ZOOM);}
 else if(d.kind==N917_LENGTH)beatDraftBars=value;
 else if(d.kind==N917_VIEW){viewTickOffset=(value-1)*windowTickSpan();pendingViewSource=VIEW_SRC_ROY2;clampViewStepOffset();}
 else if(d.kind==N917_LOOP){if(d.field)loopEndStep=value;else loopStartStep=value;syncLegacyTrackLoopBounds();}
 else if(d.kind==N917_GROOVE){if(d.field==0){grooveFinish60(true);editedGroove=value;grooveEditTrack23[selectedTrack]=value;}else {grooveBegin60();if(d.field==12||d.field==13){grooveEditAssign26();grooveShape16[editedGroove-1][d.field-12]=value;}else {for(uint8_t i=0;i<16;++i)if(grooveStepMask&(1u<<i)){int old=d.field==4?grooveGate51(editedGroove,i):grooveOffset[editedGroove-1][i][d.field-2];int next=d.base?old+value:value;if(d.field==4)grooveGateSet51(editedGroove,i,next);else grooveOffset[editedGroove-1][i][d.field-2]=clampInt(next,-99,99);}grooveEditAssign26();}}mod805Invalidate();invalidateStepTriggers();sendGroovePacket();}
 else return false;
 neoDisplayDirty=displayValueDirty=displayBpmDirty=true;return true;
}
FLASHMEM void numericRead917(int id){
 NumericValue917 d;numericId917=-1;
 if(id<0||id>=20||!numericDescribe917(id,d)){Serial.println("PC1,VALUE_ERROR,NO_NUMERIC_VALUE");return;}
 numericToken917=(numericToken917+1)&0x7FFFFFFF;if(!numericToken917)++numericToken917;
 numericId917=id;numericFingerprint917=numericHash917(d);
 Serial.printf("PC1,VALUE,%d,%lu,%d,%d,%d\n",id,(unsigned long)numericToken917,d.value,d.lo,d.hi);
}
FLASHMEM void numericWrite917(int id,int value,uint32_t token){
 NumericValue917 d;
 bool valid=id==numericId917&&token==numericToken917&&numericDescribe917(id,d)&&numericFingerprint917==numericHash917(d);
 numericId917=-1;
 if(!valid){Serial.println("PC1,VALUE_ERROR,CONTEXT_CHANGED");return;}
 if(!numericApply917(d,value)){Serial.println("PC1,VALUE_ERROR,VALUE_REJECTED");return;}
 Serial.println("PC1,VALUE_OK");
}

