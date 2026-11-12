#pragma once
FLASHMEM void mod806Checks() {
  if(playing||songEngineActive||modUi805||external_psram_size<16){
    Serial.println("MODCHECK807,BUSY");return;
  }
  memcpy(fullMods805,modUnits805,sizeof(modUnits805));
  memcpy(fullMeta806,modMeta806,sizeof(modMeta806));memcpy(fullExtra810,modExtra810,sizeof(modExtra810));
  memcpy(fullDelays805,delays805,sizeof(delays805));
  uint8_t savedTrack=selectedTrack,patterns[16];bool muted[16];
  memcpy(patterns,activePattern,16);memcpy(muted,trackMuted,sizeof(muted));
  bool savedPatternMode=patternMode,savedLatch=menuStopLatched,savedAwait=screenUiAwaitRelease;
  int16_t savedStep=selectedStep;bool oldReady=autosaveReady805;
  autosaveReady805=false;selectedTrack=0;patternMode=false;selectedStep=-1;menuStopLatched=false;
  memset(activePattern,0,16);memset(trackMuted,0,sizeof(muted));
  memset(modUnits805,0,sizeof(modUnits805));memset(modMeta806,0,sizeof(modMeta806));mod805Invalidate();
  unsigned count=0,failed=0;
  ModUndo807 savedHistory[2];memcpy(savedHistory,modHistory807,sizeof(savedHistory));uint8_t savedHistoryCount=modHistoryCount807;
  bool savedDelete=deleteButtonActive;
  auto check=[&](bool ok,const char*name){++count;if(!ok){++failed;Serial.print("MODCHECK807,FAIL,");Serial.println(name);}};
  ModMean806 mean;mean.add(0);mean.add(60);check(mean.value()==30,"zero counted");
  playing=true;internalClockRunning=false;selectedTrack=6;
  mod805Open(0);mod805Screen(4);
  check(playing&&modUi805==MOD_OBJECT806,"add ENV while playing");
  auto&m=mod806Unit(0);check(m.inputs==(1u<<6),"default TRK07");
  mod806Roy(0,0,true);check(modUi805==MOD_SOURCE806,"OA opens source");
  mod805Back();mod806Roy(2,0,true);check(modUi805==MOD_DEST806,"OC adds destination");
  mod806Roy(0,1,false);mod806Roy(0,0,true);
  mod806Roy(1,3,false);mod806Roy(1,0,true);mod806Roy(3,73,false);mod806Roy(3,0,true);
  mod805Ky(12,60);mod805Back();
  check(m.outputs[0].kind==MOD_CC&&m.outputs[0].track==9&&m.outputs[0].number==74&&m.outputs[0].depth==60,"MIDI channel CC depth");
  mod806Roy(2,0,true);mod806Roy(3,10,false);mod805Back();
  check(m.outputs[0].number==74,"J1 rolls back draft");
  mod806Delete();mod806Roy(2,0,true);
  check(!m.outputs[0].enabled&&!deleteButtonActive&&modUi805==MOD_OBJECT806,"delete output no prompt exit mode");
  check(mod807Undo()&&m.outputs[0].enabled,"undo output");
  mod806Delete();mod806Roy(0,0,true);check(!m.inputs,"delete last source");
  check(strstr(menuScreenRaw[1],"Add")!=nullptr,"empty column Add");
  check(mod807Undo()&&m.inputs==(1u<<6),"undo source");
  mod806CopyPattern(6,0,7,0);mod805Back();mod805Screen(6);
  check(modUi805==MOD_REMOVE806,"remove list separate");
  mod806Roy(0,0,true);
  check(!m.enabled&&!(modMeta806[0].tracks&(1u<<7)),"remove entire shared object");
  check(mod807Undo()&&m.enabled&&(modMeta806[0].tracks&(1u<<7)),"undo object");
  mod805Open(1);mod805Screen(4);auto&l=mod806Unit(1);
  mod806Roy(0,0,true);mod806Roy(2,2,false);mod805Back();check(modExtra810[1].mode==0,"Free draft rollback");
  mod806Roy(0,0,true);mod806Roy(2,2,false);mod806Roy(2,0,true);mod805Back();
  l.inputs=0;mod805Invalidate();check(modExtra810[1].mode==2&&isfinite(mod805UnitValue(1,192)),"Free without notes");modExtra810[1].mode=0;
  m.outputs[0]={MOD_RLV,3,0,1,60};l.outputs[0]=m.outputs[0];mod805Invalidate();
  auto seed=[&](uint16_t key,int64_t at,float value){uint8_t slot=at%96?0:1+(at/96)%7;modValues805[key][slot]={at,modEpoch805,value};};
  seed(0,96,0);seed(1,96,1);check(mod805Offset(3,MOD_RLV,96)==30,"engine mean 0 and 60");
  check(mod806Conflict(m.outputs[0]),"nonblocking collision");
  mod805Back();mod805Back();playing=false;
  memcpy(modUnits805,fullMods805,sizeof(modUnits805));memcpy(modMeta806,fullMeta806,sizeof(modMeta806));
  memcpy(delays805,fullDelays805,sizeof(delays805));memcpy(modExtra810,fullExtra810,sizeof(modExtra810));
  memcpy(activePattern,patterns,16);memcpy(trackMuted,muted,sizeof(muted));
  memcpy(modHistory807,savedHistory,sizeof(savedHistory));modHistoryCount807=savedHistoryCount;deleteButtonActive=savedDelete;
  selectedTrack=savedTrack;patternMode=savedPatternMode;selectedStep=savedStep;
  autosaveReady805=oldReady;menuStopLatched=savedLatch;screenUiAwaitRelease=savedAwait;
  modUi805=0;modSelected806=UINT16_MAX;modDirty806=0;mod805Invalidate();
  DISPLAY_SERIAL.println("CUSTOM,OFF");DISPLAY_SERIAL.println("UI|GAME");
  neoDisplayDirty=displayValueDirty=displayBpmDirty=true;
  Serial.print(failed?"MODCHECK807,FAIL,":"MODCHECK807,PASS,");Serial.print(count);Serial.print(",FAILURES=");Serial.println(failed);
}

