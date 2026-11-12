#pragma once
// Locate the assigned patch owning the selected parameter, without SD access.
FLASHMEM int kbPatchSlot03(uint8_t t,uint32_t parameter){auto&c=patch808Context(t,activePattern[t]);for(uint8_t i=0;i<16;++i){auto*d=patch808Find(patchDb808,patchId819(c,i));for(auto*q=d?d->params:nullptr;q;q=q->next)if(q->kind!=2&&q->id==parameter)return i;}return -1;}
FLASHMEM void kbCycle03(uint8_t t,KbZoneR10&z,bool advance){
 if(z.type==KB_AH_R10){auto&c=patch808Context(t,activePattern[t]);int current=kbPatchSlot03(t,z.target);if(!advance&&current>=0)return;for(int step=1;step<=16;++step){int slot=(current+step)%16;auto*d=patch808Find(patchDb808,patchId819(c,slot));auto*q=d?d->params:nullptr;while(q&&q->kind==2)q=q->next;if(q){if(slot!=current)z.target=q->id;return;}}}
 else if(z.type==KB_MOD_R10){int current=z.target<64&&mod806Unit(z.target).enabled?(int)z.target:-1;if(!advance&&current>=0)return;for(int step=1;step<=64;++step){int id=(current+step)%64;if(mod806Unit(id).enabled){z.target=id;z.parameter=kbModNext06(id,z.parameter,0);return;}}}
 else if(z.type==KB_DLY_R10){int current=z.target<4&&delayPresent925(t,z.target)?(int)z.target:-1;if(!advance&&current>=0)return;for(int step=1;step<=4;++step){int id=(current+step)%4;if(delayPresent925(t,id)){z.target=id;return;}}}
}
FLASHMEM void kbChooseTargetR10(uint8_t t,KbZoneR10&z,int delta){
 kbCycle03(t,z,false);
 if(z.type==KB_AH_R10){int slot=kbPatchSlot03(t,z.target);if(slot<0)return;auto&c=patch808Context(t,activePattern[t]);auto*d=patch808Find(patchDb808,patchId819(c,slot));uint32_t count=0,index=0;for(auto*q=d->params;q;q=q->next)if(q->kind!=2){if(q->id==z.target)index=count;++count;}if(!count)return;int wanted=clampInt((int)index+delta,0,(int)count-1);for(auto*q=d->params;q;q=q->next)if(q->kind!=2&&wanted--==0){z.target=q->id;break;}}
 else if(z.type==KB_MOD_R10)z.parameter=kbModNext06(min(z.target,63UL),z.parameter,delta);
 else if(z.type==KB_DLY_R10)z.parameter=clampInt((int)z.parameter+delta,0,11);
 else if(z.type==KB_CC_R10)z.target=clampInt((int)z.target+delta,0,127);
}
FLASHMEM bool kbZoneContextR11(){return recordContext820()&&kbZoneViewR11&&!quickView903;}
FLASHMEM bool kbKyR10(uint8_t ky,int delta,bool click){if(!kbZoneContextR11())return false;for(uint8_t i=0;i<4;++i)lockBlinkSend11(i,0);uint8_t t=keyboardTrack823(),zone=ky/4,field=ky%4;if(!kbActiveZoneR10(t,zone))return true;auto&z=kbSettings13(t).zone[zone];
 if(click){bool object=z.type==KB_AH_R10||z.type==KB_MOD_R10||z.type==KB_DLY_R10;if(field==0&&object){kbCycle03(t,z,true);z.valid=0;}else if(field==(object?1:0))z.mute=!z.mute;}
 if(delta){if(field==0){z.type=clampInt((int)z.type+delta,0,4);z.parameter=0;z.target=0;z.valid=0;kbChooseTargetR10(t,z,0);}
  if(field==1){kbChooseTargetR10(t,z,delta);z.valid=0;}
  int lo,hi;kbBoundsR10(t,z,lo,hi);if(field==2)kbCenterFor12(t,zone)=ZCLAMP57(kbCenterFor12(t,zone),delta,lo,hi);if(field==3)z.range=ZCLAMP57(z.range,delta,-(hi-lo),hi-lo);z.center=clampInt(z.center,lo,hi);
 }int low,high;kbBoundsR10(t,z,low,high);z.center=clampInt(z.center,low,high);storageDirty931=true;displayValueDirty=neoDisplayDirty=true;kbDisplayR10();return true;}
