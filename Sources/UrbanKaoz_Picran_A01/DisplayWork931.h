#pragma once
// Bits 0..3: LCD1..LCD4; bit 4: EP. No value formatting here.
inline uint8_t displayWork931(const char*line){
 if(!strncmp(line,"FIELD,",6)){
  static const uint8_t panels[16]={0,0,0,0,1,1,1,1,2,2,3,3,3,2,3,3};
  unsigned field=atoi(line+6);return field<16?(uint8_t)(16|(1u<<panels[field])):0;
 }
 if(!strncmp(line,"MODLCD|",7)||!strncmp(line,"RMX910|",7)){
  unsigned panel=line[7]-'0';return panel<4?(uint8_t)(1u<<panel):0;
 }
 if(!strncmp(line,"UI",2)||!strncmp(line,"MSG|",4)||!strncmp(line,"SCROLL",6))return 16;
 return 31;
}
