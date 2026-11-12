#pragma once
#ifndef DELAY_ECHO07
#define DELAY_ECHO07(slot,rep,ch,note,vel,gate,now) ((void)0)
#endif
#include <new>
#include "DelayTargets55.h"
#ifndef DELAY_TARGET55
#define DELAY_TARGET55(target,value,until,owner,slot,manual) ((void)0)
#endif
#ifndef DELAY_TRIGGER58
#define DELAY_TRIGGER58(owner,gate) ((void)0)
#endif
#ifndef DELAY_RESERVE58
#define DELAY_RESERVE58(n) ((void)0)
#endif
#include "Exp14.h"
namespace multi925 {
using pidi827::Config924;
struct Graph {Graph*next=nullptr;uint16_t refs=0;uint8_t owner=0;Config924 d[4]{};bool configured55[4]={};uint8_t targets[4][4];Graph(){memset(targets,255,sizeof(targets));}};
struct Job {Graph*g=nullptr;uint64_t due=0;uint32_t gate=0,tick=1,spacing=1;int32_t accel=0,scdo=0,velocity=0;uint8_t slot=0,index=0,note=0,original=0;bool manual=false;int32_t gateDelta08=0;uint32_t sourceGate57=0;};
struct Engine {
 delay55::Pool targetPool;
 Graph*graphs=nullptr,*latest[16]{};uint8_t graphCount=0,cursor=0,lockCount=0;
 pidi827::Lock925 locks[32]{};pidi827::Route925 routes[48]{};
 uint32_t rejected=0,echoes=0,updates=0;Job jobs[128]{};
 void release(Graph*g){if(g&&g->refs)--g->refs;}
 void collect(){Graph**at=&graphs;while(*at){auto*g=*at;if(!g->refs){*at=g->next;for(auto&row:g->targets)for(auto id:row)targetPool.release(id);delete g;--graphCount;}else at=&g->next;}}
 void cancel(){for(auto&j:jobs)if(j.g){release(j.g);j.g=nullptr;}lockCount=0;collect();}
 void reset(){cancel();for(auto&g:latest){release(g);g=nullptr;}collect();}
 Graph*copy(Graph*base,uint8_t owner){collect();if(graphCount>=64){++rejected;return nullptr;}auto*g=new(std::nothrow) Graph;if(!g){++rejected;return nullptr;}g->owner=owner;if(base){memcpy(g->d,base->d,sizeof(g->d));memcpy(g->targets,base->targets,sizeof(g->targets));memcpy(g->configured55,base->configured55,4);for(auto&row:g->targets)for(auto id:row)targetPool.retain(id);}else for(auto&d:g->d)d.length=1;g->next=graphs;graphs=g;++graphCount;return g;}
 Graph*editable(uint8_t t){if(latest[t]&&latest[t]->refs==1)return latest[t];auto*g=copy(latest[t],t);if(!g)return nullptr;g->refs=1;release(latest[t]);latest[t]=g;return g;}

