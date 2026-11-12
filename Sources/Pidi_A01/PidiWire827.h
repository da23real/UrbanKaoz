#pragma once
#include <stdint.h>
#include <string.h>
namespace pidi827 {
constexpr uint32_t BAUD=250000;
constexpr unsigned PAYLOAD=64;
inline uint8_t firstVelocity907(uint8_t original,int value,uint8_t mode){
 int v=mode==2?(int)original*(value<0?0:value>127?127:value)/127:mode==1?value:(int)original+value;
 return v<0?0:v>127?127:v;
}
enum Type:uint8_t {RESET=1,LEGACY_MIDI,LEGACY_DELAY,STATE,CANCEL,HEARTBEAT,MIDI,DELAY,CUT903,STATE903,DELAY907,BEGIN924,CONFIG924,COMMIT924,SOURCE924,STATE924,OBJECT925,VALUE925,LOCKS925,NOTE925,ROUTE925,LABEL07,EXP14,FX32,MIDI32,CLOCK32,PINTEST33,CV46};
#pragma pack(push,1)
struct Delay {uint32_t waitUs,spacingUs,gateUs;uint8_t track,channel,note,velocity,repeats,feedback,drum,output;};
struct Delay907 { Delay base; int8_t noteStep,scdo; };
struct Config924 {uint16_t wait,length;int8_t velocity,note,scdo,accel;uint8_t axp,oxp,repeats,mode,track,channel,mask,rolls,drum,source,chain,enabled;int8_t gat08=0;uint8_t gex08=33;};
struct Begin924 {uint32_t hash;uint8_t slot;};
struct Entry924 {uint8_t slot,index;Config924 config;};
struct Source924 {uint32_t hash,gateUs,tickUs;uint8_t slot,note,velocity,roll;};
struct State924 {uint16_t audible;uint8_t playing,continuePaused;};
struct Object925 {uint8_t track,slot;Config924 config;};
struct Value925 {int16_t value;uint8_t track,slot,field;};
struct Lock925 {int16_t value;uint8_t address;}; // address = delay slot * 16 + field
struct Note925 {uint32_t gateUs,tickUs;uint8_t track,note,velocity,roll,locks;};
struct Route925 {uint8_t track,channel,mask,drum;};
struct State {uint16_t audible;uint8_t playing;};
#pragma pack(pop)
inline uint16_t crc(const uint8_t*p,unsigned n){uint16_t c=0xffff;while(n--){c^=(uint16_t)*p++<<8;for(unsigned i=0;i<8;++i)c=(c&0x8000)?(c<<1)^0x1021:c<<1;}return c;}
inline unsigned encode(uint8_t type,uint16_t seq,const void*p,unsigned n,uint8_t*out){
 if(n>PAYLOAD)return 0;uint8_t raw[PAYLOAD+5];raw[0]=type;raw[1]=seq;raw[2]=seq>>8;if(n)memcpy(raw+3,p,n);
 uint16_t c=crc(raw,n+3);raw[n+3]=c;raw[n+4]=c>>8;unsigned used=0;out[used++]=0x7e;
 for(unsigned i=0;i<n+5;++i){uint8_t b=raw[i];if(b==0x7e||b==0x7d){out[used++]=0x7d;b^=0x20;}out[used++]=b;}out[used++]=0x7e;return used;
}
struct Decoder {
 uint8_t bytes[PAYLOAD+5];unsigned count=0;bool escaped=false,overflow=false;uint32_t errors=0;
 template<class F>void put(uint8_t b,F receive){
  if(b==0x7e){if(count&&!overflow){if(count>=5&&crc(bytes,count-2)==(uint16_t)(bytes[count-2]|bytes[count-1]<<8))receive(bytes[0],(uint16_t)(bytes[1]|bytes[2]<<8),bytes+3,count-5);else ++errors;}count=0;escaped=overflow=false;return;}
  if(overflow)return;if(b==0x7d){escaped=true;return;}if(escaped){b^=0x20;escaped=false;}if(count==sizeof(bytes)){overflow=true;++errors;return;}bytes[count++]=b;
 }
};
}
