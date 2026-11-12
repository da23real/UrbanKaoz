#pragma once
FLASHMEM const char* modModeName07(uint8_t mode){const char*names[]={"NOT","PAT","FRE","BAR","BEA","CC converter","ROL"};return names[min(mode,(uint8_t)6)];}
FLASHMEM uint8_t modNextMode07(uint8_t mode,int delta,bool lfo){
 int next=(int)mode+(delta>0?1:-1);if(!lfo&&next==2)next+=delta>0?1:-1;
 if(lfo&&next==5)next+=delta>0?1:-1;return constrain(next,0,6);
}
FLASHMEM void modSetMode07(uint16_t key,uint8_t mode){
 auto&c=ccEnv941[key];auto&m=mod806Unit(key);
 if(mode==5&&!c.enabled){m.hold=m.attack=m.decay=m.release=0;m.sustain=127;for(auto&r:m.outputs)r.depth=0;ccSeen941[key]=false;}
 if(modExtra810[key].mode!=mode)freeClock48[key]={};c.enabled=mode==5;modExtra810[key].mode=mode;modPresetDirty07[key]=true;mod805Invalidate();
}
FLASHMEM float ccEnvelope07(const ModUnit805&m,uint16_t key){
 if(!ccSeen941[key])return 0;float value=ccLevel941(key);
 // Neutral converter avoids envelope calculation entirely.
 if(!m.hold&&!m.attack&&!m.decay&&!m.release&&m.sustain==127)return value;
 auto e=modExtra810[key];float tick=max(0.001f,tickDurationMs());
 float age=(uint32_t)(millis()-ccBeginMs07[key])/tick;
 if(value)return value*held810(age,m,e);
 float tail=(uint32_t)(millis()-ccEndMs07[key])/tick;
 if(!m.release||tail>=m.release)return 0;
 float gate=(uint32_t)(ccEndMs07[key]-ccBeginMs07[key])/tick;
 return ccTail07[key]*held810(gate,m,e)*(1-curve810(tail/m.release,e.releaseCurve));
}
