#pragma once
// Explicit test command: RAM project is disposable; dedicated temporary files only.
FLASHMEM void itemChecks809(){
  if(playing){Serial.println("CHECK809,STOP_FIRST");return;}
  if(!storageWriteAvailable){Serial.println("CHECK809,SD_REQUIRED");return;}
  autosaveReady805=false;newProject807();unsigned count=0,fail=0;
  auto check=[&](bool ok,const char*label){++count;if(!ok){++fail;Serial.print("CHECK809,FAIL,");Serial.println(label);}};
  itemSeed809();File f=SD.open("/URBANKAOZ/PATCH/ARminimal.UKH",FILE_READ);Item809 ar;
  bool ok=f&&itemRead809(f,ar,ITEM_PATCH809);f.close();check(ok,"AR patch on SD");
  if(!ok){itemFree809(ar);Serial.println("CHECK809,FAIL,AR_FILE");autosaveReady805=true;return;}
  check(patch808ParamCount(ar.def)==3,"three parameters");check(ar.def->params->number==7&&ar.def->params->next->number==74&&ar.def->params->next->next->number==75,"AR MIDI mapping");
  check(itemInstallPatch809(ar),"install patch definition");itemFree809(ar);itemReturnPatch809=0;
  auto d=patch808At(0);auto q=d->params;selectedTrack=0;patternMode=false;patch808Open();patch808Assign();
  auto&c=patchDb808.context[0][0];check(c.patch==d->id&&!c.values,"assign leaves undefined values");
  patch808Set(c,q->id,91);patch808Set(c,q->next->id,45);patch808Set(c,q->next->next->id,0);
  playing=true;internalClockRunning=true;check(itemOpen809(true)&&itemChoosing809&&playing,"Save contextual live chooser");
  itemRoy809(0,1,false);itemRoy809(0,0,true);check(itemKind809==ITEM_PRESET809&&fileBrowserMode==FILE_BROWSER_SAVE&&playing,"Save preset live");
  Item809 preset;check(itemCapture809(preset),"capture current values");const char*path="/URBANKAOZ/PRESET/.V809_TEST.UKV";
  SD.mkdir("/URBANKAOZ/PRESET");bool own=!SD.exists(path);check(own&&itemAtomicWrite809(path,preset),"atomic preset SD write");
  Item809 loaded;f=SD.open(path,FILE_READ);ok=f&&itemRead809(f,loaded,ITEM_PRESET809);f.close();check(ok,"preset SD read");
  patch808Set(c,q->id,1);check(itemInstallPreset809(loaded),"matching preset install");uint16_t value=0;check(patch808Get(c,q->id,value)&&value==91,"preset restores volume");
  q->number=8;patch808Set(c,q->id,37);check(!itemInstallPreset809(loaded),"mismatched mapping refused");check(patch808Get(c,q->id,value)&&value==37,"refusal preserves values");q->number=7;
  itemPatternMode809=true;patternSelected[0][0]=patternSelected[1][0]=true;
  check(!itemInstallPreset809(loaded),"mixed incompatible selection refused");check(patch808Get(c,q->id,value)&&value==37,"multi-target transaction");
  patchDb808.context[1][0].patch=d->id;check(itemInstallPreset809(loaded),"multi-pattern compatible load");
  check(patch808Get(patchDb808.context[1][1],q->id,value)&&value==91,"second pattern values");
  patternSelected[0][0]=patternSelected[1][0]=false;itemPatternMode809=false;itemFree809(loaded);if(own)SD.remove(path);
  check(itemBack809()&&patchUi808==1&&playing,"J1 restores patch context live");while(patchUi808)patch808Back();playing=internalClockRunning=false;
  patchBank808=true;check(!itemOpen809(true),"EP home keeps full Save even on KY patch bank");patchBank808=false;
  mod805Open(0);mod805Screen(4);uint16_t key=modSelected806;mod806Unit(key).outputs[0]={MOD_CC,3,7,1,42};
  playing=internalClockRunning=true;check(itemOpen809(true)&&itemKind809==ITEM_ENV809&&playing,"ENV Save keeps transport");
  Item809 env;check(itemCapture809(env),"capture curve");env.curve.attack=8;env.rat=12;check(itemInstallMod809(env),"load curve into existing identity");
  check(mod806Unit(key).outputs[0].depth==42&&mod806Unit(key).outputs[0].track==3,"curve load preserves routes");check(modRat808[key]==12,"curve RAT load");
  itemClose809();check(modUi805==MOD_OBJECT806&&modSelected806==key&&playing,"ENV return to same modulation");
  while(modUi805)mod805Back();mod805Open(1);check(itemOpen809(false)&&itemMod809==UINT16_MAX,"LFO list load creates new");
  env.kind=ITEM_LFO809;env.curve.curve=1;check(itemInstallMod809(env),"LFO imported from list");auto lk=modSelected806;
  check(lk%2==1&&mod806Unit(lk).inputs==(1u<<modOwnerTrack805)&&mod806Unit(lk).outputs[0].enabled==0,"new LFO current source no destination");itemClose809();while(modUi805)mod805Back();
  mod805Open(2);check(itemOpen809(false)&&itemKind809==ITEM_FX809&&playing,"FX Load live");itemClose809();while(modUi805)mod805Back();playing=internalClockRunning=false;
  patch808Open();patch808Screen(6);patch808Name(0);nameSync809(true);uint32_t sid=nameSession809;
  char command[100];snprintf(command,sizeof(command),"NAMEEDIT,SET,%lu,2,AR test",(unsigned long)sid);check(nameCommand809(command)&&!strcmp(patchNameDraft808,"AR test"),"phone edits same draft");
  check(!strcmp(d->name,"ARminimal"),"phone draft not committed");snprintf(command,sizeof(command),"NAMEEDIT,COMMIT,%lu",(unsigned long)sid);nameCommand809(command);check(!strcmp(d->name,"AR test"),"phone validates name");
  patch808Name(0);snprintf(command,sizeof(command),"NAMEEDIT,SET,%lu,0,Stale",(unsigned long)sid);nameCommand809(command);check(strcmp(patchNameDraft808,"Stale")!=0,"stale session rejected");
  patch808Back();while(patchUi808)patch808Back();check(!menuStopLatched,"no orphan transport latch");
  newProject807();autosaveReady805=true;autosave807Baseline();
  Serial.print(fail?"CHECK809,FAIL,":"CHECK809,PASS,");Serial.print(count);Serial.print(",FAILURES=");Serial.println(fail);
}
