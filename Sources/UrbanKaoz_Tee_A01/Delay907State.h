#pragma once
#include "DelayTargets55.h"
void delayTargetsMenu55();void delayTargetsButton55(uint8_t);void delayTargetsRoy55(uint8_t,int,bool);bool delayTargetsKy55(uint8_t,int);void delayTargetsSelect55();bool delayTargetsSend55(uint8_t,uint8_t);
static uint8_t delayTargetSelected55[16][4]={};
struct DelayFocusInit57{DelayFocusInit57(){memset(delayTargetSelected55,4,sizeof(delayTargetSelected55));}};static DelayFocusInit57 delayFocusInit57;
static bool delayTargetsOpen55=false,delayEditDetail55=false;
static bool delayXp56=false;static uint8_t delayEditSlot55=0;
void delayTargetMain56(char*,char*);void delayTargetTurn56(int);
static bool deleteNumeric55=false;
static uint64_t delayDetachPending55=0;
inline bool delayTargetValid55(const delay55::Target&t){if(t.kind==8)return t.track<16&&t.number<128&&t.exp<=100;if((t.kind==5||t.kind==6)&&t.identity>=64)return false;return delay55::valid(t);}
bool delayTargetCell55(uint8_t,char*,char*);
// One implicit default; only edited delay objects allocate an override (never per tick).
struct __attribute__((packed)) DelayValues907 { int8_t velocity=0,note=0,scdo=0;uint8_t mode=0;int8_t accel=0;uint8_t axp=33,oxp=33;uint16_t wait=0,length=384;uint8_t repeats=3,output=16,rolls=1,enabled=1,source=1,chain=0;int8_t fixedVelocity926=50,relativeVelocity926=-40;uint8_t edited930=0;int8_t gat08=0;uint8_t gex08=33,block21=0;delay55::Target targets55[4]={}; };
DelayValues907 delayDefaults932[4];uint8_t delayDefaultsMask932=0;
struct DelayNode907 { DelayNode907*next;uint16_t key;DelayValues907 value; };
DelayNode907 *delayNodes907=nullptr,*delayStage907=nullptr;
DMAMEM DelayNode907* delayIndex925[16][4];
uint8_t delayMask925[16]={},delayChosen925[16]={};
bool delayProjectFile925=false,delayImportPending925=false;
uint8_t delayIncrementShown925=0;
uint8_t delayToggle907[4]={};
void delayImport925();void delayReset925();void delayChanged925(uint8_t,uint8_t,uint8_t,int);
bool delayPresent925(uint8_t,uint8_t);void delayToggle925(uint8_t,uint8_t);
void delayIncrement925(uint8_t);void delayQuickKey925(uint8_t);bool delayQuickDisplay925();
void delayHome925(char*,char*,char*,char*);void delayHomeRoy925(uint8_t,int32_t,bool);
void delayHomeButton925(uint8_t);
void remixEditDelta911(uint8_t,int,bool);void delayNotify925(uint8_t,uint8_t,const DelayValues907&,bool);
void delayNoteLocksReset925();void delayNoteLock925(uint8_t,uint8_t,int);
void pidiNote925(uint8_t,uint8_t,uint8_t,uint32_t,bool);
void manualDelayPrepared06(uint8_t,uint8_t,uint8_t,uint32_t);
void manualDelay1000(uint8_t,uint8_t,uint8_t,uint32_t);
void pidiProject925();void pidiSync925();void pidiRoutes925();
uint8_t delaySlot924=0;int8_t delayChainView924=-1;
uint16_t delayKey924(uint8_t,uint8_t,uint8_t);
bool delayEnabled924(uint8_t,uint8_t,uint8_t);
int delayBaseSlot924(uint8_t,uint8_t,uint8_t,uint8_t);
bool delayAssign924(uint8_t,uint8_t,uint8_t,uint8_t,int);
bool delayAdd924(uint8_t,uint8_t);void delayRemove924(uint8_t,uint8_t,uint8_t);
uint16_t delayList924(uint8_t,uint8_t,uint32_t*,uint16_t);
void delaySelect924(uint16_t);void delayChainClick924();void delayChainTurn924(int);

void delayChainDisplay924(const char*&,char*);

void pidiCancelDelays924();void pidiDelayStatus924();void pidiInvalidate924();

static const uint8_t delayRoyFields923[8] PROGMEM={2,3,1,0,9,10,7,11};
static const char delayRoyLabels923[8][5] PROGMEM={"Rep","Vel","Leng","Wait","Accl","Axp","Scdo","Oxp"};
static const uint8_t delayQvFields925[16]={2,3,1,0,9,10,7,11,4,6,5,8,15,16,255,17};
static const char delayQvNames925[16][5]={"Rep","Vel","Leng","Wait","Accl","Axp","Scdo","Oxp","TRK","Step","Roll","Exp","GAT","GEX","","BLK"};
static const uint8_t delayFields907[8] PROGMEM={4,5,0,1,2,6,3,7};
static const char delayLabels907[8][5] PROGMEM={"Cha","Rol","Wait","Leng","Reps","Note","Vel","Scdo"};
DelayValues907 delayGet907(uint16_t key);
bool delaySet907(uint16_t key,DelayValues907 value);
void delayClear907(DelayNode907*&head);
void delayRemove907(uint16_t key);
bool delayCopy907(uint16_t from,uint16_t to);
int delayBase907(uint8_t t,uint8_t p,uint8_t field);
int delayMin907(uint8_t field);
int delayMax907(uint8_t field);
int delayValue907(uint8_t t,uint8_t p,uint8_t field,bool display);
void delayMode907();
void delayMenu907(char*a,char*b,char*c,char*d);

DelayValues907 delayDefault928(uint8_t);


// Sparse object marker, plus tiny internal-RAM visual masks. No per-tick state.
bool delayMixer930=false;
uint8_t delayWorkedMask930[16]={};
uint16_t delayPadHeld930[4]={},octavePadHeld930[4]={};
uint16_t mixerNotes930=0,mixerValid930=0;
void delayMarkWorked930(uint8_t,uint8_t);
void delaySourceToggle930(uint8_t,uint8_t);
void toggleDelayMixer930();
bool delayMixerPad930(uint8_t,uint8_t,uint8_t);
bool delayMixerRender930();
bool mixerTrackNotes930(uint8_t);
uint32_t mixerMuteColor930(uint8_t,uint8_t);
bool keyboardOctavePad930(uint8_t,uint8_t,uint8_t);
void keyboardOctave930(int);

inline uint8_t delayRoyField21(uint8_t r){return r==2&&delayToggle907[r]==2?17:delayRoyFields923[r*2+delayToggle907[r]];}
