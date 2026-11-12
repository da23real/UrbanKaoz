#pragma once
// Explicit diagnostic: disposable RAM project, dedicated temporary file only.
FLASHMEM void checks810(){
  if(playing){Serial.println("CHECK810,STOP_FIRST");return;}
  autosaveReady805=false;newProject807();uint16_t pass=0,fail=0;
  auto check=[&](bool ok,const char*name){if(ok)++pass;else ++fail;Serial.printf("CHECK810,%s,%s\n",ok?"OK":"FAIL",name);};
  check(fabsf(curve810(0,100))<0.0001f&&fabsf(curve810(1,-100)-1)<0.0001f,"curve_endpoints");
  check(curve810(.25f,50)>.25f&&curve810(.25f,-50)<.25f,"curve_sign");
  ModUnit805 m={};ModExtra810 e={};m.attack=10;m.decay=10;m.sustain=64;
  check(fabsf(held810(5,m,e)-.5f)<.0001f,"attack_ticks");
  check(fabsf(held810(20,m,e)-64/127.0f)<.0001f,"sustain");
  check(modLimit810(MOD_MIDI_BEND806)==8192&&modLimit810(MOD_TIM)==96,"depth_ranges");
  midiReset810();uint32_t began=micros();
  for(int i=0;i<10000;++i)midiOffer810(0,7,i%128);
  check(midiCounters810.pending==1&&midiCounters810.peak==1&&midiCounters810.replaced==9999,"10000_changes_one_pending");
  Serial.printf("CHECK810,OFFER_10000_US,%lu\n",(unsigned long)(micros()-began));midiReset810();
  mod805Open(0);mod805Screen(4);uint16_t key=modSelected806;auto&env=mod806Unit(key);
  mod805Ky(2,1);check(env.attack==2,"attack_fine_one_tick");mod805Ky(3,50);check(modExtra810[key].attackCurve==50,"attack_curve_KY4");
  uint8_t local=0;for(;local<8;++local)if(kyFromPicoIndex(1,local)==2)break;
  if(local<8){char event[40];snprintf(event,40,"P1,B,%u,1",local);handlePicoLine(1,event);
    picoRxStats[0].valueKnown[local]=false;int old=env.attack;
    snprintf(event,40,"P1,E,%u,1,1",local);handlePicoLine(1,event);
    check(abs((int)env.attack-old)==10,"held_rotation_ten_ticks");
    snprintf(event,40,"P1,B,%u,0",local);handlePicoLine(1,event);check(!kyBtnDown[2]&&modUi805==MOD_OBJECT806,"release_no_extra_click");
  }else check(false,"KY3_mapping");
  env.outputs[0]={MOD_CC,0,7,1,60};mod806EditDest(0,false);mod806Roy(3,67,false);
  check(env.outputs[0].number==7,"CC_draft_inaudible");mod805Back();
  auto&lfo=mod806Unit(1);lfo.enabled=1;lfo.rate=96;lfo.shape=4;lfo.outputs[0]={MOD_CC,3,7,1,40};mod806Associate(modMeta806[1],0,0,true);
  env.outputs[0]={MOD_DEPTH810,0,8,1,60};mod805Invalidate();
  check(modGraphValid810(modUnits805),"mod_chain_acyclic");
  ModRoute805 loop={MOD_DEPTH810,0,0,1,10};check(!modAcyclic810(1,0,loop),"mod_feedback_refused");
  modExtra810[1]={-25,50,75,1,270};ccGap810=9;ccBudget810=350;
  while(modUi805)mod805Back();modDirty806=0;
  if(storageWriteAvailable){
    const char*path="/URBANKAOZ/PRJ/.V810_TEST.UKP";
    if(!SD.exists(path)){
      strcpy(full805ForcedPath,path);char saved[128];bool ok=saveFullProject(saved,sizeof(saved));full805ForcedPath[0]=0;
      check(ok&&full805Validate(path),"full_SD_validate");
      if(ok){modExtra810[1]={};ccGap810=2;ccBudget810=500;ok=loadFullProject(path);
        check(ok&&modExtra810[1].phase==270&&modExtra810[1].mode==1&&modExtra810[1].releaseCurve==75,"full_curve_phase_restore");
        check(ok&&ccGap810==9&&ccBudget810==350&&mod806Unit(0).outputs[0].kind==MOD_DEPTH810,"full_config_route_restore");
      }
      SD.remove(path);
    }else check(false,"temporary_path_in_use");
  }else check(false,"SD_required");
  newProject807();autosaveReady805=true;autosave807Baseline();midiClearStats810();
  Serial.printf("CHECK810,%s,OK=%u,FAILURES=%u\n",fail?"FAIL":"PASS",pass,fail);
}
