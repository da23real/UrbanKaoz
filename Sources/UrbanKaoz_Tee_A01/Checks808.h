#pragma once
// Diagnostic test: explicitly discards the disposable RAM project. No user SD file is overwritten.
FLASHMEM void checks808(){
  if(playing){Serial.println("CHECK808,STOP_FIRST");return;}
  autosaveReady805=false;newProject807();
  unsigned count=0,failed=0;
  auto test=[&](bool pass,const char*name){++count;if(!pass){++failed;Serial.print("CHECK808,FAIL,");Serial.println(name);}};
  auto d=patch808New();auto q=d?patch808Add(d):nullptr;
  test(d&&q,"PSRAM allocation");
  if(!d||!q){Serial.println("CHECK808,FAIL,ALLOCATION");return;}
  const uint32_t id=d->id,param=q->id;q->number=74;
  patchDb808.context[0][0].patch=id;patch808Set(patchDb808.context[0][0],param,37);
  patchDb808.context[3][0].patch=id;patch808Set(patchDb808.context[3][0],param,91);
  patch808Copy(0,0,0,1);patch808Set(patchDb808.context[0][2],param,63);
  uint16_t v=0;
  test(patch808Get(patchDb808.context[0][0],param,v)&&v==37,"track independent");
  test(patch808Get(patchDb808.context[3][0],param,v)&&v==91,"shared patch independent");
  test(patch808Get(patchDb808.context[0][2],param,v)&&v==63,"pattern independent");
  for(int i=0;i<20;++i)test(patch808Add(d)!=nullptr,"unbounded parameter list");
  patch808Open();patch808Roy(0,0,true);test(patchUi808==1,"assign keeps list");
  patch808Screen(6);patch808Roy(0,19,false);patch808Roy(0,0,true);test(patchUi808==3,"long list edit");
  while(patchUi808)patch808Back();
  fileBrowserMode=FILE_BROWSER_LOAD;menuStopLatched=true;closeFileBrowser();test(!menuNavigationOnly(),"Load releases transport");
  mod805Open(0);mod805Screen(4);uint16_t key=modSelected806;
  mod806Roy(2,0,true);mod806Roy(0,1,false);mod806Roy(0,0,true);mod806Roy(3,6,false);mod806Roy(3,0,true);mod805Ky(12,60);mod805Back();
  mod806Roy(2,0,true);mod806Roy(3,67,false);
  test(mod808EffectiveUnit(key).outputs[0].number==7,"uncommitted CC silent");mod805Back();
  dispatchStableButtonPress(EDIT_BUTTON_BITS[EDIT_BUTTON_DELETE]);mod806Roy(2,99,false);
  test(modDestCursor807==0&&deleteButtonActive,"Delete excludes Add");
  mod806Roy(2,0,true);test(!mod806Unit(key).outputs[0].enabled&&!deleteButtonActive,"delete removes exits");test(mod807Undo(),"delete undo");
  dispatchStableButtonPress(10);test(patternMode&&!modUi805,"Pattern accessible from MOD");
  // Test only a dedicated non-existing temporary file, never overwrite a project.
  const char*path="/URBANKAOZ/PRJ/.PATCH808.TEST";
  if(storageWriteAvailable&&!SD.exists(path)){
    File f=SD.open(path,FILE_WRITE);bool ok=f&&patch808Write(f);f.flush();f.close();
    File r=SD.open(path,FILE_READ);ok=ok&&r&&r.size()==patch808DiskSize()&&patch808Read(r,true);r.close();
    test(ok,"SD patch roundtrip");test(patch808Get(patchDb808.context[3][0],param,v)&&v==91,"SD value restored");SD.remove(path);
  }else Serial.println("CHECK808,SKIP,SD_PATCH");
  newProject807();autosaveReady805=true;autosave807Baseline();
  Serial.print(failed?"CHECK808,FAIL,":"CHECK808,PASS,");Serial.print(count);Serial.print(",FAILURES=");Serial.println(failed);
}
