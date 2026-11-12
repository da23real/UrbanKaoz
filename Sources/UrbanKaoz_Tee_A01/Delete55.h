#pragma once
static int8_t deletePending55=-1;static uint32_t deleteHash55=0;static int deleteValue55=0;static bool deletePlx55=false;
FLASHMEM void deleteCancel55(){if(deletePending55<0)return;deletePending55=-1;DISPLAY_SERIAL.println("DEF55,-1");displayValueDirty=true;}
FLASHMEM bool deleteDescribe55(uint8_t ky,NumericValue917&d){bool old=deleteButtonActive;deleteButtonActive=false;bool ok=numericDescribe917(ky,d);deleteButtonActive=old;return ok;}
FLASHMEM int deleteDefault55(const NumericValue917&d){
 if(d.kind==N917_PATCH)return 65535;
 if(d.kind==N917_NOTE)return d.field==EDIT_PITCH?60:d.field==EDIT_VEL?100:d.field==EDIT_GATE?96:0;
 if(d.kind==N917_MOD){if(d.lock==LOCK_RAT817)return 8;if(d.lock==LOCK_MOD817){if(modKind931(d.object))return d.param==5?1:384;return d.param==1?1:d.param==2?10:d.param==3?80:d.param==4?20:0;}return 0;}
 if(d.kind==N917_DELAY){auto v=delayDefault928(d.slot);int value=delayField925(v,d.t,d.field);return value+((d.field==4||d.field==17)?1:0);}
 if(d.kind==N917_DTARGET55){auto v=delayDefault928(d.slot);auto&a=v.targets55[d.object];if(a.kind!=d.param)return 0;return d.field?a.exp:a.kind==7?v.note:a.step;}
 if(d.kind==N917_VOLUME)return 90;
 return clampInt(0,d.lo,d.hi);
}
FLASHMEM bool deleteKey55(uint8_t ky,bool pressed){
 if(!deleteButtonActive||recordDelete51||ky>=16)return false;
 if(!pressed)return true;
 NumericValue917 d;if(!deleteDescribe55(ky,d)){deleteCancel55();return true;}
 deleteNumeric55=true;
 uint32_t hash=numericHash917(d);bool plx=!patternMode&&lockWantsEdit817()&&(d.kind==N917_TARGET||d.kind==N917_LFOENV27||d.kind==N917_BLOCK||d.kind==N917_MOD||d.kind==N917_DELAY||d.kind==N917_PATCH||d.kind==N917_ROLLX);
 if(deletePending55==ky&&hash==deleteHash55&&plx==deletePlx55){
  int value=deleteValue55;deleteCancel55();
  if(plx)deleteEncoderLocks15(ky);
  else{bool old=deleteButtonActive;deleteButtonActive=false;if(d.kind==N917_PATCH)d.hi=65535;numericApply917(d,value);deleteButtonActive=old;}
  return true;
 }
 deletePending55=ky;deleteHash55=hash;deletePlx55=plx;deleteValue55=plx?d.base:deleteDefault55(d);
 char text[8];if(plx||deleteValue55==65535)strcpy(text,"-");else snprintf(text,8,"%d",deleteValue55);DISPLAY_SERIAL.printf("DEF55,%u,%s\n",ky,text);return true;
}
FLASHMEM void deleteService55(){if(deletePending55<0)return;NumericValue917 d;if(!deleteButtonActive||!deleteDescribe55(deletePending55,d)||numericHash917(d)!=deleteHash55)deleteCancel55();}
