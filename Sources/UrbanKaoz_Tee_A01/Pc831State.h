#pragma once
static int numericId917=-1;
void checks917();
void numericRead917(int id);void numericWrite917(int id,int value,uint32_t token);
// All masks are volatile session state, never saved to SD or EEPROM.
struct Pc831State {
  bool active,injecting,keyMask;
  uint32_t seen,lastSeq,telemetryAt,joyAt;
  uint64_t hcMask,hcDown,neoMask,neoDown;
  uint16_t kyMask,kyDown;
  uint8_t royMask,royDown,joyMask,panel;
  int16_t joy[6];
};
DMAMEM Pc831State pc831={};
uint8_t pcNeoDirty931=15;
bool pcCommand831(char*line);
void pcService831();
