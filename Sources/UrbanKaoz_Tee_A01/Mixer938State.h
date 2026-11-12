#pragma once
enum MixerAction938:uint8_t { MIX_SELECT938,MIX_PREV938,MIX_MUTE938,MIX_SOLO938,MIX_ROLL938,MIX_DELAY938,MIX_ENV938,MIX_LFO938 };
constexpr uint16_t MIX_DEFAULT938=0|(1<<3)|(2<<6)|(4<<9);
uint16_t mixerMap938[16]={}; // XOR with default; zero is Select/Prev/Mute/Roll.
uint16_t mixerOff938[4]={}; // Roll, Delay, ENV, LFO; individual objects remain untouched.
extern uint8_t programDefault939[16];void programSend939(uint8_t,uint8_t);void programAll28();void programEdit939(int,bool,bool=false);uint8_t programFocus51(uint8_t);void programDisplay939();void programMenu939();void programReset939();
uint8_t rollDir939[16]={};
uint64_t modOff28=0;uint16_t modMixerKey28(uint8_t,uint8_t);void modMixerToggle28(uint8_t,uint8_t);bool modMixer939=false;uint8_t modColors939[4]={3,10,5,6};
uint8_t trackHomePage938=0;
inline uint8_t mixerFunction938(uint8_t t,uint8_t row){if(modMixer939)return row+4;return ((mixerMap938[t]^MIX_DEFAULT938)>>(row*3))&7;}
inline bool mixerOn938(uint8_t t,uint8_t kind){return !(mixerOff938[kind]&(1u<<t));}
void mixerRender938();void mixerTurn938(uint8_t,int);void mixerToggle938(uint8_t,uint8_t);
bool mixerActive938(uint8_t,uint8_t,bool);bool mixerMod938(uint16_t);
bool rvlLock938(uint8_t,uint8_t,int32_t,uint8_t,bool);
bool rvlFixedLock938(uint8_t,uint16_t,uint8_t);
bool rollAllowed938(uint8_t,uint8_t,int32_t,uint8_t,bool);
bool rollNoteAllowed938(uint8_t,uint16_t,uint8_t);
void mixerReset938();bool mixerWrite938(File&);bool mixerRead938(File&,bool);void mixerInstall938();

void selection805Changed();