 bool targets(const delay55::Config&c){
  if(c.track>=16||c.delay>=4)return false;unsigned active=0;
  if(latest[c.track]&&latest[c.track]->configured55[c.delay]){bool same=true;for(unsigned i=0;i<4;++i){uint8_t id=latest[c.track]->targets[c.delay][i];if(id==delay55::NONE)same&=c.targets[i].kind==delay55::NONE;else same&=!memcmp(&targetPool.entries[id].target,&c.targets[i],sizeof(delay55::Target));}if(same)return true;}
  for(unsigned t=0;t<16;++t)if(latest[t])for(unsigned d=0;d<4;++d)if(t!=c.track||d!=c.delay)for(auto id:latest[t]->targets[d])if(id<delay55::POOL&&targetPool.entries[id].target.kind<delay55::NDO)++active;
  for(auto&t:c.targets){if(!delay55::valid(t))return false;active+=t.kind<delay55::NDO;}if(active>delay55::ACTIVE){++rejected;return false;}
  uint8_t ids[4];memset(ids,255,4);for(unsigned i=0;i<4;++i){ids[i]=targetPool.take(c.targets[i]);if(c.targets[i].kind!=delay55::NONE&&ids[i]==delay55::NONE){for(auto id:ids)targetPool.release(id);++rejected;return false;}}
  auto*g=editable(c.track);if(!g){for(auto id:ids)targetPool.release(id);return false;}for(unsigned i=0;i<4;++i){targetPool.release(g->targets[c.delay][i]);g->targets[c.delay][i]=ids[i];}g->configured55[c.delay]=true;return true;
 }
 static bool valid(const Config924&d,uint8_t slot){return d.track<48&&d.mode<=2&&d.repeats<=16&&d.length>=1&&d.length<=3072&&d.wait<=3072&&d.rolls<=1&&d.source<=1&&d.enabled<=1&&d.gat08!=-128&&d.gex08<=100&&d.axp<=100&&d.oxp<=100&&d.velocity!=-128&&d.note!=-128&&d.scdo!=-128&&d.accel!=-128&&!(d.chain&~((1u<<slot)-1));}
 void route(Config924&d){auto&r=routes[d.track];d.channel=r.channel;d.mask=r.mask;}
 bool object(const pidi827::Object925&o){if(o.track>=16||o.slot>=4||!valid(o.config,o.slot))return false;if(!latest[o.track]&&!o.config.enabled&&!o.config.source&&!o.config.repeats){++updates;return true;}auto*g=editable(o.track);if(!g)return false;g->d[o.slot]=o.config;route(g->d[o.slot]);if(!o.config.enabled)cut(o.track,o.slot);++updates;return true;}
 static bool assign(Config924&d,uint8_t f,int v){
  if(f>=18)return false;int lo=(f==3||f==6||f==7||f==9||f==15)?-127:f==1?1:0;int hi=f==17?2:f<2?3072:f==2?16:f==4?15:f==5||f==12||f==14?1:f==8?2:f==10||f==11||f==16?100:f==13?7:127;if(v<lo||v>hi)return false;
  switch(f){case 0:d.wait=v;break;case 1:d.length=v;break;case 2:d.repeats=v;break;case 3:d.velocity=v;break;case 4:d.track=v*3+d.track%3;break;case 5:d.rolls=v;break;case 6:d.note=v;break;case 7:d.scdo=v;break;case 8:d.mode=v;break;case 9:d.accel=v;break;case 10:d.axp=v;break;case 11:d.oxp=v;break;case 12:d.source=v;break;case 13:d.chain=v;break;case 14:d.enabled=v;break;case 15:d.gat08=v;break;case 16:d.gex08=v;break;case 17:d.track=(d.track/3)*3+v;break;}return true;
 }
 bool value(const pidi827::Value925&v){if(v.track>=16||v.slot>=4||!latest[v.track])return false;auto d=latest[v.track]->d[v.slot];if(!assign(d,v.field,v.value)||!valid(d,v.slot))return false;auto*g=editable(v.track);if(!g)return false;route(d);g->d[v.slot]=d;if(!d.enabled)cut(v.track,v.slot);++updates;return true;}
 bool routing(const pidi827::Route925&r){if(r.track>=48||r.channel>=16||r.drum>1)return false;routes[r.track]=r;for(uint8_t t=0;t<16;++t)if(latest[t]){bool match=false;for(auto&d:latest[t]->d)match|=d.track==r.track;if(match){auto*g=editable(t);if(!g)return false;for(auto&d:g->d)if(d.track==r.track)route(d);}}return true;}
 void cut(uint8_t t,uint8_t s){for(auto&j:jobs)if(j.g&&j.g->owner==t&&j.slot==s){release(j.g);j.g=nullptr;}collect();}
 bool overrides(const uint8_t*p,unsigned n){if(n<2||p[0]+p[1]>32||n!=2+p[1]*sizeof(pidi827::Lock925))return false;if(!p[0])lockCount=0;if(p[0]!=lockCount)return false;for(uint8_t i=0;i<p[1];++i){pidi827::Lock925 v;memcpy(&v,p+2+i*sizeof(v),sizeof(v));if(v.address/32>=4||(v.address%32>=12&&v.address%32!=15&&v.address%32!=16&&v.address%32!=17))return false;locks[lockCount++]=v;}return true;}
 static int midi(int v){return v<0?0:v>127?127:v;}
 static int32_t grow(int32_t v,uint8_t percent,int32_t limit){int64_t m=v<0?-(int64_t)v:v;m=(m*(expFactor14(percent))+128)/256;if(m>limit)m=limit;return v<0?-(int32_t)m:(int32_t)m;}
 bool schedule(Graph*g,uint8_t slot,uint8_t note,uint8_t velocity,uint32_t gate,uint32_t tick,uint64_t at,bool manual=false){const auto&d=g->d[slot];if(!d.enabled||!d.repeats||!d.mask)return true;for(auto&j:jobs)if(!j.g){int first=d.mode==2?velocity*midi(d.velocity)/127:midi((d.mode==1?0:velocity)+d.velocity);j={g,at+(uint64_t)(d.wait+d.length)*tick,gate,tick,(uint32_t)d.length*tick,(int32_t)d.accel*256,(int32_t)d.scdo*256,first*256,slot,0,note,velocity,manual};j.sourceGate57=gate;j.gateDelta08=d.gat08*256;int64_t firstGate=(int64_t)gate+(int64_t)d.gat08*tick;j.gate=firstGate<1000?1000:firstGate>20000000?20000000:firstGate;++g->refs;return true;}++rejected;return false;}
 bool source(const pidi827::Note925&n,uint64_t at,bool running,uint16_t audible){
  uint8_t count=lockCount;lockCount=0;if(n.track>=16||n.note>127||n.velocity>127||n.roll>7||!n.tickUs||n.tickUs>1000000||!n.gateUs||n.gateUs>20000000||n.locks>32||(n.locks&&n.locks!=count))return false;
  auto*g=latest[n.track];if(!g){++rejected;return false;}bool manual=n.roll&2;if(!running&&!manual)return true;
  if(n.roll&4){for(uint8_t slot=0;slot<4;++slot)cut(n.track,slot);}
  if(n.locks){auto*copyGraph=copy(g,n.track);if(!copyGraph)return false;g=copyGraph;for(uint8_t i=0;i<count;++i){uint8_t s=locks[i].address/32,f=locks[i].address%32;if(!assign(g->d[s],f,f==4&&locks[i].value==16?n.track:locks[i].value)){collect();return false;}}for(auto&d:g->d)route(d);}
  // Temporary graph belongs to this attack only; no change to the track defaults.
  for(uint8_t i=0;i<4;++i){auto&d=g->d[i];if(d.source&&(!(n.roll&1)||d.rolls)&&(manual||(audible&(1u<<g->owner))))schedule(g,i,n.note,n.velocity,n.gateUs,n.tickUs,at,manual);}collect();return true;
 }
 void service(uint64_t now,pidi827::Engine&out){unsigned budget=16;for(unsigned n=0;n<128&&budget;++n){auto&j=jobs[cursor];cursor=(cursor+1)%128;if(!j.g)continue;auto*g=j.g;const auto&d=g->d[j.slot];if(!j.manual&&!(out.audible&(1u<<g->owner))){release(g);j.g=nullptr;continue;}if((!j.manual&&!out.running())||now<j.due)continue;--budget;
  bool legacy=d.mode==2&&!d.note&&!d.scdo;int velocity=legacy?(j.index?j.original*midi(d.velocity)/127:j.velocity/256):midi((j.velocity+128)/256);int pitchDelta=0;bool assigned=false;for(auto id:g->targets[j.slot])if(id<delay55::POOL){assigned=true;auto&t=targetPool.entries[id].target;if(t.kind==delay55::NDO)pitchDelta+=delay55::offset(t,j.index+1,d.note);}uint8_t pitch=midi((int)j.note+(g->configured55[j.slot]?pitchDelta:(j.index+1)*(int)d.note));unsigned needed=__builtin_popcount((unsigned)d.mask);if(velocity&&out.count+needed>2048){++rejected;continue;}
  uint32_t echoGate57=j.gate;int32_t percent57=100;bool gateTarget57=false;
  for(auto id:g->targets[j.slot])if(id<delay55::POOL){auto&t=targetPool.entries[id].target;if(t.kind==delay55::GAT){gateTarget57=true;percent57+=delay55::offset(t,j.index+1,0);}}
  if(gateTarget57){int64_t gate=(int64_t)j.sourceGate57*(percent57<0?0:percent57>1000?1000:percent57)/100;echoGate57=gate<1000?1000:gate>20000000?20000000:gate;}
  if(velocity){DELAY_RESERVE58(needed);for(auto id:g->targets[j.slot])if(id<delay55::POOL){auto t=targetPool.entries[id].target;if(t.kind<4){for(uint8_t port=0;port<8;++port)if(d.mask&(1u<<port)){t.port=port;t.channel=d.channel;t.track=d.track/3;DELAY_TARGET55(t,delay55::offset(t,j.index+1,d.note),now+echoGate57,g->owner,j.slot,j.manual);}}else if(t.kind<delay55::NDO)DELAY_TARGET55(t,delay55::offset(t,j.index+1,d.note),now+echoGate57,g->owner,j.slot,j.manual);}
  if(needed)DELAY_TRIGGER58(g->owner,echoGate57);DELAY_RESERVE58(0);DELAY_ECHO07(j.slot,j.index+1,d.channel,pitch,velocity,d.drum?0:echoGate57,now);for(uint8_t port=0;port<8;++port)if(d.mask&(1u<<port))out.push({uint8_t(0x90|d.channel),pitch,(uint8_t)velocity,uint8_t(j.manual?pidi827::MANUAL_ECHO_ON:pidi827::ECHO_ON),g->owner,d.drum?0:echoGate57,0,port});++echoes;for(uint8_t k=j.slot+1;k<4;++k)if(latest[g->owner]&&latest[g->owner]->d[k].enabled&&(g->d[k].chain&(1u<<j.slot))&&(j.manual||(out.audible&(1u<<g->owner))))schedule(g,k,pitch,velocity,echoGate57,j.tick,j.due,j.manual);}
  j.gateDelta08=grow(j.gateDelta08,d.gex08,127*256);int64_t nextGate=(int64_t)j.gate+(int64_t)j.gateDelta08*j.tick/256;j.gate=nextGate<1000?1000:nextGate>20000000?20000000:nextGate;
  j.original=velocity;if(++j.index>=d.repeats||(legacy&&!velocity)){release(g);j.g=nullptr;continue;}int64_t spacing=(int64_t)j.spacing+(int64_t)j.accel*j.tick/256;if(spacing<j.tick)spacing=j.tick;if(spacing>(int64_t)3072*j.tick)spacing=(int64_t)3072*j.tick;j.spacing=spacing;j.due+=j.spacing;j.accel=grow(j.accel,d.axp,128*256);j.velocity+=j.scdo;if(j.velocity<0)j.velocity=0;if(j.velocity>127*256)j.velocity=127*256;j.scdo=grow(j.scdo,d.oxp,127*256);
 }collect();}
 unsigned pending()const{unsigned n=0;for(auto&j:jobs)n+=j.g!=nullptr;return n;}
};
}
