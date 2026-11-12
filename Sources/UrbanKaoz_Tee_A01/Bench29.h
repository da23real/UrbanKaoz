#pragma once
FLASHMEM void benchPrepare29(unsigned tracks,unsigned notes,unsigned bars,unsigned blocks,unsigned tempo,unsigned routing){
 playing=internalClockRunning=false;pendingClockTicks=0;benchActive29=benchPrepared29=false;
 allNotesOff();pidiCancelDelays924();delayClear907(delayNodes907);pidiProject925();
 showGameScreen();quickView903=0;trackMenuActive=false;keyboardEnabled823=kbZoneViewR11=false;
 patternMode=grooveEditMode=remixMode=songMode=songEngineActive=false;selectedTrack=0;
 clearStepSelection();lockReset817();memset(linkMask12,0,sizeof(linkMask12));blockReset21();patch808Init();programReset939();
 for(auto&m:modUnits805)m={};mod805Invalidate();trackSolo818=0;
 for(unsigned t=0;t<16;++t){clearTrackPatternData(t);trackBars[t]=bars;trackMuted[t]=t>=tracks;trackPlayMode[t]=TRACK_PLAY_MULTI;trackLoopStartStep[t]=1;trackLoopEndStep[t]=bars*16;activePattern[t]=0;pendingPattern[t]=-1;trackRoll[t]={};trackPitchBend[t]=0;programDefault939[t]=0;
  for(unsigned b=0;b<3;++b)blocks21[t][b]={0,(uint8_t)(b<blocks?(routing==1?8:1+(t*3+b)%8):0),(uint8_t)((t+b)%16),0};
  if(t<tracks)for(unsigned step=0;step<bars*16;step+=16/notes){stepActive[t][step]=true;stepMidiNote[t][step]=48+t;stepLockPitch[t][step]=true;stepVel[t][step]=100;stepLockVel[t][step]=true;stepGateTicks[t][step]=24;stepLockGate[t][step]=true;}
 }
 resetGrooveBridges();invalidateStepTriggers();loopStartStep=1;loopEndStep=bars*16;transportTickAbsolute=measureTick=0;evaluateCurrentTickOnNextClock=true;bpm=tempo;updateInternalClockPeriod();pidiStateDirty931=true;pidiRoutesDirty931=0xffff;activeNoteRejects15=0;
 benchBlocks29=blocks;benchTracks29=tracks;benchNotes29=notes;benchPrepared29=true;
 Serial.printf("BENCH29,PREPARED,TRACKS=%u,NOTES_PER_BAR=%u,BARS=%u,BLOCKS=%u,BPM=%u,ROUTING=%u\n",tracks,notes,bars,blocks,tempo,routing);
}
FLASHMEM void benchStop29(){benchActive29=false;playing=internalClockRunning=false;pendingClockTicks=0;benchElapsed29=micros()-benchStart29;allNotesOff();pidiStateDirty931=true;}
#include "Bench30.h"
#include "Bench49.h"
FLASHMEM bool benchCommand29(const char*line){
 if(benchCommand49(line))return true;
 if(!strncmp(line,"B31OPT,",7)){unsigned bits;if(!playing&&sscanf(line+7,"%u",&bits)==1&&bits<=31){optim31=bits;++modEpoch805;lockEventInvalidate30();indexBuilds31=sharedHits31=sampleHits31=0;Serial.printf("BENCH31,OPT=%u\n",optim31);}else Serial.println("BENCH31,INVALID");return true;}
 if(!strcmp(line,"B31STAT")){Serial.printf("BENCH31,OPT=%u,INDEX=%lu,SHARED=%lu,SAMPLE=%lu\n",optim31,indexBuilds31,sharedHits31,sampleHits31);return true;}
 if(!strncmp(line,"B31MOD,",7)){unsigned n,rat;if(benchPrepared29&&!playing&&sscanf(line+7,"%u,%u",&n,&rat)==2&&n<=64&&rat>=1&&rat<=3072){benchEffects30(0);for(unsigned k=0;k<n;++k){auto&m=modUnits805[k];m.enabled=1;m.rate=1000;m.shape=4;modKinds931[k]=1;modExtra810[k].mode=2;modRat808[k]=rat;m.outputs[0]={MOD_CC,0,(uint8_t)(20+k),1,63,1,0,64};}mod805Invalidate();Serial.printf("BENCH31,MOD=%u,RAT=%u\n",n,rat);}else Serial.println("BENCH31,INVALID");return true;}

 if(!strncmp(line,"B30FX,",6)){unsigned effect;if(benchPrepared29&&!benchActive29&&sscanf(line+6,"%u",&effect)==1&&effect<=3){benchEffects30(effect);Serial.printf("BENCH30,FX=%u\n",effect);}else Serial.println("BENCH30,INVALID" );return true;}
 if(!strncmp(line,"B30OPT,",7)){blockTargeted30=line[7]!='0';++modEpoch805;Serial.printf("BENCH30,OPT=%u\n",blockTargeted30);return true;}
 if(!strncmp(line,"B29OPT,",7)){blockOptimize29=line[7]!='0';Serial.printf("BENCH29,OPT=%u\n",blockOptimize29);return true;}
 if(!strncmp(line,"B29PREP,",8)){unsigned t,n,b,k,tempo,route;if(sscanf(line+8,"%u,%u,%u,%u,%u,%u",&t,&n,&b,&k,&tempo,&route)==6&&t>=1&&t<=16&&(n==4||n==16)&&b>=1&&b<=64&&k>=1&&k<=3&&tempo>=40&&tempo<=300&&route<=1)benchPrepare29(t,n,b,k,tempo,route);else Serial.println("BENCH29,INVALID");return true;}
 if(!strncmp(line,"B29RUN,",7)){unsigned bars;if(!benchPrepared29||sscanf(line+7,"%u",&bars)!=1||bars<1||bars>64){Serial.println("BENCH29,INVALID");return true;}memset(benchOn29,0,sizeof(benchOn29));memset(benchOff29,0,sizeof(benchOff29));memset(benchTickHist29,0,sizeof(benchTickHist29));benchTickSum29=benchTickMax29=benchTicks29=benchQueuePeak29=0;resetStats830();memset(perf812,0,sizeof(perf812));perfEndValid812=false;perfEnabled812=true;profile830=true;transportMax821=transportBacklog821=0;benchTickLimit29=bars*1536;Serial.println("BENCH29,START");benchStart29=micros();transportTickAbsolute=measureTick=0;evaluateCurrentTickOnNextClock=true;pendingClockTicks=0;benchActive29=playing=internalClockRunning=true;pidiStateDirty931=true;return true;}
 if(!strcmp(line,"B29STOP")){benchStop29();benchPrepared29=false;profile830=false;perfEnabled812=false;Serial.println("BENCH29,STOP");return true;}
 if(!strcmp(line,"B29STAT")){if(benchActive29){Serial.println("BENCH29,RUNNING");return true;}Serial.printf("BENCH29,RESULT,TICKS=%lu,ELAPSED_US=%lu,TICK_MAX_US=%lu,TICK_SUM_US=%llu,QUEUE_PEAK=%lu,REJECTED=%lu\n",benchTicks29,benchElapsed29,benchTickMax29,benchTickSum29,benchQueuePeak29,activeNoteRejects15);for(unsigned p=0;p<8;++p)Serial.printf("BENCH29,PORT=%u,ON=%lu,OFF=%lu\n",p+1,benchOn29[p],benchOff29[p]);for(unsigned i=0;i<16;++i)Serial.printf("BENCH29,HIST_UPPER_US=%lu,N=%lu\n",1UL<<i,benchTickHist29[i]);status830();pidiStats827();return true;}
 return false;
}


