#pragma once
FLASHMEM void snkozDarkA01(){
 for(uint8_t n=0;n<4;++n){for(uint8_t p=0;p<16;++p)neoPixel821(n,p,0);diagShowNeo814(n);}
}
// Event-driven test output. No periodic recovery, retry, or musical-state allocation.
FLASHMEM void snkozMidi935(bool on){
 if(!snkozMaskA01)return;
 pidiEnsure827();uint8_t bytes[]={(uint8_t)((on?0x90:0x80)|snkozChannelA01),snkozPitchA01,(uint8_t)(on?100:0),snkozMaskA01};
 // Direct MIDI packet only: no audition/modulation/manualDelay call.
 // Note Off uses the same frozen channel, pitch and output as Note On.
 if(pidiPacket827(pidi827::MIDI,bytes,4)){MidiScope902 route(snkozMaskA01,snkozTrackA01);synkozNote934(bytes[0],bytes[1],bytes[2]);snkozNote935=on;}
}
FLASHMEM void snkozVisual935(){
 // Automatic series only: dark preparation; normal capture retains 4,3,2,1.
 if(snkozSeriesA01&&synkozPreroll933){snkozDarkA01();return;}
 uint8_t beat=(synkozTick933/PPQN)%4;
 const uint16_t digits[]={0x4755,0x7467,0x7247,0x7232}; // 4,3,2,1; row-major 4x4
 const uint32_t countColors[]={0xFF0000,0xFF5000,0xFFFF00,0x00FF00};
 bool first=synkozTick933<8UL*PPQN;
 uint32_t color=snkozSeriesA01?0xFF5000:(synkozPreroll933?countColors[beat]:(first?(beat?0xFF0000:0xFF5000):(beat?0x205060:0x402060)));
 for(uint8_t n=0;n<4;++n)for(uint8_t p=0;p<16;++p)
  neoPixel821(n,p,n==beat&&(!synkozPreroll933||(digits[beat]&(1u<<p)))?color:0);
 for(uint8_t n=0;n<4;++n)diagShowNeo814(n);
}
