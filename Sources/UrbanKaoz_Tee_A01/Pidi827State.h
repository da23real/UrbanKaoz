#pragma once
#include "Remote54State.h"
uint16_t pidiRoutesDirty931=0xffff;bool pidiStateDirty931=true;
void pidiManual931();
void pidiStopEdge931(bool);void pidiStopService931();
#include "PidiWire827.h"
bool rollMod827[16]={};
EXTMEM int64_t rollModAt827[16];
EXTMEM uint32_t rollModGate827[16];
void pidiInit827();void pidiService827();void pidiMidi827(uint8_t,uint8_t,uint8_t);
void pidiCancel827();void pidiReload827();void pidiStats827();
void rollTriggerMod827(uint8_t track,uint32_t gateUs);

bool pidiPacket827(uint8_t,const void*,unsigned);

static bool analyzerCommand28(const char*);
