#pragma once
#include "Harmony32.h"
namespace effects32 {
using namespace pidi827;
static harmony32::Config configs[16];
struct Held {uint64_t until=0;uint32_t order=0;uint8_t track=0,port=0,ch=0,input=0,note=0,vel=0;bool used=false,arp=false,down=false;};
struct Pool {uint64_t due=0;uint32_t step=0;uint8_t track=0,port=0,ch=0;bool used=false;};
static Held held[512];static Pool pools[384];static uint32_t order=0,overflow=0,randomState=0x32A01234,tickUs=1359;static uint64_t now=0;static bool running=false;static uint16_t audible=0xffff,tracked=0;
inline void clear(){tracked=0;for(auto&h:held)h.used=false;for(auto&p:pools)p.used=false;}
inline void clearArp(){for(auto&h:held)if(h.arp)h.used=false;for(auto&p:pools)p.used=false;}
inline void reset(){clear();for(auto&c:configs)c=harmony32::Config{};running=false;}
inline bool same(const Held&h,const Message&m){return h.used&&h.track==m.track&&h.port==m.output&&h.ch==(m.s&15)&&h.input==m.a;}
inline bool filter(Message&m){
 if((m.s&0xf0)==0xb0&&(m.a==120||m.a==123)){for(auto&h:held)if(h.used&&h.port==m.output&&h.ch==(m.s&15))h.used=false;for(auto&p:pools)if(p.used&&p.port==m.output&&p.ch==(m.s&15))p.used=false;return true;}
 if(m.track>=16||m.kind==ECHO_OFF||m.kind==ARP_ON32||m.kind==DIRECT32)return true;
 auto&c=configs[m.track];if(!c.chord&&!c.arp&&!(tracked&(1u<<m.track)))return true;unsigned type=m.s&0xf0;bool on=type==0x90&&m.b,off=type==0x80||(type==0x90&&!m.b);
 if(!on&&!off)return true;
 if(m.kind==DIRECT){
  if(off){for(auto&h:held)if(same(h,m)&&!h.until){if(h.arp){h.down=false;if(!c.latch)h.used=false;return false;}m.a=h.note;m.kind=DIRECT32;h.used=false;
    for(auto&other:held)if(other.used&&!other.arp&&other.port==m.output&&other.ch==(m.s&15)&&other.note==m.a)return false;return true;}return !c.chord&&!c.arp;}
  // Retrigger of one input retains its mapping until its matching release.
  for(auto&h:held)if(same(h,m)&&!h.until){if(h.arp){h.down=true;h.vel=m.b;return false;}m.a=h.note;m.kind=DIRECT32;return true;}
 }
 if(!on)return true;
 int n=harmony32::pitch(m.a,c);if(n<0)return false;
 if(!c.arp&&m.kind!=DIRECT){m.a=n;return true;}
 if(!c.chord&&!c.arp)return true;
 if(c.arp&&c.latch){bool down=false;for(auto&h:held)if(h.used&&h.arp&&h.track==m.track&&h.down)down=true;if(!down)for(auto&h:held)if(h.used&&h.arp&&h.track==m.track)h.used=false;}
 Held*slot=nullptr;for(auto&h:held)if(!h.used){slot=&h;break;}if(!slot){++overflow;return false;}
 if(m.kind==DIRECT)tracked|=1u<<m.track;
 *slot={m.kind==DIRECT?0:now+(m.gate?m.gate:tickUs*96u),++order,m.track,m.output,uint8_t(m.s&15),m.a,uint8_t(n),m.b,true,c.arp!=0,true};
 if(c.arp){Pool*pool=nullptr;for(auto&p:pools)if(p.used&&p.track==m.track&&p.port==m.output&&p.ch==(m.s&15)){pool=&p;break;}
  if(!pool){for(auto&p:pools)if(!p.used){pool=&p;p={now,0,m.track,m.output,uint8_t(m.s&15),true};break;}}
  if(!pool){slot->used=false;++overflow;}return false;}
 m.a=n;m.kind=DIRECT32;return true;
}
inline void service(uint64_t at,Engine&out){now=at;if(!running)return;bool enabled=false;for(auto&c:configs)enabled|=c.arp!=0;if(!enabled)return;for(auto&h:held)if(h.used&&h.until&&h.until<=now)h.used=false;
 for(auto&p:pools)if(p.used){if(!running||!(audible&(1u<<p.track))||now<p.due)continue;const auto&c=configs[p.track];Held*notes[128];unsigned size=0;
  for(auto&h:held)if(h.used&&h.arp&&h.track==p.track&&h.port==p.port&&h.ch==p.ch){if(size<128)notes[size++]=&h;}
  if(!size||!c.arp){p.used=false;continue;}if(!running||!(audible&(1u<<p.track))||now<p.due)continue;
  for(unsigned i=1;i<size;++i){auto*v=notes[i];unsigned j=i;while(j&&(c.arp==4?notes[j-1]->order>v->order:notes[j-1]->note>v->note)){notes[j]=notes[j-1];--j;}notes[j]=v;}
  unsigned total=size*c.octaves,step=p.step++,index=step%total;
  if(c.arp==2)index=total-1-index;else if(c.arp==3&&total>1){index=step%(2*total-2);if(index>=total)index=2*total-2-index;}
  else if(c.arp==5){randomState^=randomState<<13;randomState^=randomState>>17;randomState^=randomState<<5;index=randomState%total;}
  auto*h=notes[index%size];int note=h->note+12*(index/size);uint64_t period=(uint64_t)tickUs*harmony32::rates[c.rate];
  if(note<=127)out.push({uint8_t(0x90|p.ch),uint8_t(note),h->vel,ARP_ON32,p.track,uint32_t(period*c.gate/100),0,p.port});
  p.due+=period;if(p.due<=now)p.due=now+period; // no catch-up burst after overload
 }
}
}