FLASHMEM bool kbKeyR10(uint8_t ky,bool down){if(!kbZoneContextR11())return false;bool click=!down&&kyBtnDown[ky]&&!kyRotatedWhileDown[ky];kyBtnDown[ky]=down;if(down)kyRotatedWhileDown[ky]=false;if(click)kbKyR10(ky,0,true);return true;}
// Four three-character fields, one per KY, matching the usual LCD grid.
FLASHMEM void kbNumber03(char*out,int32_t value){if(value>=1000)snprintf(out,8,"%ldk%ld",(long)(value/1000),(long)((value%1000)/100));else if(value<=-100)snprintf(out,8,"-%ldh",(long)(-value/100));else snprintf(out,8,"%ld",(long)value);}
FLASHMEM bool kbDisplayR10(){if(!kbZoneContextR11())return false;for(uint8_t i=0;i<4;++i)lockBlinkSend11(i,0);uint8_t t=keyboardTrack823();const char*types[]={"NOT","AH","MOD","DLY","CC"};const char*delay[]={"Wai","Len","Rep","Vel","Out","Rol","Not","Cnd","Mod","Acc","Axp","Oxp"};
 for(uint8_t id=0;id<4;++id){char a[17]="                ",b[17]="                ";auto&z=kbSettings13(t).zone[id];if(kbActiveZoneR10(t,id)){
  char object[8]="",precision[8]="",center[8],range[8];const char*label="";
  if(z.type==KB_CC_R10){auto*q=patchAddress10(patch808Context(t,activePattern[t]),0,z.target,7);label=q?q->shortName:"CC#";kbNumber03(precision,z.target);}
  else if(z.type==KB_AH_R10){auto*q=kbPatchR10(t,z.target);int slot=kbPatchSlot03(t,z.target);if(q&&slot>=0){label=patchLabel10(patch808Context(t,activePattern[t]),*q,true);kbNumber03(object,slot+1);}}
  else if(z.type==KB_MOD_R10&&z.target<64&&mod806Unit(z.target).enabled){label=kbModName06(z.target,z.parameter);if(z.parameter>=8)kbNumber03(precision,z.parameter-7);kbNumber03(object,z.target+1);}
  else if(z.type==KB_DLY_R10&&z.target<4&&delayPresent925(t,z.target)){label=delay[min(z.parameter,(uint8_t)11)];kbNumber03(object,z.target+1);}
  kbNumber03(center,kbCenterFor12(t,id));kbNumber03(range,z.range);
  snprintf(a,17,"%-3.3s %-3.3s Ctr Rng ",types[z.type],label);
  snprintf(b,17,"%-3.3s %-3.3s %-3.3s %-3.3s ",object,precision,center,range);
 }if(strcmp(modPreviousLcd806[id][0],a)||strcmp(modPreviousLcd806[id][1],b)){strcpy(modPreviousLcd806[id][0],a);strcpy(modPreviousLcd806[id][1],b);DISPLAY_SERIAL.printf("MODLCD|%u|%-16.16s|%-16.16s\n",id,a,b);}}return true;
}
FLASHMEM void kbOpenR10(bool track){kbMenuTrackR10=track;kbMenuR10=track?3:1;kbRenderMenuR10();}
// Track opens zoning directly. J1 right opens Select; left returns.
FLASHMEM void kbRenderMenuR10(){char a[21];auto&k=kbSettings13(selectedTrack);
 if(kbMenuR10==3){snprintf(a,21,"%-10s%s",k.split==0?"Normal":k.split==1?"2 zones":"4 zones",k.panels?"Neo":"Lines");sendMenuScreen(a,"","","");}
 else if(kbMenuR10==2){snprintf(a,21,"Select %s",kbSelectHearR10?"ON":"OFF");sendMenuScreen(a,"","","");}
 else sendMenuScreen("","","Select","");
}
FLASHMEM bool kbRoyR10(uint8_t r,int d,bool click){if(!kbMenuR10)return false;
 if(kbMenuR10==3){auto&k=kbSettings13(selectedTrack);if(r==0&&(d||click))k.split=click?(k.split+1)%3:clampInt((int)k.split+d,0,2);if(r==2&&(d||click))k.panels=click?!k.panels:d>0;}
 else if(kbMenuR10==2){if(r==0&&(d||click)){kbSelectHearR10=click?!kbSelectHearR10:d>0;kbPrefsSaveR10();}}
 else if(click&&r==0)kbMenuR10=2;
 storageDirty931=true;neoDisplayDirty=true;kbRenderMenuR10();return true;
}
FLASHMEM bool kbScreenR10(uint8_t i){if(!kbMenuR10)return false;if(kbMenuR10==1&&i<4)return kbRoyR10(i,0,true);return true;}
FLASHMEM bool kbBackR10(){if(!kbMenuR10)return false;if(kbMenuR10==2){kbMenuR10=kbMenuTrackR10?3:1;kbRenderMenuR10();}else{kbMenuR10=0;if(kbMenuTrackR10)sendTrackMenu();else sendSettingsPage();}return true;}
