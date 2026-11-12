#pragma once
#include "ModEngine54.h"
static mod54::Engine remote54;static bool remoteCommitArmed54=false;
static uint16_t cvRemote54[4]={};
static bool modEmit54(const mod54::Address&a,int v,bool immediate=false){
 if(a.kind==4){cvRemote54[a.number]=v;dac46::set(cvRemote54);return true;}
 if(!immediate&&(engine.count>=128||outputs[a.port].count>=16))return false;
 unsigned need=a.kind==2?5:a.kind==3?6:1;if(engine.count+need+delayReserve58>2048)return false;
 auto send=[&](uint8_t s,uint8_t x,uint8_t y){return engine.push({s,x,y,DIRECT,a.track,0,0,a.port});};
 if(a.kind==0)return send(0xB0|a.channel,a.number,v);
 if(a.kind==1){v+=8192;return send(0xE0|a.channel,v&127,v>>7);}
 send(0xB0|a.channel,99,a.number>>7);send(0xB0|a.channel,98,a.number&127);send(0xB0|a.channel,6,a.kind==3?v>>7:v);
 if(a.kind==3)send(0xB0|a.channel,38,v&127);send(0xB0|a.channel,101,127);send(0xB0|a.channel,100,127);return true;
}
static int modFinal54(const mod54::Address&a){int lo=a.kind==1?-8192:0,hi=a.kind==1?8191:a.kind==3?16383:a.kind==4?4095:127;return mod54::bound(remote54.baseValue55(a)+(a.count?lroundf(a.sum/a.count):0),lo,hi);}
static void modRefresh54(mod54::Address&a){remote54.evaluate(micros());int v=modFinal54(a);if(modEmit54(a,v,true)){a.last=v;a.sent=micros();a.known=true;}else a.known=false;}
static bool modPacket54(uint8_t type,const uint8_t*p,unsigned n){
 if(type<mod54::CONFIG||type>mod54::SYNC)return false;bool ok=false;
 if(type==mod54::CLEAR&&!n){for(auto&a:remote54.addresses)if(a.used){a.sum=0;a.count=0;modEmit54(a,modFinal54(a),true);}remote54.begin();remoteCommitArmed54=true;ok=true;}
 else if(type==mod54::SYNC&&!n){if(remote54.suspended){remote54.begin();remoteCommitArmed54=true;}ok=true;}else if(type==mod54::COMMIT&&!n){ok=true;if(remoteCommitArmed54||!remote54.suspended){remote54.rebuild();ok=remote54.valid;remote54.suspended=!ok;remoteCommitArmed54=false;}}
 else if(type==mod54::CONFIG&&n==sizeof(mod54::Config)){mod54::Config c;memcpy(&c,p,n);ok=remote54.configure(c);}
 else if(type==mod54::ROUTE&&n==sizeof(mod54::Route)){mod54::Route r;memcpy(&r,p,n);ok=remote54.route(r);}
 else if(type==mod54::SOURCE&&n==sizeof(mod54::Source)){mod54::Source s;memcpy(&s,p,n);ok=remote54.source(s);}
 else if(type==mod54::CLOCK&&n==sizeof(mod54::Clock)){mod54::Clock c;memcpy(&c,p,n);if(c.tickUs>=100&&c.tickUs<=200000&&c.playing<2){if(!c.playing||c.tick<remote54.clock.tick)for(auto&state:remote54.states){state.delayBegin58=-1;state.delayGate58=0;state.cached=false;}remote54.clock=c;remote54.anchor=micros();ok=true;}}
 else if((type==mod54::BASE&&n==sizeof(mod54::Base))||(type==mod54::LOCK&&n==sizeof(mod54::Lock))){mod54::Lock l{};memcpy(&l,p,n);auto&b=l.base;if(b.kind<5&&b.channel<16&&b.number<(b.kind==4?4:b.kind==0?128:16384)&&l.active<2){remote54.base(b,type==mod54::LOCK,l.active);for(unsigned port=0;port<8;++port)if(b.mask&(1u<<port)){int i=remote54.address(b.kind,port,b.channel,b.number,false);if(i>=0)modRefresh54(remote54.addresses[i]);}ok=true;}}
 if(!ok){++remote54.errors;remote54.suspended=true;}return true;
}
static bool modMidi54(uint8_t s,uint8_t a,uint8_t b,uint8_t port){
 uint8_t kind=(s&240)==176?0:(s&240)==224?1:255;if(kind==255)return false;
 int i=remote54.address(kind,port,s&15,kind==1?0:a,false);if(i<0||(remote54.suspended&&!(remote54.delayKeep55&&remote54.delayKeep55(kind,port,s&15,kind==1?0:a))))return false;
 auto&v=remote54.addresses[i];v.base=kind==1?int(a)+(int(b)<<7)-8192:b;modRefresh54(v);return true;
}
static void modDelayTrigger58(uint8_t track,uint32_t gateUs){
 if(track>=16||remote54.suspended||!remote54.clock.playing)return;
 uint32_t now=micros(),unit=max(1u,remote54.clock.tickUs);int64_t tick=remote54.clock.tick+(uint32_t)(now-remote54.anchor)/unit;
 for(unsigned k=0;k<64;++k)if(remote54.params[k].enabled&&remote54.params[k].delay58&&(remote54.params[k].inputs58&(1u<<track))){auto&s=remote54.states[k];s.delayBegin58=tick;s.delayGate58=max(1u,gateUs/unit);s.cached=false;}
 remote54.evaluate(now);remote54.output(now,[](const mod54::Address&a,int v){return modEmit54(a,v,true);});
}
static void modService54(){static uint32_t at=0;uint32_t now=micros();if((uint32_t)(now-at)<1000)return;at=now;remote54.evaluate(now);remote54.output(now,[](const mod54::Address&a,int v){return modEmit54(a,v);});}

