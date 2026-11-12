#pragma once
// Temporary USB diagnostics. No SD/EEPROM writes; no allocation or serial output
// in measured paths. Nested durations are inclusive, never add them together.
enum DiagKind814 : uint8_t { D814_TRACK, D814_CLEAR, D814_VIEW, D814_AUDITION,
  D814_MENU, D814_NEO, D814_TX, D814_CATCHUP, D814_TRANSPORT, D814_LOOP, D814_COUNT };
struct DiagEvent814 { uint32_t at,us,ms,cycles,ticks; uint8_t kind,track,mode,reserved; };
struct DiagStat814 { uint32_t count,maxUs,maxMs,maxTicks; };
static DiagEvent814 diagEvents814[64];
static DiagStat814 diagStats814[D814_COUNT];
static bool diagActive814=false;
static bool diagFrozen814=false;
static uint8_t diagNeo814=0,diagWrite814=0,diagUsed814=0;
static int8_t diagQueuedTrack814=-1;
static uint32_t diagLease814=0,diagOverwritten814=0,diagGeneration814=0;
extern volatile uint32_t pendingClockTicks;
extern uint8_t selectedTrack;
static void diagRecord814(uint8_t kind,uint32_t us,uint32_t ms,uint32_t cycles,uint32_t ticks,uint8_t track){
  auto&s=diagStats814[kind];++s.count;
  if(us>s.maxUs)s.maxUs=us;if(ms>s.maxMs)s.maxMs=ms;if(ticks>s.maxTicks)s.maxTicks=ticks;
  // Keep slow operations plus every track change. Ring overwrites oldest entries.
  if(diagFrozen814||(us<10000&&kind!=D814_TRACK))return;
  diagEvents814[diagWrite814]={millis(),us,ms,cycles,ticks,kind,track,diagNeo814,0};
  diagWrite814=(diagWrite814+1)%64;
  if(diagUsed814<64)++diagUsed814;else ++diagOverwritten814;
  // Preserve the first >=500 ms loop and its preceding operations for later dump.
  // Counters continue, only the event ring freezes. CLEAR re-arms the capture.
  if(kind==D814_LOOP&&us>=500000)diagFrozen814=true;
}
struct DiagScope814 {
  bool enabled;uint8_t kind,track;uint32_t startUs,startMs,startCycles,ticks,generation;
  DiagScope814(uint8_t k,uint32_t t=0):enabled(diagActive814),kind(k),track(selectedTrack),ticks(t){
    if(enabled){generation=diagGeneration814;startMs=millis();startCycles=ARM_DWT_CYCCNT;startUs=micros();}
  }
  ~DiagScope814(){if(enabled&&diagActive814&&generation==diagGeneration814){
    uint32_t us=micros()-startUs,cycles=ARM_DWT_CYCCNT-startCycles,ms=millis()-startMs;
    diagRecord814(kind,us,ms,cycles,ticks,track);
  }}
};
static void diagReset814(){memset(diagStats814,0,sizeof(diagStats814));diagWrite814=diagUsed814=0;diagOverwritten814=0;diagFrozen814=false;++diagGeneration814;}
