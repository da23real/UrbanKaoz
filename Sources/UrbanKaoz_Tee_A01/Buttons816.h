#pragma once
struct Button816 {
 bool raw=false,stable=false;uint32_t changed=0;
 bool sample(bool value,uint32_t now){if(value==raw)return false;raw=value;changed=now;return true;}
 bool poll(uint32_t now,uint16_t delay){if(raw==stable||(uint32_t)(now-changed)<delay)return false;stable=raw;return true;}
};
static Button816 ky23Buttons816[2];
static uint16_t ky23Debounce816=15;
