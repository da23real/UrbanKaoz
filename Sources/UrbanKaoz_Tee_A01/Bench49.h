#pragma once
// Explicit synthetic fixtures, never invoked at boot or by normal UI actions.
// B49PREP is accepted only after the existing explicit B29PREP diagnostic.
FLASHMEM bool benchCommand49(const char*line){
 if(!strncmp(line,"B59FX,",6)){
  unsigned mode;if(!benchPrepared29||playing||sscanf(line+6,"%u",&mode)!=1||mode>3){Serial.println("BENCH59,INVALID");return true;}
  indReset50();lockReset817();for(auto&m:modUnits805)m={};
  for(uint8_t t=0;t<benchTracks29;++t){auto&m=modUnits805[t];m.enabled=mode!=0;m.rate=384;m.inputs=1u<<t;m.attack=24;m.decay=48;m.sustain=64;m.release=48;m.shape=4;modMeta806[t].tracks=1u<<t;modKinds931[t]=mode==2||(mode==3&&(t&1));modExtra810[t].mode=0;modRat808[t]=8;auto a=blocks21[t][0];m.outputs[0]={MOD_CC,a.channel,27,1,10,a.port,a.channel,40};ind50.set(t*16,modKinds931[t]?IND_LFO50:IND_ENV50,LOCK_DEPTH817,t,0,20);ind50.flags[t*16+1]=0;
   if(mode==3)for(unsigned s=0;s<trackBars[t]*16;s+=2){noteLocks817[lockCount817++]={(int32_t)s*96,20+(int)s%50,t,27,t,0,(uint8_t)(48+t),LOCK_CC817};}
  }playbackLocks828=true;indChanged50();Serial.printf("BENCH59,FX=%u,IND_P1_DEF_P2\n",mode);return true;
 }

 if(!strcmp(line,"B49PREP")){
   if(!benchPrepared29||playing){Serial.println("BENCH49,INVALID");return true;}
   autoReset46();selectButtonActive=selectionMode=shiftButtonActive=columnSelectionMode=false;patternChangeNext=true;
   for(uint8_t t=0;t<benchTracks29;++t){
     pattern805CaptureLive(t,patternWork906);patternWork906.bars=1+t%4;
     if(!storePattern906(t,1,patternWork906)){Serial.println("BENCH49,MEMORY_ERROR");return true;}
     patternDirty805[t][1]=true;patternBank[t][1].initialized=true;patternBank[t][1].bars=patternWork906.bars;patternBank[t][1].hasNotes=true;
   }
   Serial.printf("BENCH49,PREPARED,TRACKS=%u\n",benchTracks29);return true;
 }
 if(!strncmp(line,"B49QUEUE,",9)){
   unsigned pattern,mask;if(!benchPrepared29||sscanf(line+9,"%u,%u",&pattern,&mask)!=2||pattern>1||mask>65535){Serial.println("BENCH49,INVALID");return true;}
   for(uint8_t t=0;t<benchTracks29;++t)if(mask&(1u<<t))requestTrackPattern(t,pattern);
   Serial.printf("BENCH49,QUEUED,PATTERN=%u,MASK=%u\n",pattern,mask);return true;
 }
 if(!strcmp(line,"B49STAT")){
   for(int i=0;i<8;++i)Serial.printf("TRANS59,PHASE=%d,MAX_US=%lu\n",i,transitionMax59[i]);
   uint16_t active=0,waiting=0,ready=0;for(uint8_t t=0;t<16;++t){if(activePattern[t]==1)active|=1u<<t;if(pendingPattern[t]>=0)waiting|=1u<<t;if(grooveBridgeReady[t])ready|=1u<<t;}
   Serial.printf("BENCH49,STATE,ACTIVE_P2=%u,WAITING=%u,BRIDGES=%u\n",active,waiting,ready);return true;
 }
 return false;
}
