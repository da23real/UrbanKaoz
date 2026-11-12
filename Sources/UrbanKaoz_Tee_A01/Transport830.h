#pragma once
// RAM-only profiling. Nested scopes report inclusive AND exclusive time.
// DWT subtraction is modulo 32 bits (7.15 s at 600 MHz); transport wall time
// remains measured by micros() in the existing OPT821 diagnostic.
enum Phase830 : uint8_t { P830_TRANSPORT, P830_TICK, P830_TRACK, P830_AGENDA,
  P830_LIST, P830_SONG, P830_BRIDGE, P830_SOURCE, P830_CURVE, P830_FRAME,
  P830_COLLECT, P830_FLUSH, P830_OFFER, P830_MIDI, P830_PIDI, P830_DEPS30, P830_INDEX30, P830_ADDR30, P830_ROLL30, P830_RVL30, P830_COUNT };
struct Stat830 { uint64_t inclusive,exclusive; uint32_t count,maximum; uint8_t track; };
EXTMEM Stat830 stats830[P830_COUNT];
static bool profile830=false,optimize830=true;
static uint8_t profileTrack830=255;
struct Scope830;
static Scope830* scope830=nullptr;
struct Scope830 {
  Scope830* parent; uint32_t start,children; uint8_t phase,track; bool enabled;
  Scope830(uint8_t p):parent(scope830),children(0),phase(p),track(profileTrack830),enabled(profile830){
    if(enabled){start=ARM_DWT_CYCCNT;scope830=this;}
  }
  ~Scope830(){if(enabled){uint32_t elapsed=ARM_DWT_CYCCNT-start;auto&s=stats830[phase];
    ++s.count;s.inclusive+=elapsed;s.exclusive+=elapsed-children;
    if(elapsed>s.maximum){s.maximum=elapsed;s.track=track;}
    scope830=parent;if(parent)parent->children+=elapsed;
  }}
};
static uint32_t sourceHits830=0,sourceMisses830=0,sourceVoices830=0,frameBuilds830=0;
static uint32_t flushVisited830=0,agendaFallback830=0,agendaEligible830=0;
static uint32_t lateTicks830=0,lateNotes830=0,maxTickAge830=0,maxNoteAge830=0;
// Exact enqueue timestamps, independent of later tempo changes. pendingClockTicks
// remains the queue length, so the existing stop/jump resets also discard dates.
EXTMEM volatile uint32_t clockDates830[4096];
static volatile uint16_t clockWrite830=0;
extern volatile uint32_t clockOverruns821;
static bool clockContext830=false;
static uint32_t tickDue830=0;
// Initial audible tolerance, exposed in LATSTAT830. Not a claim of calibration.
static constexpr uint32_t LATE_LIMIT830=20000;
inline void clockEnqueue830(uint32_t at){
  // Keep the musical tick count even when the timestamp history fills up.
  // Only dates older than 4096 ticks are overwritten (>=204.8 ms at the
  // fastest allowed 50 us clock). Such ticks are certainly already expired.
  if(pendingClockTicks==UINT32_MAX){++clockOverruns821;return;}
  if(pendingClockTicks>=4096)++clockOverruns821;
  clockDates830[clockWrite830]=at;clockWrite830=(clockWrite830+1)&4095;++pendingClockTicks;
}
// Caller holds interrupts off. Overwritten dates use the oldest retained date
// as a conservative lower bound on age, without discarding musical ticks.
inline bool clockTake830(){
  if(!pendingClockTicks)return false;
  tickDue830=clockDates830[pendingClockTicks>4096?clockWrite830:(clockWrite830-pendingClockTicks)&4095];
  --pendingClockTicks;return true;
}
inline uint32_t tickAge830(){return clockContext830?(uint32_t)(micros()-tickDue830):0;}
inline bool tickExpired830(){return clockContext830&&tickAge830()>LATE_LIMIT830;}
FLASHMEM bool rejectLateNote830(){
  if(!clockContext830)return false;
  uint32_t age=tickAge830();maxNoteAge830=max(maxNoteAge830,age);
  if(age<=LATE_LIMIT830)return false;
  ++lateNotes830;return true;
}
FLASHMEM void resetStats830(){
  memset(stats830,0,sizeof(stats830));sourceHits830=sourceMisses830=sourceVoices830=frameBuilds830=0;
  flushVisited830=agendaFallback830=agendaEligible830=0;
  lateTicks830=lateNotes830=maxTickAge830=maxNoteAge830=0;
}
void status830();
void checks830();
bool timingMod830(uint8_t track);
static uint32_t agendaSlice830=0;
static bool agendaBuilding830=false;

