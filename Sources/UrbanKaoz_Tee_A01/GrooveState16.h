#pragma once
// Fields 2 and 4 encode unsigned gate percentage, little endian (format 28).
DMAMEM int8_t grooveOffset[GROOVE_COUNT][GROOVE_STEPS][5];
extern bool grooveBypass23;
bool grooveGateTens51=false;
uint16_t grooveGate51(uint8_t sheet,uint8_t step){if(!sheet||sheet>GROOVE_COUNT)return 100;auto&v=grooveOffset[sheet-1][step%16];return (uint8_t)v[2]|((uint16_t)(uint8_t)v[4]<<8);}
void grooveGateSet51(uint8_t sheet,uint8_t step,int value){value=value<0?0:value>1000?1000:value;auto&v=grooveOffset[sheet-1][step%16];v[2]=(uint8_t)value;v[4]=(uint8_t)(value>>8);}
void grooveLegacy51(){for(uint8_t s=1;s<=GROOVE_COUNT;++s)for(uint8_t i=0;i<16;++i)grooveGateSet51(s,i,100);}
void grooveZeroMigration57(){for(uint8_t s=1;s<=GROOVE_COUNT;++s)for(uint8_t i=0;i<16;++i)if(!grooveGate51(s,i))grooveGateSet51(s,i,100);}
#include "GrooveMath16.h"
uint8_t grooveShape16[GROOVE_COUNT][2]; // per sheet: SMO, width, percent
uint8_t grooveEditTrack23[16]={1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
bool grooveBypass23=false;
bool grooveAllOff59=false;
uint16_t grooveTrackOff59=0;
bool grooveView16=false,oovMenu16=false; uint8_t oovLow16=2,oovHigh16=5,oovOld16[2];
int8_t oovRoy16=-1;
int grooveTime16(uint8_t sheet,int tick){return !grooveBypass23&&sheet&&sheet<=GROOVE_COUNT?groove16::ticks(&grooveOffset[sheet-1][0][3],5,tick,grooveShape16[sheet-1][0],grooveShape16[sheet-1][1]):0;}
void grooveDefaults16(){memset(grooveShape16,50,sizeof(grooveShape16));for(uint8_t s=1;s<=GROOVE_COUNT;++s)for(uint8_t i=0;i<16;++i)grooveGateSet51(s,i,100);}
uint8_t grooveSheet35(uint8_t);void grooveChooseSheet35(uint8_t,uint8_t);void grooveTrack35(uint8_t);
void sendGrooveVisual16(); void oovRender16(); void oovClose16(bool); void oovTurn16(uint8_t,int);
void oovLoad16(){uint8_t b[5];EEPROM.get(3760,b);if(b[0]==0x16&&b[1]>=1&&b[1]<b[2]&&b[2]<=99&&b[3]==(uint8_t)(b[1]^b[2]^0xA6)&&b[4]==0x6F){oovLow16=b[1];oovHigh16=b[2];}}
void oovSave16(){uint8_t b[5]={0x16,oovLow16,oovHigh16,(uint8_t)(oovLow16^oovHigh16^0xA6),0x6F};EEPROM.put(3760,b);}
