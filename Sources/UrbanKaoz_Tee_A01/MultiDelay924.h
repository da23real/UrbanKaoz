#pragma once
// Four sparse track-owned objects. Pattern is ignored for configuration lookup.
uint16_t delayKey924(uint8_t t,uint8_t,uint8_t s){return t*16+s*256;}
FLASHMEM bool delayEnabled924(uint8_t t,uint8_t,uint8_t s){return t<16&&s<4&&(delayMask925[linkOwner12(t,LINK_DLY12)]&(1u<<s));}
FLASHMEM int delayField925(const DelayValues907&d,uint8_t t,uint8_t f){return f==0?d.wait:f==1?d.length:f==2?d.repeats:f==3?d.velocity:f==4?(d.output==16?t:d.output):f==5?d.rolls:f==6?d.note:f==7?(d.scdo==-128?0:d.scdo):f==8?d.mode:f==9?d.accel:f==10?d.axp:f==11?d.oxp:f==12?d.source:f==13?d.chain:f==15?d.gat08:f==16?d.gex08:f==17?d.block21:d.enabled;}
FLASHMEM int delayBaseSlot924(uint8_t t,uint8_t p,uint8_t s,uint8_t f){return indBase50(t,p<16?p:indRuntimePattern50(t),LOCK_DELAY817,t*16+s*256,f,delayField925(delayGet907(t*16+s*256),t,f));}
FLASHMEM void delayNotify925(uint8_t t,uint8_t s,const DelayValues907&old,bool created){if(modOwnerTrack805==t&&(modUi805==MOD_FX806||(modUi805==MOD_DELAY806&&delaySlot924==s)))delayMenuDirty817=true;if(created){delayChanged925(t,s,254,0);return;}auto now=delayGet907(t*16+s*256);if(memcmp(now.targets55,old.targets55,sizeof(now.targets55))){delayChanged925(t,s,255,0);return;}for(uint8_t f=0;f<18;++f)if(delayField925(now,t,f)!=delayField925(old,t,f))delayChanged925(t,s,f,delayField925(now,t,f));}
FLASHMEM bool delayAssign924(uint8_t t,uint8_t,uint8_t s,uint8_t f,int v){auto d=delayGet907(t*16+s*256);
if(!delayPresent925(t,s))d.enabled=0;
if(f==0)d.wait=v;
else if(f==1)d.length=v;
else if(f==2)d.repeats=v;
else if(f==3){d.velocity=v;
if(d.mode==1)d.fixedVelocity926=v;
else d.relativeVelocity926=v;
}else if(f==4)d.output=v;
else if(f==5)d.rolls=v;
else if(f==6)d.note=v;
else if(f==7)d.scdo=v;
else if(f==8){if(d.mode==1)d.fixedVelocity926=d.velocity;
else if(d.mode==0)d.relativeVelocity926=d.velocity;
d.mode=v;
if(v==1)d.velocity=d.fixedVelocity926;
else if(v==0)d.velocity=d.relativeVelocity926;
}else if(f==9)d.accel=v;
else if(f==10)d.axp=v;
else if(f==11)d.oxp=v;
else if(f==12)d.source=v;
else if(f==13)d.chain=v;
else if(f==14)d.enabled=v;
else if(f==15)d.gat08=v;
else if(f==16)d.gex08=v;
else if(f==17)d.block21=v;
else return false;
return delaySet907(t*16+s*256,d);
}
FLASHMEM bool delayAdd924(uint8_t t,uint8_t){for(uint8_t s=0;s<4;++s)if(!delayPresent925(t,s)){if(!delaySet907(t*16+s*256,delayDefault928(s)))return false;delayChosen925[t]=delaySlot924=s;return true;}DISPLAY_SERIAL.println("MSG|4 DELAYS MAX");return false;}
FLASHMEM void delayRemove924(uint8_t t,uint8_t,uint8_t s){delayRemove907(t*16+s*256);for(uint8_t k=s+1;k<4;++k)if(delayPresent925(t,k)){auto d=delayGet907(t*16+k*256);if(d.chain&(1u<<s)){d.chain&=~(1u<<s);delaySet907(t*16+k*256,d);}}}
FLASHMEM void delayToggle925(uint8_t t,uint8_t s){if(!delayPresent925(t,s))delaySet907(t*16+s*256,delayDefault928(s));else delayAssign924(t,0,s,14,!delayEnabled924(t,0,s));}
FLASHMEM uint16_t delayList924(uint8_t t,uint8_t,uint32_t*out,uint16_t cap){uint16_t n=0;for(uint8_t owner=0;owner<16;++owner)for(uint8_t s=0;s<4;++s)if(delayPresent925(owner,s)&&(owner==t||delayBaseSlot924(owner,0,s,4)==t)&&n<cap)out[n++]=owner*16+s*256;return n;}
FLASHMEM void delaySelect924(uint16_t key){delayIncrementShown925=0;modOwnerTrack805=(key%256)/16;modOwnerPattern805=0;delaySlot924=min((uint8_t)(key/256),(uint8_t)3);delayChosen925[modOwnerTrack805]=delaySlot924;}
FLASHMEM void delayChainClick924(){}
FLASHMEM void delayChainTurn924(int){}
FLASHMEM void delayChainDisplay924(const char*&,char*){}
FLASHMEM void delayReset925(){delayClear907(delayNodes907);delayClear907(delayStage907);memset(delayChosen925,0,sizeof(delayChosen925));delayImportPending925=false;pidiProject925();}
FLASHMEM void delayImport925(){
 if(!delayImportPending925)return;
 struct RestoreLinks12{uint8_t mask[16];RestoreLinks12(){memcpy(mask,linkMask12,16);memset(linkMask12,0,16);}~RestoreLinks12(){memcpy(linkMask12,mask,16);}} restoreLinks;
 static EXTMEM DelayValues907 values[16][4];for(auto&row:values)for(auto&v:row)v=DelayValues907{};uint8_t present[16]={};
 for(uint8_t t=0;t<16;++t){uint8_t p=delayProjectFile925?0:activePattern[t];
  if(!delayProjectFile925&&delays805[t][p].enabled){auto&d=values[t][0];d.length=delays805[t][p].ticks;d.repeats=delays805[t][p].repeats;d.enabled=delays805[t][p].enabled;d.wait=delayExtra817[t][p].wait;d.output=delayExtra817[t][p].output;d.rolls=delayExtra817[t][p].rolls;d.edited930=1;present[t]|=1; // Legacy files have no reliable unedited marker.
}
  for(auto*n=delayNodes907;n;n=n->next)if((n->key%256)/16==t&&n->key%16==p&&n->key/256<4){uint8_t s=n->key/256;if(!delayProjectFile925&&!s){memcpy(&values[t][0],&n->value,7);}else values[t][s]=n->value;if(delayProjectFile925||s)present[t]|=1u<<s;}
 }
 delayClear907(delayNodes907);
 for(uint8_t t=0;t<16;++t)for(uint8_t s=0;s<4;++s)if(present[t]&(1u<<s))delaySet907(t*16+s*256,values[t][s]);
 delayImportPending925=false;applyDelayDefaults932();pidiProject925();
}
FLASHMEM void delayIncrement925(uint8_t f){if(f>1)return;toggleTimeUnit38();delayIncrementShown925=3;delayMenuDirty817=modDisplayDirty810=displayValueDirty=true;}

FLASHMEM void delayMarkWorked930(uint8_t t,uint8_t s){
 if(t>=16||s>=4||(delayWorkedMask930[t]&(1u<<s)))return;
 auto d=delayGet907(t*16+s*256);if(!delayPresent925(t,s))d.enabled=0;
 d.edited930=1;delaySet907(t*16+s*256,d);
}
FLASHMEM void delaySourceToggle930(uint8_t t,uint8_t s){
 if(!s||s>=4||t>=16)return;
 delayAssign924(t,0,s,12,!delayGet907(t*16+s*256).source);
 delayMenuDirty817=modDisplayDirty810=displayValueDirty=neoDisplayDirty=true;
}

