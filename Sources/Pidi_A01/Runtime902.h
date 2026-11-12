#pragma once
#include <Arduino.h>
#include "Dac46.h"
#include "MidiAnalyzer28.h"
#include "FinalMidi54.h"
#include <USB.h>
#include <SerialPIO.h>
#include "DelayEngine827.h"
#include "Effects32.h"
#include "DelayUsb07.h"
#define DELAY_ECHO07(slot,rep,ch,note,vel,gate,now) delayUsb07::echo(slot,rep,ch,note,vel,gate,now)
#include "DelayTargets55.h"
static void delayTarget55(const delay55::Target&,int,uint64_t,uint8_t,uint8_t,bool=false);
#define DELAY_TARGET55(t,v,end,owner,slot,manual) delayTarget55(t,v,end,owner,slot,manual)
static unsigned delayReserve58=0;
#define DELAY_RESERVE58(n) (delayReserve58=(n))
static void modDelayTrigger58(uint8_t,uint32_t);
#define DELAY_TRIGGER58(owner,gate) modDelayTrigger58(owner,gate)
#include "MultiDelay925.h"
using namespace pidi827;
// OUT1..8: 4 (GP2), 5 (GP3), 6 (GP4), 7 (GP5), 9 (GP6), 10 (GP7), 11 (GP8), 12 (GP9).
// RS485: 15 (GP11/direction), 16 (GP12/TX0), 17 (GP13/RX0).
static SerialPIO out1(2,NOPIN),out2(3,NOPIN),out3(4,NOPIN),out4(5,NOPIN),out5(6,NOPIN),out6(7,NOPIN),out7(8,NOPIN),out8(9,NOPIN);
static SerialPIO* ports[8]={&out1,&out2,&out3,&out4,&out5,&out6,&out7,&out8};
static Engine engine;static multi925::Engine multi;static Decoder decoder;
struct PortTx {Message queue[64],current{},after{};uint8_t head=0,tail=0,count=0,pos=0,length=3;bool active=false,haveAfter=false,cutCurrent=false;uint8_t panicChannel=16;};
static PortTx outputs[8];
static uint8_t active903[8][16][16]={},release903[8][16][16]={};
static bool releasesPending903=false;
static bool ready=false,haveSequence=false;static uint16_t expected=0;
static uint32_t rejectedNotes15=0,localReleases15=0;
static uint32_t frames=0,gaps=0,notes[8]={},offs[8]={},requests=0,panics=0,lastLinkMs=0,lastMicros=0,noteLedUntil=0;
static uint64_t wall=0;static uint32_t resetRequests925=0,linkTimeouts925=0,invalidFrames925=0;
static uint64_t timeUs(){uint32_t now=micros();wall+=(uint32_t)(now-lastMicros);lastMicros=now;return wall;}
#include "ModRuntime54.h"
#include "DelayRuntime55.h"
static void pinTestEnd33();
static bool pinTestActive33=false;static uint8_t pinTestPort33=0,pinTestHigh33=1;static uint32_t pinTestSeen33=0;
static void panic(){delayLeases55.cut();remote54.reset();remoteCommitArmed54=false;pinTestEnd33();effects32::clear();delayUsb07::cancel();
 multi.cancel();ready=false;haveSequence=false;engine.playing=false;engine.continuePaused=false;engine.cancel();engine.count=engine.head=engine.tail=0;
 for(auto&g:engine.gates)g.used=false;memset(engine.owners,0,sizeof(engine.owners));
 for(auto&o:outputs){o.count=o.head=o.tail=0;o.haveAfter=false;o.panicChannel=0;}++panics;
}
static void cancelDelays(){delayClear55();effects32::clearArp();delayUsb07::cancel();
 multi.cancel();engine.cancel();for(auto&o:outputs){if(o.haveAfter&&(o.after.kind==ECHO_ON||o.after.kind==MANUAL_ECHO_ON||o.after.kind==ARP_ON32))o.haveAfter=false;unsigned count=o.count;
  for(unsigned i=0;i<count;++i){Message m=o.queue[o.head];o.head=(o.head+1)%64;--o.count;if(m.kind!=ECHO_ON&&m.kind!=MANUAL_ECHO_ON&&m.kind!=ARP_ON32){o.queue[o.tail]=m;o.tail=(o.tail+1)%64;++o.count;}}
  if(o.active&&(o.current.kind==ECHO_ON||o.current.kind==MANUAL_ECHO_ON||o.current.kind==ARP_ON32)){if(!o.pos)o.active=false;else o.cutCurrent=true;}
 }
}
static bool isOn903(const Message&m){return (m.s&0xf0)==0x90&&m.b;}
static void cut903(uint8_t flags){
 if(flags&1)cancelDelays();
 if(!(flags&2))return;effects32::clear();
 for(unsigned port=0;port<8;++port){auto&o=outputs[port];
  for(unsigned c=0;c<16;++c)for(unsigned b=0;b<16;++b)release903[port][c][b]|=active903[port][c][b];
  if(o.haveAfter&&isOn903(o.after))o.haveAfter=false;
  if(o.active&&isOn903(o.current)){if(o.pos)o.cutCurrent=true;else o.active=false;}
  unsigned n=o.count;for(unsigned i=0;i<n;++i){Message m=o.queue[o.head];o.head=(o.head+1)%64;--o.count;if(!isOn903(m)){o.queue[o.tail]=m;o.tail=(o.tail+1)%64;++o.count;}}
 }
 unsigned n=engine.count;for(unsigned i=0;i<n;++i){Message m=engine.queue[engine.head];engine.head=(engine.head+1)%2048;--engine.count;if(!isOn903(m))engine.push(m);}
 releasesPending903=true;
}
static bool releaseNext903(uint8_t port,Message&m){
 if(!releasesPending903)return false;
 for(uint8_t c=0;c<16;++c)for(uint8_t b=0;b<16;++b)if(release903[port][c][b]){
  uint8_t bit=__builtin_ctz((unsigned)release903[port][c][b]);release903[port][c][b]&=~(1u<<bit);
  m={uint8_t(0x80|c),uint8_t(b*8+bit),0,DIRECT,0,0,0,port};return true;
 }return false;
}
static void releaseAddress15(uint8_t port,uint8_t channel,uint8_t note){
 auto matches=[&](const Message&m){return m.output==port&&(m.s&15)==channel&&m.a==note&&isOn903(m);};
 unsigned count=engine.count;for(unsigned i=0;i<count;i++){Message m=engine.queue[engine.head];engine.head=(engine.head+1)%2048;--engine.count;if(!matches(m))engine.push(m);}
 auto&o=outputs[port];count=o.count;for(unsigned i=0;i<count;i++){Message m=o.queue[o.head];o.head=(o.head+1)%64;--o.count;if(!matches(m)){o.queue[o.tail]=m;o.tail=(o.tail+1)%64;++o.count;}}
 if(o.haveAfter&&matches(o.after))o.haveAfter=false;
 if(o.active&&matches(o.current)){if(o.pos)o.cutCurrent=true;else o.active=false;}
 release903[port][channel][note/8]|=1u<<(note%8);releasesPending903=true;++localReleases15;
}
#include "PinTest33.h"
static void receive(uint8_t type,uint16_t seq,const uint8_t*p,unsigned n){
 lastLinkMs=millis();
 if(type==HEARTBEAT&&!n){if(!haveSequence){haveSequence=true;expected=seq;}if(seq!=expected){++gaps;panic();return;}expected=seq+1;ready=true;++frames;return;}
 if(type==RESET&&!n){++resetRequests925;multi.reset();effects32::reset();panic();haveSequence=true;expected=seq+1;ready=true;++frames;return;}
 if(!haveSequence)return;if(seq!=expected){++gaps;panic();return;}++expected;++frames;
 if(type==50&&n==1&&p[0]<64){for(auto&e:multi.targetPool.entries)if(e.refs&&(e.target.kind==5||e.target.kind==6)&&e.target.identity/17==p[0])e.target.kind=delay55::NONE;for(auto&l:delayLeases55.leases)if(l.used&&(l.target.kind==5||l.target.kind==6)&&l.target.identity/17==p[0])l.used=false;for(auto&s:remote54.states)s.cached=false;return;}
 if(type==delay55::CONFIG&&n==sizeof(delay55::Config)){delay55::Config c;memcpy(&c,p,n);if(!multi.targets(c))++multi.rejected;else delayConfigure55(c);return;}
 if(modPacket54(type,p,n))return;
 if(type==PINTEST33){if(n==2&&(p[0]<8||p[0]==255)&&p[1]<2)pinTestSet33(p[0],p[1]);else ++invalidFrames925;return;}
 if(type==CV46){if(n!=8){++invalidFrames925;return;}uint16_t v[4];memcpy(v,p,8);for(auto x:v)if(x>4095){++invalidFrames925;return;}for(unsigned ch=0;ch<4;++ch){int i=remote54.address(4,0,0,ch,false);cvRemote54[ch]=i>=0?modFinal54(remote54.addresses[i]):v[ch];}dac46::set(cvRemote54);return;}
 if(pinTestActive33)return;
 if((type==MIDI&&n==4||type==MIDI32&&n==5&&(p[4]<16||p[4]==255))&&p[0]>=0x80&&p[0]<0xf0&&p[1]<128&&p[2]<128){
  if((p[0]&0xf0)==0x90&&p[2])for(uint8_t port=0;port<8;++port)if(p[3]&(1u<<port))delaySource57(port,p[0]&15,n==5?p[4]:255);
  analyzerPush28(p[0],p[1],p[2],p[3]);for(uint8_t port=0;port<8;++port)if(p[3]&(1u<<port))if(!modMidi54(p[0],p[1],p[2],port)&&!engine.push({p[0],p[1],p[2],DIRECT,uint8_t(n==5?p[4]:255),0,0,port})){
   if((p[0]&0xf0)==0x80||((p[0]&0xf0)==0x90&&!p[2])){releaseAddress15(port,p[0]&15,p[1]);}
   else if((p[0]&0xf0)==0x90)++rejectedNotes15;
  }
 }else if(type==FX32&&n==sizeof(harmony32::Packet)){harmony32::Packet v;memcpy(&v,p,n);if(v.track<16&&harmony32::valid(v.config)){effects32::configs[v.track]=v.config;for(auto&h:effects32::held)if(h.used&&h.arp&&h.track==v.track)h.used=false;for(auto&g:engine.gates)if(g.used&&g.track==v.track)g.due=0;}else ++invalidFrames925;
 }else if(type==CLOCK32&&n==sizeof(harmony32::Clock)){harmony32::Clock v;memcpy(&v,p,n);if(v.tickUs>=100&&v.tickUs<=200000&&v.running<2){if(effects32::running&&!v.running)effects32::clearArp();for(auto&h:effects32::held)if(h.used&&h.arp&&!(v.audible&(1u<<h.track)))h.used=false;for(auto&g:engine.gates)if(g.used&&g.arp32&&(!v.running||!(v.audible&(1u<<g.track))))g.due=0;effects32::audible=v.audible;effects32::tickUs=v.tickUs;effects32::running=v.running;}else ++invalidFrames925;
 }else if(type==EXP14&&n==2){uint16_t v;memcpy(&v,p,2);if(v>=1&&v<=100)expMaximum14=v;else ++invalidFrames925;
 }else if(type==LABEL07&&n==1&&p[0]<=16){delayUsb07::labelChannel=p[0];
 }else if(type==OBJECT925&&n==sizeof(Object925)){Object925 v;memcpy(&v,p,n);if(!multi.object(v))++multi.rejected;
 }else if(type==VALUE925&&n==sizeof(Value925)){Value925 v;memcpy(&v,p,n);if(!multi.value(v))++multi.rejected;
 }else if(type==ROUTE925&&n==sizeof(Route925)){Route925 v;memcpy(&v,p,n);if(!multi.routing(v))++multi.rejected;
 }else if(type==LOCKS925){if(!multi.overrides(p,n)){multi.lockCount=0;++multi.rejected;}
 }else if(type==NOTE925&&n==sizeof(Note925)){Note925 v;memcpy(&v,p,n);if(multi.source(v,timeUs(),engine.running(),engine.audible))++requests;else ++multi.rejected;
 }else if(type==STATE924&&n==sizeof(State924)){State924 v;memcpy(&v,p,n);if(v.playing>1||v.continuePaused>1){panic();return;}if(engine.running()&&!v.playing&&!v.continuePaused)cancelDelays();engine.audible=v.audible;engine.playing=v.playing;engine.continuePaused=v.continuePaused;for(auto&g:engine.gates)if(g.used&&!g.manual&&!(v.audible&(1u<<g.track)))g.due=0;
 }else if(type==DELAY907&&n==sizeof(Delay907)){Delay907 d;memcpy(&d,p,n);if(d.noteStep==-128||d.scdo==-128||!engine.delay(d.base,timeUs(),true,d.noteStep,d.scdo))panic();else ++requests;
 }else if(type==DELAY&&n==sizeof(Delay)){Delay d;memcpy(&d,p,n);if(!engine.delay(d,timeUs()))panic();else ++requests;}
 else if(type==STATE&&n==sizeof(State)){State s;memcpy(&s,p,n);if(s.playing>1)panic();else{if(!s.playing)cancelDelays();engine.state(s);}}
 else if(type==STATE903&&n==sizeof(State)){State st;memcpy(&st,p,n);if(st.playing>1){panic();return;}engine.audible=st.audible;engine.playing=st.playing;engine.continuePaused=true;}
 else if(type==CUT903&&n==1&&p[0]<4)cut903(p[0]);
 else if(type==CANCEL&&!n)cancelDelays();else {++invalidFrames925;panic();}
}
static void drain(){
 if(pinTestActive33)return;
 // Dispatch bounded batches then service each physical UART independently.
 for(unsigned i=0;i<64&&engine.count;++i){Message m=engine.queue[engine.head];if(m.output>=8){panic();return;}if(outputs[m.output].count==64)break;
  // Pop exactly one queue entry, preserving the checked destination.
  engine.head=(engine.head+1)%2048;--engine.count;auto&o=outputs[m.output];o.queue[o.tail]=m;o.tail=(o.tail+1)%64;++o.count;
 }
 for(uint8_t port=0;port<8;++port){auto&o=outputs[port];auto&serial=*ports[port];
  for(unsigned budget=0;budget<12&&serial.availableForWrite()>0;++budget){
   if(!o.active){
    if(o.panicChannel<16){o.current={uint8_t(0xb0|o.panicChannel++),123,0,DIRECT,0,0,0,port};}
    else if(releaseNext903(port,o.current)){}
    else if(o.haveAfter){o.current=o.after;o.haveAfter=false;}
    else {if(!o.count)break;o.current=o.queue[o.head];o.head=(o.head+1)%64;--o.count;
      if(o.current.kind==ECHO_OFF&&engine.owners[port][o.current.s&15][o.current.a]!=o.current.token)continue;
      if(o.current.kind==ARP_ON32&&(!effects32::running||!(effects32::audible&(1u<<o.current.track))))continue;
      if(o.current.kind==ECHO_ON&&(!engine.running()||!(engine.audible&(1u<<o.current.track))))continue;
      if((o.current.s&0xf0)==0x90&&o.current.b&&engine.owners[port][o.current.s&15][o.current.a]){o.after=o.current;o.haveAfter=true;o.current={uint8_t(0x80|(o.current.s&15)),o.current.a,0,DIRECT,0,0,0,port};}
    }
    if(!engine.canStart15(o.current)){++rejectedNotes15;++engine.overflow;continue;}
    o.pos=0;o.length=((o.current.s&0xf0)==0xc0||(o.current.s&0xf0)==0xd0)?2:3;o.active=true;
   }
   auto&m=o.current;serial.write(o.pos==0?m.s:o.pos==1?m.a:m.b);
   if(++o.pos==o.length){finalMidi54::push(timeUs(),port,m.track,m.s,m.a,m.b,m.kind);o.active=false;if(!engine.emitted(m,timeUs())){release903[port][m.s&15][m.a/8]|=1u<<(m.a%8);releasesPending903=true;++localReleases15;}
    if(isOn903(m)){active903[port][m.s&15][m.a/8]|=1u<<(m.a%8);
     if(o.cutCurrent){release903[port][m.s&15][m.a/8]|=1u<<(m.a%8);releasesPending903=true;}}
    else if((m.s&0xf0)==0x80||((m.s&0xf0)==0x90&&!m.b))active903[port][m.s&15][m.a/8]&=~(1u<<(m.a%8));
    else if((m.s&0xf0)==0xb0&&(m.a==120||m.a==123))memset(active903[port][m.s&15],0,16);
    o.cutCurrent=false;
    if((m.s&0xf0)==0x90&&m.b){++notes[port];noteLedUntil=millis()+30;}else if((m.s&0xf0)==0x80||(m.s&0xf0)==0x90)++offs[port];
   }
  }
 }
}
static void serviceRelease903(){if(!releasesPending903)return;for(auto&port:release903)for(auto&ch:port)for(auto b:ch)if(b)return;releasesPending903=false;}
static void report(){
 pinTestReport33();
 Serial.printf("MOD54,NODES=%u,ERRORS=%lu,CALCULATIONS=%lu,RAM=%u,SUSPENDED=%u\n",remote54.orderCount,remote54.errors,remote54.computed,(unsigned)sizeof(remote54),remote54.suspended);
 Serial.printf("FX32,OVERFLOW=%lu\n",effects32::overflow);
 Serial.printf("CAPACITY15,REJECTED_NOTES=%lu,LOCAL_RELEASES=%lu\n",rejectedNotes15,localReleases15);
 dac46::report();
 Serial.printf("EXP14,MAX=%u\n",expMaximum14);
 Serial.printf("PIDI,FW=V1,REV=VA01.58,MODE=MULTI_DELAY_8OUT,READY=%u,FRAMES=%lu,CRC=%lu,GAPS=%lu,OVERFLOW=%lu,QUEUE=%u,DELAY_REQUESTS=%lu,ECHOES=%lu,JOBS=%u,LATE_MAX_US=%lu,PANICS=%lu\n",ready,frames,decoder.errors,gaps,engine.overflow,engine.count,requests,engine.echoes,engine.pendingJobs(),engine.lateMaxUs,panics);
 Serial.printf("MULTI925,JOBS=%u,GRAPHS=%u,ECHOES=%lu,REJECTED=%lu\n",multi.pending(),multi.graphCount,multi.echoes,multi.rejected);
 Serial.printf("LINK925,RESETS=%lu,TIMEOUTS=%lu,INVALID=%lu,UPDATES=%lu\n",resetRequests925,linkTimeouts925,invalidFrames925,multi.updates);
 for(uint8_t i=0;i<8;++i)Serial.printf("OUT=%u,ON=%lu,OFF=%lu,QUEUE=%u\n",i+1,notes[i],offs[i],outputs[i].count);
}
#include "Checks902.h"
#include "Checks903.h"
void setup(){filter32=effects32::filter;USB.setProduct("UrbanKaoz Delay");MidiUSB.setName("UrbanKaoz Delay");MidiUSB.begin();Serial.begin(115200);pinMode(LED_BUILTIN,OUTPUT);pinMode(11,OUTPUT);digitalWrite(11,LOW);Serial1.setTX(12);Serial1.setRX(13);Serial1.setFIFOSize(4096);Serial1.begin(BAUD);for(auto*p:ports)p->begin(31250);lastMicros=micros();dac46::begin();}
void loop(){
 if(pinTestActive33&&(uint32_t)(millis()-pinTestSeen33)>1500)pinTestEnd33();
 effects32::now=timeUs();uint32_t errors=decoder.errors;for(unsigned i=0;i<256&&Serial1.available();++i)decoder.put(Serial1.read(),receive);
 if(decoder.errors!=errors)panic();if(ready&&millis()-lastLinkMs>2000){++linkTimeouts925;panic();}if(!pinTestActive33){drain();multi.service(timeUs(),engine);effects32::service(timeUs(),engine);engine.service(timeUs());drain();serviceRelease903();delayUsb07::service(timeUs());}
 delayService55();modService54();dac46::service();finalMidi54::service();
 digitalWrite(LED_BUILTIN,(int32_t)(noteLedUntil-millis())>0);analyzerService28();
 static char command[20];static unsigned pos=0;for(unsigned i=0;i<32&&Serial.available();++i){char c=Serial.read();if(c=='\n'||c=='\r'){command[pos]=0;if(finalMidi54::command(command,timeUs())){}else if(analyzerCommand28(command)){}else if(!strcmp(command,"STATUS"))report();else if(!strcmp(command,"CVREAD46"))dac46::readback();else if(!strcmp(command,"CVRECOVER48")){dac46::recover();dac46::report();}else if(!strcmp(command,"CHECK902"))checks902();else if(!strcmp(command,"CHECK903"))checks903();pos=0;}else if(pos<sizeof(command)-1)command[pos++]=c;}
}




