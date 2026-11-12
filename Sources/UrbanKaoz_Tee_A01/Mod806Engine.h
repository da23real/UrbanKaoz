#pragma once
#include "Envelope805.h"
#include "ManualMod06.h"

bool mod805OwnerPlaying(uint8_t t,uint8_t p,int64_t at);
struct ModSourceCache805 { int64_t begin, next; uint32_t gate, epoch; int64_t validFrom, validUntil;int32_t identity=0;uint8_t pattern=255,note=255; };
EXTMEM ModSourceCache805 modSourceCache805[16][8];
uint8_t modSourceCacheCursor805[16] = {};
uint32_t modLastService805 = 0;
EXTMEM int16_t modLastCC805[128][129];
EXTMEM ModMean806 modMidiMix806[128][129];
EXTMEM uint16_t modMidiAddresses810[128*129];uint16_t modMidiAddressCount810=0;
EXTMEM int16_t modBase19[128][129];
FLASHMEM void modMidiPort19(uint8_t c,uint16_t o,float value,uint8_t mask,int base){for(uint8_t port=0;port<8;++port)if(mask&(1u<<port)){uint8_t address=port*16+c;auto&mix=modMidiMix806[address][o];if(!mix.count){modMidiAddresses810[modMidiAddressCount810++]=address*129+o;modBase19[address][o]=o<128?autoMidiBase46(port,c,o,base):base;}mix.add(value);}}
FLASHMEM void modMidiAdd810(uint8_t c,uint16_t o,float value){int base=0;if(o<128)patch808Base(c,o,base);modMidiPort19(c,o,value,channelMask902(c),base);}
void modCancelPort19(uint8_t,uint16_t);

EXTMEM bool modMidiWas806[128][129];
EXTMEM uint16_t modMidiPrevious830[128*129];
static uint16_t modMidiPreviousCount830=0;
EXTMEM int64_t modMidiBegin806[16];
EXTMEM uint32_t modMidiGate806[16];
EXTMEM uint8_t modMidiNote806[16];
EXTMEM bool modMidiHeld806[16];
FLASHMEM ModUnit805 &mod806Unit(uint16_t key) {
  return modUnits805[key];
}
FLASHMEM bool mod806Playing(uint16_t key,int64_t at) {
  if(!songEngineActive) {
    for(uint8_t t=0;t<16;++t)if(!trackSilent818(t)&&modAssociated12(key,t))return true;
  } else for(const auto &c:songClips)
    if(c.used&&!trackSilent818(c.track)&&modAssociated12(key,c.track)&&
       at>=(int64_t)c.position*96+(modKind931(key)?0:min(0,indBase50(c.track,c.pattern,LOCK_MOD817,key,0,holdBase50(key,modUnits805[key]))))&&at<(int64_t)(c.position+c.length)*96)return true;
  return false;
}
FLASHMEM void mod806DetachPattern(uint8_t t,uint8_t p) {
  // Pattern deletion does not detach project-wide track modulations.
  autoClearPattern46(t,p);ind50.useDef(t*16+p);indChanged50();lockClearPattern817(t,p);mod805Invalidate();
}
FLASHMEM void mod806CopyPattern(uint8_t ft,uint8_t fp,uint8_t tt,uint8_t tp) {
// Pattern copy does not modify track modulation ownership.
  unsigned add50=0,remove50=0;for(auto&e:ind50.entries)if(e.state==1){add50+=e.owner==ft*16+fp;remove50+=e.owner==tt*16+tp;}if(ind50.count-remove50+add50>IndStore50::CAPACITY){DISPLAY_SERIAL.println("MSG|IND COPY CAPACITY");return;}
  if(!autoCopyPattern46(ft,fp,tt,tp)){DISPLAY_SERIAL.println("MSG|AUTO COPY CAPACITY");return;}
  if(!delayCopy907(ft*16+fp,tt*16+tp)){DISPLAY_SERIAL.println("MSG|DELAY MEMORY FULL");return;}lockCopyPattern817(ft,fp,tt,tp);
  if(!ind50.copy(ft*16+fp,tt*16+tp)){DISPLAY_SERIAL.println("MSG|IND COPY CAPACITY");return;}
  patch808Copy(ft,fp,tt,tp);indChanged50();
}

