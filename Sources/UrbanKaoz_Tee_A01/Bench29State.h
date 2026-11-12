#pragma once
// Explicit synthetic benchmark only; no user-state capture or persistence.
static uint32_t transitionMax59[8]={};
static bool benchActive29=false,benchPrepared29=false;
static uint32_t benchTickLimit29=0,benchTicks29=0,benchStart29=0,benchElapsed29=0;
static uint32_t benchOn29[8]={},benchOff29[8]={},benchQueuePeak29=0;
static uint32_t benchTickHist29[16]={},benchTickMax29=0;static uint64_t benchTickSum29=0;
static uint8_t benchBlocks29=1,benchTracks29=1,benchNotes29=4;
inline void benchMidi29(uint8_t s,uint8_t b,uint8_t mask){if(!benchPrepared29)return;for(uint8_t p=0;p<8;++p)if(mask&(1u<<p)){if((s&0xf0)==0x90&&b)++benchOn29[p];else if((s&0xf0)==0x80||((s&0xf0)==0x90&&!b))++benchOff29[p];}}
inline void benchTickSample29(uint32_t us){benchTickMax29=max(benchTickMax29,us);benchTickSum29+=us;uint8_t i=0;while(i<15&&us>(1UL<<i))++i;++benchTickHist29[i];}
