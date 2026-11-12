#pragma once
static delay55::Leases delayLeases55;
static bool delayKeep55(uint8_t kind,uint8_t port,uint8_t channel,uint16_t number){for(auto&l:delayLeases55.leases)if(l.used&&l.target.kind==kind&&l.target.port==port&&l.target.channel==channel&&l.target.number==number)return true;for(auto*g=multi.graphs;g;g=g->next)for(unsigned slot=0;slot<4;++slot)for(auto id:g->targets[slot])if(id<delay55::POOL){auto&t=multi.targetPool.entries[id].target;if(t.kind==kind&&t.number==number&&(kind<4?((g->d[slot].mask&(1u<<port))&&g->d[slot].channel==channel):(t.port==port&&t.channel==channel)))return true;}return false;}
static int delayBase55(uint8_t kind,uint8_t port,uint8_t channel,uint16_t number,int base){delay55::Target t;t.kind=kind;t.port=port;t.channel=channel;t.number=number;return delayLeases55.value(t,base);}
static int delayParam55(uint16_t identity,uint8_t kind,uint8_t slot,int base){delay55::Target t;t.kind=kind;t.identity=identity;t.slot=slot;return delayLeases55.value(t,base);}
static void delayConfigure55(const delay55::Config&c){
 remote54.delayBase55=delayBase55;remote54.delayParam55=delayParam55;remote54.delayKeep55=delayKeep55;
 for(auto&t:c.targets)if(t.kind<5){int i=remote54.address(t.kind,t.port,t.channel,t.number);if(i>=0){auto&a=remote54.addresses[i];a.base=t.base;a.track=t.track;}}
}
static void delayClear55(){
 bool dirty[mod54::A]={};for(auto&l:delayLeases55.leases)if(l.used){if(l.target.kind<5){int i=remote54.address(l.target.kind,l.target.port,l.target.channel,l.target.number,false);if(i>=0)dirty[i]=true;}l.used=false;}
 for(auto&s:remote54.states)s.cached=false;remote54.evaluate(micros());for(unsigned i=0;i<mod54::A;++i)if(dirty[i])modRefresh54(remote54.addresses[i]);
}
static void delayTarget55(const delay55::Target&t,int offset,uint64_t until,uint8_t owner,uint8_t slot,bool manual){
 int base=t.base,index=-1;
 if(t.kind<5){index=remote54.address(t.kind,t.port,t.channel,t.number,false);bool created=index<0;if(created)index=remote54.address(t.kind,t.port,t.channel,t.number);if(index<0){++delayLeases55.refused;return;}auto&a=remote54.addresses[index];if(created){a.base=t.base;a.track=t.track;}base=a.locked?a.lock:a.base;}
 else{bool found=false;for(unsigned i=0;i<mod54::N;++i)if(remote54.params[i].enabled&&remote54.states[i].identity==t.identity){auto&p=remote54.params[i];if(t.kind==5)base=p.lfo?(t.slot?p.phase:p.cycle):t.slot==0?p.hold:t.slot==1?p.attack:t.slot==2?p.decay:t.slot==3?p.sustain:p.release;else for(auto&r:remote54.routes)if(r.enabled&&r.source==i&&r.sourceSlot==t.slot)base=r.depth;found=true;break;}if(!found){++delayLeases55.refused;return;}}
 if(!delayLeases55.put(t,base+offset,until,owner,slot,manual))return;
 remote54.evaluate(micros());if(index>=0){auto&a=remote54.addresses[index];modEmit54(a,modFinal54(a),true);a.known=false;}
}
// A source attack restores its own base, while queued echo series stay intact.
static void delaySource57(uint8_t port,uint8_t channel,uint8_t track){
 bool dirty[mod54::A]={},changed=false;
 for(auto&l:delayLeases55.leases)if(l.used&&((l.target.kind<4&&l.target.port==port&&l.target.channel==channel)||(l.target.kind>=4&&l.owner==track))){
  if(l.target.kind<5){int i=remote54.address(l.target.kind,l.target.port,l.target.channel,l.target.number,false);if(i>=0)dirty[i]=true;}
  l.used=false;changed=true;
 }
 if(!changed)return;
 for(auto&s:remote54.states)s.cached=false;remote54.evaluate(micros());
 for(unsigned i=0;i<mod54::A;++i)if(dirty[i])modRefresh54(remote54.addresses[i]);
}
static void delayService55(){
 uint64_t now=timeUs();bool changed=false;bool dirty[mod54::A]={};
 for(auto&l:delayLeases55.leases)if(l.used&&(now>=l.until||(!l.manual&&!(engine.audible&(1u<<l.owner)))||!multi.latest[l.owner]||!multi.latest[l.owner]->d[l.delay].enabled)){l.used=false;changed=true;if(l.target.kind<5){int i=remote54.address(l.target.kind,l.target.port,l.target.channel,l.target.number,false);if(i>=0)dirty[i]=true;}}
 if(changed){for(auto&s:remote54.states)s.cached=false;remote54.evaluate(micros());for(unsigned i=0;i<mod54::A;++i)if(dirty[i]){auto&a=remote54.addresses[i];a.known=false;int v=modFinal54(a);if(modEmit54(a,v,true)){a.last=v;a.known=true;a.sent=micros();}}}
}
