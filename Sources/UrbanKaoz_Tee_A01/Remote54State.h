#pragma once
#include "ModWire54.h"
bool remoteEnabled54=true;
bool remoteOwn54(uint16_t);void remotePrepare54(int64_t);void remoteInvalidate54();
bool remoteAddress54(uint8_t,uint8_t,uint8_t,uint16_t);
bool remoteBase54(uint8_t,uint8_t,uint16_t,int,uint8_t,bool=false,bool=false);
void remoteLock54(uint8_t,uint8_t,uint16_t,int);
uint32_t remoteFallback54=0,remoteCapacity54=0;uint8_t remoteNodes54=0;
