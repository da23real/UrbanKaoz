#pragma once
#include <stdint.h>
namespace rmx916 {
enum Hit:uint8_t {PadO,PadR,Chain};
enum Length:uint8_t {Pad,Fixed,Hold};
inline int aligned(int value,int delta,int quantum,int maximum=1536){
 if(!delta)return value;
 int64_t next=quantum==1?(int64_t)value+delta:delta>0?
  ((int64_t)value/quantum+delta)*quantum:
  (((int64_t)value+quantum-1)/quantum+delta)*quantum;
 return next<1?1:next>maximum?maximum:(int)next;
}
inline unsigned cycle(unsigned wait,unsigned copies,unsigned bars,unsigned post){return wait+copies*bars+post;}
// Complete the final cycle; short originals repeat only in preserved bars.
inline unsigned length(unsigned original,unsigned cycle){return cycle?((original+cycle-1)/cycle)*cycle:0;}
inline int target(unsigned bar,unsigned wait,unsigned copies,unsigned bars,unsigned post){
 unsigned span=cycle(wait,copies,bars,post);if(!span||!bars)return -1;
 unsigned at=bar%span;return at>=wait&&at<wait+copies*bars?(at-wait)%bars:-1;
}
inline bool replace(bool active,uint16_t tracks,uint8_t track,uint8_t hit){return active&&hit==PadR&&(tracks&(1u<<track));}
}
