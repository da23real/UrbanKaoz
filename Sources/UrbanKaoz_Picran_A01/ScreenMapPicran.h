#pragma once
#include "ScreenMapCore.h"
uint8_t screenMapRoles[4]={2,1,3,4};
bool screenPhysicalPresent[4]={};
bool stepLayoutVisible26=false;uint8_t stepLayoutValue26=1;
uint16_t grooveUsers26=0;
uint8_t grooveTrack35=1,grooveList35=1;
uint8_t screenIdentify26=255;bool screenIdentifyPhase26=true;
void screenIdentifyService26(){static uint8_t previous=255;static bool phase=true;bool next=(millis()/350)%2==0;if(previous==screenIdentify26&&phase==next)return;
 uint8_t old=previous;previous=screenIdentify26;phase=next;screenIdentifyPhase26=next;
 for(uint8_t p=0;p<4;++p)if(screenPhysicalPresent[p]&&(p==old||p==screenIdentify26)){auto &bus=Wire1;bus.beginTransmission(LCD_ADDRESS[p]);bus.write(p==screenIdentify26&&!next?0:8);bus.endTransmission();}}
// Initialization/recovery use local physical descriptors; renderers use screens[].
void screenMapInvalidateGlyphs();
void screenMapPresence(){for(unsigned role=0;role<4;++role){screens[role].present=false;for(unsigned p=0;p<4;++p)if(screenPhysicalPresent[p]&&screenMapRoles[p]==role+1)screens[role].present=true;}}
void screenPhysicalWrite(const LcdScreen &s,uint8_t value){s.bus->beginTransmission(s.address);s.bus->write(((screenIdentify26<4&&s.address==LCD_ADDRESS[screenIdentify26]&&!screenIdentifyPhase26)?(value&~0x08):(value|0x08)));s.bus->endTransmission();}
void screenMapWrite(const LcdScreen &s,uint8_t value){for(unsigned role=0;role<4;++role)if(&s==&screens[role]){for(unsigned p=0;p<4;++p)if(screenPhysicalPresent[p]&&screenMapRoles[p]==role+1){LcdScreen hw=s;hw.address=LCD_ADDRESS[p];screenPhysicalWrite(hw,value);}return;}screenPhysicalWrite(s,value);}
