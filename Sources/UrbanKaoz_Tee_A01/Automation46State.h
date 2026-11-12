#pragma once
constexpr uint16_t AUTO_LANES46=256,AUTO_POINTS46=60000,AUTO_NONE46=65535,AUTO_MUTES46=4096;
struct AutoLane46 {ModRoute805 route;uint16_t target,parameter;uint8_t used,track,pattern,type,kind;char name[8];};
// type 0: frozen modulation contribution; 1: parameter/CC/NRPN automation.
struct AutoPoint46 {uint32_t tick,end;float value;uint16_t lane,next;};
struct AutoMute46 {uint32_t tick,end;uint16_t key;uint8_t track,pattern,slot;int32_t identity=-1;uint8_t note=128;int16_t depth=0;};
// Note-scoped windows may cross the loop end; duration stays unchanged.
inline void autoShiftScope49(AutoMute46&m,int32_t delta,uint32_t length){
 if(m.note>=128||!length||m.end<=m.tick)return;
 uint32_t duration=m.end-m.tick;
 int64_t start=((int64_t)m.tick+delta)%length;if(start<0)start+=length;
 m.tick=(uint32_t)start;m.end=m.tick+min(duration,length);
}
inline bool autoContains49(const AutoMute46&m,uint32_t tick,uint32_t length){
 if(m.note>=128||!length)return tick>=m.tick&&tick<m.end;
 return (tick%length+length-m.tick%length)%length<min(m.end-m.tick,length);
}
// Return each linear part for clipboard clipping, without changing live data.
inline uint8_t autoParts49(const AutoMute46&m,uint32_t length,AutoMute46*out){
 out[0]=m;if(m.note>=128||!length)return 1;
 uint32_t duration=min(m.end-m.tick,length);out[0].tick=m.tick%length;
 if(duration==length){out[0].tick=0;out[0].end=length;return 1;}
 out[0].end=out[0].tick+duration;
 if(out[0].end<=length)return 1;
 out[1]=m;out[1].tick=0;out[1].end=out[0].end-length;out[0].end=length;return 2;
}
EXTMEM AutoLane46 autoLanes46[AUTO_LANES46],autoStageLanes46[AUTO_LANES46];
EXTMEM AutoPoint46 autoPoints46[AUTO_POINTS46],autoStagePoints46[AUTO_POINTS46];
EXTMEM AutoMute46 autoMutes46[AUTO_MUTES46],autoStageMutes46[AUTO_MUTES46];
uint16_t autoCount46=0,autoMuteCount46=0,autoStageCount46=0,autoStageMuteCount46=0;
EXTMEM uint16_t autoHead46[AUTO_LANES46],autoCursor46[AUTO_LANES46];
EXTMEM uint32_t autoLastTick46[AUTO_LANES46];
uint16_t autoKinds46=0;uint64_t autoIoMask46=0;bool autoRoute46=false,autoCvPresent46=false,cvEnabled46=false;uint16_t cvValues46[4]={};
EXTMEM int32_t autoSent46[AUTO_LANES46],autoCaptured46[AUTO_LANES46];bool autoSentValid46[AUTO_LANES46]={},autoCaptureValid46[AUTO_LANES46]={};uint16_t autoSentAddress46[AUTO_LANES46]={};
uint8_t patMenu46=0;uint16_t autoSelected46=AUTO_NONE46;
bool autoRecord50=false;
bool autoBusy46=false,autoPlayback46=false;int8_t autoRenderTrack46=-1;int64_t autoRenderTick46=0;
void autoReset46();void autoReindex46();bool autoWrite46(File&);bool autoRead46(File&,bool);void autoInstall46();
void autoClearPattern46(uint8_t,uint8_t);bool autoCopyPattern46(uint8_t,uint8_t,uint8_t,uint8_t);
void autoManual46(uint8_t,uint8_t,uint16_t,uint16_t,int);
int autoParameter46(uint8_t,uint16_t,uint16_t,int);void autoMidi46(int64_t);void autoCv46(float*,uint16_t*,uint16_t*);
bool autoLaneValue46(uint16_t,float&);void patRender46();void patButton46(uint8_t);void patRoy46(uint8_t,int,bool);void patBack46();void autoNameDone46(bool);
bool joyConflict46(uint16_t);
void autoCcInput46(uint8_t,uint8_t,uint8_t);void autoIoInput46(uint16_t,uint8_t);
void writeSelect46();bool writePrepare46();void writeCommit46();void writeFree46();
float writeValue46(const ModUnit805&,uint8_t,int64_t,uint16_t);
int writeLockValue46(uint8_t,uint16_t,uint16_t,int);
bool autoLive46(uint16_t,uint8_t,int64_t,const ModUnit805&,float&,float&);
void autoGraph46();void autoDetach46(uint16_t);int autoDepthDisplay46(uint16_t,uint8_t,int);
void autoMix46();void autoNrpn46();float autoIoValue46(uint16_t,float);
EXTMEM float modRouteValues46[64][8];
bool autoUndo46();bool autoRemove46(uint16_t);void autoUndoClear46();
void autoNoteRemove46(uint8_t,int32_t,uint8_t);void autoNoteMove46(uint8_t,int32_t,uint8_t,int32_t);void autoNotePitch46(uint8_t,int32_t,uint8_t,uint8_t);
void autoNoteUndoCapture46(uint8_t,uint8_t,int32_t,uint8_t);void autoNoteUndoShift46();bool autoNoteUndoRoom46(uint8_t);void autoNoteUndoInstall46(uint8_t);
void autoModHistoryReset46(uint8_t);void autoModCapture46(uint16_t,uint8_t);bool autoModRoom46(uint8_t);void autoModRestore46(uint8_t);

bool autoDepthEdit46(uint16_t,uint8_t,int,int,int);
void autoHistoryClear46();

struct AutoClip46 {IndEntry50*ind=nullptr;uint16_t indCount=0;uint8_t indFlags=0;bool indCaptured=false;AutoClip46*next=nullptr;AutoLane46*lanes=nullptr;AutoPoint46*points=nullptr;AutoMute46*mutes=nullptr;uint16_t laneCount=0,count=0,muteCount=0;};
void autoClipFree46(AutoClip46*&);bool autoClipCapture46(AutoClip46*&,uint8_t,uint8_t,uint32_t,uint32_t);
bool autoClipRoom46(const AutoClip46*);void autoClipInstall46(const AutoClip46*,uint8_t,uint8_t,uint32_t,bool);
AutoClip46*autoClipAlloc46(uint16_t,uint16_t,uint16_t);bool autoClipValid46(const AutoClip46*);void autoClipDetach46(uint16_t);

int autoScopedDepth46(uint8_t,uint8_t,uint16_t,uint8_t,int32_t,uint8_t,int);

void writeDraftClear46();

int autoMidiBase46(uint8_t,uint8_t,uint8_t,int);
