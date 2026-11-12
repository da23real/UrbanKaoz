#pragma once
#include <stdint.h>
// Shared scale: 0%=x1; 100%=the configured maximum (default x10).
static uint16_t expMaximum14=10;
static uint16_t expDelayMaximum15=10,expEnvMaximum15=10;
static uint8_t expDefault15[3]={33,33,33};
static uint8_t expFamily15=0;
inline int32_t expFactor14(int percent){if(percent<0)percent=-percent;if(percent>100)percent=100;return 256+(int32_t)(expMaximum14-1)*percent*256/100;}
inline float expPower14(int percent,uint16_t maximum=0){if(percent<0)percent=-percent;if(percent>100)percent=100;return 1.0f+((maximum?maximum:expMaximum14)-1)*percent/100.0f;}
