#pragma once
// A small input draft, not copies of the selected notes. Rendering is pure.
struct DraftTurn23 {int32_t delta;uint32_t count;uint8_t unit;};
static EXTMEM DraftTurn23 draftTurns23[EDIT_MODE_COUNT+6][32];
static uint8_t draftCount23[EDIT_MODE_COUNT+6]={};
FLASHMEM bool draftAdd23(int mode,int32_t delta){
 auto&n=draftCount23[mode];auto*list=draftTurns23[mode];uint8_t unit=mode==EDIT_GATE?gateEditMode:0;
 if(n&&list[n-1].delta==delta&&list[n-1].unit==unit){++list[n-1].count;return true;}
 if(n==32)return false;list[n++]={delta,1,unit};return true;
}
FLASHMEM int draftValue23(int mode,int value){
 int lo=mode==EDIT_VEL?1:mode==EDIT_TIME?MICRO_TICK_MIN:mode==EDIT_GATE?GATE_TICK_MIN:0;
 int hi=mode==EDIT_TIME?MICRO_TICK_MAX:mode==EDIT_GATE?GATE_TICK_MAX:127;
 if(mode>=4){lo=clampRollFieldValue(mode-4,-32768);hi=clampRollFieldValue(mode-4,32767);}
 int prior=0,held=0;
 for(uint8_t i=0;i<draftCount23[mode];++i){auto&s=draftTurns23[mode][i];int dir=s.delta>0?1:-1;if(dir!=prior)held=0;prior=dir;uint32_t count=s.count;
  if(mode==4){int64_t next=(int64_t)value+(int64_t)s.delta*count;value=next<lo?lo:next>hi?hi:(int)next;continue;}
  if(mode==EDIT_GATE){
   if(s.unit==GATE_EDIT_MUSICAL){for(uint32_t n=0;n<min(count,(uint32_t)GATE_MUSICAL_COUNT+2);++n)value=nextMusicalGate(value,dir);}
   else {int64_t next=(int64_t)value+(int64_t)dir*(s.unit==GATE_EDIT_24T?TICKS_PER_STEP:1)*count;value=next<lo?lo:next>hi?hi:(int)next;}
   continue;
  }
  if(mode==EDIT_GATE&&s.unit==GATE_EDIT_MUSICAL){
   const uint32_t cycle=GATE_MUSICAL_COUNT+3+(GATE_MUSICAL_VALUES[0]>lo)+(GATE_MUSICAL_VALUES[GATE_MUSICAL_COUNT-1]<hi);
   while(count){int edge=dir>0?hi:lo;if(value==edge){uint32_t until=4-held;if(count<until){held+=count;break;}count-=until;held=0;value=dir>0?lo:hi;count%=cycle;}
    else{value=nextMusicalGate(value,dir);held=0;--count;}}
   continue;
  }
  int inc=mode==EDIT_GATE?(s.unit==GATE_EDIT_24T?TICKS_PER_STEP:1):abs(s.delta);
  uint32_t cycle=(hi-lo+inc-1)/inc+4;
  while(count){int edge=dir>0?hi:lo;uint32_t distance=(abs(edge-value)+inc-1)/inc;
   if(distance){uint32_t take=min(count,distance);value=clampInt(value+(int64_t)dir*inc*take,lo,hi);count-=take;held=0;if(!count)break;}
   uint32_t until=4-held;if(count<until){held+=count;break;}count-=until;held=0;value=dir>0?lo:hi;count%=cycle;
  }
 }
 return value;
}
