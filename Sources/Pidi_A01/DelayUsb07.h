#pragma once
#include <MIDIUSB.h>
namespace delayUsb07 {
struct Packet{uint8_t bytes[4];int16_t gate;};
struct Gate{uint64_t due;uint8_t channel,note;bool used,sent,offQueued;};
static Packet queue[256];static Gate gates[128];static unsigned head=0,tail=0,count=0;
static uint8_t labelChannel=16;static uint32_t dropped=0;
static void push(uint8_t status,uint8_t a,uint8_t b,int16_t gate=-1){queue[tail]={{uint8_t(status>>4),status,a,b},gate};tail=(tail+1)%256;++count;}
static void echo(uint8_t slot,uint8_t repetition,uint8_t channel,uint8_t note,uint8_t velocity,uint32_t gate,uint64_t now){
 if(!MidiUSB.connected())return;
 Gate*free=nullptr;for(auto&g:gates)if(!g.used){free=&g;break;}
 if(!free||count+(labelChannel?2:1)>256){++dropped;return;}
 if(labelChannel)push(0xB0|(labelChannel-1),65+slot,repetition);
 push(0x90|channel,note,velocity,free-gates);*free={now+(gate?gate:1000),channel,note,true,false,false};
}
static void cancel(){head=tail=count=0;for(auto&g:gates)if(g.used){if(!g.sent)g.used=false;else {g.due=0;g.offQueued=false;}}}
static void service(uint64_t now){
 if(!MidiUSB.connected()){head=tail=count=0;for(auto&g:gates)g.used=false;return;}
 for(auto&g:gates)if(g.used&&!g.offQueued&&now>=g.due&&count<256){push(0x80|g.channel,g.note,0,&g-gates);g.offQueued=true;}
 for(unsigned budget=0;budget<32&&count;++budget){auto&p=queue[head];if(!MidiUSB.writePacket(p.bytes))break;if(p.gate>=0){auto&g=gates[p.gate];if(p.bytes[0]==9)g.sent=true;else g.used=false;}head=(head+1)%256;--count;}
}
}
