#pragma once
extern uint16_t modEnabledCount805;
// Short-lived agenda: no input handler runs within a transport slice. Rebuild
// after every return to callers, avoiding stale notes after edits/record/undo.
// Bits name candidate steps, not cached note values: the original emit path
// still owns voice order, cycle guards, plocks, gates and bridge suppression.
static constexpr uint8_t AGENDA_HORIZON822=16;
static constexpr uint16_t AGENDA_WORDS822=(STEP_COUNT+31)/32;
EXTMEM uint32_t agendaSteps822[TRACK_COUNT][AGENDA_HORIZON822][AGENDA_WORDS822];
static uint32_t agendaStart822[TRACK_COUNT],agendaLength822[TRACK_COUNT];
static uint32_t agendaEpoch822[TRACK_COUNT];
static uint16_t agendaValid822=0;
static bool agendaActive822=false;
static uint32_t agendaBuilds822=0,agendaEmpty822=0,agendaFallback822=0;
static uint32_t agendaBuildUs822=0;

FLASHMEM void agendaBegin822(){agendaValid822=0;agendaActive822=true;agendaBuildUs822=0;++agendaSlice830;}
FLASHMEM void agendaEnd822(){agendaActive822=false;}
FLASHMEM void agendaMark822(uint8_t track,uint16_t step,int32_t tick){
  const int32_t length=agendaLength822[track];
  tick%=length;if(tick<0)tick+=length;
  uint32_t offset=((uint32_t)tick+length-agendaStart822[track])%length;
  if(offset<AGENDA_HORIZON822)
    agendaSteps822[track][offset][step/32]|=1UL<<(step%32);
}
FLASHMEM void agendaVoice822(uint8_t track,uint16_t step,uint8_t voice){
  int32_t target=(int32_t)step*TICKS_PER_STEP+audibleMicroTick(track,step,voice);
  agendaMark822(track,step,target);
  TrackRollSettings roll=audibleRollSettings(track,step,voice);
  if(!roll.start||(!roll.level&&!rollExtra1000(track,activePattern[track],(int32_t)step*96+logicalMicroTick(track,step,voice),noteAtVoice(track,step,voice)).v[1]))return;
  const int32_t origin38=target;int32_t tick=target-roll.start;target-=rollStop09(roll,rollExtra1000(track,activePattern[track],(int32_t)step*96+logicalMicroTick(track,step,voice),noteAtVoice(track,step,voice)));int32_t spacing=clampInt(roll.speed,1,TICKS_PER_STEP);
  int32_t acceleration=(int32_t)roll.accel*256;
  for(uint16_t repeat=0;repeat<MAX_PREROLL_NOTES&&tick<target;++repeat){
    if(rollDirection60(track,activePattern[track],lockTick817(track,step,voice),noteAtVoice(track,step,voice))!=1)agendaMark822(track,step,tick);if(rollDirection60(track,activePattern[track],lockTick817(track,step,voice),noteAtVoice(track,step,voice))!=0)agendaMark822(track,step,2*origin38-tick);
    if(spacing>=target-tick)break;
    tick+=spacing;
    rollAdvance60(spacing,acceleration,roll.expo,repeat);
  }
}
FLASHMEM const uint32_t* agendaDue822(uint8_t track,uint32_t local){
  // Timing modulation is sampled at the current tick (including wall-time
  // Free LFOs). Do not predict it using a frozen value. Lists/Song retain
  // their occurrence-aware paths. Bridges may change patterns mid-slice.
  if(!agendaActive822||(optimize830?timingMod830(track):modEnabledCount805!=0)||pendingPattern[track]>=0||grooveBridgeReady[track]){
    ++agendaFallback822;++agendaFallback830;return nullptr;
  }
  ++agendaEligible830;
  uint32_t length=trackSequenceTicks(track);
  if(!length)return nullptr;
  uint32_t offset=(local+length-agendaStart822[track])%length;
  if(!(agendaValid822&(1u<<track))||agendaLength822[track]!=length||
     agendaEpoch822[track]!=modEpoch805||offset>=AGENDA_HORIZON822){
    Scope830 measured(P830_AGENDA);uint32_t began=micros();
    agendaStart822[track]=local;agendaLength822[track]=length;agendaEpoch822[track]=modEpoch805;
    memset(agendaSteps822[track],0,sizeof(agendaSteps822[track]));
    uint16_t count=trackStepCount(track);
    memset(fineHead821,255,count);
    for(int fine=FINE_EVENTS_PER_TRACK-1;fine>=0;--fine){
      const auto &event=fineNote[track][fine];
      if(!event.active||event.step>=count)continue;
      fineNext821[fine]=fineHead821[event.step];fineHead821[event.step]=fine;
    }
    agendaBuilding830=optimize830;
    for(uint16_t step=0;step<count;++step){
      if(stepActive[track][step])agendaVoice822(track,step,0);
      uint8_t voice=baseVoiceCountAtStep(track,step);
      for(uint8_t fine=fineHead821[step];fine!=255;fine=fineNext821[fine])
        agendaVoice822(track,step,voice++);
    }
    agendaBuilding830=false;
    agendaValid822|=1u<<track;++agendaBuilds822;offset=0;
    agendaBuildUs822+=(uint32_t)(micros()-began);
  }
  const uint32_t* due=agendaSteps822[track][offset];
  bool any=false;for(uint16_t word=0;word<AGENDA_WORDS822;++word)any|=due[word]!=0;
  if(!any)++agendaEmpty822;
  return due;
}
FLASHMEM void agendaStatus822(){
  Serial.printf("AGENDA822,BUILDS=%lu,EMPTY=%lu,FALLBACK=%lu\n",agendaBuilds822,agendaEmpty822,agendaFallback822);
}
FLASHMEM uint16_t agendaNext822(const uint32_t* due,uint16_t from,uint16_t count){
  if(!due)return from;
  while(from<count){
    uint16_t word=from/32;
    uint32_t bits=due[word]&(~0UL<<(from%32));
    if(bits)return min(count,(uint16_t)(word*32+__builtin_ctz(bits)));
    from=(word+1)*32;
  }
  return count;
}
