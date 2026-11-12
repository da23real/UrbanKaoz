#pragma once
#include <stdint.h>
#include <string.h>
#include "Exp14.h"
namespace delay55 {
constexpr unsigned TARGETS=4,POOL=64,LEASES=64,ACTIVE=16;
constexpr uint8_t CONFIG=49,NONE=255,NDO=7,GAT=9;
#pragma pack(push,1)
struct Target {int16_t step=0,base=0;uint16_t number=0,identity=0;uint8_t kind=NONE,port=0,channel=0,slot=0,exp=0,track=255;};
struct Config {uint8_t track,delay;Target targets[TARGETS];};
#pragma pack(pop)
inline bool valid(const Target&t){return (t.kind==GAT&&t.exp<=100)||t.kind==NONE||(t.kind<=NDO&&t.port<8&&t.channel<16&&t.exp<=100&&t.slot<8&&(t.kind!=0||t.number<128)&&(t.kind!=4||t.number<4)&&((t.kind!=2&&t.kind!=3)||t.number<16384)&&((t.kind!=5&&t.kind!=6)||t.identity<64*17)&&(t.kind!=5||t.slot<5));}
inline int32_t offset(const Target&t,unsigned repeat,int noteStep){int32_t step=(t.kind==NDO?noteStep:t.step)*256,sum=0;for(unsigned i=0;i<repeat;++i){sum+=step;int64_t next=(int64_t)step*expFactor14(t.exp)/256;step=next< -8388352?-8388352:next>8388352?8388352:next;}return sum/256;}
struct Pool {
 struct Entry{Target target{};uint16_t refs=0;} entries[POOL];uint32_t refused=0;
 uint8_t take(const Target&t){if(t.kind==NONE)return NONE;for(unsigned i=0;i<POOL;++i)if(entries[i].refs&&!memcmp(&t,&entries[i].target,sizeof(t))){++entries[i].refs;return i;}for(unsigned i=0;i<POOL;++i)if(!entries[i].refs){entries[i]={t,1};return i;}++refused;return NONE;}
 void retain(uint8_t id){if(id<POOL)++entries[id].refs;}
 void release(uint8_t id){if(id<POOL&&entries[id].refs)--entries[id].refs;}
};
struct Leases {
 struct Lease{Target target{};uint64_t until=0;uint32_t serial=0;int32_t value=0;uint8_t owner=0,delay=0;bool used=false,manual=false;} leases[LEASES];
 uint32_t sequence=0,refused=0;
 static bool same(const Target&a,const Target&b){return a.kind==b.kind&&(a.kind>=5?(a.identity==b.identity&&a.slot==b.slot):(a.port==b.port&&a.channel==b.channel&&a.number==b.number));}
 unsigned free()const{unsigned n=0;for(auto&l:leases)n+=!l.used;return n;}
 bool put(const Target&t,int value,uint64_t until,uint8_t owner,uint8_t delay,bool manual=false){for(auto&l:leases)if(!l.used){l={t,until,++sequence,value,owner,delay,true,manual};return true;}++refused;return false;}
 int value(const Target&t,int base)const{const Lease*best=nullptr;for(auto&l:leases)if(l.used&&same(l.target,t)&&(!best||(int32_t)(l.serial-best->serial)>0))best=&l;return best?best->value:base;}
 bool expire(uint64_t now){bool changed=false;for(auto&l:leases)if(l.used&&now>=l.until){l.used=false;changed=true;}return changed;}
 void cut(int owner=-1,int delay=-1){for(auto&l:leases)if((owner<0||l.owner==owner)&&(delay<0||l.delay==delay))l.used=false;}
};
}

