#pragma once
void indPatchRefresh51(uint8_t t,int parameter=-1);
#include "PatternInd50.h"
EXTMEM IndStore50 ind50,indStage50;
int8_t indContext50=-1;
bool indEditMode52=false; // Pad-toggle mode only; value editing follows the pattern.
bool indEditing59();uint8_t indFocus59(uint8_t);bool indEnabled59(uint8_t,uint8_t);
void indToggle52();
uint8_t indEditTrack50=0,indEditPattern50=0;
int indFamily50(uint8_t,uint16_t,uint16_t);
int indBase50(uint8_t,uint8_t,uint8_t,uint16_t,uint16_t,int);
int indLive50(uint8_t,uint16_t,uint16_t,int);
int indDisplay50(uint8_t,uint16_t,uint16_t,int);
bool indEdit50(uint8_t,uint16_t,uint16_t,int,int,int,int);
void indChanged50();void indRender50();bool indPad50(uint8_t,uint8_t,uint8_t);void indButton50(uint8_t);
void indReset50();bool indWrite50(File&);bool indRead50(File&,bool);void indInstall50();
struct IndContext50 {int8_t previous;IndContext50(int8_t t):previous(indContext50){indContext50=t;}~IndContext50(){indContext50=previous;}};

int16_t envEarly50[64]={},cvShift50[4]={};
int64_t queryTick50=-1;int32_t queryOffset50=0;
int holdBase50(uint16_t,const ModUnit805&);int holdValue50(uint16_t,const ModUnit805&);void holdSet50(uint16_t,int);
void cvTimingRender50();bool cvTimingRoy50(uint8_t,int32_t,bool);
struct ModSourceCache805;bool upcomingSource50(uint8_t,int64_t,ModSourceCache805&);
struct QueryTime50 {int64_t previous;int32_t offset;QueryTime50(int64_t at,int32_t shift):previous(queryTick50),offset(queryOffset50){queryTick50=at;queryOffset50=shift;}~QueryTime50(){queryTick50=previous;queryOffset50=offset;}};
uint16_t indModOwners50(uint16_t);uint8_t indRuntimePattern50(uint8_t,uint16_t=65535);
constexpr uint8_t IND_NOTE50=16,IND_ROLL50=17,IND_LIST50=18;
int indNote50(uint8_t,uint8_t,bool=false);bool indNoteEdit50(uint8_t,int);bool indPatternEdit50(uint8_t,int32_t,int8_t);
int indPatchValue50(const PatchContext808&,const PatchParam808&,int);bool indPatchEdit50(PatchContext808&,const PatchParam808&,uint16_t);
int indPatchOwner50(const PatchContext808&);
bool indEntryValid50(const IndEntry50&);
struct Forecast50 {int32_t identity=0;uint8_t track=0,pattern=0,note=0;bool valid=false;};
EXTMEM Forecast50 forecast50[64];
void prepareForecast50(uint16_t,int64_t);int forecastValue50(uint8_t,uint16_t,uint16_t,int,bool&);void indClocksReset50(int=-1);

void indMarks50();
