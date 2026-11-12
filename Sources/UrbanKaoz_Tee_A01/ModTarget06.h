#pragma once
const char* modMidiLabel10(const ModRoute805&,bool);
bool targetLocked11=false;
// The adjacent destination value follows the same selection/P.Lock edit rules.
FLASHMEM bool modTargetBase06(const ModRoute805&r,int delta,int&value,const char*&label){
 targetLocked11=false;if(!r.enabled)return false;if(ioVirtual53(r)){value=clampInt(r.reference+delta,0,127);label="VCC";if(delta){for(auto&m:modUnits805)for(auto&q:m.outputs)if(ioVirtual53(q)&&q.channel==r.channel&&q.number==r.number)q.reference=value;ioVirtualSend53(r.channel,r.number,value);storageDirty931=true;mod805Invalidate();}return true;}if(r.kind==MOD_CV46){value=clampInt(r.reference+delta,0,4095);label="Base";if(delta){for(auto&m:modUnits805)for(auto&q:m.outputs)if(q.enabled&&q.kind==MOD_CV46&&q.track==r.track)q.reference=value;for(auto&l:autoLanes46)if(l.used&&!l.type&&l.route.kind==MOD_CV46&&l.route.track==r.track)l.route.reference=value;++modEpoch805;storageDirty931=true;}return true;}if(modBaseEdit43(r,delta,value,label))return true;if(r.kind==MOD_TRACK21)return modTrackBase21(r,delta,value,label);
 if(r.port && (r.kind==MOD_CC||r.kind==MOD_MIDI_BEND806)){
  value=clampInt((int)r.reference+delta,r.kind==MOD_MIDI_BEND806?-8192:0,r.kind==MOD_MIDI_BEND806?8191:127);label=r.kind==MOD_MIDI_BEND806?"Bnd":modMidiLabel10(r,true);
  if(delta){for(auto&m:modUnits805)for(auto&q:m.outputs)if(q.enabled&&q.port==r.port&&q.channel==r.channel&&q.number==r.number&&((q.kind==MOD_MIDI_BEND806)==(r.kind==MOD_MIDI_BEND806)))q.reference=value;
   MidiScope902 route(1u<<(r.port-1));midiOffer810(r.channel,r.kind==MOD_MIDI_BEND806?128:r.number,value,false);++modEpoch805;storageDirty931=true;}
  return true;
 }
 if(r.kind==MOD_PATCH810||r.kind==MOD_CC){
  for(uint8_t t=0;t<16;++t){if(r.kind==MOD_PATCH810&&t!=r.track)continue;
   auto&c=patch808Context(t,patch808PlayingPattern(t));
   if(r.kind==MOD_CC&&patch808Channel(t,c)!=r.track)continue;
   for(auto*q=patchNext819(c,nullptr);q;q=patchNext819(c,q))if(!q->kind&&q->number==r.number){
    uint16_t base=0;patchValue819(c,*q,base);value=clampInt((int)base+delta,0,127);label=patchLabel10(c,*q,true);
    if(lockWantsEdit817()){if(delta)lockEdit817(LOCK_CC817,t,q->number,base,delta,0,127);value=lockDisplay817(LOCK_CC817,t,q->number,base);targetLocked11=value!=base;return true;}
    if(delta){if(!patchSetValue819(c,*q,value))return false;MidiScope902 route(outMask902(t),t);patch808SendParam(patch808Channel(t,c),*q,value);}
    return true;
   }
  }
  // A raw CC remains editable without a patch file or parameter label.
  uint8_t t=r.kind==MOD_PATCH810?r.track:selectedTrack;auto&c=patch808Context(t,patch808PlayingPattern(t));uint8_t channel=r.kind==MOD_CC?r.track:patch808Channel(t,c);
  auto&pending=midiPending810[firstOut12(t)*16+channel][r.number];int base=pending.known?pending.last:0;
  base=lockOriginal11(LOCK_CC817,channel,r.number,base);
  label="midi";if(lockWantsEdit817()){if(delta)lockEdit817(LOCK_CC817,t,r.number,base,delta,0,127);value=lockDisplay817(LOCK_CC817,t,r.number,base);targetLocked11=value!=base;return true;}
  value=clampInt(base+delta,0,127);if(delta){MidiScope902 route(outMask902(t),t);midiOffer810(channel,r.number,value,false);}return true;
 }
 if(modIsTarget811(r)){
  uint16_t key=modTarget810(r);if(key>=64||!mod806Unit(key).enabled)return false;
  auto&m=mod806Unit(key);uint8_t field=r.number%8;int lo=0,hi=65535;
  if(r.kind==MOD_DEPTH810){if(!m.outputs[field].enabled)return false;value=m.outputs[field].depth;label="Dep";hi=modLimit810(m.outputs[field].kind);lo=-hi;if(hi==8192)--hi;}
  else {if(field>=modParamCount811(key))return false;label=modParamName811(key,field);
   if(modKind931(key)){value=field?modExtra810[key].phase:m.rate;hi=field?360:LFO_MAX42;lo=field?0:1;}
   else {value=field==0?m.hold:field==1?m.attack:field==2?m.decay:field==3?m.sustain:m.release;if(field==3)hi=127;}
  }
  uint8_t lk=r.kind==MOD_DEPTH810?LOCK_DEPTH817:(modKind931(key)&&field)?LOCK_EXTRA817:LOCK_MOD817;uint8_t param=(modKind931(key)&&field&&r.kind!=MOD_DEPTH810)?4:field;
  if(lockWantsEdit817()){int base=value;if(delta)lockEdit817(lk,key,param,base,delta,lo,hi);value=lockDisplay817(lk,key,param,base);targetLocked11=value!=base;return true;}
  value=clampInt(value+delta,lo,hi);
  if(delta){if(r.kind==MOD_DEPTH810)m.outputs[field].depth=value;
   else if(modKind931(key)){if(field)modExtra810[key].phase=value;else m.rate=value;}
   else {if(field==0)m.hold=value;if(field==1)m.attack=value;if(field==2)m.decay=value;if(field==3)m.sustain=value;if(field==4)m.release=value;}
  }return true;
 }
 return false;
}
