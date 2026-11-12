#pragma once
FLASHMEM uint8_t kbModNext06(uint16_t key,uint8_t current,int delta){
 uint8_t ids[16],count=0;for(uint8_t i=0;i<(modKind931(key)?4:5);++i)ids[count++]=i;
 for(uint8_t i=0;i<8;++i)if(mod806Unit(key).outputs[i].enabled)ids[count++]=8+i;
 int index=0;while(index<count&&ids[index]!=current)++index;if(index==count)index=0;
 return ids[WRAP23(index,delta,0,count-1)];
}
FLASHMEM const char* kbModName06(uint16_t key,uint8_t parameter){
 static const char*lfo[]={"Cycle","Shape","Phase","Mode"};
 return parameter>=8?"Dep":modKind931(key)?lfo[min(parameter,(uint8_t)3)]:modParamName811(key,parameter);
}
FLASHMEM void kbModBounds06(uint16_t key,uint8_t parameter,int&lo,int&hi){
 lo=0;hi=65535;
 if(parameter>=8){hi=ccEnv941[key].enabled?90:modLimit810(mod806Unit(key).outputs[parameter-8].kind);lo=-hi;if(hi==8192)--hi;}
 else if(modKind931(key)){lo=parameter?0:1;hi=parameter==0?LFO_MAX42:parameter==1?4:parameter==2?360:4;}
 else if(parameter==3)hi=127;
}