EXTMEM uint16_t modEnabled805[64];
uint16_t modEnabledCount805 = 0;
struct ModValueCache805 { int64_t at; uint32_t epoch; float value;int8_t context; };
EXTMEM ModValueCache805 modValues805[64][8];
FLASHMEM void mod805Invalidate() {
  mixerValid930=0;
  graphDirty810=true;
  if (++modEpoch805 == 0) ++modEpoch805;
  cvEnabled46=false;for(const auto&m:modUnits805)if(m.enabled)for(const auto&r:m.outputs)if(r.enabled&&r.kind==MOD_CV46)cvEnabled46=true;
  modEnabledCount805 = 0;ioMask38=0;for(uint16_t k=0;k<64;++k)if(modUnits805[k].enabled&&ioKey38(k))ioMask38|=1ULL<<k;
  for(uint16_t key=0;key<64;++key)if(playbackMod828(key)&&modUnits805[key].enabled)modEnabled805[modEnabledCount805++]=key;
}
FLASHMEM void mod805Init() {ioVirtualReset53();autoReset46();memset(ioSeen38,0,sizeof(ioSeen38));memset(ioMidiStart38,0,sizeof(ioMidiStart38));memset(trackNames07,0,sizeof(trackNames07));memset(modNames07,0,sizeof(modNames07));memset(modPresetDirty07,0,sizeof(modPresetDirty07));memset(ccEnv941,0,sizeof(ccEnv941));memset(ccSeen941,0,sizeof(ccSeen941));
  modMidiPreviousCount830=0;
  memset(rollMod827,0,sizeof(rollMod827));memset(rollModAt827,255,sizeof(rollModAt827));
  rollReset1000();lockReset817();realtimeInit810();
  memset(modMeta806,0,sizeof(modMeta806));
  memset(modMidiWas806,0,sizeof(modMidiWas806));memset(modMidiMix806,0,sizeof(modMidiMix806));modMidiAddressCount810=0;ioVirtualClear53();
  memset(modMidiHeld806,0,sizeof(modMidiHeld806));
  for(auto&s:manualMod06)s=ManualMod06{};manualModMask06=0;
  for(auto &begin:modMidiBegin806)begin=INT64_MIN/2;
  modEnabledCount805=0;memset(virtualLinks805,0,sizeof(virtualLinks805));
  delayReset925();pidiCancelDelays924();
  memset(modUnits805,0,sizeof(modUnits805));memset(modKinds931,0,sizeof(modKinds931));for(auto&m:modUnits805){m.attack=1;m.decay=10;m.sustain=80;m.release=20;m.rate=384;} memset(delays805,0,sizeof(delays805));
  memset(modSourceCache805,0,sizeof(modSourceCache805)); 
  memset(modLastCC805,255,sizeof(modLastCC805));
  memset(modValues805,0,sizeof(modValues805));
  for (uint8_t t=0;t<16;++t) for(uint8_t p=0;p<16;++p) {

    delays805[t][p]={192,3,85,0};delayExtra817[t][p]={0,16,1};
  }
}
FLASHMEM bool mod805OwnerPlaying(uint8_t t,uint8_t p,int64_t at) {
  if (trackSilent818(t)) return false;
  if (!songEngineActive) return p==activePattern[t];
  for(const auto &c:songClips) if(c.used&&c.track==t&&c.pattern==p&&at>=(int64_t)c.position*96&&at<(int64_t)(c.position+c.length)*96) return true;
  return false;
}
FLASHMEM bool mod805OwnerPresent(uint8_t t,uint8_t p) {
  // Track-wide Delay/modulation objects are not content of every pattern.
  return false;
}
FLASHMEM ModSourceCache805 mod805Source(uint8_t t,int64_t at) {
  for(const auto &c:modSourceCache805[t]) if(c.epoch==modEpoch805&&at>=c.validFrom&&at<c.validUntil){++sourceHits830;return c;}
  Scope830 measured(P830_SOURCE);++sourceMisses830;
  ModSourceCache805 result={INT64_MIN/2,INT64_MAX/2,0,modEpoch805,at,INT64_MAX/2};
  if(upcomingSource50(t,at,result)){result.validUntil=result.next;modSourceCache805[t][modSourceCacheCursor805[t]++%8]=result;return result;}
  if(modSourceAudible12(t)) {
    if(songEngineActive) {
      for(const auto &c:songClips) if(c.used&&c.track==t) {
        if(c.pattern==activePattern[t]) {
          for(uint16_t step=c.offset;step<c.offset+c.length&&step<trackStepCount(t);++step){uint8_t voices=optimize830?noteCountAtStep(t,step):0;for(uint8_t voice=0;voice<(optimize830?voices:noteCountAtStep(t,step));++voice){
            ++sourceVoices830;
            BlockScope21 sourceScope42(0);blockResolved21=blockAtNote21(t,0,c.pattern,step*96+logicalMicroTick(t,step,voice),noteAtVoice(t,step,voice));blockResolvedValid21=true;
            int32_t onset=defaultMov41[t][0]+(int32_t)step*96+logicalMicroTick(t,step,voice)+pendingEditedValue(t,step,voice,EDIT_TIME,effectiveMicroTick(t,step,voice));
            uint32_t gate=pendingEditedValue(t,step,voice,EDIT_GATE,effectiveGateTicks(t,step,voice));int note=0,velocity=1;int16_t bend=0;songApplyGroove(t,c.pattern,step*96,onset,note,velocity,gate,bend);
            int64_t tick=((int64_t)c.position-c.offset)*96+onset;
            if(tick<=at&&tick>=result.begin){result.gate=tick==result.begin?max(result.gate,gate):gate;result.begin=tick;result.identity=step*96+logicalMicroTick(t,step,voice);result.pattern=c.pattern;result.note=noteAtVoice(t,step,voice);}
            if(tick>at&&tick<result.next)result.next=tick;
          }}
          continue;
        }
        const auto &cache=songCache[t][c.pattern]; if(!cache.valid) continue;
        for(uint32_t i=0;i<cache.count;++i) {
          ++sourceVoices830;const auto &n=songNotes[cache.first+i]; if(n.logical<(int32_t)c.offset*96||n.logical>=(int32_t)(c.offset+c.length)*96) continue;
          BlockScope21 sourceScope42(0);blockResolved21=blockAtNote21(t,0,c.pattern,songLockTick817[cache.first+i],n.note);blockResolvedValid21=true;
          int32_t onset=n.tick;uint32_t gate=n.gate;int note=0,velocity=1;int16_t bend=0;songApplyGroove(t,c.pattern,n.logical,onset,note,velocity,gate,bend);
          int64_t tick=((int64_t)c.position-c.offset)*96+onset;
          if(tick<=at&&tick>=result.begin) { result.gate=tick==result.begin?max(result.gate,gate):gate;result.begin=tick;result.identity=songLockTick817[cache.first+i];result.pattern=c.pattern;result.note=n.note; }
          if(tick>at&&tick<result.next) result.next=tick;
        }
      }
      uint32_t lo=0,hi=zlCount;while(lo<hi){uint32_t mid=(lo+hi)/2;if((int64_t)zlEvents[mid].tick<=at)lo=mid+1;else hi=mid;}
      for(uint32_t i=lo;i>0;){const auto&e=zlEvents[--i];if((int64_t)e.tick<result.begin)break;if(e.track!=t||e.kind)continue;
        if((int64_t)e.tick>=result.begin){result.gate=(int64_t)e.tick==result.begin?max(result.gate,e.gate):e.gate;result.begin=e.tick;result.pattern=254;result.identity=e.tick;result.note=255;}}
      for(uint32_t i=lo;i<zlCount;++i){const auto&e=zlEvents[i];if((int64_t)e.tick>=result.next)break;if(e.track!=t||e.kind)continue;result.next=e.tick;break;}

    } else {
      uint32_t length=trackSequenceTicks(t);
      if(length) {
        uint32_t master=bridgeMasterTickAt((uint64_t)max((int64_t)0,at));
        uint32_t local=(master+length-grooveTrackPhase[t]%length)%length;
        int64_t origin=at-local;
        for(uint16_t s=0;s<trackStepCount(t);++s){uint8_t voices=optimize830?noteCountAtStep(t,s):0;for(uint8_t v=0;v<(optimize830?voices:noteCountAtStep(t,s));++v) {
          ++sourceVoices830;
          int32_t offset=clampInt(logicalMicroTick(t,s,v)+pendingEditedValue(t,s,v,EDIT_TIME,effectiveMicroTick(t,s,v)),-96,96);
          int baseGate=pendingEditedValue(t,s,v,EDIT_GATE,effectiveGateTicks(t,s,v));
          auto address42=blockAtNote21(t,0,activePattern[t],(int32_t)s*96+logicalMicroTick(t,s,v),noteAtVoice(t,s,v));
          int32_t placed42=((int64_t)s*96+defaultMov41[t][0])%length;if(placed42<0)placed42+=length;
          for(int8_t cycle=-1;cycle<=1;++cycle){int64_t head=origin+placed42+(int64_t)cycle*length;if(head<0)continue;
            uint8_t sheet=grooveSheet40(t,activePattern[t],address42.groove,head>=grooveListOrigin[t]?(head-grooveListOrigin[t])/1536:0);
            int shifted=offset+address42.tim,gate=baseGate;if(!grooveBypass23&&sheet>0&&sheet<=GROOVE_COUNT){shifted+=grooveTime16(sheet,(int32_t)s*96+offset);gate=(int64_t)gate*grooveGate51(sheet,s)/100;}
            int64_t tick=head+shifted;if(tick<0)continue;
            if(tick<=at&&tick>=result.begin){uint32_t value=clampInt(gate,GATE_TICK_MIN,GATE_TICK_MAX);result.gate=tick==result.begin?max(result.gate,value):value;result.begin=tick;result.identity=s*96+logicalMicroTick(t,s,v);result.pattern=activePattern[t];result.note=noteAtVoice(t,s,v);}
            if(tick>at&&tick<result.next)result.next=tick;
          }
        }}
      }
    }
  }
  // The next note in the full track is not necessarily the next event in the
  // played loop. Re-evaluate at every master/track boundary, even in silence.
  // Keep note time separate from cache validity: look-ahead queries may arrive
  // out of order and must never reuse a result computed in a later loop.
  result.validUntil=result.next;
  if(!songEngineActive&&at>=0)
    result.validUntil=min(result.validUntil,(int64_t)nextBridgeBoundary(t,(uint64_t)at));
  modSourceCache805[t][modSourceCacheCursor805[t]++%8]=result;return result;
}
FLASHMEM float mod805Value(const ModUnit805 &m,uint8_t type,int64_t at,uint16_t key) {
  if(autoRenderTrack46>=0)return writeValue46(m,type,at,key);
  if(!playbackMod828(key))return 0;
  if(!type&&ccEnv941[key].enabled)return ccEnvelope07(m,key);
  // Phase origin and amplitude-envelope trigger are independent. NOTE envelope
  // needs the last note even when the oscillator is Free/Pattern/Bar/Beat.
  if(!(type&&lfoEnvMode36(m)==2)&&((type&&modMode810(key)==2)||(playing&&(modMode810(key)==1||modMode810(key)==3||modMode810(key)==4))))return modValue810(m,type,at,key,-1,0);
  int64_t latest=INT64_MIN/2;uint32_t gate=0;
  if(modMode810(key)==6&&!(type&&lfoEnvMode36(m)==2)){for(uint8_t t=0;t<16;++t)if(m.inputs&(1u<<t)){auto&w=rollWindows44[t];if(w.begin>=0&&w.begin<=at&&w.begin>latest){latest=w.begin;gate=(uint32_t)max((int64_t)0,w.end-w.begin);}}return modValue810(m,type,at,key,latest,gate);}
  for(uint8_t t=0;t<16;++t) if((m.inputs&(1u<<t))) { auto c=(playing&&m.roll58!=2)?mod805Source(t,at-(type?0:min(0,holdValue50(key,m)))):ModSourceCache805{INT64_MIN/2,INT64_MAX/2,0,0,0,0};if(m.roll58&&rollModAt827[t]>=0&&rollModAt827[t]<=at&&rollModAt827[t]>c.begin){c.begin=rollModAt827[t];c.gate=rollModGate827[t];}if(c.begin>latest){latest=c.begin;gate=c.gate;}else if(c.begin==latest)gate=max(gate,c.gate); }
  for(uint8_t c=0;c<16;++c)if(mod808EffectiveMeta(key).midiInputs&(1u<<c)) {
    int64_t begin=modMidiBegin806[c];
    uint32_t length=modMidiHeld806[c]?(uint32_t)max((int64_t)0,at-begin)+1:modMidiGate806[c];
    if(begin>latest){latest=begin;gate=length;}else if(begin==latest)gate=max(gate,length);
  }
  uint32_t manualAge=0,manualGate=0;
  if(manualModSource06(key,manualAge,manualGate)&&(latest<0||(int64_t)manualAge<=at-latest)){
    uint16_t rat=max(1,lockValue817(LOCK_RAT817,key,0,modRat808[key]));
    // Use a local origin so stopped transport (tick zero) remains a valid trigger.
    return modValue810(m,type,manualAge-manualAge%rat,key,0,manualGate,true);
  }
  return modValue810(m,type,at,key,latest,gate);
}
FLASHMEM float mod805UnitValue(uint16_t key,int64_t at) {
  if(autoRenderTrack46>=0){uint16_t rat=max((uint16_t)1,modRat808[key]);if(modMode810(key)!=2)at-=at%rat;return writeValue46(mod808EffectiveUnit(key),modKind931(key),at,key);}
  if(!playbackMod828(key))return 0;
  if(ccActive941(key)||manualModActive06(key))return mod805Value(mod808EffectiveUnit(key),modKind931(key),at,key);
  // Free is evaluated against wall time, independently of stopped transport.
  if(modKind931(key)&&modMode810(key)==2)return mod805Value(mod808EffectiveUnit(key),1,at,key);
  if(!modKind931(key)&&holdValue50(key,mod806Unit(key))<0)return mod805Value(mod808EffectiveUnit(key),0,at,key);
  // Hold each sampled value for RAT ticks; the curve's cycle length is unchanged.
  uint16_t rat=max(1,lockValue817(LOCK_RAT817,key,0,modRat808[key]));at=max((int64_t)0,at);int64_t origin41=0;uint8_t mode41=modMode810(key);if(modKind931(key)&&(mode41==3||mode41==4)){int64_t period41=(mode41==3?1536LL:384LL)*max((uint16_t)1,mod808EffectiveUnit(key).hold);origin41=at-at%period41;}at-= (at-origin41)%rat;
  uint8_t slot=at%96?0:1+(at/96)%7;auto &c=modValues805[key][slot];
  if(c.epoch==modEpoch805&&c.at==at&&c.context==indContext50)return c.value;
  c.at=at;c.epoch=modEpoch805;c.context=indContext50;c.value=mod805Value(mod808EffectiveUnit(key),modKind931(key),at,key);return c.value;
}
FLASHMEM int mod805Offset(uint8_t track,uint8_t parameter,int64_t at) {
  (void)at;if(!ioMask38)return 0;float sum=0;unsigned count=0;
  for(uint16_t n=0;n<modEnabledCount805;++n){uint16_t k=modEnabled805[n];if(!ioActive38(k)||!playbackMod828(k))continue;for(uint8_t j=0;j<8;++j){const auto&r=modUnits805[k].outputs[j];if(r.enabled&&r.track==track&&r.kind==parameter){sum+=ioLevel38(k)*lockValue817(LOCK_DEPTH817,k,j,r.depth);++count;}}}
  return count?lroundf(sum/count):0;
}
FLASHMEM void mod805SongApply(uint8_t t,int64_t logical,int32_t &at,int &note,int &vel,uint32_t &gate) {
  if(!modEnabledCount805)return;ioNote38(t,note,vel,gate,false);
  at+=clampInt(mod805Offset(t,MOD_TIM,logical),-96,96);
  note=clampInt(note+mod805Offset(t,MOD_NOT,logical),0,127);
  vel=clampInt(vel+mod805Offset(t,MOD_VEL,logical),0,127);
  gate=clampInt((int)gate+mod805Offset(t,MOD_GAT,logical),GATE_TICK_MIN,GATE_TICK_MAX);
}
FLASHMEM void mod805SongRoll(uint8_t t,int64_t logical,int64_t target,uint32_t gate,uint8_t note,TrackRollSettings roll,uint64_t end,int32_t sourceTick,uint8_t originalNote,uint8_t pattern,uint8_t originalVelocity,bool fixedRvl) {
  fixedRvl|=(roll.expo&0x80)!=0;roll.expo&=0x7f;
  if(!rollAllowed938(t,pattern,sourceTick,originalNote,fixedRvl))return;
  for(uint8_t f=0;f<6;++f)setRollFieldValue(roll,f,rollFieldValue(roll,f)+mod805Offset(t,MOD_RST+f,logical));
  auto rx=rollExtra1000(t,pattern,sourceTick,originalNote);
  if(!roll.start||(!roll.level&&!rx.v[1]))return;
  const int64_t origin38=target;int64_t tick=target-roll.start;target-=rollStop09(roll,rx);int32_t spacing=clampInt(roll.speed,1,96),acceleration=(int32_t)roll.accel*256;
  for(uint16_t repeat=0;repeat<MAX_PREROLL_NOTES&&tick<target;++repeat){
    if(((rollDirection60(t,pattern,sourceTick,originalNote)!=1&&tick==(int64_t)songTick)||(rollDirection60(t,pattern,sourceTick,originalNote)!=0&&2*origin38-tick==(int64_t)songTick))&&songTick<end){uint32_t length=min((uint64_t)gate,end-songTick);if(songLoopEnabled)length=min((uint64_t)length,(uint64_t)songEndBar*1536-songTick);
      int rv=rollVelocity1000(roll,rx,originalVelocity,repeat);rollEvent44(t,roll,rx,(int32_t)((int64_t)songTick-origin38),sourceTick,length,rollDirection60(t,pattern,sourceTick,originalNote));rollTrigger817=true;if(rv)triggerTrackNoteWithFixedGate(t,rollPitch1000(rx,note,repeat),rv,max(1000UL,(uint32_t)(length*tickDurationMs()*1000.0f)),0,sourceTick,originalNote,pattern);rollTrigger817=false;rollHoldUs44=0;}
    if(spacing>=target-tick)break;tick+=spacing;
    rollAdvance60(spacing,acceleration,roll.expo,repeat);
  }
}
FLASHMEM void mod805Note(uint8_t t,uint8_t note,uint8_t vel,uint32_t gateUs) {
 sourceTrigger909(0,t);
 if(remixNoteDelay911(t,note,vel,gateUs))return;
 pidiNote925(t,note,vel,gateUs,rollTrigger817);
}

