#pragma once
#include "Delay907State.h"

enum ModTarget805 : uint8_t { MOD_NOT, MOD_VEL, MOD_GAT, MOD_TIM, MOD_PBN,
  MOD_RST, MOD_RSP, MOD_RAC, MOD_REX, MOD_RCV, MOD_RLV, MOD_CC, MOD_VIRTUAL };
struct ModRoute805 { uint8_t kind, track, number, enabled; int16_t depth; uint8_t port=0,channel=0; int16_t reference=0; uint8_t block21=0;uint32_t patch21=0,param21=0; };
bool cvShared50(uint8_t ch,uint16_t skip,uint8_t slot);
constexpr uint32_t LFO_MAX42=3599999;
struct ModLegacy42 {uint16_t inputs,hold,attack,decay,release,rate;uint8_t sustain,curve,shape,enabled;ModRoute805 outputs[8];};
struct ModUnit805 {
  uint16_t inputs; uint16_t hold, attack, decay, release;
  uint32_t rate; uint8_t sustain, curve, shape, enabled;
  uint8_t roll58=0,delay58=0; // OFF=0 ON=1 ONLY=2; filters are per modulation.
  ModRoute805 outputs[8];
};
// LFO27: decay stores envelope wait (hold remains Bar trigger interval),
// attack/release retain their duration meaning; sustain=126 is explicit On.
// Legacy readers below reset sustain to 80, so missing information is Off.
struct FreeClock48 {uint64_t at;double phase;float period;bool valid;};
EXTMEM FreeClock48 freeClock48[64];
inline uint8_t lfoEnvMode36(const ModUnit805&m){return m.sustain>=123&&m.sustain<=126?127-m.sustain:0;}
inline void lfoEnvSet36(ModUnit805&m,uint8_t mode){m.sustain=mode?127-mode:80;}
inline bool lfoEnvelopeOn27(const ModUnit805&m){return lfoEnvMode36(m)!=0;}
inline const char* lfoEnvName36(const ModUnit805&m){const char*n[]={"OFF","CYC","NOT","BAR","BEA"};return n[lfoEnvMode36(m)];}
void toggleTimeUnit38();
bool fineTimeUnit38();

inline float lfoEnvelope27(const ModUnit805&m,int curve,float age){
 if(!lfoEnvelopeOn27(m))return 1;
 if(age<0||age<m.decay)return 0;age-=m.decay;
 if(m.attack&&age<m.attack)return age/m.attack;age-=m.attack;
 if(!m.release||age>=m.release)return 0;
 float x=age/m.release;
 // Positive curvature delays the fall; +100 stays >99% through 80%.
 float power=powf(32.0f,abs(curve)/100.0f);
 return curve>=0?1-powf(x,power):powf(1-x,power);
}
// Compact live pool. Only the disk codec knows the legacy 512-slot layout.
EXTMEM ModUnit805 modUnits805[64]; // Common ENV/LFO pool, independent of patterns.
uint8_t modKinds931[64]={},fullKinds931[64]={};
bool modStaging931=false;
uint8_t modKind931(uint16_t key){return key<64?(modStaging931?fullKinds931[key]:modKinds931[key]):0;}
int16_t fullMap931[512]; // Legacy disk identity -> compact slot, only used during loading.
void modDetach45(uint16_t key,uint8_t history=255);
void modHistoryPrepare45(uint8_t count);
bool modHistoryReady45(uint8_t index);
void modHistoryRestore45(uint8_t index);
bool saveCheck45(bool clean);
struct NoteLock817;
void lockDiagnostic45(const char*,uint16_t,const NoteLock817&);
uint16_t modFreeSlot931(){for(uint16_t key=0;key<64;++key)if(!modUnits805[key].enabled)return key;return UINT16_MAX;}
uint16_t modDiskKey931(uint16_t key){return key<64?key*2+modKinds931[key]:UINT16_MAX;}

// Input CC state is sparse per existing envelope.

