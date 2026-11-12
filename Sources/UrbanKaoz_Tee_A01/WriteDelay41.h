#pragma once
// Offline rendering of the acyclic four-delay graph. Arithmetic follows
// Pidi VA01.33 MultiDelay925; echo notes retain their resolved output address.
struct WriteEcho41 {FineNoteEvent event;int32_t tick;uint8_t track,port,channel;};
static WriteEcho41* writeEcho41=nullptr;
static uint16_t writeEchoCount41=0;
static const char* writeError41=nullptr;
static uint32_t writeTickUs41=1;
static EXTMEM DelayValues907 writeDelayConfig41[16][4];
static uint32_t writeCutAt41[16];
FLASHMEM static int32_t writeGrow41(int32_t v,uint8_t curve,int32_t limit,uint16_t maximum=0){int64_t n=v<0?-(int64_t)v:v;int32_t factor=maximum?256+(maximum-1)*curve*256/100:expFactor14(curve);n=(n*factor+128)/256;n=min(n,(int64_t)limit);return v<0?-n:n;}
FLASHMEM static void writeEchoFree41(){extmem_free(writeEcho41);writeEcho41=nullptr;writeEchoCount41=0;}
FLASHMEM static bool writeDelaySeries41(uint8_t t,uint8_t slot,int64_t at,uint8_t pitch,uint8_t velocity,uint32_t gate,bool roll){
 const auto&d=writeDelayConfig41[t][slot];if(!d.enabled||!d.repeats||(roll&&!d.rolls))return true;
 uint8_t dest=d.output==16?t:d.output;auto route=blocks21[linkOwner12(dest,LINK_MOUT12)][d.block21];if(!route.port)return true;
 int32_t vel=(d.mode==2?velocity*clampInt(d.velocity,0,127)/127:clampInt((d.mode==1?0:velocity)+d.velocity,0,127))*256;
 int32_t acc=d.accel*256,scdo=(d.scdo==-128?0:d.scdo)*256,gd=d.gat08*256;int64_t spacing=(int64_t)d.length*writeTickUs41;
 int64_t due=at+(int64_t)(d.wait+d.length)*writeTickUs41;
 gate=constrain((int64_t)gate+(int64_t)d.gat08*writeTickUs41,(int64_t)1000,(int64_t)20000000);
 int original=velocity;bool legacy=d.mode==2&&!d.note&&(d.scdo==0||d.scdo==-128);
 for(uint8_t rep=0;rep<d.repeats;++rep){
  if(writeCutAt41[t]!=UINT32_MAX&&due>=(int64_t)writeCutAt41[t]*writeTickUs41)break;
  int v=legacy?(rep?original*clampInt(d.velocity,0,127)/127:vel/256):clampInt((vel+128)/256,0,127);
  int shift55=0;for(auto&a:d.targets55)if(a.kind==7)shift55+=delay55::offset(a,rep+1,d.note);uint8_t n=clampInt(pitch+shift55,0,127);
  if(v){
   if(writeEchoCount41>=LOCK_CAPACITY817/7){writeError41="WRITE DLY CAPACITY";return false;}
   auto&e=writeEcho41[writeEchoCount41++];e={};e.track=t;e.port=route.port;e.channel=route.channel;
   int32_t len=trackSequenceTicks(t);int64_t tick=(due+writeTickUs41/2)/writeTickUs41;
   // MOV remains live unless selected in WRITE; compensate it in logical time.
   tick-= (writeMask41&2)?0:defaultMov41[t][0];e.tick=(tick%len+len)%len;
   uint16_t st;int16_t micro;tickToStepAndMicro(t,e.tick,st,micro);
   e.event.active=true;e.event.step=st;e.event.logicalMicro=micro;e.event.note=n;e.event.vel=v;
   e.event.gate=constrain((gate+writeTickUs41/2)/writeTickUs41,1u,65535u);
   e.event.pxMask=PX_PITCH|PX_VEL|PX_GATE|PX_TIME|(63u<<PX_ROLL_SHIFT);e.event.roll={};
   for(uint8_t next=slot+1;next<4;++next)if(writeDelayConfig41[t][next].chain&(1u<<slot))if(!writeDelaySeries41(t,next,due,n,v,gate,false))return false;
  }
  gd=writeGrow41(gd,d.gex08,127*256,expDelayMaximum15);gate=constrain((int64_t)gate+(int64_t)gd*writeTickUs41/256,(int64_t)1000,(int64_t)20000000);
  original=v;if(legacy&&!v)break;
  spacing=constrain(spacing+(int64_t)acc*writeTickUs41/256,(int64_t)writeTickUs41,(int64_t)3072*writeTickUs41);due+=spacing;
  acc=writeGrow41(acc,d.axp,128*256,expDelayMaximum15);vel=clampInt(vel+scdo,0,127*256);scdo=writeGrow41(scdo,d.oxp,127*256,expDelayMaximum15);
 }
 return true;
}
FLASHMEM static bool writeDelayPrepare41(){
 writeEchoFree41();writeError41=nullptr;writeTickUs41=max(1u,(uint32_t)(tickDurationMs()*1000.0f));
 writeEcho41=(WriteEcho41*)extmem_malloc((LOCK_CAPACITY817/7)*sizeof(WriteEcho41));if(!writeEcho41){writeError41="WRITE MEMORY FULL";return false;}
 for(uint8_t t=0;t<16;++t)if(writeTracks41&(1u<<t)){
  for(uint8_t s=0;s<4;++s){writeDelayConfig41[t][s]=delayGet907(t*16+s*256);if(!delayPresent925(t,s)||!mixerOn938(t,1))writeDelayConfig41[t][s].enabled=0;}
  for(uint8_t i=0;i<4;++i)if(writeDelayConfig41[t][i].enabled)for(auto&a:writeDelayConfig41[t][i].targets55)if(a.kind!=255&&a.kind!=7){writeError41="WRITE DLY TARGET LIVE";return false;}
  // Delay's source is block 1, just as mod805Note in the live engine.
  BlockScope21 scope(0);
  for(uint16_t st=0;st<trackStepCount(t);++st)for(uint8_t voice=0;voice<noteCountAtStep(t,st);++voice){
   int32_t logical=lockTick817(t,st,voice);blockResolved21=blockAtNote21(t,0,activePattern[t],logical,noteAtVoice(t,st,voice));blockResolvedValid21=true;if(!blockResolved21.port)continue;
   int32_t len=trackSequenceTicks(t),at=((int64_t)st*96+audibleMicroTick(t,st,voice)+blockResolved21.tim)%len;if(at<0)at+=len;
   writeCutAt41[t]=UINT32_MAX;
   if(delayCut12&(1u<<linkOwner12(t,LINK_DLY12))){writeCutAt41[t]=at+len;for(uint16_t k=0;k<trackStepCount(t);++k)for(uint8_t v=0;v<noteCountAtStep(t,k);++v){int32_t next=((int64_t)k*96+audibleMicroTick(t,k,v))%len;if(next<0)next+=len;if(next<=at)next+=len;writeCutAt41[t]=min(writeCutAt41[t],(uint32_t)next);}}
   uint8_t pitch=blockPitch34(t,audibleNoteAtVoice(t,st,voice)),vel=audibleVel(t,st,voice);uint32_t gate=constrain((uint32_t)audibleGateTicks(t,st,voice)*writeTickUs41,1000u,20000000u);
   for(uint8_t s=0;s<4;++s)if(!s||writeDelayConfig41[t][s].source)if(!writeDelaySeries41(t,s,(int64_t)at*writeTickUs41,pitch,vel,gate,false))return false;
   auto r=audibleRollSettings(t,st,voice);auto x=rollExtra1000(t,activePattern[t],logical,noteAtVoice(t,st,voice));
   if(r.start&&(r.level||x.v[1])){
    int32_t relative=at-r.start,stop=at-rollStop09(r,x),spacing=clampInt(r.speed,1,96),acc=r.accel*256;
    for(uint16_t rep=0;rep<MAX_PREROLL_NOTES&&relative<stop;++rep){
     for(uint8_t side=0;side<2;++side){if((side==0&&rollDirection60(t,activePattern[t],logical,noteAtVoice(t,st,voice))==1)||(side==1&&rollDirection60(t,activePattern[t],logical,noteAtVoice(t,st,voice))==0))continue;
      int32_t tick=side?2*at-relative:relative;tick=(tick%len+len)%len;
      uint8_t rv=rollVelocity1000(r,x,vel,rep),rn=blockPitch34(t,rollPitch1000(x,audibleNoteAtVoice(t,st,voice),rep));
      if(rv)for(uint8_t s=0;s<4;++s)if(!s||writeDelayConfig41[t][s].source)if(!writeDelaySeries41(t,s,(int64_t)tick*writeTickUs41,rn,rv,gate,true))return false;
     }
     if(spacing>=stop-relative)break;relative+=spacing;
     rollAdvance60(spacing,acc,r.expo,rep);
    }
   }
  }
 }
 if(lockCount817+writeEchoCount41*7>LOCK_CAPACITY817){writeError41="WRITE P.LOCK CAPACITY";return false;}
 return true;
}
FLASHMEM static bool writeDelayRoom41(bool before=true){
 uint16_t needed[16]={};for(uint16_t i=0;i<writeEchoCount41;++i){auto&e=writeEcho41[i];++needed[e.track];
  for(uint16_t st=0;st<trackStepCount(e.track);++st)for(uint8_t v=0;v<noteCountAtStep(e.track,st);++v){int32_t tick=lockTick817(e.track,st,v);if(before){if(writeMask41&1)tick+=effectiveMicroTick(e.track,st,v);if(writeMask41&2)tick+=defaultMov41[e.track][0];int32_t len=trackSequenceTicks(e.track);tick=(tick%len+len)%len;}
   if(noteAtVoice(e.track,st,v)==e.event.note&&tick==e.tick){writeError41="WRITE DLY NOTE COLLISION";return false;}
   if(before&&(writeMask41&4)){
    auto r=configuredRollSettings(e.track,st,v);auto x=rollExtra1000(e.track,activePattern[e.track],lockTick817(e.track,st,v),noteAtVoice(e.track,st,v));
    if(r.start&&(r.level||x.v[1])){int32_t origin=st*96+audibleMicroTick(e.track,st,v)-((writeMask41&2)?0:defaultMov41[e.track][0]);
     int32_t relative=origin-r.start,stop=origin-rollStop09(r,x),spacing=clampInt(r.speed,1,96),acc=r.accel*256;
     for(uint16_t rep=0;rep<MAX_PREROLL_NOTES&&relative<stop;++rep){for(uint8_t side=0;side<2;++side){if((side==0&&rollDirection60(e.track,activePattern[e.track],lockTick817(e.track,st,v),noteAtVoice(e.track,st,v))==1)||(side==1&&rollDirection60(e.track,activePattern[e.track],lockTick817(e.track,st,v),noteAtVoice(e.track,st,v))==0))continue;
      if(e.tick==(side?2*origin-relative:relative)&&e.event.note==rollPitch1000(x,noteAtVoice(e.track,st,v),rep)){writeError41="WRITE DLY ROLL COLLISION";return false;}}
      if(spacing>=stop-relative)break;relative+=spacing;rollAdvance60(spacing,acc,r.expo,rep);
     }
    }
   }
  }
  for(uint16_t j=0;j<i;++j){auto&b=writeEcho41[j];if(b.track==e.track&&b.tick==e.tick&&b.event.note==e.event.note){writeError41="WRITE DLY ECHO COLLISION";return false;}}
 }
 for(uint8_t t=0;t<16;++t){uint16_t free=0;for(auto&e:fineNote[t])free+=!e.active;
  if(before&&(writeTracks41&(1u<<t))&&(writeMask41&7)){
   uint32_t reserve=0;for(uint16_t st=0;st<trackStepCount(t);++st)for(uint8_t v=0;v<noteCountAtStep(t,st);++v){++reserve;
    if(writeMask41&4){auto r=configuredRollSettings(t,st,v);if(r.start){auto x=rollExtra1000(t,activePattern[t],lockTick817(t,st,v),noteAtVoice(t,st,v));int32_t pos=-r.start,stop=-rollStop09(r,x),spacing=clampInt(r.speed,1,96),acc=r.accel*256;for(uint16_t rep=0;rep<MAX_PREROLL_NOTES&&pos<stop;++rep){reserve+=rollDirection60(t,activePattern[t],lockTick817(t,st,v),noteAtVoice(t,st,v))==2?2:1;if(spacing>=stop-pos)break;pos+=spacing;rollAdvance60(spacing,acc,r.expo,rep);}}}}
   free=reserve>=FINE_EVENTS_PER_TRACK?0:FINE_EVENTS_PER_TRACK-reserve;
  }
  if(needed[t]>free){writeError41="WRITE DLY NOTE CAPACITY";return false;}}
 return true;
}
FLASHMEM static void writeDelayCommit41(){
 // Use fine slots even on empty steps: never replace a mono note.
 for(uint16_t i=0;i<writeEchoCount41;++i){auto&e=writeEcho41[i];for(auto&f:fineNote[e.track])if(!f.active){f=e.event;break;}
  const uint8_t fields[7]={0,1,2,3,4,8,12};int values[7]={e.port,e.channel,0,0,0,0,0};
  for(uint8_t k=0;k<7;++k)noteLocks817[lockCount817++]={e.tick,values[k],e.track,fields[k],e.track,activePattern[e.track],e.event.note,LOCK_BLOCK21};
 }
 // Per-track mixer state preserves linked neighbours and every delay setting.
 for(uint8_t t=0;t<16;++t)if(writeTracks41&(1u<<t)){mixerOff938[1]|=1u<<t;for(uint8_t s=0;s<4;++s)delayChanged925(t,s,255,0);}
 ++modEpoch805;lockEventInvalidate30();invalidateStepTriggers();
}

