#pragma once
// Main-loop wall times include interrupt work. No printing or allocation while sampling.
struct PerfEntry812 {uint32_t maximum,count,over10ms,at;uint64_t total;uint16_t context;};
static PerfEntry812 perf812[32]={};
static const char*perfNames812[32]={"LOOP","KY_A","J1_A","USB","Y5_SONG","INDICATORS","TRANSPORT_BUTTONS","CLIPBOARD","TRANSPORT","NOTE_OFF","PATTERN_BLINK","KY_B","KY_HELP","J1_B","J1_DEBUG","DISPLAY_INPUT","NEO_INPUT","NEO_HOLD","ROY","SETTINGS_RETRY","VIEW_AUDIT","NEO_OUTPUT","LCD_VALUE","LCD_BPM","MOD","PATCH_UI","MIDI_DISPLAY","NAME_SYNC","AUTOSAVE"};
static bool perfEnabled812=false,perfSkip812=false;
static uint16_t perfContext812=0;
static uint32_t perfStart812=0;
static inline void perfAdd812(uint8_t id,uint32_t elapsed){
  if(!perfEnabled812||perfSkip812)return;auto&e=perf812[id];++e.count;e.total+=elapsed;if(elapsed>=10000)++e.over10ms;
  if(elapsed>e.maximum){e.maximum=elapsed;e.at=millis();e.context=perfContext812;}
}
static uint32_t perfEndAt812=0;static bool perfEndValid812=false;
static inline void perfBegin812(uint16_t context){if(!perfEnabled812)return;perfSkip812=false;perfContext812=context;perfStart812=micros();if(perfEndValid812)perfAdd812(29,(uint32_t)(perfStart812-perfEndAt812));}
static inline void perfEnd812(){if(!perfEnabled812)return;perfAdd812(0,(uint32_t)(micros()-perfStart812));perfEndAt812=micros();perfEndValid812=!perfSkip812;}
#define PERF812(id,statement) do{if(perfEnabled812){uint32_t start812=micros();statement;perfAdd812(id,(uint32_t)(micros()-start812));}else{statement;}}while(0)
FLASHMEM bool perfCommand812(const char*line){
  if(strcmp(line,"PERFSTAT")&&strcmp(line,"PERFCLEAR")&&strcmp(line,"PERFON")&&strcmp(line,"PERFOFF"))return false;
  perfSkip812=true; // Do not attribute this diagnostic's own printing to a musical stall.
  if(!strcmp(line,"PERFCLEAR")){memset(perf812,0,sizeof(perf812));Serial.println("PERF812,CLEARED");return true;}
  if(!strcmp(line,"PERFON")||!strcmp(line,"PERFOFF")){perfEnabled812=!strcmp(line,"PERFON");Serial.printf("PERF812,ENABLED=%u\n",perfEnabled812);return true;}
  Serial.printf("PERF812,BEGIN,ENABLED=%u,MS=%lu\n",perfEnabled812,(unsigned long)millis());
  for(uint8_t i=0;i<30;++i){const auto&e=perf812[i];Serial.printf("PERF812,%s,MAX_US=%lu,AVG_US=%lu,N=%lu,GE10MS=%lu,AT_MS=%lu,CTX=%u\n",i==29?"OUTSIDE_LOOP":perfNames812[i],(unsigned long)e.maximum,(unsigned long)(e.count?e.total/e.count:0),(unsigned long)e.count,(unsigned long)e.over10ms,(unsigned long)e.at,e.context);}
  Serial.println("PERF812,END");return true;
}
