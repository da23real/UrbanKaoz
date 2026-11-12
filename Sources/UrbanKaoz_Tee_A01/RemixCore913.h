#pragma once
#include "RemixCore911.h"
namespace rmx913 {
enum Phase:uint8_t {Source,Wait,Target};
inline Phase phase(uint64_t tick,uint16_t bars,uint16_t hear,uint16_t wait){
 (void)bars;if(!hear)return Source;
 unsigned bar=(tick/rmx911::Bar)%hear;wait=wait<hear?wait:hear-1;
 return bar==hear-1?Target:bar>=hear-1-wait&&bar<hear-1?Wait:Source;
}
inline int quantShift(int tick,unsigned quantum){if(!quantum)return 0;int r=rmx911::wrap(tick,quantum);return r<=(int)quantum/2?-r:(int)quantum-r;}
// Raw pad 0 is the existing bottom-left origin. Only the two note grids rotate.
inline uint8_t step(uint8_t raw,uint8_t layout){unsigned x=raw%4,y=raw/4;switch(layout%4){case 1:return x*4+y;case 2:return (3-y)*4+x;case 3:return x*4+3-y;default:return raw;}}
inline bool eraseStep(rmx911::Model&m,uint8_t step){bool changed=false;for(uint32_t i=0;i<m.notes.size;)if(rmx911::targetStep912(m.notes[i].tick)==step){m.remove(i);changed=true;}else ++i;return changed;}
// Caller records emitted attacks, not a pasted clip. Already emitted rolls become
// plain attacks; their delay routing and patch locks remain sparse properties.
inline bool capture(rmx911::Model&dst,const rmx911::Model&src,const rmx911::Note&note,int at,int shift,uint8_t rollVelocity,bool rollDelay){
 unsigned size=dst.notes.size,props=dst.properties.size;auto id=dst.nextId;auto mask=dst.mask;
 if(!dst.append(src,note,note.track,at+shift,rmx911::Bar)){dst.notes.size=size;dst.properties.size=props;dst.nextId=id;dst.mask=mask;return false;}
 auto&n=dst.notes[dst.notes.size-1];n.time=0;if(rollVelocity)n.velocity=rollVelocity;
 bool ok=dst.set(n,rmx911::Roll,5,0)&&dst.set(n,rmx911::Roll,0,0);
 if(rollVelocity)ok=ok&&dst.set(n,rmx911::Delay,300,1);
 if(!ok){dst.notes.size=size;dst.properties.size=props;dst.nextId=id;dst.mask=mask;}
 return ok;
}
}
