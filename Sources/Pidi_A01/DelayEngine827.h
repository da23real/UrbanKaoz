#pragma once
#include "PidiWire827.h"
namespace pidi827 {
enum Kind:uint8_t {DIRECT,ECHO_ON,ECHO_OFF,MANUAL_ECHO_ON,ARP_ON32,DIRECT32};
struct Message {uint8_t s,a,b,kind,track;uint32_t gate,token;uint8_t output=0;};
static bool (*filter32)(Message&)=nullptr;
struct Job {Delay d{};uint64_t due=0;uint8_t remaining=0,velocity=0;int8_t noteStep=0,scdo=0;bool linear=false;};
struct Gate {uint64_t due=0;uint32_t token=0;uint8_t channel=0,note=0,track=0;bool used=false;uint8_t output=0;bool manual=false,arp32=false;};
struct Engine {
 Job jobs[256]{};Gate gates[256]{};Message queue[2048]{};
 uint32_t owners[8][16][128]{};uint32_t nextToken=0,overflow=0,echoes=0,lateMaxUs=0;
 unsigned head=0,tail=0,count=0,cursor=0;uint16_t audible=0xffff;bool playing=false,continuePaused=false;
 bool running()const{return playing||continuePaused;}
 bool push(Message m){if(filter32&&!filter32(m))return true;if(count==2048){++overflow;return false;}queue[tail]=m;tail=(tail+1)%2048;++count;return true;}
 bool pop(Message&m){while(count){m=queue[head];head=(head+1)%2048;--count;if(m.kind==ECHO_ON&&(!running()||!(audible&(1u<<m.track))))continue;if(m.kind==ECHO_OFF&&owners[m.output][m.s&15][m.a]!=m.token)continue;return true;}return false;}
 bool delay(const Delay&d,uint64_t now,bool linear=false,int8_t noteStep=0,int8_t scdo=0){
  if(d.output>=8||d.track>=16||d.channel>=16||d.note>=128||(!linear&&!d.velocity)||d.velocity>127||!d.repeats||d.repeats>16||d.feedback>127||d.drum>1||!d.spacingUs||!d.gateUs||d.spacingUs>1800000000u||d.waitUs>1800000000u||d.gateUs>20000000u)return false;
  if(!playing)return true;
  for(auto&j:jobs)if(!j.remaining){j={d,now+(uint64_t)d.waitUs+d.spacingUs,d.repeats,d.velocity,noteStep,scdo,linear};return true;}
  ++overflow;return false;
 }
 void cancel(){for(auto&j:jobs)j.remaining=0;for(auto&g:gates)if(g.used)g.due=0;
  // Discard future echo Note On messages, retaining direct MIDI and pending Note Off.
  unsigned original=count;for(unsigned i=0;i<original;++i){Message m=queue[head];head=(head+1)%2048;--count;if(m.kind!=ECHO_ON&&m.kind!=MANUAL_ECHO_ON&&m.kind!=ARP_ON32)push(m);}
 }
 void state(State s){audible=s.audible;if(playing&&!s.playing)cancel();playing=s.playing;for(auto&g:gates)if(g.used&&!g.manual&&!(audible&(1u<<g.track)))g.due=0;}
 void service(uint64_t now){
  // Release gates first. Output congestion must never lose an outstanding Note Off.
  for(auto&g:gates)if(g.used&&g.due<=now){
   if(owners[g.output][g.channel][g.note]!=g.token){g.used=false;continue;}
   if(count>=2048)break;
   push({uint8_t(0x80|g.channel),g.note,0,ECHO_OFF,g.track,0,g.token,g.output});g.used=false;
  }
  if(!running())return;
  // Bounded burst; due time advances from the original schedule, never from service time.
  unsigned emitted=0;for(unsigned visited=0;visited<256&&emitted<16;++visited){unsigned i=cursor;cursor=(cursor+1)%256;auto&j=jobs[i];if(!j.remaining||j.due>now)continue;
   if(count>=2048)break;unsigned index=j.d.repeats-j.remaining;
   int raw=j.linear?(int)j.d.velocity+(int)index*j.scdo:(uint16_t)j.velocity*j.d.feedback/127;
   uint8_t v=raw<0?0:raw>127?127:raw;
   int pitch=(int)j.d.note+(j.linear?(int)(index+1)*j.noteStep:0);uint8_t note=pitch<0?0:pitch>127?127:pitch;
   if(!v&&!j.linear){j.remaining=0;continue;}
   if(v&&(audible&(1u<<j.d.track))){push({uint8_t(0x90|j.d.channel),note,v,ECHO_ON,j.d.track,j.d.drum?0:j.d.gateUs,0,j.d.output});++emitted;uint64_t late=now-j.due;if(late>lateMaxUs)lateMaxUs=late>0xffffffffu?0xffffffffu:(uint32_t)late;}
   j.velocity=v;--j.remaining;j.due+=j.d.spacingUs;
  }
 }
 bool canStart15(const Message&m)const{
  if((m.kind!=ECHO_ON&&m.kind!=MANUAL_ECHO_ON&&m.kind!=ARP_ON32)||!m.gate)return true;
  for(const auto&g:gates)if(!g.used||(g.output==m.output&&g.channel==(m.s&15)&&g.note==m.a))return true;
  return false;
 }
 bool emitted(const Message&m,uint64_t now){
  unsigned ch=m.s&15,kind=m.s&0xf0;
  if(kind==0x90&&m.b){
   // Cancel prior delayed gates on this address before arming a new note.
   for(auto&g:gates)if(g.used&&g.output==m.output&&g.channel==ch&&g.note==m.a)g.used=false;
   owners[m.output][ch][m.a]=0;
   if(m.kind==ECHO_ON||m.kind==MANUAL_ECHO_ON||m.kind==ARP_ON32){++echoes;if(m.gate){owners[m.output][ch][m.a]=++nextToken;if(!nextToken)owners[m.output][ch][m.a]=++nextToken;for(auto&g:gates)if(!g.used){g={now+m.gate,owners[m.output][ch][m.a],(uint8_t)ch,m.a,m.track,true,m.output,m.kind==MANUAL_ECHO_ON,m.kind==ARP_ON32};return true;}++overflow;return false;}}
  }else if(kind==0x80||(kind==0x90&&!m.b)){
   owners[m.output][ch][m.a]=0;for(auto&g:gates)if(g.used&&g.output==m.output&&g.channel==ch&&g.note==m.a)g.used=false;
  }else if(kind==0xb0&&(m.a==120||m.a==123)){
   for(auto&g:gates)if(g.used&&g.output==m.output&&g.channel==ch)g.used=false;memset(owners[m.output][ch],0,sizeof(owners[m.output][ch]));
  }
  return true;
 }
 unsigned pendingJobs()const{unsigned n=0;for(auto&j:jobs)n+=j.remaining!=0;return n;}
};
}
