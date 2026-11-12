#pragma once
char trackNames07[16][8]={},modNames07[64][8]={};
char trackNamesStage07[16][8]={},modNamesStage07[64][8]={};
uint8_t nameOwner07=0,nameReturnMod07=0;
uint16_t nameMod07=0;
bool modPresetDirty07[64]={};
uint8_t nameLimit07(uint8_t target){return target>=3?7:target==0?48:target==1?6:3;}
void objectName07(uint8_t target);
void objectNameDone07(bool commit);
void modName07(uint16_t key,char*out,size_t size){if(key<64&&modNames07[key][0])snprintf(out,size,"%s",modNames07[key]);else snprintf(out,size,"%s%02u",modKind931(key)?"LFO":"ENV",key+1);}
bool patchEnter07();bool patchParent07();
uint8_t delayLabelChannel07=16,delayLabelMenu07=0;
void delayLabelRender07();bool delayLabelRoy07(uint8_t,int,bool);
void quickModClick07(uint8_t ky);
void modPresetTurn07(uint16_t key,int delta);
const char* modModeName07(uint8_t mode);
uint8_t modNextMode07(uint8_t mode,int delta,bool lfo);
void modSetMode07(uint16_t key,uint8_t mode);
float ccEnvelope07(const ModUnit805&m,uint16_t key);
bool ccFine07[64]={};

bool modTimingKey36(uint16_t,uint8_t);
void modTimingClick36(uint16_t,uint8_t);
