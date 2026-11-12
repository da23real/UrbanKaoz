#ifndef URBAN_MOD_WIRE54_INCLUDED
#define URBAN_MOD_WIRE54_INCLUDED
#include <stdint.h>
namespace mod54 {
constexpr uint8_t CONFIG=40,SOURCE=41,ROUTE=42,CLOCK=43,BASE=44,LOCK=45,CLEAR=46,COMMIT=47,SYNC=48;
constexpr unsigned N=64,R=512,A=512;
#pragma pack(push,1)
struct Params {uint32_t cycle;int32_t hold;uint16_t attack,decay,release,rat,interval;int16_t phase;int8_t ac,dc,rc;uint8_t sustain,shape,lfo,mode,env,ms,maximum,enabled;uint16_t inputs58;uint8_t delay58;};
struct Config {uint8_t id;uint16_t identity;Params p;double phaseSeed;uint32_t wallSeed;float periodSeed;uint8_t seedValid;};
struct Source {uint8_t id;int64_t begin,noteBegin;uint32_t gate;float input;uint8_t active;};
// kind: 0 CC, 1 bend, 2/3 NRPN7/14, 4 CV, 5 parameter, 6 depth.
struct Route {uint16_t id,number;int16_t base,depth,limit,lockValue;uint8_t source,kind,port,channel,target,slot,enabled,track,sourceSlot,locked;};
struct Clock {uint64_t tick;uint32_t wall,tickUs;uint8_t playing;};
struct Base {uint16_t number;int16_t value;uint8_t kind,mask,channel;};
struct Lock {Base base;uint8_t active;};
#pragma pack(pop)
}

#endif