FLASHMEM void mod806MidiNoteEvent(uint8_t channel,uint8_t note,uint8_t velocity,bool on) {
  if(channel>=16)return;ioMidiNote38(channel,note,velocity,on);
  int64_t now=songEngineActive?songTick:transportTickAbsolute;
  if(on&&velocity){
    sourceTrigger909(1,channel);
    modMidiBegin806[channel]=now;modMidiHeld806[channel]=true;modMidiNote806[channel]=note;
  }else if(modMidiHeld806[channel]&&modMidiNote806[channel]==note){
    modMidiGate806[channel]=max((int64_t)0,now-modMidiBegin806[channel]);modMidiHeld806[channel]=false;
  }else return;
  ++modEpoch805;
}
FLASHMEM void mod806UsbNoteOn(uint8_t channel,uint8_t note,uint8_t velocity) {
  if(controller940)return;
  if(channel>=1&&channel<=16)mod806MidiNoteEvent(channel-1,note,velocity,true);
}
FLASHMEM void mod806UsbNoteOff(uint8_t channel,uint8_t note,uint8_t velocity) {
  if(channel>=1&&channel<=16)mod806MidiNoteEvent(channel-1,note,velocity,false);
}
FLASHMEM void mod806CollectMidi(int64_t at) {
 BlockScope21 destinationScope55(blockIndex21());
  static uint32_t epoch=0,wall=0;static int64_t tick=-1;
  if(epoch==modEpoch805&&tick==at&&wall==millis())return;
  Scope830 measured(P830_COLLECT);
  epoch=modEpoch805;tick=at;wall=millis();
  modFrame810(at);modTrackCollect21(at);for(uint16_t i=0;i<modMidiAddressCount810;++i){uint16_t a=modMidiAddresses810[i];modMidiMix806[a/129][a%129]={};}modMidiAddressCount810=0;ioVirtualClear53();
  for(uint16_t n=0;n<modEnabledCount805;++n){
    uint16_t key=modEnabled805[n];if(remoteOwn54(key))continue;uint16_t owners50=indModOwners50(key);for(unsigned pass50=0;pass50<17;++pass50){if(owners50?(pass50==16||!(owners50&(1u<<pass50))):pass50!=16)continue;IndContext50 context50(pass50<16?pass50:-1);
    if(!playbackMod828(key)||((!playing||!mod806Playing(key,at))&&!ccActive941(key)&&!ioActive38(key)&&!(modKind931(key)&&modMode810(key)==2)&&!manualModActive06(key)))continue;
    float value=modLevelAt811(key,at);auto m=mod808EffectiveUnit(key);
    for(uint8_t j=0;j<8;++j){auto original=m.outputs[j];auto r=modResolve810(original);if(!r.enabled)continue;int64_t routeAt=at;
      if(original.kind==MOD_PATCH810&&original.track<16&&(trackCcMask933&(1u<<original.track)))routeAt=max((int64_t)0,(int64_t)(songEngineActive?songTick:transportTickAbsolute)-trackCc933[original.track]);
      float depth=modDepth810(key,j,routeAt);if(!r.enabled||!depth)continue;value=modRouteValues46[key][j];if(ioVirtual53(original)){ioVirtualAdd53(original.channel,original.number,original.reference+value*depth);continue;}
      if(original.kind==MOD_PATCH810){for(uint8_t c=0;c<16;++c)if(c!=original.track&&linkOwner12(c,modKind931(key)?LINK_LFO12:LINK_ENV12)==original.track&&modAssociated12(key,c)&&!trackSilent818(c)){auto projected=original;projected.track=c;auto target=modResolve810(projected);if(mod806MidiTarget(target)){MidiScope902 scope(outMask902(c),c);int base=0;patch808Base(target.track,target.number,base);modMidiPort19(target.track,target.kind==MOD_CC?target.number:128,value*depth,outMask902(c),base);}}}
      if(modTrackMidi43(original)&&mod806MidiTarget(r)){int base=modReference43(original);modMidiPort19(r.track,r.kind==MOD_CC?r.number:128,value*depth-(ccEnv941[key].enabled?base:0),1u<<(r.port-1),base);}
      else if(original.kind==MOD_TRACK21&&mod806MidiTarget(r)){modMidiPort19(r.track,r.number,value*depth-(ccEnv941[key].enabled?r.reference:0),1u<<(r.port-1),r.reference);}
      else if(original.kind==MOD_PATCH810&&mod806MidiTarget(r)){MidiScope902 scope(outMask902(original.track),original.track);int base=modReference43(original);modMidiPort19(r.track,r.kind==MOD_CC?r.number:128,value*depth-(ccEnv941[key].enabled?base:0),outMask902(original.track),base);}
      else if(original.port&&mod806MidiTarget(r)){int base=modReference43(original);modMidiPort19(r.track,r.kind==MOD_CC?r.number:128,value*depth-(ccEnv941[key].enabled?base:0),1u<<(original.port-1),base);}
      else if(mod806MidiTarget(r)){int base941=0;if(ccEnv941[key].enabled&&r.kind==MOD_CC)patch808Base(r.track,r.number,base941);modMidiAdd810(r.track,r.kind==MOD_CC?r.number:128,value*depth-base941);}
      else if(r.kind==MOD_PBN)modMidiPort19(midiChannelForTrack(r.track),128,indNote50(r.track,4)+value*depth,outMask902(r.track),0);
      else if(r.kind==MOD_VIRTUAL)for(const auto &link:virtualLinks805)
        if(link.enabled&&link.bus==r.number&&link.kind==MOD_PBN)
          modMidiPort19(midiChannelForTrack(link.track),128,indNote50(link.track,4)+value*depth*link.depth/127.0f,outMask902(link.track),0);
    }
  }
  }autoMix46();
}
FLASHMEM bool mod806ChannelBend(uint8_t channel,int64_t at,int &out) {
  mod806CollectMidi(at);
  uint8_t port=0,mask=midiMask902(channel);while(port<7&&!(mask&(1u<<port)))++port;
  const auto &mix=modMidiMix806[port*16+channel][128];
  if(!mix.count)return false;out=clampInt((int)lroundf(mix.value()),-8192,8191);return true;
}
FLASHMEM void mod806MidiService() {MidiInputScope53 scope53(2);
  // Note triggers on the configured DIN UART. No new UART/pin assignment.
  static uint8_t status=0,data[2]={},count=0;
  while(MIDI_SERIAL.available()) {
    uint8_t b=MIDI_SERIAL.read();
    if(b>=0xF8)continue;
    if(b&0x80){status=b<0xF0?b:0;count=0;continue;}
    if(!status)continue;
    data[count++]=b;
    uint8_t kind=status&0xF0,needed=(kind==0xC0||kind==0xD0)?1:2;
    if(count<needed)continue;count=0;
    uint8_t c=status&15;
    if(kind==0x90)mod806MidiNoteEvent(c,data[0],data[1],data[1]!=0);
    else if(kind==0x80)mod806MidiNoteEvent(c,data[0],data[1],false);
    else if(kind==0xB0)ccReceive941(c,data[0],data[1]);
  }
}
FLASHMEM void modStop811(){modTrackStop21();
  modMidiPreviousCount830=0;
  memset(rollModAt827,255,sizeof(rollModAt827));pidiCancel827();
  memset(modMidiHeld806,0,sizeof(modMidiHeld806));
  for(auto&s:manualMod06)s=ManualMod06{};manualModMask06=0;
  for(auto &begin:modMidiBegin806)begin=INT64_MIN/2;
  for(uint16_t c=0;c<128;++c)for(uint16_t o=0;o<129;++o){modCancelPort19(c,o);modMidiWas806[c][o]=false;}
  for(uint16_t i=0;i<modMidiAddressCount810;++i){uint16_t a=modMidiAddresses810[i];modMidiMix806[a/129][a%129]={};}modMidiAddressCount810=0;ioVirtualClear53();
  mod805Invalidate();
}
FLASHMEM void mod805Service() {
  autoMidi46(songEngineActive?songTick:transportTickAbsolute);
  mod806UiService(); // Serial8 is now the private Pidi link, not a DIN MIDI input.
  pidiService827();cvCollect46(songEngineActive?songTick:transportTickAbsolute);
  static bool wasPlaying=false;
  if(wasPlaying!=playing){
    wasPlaying=playing;mod805Invalidate();
    if(!playing){suppressCancel903=stableTransport904&&keepDelayPaused903;modStop811();suppressCancel903=false;}
  }

  if(controller940)return;
  if(!playing){bool active=false;for(uint16_t n=0;n<modEnabledCount805;++n){uint16_t k=modEnabled805[n];if(ioActive38(k)||ccActive941(k)||(modKind931(k)&&modMode810(k)==2)||manualModActive06(k)){active=true;break;}}if(!active&&!modMidiPreviousCount830&&!modTrackPending21())return;}
  uint64_t now=songEngineActive?songTick:transportTickAbsolute;
  // Tee services active repeat series; Pidi receives ordinary MIDI messages.
  // RAT changes sampling cadence. UART capacity bounds output, not curve time.
  static int64_t lastTick=-1;static uint32_t lastWall=0,lastEpoch=0;
  int64_t evaluation=max((int64_t)0,(int64_t)now-midiAdvance808);
  if(lastTick==evaluation&&lastWall==millis()&&lastEpoch==modEpoch805)return;
  lastTick=evaluation;lastWall=millis();lastEpoch=modEpoch805;
  mod808FlushMidi(evaluation,4);
}
FLASHMEM void mod808FlushMidi(int64_t at,uint8_t budget){
  MidiScope902 globalMidi(255,255); // Global MIDI destinations must not inherit a triggering note's port.
  Scope830 measured(P830_FLUSH);
  (void)budget; // Legacy argument: this prepares values, midiService810 budgets TX.
  mod806CollectMidi(at);ioVirtualFlush53();cvCollect46(at);
  // Visit removed addresses once to cancel obsolete automation, then only the
  // current active set. This preserves cancellation at depth zero and bypass.
  if(optimize830){
    for(uint16_t i=0;i<modMidiPreviousCount830;++i){uint16_t a=modMidiPrevious830[i];++flushVisited830;
      if(!modMidiMix806[a/129][a%129].count){modCancelPort19(a/129,a%129);modMidiWas806[a/129][a%129]=false;}
    }
  }
  uint16_t count=optimize830?modMidiAddressCount810:128*129;
  for(uint16_t i=0;i<count;++i){
    uint16_t address=optimize830?modMidiAddresses810[i]:i;
    uint8_t c=address/129;uint16_t o=address%129;++flushVisited830;
    auto &mix=modMidiMix806[c][o];bool active=mix.count!=0;
    if(!active){
      if(modMidiWas806[c][o])modCancelPort19(c,o); // depth zero is silence, not value zero.
      modMidiWas806[c][o]=false;continue;
    }
    int base=modBase19[c][o];
    int out=clampInt(base+(int)lroundf(mix.value()),o==128?-8192:0,o==128?8191:127);
    MidiScope902 route19(1u<<(c/16));modRemember937(c%16,o,base);midiOffer810(c%16,o,out,true);modLastCC805[c][o]=out;modMidiWas806[c][o]=true;
  }
  modMidiPreviousCount830=modMidiAddressCount810;
  memcpy(modMidiPrevious830,modMidiAddresses810,modMidiPreviousCount830*sizeof(uint16_t));
}
#include "Timing830.h"
#include "Delay907.h"
#include "MultiDelay924.h"
#include "Mod806Ui.h"


