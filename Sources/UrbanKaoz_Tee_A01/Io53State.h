#pragma once
// Virtual channels travel over the existing Snkoz USB serial protocol.
inline bool ioVirtual53(const ModRoute805&r){return r.kind==MOD_CC&&r.block21==4&&r.channel<16;}
uint8_t midiInputPort53=0; // 0 internal/unspecified, 1 USB host/PC, 2 DIN
struct MidiInputScope53 {uint8_t previous;MidiInputScope53(uint8_t p):previous(midiInputPort53){midiInputPort53=p;}~MidiInputScope53(){midiInputPort53=previous;}};
int16_t ioLearnKey53=-1;uint8_t ioLearnPort53=1;
bool ioInputMatches53(const ModUnit805&m){return m.rate<100001||m.rate>100003||m.rate==100001||m.rate==100001+midiInputPort53;}
void ioLearn53(uint8_t,uint8_t,uint8_t,bool);
void ioVirtualSend53(uint8_t,uint8_t,uint8_t);
EXTMEM float ioVirtualSum53[2048];EXTMEM uint16_t ioVirtualCount53[2048],ioVirtualKeys53[2048];uint16_t ioVirtualUsed53=0;
void ioVirtualClear53(){for(uint16_t i=0;i<ioVirtualUsed53;++i){auto k=ioVirtualKeys53[i];ioVirtualSum53[k]=0;ioVirtualCount53[k]=0;}ioVirtualUsed53=0;}
void ioVirtualAdd53(uint8_t c,uint8_t cc,float v){if(c>15||cc>127)return;uint16_t k=c*128+cc;if(!ioVirtualCount53[k])ioVirtualKeys53[ioVirtualUsed53++]=k;ioVirtualSum53[k]+=v;++ioVirtualCount53[k];}
void ioVirtualFlush53(){for(uint16_t i=0;i<ioVirtualUsed53;++i){auto k=ioVirtualKeys53[i];ioVirtualSend53(k/128,k%128,constrain((int)lroundf(ioVirtualSum53[k]/ioVirtualCount53[k]),0,127));}}
EXTMEM int16_t ioVirtualLast53[16][128];
void ioVirtualReset53(){for(auto&row:ioVirtualLast53)for(auto&v:row)v=-1;}
