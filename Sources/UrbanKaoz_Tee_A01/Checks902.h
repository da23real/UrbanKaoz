#pragma once
FLASHMEM void checks902(){
 if(playing||nrpnPending810||midiCounters810.pending){Serial.println("CHECK902,BUSY");return;}
 for(const auto&n:activeNotes)if(n.active){Serial.println("CHECK902,BUSY");return;}
 unsigned pass=0,fail=0;auto check=[&](bool ok,const char*name){ok?++pass:++fail;Serial.printf("CHECK902,%s,%s\n",ok?"PASS":"FAIL",name);};
 uint8_t oldOut=trackOut902[0];uint16_t oldContext=midiContext902;
 trackOut902[0]=0;check(outMask902(0)==0,"OFF");
 for(uint8_t i=1;i<=8;++i){trackOut902[0]=1u<<(i-1);check(outMask902(0)==(1u<<(i-1)),"OUT_MASK");}
 trackOut902[0]=oldOut;
 {MidiScope902 a(2);{MidiScope902 b(128);check(midiMask902(0)==128,"nested_scope");}check(midiMask902(0)==2,"scope_restore");}
 check(midiContext902==oldContext,"outer_scope_restore");
 auto a=midiPending810[0][7],b=midiPending810[16][7];auto counters=midiCounters810;
 midiPending810[0][7]={};midiPending810[16][7]={};
 {MidiScope902 route(1);midiOffer810(0,7,23);}
 {MidiScope902 route(2);midiOffer810(0,7,91);}
 check(midiPending810[0][7].value==23&&midiPending810[16][7].value==91,"CC_same_channel_independent_outputs");
 midiPending810[0][7]=a;midiPending810[16][7]=b;midiCounters810=counters;
 {MidiScope902 route(1);midiNrpn810(0,1234,14,12345);}
 {MidiScope902 route(128);midiNrpn810(0,1234,7,97);}
 check(nrpnPending810&&nrpnPending810->output==0&&nrpnPending810->value==12345&&nrpnPending810->next&&nrpnPending810->next->output==7&&nrpnPending810->next->value==97,"NRPN_independent_7_14bit");
 while(nrpnPending810){auto*p=nrpnPending810;nrpnPending810=p->next;extmem_free(p);}midiCounters810=counters;
 ModUnit805 env={};env.hold=4;env.attack=8;env.decay=12;env.sustain=70;env.release=16;
 auto extra=modExtra810[0];modExtra810[0]={};
 const int at[6]={1,5,13,25,41,57};const float sustain=70/127.0f;
 const float expected[6]={0,0.125f,1-(1-sustain)/12,sustain,sustain*15/16,0};
 for(unsigned i=0;i<6;++i)check(fabsf(modValue810(env,0,at[i],0,0,40)-expected[i])<0.0001f,"envelope_value");
 modExtra810[0]=extra;
 for(uint8_t role=8;role<12;++role)check(&colourChoice902(role)==&modeColors902[role-8],"mode_colour_address");
 check(sizeof(PersistedSeqSettings)==48&&sizeof(Settings902)<=32,"settings_layout");
 Serial.printf("CHECK902,SUMMARY,PASS=%u,FAIL=%u\n",pass,fail);
}
