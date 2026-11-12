#pragma once
struct ModLegacyMeta931 {uint16_t patterns[16],midiInputs;};
// Disk compatibility only: legacy slots are streamed one at a time.
struct ModStageScope931 { ModStageScope931(){modStaging931=true;} ~ModStageScope931(){modStaging931=false;} };
inline bool modLock931(uint8_t kind){return kind==LOCK_MOD817||kind==LOCK_DEPTH817||kind==LOCK_RAT817||kind==LOCK_EXTRA817;}
inline void modRouteKey931(ModRoute805&r,uint16_t key){r.track=key/32;r.number=(key%32)*8+r.number%8;}
FLASHMEM bool modReadUnits931(File&f,bool wide42){
 memset(fullMap931,255,sizeof(fullMap931));memset(fullMods805,0,sizeof(fullMods805));memset(fullKinds931,0,sizeof(fullKinds931));
 for(auto&m:fullMods805)m.rate=384;
 uint16_t count=0;
 for(uint16_t old=0;old<512;++old){ModUnit805 m{};if(wide42){if(f.read((uint8_t*)&m,sizeof(m))!=sizeof(m))return false;}else{ModLegacy42 old42{};if(f.read((uint8_t*)&old42,sizeof(old42))!=sizeof(old42))return false;m.inputs=old42.inputs;m.hold=old42.hold;m.attack=old42.attack;m.decay=old42.decay;m.release=old42.release;m.rate=old42.rate;m.sustain=old42.sustain;m.curve=old42.curve;m.shape=old42.shape;m.enabled=old42.enabled;memcpy(m.outputs,old42.outputs,sizeof(m.outputs));}if(m.enabled>1||m.roll58>2||m.delay58>2||m.rate>LFO_MAX42)return false;
  if(!m.enabled)continue;
  if(count==64){DISPLAY_SERIAL.println("MSG|PROJECT >64 MODS");Serial.println("FULL,INVALID,MOD_CAPACITY_64_PROJECT_UNCHANGED");return false;}
  for(auto&r:m.outputs)if(!r.enabled)r={}; // Removed destinations carry no disk identity.
  fullMap931[old]=count;fullKinds931[count]=old%2;fullMods805[count++]=m;
 }
 for(auto&m:fullMods805)for(auto&r:m.outputs)if(r.enabled&&modIsTarget811(r)){
  uint16_t old=modTarget810(r);if(old>=512||fullMap931[old]<0)return false;
  modRouteKey931(r,fullMap931[old]);
 }
 return true;
}
FLASHMEM bool modReadMeta931(File&f){
 for(uint16_t old=0;old<512;++old){ModLegacyMeta931 m{};if(f.read((uint8_t*)&m,sizeof(m))!=sizeof(m))return false;
  if(fullMap931[old]>=0){auto&dest=fullMeta806[fullMap931[old]];dest={0,m.midiInputs};for(uint8_t tr=0;tr<16;++tr)if(m.patterns[tr])dest.tracks|=1u<<tr;}}
 return true;
}
FLASHMEM bool modReadExtra931(File&f){
 memset(fullExtra810,0,sizeof(fullExtra810));
 for(uint16_t old=0;old<512;++old){ModExtra810 e{};if(f.read((uint8_t*)&e,sizeof(e))!=sizeof(e))return false;
  if(fullMap931[old]>=0)fullExtra810[fullMap931[old]]=e;}
 return true;
}
FLASHMEM bool modWriteUnits931(File&f){
 for(uint16_t old=0;old<512;++old){ModUnit805 m{};m.rate=384;uint16_t key=old/2;
  if(key<64&&modUnits805[key].enabled&&modKinds931[key]==old%2){m=modUnits805[key];for(auto&r:m.outputs)if(r.enabled&&modIsTarget811(r)){uint16_t target=modTarget810(r);if(target>=64||!modUnits805[target].enabled){Serial.printf("FULL,INVALID,MOD_ROUTE,SOURCE=%u,TARGET=%u\n",key,target);return false;}modRouteKey931(r,modDiskKey931(target));}}
  for(auto&r:m.outputs)if(!r.enabled)r={};
  if(f.write((uint8_t*)&m,sizeof(m))!=sizeof(m))return false;}
 return true;
}
FLASHMEM bool modWriteMeta931(File&f){
 for(uint16_t old=0;old<512;++old){ModLegacyMeta931 m{};uint16_t key=old/2;
  if(key<64&&modUnits805[key].enabled&&modKinds931[key]==old%2){m.midiInputs=modMeta806[key].midiInputs;for(uint8_t tr=0;tr<16;++tr)m.patterns[tr]=(modMeta806[key].tracks&(1u<<tr))?0xffff:0;}
  if(f.write((uint8_t*)&m,sizeof(m))!=sizeof(m))return false;}
 return true;
}
FLASHMEM bool modWriteExtra931(File&f){
 for(uint16_t old=0;old<512;++old){ModExtra810 e{};uint16_t key=old/2;
  if(key<64&&modUnits805[key].enabled&&modKinds931[key]==old%2)e=modExtra810[key];
  if(f.write((uint8_t*)&e,sizeof(e))!=sizeof(e))return false;}
 return true;
}
FLASHMEM bool modWriteLocks931(File&f){
 for(uint16_t i=0;i<LOCK_CAPACITY817;++i){auto l=noteLocks817[i];if(i<lockCount817&&modLock931(l.kind)){if(l.target>=64||!modUnits805[l.target].enabled){lockDiagnostic45("WRITE_TARGET",i,l);return false;}l.target=modDiskKey931(l.target);}
  if(f.write((uint8_t*)&l,sizeof(l))!=sizeof(l))return false;}
 return true;
}
