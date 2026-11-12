#pragma once
FLASHMEM void checks816(){
 if(playing||!trackContext815()||kyBtnDown[0]){Serial.println("CHECK816,BUSY");return;}
 uint16_t pass=0,fail=0;auto check=[&](bool ok,const char*name){if(ok)++pass;else ++fail;Serial.printf("CHECK816,%s,%s\n",ok?"OK":"FAIL",name);};
 check(hc165PressDebounceMs==15&&hc165ReleaseDebounceMs==15&&roySwitchDebounceMs==15&&picoButtonDebounceMs==15&&ky23Debounce816==15,"defaults_15ms");
 Button816 b;b.sample(true,100);check(!b.poll(114,15)&&b.poll(115,15)&&b.stable,"press_15ms");b.sample(false,120);check(!b.poll(134,15)&&b.poll(135,15)&&!b.stable,"release_15ms");
 b.sample(true,200);b.sample(false,204);check(!b.poll(250,15),"bounce_rejected");
 bool muted0=trackMuted[0],muted1=trackMuted[1];uint8_t nav=trackNav815;trackNav815=2;
 auto ky=[](bool down){char line[20];snprintf(line,sizeof(line),"P1,B,0,%u",down);handlePicoLine(1,line);};
 auto pad=[](uint8_t p){onPadEdge(0,p,SEESAW_KEYPAD_EDGE_RISING);onPadEdge(0,p,SEESAW_KEYPAD_EDGE_FALLING);};
 trackMuted[0]=trackMuted[1]=false;ky(true);pad(0);ky(false);check(trackMuted[0]&&!trackMuted[1],"mute_persists_after_KY_release");
 ky(true);check(trackMuted[0],"KY_press_does_not_unmute");pad(1);ky(false);check(trackMuted[0]&&trackMuted[1],"two_tracks_muted_independently");
 ky(true);pad(0);ky(false);check(!trackMuted[0]&&trackMuted[1],"unmute_only_target");
 trackMuted[0]=muted0;trackMuted[1]=muted1;trackNav815=nav;
 uint16_t key=0;while(key<64&&mod806Unit(key).enabled)key+=2;
 if(key<64){auto saved=mod806Unit(key);auto extra=modExtra810[key];uint16_t oldSelected=modSelected806;uint8_t oldType=modType805;
  modSelected806=key;modType805=0;modUi805=MOD_OBJECT806;auto&m=mod806Unit(key);m={};m.enabled=1;m.attack=10;
  applyParamDeltaFromKy(2,2);check(m.attack==11,"MOD_one_step");kyBtnDown[2]=true;applyParamDeltaFromKy(2,20);kyBtnDown[2]=false;check(m.attack==21,"MOD_held_ten_steps");
  mod806EditDest(0,true);check(m.outputs[0].kind==MOD_PATCH810&&m.outputs[0].enabled&&modFields806[0]==2,"new_destination_patch");
  const int order[]={3,1,0};bool ordered=true;for(int type:order){modRouteFields810(false,0,1);ordered&=modFields806[0]==type;}check(ordered,"destination_patch_mod_midi_track");
  modUi805=0;modDirty806=0;modDepthSlot806=-1;mod806Unit(key)=saved;modExtra810[key]=extra;modSelected806=oldSelected;modType805=oldType;mod805Invalidate();
 }else check(false,"free_mod_slot");
 char oldName[17];memcpy(oldName,lastFileName816,17);rememberFileName816("/URBANKAOZ/PRJ/projict.UKP");check(!strcmp(lastFileName816,"projict"),"last_filename_without_extension");memcpy(lastFileName816,oldName,17);
 showGameScreen();neoDisplayDirty=displayValueDirty=true;
 Serial.printf("CHECK816,%s,PASS=%u,FAIL=%u\n",fail?"FAIL":"PASS",pass,fail);
}
