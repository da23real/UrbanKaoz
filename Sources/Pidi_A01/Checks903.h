#pragma once
static void checks903(){
 if(engine.playing||engine.count||engine.pendingJobs()){Serial.println("CHECK903,BUSY");return;}
 for(auto&g:engine.gates)if(g.used){Serial.println("CHECK903,BUSY_GATE");return;}
 for(auto&o:outputs)if(o.active||o.count||o.haveAfter){Serial.println("CHECK903,BUSY_TX");return;}
 unsigned pass=0,fail=0;auto check=[&](bool ok,const char*n){ok?++pass:++fail;Serial.printf("CHECK903,%s,%s\n",ok?"PASS":"FAIL",n);};
 auto oldEchoes=engine.echoes,oldToken=engine.nextToken,oldOverflow=engine.overflow,oldLate=engine.lateMaxUs;auto oldAudible=engine.audible;bool oldContinue=engine.continuePaused;
 engine.audible=65535;engine.playing=true;engine.continuePaused=true;
 Delay d{0,1000,500,0,0,61,100,2,127,0,7};Message m{};
 engine.delay(d,0);engine.playing=false;engine.service(1000);
 check(engine.pop(m)&&m.kind==ECHO_ON&&m.output==7,"pending_delay_runs_while_paused");engine.emitted(m,1000);
 engine.service(1500);check(engine.pop(m)&&m.kind==ECHO_OFF&&m.output==7,"natural_gate_finishes_while_paused");engine.emitted(m,1500);
 cut903(2);check(engine.pendingJobs()==1,"notes_cut_preserves_future_repeats");
 engine.service(2000);check(engine.pop(m)&&m.kind==ECHO_ON,"future_repeat_after_notes_cut");engine.emitted(m,2000);
 cut903(1);engine.service(2001);check(engine.pop(m)&&m.kind==ECHO_OFF,"delay_cut_releases_echo_gate");engine.emitted(m,2001);
 engine.playing=true;engine.delay(d,3000);cut903(1);check(engine.pendingJobs()==0,"delay_cut_cancels_jobs");
 outputs[0].active=true;outputs[0].pos=1;outputs[0].current={0x90,64,100,ECHO_ON,0,1000,0,0};cancelDelays();
 check(outputs[0].active&&outputs[0].cutCurrent,"partial_echo_finishes_then_releases");outputs[0].active=outputs[0].cutCurrent=false;outputs[0].pos=0;
 active903[7][3][8]=4;cut903(2);check(releaseNext903(7,m)&&m.s==0x83&&m.a==66&&m.output==7,"notes_cut_targets_original_port_channel");active903[7][3][8]=0;
 check(engine.count==0,"no_test_output_transmitted");serviceRelease903();
 engine.playing=false;engine.continuePaused=oldContinue;engine.audible=oldAudible;engine.echoes=oldEchoes;engine.nextToken=oldToken;engine.overflow=oldOverflow;engine.lateMaxUs=oldLate;
 Serial.printf("CHECK903,SUMMARY,PASS=%u,FAIL=%u\n",pass,fail);
}
