#pragma once
// Explicit command, disposable project, no serial output per tick.
FLASHMEM void checks811(){
  if(playing){Serial.println("CHECK811,STOP_FIRST");return;}
  autosaveReady805=false;newProject807();uint16_t pass=0,fail=0;
  auto check=[&](bool ok,const char*name){if(ok)++pass;else ++fail;Serial.printf("CHECK811,%s,%s\n",ok?"OK":"FAIL",name);};
  trackBars[0]=4;loopStartStep=1;loopEndStep=16;grooveTrackPhase[0]=0;
  for(uint16_t s=0;s<16;s+=4){stepActive[0][s]=true;stepGateTicks[0][s]=96;stepLockGate[0][s]=true;stepMicroTick[0][s]=0;stepLogicalMicro[0][s]=0;}
  auto&m=mod806Unit(0);m.enabled=1;m.inputs=1;m.attack=0;m.decay=32;m.sustain=0;m.release=16;m.outputs[0]={MOD_CC,0,7,1,127};modRat808[0]=1;
  mod806Associate(modMeta806[0],0,0,true);mod805Invalidate();bool sourceOk=true,envOk=true;uint32_t began=micros();
  for(int64_t at=0;at<384*500;at+=8){auto src=mod805Source(0,at);sourceOk&=src.begin==(at/384)*384;float expected=at%384<32?1-(at%384)/32.f:0;envOk&=fabsf(mod805UnitValue(0,at)-expected)<.001f;}
  check(sourceOk,"500_retriggers_short_master_loop");check(envOk,"500_complete_envelopes_no_parameter_edit");Serial.printf("CHECK811,ENVELOPE_TEST_US,%lu\n",(unsigned long)(micros()-began));
  check(mod805Source(0,6144+22).begin==6144&&mod805Source(0,1536+22).begin==1536,"out_of_order_cache");
  modSelected806=0;modUi805=MOD_OBJECT806;mod806EditDest(0,false);mod806Roy(1,3,false);mod806Roy(3,67,false);
  check(m.outputs[0].track==0&&m.outputs[0].number==7,"draft_route_inaudible");mod806Roy(0,0,true);
  check(!modDirty806&&m.outputs[0].track==3&&m.outputs[0].number==74,"OA_validates_whole_destination_row");
  mod806Roy(1,2,false);mod806Roy(3,1,false);mod805Back();check(modUi805==MOD_OBJECT806&&m.outputs[0].track==3&&m.outputs[0].number==74,"J1_cancel_and_back_one_action");
  mod806EditSource(0,false);mod806Roy(0,1,false);mod806Roy(1,3,false);mod806Roy(0,0,true);
  check(m.inputs==0&&modMeta806[0].midiInputs==8&&!modDirty806,"OA_validates_whole_source_row");mod805Back();
  auto&other=mod806Unit(2);other.enabled=1;other.outputs[1]={MOD_CC,0,7,1,20};other.outputs[4]={MOD_CC,3,7,1,60};
  check(modExistingOutput811(2,0,0)==1&&modExistingOutput811(2,1,1)==4&&modExistingOutput811(2,4,-1)==1,"existing_outputs_only");
  m.outputs[0]={MOD_PARAM811,0,17,1,32};mod805Invalidate();
  check(modGraphValid810(modUnits805),"ENV_to_Attack_acyclic");ModRoute805 back={MOD_DEPTH810,0,0,1,20};check(!modAcyclic810(2,0,back),"mixed_parameter_depth_cycle_refused");
  while(modUi805)mod805Back();
  auto d=patch808New();bool assigned=false;if(d){patchCursor808=patch808Count()-1;patch808Assign();assigned=patchAssigned811(d->id);}
  check(assigned,"assigned_patch_marker");
  auto oldStartup=startupMode811;auto oldEnabled=autosaveEnabled811;auto oldMinutes=autosaveMinutes811;
  startupMode811=2;autosaveEnabled811=false;autosaveMinutes811=9;settingsSave811();startupMode811=0;autosaveEnabled811=true;autosaveMinutes811=1;settingsLoad811();
  check(startupMode811==2&&!autosaveEnabled811&&autosaveMinutes811==9,"startup_autosave_EEPROM_roundtrip");startupMode811=oldStartup;autosaveEnabled811=oldEnabled;autosaveMinutes811=oldMinutes;settingsSave811();
  if(storageWriteAvailable){const char*path="/URBANKAOZ/PRJ/.V811_TEST.UKP";if(!SD.exists(path)){
    strcpy(full805ForcedPath,path);char saved[128];bool ok=saveFullProject(saved,sizeof(saved));full805ForcedPath[0]=0;check(ok&&full805Validate(path),"new_full_format_validate");
    if(ok){m.outputs[0]={};ok=loadFullProject(path);check(ok&&mod806Unit(0).outputs[0].kind==MOD_PARAM811&&mod806Unit(0).outputs[0].number==17,"parameter_target_SD_roundtrip");}SD.remove(path);
  }else check(false,"temporary_path_in_use");}else check(false,"SD_required");
  newProject807();autosaveReady805=true;autosave807Baseline();midiClearStats810();
  Serial.printf("CHECK811,%s,OK=%u,FAILURES=%u\n",fail?"FAIL":"PASS",pass,fail);
}
