#pragma once
// Manual notes are a source, not a transport mode. Their age advances while stopped.
struct ManualMod06 { uint32_t started=0,gateMs=0; uint8_t owner=255; bool valid=false,held=false; };
ManualMod06 manualMod06[16];
uint16_t manualModMask06=0;
FLASHMEM void manualModStart06(uint8_t track,uint8_t owner,uint32_t gateUs){
 manualMod06[track]={millis(),(gateUs+999)/1000,owner,true,owner<64||owner==254};manualModMask06|=1u<<track;++modEpoch805;
 sourceTrigger909(0,track);
}
FLASHMEM void manualModStop06(uint8_t track,uint8_t owner){auto&s=manualMod06[track];if(s.valid&&s.held&&s.owner==owner){s.gateMs=millis()-s.started;s.held=false;++modEpoch805;}}
FLASHMEM bool manualModSource06(uint16_t key,uint32_t&age,uint32_t&gate){
 if(key>=64||!manualModMask06)return false;auto m=mod808EffectiveUnit(key);if(!(m.inputs&manualModMask06))return false;bool found=false;
 float tick=max(0.001f,tickDurationMs());uint32_t now=millis();
 for(uint8_t t=0;t<16;++t)if(m.inputs&(1u<<t)){const auto&s=manualMod06[t];if(!s.valid)continue;
  uint32_t ms=now-s.started,a=(uint32_t)(ms/tick),g=s.held?a+1:(uint32_t)(s.gateMs/tick);
  if(!s.held&&a>g+(modKind931(key)?0:m.release))continue;
  if(!found||a<age){age=a;gate=g;found=true;}
 }return found;
}
FLASHMEM bool manualModActive06(uint16_t key){uint32_t age=0,gate=0;return manualModSource06(key,age,gate);}
