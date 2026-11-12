#pragma once
#include "Io53State.h"
// I/O shares the bounded modulation pool and destination graph. ENV's unused
// shape/curve pair tags its serialized source fields; no duplicate route pool.
inline bool ioUnit38(const ModUnit805&m){return m.shape==4&&m.curve==2;}
inline bool ioKey38(uint16_t k){return k<64&&!modKind931(k)&&ioUnit38(modUnits805[k]);}
bool ioMenu38=false;
uint64_t ioMask38=0;
EXTMEM float ioSample38[64]={};bool ioSeen38[64]={};
EXTMEM uint32_t ioMidiStart38[16][128]={};
inline bool ioActive38(uint16_t k){return ioKey38(k)&&ioSeen38[k];}
float autoIoValue46(uint16_t,float);
inline float ioLevel38(uint16_t k){const auto&m=modUnits805[k];return ioSeen38[k]?constrain((autoIoValue46(k,ioSample38[k])-m.decay)/max((uint16_t)1,m.release),-1.0f,1.0f):0;}
void ioCc38(uint8_t,uint8_t,uint8_t);
void ioNote38(uint8_t,uint8_t,uint8_t,uint32_t,bool);
void ioMidiNote38(uint8_t,uint8_t,uint8_t,bool);
bool ioDisplay38();bool ioKy38(uint8_t,int32_t);bool ioMenuRender38();bool ioRoy38(uint8_t,int32_t,bool);
const char* ioSourceName38(uint16_t);
