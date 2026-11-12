#pragma once
EXTMEM int64_t modPatternEventAt810[64];
EXTMEM uint16_t modOrder810[64],modIndegree810[64],modQueue810[64];
EXTMEM float modLevel810[64],modDepths810[64][8],modSums810[64][8];
EXTMEM uint16_t modCounts810[64][8];
EXTMEM float modParamSums811[64][5];
EXTMEM uint16_t modParamCounts811[64][5];
int16_t modEvaluating811=-1;uint16_t modEffectivePhase811=0;
FLASHMEM bool modIsTarget811(const ModRoute805&r){return r.kind==MOD_DEPTH810||r.kind==MOD_PARAM811;}
FLASHMEM uint8_t modParamCount811(uint16_t key){return ioKey38(key)?0:modKind931(key)?2:5;}
FLASHMEM const char* modParamName811(uint16_t key,uint8_t param){
  static const char*env[]={"Hold","Attack","Decay","Sustain","Release"};
  static const char*lfo[]={"Cycle","Phase"};
  return param<modParamCount811(key)?(modKind931(key)?lfo[param]:env[param]):"?";
}
FLASHMEM int modExistingOutput811(uint16_t key,int current,int direction){
  if(key>=64||!mod806Unit(key).enabled)return -1;
  const auto&m=mod806Unit(key);
  if(!direction&&current>=0&&current<8&&m.outputs[current].enabled)return current;
  int start=direction?current+(direction>0?1:-1):0,step=direction<0?-1:1;
  for(int i=start;i>=0&&i<8;i+=step)if(m.outputs[i].enabled)return i;
  if(current>=0&&current<8&&m.outputs[current].enabled)return current;
  for(int i=0;i<8;++i)if(m.outputs[i].enabled)return i;
  return -1;
}
FLASHMEM float modLevelAt811(uint16_t key,int64_t at){modFrame810(at);return modLevel810[key];}
uint16_t modOrderCount810=0;bool modGraphOk810=true;
uint32_t modFrameEpoch810=0,modFrameWall810=0;int64_t modFrameTick810=-1;
FLASHMEM void modExtraReset810(uint16_t key){envEarly50[key]=0;indClocksReset50(key);freeClock48[key]={};modExtra810[key]={0,0,0,0,0};ccEnv941[key]={};ccSeen941[key]=false;modPatternEventAt810[key]=-1;}
FLASHMEM void realtimeInit810(){memset(freeClock48,0,sizeof(freeClock48));ccGap810=2;ccBudget810=500;fullMigrateNeeded810=false;memset(modExtra810,0,sizeof(modExtra810));for(auto&v:modPatternEventAt810)v=-1;graphDirty810=true;modFrameEpoch810=0;midiReset810();}
FLASHMEM float curve810(float x,int curve){x=max(0.0f,min(1.0f,x));float power=expPower14(curve,expEnvMaximum15);return curve>0?1-powf(1-x,power):curve<0?powf(x,power):x;}
FLASHMEM float heldSigned50(float age,const ModUnit805&m,const ModExtra810&e,int hold){
  if(age<hold)return 0;age-=hold;if(m.attack&&age<m.attack)return curve810(age/m.attack,e.attackCurve);age-=m.attack;
  float level=m.sustain/127.0f;if(m.decay&&age<m.decay)return 1-(1-level)*curve810(age/m.decay,e.decayCurve);return level;
}
FLASHMEM float held810(float age,const ModUnit805&m,const ModExtra810&e){return heldSigned50(age,m,e,m.hold);}
#include "ModModes07.h"
FLASHMEM void modPatternEvent810(uint8_t t,uint8_t p,int64_t at){
  for(uint16_t key=0;key<64;++key)if(mod806Unit(key).enabled&&modAssociated12(key,t))modPatternEventAt810[key]=at;
  ++modEpoch805;
}
FLASHMEM int64_t modPatternStart810(uint16_t key,int64_t at){if(autoRenderTrack46>=0)return 0;
  int64_t latest=modPatternEventAt810[key]<=at?modPatternEventAt810[key]:-1;
  for(uint8_t t=0;t<16;++t)if(!trackMuted[t]){
    if(songEngineActive){for(const auto&c:songClips)if(c.used&&c.track==t&&modAssociated12(key,t)&&at>=(int64_t)c.position*96&&at<(int64_t)(c.position+c.length)*96)latest=max(latest,(int64_t)c.position*96);}
    else if(modAssociated12(key,t)){if(grooveBridgeReady[t]&&!grooveBridgeSwitched[t]&&pendingPattern[t]>=0&&at>=(int64_t)grooveBridgeAt[t])latest=max(latest,(int64_t)grooveBridgeAt[t]);uint32_t len=trackSequenceTicks(t);if(len){uint32_t phase=(bridgeMasterTickAt(max((int64_t)0,at))+len-grooveTrackPhase[t]%len)%len;latest=max(latest,at-phase);}}
  }return latest;
}
struct FreeSample31 {uint64_t elapsed;uint32_t epoch;float tickMs,value;bool valid;};
EXTMEM FreeSample31 freeSamples31[64],indFreeSamples50[16][64];
static EXTMEM FreeClock48 indFreeClock50[16][64];
FLASHMEM float modValue810(const ModUnit805&m,uint8_t type,int64_t at,uint16_t key,int64_t latest,uint32_t gate,bool manual){
  Scope830 measured(P830_CURVE);
  int hold50=type?m.hold:holdValue50(key,m);int64_t triggerAt50=at-min(0,hold50);int64_t noteTrigger36=latest;auto e=modExtra810[key];e.attackCurve=lockValue817(LOCK_EXTRA817,key,0,e.attackCurve);e.decayCurve=lockValue817(LOCK_EXTRA817,key,1,e.decayCurve);e.releaseCurve=lockValue817(LOCK_EXTRA817,key,2,e.releaseCurve);e.mode=lockValue817(LOCK_EXTRA817,key,3,e.mode);e.phase=lockValue817(LOCK_EXTRA817,key,4,e.phase);
  int64_t oscillatorAt51=manual&&type&&(e.mode==1||e.mode==3||e.mode==4||e.mode==6)?(int64_t)(songEngineActive?songTick:transportTickAbsolute):at;
  if((!manual||type)&&(playing||autoRenderTrack46>=0)){
    if(type)triggerAt50=oscillatorAt51;
    if(e.mode==1)latest=modPatternStart810(key,triggerAt50);
    else if(e.mode==3||e.mode==4){int64_t period=e.mode==3?1536LL*(type?max((uint16_t)1,m.hold):(uint16_t)1):384LL*(type?max((uint16_t)1,m.hold):(uint16_t)1);latest=triggerAt50-triggerAt50%period;}
    if(!type&&(e.mode==1||e.mode==3||e.mode==4))gate=max(1,hold50+(int)m.attack+(int)m.decay);
  }
  if(type&&e.mode==6&&lfoEnvMode36(m)==2){latest=-1;gate=0;for(uint8_t t=0;t<16;++t)if(m.inputs&(1u<<t)){auto&w=rollWindows44[t];if(w.begin>=0&&w.begin<=oscillatorAt51&&w.begin>latest){latest=w.begin;gate=(uint32_t)max((int64_t)0,w.end-w.begin);}}}
  if(!type){if(latest<0)return 0;float age=at-latest;if(hold50<0){unsigned rat=max(1,lockValue817(LOCK_RAT817,key,0,modRat808[key]));if(age<=gate&&age>=hold50)age=hold50+floorf((age-hold50)/rat)*rat;else if(age>gate)age=gate+floorf((age-gate)/rat)*rat;}
    if(age<=gate){return heldSigned50(age,m,e,hold50);}
    if(!m.release||age>=gate+m.release)return 0;return heldSigned50(gate,m,e,hold50)*(1-curve810((age-gate)/m.release,e.releaseCurve));}
  
  uint64_t elapsed=0;
  if(e.mode==2&&autoRenderTrack46>=0){elapsed=(uint64_t)(at*tickDurationMs());uint16_t rat=max(1,lockValue817(LOCK_RAT817,key,0,modRat808[key]));elapsed-=elapsed%rat;}
  else if(e.mode==2){static uint32_t previous=0;static uint64_t uptime=0;uint32_t now=millis();uptime+=(uint32_t)(now-previous);previous=now;uint16_t rat=max(1,lockValue817(LOCK_RAT817,key,0,modRat808[key]));elapsed=uptime-uptime%rat;}
  else {if(latest<0)return 0;elapsed=max((int64_t)0,oscillatorAt51-latest);}
  if(e.mode==6&&oscillatorAt51>=latest+gate)return 0;
  bool ms=m.curve==2||(m.curve==0&&e.mode==2);
  double freePhase48=0;
  if(e.mode==2&&autoRenderTrack46<0){
    auto&clock=indContext50>=0?indFreeClock50[indContext50][key]:freeClock48[key];float period=max(0.001f,(float)max((uint32_t)1,m.rate)*(ms?1.0f:tickDurationMs()));
    // Integrate the elapsed part using the previous cycle duration. A new note
    // may change Cycle through a PLX, but must not reposition a Free oscillator.
    if(!clock.valid){clock.phase=fmod((double)elapsed/period,1.0);clock.valid=true;}
    else if(elapsed>=clock.at)clock.phase=fmod(clock.phase+(double)(elapsed-clock.at)/clock.period,1.0);
    clock.at=elapsed;clock.period=period;freePhase48=fmod(clock.phase+(double)queryOffset50*tickDurationMs()/period+1.0,1.0);
  }
  if(e.mode==2&&!ms)elapsed=(uint64_t)(elapsed/max(0.001f,tickDurationMs()));
  else if(e.mode!=2&&ms)elapsed=(uint64_t)(elapsed*tickDurationMs());
  bool cache31=!queryOffset50&&autoRenderTrack46<0&&(optim31&8)&&e.mode==2&&!manual&&modEvaluating811<0&&lfoEnvMode36(m)<2;
  auto&cached31=indContext50>=0?indFreeSamples50[indContext50][key]:freeSamples31[key];float tickMs31=tickDurationMs();
  if(cache31&&cached31.valid&&cached31.elapsed==elapsed&&cached31.epoch==modEpoch805&&cached31.tickMs==tickMs31){++sampleHits31;return cached31.value;}
  float result31=[&](){
  float age36=ms?elapsed/max(0.001f,tickDurationMs()):(float)elapsed;
  uint8_t env36=lfoEnvMode36(m);
  if(env36==1)age36=e.mode==2&&autoRenderTrack46<0?(float)(freePhase48*max((uint32_t)1,m.rate)/(ms?max(0.001f,tickDurationMs()):1.0f)):ms?(elapsed%max((uint32_t)1,m.rate))/max(0.001f,tickDurationMs()):(float)(elapsed%max((uint32_t)1,m.rate));
  else if(env36==2)age36=noteTrigger36<0?-1:(float)max((int64_t)0,at-noteTrigger36);
  else if(env36>=3){int64_t span36=(env36==3?1536LL:384LL)*max((uint16_t)1,m.hold);int64_t clock51=manual?(int64_t)(songEngineActive?songTick:transportTickAbsolute):at;age36=clock51<0?-1:(float)(clock51%span36);}
  float gain27=lfoEnvelopeOn27(m)?lfoEnvelope27(m,e.releaseCurve,age36):1;
  float phase=fmodf((e.mode==2&&autoRenderTrack46<0?(float)freePhase48:(float)(elapsed%max((uint32_t)1,m.rate))/max((uint32_t)1,m.rate))+(modEvaluating811==(int16_t)key?modEffectivePhase811:e.phase)/360.0f,1.0f);
  if(m.shape==1)return gain27*(phase<0.5f?1.0f:-1.0f);if(m.shape==2)return gain27*(phase*2-1);if(m.shape==3)return gain27*(1-phase*2);
  if(m.shape==4)return gain27*sinf(phase*6.28318530718f);
  return gain27*(phase<0.5f?phase*4-1:3-phase*4);
  }();
  if(cache31)cached31={elapsed,modEpoch805,tickMs31,result31,true};
  return result31;
}
FLASHMEM bool modTrackMidi43(const ModRoute805&r){return r.block21==3&&(r.kind==MOD_CC||r.kind==MOD_MIDI_BEND806);}
FLASHMEM ModRoute805 modResolve810(ModRoute805 r){
  if(modTrackMidi43(r)){auto b=blockAtNote21(r.track,0,255,-1,255);r.enabled=r.enabled&&b.port;r.port=b.port;r.channel=r.track=b.channel;return r;}
  if(r.kind==MOD_TRACK21)return modTrackResolve21(r);
  if(r.kind<=MOD_RLV){r.enabled=0;return r;} // Track outputs retired, preserved only for old file compatibility.
  if(r.kind==MOD_PATCH810){r.kind=MOD_CC;r.track=patch808Channel(r.track,patch808Context(r.track,patch808PlayingPattern(r.track)));return r;}
  if(r.port && r.kind!=MOD_PATCH810 && (r.kind==MOD_CC||r.kind==MOD_MIDI_BEND806))r.track=r.channel;
  return r;
}
FLASHMEM uint16_t modTarget810(const ModRoute805&r){return (uint16_t)r.track*32+r.number/8;}
FLASHMEM bool modGraphValid810(const ModUnit805*units){
  memset(modIndegree810,0,sizeof(modIndegree810));uint16_t enabled=0;
  for(uint16_t k=0;k<64;++k)if(units[k].enabled){++enabled;for(const auto&r:units[k].outputs)if(r.enabled&&modIsTarget811(r)){uint16_t to=modTarget810(r);if(to>=64)return false;if(units[to].enabled)++modIndegree810[to];}}
  uint16_t head=0,tail=0;for(uint16_t k=0;k<64;++k)if(units[k].enabled&&!modIndegree810[k])modQueue810[tail++]=k;
  while(head<tail){uint16_t k=modQueue810[head++];for(const auto&r:units[k].outputs)if(r.enabled&&modIsTarget811(r)){uint16_t to=modTarget810(r);if(units[to].enabled&&!--modIndegree810[to])modQueue810[tail++]=to;}}
  return tail==enabled;
}
FLASHMEM bool modAcyclic810(uint16_t from,uint8_t slot,const ModRoute805&r){
  auto&original=mod806Unit(from).outputs[slot];auto saved=original;original=r;
  bool ok=modGraphValid810(modUnits805);original=saved;graphDirty810=true;return ok;
}
FLASHMEM void modFrame810(int64_t at){
 remotePrepare54(at);QueryTime50 query50(at,queryOffset50);static int8_t lastContext50=-2;if(lastContext50!=indContext50){modFrameEpoch810=0;lastContext50=indContext50;}
  if(!modEnabledCount805)return;
  uint32_t wall=millis();if(modFrameEpoch810==modEpoch805&&modFrameTick810==at&&modFrameWall810==wall)return;
  struct ForecastScope50 { ~ForecastScope50(){for(auto &f:forecast50)f.valid=false;} } forecastScope50;
  Scope830 measured(P830_FRAME);++frameBuilds830;
  if(graphDirty810){modGraphOk810=modGraphValid810(modUnits805);modOrderCount810=0;if(modGraphOk810)for(uint16_t i=0;i<64;++i)if(mod806Unit(i).enabled)++modOrderCount810;memcpy(modOrder810,modQueue810,modOrderCount810*2);graphDirty810=false;}
  modFrameEpoch810=modEpoch805;modFrameTick810=at;modFrameWall810=wall;
  for(uint16_t n=0;n<modEnabledCount805;++n){uint16_t key=modEnabled805[n];memset(modSums810[key],0,sizeof(modSums810[key]));memset(modCounts810[key],0,sizeof(modCounts810[key]));memset(modDepths810[key],0,sizeof(modDepths810[key]));modLevel810[key]=0;memset(modParamSums811[key],0,sizeof(modParamSums811[key]));memset(modParamCounts811[key],0,sizeof(modParamCounts811[key]));}
  if(!modGraphOk810)return;autoGraph46();
  for(uint16_t n=0;n<modOrderCount810;++n){uint16_t key=modOrder810[n];if(remoteOwn54(key))continue;if(!playbackMod828(key))continue;prepareForecast50(key,at);auto m=mod808EffectiveUnit(key);
    bool active=autoRenderTrack46>=0||ioActive38(key)||(playing&&mod806Playing(key,at))||ccActive941(key)||(modKind931(key)&&modMode810(key)==2)||manualModActive06(key);
    bool modified=false;modEffectivePhase811=lockValue817(LOCK_EXTRA817,key,4,modExtra810[key].phase);
    for(uint8_t p=0;p<modParamCount811(key);++p)if(modParamCounts811[key][p]){
      modified=true;float offset=modParamSums811[key][p]/modParamCounts811[key][p];int base=0,limit=65535;
      if(modKind931(key)){base=p?modEffectivePhase811:m.rate;limit=p?360:LFO_MAX42;}
      else {base=p==0?m.hold:p==1?m.attack:p==2?m.decay:p==3?m.sustain:m.release;if(p==3)limit=127;}
      int v=clampInt((int)lroundf(base+offset),(modKind931(key)&&p==0)?1:0,limit);
      if(modKind931(key)){if(p)modEffectivePhase811=v;else m.rate=v;}
      else {if(p==0)m.hold=v;if(p==1)m.attack=v;if(p==2)m.decay=v;if(p==3)m.sustain=v;if(p==4)m.release=v;}
    }
    float value=0;
    if(active&&!ioKey38(key)){
      if(modified){modEvaluating811=key;uint16_t rat=max(1,lockValue817(LOCK_RAT817,key,0,modRat808[key]));int64_t sample=max((int64_t)0,at);int64_t origin41=0;uint8_t mode41=modMode810(key);if(modKind931(key)&&(mode41==3||mode41==4)){int64_t period41=(mode41==3?1536LL:384LL)*max((uint16_t)1,m.hold);origin41=sample-sample%period41;}if(autoRenderTrack46<0||modMode810(key)!=2)sample-=(sample-origin41)%rat;value=mod805Value(m,modKind931(key),sample,key);modEvaluating811=-1;}
      else value=mod805UnitValue(key,at);
    }
    if(ioKey38(key))value=ioLevel38(key);
    modLevel810[key]=value;
    for(uint8_t j=0;j<8;++j){auto r=m.outputs[j];if(!r.enabled)continue;
      float depth=r.depth+(modCounts810[key][j]?modSums810[key][j]/modCounts810[key][j]:0);
      int limit=modLimit810(r.kind);
      depth=max((float)-limit,min((float)(limit==8192?8191:limit),depth));if(ccEnv941[key].enabled){depth=constrain(depth,-90.0f,90.0f);depth=depth>=0?1+depth/10.0f:1/(1-depth/10.0f);}float routeValue46=value;if(!autoLive46(key,j,at,m,routeValue46,depth))depth=0;modRouteValues46[key][j]=routeValue46;modDepths810[key][j]=depth;
      if(modIsTarget811(r)&&active&&depth!=0){uint16_t to=modTarget810(r);uint8_t dest=r.number%8;if(to<64&&playbackMod828(to)&&mod806Unit(to).enabled){if(r.kind==MOD_PARAM811&&dest<modParamCount811(to)){modParamSums811[to][dest]+=routeValue46*depth;++modParamCounts811[to][dest];}else if(r.kind==MOD_DEPTH810&&mod806Unit(to).outputs[dest].enabled){modSums810[to][dest]+=routeValue46*depth;++modCounts810[to][dest];}}}
    }
  }
}
FLASHMEM float modDepth810(uint16_t key,uint8_t slot,int64_t at){modFrame810(at);return modDepths810[key][slot];}
FLASHMEM const char* modPatchName810(uint8_t t,uint8_t cc,bool shortName){
  const auto&ctx=patch808Context(t,patch808PlayingPattern(t));
  auto*q=patchAddress10(ctx,0,cc,7);return q?(shortName?q->shortName:q->name):"MIDI";
}
FLASHMEM const char* modMidiLabel10(const ModRoute805&r,bool shortName){
 uint8_t t=(r.kind==MOD_PATCH810||modTrackMidi43(r))?r.track:modOwnerTrack805;
 const auto&c=patch808Context(t,patch808PlayingPattern(t));
 if(r.kind==MOD_CC&&!modTrackMidi43(r)&&patch808Channel(t,c)!=r.track)return "MIDI";
 return modPatchName810(t,r.number,shortName);
}
FLASHMEM uint8_t modPatchNext810(uint8_t t,uint8_t cc,int delta){
  const auto&ctx=patch808Context(t,patch808PlayingPattern(t));PatchParam808*prev=nullptr,*first=nullptr;
  for(auto q=patchNext819(ctx,nullptr);q;q=patchNext819(ctx,q))if(!q->kind){if(!first)first=q;if(q->number==cc){if(delta<0)return prev?prev->number:cc;for(auto next=patchNext819(ctx,q);next;next=patchNext819(ctx,next))if(!next->kind)return next->number;return cc;}prev=q;}
  return first?first->number:cc;
}
FLASHMEM void modMigrateUnit810(ModUnit805&m,ModExtra810&e,bool lfo){
  e={};float tick=max(0.01f,tickDurationMs());
  if(!lfo){m.hold=min(65535,(int)lroundf(m.hold*10/tick));m.attack=min(65535,(int)lroundf(m.attack*10/tick));m.decay=min(65535,(int)lroundf(m.decay*10/tick));m.release=min(65535,(int)lroundf(m.release*10/tick));e.attackCurve=e.decayCurve=e.releaseCurve=m.curve==1?-25:m.curve==2?25:0;}
  else if(m.curve==1){e.mode=2;m.rate=clampInt(lroundf(m.rate*tick),1,65535);}m.curve=0;
}
FLASHMEM void modMigrate810(){
  for(uint16_t k=0;k<64;++k)modMigrateUnit810(mod806Unit(k),modExtra810[k],modKind931(k));
  graphDirty810=true;
}
FLASHMEM void realtimeService810(){
  midiService810();static uint32_t displayAt=0,previousLoop=0;if(profile830){uint32_t now=micros();if(previousLoop)midiCounters810.maxLoopUs=max(midiCounters810.maxLoopUs,now-previousLoop);previousLoop=now;}else previousLoop=0;
  if(millis()-displayAt>=33){displayAt=millis();if(epRefreshR10){epRefreshR10=false;if(modUi805&&!quickWorking903){display821.invalidate();mod805Menu();modDisplayDirty810=true;}}if(delayMenuDirty817){delayMenuDirty817=false;if(modUi805==MOD_DELAY806||modUi805==MOD_FX806)mod805Menu();}if(modDisplayDirty810){modDisplayDirty810=false;mod805Display();}if(patchDisplayDirty810){patchDisplayDirty810=false;patch808Display();}}
}
