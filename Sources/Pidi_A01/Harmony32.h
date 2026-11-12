#pragma once
#include <stdint.h>
namespace harmony32 {
// Stable catalogue identity: 1 + quality*12 + root; 0 bypasses filtering.
static const uint16_t masks[]={0x091,0x089,0x891,0x491,0x489,0x049,0x111,0x085,0x0A1,0x291,0x289,0x001};
static const char* const qualities[]={"maj","min","maj7","7","min7","dim","aug","sus2","sus4","6","min6","root"};
static const char* const roots[]={"C","C#","D","D#","E","F","F#","G","G#","A","A#","B"};
constexpr unsigned count=144;
#pragma pack(push,1)
struct Config {uint8_t chord=0,policy=1,arp=0,rate=3,octaves=1,gate=70,latch=0;};
struct Packet {uint8_t track;Config config;};
struct Clock {uint32_t tickUs;uint16_t audible;uint8_t running;};
#pragma pack(pop)
inline bool valid(const Config&c){return c.chord<=count&&c.policy<4&&c.arp<6&&c.rate<8&&c.octaves>=1&&c.octaves<=4&&c.gate>=1&&c.gate<=100&&c.latch<2;}
inline uint16_t mask(uint8_t id){if(!id)return 4095;unsigned root=(id-1)%12;uint16_t m=masks[(id-1)/12];return ((m<<root)|(m>>(12-root)))&4095;}
inline int pitch(uint8_t note,const Config&c){uint16_t m=mask(c.chord);if(m&(1u<<(note%12)))return note;if(!c.policy)return -1;
 for(int d=1;d<128;++d){int lo=note-d,hi=note+d;if(c.policy!=2&&lo>=0&&(m&(1u<<(lo%12))))return lo;if(c.policy!=3&&hi<=127&&(m&(1u<<(hi%12))))return hi;}return -1;}
// 1/2, 1/4, 1/8, 1/16, 1/32, 1/8T, 1/16T, 1/8 dotted; 384 PPQN.
static const uint16_t rates[]={768,384,192,96,48,128,64,288};
}
