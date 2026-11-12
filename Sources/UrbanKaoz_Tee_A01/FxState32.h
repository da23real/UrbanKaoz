#pragma once
#include "Harmony32.h"
harmony32::Config fxConfig32[16],fxStage32[16];
uint8_t chordUses32[145]={};uint16_t fxDirty32=0xffff;bool fxClockValid32=false;
bool chordHarmonic32=true;uint8_t chordDraft32=0,arpField32=0;
void fxSync32();void fxRender32();void fxTurn32(uint8_t,int,bool);void fxButton32(uint8_t);void fxOpen32(uint8_t);
void fxRebuild32(){memset(chordUses32,0,sizeof(chordUses32));for(auto&c:fxConfig32)if(c.chord)++chordUses32[c.chord];fxDirty32=0xffff;}
void stopActiveNotesForTrack(uint8_t);
void fxSet32(uint8_t t,const harmony32::Config&c){if(t>=16||!harmony32::valid(c))return;auto&old=fxConfig32[t];if(!memcmp(&old,&c,sizeof(c)))return;stopActiveNotesForTrack(t);if(old.chord&&chordUses32[old.chord])--chordUses32[old.chord];old=c;if(c.chord)++chordUses32[c.chord];fxDirty32|=1u<<t;}

bool chordDetect35=false;
uint16_t chordNotes35();unsigned chordList35(uint8_t*);void chordRender35();void chordTurn35(uint8_t,int,bool);
