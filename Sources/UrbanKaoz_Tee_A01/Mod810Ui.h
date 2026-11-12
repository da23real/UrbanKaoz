#pragma once
#include "ModTarget06.h"
FLASHMEM uint8_t modMode810(uint16_t key){return lockValue817(LOCK_EXTRA817,key,3,modExtra810[key].mode);}
FLASHMEM int modLimit810(uint8_t kind){
  if(kind==MOD_CV46)return 4095;if(kind==MOD_PARAM811)return 32767;if(kind==MOD_TRACK21)return 16383;
  if(kind==MOD_PBN||kind==MOD_MIDI_BEND806)return 8192;
  if(kind==MOD_GAT||kind==MOD_VIRTUAL||kind==MOD_DEPTH810)return 8191;
  if(kind==MOD_TIM||kind==MOD_RSP)return 96;if(kind==MOD_RST)return 192;
  if(kind==MOD_RAC)return 128;if(kind==MOD_REX||kind==MOD_RCV)return 100;return 127;
}
FLASHMEM ModRoute805 modRouteDraft810(const int*f,const ModRoute805&base){
  if(f[0]==5){auto r=base;r.kind=MOD_CV46;r.track=clampInt(f[1],0,3);r.port=r.channel=r.number=r.block21=0;r.patch21=r.param21=0;r.reference=base.kind==MOD_CV46?clampInt(r.reference,0,4095):0;for(const auto&m:modUnits805)for(const auto&q:m.outputs)if(q.enabled&&q.kind==MOD_CV46&&q.track==r.track)r.reference=q.reference;r.depth=clampInt(r.depth,-4095,4095);return r;}
  if(f[0]==4)return modTrackDraft21(f,base);
  auto r=base;if(r.block21==3)r.block21=0;r.track=clampInt(f[1],0,15);r.number=clampInt(f[3],0,127);
  if(f[0]==0)r.kind=clampInt(f[2],0,MOD_RLV);
  if(f[0]==1)r.kind=f[2]==MOD_MIDI_BEND806?MOD_MIDI_BEND806:MOD_CC;
  if(f[0]==2){r.kind=MOD_PATCH810;r.number=clampInt(f[2],0,127);}
  if(f[0]==3){r.kind=f[2]?MOD_DEPTH810:MOD_PARAM811;uint16_t key=clampInt(f[1],0,511);r.track=key/32;r.number=(key%32)*8+clampInt(f[3],0,7);}
  if(r.kind==MOD_CC||r.kind==MOD_PATCH810||r.kind==MOD_MIDI_BEND806){r.reference=clampInt(r.reference,r.kind==MOD_MIDI_BEND806?-8192:0,r.kind==MOD_MIDI_BEND806?8191:127);}else{r.port=0;r.channel=0;r.reference=0;}
  if(f[0]==1){if(ioVirtual53(base)){r.block21=4;r.port=1;r.channel=r.track;}else{r.block21=3;r.port=0;r.channel=0;}}
  else if(r.kind==MOD_CC||r.kind==MOD_PATCH810||r.kind==MOD_MIDI_BEND806){if(!r.port){int value=0;const char*label="";modTargetBase06(r,0,value,label);r.reference=value;r.channel=modResolve810(r).track;r.port=firstOut12(r.kind==MOD_PATCH810?r.track:modOwnerTrack805)+1;}else if(r.kind!=MOD_PATCH810)r.channel=r.track;}
  int limit=ccEnv941[modSelected806].enabled?90:modLimit810(r.kind);r.depth=clampInt(r.depth,-limit,limit==8192?8191:limit);return r;
}
FLASHMEM bool modRouteFields810(bool commit,uint8_t roy,int32_t delta){
  if(roy>3)return false;auto&route=mod806Unit(modSelected806).outputs[modOutputCursor805];
  if(!commit&&roy==3&&modFields806[0]!=3&&modFields806[0]!=4&&(modFields806[0]!=1||modFields806[2]==MOD_MIDI_BEND806))return true;
  if(delta){
    if(roy==0){int previous=modFields806[0];
      const uint8_t order[]={2,3,1,4,5,0};int last38=ioKey38(modSelected806)?5:4;int ordinal=0;while(ordinal<last38&&order[ordinal]!=previous)++ordinal;
      modFields806[0]=order[WRAP23(ordinal,delta,0,last38)];
      if(previous!=modFields806[0]){modFields806[1]=modFields806[0]==5?0:modOwnerTrack805;modFields806[2]=modFields806[0]==1?MOD_CC:0;
        if(modFields806[0]==2)modFields806[2]=modPatchNext810(modOwnerTrack805,255,1);
        if(modFields806[0]==3){for(uint16_t k=0;k<64;++k)if(k!=modSelected806&&mod806Unit(k).enabled){modFields806[1]=k;break;}}
        modDirty806|=6;
      }
    }
    if(roy==1){if(modFields806[0]==3){int step=delta>0?1:-1,next=modFields806[1]+step;while(next>=0&&next<64&&(next==modSelected806||!mod806Unit(next).enabled))next+=step;if(next>=0&&next<64)modFields806[1]=next;}
      else modFields806[1]=clampInt(modFields806[1]+delta,0,modFields806[0]==5?3:15);
      if(modFields806[0]==2){modFields806[2]=modPatchNext810(modFields806[1],modFields806[2],0);modDirty806|=4;}
    }
    if(roy==2){if(modFields806[0]==5){}else if(modFields806[0]==4){int count=0;uint8_t block;while(count<48&&blockPatchAt21(modFields806[1],count,block))++count;modFields806[2]=WRAP23(modFields806[2],delta,0,max(0,count-1));modFields806[3]=0;}else if(modFields806[0]==0)modFields806[2]=WRAP23(modFields806[2],delta,0,MOD_RLV);
      else if(modFields806[0]==1)modFields806[2]=delta>0?MOD_MIDI_BEND806:MOD_CC;
      else if(modFields806[0]==2)modFields806[2]=modPatchNext810(modFields806[1],modFields806[2],delta);
      else {modFields806[2]=WRAP23(modFields806[2],delta,0,1);modFields806[3]=0;}
    }
    if(roy==3){
      if(modFields806[0]==4){uint8_t block;auto*d=blockPatchAt21(modFields806[1],modFields806[2],block);modFields806[3]=WRAP23(modFields806[3],delta,0,max(0,(int)patchEditableCount44(d)-1));}
      else if(modFields806[0]==3)modFields806[3]=modFields806[2]?modExistingOutput811(modFields806[1],modFields806[3],delta):WRAP23(modFields806[3],delta,0,modParamCount811(modFields806[1])-1);
      else modFields806[3]=WRAP23(modFields806[3],delta,0,127);
    }
    if(modFields806[0]==3){if(modFields806[2])modFields806[3]=modExistingOutput811(modFields806[1],modFields806[3],0);else modFields806[3]=clampInt(modFields806[3],0,modParamCount811(modFields806[1])-1);}
    modDirty806|=1u<<roy;
  }
  if(commit){
    // Type + dependent fields form one atomic routing choice; a partial type change
    // must never turn a track index into an unrelated modulation identity.
    int candidate[4];memcpy(candidate,modFields806,sizeof(candidate));
    auto next=modRouteDraft810(candidate,route);next.enabled=1;
    bool firstCv48=next.kind==MOD_CV46;for(const auto&q:mod806Unit(modSelected806).outputs)if(q.enabled&&q.kind==MOD_CV46)firstCv48=false;
    if(candidate[0]==4&&!next.patch21){DISPLAY_SERIAL.println("MSG|ASSIGN PATCH TO BLK");return true;}
    if(next.kind<=MOD_RLV&&!ioKey38(modSelected806)){DISPLAY_SERIAL.println("MSG|TRACK MOD RETIRED");return true;}
    if(next.kind==MOD_PATCH810){const auto&ctx=patch808Context(next.track,patch808PlayingPattern(next.track));bool found=false;
      for(auto q=patchNext819(ctx,nullptr);q;q=patchNext819(ctx,q))if(!q->kind&&q->number==next.number)found=true;
      if(!found){DISPLAY_SERIAL.println("MSG|ASSIGN A CC PATCH");return true;}
    }
    if(modIsTarget811(next)){auto target=modTarget810(next);
      if(!mod806Unit(target).enabled||(next.kind==MOD_DEPTH810&&(candidate[3]<0||!mod806Unit(target).outputs[next.number%8].enabled))||(next.kind==MOD_PARAM811&&next.number%8>=modParamCount811(target))){DISPLAY_SERIAL.println("MSG|SELECT EXISTING OUT");return true;}
    }
    if(!modAcyclic810(modSelected806,modOutputCursor805,next)){DISPLAY_SERIAL.println("MSG|MOD LOOP REFUSED");return true;}
    route=next;modAdding806=false;if(firstCv48)modRat808[modSelected806]=32;memcpy(modSavedFields806,candidate,sizeof(candidate));
    modDirty806=0;
    mod805Invalidate();
  }return true;
}
FLASHMEM void modRouteRender810(char*a,char fields[4][12],char*c){
  const int*f=modFields806;auto r=modRouteDraft810(f,mod806Unit(modSelected806).outputs[modOutputCursor805]);
  if(f[0]==5){strcpy(a,"Type Output         ");strcpy(fields[0],"CV");snprintf(fields[1],12,"CV%u%s",f[1]+1,cvShared50(f[1],modSelected806,modOutputCursor805)?"*":"");strcpy(c,"Global CV output");return;}
  if(f[0]==4){strcpy(a,"Type Track Patch Par");modTrackRender21(f,fields,c);return;}
  strcpy(a,f[0]==3?"Type Mod Cible Choix":"Type Track Object CC");
  strcpy(fields[0],f[0]==0?"BAS":f[0]==1?"MIDI":f[0]==2?"PATCH":"MOD");
  if(f[0]==3){
    modName07(f[1],fields[1],12);strcpy(fields[2],f[2]?"Depth":"Para");
    if(!f[2]){snprintf(fields[3],12,"%.6s",modParamName811(f[1],f[3]));snprintf(c,21,"Parameter: %s",modParamName811(f[1],f[3]));}
    else if(f[3]<0){strcpy(fields[3],"None");strcpy(c,"No output created");}
    else {const auto&dest=mod806Unit(f[1]).outputs[f[3]];modRouteLabel810(dest,fields[3]);snprintf(c,21,"Depth: %.10s",fields[3]);}
  }
  else {snprintf(fields[1],12,"%02u",f[1]+1);
    snprintf(fields[2],12,"%.5s",(r.kind==MOD_PATCH810||r.kind==MOD_CC)?modMidiLabel10(r,true):mod806ShortTarget(r.kind));
    if(r.kind==MOD_CC||r.kind==MOD_PATCH810)snprintf(fields[3],12,"%03u",r.number);
    snprintf(c,21,"%.17s%s",(r.kind==MOD_PATCH810||r.kind==MOD_CC)?modMidiLabel10(r,false):mod806TargetName(r.kind),mod806Conflict(r)?" !":"");
  }
}
FLASHMEM void modRouteLabel810(const ModRoute805&r,char*out){
  if(r.kind==MOD_CV46){unsigned count=0;for(const auto&m:modUnits805)if(m.enabled)for(const auto&q:m.outputs)if(q.enabled&&q.kind==MOD_CV46&&q.track==r.track)++count;snprintf(out,11,"CV%u%s",r.track+1,count>1?"*":"");return;}
  if(modTrackMidi43(r)){snprintf(out,11,r.kind==MOD_CC?"T%02u CC%03u":"T%02u Bend",r.track+1,r.number);return;}
  if(ioVirtual53(r)){snprintf(out,11,"V%u CC%u",r.channel+17,r.number);return;}if(r.kind==MOD_TRACK21){snprintf(out,11,"T%02u B%u",r.track+1,r.block21+1);return;}
  if(modIsTarget811(r)){uint16_t key=modTarget810(r);char name[8];modName07(key,name,8);snprintf(out,11,"%.6s %.3s",name,r.kind==MOD_DEPTH810?"Dep":modParamName811(key,r.number%8));}
  else if(r.kind==MOD_PATCH810)snprintf(out,11,"%.3s T%02u%s",modPatchName810(r.track,r.number,true),r.track+1,mod806Conflict(r)?"!":"");
  else if(r.kind==MOD_CC){const char*label=modMidiLabel10(r,true);if(strcmp(label,"MIDI"))snprintf(out,11,"M%02u %.3s",r.track+1,label);else snprintf(out,11,"M%02u CC%03u",r.track+1,r.number);}
  else snprintf(out,11,"%.4s %02u%s",mod806ShortTarget(r.kind),r.track+1,mod806Conflict(r)?"!":"");
}
FLASHMEM void modDuration810(char*out,uint16_t value,int8_t curve){
  char time[8];if(value>9999)snprintf(time,8,"%uk",value/1000);else snprintf(time,8,"%u",value);
  musical937(time,value);snprintf(out,16,"%-4s%+4d",time,curve);
}

