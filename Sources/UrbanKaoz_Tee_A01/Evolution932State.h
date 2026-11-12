#pragma once
// ROY: NO periodic safety scans/resends/resync. A targeted recovery requires
// explicit authorization. This does not prohibit Note Off or nonblocking queues.
uint8_t patternColors932[4]={0,6,3,5},trackMixColors932[4]={0,10,7,9},delayMixColors932[4]={0,10,8,9};
uint8_t indColors51[4]={8,8,2,9};bool indColorsPage51=false;
uint8_t colorGroup932=0,colorBackup932[4]={};
uint8_t colorBrightness11[8][4]={{40,100,100,80},{100,100,100,100},{100,100,100,100},{100,100,100,100},{100,100,100,100},{100,100,100,100},{100,100,100,100},{100,100,100,100}};
uint8_t plxColors11[2]={10,7},plxBrightness11[2]={50,100},colorLast11=0;bool colorPlxRow11=false;
uint32_t colorScale11(uint32_t,uint8_t);uint8_t colorRoleBrightness11(uint8_t);void colorsSave11();void colorsLoad11();void colorsClick11(uint8_t);void colorsBrightness11(int);bool colorsPreview11();uint32_t plxColor11(uint8_t,bool,bool);uint8_t&colorRef932(uint8_t);
uint8_t lfoDigit932=0,lfoQuantum932=0;
bool assignOpen932=false;uint8_t assignButton932=255,assignFunction932=255,assignLed932=255;
int8_t assignConflict932=-1;
uint8_t buttonMap932[64],buttonLed932[64];
inline void musical937(char*s,int ticks){if(ticks&&ticks%384==0){while(*s==' '||*s=='+'||*s=='-')++s;if(*s>='0'&&*s<='9')*s=(char)(0xA0+*s-'0');}}
bool neoInput932=false;
void alertAction08();
struct NeoInputScope932 {bool saved;NeoInputScope932(bool active=true):saved(neoInput932){neoInput932|=active;}~NeoInputScope932(){neoInput932=saved;}};
void loadPrefs932();void savePrefs932();void loadDelayDefaults932();void applyDelayDefaults932();
uint32_t songPcSeen59=0;bool songPcPresent59=false;
void service932();bool songScreen932();void songMissing932();
bool lfoMs932(uint16_t);int lfoStep932(uint16_t);void lfoIncrement932(int delta=1);void lfoText932(char*,uint32_t,uint16_t);bool lfoClick932(uint8_t,bool);
void colorsOpen932(uint8_t);void colorsRender932();void colorsTurn932(uint8_t,int);void colorsCommit932();void colorsCancel932();
uint32_t palette932(uint8_t,uint8_t);uint32_t mixerColor932(uint8_t,uint8_t,bool);
void assignRender932();void assignStart932();void assignTurn932(uint8_t,int);void assignChoice932(bool);void assignCancel932();
bool assignCapture932(uint64_t);uint64_t mapButtons932(uint64_t);uint8_t mappedLedState932(uint8_t);

