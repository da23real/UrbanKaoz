#pragma once
static void checks902(){
 if(engine.playing||engine.count||engine.pendingJobs()){Serial.println("CHECK902,BUSY");return;}
 for(const auto&g:engine.gates)if(g.used){Serial.println("CHECK902,BUSY");return;}
 for(const auto&o:outputs)if(o.active||o.count||o.haveAfter){Serial.println("CHECK902,BUSY");return;}
 unsigned pass=0,fail=0;auto check=[&](bool ok,const char*n){ok?++pass:++fail;Serial.printf("CHECK902,%s,%s\n",ok?"PASS":"FAIL",n);};
 uint32_t oldEchoes=engine.echoes,oldToken=engine.nextToken,oldOverflow=engine.overflow,oldLate=engine.lateMaxUs;
 uint16_t audible=engine.audible;engine.playing=true;engine.audible=65535;
 Message m0={0x90,60,100,ECHO_ON,0,1000,0,0},m7=m0;m7.output=7;
 check(engine.emitted(m0,100)&&engine.emitted(m7,100),"two_ports_same_note");
 auto token=engine.owners[7][0][60];
 check(engine.emitted({0x80,60,0,DIRECT,0,0,0,0},200)&&engine.owners[7][0][60]==token,"OFF_does_not_cut_other_port");
 engine.service(1100);Message m{};check(engine.pop(m)&&m.output==7&&m.s==0x80,"gate_returns_original_port");engine.emitted(m,1100);
 Delay d={0,1000,500,0,0,61,100,1,127,0,7};check(engine.delay(d,2000),"delay_accepts_OUT8");engine.service(3000);
 check(engine.pop(m)&&m.output==7&&m.s==0x90,"delay_routes_OUT8");engine.emitted(m,3000);engine.cancel();engine.service(3001);
 check(engine.pop(m)&&m.output==7&&m.s==0x80,"cancel_releases_original_port");engine.emitted(m,3001);
 d.output=8;check(!engine.delay(d,0),"invalid_output_rejected");
 uint8_t bytes[4]={0xb0,99,9,128},frame[140];unsigned n=encode(MIDI,31,bytes,4,frame);Decoder test;
 bool decoded=false;for(unsigned i=0;i<n;++i)test.put(frame[i],[&](uint8_t kind,uint16_t seq,const uint8_t*p,unsigned size){decoded=kind==MIDI&&seq==31&&size==4&&!memcmp(p,bytes,4);});
 check(decoded&&!test.errors,"wire_roundtrip_output_mask");
 frame[n-2]^=1;for(unsigned i=0;i<n;++i)test.put(frame[i],[](uint8_t,uint16_t,const uint8_t*,unsigned){});check(test.errors==1,"CRC_rejects_corruption");
 engine.playing=false;engine.audible=audible;engine.echoes=oldEchoes;engine.nextToken=oldToken;engine.overflow=oldOverflow;engine.lateMaxUs=oldLate;
 Serial.printf("CHECK902,SUMMARY,PASS=%u,FAIL=%u\n",pass,fail);
}