struct Delay805 { uint16_t ticks; uint8_t repeats, velocity, enabled; };
EXTMEM Delay805 delays805[16][16];
struct VirtualLink805 { uint8_t bus, kind, track, enabled; int16_t depth; };
VirtualLink805 virtualLinks805[32] = {};
uint8_t modUi805 = 0, modType805 = 0, modOwnerTrack805 = 0, modOwnerPattern805 = 0;
uint8_t modOutputCursor805 = 0;
extern int8_t modDepthSlot806;
extern uint8_t modDestCursor807;
int64_t modLogicalTick805 = -1;
uint32_t modEpoch805 = 1;
// Bench-selectable, exact optimizations: index, lease-local invalidation,
// shared constant address, identical free-LFO sample, musical service between I2C reads.
// I2C interleaving stays experimental: lower latency but more scheduler work.
uint8_t optim31=31;
uint32_t leaseEpoch31=0,indexBuilds31=0,sharedHits31=0,sampleHits31=0;
inline uint32_t registryEpoch31(){return modEpoch805-((optim31&2)?leaseEpoch31:0);}
inline void leaseChanged31(){++modEpoch805;++leaseEpoch31;}

#include "Io38State.h"
#include "CcEnvelope941.h"
void mod805Init();
void mod805Invalidate();
void mod805Open(uint8_t type);
void mod805Menu();
bool mod805Screen(uint8_t index);
bool mod805Ky(uint8_t index, int32_t delta);
bool mod805Display();
bool mod805Back();
int mod805Offset(uint8_t track, uint8_t parameter, int64_t at);
void mod805Service();
void mod805Note(uint8_t track, uint8_t note, uint8_t velocity, uint32_t gateUs);
bool mod805OwnerPresent(uint8_t track, uint8_t pattern);
void mod805SongApply(uint8_t track, int64_t logical, int32_t &at, int &note, int &vel, uint32_t &gate);


#include "Mod806Model.h"
EXTMEM ModMeta806 modMeta806[MOD806_CAPACITY];
enum ModPage806 : uint8_t { MOD_CLOSED806, MOD_LIST806, MOD_OBJECT806,
  MOD_SOURCES806, MOD_DESTS806, MOD_SOURCE806, MOD_DEST806,
  MOD_REMOVE806, MOD_CONFIRM806, MOD_FX806, MOD_DELAY806, MOD_CC941 };
uint16_t modSelected806 = UINT16_MAX;
uint16_t modPage806 = 0, modReturnPage806 = 0, modListPage806 = 0;
uint8_t modParent806 = 0, modConfirmParent806 = 0;
uint8_t modSourceKind806 = 0, modSourceIndex806 = 0;
#include "SourcePulse909.h"
SourcePulse909 sourcePulse909;
uint8_t sourceWatchKey909(){return modUi805==MOD_SOURCE806?modSourceKind806*16+modSourceIndex806:255;}
void sourceTrigger909(uint8_t kind,uint8_t index){sourcePulse909.watch(sourceWatchKey909());sourcePulse909.trigger(kind*16+index,millis());}
bool modRemove806 = false, modAdding806 = false;
uint8_t modCommittedSourceKind806 = 0, modCommittedSourceIndex806 = 0;
uint8_t modDirty806 = 0;
uint32_t modBlinkMs806 = 0;
bool modBlink806 = false;
ModUnit805 &mod806Unit(uint16_t key);
bool mod806Playing(uint16_t key, int64_t at);
bool mod806Roy(uint8_t roy, int32_t delta, bool click);
bool mod806Navigate(int x);
bool mod806Delete();
void mod806DetachPattern(uint8_t track, uint8_t pattern);
void mod806CopyPattern(uint8_t fromTrack,uint8_t fromPattern,uint8_t toTrack,uint8_t toPattern);
void mod806UiService();
void mod806MidiService();

void mod806Checks();
bool mod807Undo();
void mod807ClearHistory();

void mod806UsbNoteOn(uint8_t channel,uint8_t note,uint8_t velocity);
void mod806UsbNoteOff(uint8_t channel,uint8_t note,uint8_t velocity);
bool mod806ChannelBend(uint8_t channel,int64_t at,int &out);

struct DelayExtra817 { uint16_t wait; uint8_t output, rolls; };
EXTMEM DelayExtra817 delayExtra817[16][16], fullDelayExtra817[16][16];
bool rollTrigger817=false, delayMenuDirty817=false;
uint8_t delayTimeMode817[2]={};
void delayEdit817(uint8_t field,int32_t delta);
void followTrack817();

