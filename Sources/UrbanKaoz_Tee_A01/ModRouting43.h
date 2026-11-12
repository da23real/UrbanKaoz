#pragma once
// Read the unmodulated patch base, never the last MIDI value produced by the LFO.
FLASHMEM PatchParam808* modBasePatch43(const ModRoute805&r,uint8_t&t,uint8_t&b){
 if((r.kind!=MOD_CC&&r.kind!=MOD_PATCH810)||!patchDb808.defs)return nullptr;
 bool fixed=modTrackMidi43(r)||r.kind==MOD_PATCH810;
 for(t=fixed?r.track:0;t<16;++t){for(b=0;b<(fixed?1:3);++b){
  if(!fixed){auto a=blockAtNote21(t,b,255,-1,255);if(!r.port||a.port!=r.port||a.channel!=r.channel)continue;}
  auto&c=blockPatchContext21(t,b);for(auto*q=patchNext819(c,nullptr);q;q=patchNext819(c,q))if(!q->kind&&q->number==r.number)return q;
 }if(fixed)break;}return nullptr;
}
FLASHMEM int modReference43(const ModRoute805&r){
 if(modTrackMidi43(r)&&r.kind==MOD_MIDI_BEND806)return trackPitchBend[r.track];
 uint8_t t,b;auto*q=modBasePatch43(r,t,b);uint16_t base=0;if(q&&patchValue819(blockPatchContext21(t,b),*q,base))return base;
 return r.reference;
}
FLASHMEM bool modBaseEdit43(const ModRoute805&r,int delta,int&value,const char*&label){
 if(modTrackMidi43(r)&&r.kind==MOD_MIDI_BEND806){value=ZCLAMP57(trackPitchBend[r.track],delta,-8192,8191);label="Bnd";if(delta){trackPitchBend[r.track]=value;auto a=modResolve810(r);if(a.enabled){MidiScope902 scope(1u<<(a.port-1));midiOffer810(a.channel,128,value,false);}storageDirty931=true;mod805Invalidate();}return true;}
 uint8_t t,b;auto*q=modBasePatch43(r,t,b);if(!q){if(!modTrackMidi43(r))return false;value=clampInt((int)r.reference+delta,0,127);label="CC";if(delta){for(auto&m:modUnits805)for(auto&dest:m.outputs)if(modTrackMidi43(dest)&&dest.track==r.track&&dest.kind==r.kind&&dest.number==r.number)dest.reference=value;auto a=modResolve810(r);if(a.enabled){MidiScope902 scope(1u<<(a.port-1));midiOffer810(a.channel,r.number,value,false);}storageDirty931=true;mod805Invalidate();}return true;}
 auto&c=blockPatchContext21(t,b);uint16_t base=clampInt(r.reference,0,127);patchRawValue50(c,*q,base);if(base==65535)base=0;value=clampInt(lockDisplay817(LOCK_CC817,t,q->number,base)+delta,0,127);label=q->shortName;
 if(lockWantsEdit817()){if(delta)lockEdit817(LOCK_CC817,t,q->number,base,delta,0,127);value=lockDisplay817(LOCK_CC817,t,q->number,base);targetLocked11=value!=base;return true;}
 if(delta){if(!patchSetValue819(c,*q,value)){DISPLAY_SERIAL.println("MSG|PATCH MEMORY FULL");return true;}auto a=blockAtNote21(t,b,255,-1,255);if(a.port){MidiScope902 scope(1u<<(a.port-1),t);patch808SendParam(a.channel,*q,value);}storageDirty931=true;mod805Invalidate();}return true;
}

