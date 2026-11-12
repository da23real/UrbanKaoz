#pragma once
// ROY: no polling recovery, periodic resends or automatic resynchronization.
// Musical clocks and bounded nonblocking queues are required, not recovery scans.
bool synkozEnabled933=false,synkozPreroll933=false,synkozTaking933=false;
uint32_t synkozTick933=0,synkozSequence933=0,synkozDrops933=0;
struct SynkozEvent933 {uint32_t seq,us,tick;uint8_t type,note,velocity,channel;};
SynkozEvent933 synkozQueue933[64];uint8_t synkozHead933=0,synkozCount933=0;
uint16_t delayMute933=0;int16_t trackCc933[16]={};uint16_t trackCcMask933=0;
uint8_t trackPage933=0;int16_t trackDraft933=0;
void synkozService933();bool synkozClock933();void synkozStop933();
bool synkozCommand933(const char*,unsigned,const char*);
void loadPrefs933();void trackOpen933(uint8_t);void trackButton933(uint8_t);void trackRoy933(uint8_t,int,bool);
void delayEntry933();

bool pcNumber831(const char*,long&);

bool pcNumber831(const char*,long&);

uint16_t fullDelayMute933=0,fullCcMask933=0;int16_t fullCc933[16]={};
void installPrefs933();

bool functionAllowed932(uint8_t);

// V9.34: one resume anchor, never a per-tick snapshot or recovery scan.
bool synkozResume934=false,synkozSong934=false;
uint64_t synkozAnchor934=0;uint32_t synkozMaxResume934=0;
void synkozNote934(uint8_t,uint8_t,uint8_t);
bool snkozTest935=false,snkozNote935=false,snkozSeriesA01=false;
// A01 test routing: frozen from a UK track, no modulation/Delay input.
bool snkozConfiguredA01=false;
uint8_t snkozTrackA01=0,snkozPitchA01=60,snkozChannelA01=0,snkozMaskA01=1;
void snkozVisual935();

bool controller940=false;bool playHeld923=false;uint32_t playAt923=0,playThreshold923=0;uint8_t controllerLeds940[16]={};
void controllerSet940(bool active);
void controllerEmit940(const char*kind,int id,int value);
