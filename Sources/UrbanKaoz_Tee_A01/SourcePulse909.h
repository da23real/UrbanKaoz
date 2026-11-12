#pragma once
#include <stdint.h>
// Only the source currently visible is monitored; no per-source/tick history.
struct SourcePulse909 {
  uint32_t began=0; uint8_t source=255; bool active=false;
  void watch(uint8_t key){if(source!=key){source=key;active=false;}}
  void trigger(uint8_t key,uint32_t now){if(source==key&&!active){began=now;active=true;}}
  bool dark(uint32_t now){if(active&&uint32_t(now-began)>=120)active=false;return active;}
};