FLASHMEM bool delayQuickDisplay925(){
 const char*units[]={"fin","\x14","\x13","\x12"};uint8_t t=modOwnerTrack805,slot=delaySlot924;
 for(uint8_t screen=0;screen<4;++screen){uint16_t blink11=0;char a[17],b[17];memset(a,' ',16);memset(b,' ',16);a[16]=b[16]=0;
  for(uint8_t col=0;col<4;++col){uint8_t k=screen*4+col,f=delayQvFields925[k];char value[12];int v=0;
   char targetLabel55[5];if(delayTargetCell55(k,targetLabel55,value)){memcpy(a+col*4,targetLabel55,strlen(targetLabel55));memcpy(b+col*4,value,min((size_t)4,strlen(value)));continue;}
   if(k==14){memcpy(a+col*4,"Cut",3);const char*v=(delayCut12&(1u<<linkOwner12(t,LINK_DLY12)))?"ON":"OFF";memcpy(b+col*4,v,strlen(v));continue;}if(k>15)continue;memcpy(a+col*4,delayQvNames925[k],strlen(delayQvNames925[k]));
   if(!delayPresent925(t,slot)){strcpy(value,"--");}
   else {v=delayValue907(t,0,f,true);if(lockDifferent11(LOCK_DELAY817,delayKey924(t,0,slot),f,delayBaseSlot924(t,0,slot,f)))blink11|=15u<<(col*4);
    if(remixMode&&remixSelection911.steps&&k<12){auto&m=remixSelection911.role==0?remixSource911:remixDraft911;for(uint32_t n=0;n<m.notes.size;++n)if(remixSelection911.contains(m.notes[n],m,remixSelection911.role==0)){int at=m.find(m.notes[n].id,rmx911::Delay,100+slot*32+f);if(at>=0)v=m.properties[at].value-4096;break;}}
    if(f<2&&(delayIncrementShown925&(1u<<f)))strcpy(value,units[delayTimeMode817[f]%4]);
    else if(f==5)strcpy(value,v?"On":"Off");else if(f==8)strcpy(value,v==1?"Fix":v==2?"Old":"+/-");
    else snprintf(value,sizeof(value),(f==3||f==6||f==7||f==9||f==15)?"%+d":"%d",(f==4||f==17)?v+1:v);
   }if(f<2&&!(delayIncrementShown925&(1u<<f)))musical937(value,v);if(k<12){const char*label=delayLabel926(f,v,delayQvNames925[k]);memset(a+col*4,' ',4);memcpy(a+col*4,label,min((size_t)4,strlen(label)));}memcpy(b+col*4,value,min((size_t)4,strlen(value)));
  }
  lockBlinkSend11(screen,blink11);
  if(strcmp(modPreviousLcd806[screen][0],a)||strcmp(modPreviousLcd806[screen][1],b)){strcpy(modPreviousLcd806[screen][0],a);strcpy(modPreviousLcd806[screen][1],b);DISPLAY_SERIAL.printf("MODLCD|%u|%s|%s\n",screen,a,b);}
 }return true;
}
FLASHMEM void modDepthDisplay50(){
 if(modSelected806>=64)return;
 char top[17],bottom[17],name[16],part[16];memset(top,' ',16);memset(bottom,' ',16);top[16]=bottom[16]=0;
 modName07(modSelected806,name,sizeof(name));memcpy(top,"DEP",3);memcpy(top+11,name,min((size_t)5,strlen(name)));if(modPresetDirty07[modSelected806]&&top[11]!=' ')top[11]|=0x80;
 if(modDepthSlot806>=0){
  auto&r=mod806Unit(modSelected806).outputs[modDepthSlot806];int base=0;const char*label="REF";
  bool hasBase=modTargetBase06(r,0,base,label);memcpy(top+8,label,min((size_t)3,strlen(label)));
  int depth=lockDisplay817(LOCK_DEPTH817,modSelected806,modDepthSlot806,r.depth);
  if(ccEnv941[modSelected806].enabled){if(!depth)strcpy(part,"0");else if(abs(depth)==90)snprintf(part,sizeof(part),"%s10",depth<0?"/":"x");else snprintf(part,sizeof(part),"%s%d.%d",depth<0?"/":"x",1+abs(depth)/10,abs(depth)%10);}
  else snprintf(part,sizeof(part),"%+d",depth);
  memcpy(bottom,part,min((size_t)7,strlen(part)));
  if(hasBase){snprintf(part,sizeof(part),"%d",base);memcpy(bottom+8,part,min((size_t)4,strlen(part)));}
  if(r.kind==MOD_CV46){snprintf(part,sizeof(part),"D%u",cvDepthStep48);memcpy(top,part,min((size_t)4,strlen(part)));memcpy(top+8,"REF",3);}
 }
 if(modKind931(modSelected806)){snprintf(part,sizeof(part),"%u",lockDisplay817(LOCK_RAT817,modSelected806,0,modRat808[modSelected806]));memcpy(bottom+12,part,min((size_t)4,strlen(part)));}
 if(strcmp(modPreviousLcd806[2][0],top)||strcmp(modPreviousLcd806[2][1],bottom)){strcpy(modPreviousLcd806[2][0],top);strcpy(modPreviousLcd806[2][1],bottom);DISPLAY_SERIAL.printf("MODLCD|2|%s|%s\n",top,bottom);}
}
FLASHMEM bool modDisplay810(){
  if(!modUi805)return false;indMarks50();if(modUi805==MOD_DELAY806)return delayQuickDisplay925();targetLocked11=false;char value[8][16]={};const char*labels[8]={"","","","","","","",""};
  if(modSelected806<64){auto m=mod806Unit(modSelected806);auto e=modExtra810[modSelected806];uint16_t key=modSelected806;
    if(modKind931(key)){m.rate=lockDisplay817(LOCK_MOD817,key,0,m.rate);m.shape=lockDisplay817(LOCK_MOD817,key,1,m.shape);}
    else {m.hold=max(0,lockDisplay817(LOCK_MOD817,key,0,holdBase50(key,m)));m.attack=lockDisplay817(LOCK_MOD817,key,1,m.attack);m.decay=lockDisplay817(LOCK_MOD817,key,2,m.decay);m.sustain=lockDisplay817(LOCK_MOD817,key,3,m.sustain);m.release=lockDisplay817(LOCK_MOD817,key,4,m.release);}
    e.attackCurve=lockDisplay817(LOCK_EXTRA817,key,0,e.attackCurve);e.decayCurve=lockDisplay817(LOCK_EXTRA817,key,1,e.decayCurve);e.releaseCurve=lockDisplay817(LOCK_EXTRA817,key,2,e.releaseCurve);e.mode=lockDisplay817(LOCK_EXTRA817,key,3,e.mode);e.phase=lockDisplay817(LOCK_EXTRA817,key,4,e.phase);
    if(modDepthSlot806>=0)m.outputs[modDepthSlot806].depth=lockDisplay817(LOCK_DEPTH817,key,modDepthSlot806,m.outputs[modDepthSlot806].depth);
    if(!modType805){const char*names[]={"Hold tk","Attack","Decay","Sustain","Release"};
      for(uint8_t i=0;i<5;++i)labels[i]=names[i];{int hold=lockDisplay817(LOCK_MOD817,key,0,holdBase50(key,m));snprintf(value[0],16,"%d",hold);if(hold>=0)musical937(value[0],hold);}
      modDuration810(value[1],m.attack,e.attackCurve);modDuration810(value[2],m.decay,e.decayCurve);
      snprintf(value[3],16,"%u",m.sustain);modDuration810(value[4],m.release,e.releaseCurve);
    }else{
      labels[0]=lfoMs932(modSelected806)?"Cycle ms":"Cycle tk";labels[1]="Shape";labels[2]="Phase";labels[3]="Mode";
      lfoText932(value[0],m.rate,modSelected806);const char*shape[]={"Triangle","Square","Saw up","Saw down","Sine"};strcpy(value[1],shape[min(m.shape,(uint8_t)4)]);
      snprintf(value[2],16,"%u",e.phase);const char* modes36[]={"NOT","PAT","FRE","BAR","BEA"};strcpy(value[3],modes36[min(e.mode,(uint8_t)4)]);
    }
    labels[7]=modType805&&e.mode==2?"RAT ms":"RAT tk";snprintf(value[7],16,"%u",lockDisplay817(LOCK_RAT817,modSelected806,0,modRat808[modSelected806]));
    if(modDepthSlot806>=0&&m.outputs[modDepthSlot806].enabled){labels[6]="Depth";snprintf(value[6],16,"%+d",m.outputs[modDepthSlot806].depth);}
  }
  if(modUi805==MOD_DELAY806){for(uint8_t i=0;i<8;++i){uint8_t field=delayFields907[i];labels[i]=delayLabels907[i];int v=delayValue907(modOwnerTrack805,modOwnerPattern805,field,true);
   if(field==4)snprintf(value[i],16,"T%02d",v+1);else if(field==5)strcpy(value[i],v?"ON":"OFF");else snprintf(value[i],16,(field==6||field==7||(field==3&&delayValue907(modOwnerTrack805,modOwnerPattern805,8,true)==0))?"%+d":"%d",v);}}
  if(modUi805==MOD_DELAY806&&delayChainView924>=0)delayChainDisplay924(labels[7],value[7]);
  char presetLabel07[16];if(modSelected806<64){modName07(modSelected806,presetLabel07,sizeof(presetLabel07));labels[5]="";strcpy(value[5],presetLabel07);if(modPresetDirty07[modSelected806]&&value[5][0])value[5][0]|=0x80;}
  char targetLabel[16];if(modSelected806<64&&modDepthSlot806>=0){char destination09[12];modRouteLabel810(mod806Unit(modSelected806).outputs[modDepthSlot806],destination09);if(mod806Unit(modSelected806).outputs[modDepthSlot806].kind==MOD_CC)snprintf(targetLabel,16,"Dep C%03u",mod806Unit(modSelected806).outputs[modDepthSlot806].number);else snprintf(targetLabel,16,"Dep %.4s",destination09);labels[6]=targetLabel;int base=0;const char* name="";if(modTargetBase06(mod806Unit(modSelected806).outputs[modDepthSlot806],0,base,name)){snprintf(targetLabel,16,"Dep %.3s",name);labels[6]=targetLabel;snprintf(value[6],16,"%-4d%d",lockDisplay817(LOCK_DEPTH817,modSelected806,modDepthSlot806,mod806Unit(modSelected806).outputs[modDepthSlot806].depth),base);}}
  if(modSelected806<64&&modDepthSlot806>=0&&ccEnv941[modSelected806].enabled){int depth=lockDisplay817(LOCK_DEPTH817,modSelected806,modDepthSlot806,mod806Unit(modSelected806).outputs[modDepthSlot806].depth);char gain[8];if(!depth)strcpy(gain,"0");else if(abs(depth)==90)snprintf(gain,8,"%s10",depth<0?"/":"x");else snprintf(gain,8,"%s%d.%d",depth<0?"/":"x",1+abs(depth)/10,abs(depth)%10);int base;const char* name="";if(modTargetBase06(mod806Unit(modSelected806).outputs[modDepthSlot806],0,base,name))snprintf(value[6],16,"%-4s%d",gain,base);else strcpy(value[6],gain);}
  if(modSelected806<64&&modDepthSlot806>=0&&mod806Unit(modSelected806).outputs[modDepthSlot806].kind==MOD_CV46){snprintf(targetLabel,16,"D%-3uB%-3u",cvDepthStep48,cvBaseStep48);labels[6]=targetLabel;}
  uint16_t masks[4]={};if(modSelected806<64){uint16_t k=modSelected806;auto&m=mod806Unit(k);auto&e=modExtra810[k];
   auto field=[&](uint8_t ky,uint8_t kind,uint8_t param,int base,uint8_t width=4){if(lockDifferent11(kind,k,param,base))masks[ky/4]|=((1u<<width)-1)<<(ky%4*4);};
   if(modKind931(k)){field(0,LOCK_MOD817,0,m.rate,8);field(2,LOCK_MOD817,1,m.shape,8);field(4,LOCK_EXTRA817,4,e.phase,8);field(6,LOCK_EXTRA817,3,e.mode,8);}
   else{field(0,LOCK_MOD817,0,m.hold,8);field(2,LOCK_MOD817,1,m.attack);field(3,LOCK_EXTRA817,0,e.attackCurve);field(4,LOCK_MOD817,2,m.decay);field(5,LOCK_EXTRA817,1,e.decayCurve);field(6,LOCK_MOD817,3,m.sustain,8);field(8,LOCK_MOD817,4,m.release);field(9,LOCK_EXTRA817,2,e.releaseCurve);}
   if(modDepthSlot806>=0)field(12,LOCK_DEPTH817,modDepthSlot806,m.outputs[modDepthSlot806].depth);if(targetLocked11)masks[3]|=0x00F0;field(14,LOCK_RAT817,0,modRat808[k],8);
  }
  if(ioDisplay38())return true;
  if(modType805&&modSelected806<64){
   char depth27[8];snprintf(depth27,8,"DE%u",modDepthSlot806<0?0:modDepthSlot806+1);
   if(lfoMs932(modSelected806)){const char*first=strchr(value[0],':');const char*second=first?strchr(first+1,':'):nullptr;if(first&&second){int lo=lfoDigit932%3==0?second-value[0]+1:lfoDigit932%3==1?first-value[0]+1:0;int hi=lfoDigit932%3==0?strlen(value[0]):lfoDigit932%3==1?second-value[0]:first-value[0];for(int i=lo;i<hi&&i<12;++i)masks[0]|=1u<<i;}}
   char a36[17],b36[17],cycle40[13];snprintf(cycle40,13,lfoMs932(modSelected806)?"ms +%d":"tk +%d",lfoStep932(modSelected806));snprintf(a36,17,"%-12.12sSHP ",cycle40);snprintf(b36,17,"%-12.12s    ",value[0]);DISPLAY_SERIAL.printf("MODLCD|0|%s|%s\n",a36,b36);
   auto shown52=mod806Unit(modSelected806);shown52.hold=indDisplay50(LOCK_MOD817,modSelected806,5,shown52.hold);shown52.sustain=indDisplay50(LOCK_MOD817,modSelected806,6,shown52.sustain);snprintf(b36,17,"%-4.4s%-4.4s%02u  %-3s ",value[2],value[3],max((uint16_t)1,shown52.hold),lfoEnvName36(shown52));DISPLAY_SERIAL.printf("MODLCD|1|FAZ TRG ICH ENV |%s\n",b36);
   DISPLAY_SERIAL.printf("GRAPH36,0,12,0,%u\n",mod806Unit(modSelected806).shape);
   char top[17],bottom[17],wait[8],attack[8],release[8];auto m=mod806Unit(modSelected806);m.decay=lockDisplay817(LOCK_MOD817,modSelected806,2,m.decay);m.attack=lockDisplay817(LOCK_MOD817,modSelected806,3,m.attack);m.release=lockDisplay817(LOCK_MOD817,modSelected806,4,m.release);
   modDepthDisplay50();
   snprintf(wait,sizeof(wait),"%u",m.decay);snprintf(attack,sizeof(attack),"%u",m.attack);snprintf(release,sizeof(release),"%u",m.release);musical937(wait,m.decay);musical937(attack,m.attack);musical937(release,m.release);
   snprintf(bottom,17,"%-4.4s%-4.4s%-4.4s%+4d",wait,attack,release,lockDisplay817(LOCK_EXTRA817,modSelected806,2,modExtra810[modSelected806].releaseCurve));
   char timing36[17]="HOL ATK REL CURV";for(int f=0;f<3;++f)timing36[f*4+3]=fineTimeUnit38()?'f':0x14;
   if(strcmp(modPreviousLcd806[3][0],timing36)||strcmp(modPreviousLcd806[3][1],bottom)){strcpy(modPreviousLcd806[3][0],timing36);strcpy(modPreviousLcd806[3][1],bottom);DISPLAY_SERIAL.printf("MODLCD|3|%s|%s\n",timing36,bottom);}
   DISPLAY_SERIAL.printf("GRAPH36,3,12,2,%d\n",lockDisplay817(LOCK_EXTRA817,modSelected806,2,modExtra810[modSelected806].releaseCurve));lockBlinkSend11(0,masks[0]);lockBlinkSend11(1,masks[1]);lockBlinkSend11(2,(masks[3]&255)|((masks[3]&0xFF00)));lockBlinkSend11(3,0);return true;
  }
  if(!modType805){
   mod806Small(0,labels[0],value[0],labels[1],value[1]);mod806Small(1,labels[2],value[2],labels[3],value[3]);
   modDepthDisplay50();mod806Small(3,labels[4],value[4],labels[7],value[7]);
   DISPLAY_SERIAL.printf("GRAPH36,0,12,1,%d,%u\nGRAPH36,1,4,3,%d,%u\nGRAPH36,3,4,3,%d,%u\n",modExtra810[modSelected806].attackCurve,expEnvMaximum15,modExtra810[modSelected806].decayCurve,expEnvMaximum15,modExtra810[modSelected806].releaseCurve,expEnvMaximum15);lockBlinkSend11(0,masks[0]);lockBlinkSend11(1,masks[1]);lockBlinkSend11(2,(masks[3]&255)|(masks[2]&0xFF00));lockBlinkSend11(3,(masks[2]&255)|(masks[3]&0xFF00));return true;
  }
  for(uint8_t i=0;i<4;++i){mod806Small(i,labels[2*i],value[2*i],labels[2*i+1],value[2*i+1]);lockBlinkSend11(i,masks[i]);}return true;
}
FLASHMEM bool modKy810(uint8_t ky,int32_t delta){
  if(!modUi805)return false;if(!delta)return true;
  if(modUi805==MOD_DELAY806){if(delayTargetsKy55(ky,delta))return true;if(ky==14){uint16_t bit=1u<<linkOwner12(modOwnerTrack805,LINK_DLY12);if(delta>0)delayCut12|=bit;else delayCut12&=~bit;modDisplayDirty810=displayValueDirty=true;return true;}if(ky>=16)return true;uint8_t f=delayQvFields925[ky];
   if(ky<12){if(remixMode&&remixSelection911.steps){int quantum=f<2?(delayTimeMode817[f]?96:1):1;if(f<2)delayIncrementShown925&=~(1u<<f);remixEditDelta911(10+f,delta*quantum,false);}else delayEdit817(f,delta);}
   else if(ky<14)delayEdit817(f,delta);
   return true;}

  if(modSelected806>=64)return true;
  if(ioKey38(modSelected806)){if(ioKy38(ky,delta))return true;ky=ky==8?12:13;}
  if(!modType805&&!ioKey38(modSelected806)){int f=ky==0?0:ky==2?1:ky==4?2:ky==6?3:ky==12?4:-1;if(f>=0&&f!=3)delta*=fineTimeUnit38()?1:96;if(ky==9||ky==11)return true;if(ky==8)ky=12;else if(ky==10)ky=13;else if(ky==12||ky==13)ky-=4;}
  if(modType805&&ky>=12){if(ky<=14)delta*=fineTimeUnit38()?1:96;auto&m=mod806Unit(modSelected806);auto&e=modExtra810[modSelected806];
   if(ky<=14){int base38=ky==12?m.decay:ky==13?m.attack:m.release;if(lockEdit817(LOCK_MOD817,modSelected806,ky-10,base38,delta,0,65535)){modDisplayDirty810=displayValueDirty=true;return true;}}
   if(ky==12)m.decay=clampInt((int)m.decay+delta,0,65535);
   if(ky==13)m.attack=clampInt((int)m.attack+delta,0,65535);
   if(ky==14)m.release=clampInt((int)m.release+delta,0,65535);
   if(ky==15)delta*=curveFine41?1:10;
   if(ky==15&&lockEdit817(LOCK_EXTRA817,modSelected806,2,e.releaseCurve,delta,-100,100)){modDisplayDirty810=displayValueDirty=true;return true;}
   if(ky==15)e.releaseCurve=ZCLAMP57(e.releaseCurve,delta,-100,100);
   modPresetDirty07[modSelected806]=storageDirty931=true;mod805Invalidate();modDisplayDirty810=displayValueDirty=true;return true;
  }
  if(modType805){auto&m=mod806Unit(modSelected806);if(ky==6){if(indEdit50(LOCK_MOD817,modSelected806,5,m.hold,delta,1,99))return true;m.hold=clampInt((int)m.hold+delta,1,99);modPresetDirty07[modSelected806]=storageDirty931=true;mod805Invalidate();displayValueDirty=true;return true;}if(ky==7){auto shown=m;shown.sustain=indDisplay50(LOCK_MOD817,modSelected806,6,m.sustain);if(indEditing59()&&!lfoEnvMode36(shown))return true;if(indEditing59()&&lfoEnvMode36(shown)){lfoEnvSet36(shown,WRAP23(lfoEnvMode36(shown),delta,1,4));if(indEdit50(LOCK_MOD817,modSelected806,6,m.sustain,0,shown.sustain,shown.sustain))return true;}if(lfoEnvMode36(m)){lfoEnvSet36(m,WRAP23(lfoEnvMode36(m),delta,1,4));}modPresetDirty07[modSelected806]=storageDirty931=true;mod805Invalidate();displayValueDirty=true;return true;}if(ky==3)ky=2;else if(ky==5)ky=6;
 if(ky==9)return true;if(ky==8)ky=12;else if(ky==10)ky=13;else if(ky==11)ky=14;}
  if(ky!=13&&ky!=1&&ky!=10&&ky!=11&&ky!=15)modPresetDirty07[modSelected806]=true;if(modType805&&ky==1){lfoIncrement932(delta);return true;}if(modType805&&ky==0)delta*=lfoStep932(modSelected806);auto&m=mod806Unit(modSelected806);auto&e=modExtra810[modSelected806];
  if(ky==13){if(modDepthSlot806>=0&&m.outputs[modDepthSlot806].kind==MOD_CV46)delta*=cvBaseStep48;int base;const char* name="";if(modDepthSlot806>=0&&modTargetBase06(m.outputs[modDepthSlot806],delta,base,name)){mod805Invalidate();storageDirty931=true;modDisplayDirty810=displayValueDirty=true;}return true;}
  uint8_t kind=LOCK_MOD817,param=255;int base=0,lo=0,hi=65535;
  if(ky==14){kind=LOCK_RAT817;param=0;base=modRat808[modSelected806];lo=1;hi=3072;}
  else if(ky==12&&modDepthSlot806>=0){kind=LOCK_DEPTH817;param=modDepthSlot806;base=m.outputs[param].depth;hi=ccEnv941[modSelected806].enabled?90:modLimit810(m.outputs[param].kind);if(ccEnv941[modSelected806].enabled&&!ccFine07[modSelected806])delta*=10;lo=-hi;if(hi==8192)--hi;}
  else if(!modType805){if(ky==0){param=0;base=holdBase50(modSelected806,m);lo=-384;}if(ky==2){param=1;base=m.attack;}if(ky==4){param=2;base=m.decay;}if(ky==6){param=3;base=m.sustain;hi=127;}if(ky==8){param=4;base=m.release;}
    if(ky==3||ky==5||ky==9){delta*=curveFine41?1:10;kind=LOCK_EXTRA817;param=ky==3?0:ky==5?1:2;base=param==0?e.attackCurve:param==1?e.decayCurve:e.releaseCurve;lo=-100;hi=100;}}
  else {if(ky==0){param=0;base=m.rate;lo=1;hi=lfoMs932(modSelected806)?LFO_MAX42:65535;}if(ky==2){param=1;base=m.shape;hi=4;}if(ky==4){kind=LOCK_EXTRA817;param=4;base=e.phase;hi=360;}if(ky==6){kind=LOCK_EXTRA817;param=3;base=e.mode;hi=4;}}
  if(!modType805&&kind==LOCK_MOD817&&param!=255){int quantum=1;int shown=lockDisplay817(kind,modSelected806,param,base);int next=delta>0?(shown/quantum+delta)*quantum:((shown+quantum-1)/quantum+delta)*quantum;delta=clampInt(next,lo,hi)-shown;}
  if(ky==12&&modDepthSlot806>=0&&m.outputs[modDepthSlot806].kind==MOD_MIDI_BEND806)delta*=100;
  if(kind==LOCK_DEPTH817&&param<8&&m.outputs[param].kind==MOD_CV46)delta*=cvDepthStep48;
  if(param!=255&&lockEdit817(kind,modSelected806,param,base,delta,lo,hi)){modDisplayDirty810=true;return true;}
  if(ky==14)modRat808[modSelected806]=WRAP23(modRat808[modSelected806],delta,1,3072);
  else if(ky==12&&modDepthSlot806>=0){auto&r=m.outputs[modDepthSlot806];int limit=ccEnv941[modSelected806].enabled?90:modLimit810(r.kind);r.depth=WRAP23(r.depth,delta,-limit,limit==8192?8191:limit);}
  else if(!modType805){
    if(ky==0)holdSet50(modSelected806,ZCLAMP57(holdBase50(modSelected806,m),delta,-384,65535));
    if(ky==2)m.attack=WRAP23(m.attack,delta,0,65535);
    if(ky==3)e.attackCurve=WRAP23(e.attackCurve,delta,-100,100);
    if(ky==4)m.decay=WRAP23(m.decay,delta,0,65535);
    if(ky==5)e.decayCurve=WRAP23(e.decayCurve,delta,-100,100);
    if(ky==6)m.sustain=WRAP23(m.sustain,delta,0,127);
    if(ky==8)m.release=WRAP23(m.release,delta,0,65535);
    if(ky==9)e.releaseCurve=WRAP23(e.releaseCurve,delta,-100,100);
  }else{
    if(ky==0)m.rate=clampInt((int64_t)m.rate+delta,1,lfoMs932(modSelected806)?LFO_MAX42:65535);
    if(ky==2)m.shape=WRAP23(m.shape,delta,0,4);
    if(ky==4)e.phase=WRAP23(e.phase,delta,0,360);
    if(ky==6){modSetMode07(modSelected806,WRAP23(e.mode,delta,0,4));modSavedTrigger807=modDraftMode810=e.mode;modDirty806&=~4;}
  }
  mod805Invalidate();modDisplayDirty810=true;return true;
}

