#pragma once
inline uint8_t rollKey26(uint8_t k){static const uint8_t order[16]={10,11,0,1,8,12,3,13,2,4,6,7,14,15,5,9};return order[k&15];}
// Additional roll values allocate only when they differ from the legacy defaults.
struct RollExtra1000 { int16_t v[8]={33,0,-60,0,0,0,0,0}; }; // Vex, relative velocity mode/value, NOS, fixed NOS, ROT
struct RollWindow44 {int64_t begin=-1,end=-1;uint32_t identity=0;};
RollWindow44 rollWindows44[16];
uint32_t rollHoldUs44=0;uint64_t rollIdentity44=0;
void rollEvent44(uint8_t,const TrackRollSettings&,const RollExtra1000&,int32_t,int32_t,uint32_t,uint8_t direction=255);
struct RollNode1000 { RollNode1000* next; uint8_t track; RollExtra1000 value; };
RollNode1000* rollNodes1000=nullptr;
RollExtra1000 rollExtra1000(uint8_t t,uint8_t p=255,int32_t tick=-1000,uint8_t note=255);
int rollVelocity1000(const TrackRollSettings&,const RollExtra1000&,uint8_t,uint16_t);
int rollPitch1000(const RollExtra1000&,uint8_t,uint16_t);
int rollStop09(const TrackRollSettings&,const RollExtra1000&);
void rollEdit1000(uint8_t field,int delta);
void rollClick1000(uint8_t field);
bool rollWrite1000(File&);
bool rollRead1000(File&,bool);
void rollInstall1000();
void rollReset1000();

void rollQuickDisplay1000();bool rollQuickEdit1000(uint8_t,int);

void rollQuickDisplay1000();bool rollQuickEdit1000(uint8_t,int);

void rollDefault15(uint8_t track);

inline int32_t rollWave60(int32_t delta,uint8_t mode,uint16_t i){
 static const int16_t wave[8]={427,337,120,-97,-157,-97,120,337};
 return mode==101?(int32_t)((int64_t)delta*wave[i&7]/256):mode==102?((i&7)<4?delta*2:0):delta;
}
void rollAdvance60(int32_t& spacing,int32_t& acceleration,uint8_t mode,uint16_t index);
bool rollClick60(uint8_t key,bool down);

int rollDirection60(uint8_t t,uint8_t p,int32_t tick,uint8_t note,bool draft=false);
