#pragma once
void checks902();
uint8_t midiChannelForTrack(uint8_t track);
uint8_t modeColors902[4]={5,4,1,3};
uint8_t trackOut902[16]={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
bool trackMidiMenu902=false;uint8_t trackMidiDraft902=1;
const char* modeNames902[4]={"SELECT","PREV","MUTE","SOLO"};
uint8_t& colourChoice902(uint8_t role){return role<8?neoColorChoice[role]:modeColors902[role-8];}
const char* colourName902(uint8_t role){return role<8?NEO_COLOR_ROLE_NAME[role]:modeNames902[role-8];}
struct Settings902 {uint32_t magic;uint8_t outputs[16],colors[4];uint16_t crc;};
void save902(){Settings902 s={};s.magic=0x554B0A12;memcpy(s.outputs,trackOut902,16);memcpy(s.colors,modeColors902,4);s.crc=pidi827::crc((uint8_t*)&s,24);EEPROM.put(3000,s);}
void load902(){Settings902 s={};EEPROM.get(3000,s);if((s.magic!=0x554B0902&&s.magic!=0x554B0A12)||s.crc!=pidi827::crc((uint8_t*)&s,24))return;if(s.magic==0x554B0902){for(auto v:s.outputs)if(v>8)return;for(auto&v:s.outputs)v=v?1u<<(v-1):0;}for(auto v:s.colors)if(v>=NEO_PALETTE_COUNT)return;memcpy(trackOut902,s.outputs,16);pidiRoutesDirty931=0xffff;memcpy(modeColors902,s.colors,4);}
uint8_t outMask902(uint8_t t){return blockMask21(t);}
uint16_t midiContext902=256;uint8_t midiTrack940=255;
struct MidiScope902 {uint16_t old;uint8_t oldTrack;MidiScope902(uint8_t mask,uint8_t track=254):old(midiContext902),oldTrack(midiTrack940){midiContext902=mask;if(track!=254)midiTrack940=track;}~MidiScope902(){midiContext902=old;midiTrack940=oldTrack;}};
uint8_t channelMask902(uint8_t ch){uint8_t mask=0;for(uint8_t t=0;t<16;++t)if(midiChannelForTrack(t)==ch)mask|=outMask902(t);return mask;}
uint8_t midiMask902(uint8_t ch){return midiContext902==256?channelMask902(ch):midiContext902;}


uint8_t firstOut12(uint8_t t){uint8_t m=outMask902(t);for(uint8_t i=0;i<8;++i)if(m&(1u<<i))return i;return 0;}
