#pragma once
// Final bytes accepted by the physical output, including generated delay notes.
// Explicit subscription only; never blocks the musical UARTs on USB backpressure.
namespace finalMidi54 {
struct Event {uint64_t us;uint32_t seq;uint8_t port,track,status,a,b,kind;};
static Event queue[256];static uint16_t head=0,count=0;
static uint32_t sequence=0,drops=0,until=0;static bool enabled=false,ending=false;
static void push(uint64_t us,uint8_t port,uint8_t track,uint8_t status,uint8_t a,uint8_t b,uint8_t kind){
 if(!enabled)return;uint32_t seq=++sequence;if(count==256){++drops;return;}
 queue[(head+count)%256]={us,seq,port,track,status,a,b,kind};++count;
}
static bool command(const char*cmd,uint64_t now){
 if(!strcmp(cmd,"MF54,ON")){head=count=0;sequence=drops=0;enabled=true;ending=false;until=millis()+5000;Serial.printf("MF54,READY,%llu\n",(unsigned long long)now);return true;}
 if(!strcmp(cmd,"MF54,PING")){if(enabled)until=millis()+5000;Serial.printf("MF54,T,%llu,%u\n",(unsigned long long)now,enabled||ending);return true;}
 if(!strcmp(cmd,"MF54,OFF")){enabled=false;ending=true;until=millis()+5000;return true;}
 return false;
}
static void service(){
 if(!enabled&&!ending)return;if(!Serial||(int32_t)(millis()-until)>=0){enabled=ending=false;count=0;return;}
 for(unsigned i=0;i<8&&count&&Serial.availableForWrite()>=112;++i){const auto&e=queue[head];
  Serial.printf("MF54,E,%lu,%llu,%u,%u,%u,%u,%u,%u,%lu\n",(unsigned long)e.seq,(unsigned long long)e.us,e.port,e.track,e.status,e.a,e.b,e.kind,(unsigned long)drops);
  head=(head+1)%256;--count;
 }
 if(ending&&!count&&Serial.availableForWrite()>=80){Serial.printf("MF54,END,%lu,%lu,0\n",(unsigned long)sequence,(unsigned long)drops);ending=false;}
}
}
