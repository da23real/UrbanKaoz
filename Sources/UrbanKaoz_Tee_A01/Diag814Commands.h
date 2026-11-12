#pragma once
static const char* const diagNames814[D814_COUNT]={"TRACK","CLEAR_SELECTION","RESTORE_VIEW","AUDITION","TRACK_MENU","NEO_TOTAL","NEO_TX","NEO_CATCHUP","TRANSPORT","LOOP"};
static void diagOff814(){
  bool refresh=diagNeo814!=0;diagActive814=false;diagNeo814=0;diagQueuedTrack814=-1;++diagGeneration814;
  if(refresh){neoDisplayDirty=true;lastNeoRefreshMs=millis()-NEO_MIN_REFRESH_MS;}
}
static void diagService814(){
  if(diagActive814&&(!Serial||(uint32_t)(millis()-diagLease814)>=120000))diagOff814();
}
// Exact decimal parser: no signs, whitespace, overflow or ignored suffixes.
static bool diagNumber814(const char*s,uint32_t maxValue,uint32_t&value){
  if(!*s)return false;value=0;
  while(*s){if(*s<'0'||*s>'9')return false;uint32_t d=*s++-'0';if(d>maxValue||value>(maxValue-d)/10)return false;value=value*10+d;}
  return true;
}
FLASHMEM bool diagCommand814(const char*line){
  if(strncmp(line,"DIAG",4))return false;
  if(!strcmp(line,"DIAGON")||!strcmp(line,"DIAGCLEAR"))inputReset815();
  if(!strcmp(line,"DIAGBUTTONS")){
    if(playing){Serial.println("INPUT815,ERROR,STOP_PLAYBACK_FIRST");return true;}
    Serial.printf("INPUT815,BEGIN,N=%u,OVERWRITTEN=%lu,MAX_SCAN_MS=%lu\n",inputUsed815,(unsigned long)inputOverwritten815,(unsigned long)inputMaxScan815);
    uint16_t first=(inputWrite815+128-inputUsed815)%128;
    for(uint16_t i=0;i<inputUsed815;++i){auto&e=inputEvents815[(first+i)%128];Serial.printf("INPUT815,EVENT,AT=%lu,SRC=%u,ID=%u,DOWN=%u,STAGE=%u,CTX=%u,US=%lu\n",(unsigned long)e.at,e.source,e.id,e.pressed,e.stage,e.context,(unsigned long)e.us);}
    Serial.println("INPUT815,END");return true;
  }
  if(!strcmp(line,"DIAGON")){diagOff814();diagReset814();diagLease814=millis();diagActive814=true;Serial.println("DIAG814,ON,LEASE_MS=120000");return true;}
  if(!strcmp(line,"DIAGOFF")){diagOff814();Serial.println("DIAG814,OFF,NORMAL_RESTORED");return true;}
  if(!strcmp(line,"DIAGSTAT")){
    Serial.printf("DIAG814,CAPTURE_FROZEN=%u\n",diagFrozen814);
    Serial.printf("DIAG814,STATE,ON=%u,NEO=%u,TRK=%u,QUEUED=%d,TICKS=%lu,EVENTS=%u,OVERWRITTEN=%lu,CPU_HZ=%lu\n",diagActive814,diagNeo814,selectedTrack+1,diagQueuedTrack814<0?-1:diagQueuedTrack814+1,(unsigned long)pendingClockTicks,diagUsed814,(unsigned long)diagOverwritten814,(unsigned long)F_CPU_ACTUAL);
    return true;
  }
  // Dump only stopped; OFF preserves captured data. No lease renewal by polling.
  if(!strcmp(line,"DIAGDUMP")){
    if(playing){Serial.println("DIAG814,ERROR,STOP_PLAYBACK_FIRST");return true;}
    ++diagGeneration814;
    Serial.println("DIAG814,DUMP,INCLUSIVE_TIMES,DWT_MODULO_32BIT");
    for(uint8_t i=0;i<D814_COUNT;++i){auto&s=diagStats814[i];Serial.printf("DIAG814,STAT,%s,N=%lu,MAX_US=%lu,MAX_MS=%lu,MAX_TICKS=%lu\n",diagNames814[i],(unsigned long)s.count,(unsigned long)s.maxUs,(unsigned long)s.maxMs,(unsigned long)s.maxTicks);}
    uint8_t first=(diagWrite814+64-diagUsed814)%64;
    for(uint8_t i=0;i<diagUsed814;++i){auto&e=diagEvents814[(first+i)%64];Serial.printf("DIAG814,EVENT,%s,AT_MS=%lu,US=%lu,MS=%lu,CYCLES=%lu,TICKS=%lu,TRK=%u,NEO=%u\n",diagNames814[e.kind],(unsigned long)e.at,(unsigned long)e.us,(unsigned long)e.ms,(unsigned long)e.cycles,(unsigned long)e.ticks,e.track+1,e.mode);}
    Serial.println("DIAG814,END");return true;
  }
  if(!diagActive814){Serial.println("DIAG814,ERROR,ENABLE_FIRST");return true;}
  if(!strcmp(line,"DIAGPING")){diagLease814=millis();Serial.println("DIAG814,PONG");return true;}
  if(!strcmp(line,"DIAGCLEAR")){diagReset814();Serial.println("DIAG814,CLEARED");return true;}
  if(!strncmp(line,"DIAGNEO,",8)){
    uint8_t mode;
    if(!strcmp(line+8,"NORMAL"))mode=0;else if(!strcmp(line+8,"CALC"))mode=1;else if(!strcmp(line+8,"FROZEN"))mode=2;
    else{Serial.println("DIAG814,ERROR,NEO_MODE");return true;}
    diagNeo814=mode;neoDisplayDirty=true;lastNeoRefreshMs=millis()-NEO_MIN_REFRESH_MS;
    Serial.printf("DIAG814,NEO=%u\n",mode);return true;
  }
  if(!strncmp(line,"DIAGTRK,",8)){
    uint32_t track;
    if(!diagNumber814(line+8,TRACK_COUNT,track)||track==0){Serial.println("DIAG814,ERROR,TRACK_RANGE");return true;}
    if(menuNavigationOnly()||patternMode||songMode){Serial.println("DIAG814,ERROR,HOME_TRACK_MODE_REQUIRED");return true;}
    if(diagQueuedTrack814>=0){Serial.println("DIAG814,ERROR,TRACK_PENDING");return true;}
    diagQueuedTrack814=track-1;Serial.printf("DIAG814,TRK,QUEUED=%lu\n",(unsigned long)track);return true;
  }
  Serial.println("DIAG814,ERROR,UNKNOWN_COMMAND");return true;
}
