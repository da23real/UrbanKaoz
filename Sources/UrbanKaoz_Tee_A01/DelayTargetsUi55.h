#pragma once

// These fields are edited on EP/ROY only. Do not expose an alternate KY path.
FLASHMEM bool delayTargetCell55(uint8_t ky,char*label,char*value){if(ky!=6&&ky!=7&&ky!=9&&ky!=11)return false;label[0]=value[0]=0;return true;}
FLASHMEM uint8_t delayFocus55(){return delayTargetSelected55[modOwnerTrack805][delaySlot924];}
FLASHMEM bool delayTargetWire55(delay55::Target&t,uint8_t owner){
 if(t.kind==8){if(t.track>=16)return false;auto b=blockAtNote21(t.track,0,255,-1,255);if(!b.port)return false;auto&c=blockPatchContext21(t.track,0);auto*q=patchAddress10(c,0,t.number,7);if(!q)return false;uint16_t base=0;patchValue819(c,*q,base);t.base=base==65535?0:base;t.kind=0;t.port=b.port-1;t.channel=b.channel;}
 else if(t.kind==5||t.kind==6){if(t.identity>=64)return false;int id=remoteMap54[t.identity][owner];if(id<0)id=remoteMap54[t.identity][16];if(id<0)return false;t.identity=remoteConfig54[id].identity;}
 return delay55::valid(t);
}
FLASHMEM delay55::Config delayTargetsConfig56(uint8_t track,uint8_t slot){
 delay55::Config c{};c.track=track;c.delay=slot;auto d=delayGet907(track*16+slot*256);
 uint8_t destination=d.output<16?d.output:track;auto route=blockAtNote21(destination,d.block21,255,-1,255);
 for(unsigned i=0;i<4;++i){auto&t=c.targets[i];t=delayPresent925(track,slot)?d.targets55[i]:delay55::Target{};
  if(t.kind==8){auto&ctx=blockPatchContext21(destination,d.block21);auto*q=patchAddress10(ctx,0,t.number,7);uint16_t base=0;if(!q){t.kind=255;continue;}patchValue819(ctx,*q,base);t.base=base==65535?0:base;t.kind=0;}
  if(t.kind<4){if(!route.port){t.kind=255;continue;}t.port=route.port-1;t.channel=route.channel;t.track=destination;}
  if(!delayTargetWire55(t,destination))t.kind=delay55::NONE;
 }
 return c;
}
FLASHMEM bool delayTargetsSend55(uint8_t track,uint8_t slot){auto c=delayTargetsConfig56(track,slot);return pidiPacket827(delay55::CONFIG,&c,sizeof(c));}
FLASHMEM void delayTargetLabel55(const delay55::Target&t,char*out,unsigned size){
 if(t.kind==delay55::NONE){snprintf(out,size,"Add");return;}
 if(t.kind==delay55::NDO){snprintf(out,size,"NDO");return;}
 if(t.kind==9){snprintf(out,size,"GAT");return;}
 if(t.kind==8){auto d=delayGet907(modOwnerTrack805*16+delaySlot924*256);snprintf(out,size,"%s",modPatchName810(d.output<16?d.output:modOwnerTrack805,t.number,true));return;}
 if(t.kind==4){snprintf(out,size,"CV%u",t.number+1);return;}
 if(t.kind>=5){char name[8];modName07(t.identity,name,8);snprintf(out,size,"%.6s %.4s",name,t.kind==6?"DEP":modParamName811(t.identity,t.slot));return;}
 snprintf(out,size,"CC%u",t.number);
}
static void delayCell56(char*row,uint8_t col,const char*text){memcpy(row+col*5,text,min((size_t)5,strlen(text)));}
static uint8_t delayOutputTrack56(const DelayValues907&d){return d.output<16?d.output:modOwnerTrack805;}
FLASHMEM void delayTargetMain56(char*a,char*b){
 auto d=delayGet907(modOwnerTrack805*16+delaySlot924*256);uint8_t i=delayFocus55();char name[24],value[12];int v=0;
 if(i==4){strcpy(name,d.scdo==-128?"-":"Scdo");v=delayXp56?d.oxp:d.scdo==-128?0:d.scdo;}
 else{auto&t=d.targets55[i];delayTargetLabel55(t,name,sizeof(name));v=delayXp56?t.exp:t.kind==7?d.note:t.step;if(t.kind==255)strcpy(name,"-");}
 if(delayXp56)strcpy(name,"XP");snprintf(value,sizeof(value),delayXp56?"%d":i<4&&d.targets55[i].kind==9?"%+d%%":"%+d",v);memset(a+15,' ',5);memset(b+15,' ',5);delayCell56(a,3,name);delayCell56(b,3,value);
}
FLASHMEM void delayTargetsMenu55(){
 auto d=delayGet907(modOwnerTrack805*16+delaySlot924*256);char rows[4][21];for(auto&row:rows){memset(row,' ',20);row[20]=0;}
 if(!delayEditDetail55){
  // Same list convention as MOD: fixed >, encoder browsing, click to edit.
  unsigned first=delayEditSlot55<3?0:delayEditSlot55-2;
  for(unsigned row=0;row<3;++row){unsigned position=first+row;if(position>4)break;unsigned i=position?position-1:4;char label[24];if(i==4)strcpy(label,d.scdo==-128?"Add":"Scdo");else delayTargetLabel55(d.targets55[i],label,sizeof(label));rows[row][0]=position==delayEditSlot55?'>':' ';memcpy(rows[row]+1,label,min((size_t)19,strlen(label)));}
  strcpy(rows[3],"Edit           Del  ");
 }else{
  auto&t=d.targets55[delayEditSlot55];const char*kind=t.kind==9?"GAT":t.kind==8?"Patch":t.kind==7?"NDO":t.kind==4?"CV":t.kind==5?"MOD":t.kind==6?"DEP":"CC";
  delayCell56(rows[0],0,"Type");delayCell56(rows[1],0,kind);
  if(t.kind==5||t.kind==6){char name[8];modName07(t.identity,name,8);delayCell56(rows[0],1,"MOD");delayCell56(rows[1],1,name);delayCell56(rows[0],2,"Param");delayCell56(rows[1],2,t.kind==6?"DEP":modParamName811(t.identity,t.slot));}
  else if(t.kind!=7&&t.kind!=9){char name[24];delayTargetLabel55(t,name,sizeof(name));delayCell56(rows[0],1,"Param");delayCell56(rows[1],1,name);}
  if(t.kind==0||t.kind==4){char value[12];snprintf(value,sizeof(value),"%d",t.base);delayCell56(rows[0],3,"Base");delayCell56(rows[1],3,value);}
  strcpy(rows[3],"Back           Done ");
 }
 sendMenuScreen(rows[0],rows[1],rows[2],rows[3]);
}
FLASHMEM bool delayTargetStore55(DelayValues907&d){
 unsigned count=0;for(auto*n=delayNodes907;n;n=n->next)if(n->key!=modOwnerTrack805*16+delaySlot924*256)for(auto&t:n->value.targets55)count+=t.kind!=255&&t.kind!=7&&t.kind!=9;
 for(auto&t:d.targets55)count+=t.kind!=255&&t.kind!=7&&t.kind!=9;if(count>delay55::ACTIVE){DISPLAY_SERIAL.println("MSG|16 DELAY TARGETS MAX");return false;}
 storageDirty931=true;bool ok=delaySet907(modOwnerTrack805*16+delaySlot924*256,d);displayValueDirty=modDisplayDirty810=true;return ok;
}
FLASHMEM void delayTargetsButton55(uint8_t index){
 if(delayEditDetail55){if(index==4||index==7){delayEditDetail55=false;++delayEditSlot55;delayTargetsMenu55();}return;}
 if(index==4){if(delayEditSlot55==0){auto d=delayGet907(modOwnerTrack805*16+delaySlot924*256);if(d.scdo==-128){d.scdo=0;delayTargetStore55(d);}delayTargetSelected55[modOwnerTrack805][delaySlot924]=4;delayTargetsOpen55=false;mod805Menu();return;}--delayEditSlot55;auto d=delayGet907(modOwnerTrack805*16+delaySlot924*256);if(d.targets55[delayEditSlot55].kind==255){d.targets55[delayEditSlot55]={};d.targets55[delayEditSlot55].kind=7;if(!delayTargetStore55(d))return;}delayTargetSelected55[modOwnerTrack805][delaySlot924]=delayEditSlot55;delayEditDetail55=true;}
 if(index==7){auto d=delayGet907(modOwnerTrack805*16+delaySlot924*256);if(delayEditSlot55==0)d.scdo=-128;else {auto i=delayEditSlot55-1;if(d.targets55[i].kind==7)d.note=0;d.targets55[i]={};}delayTargetStore55(d);}
 delayTargetsMenu55();
}
FLASHMEM void delayTargetsRoy55(uint8_t roy,int delta,bool click){
 if(!delayEditDetail55){if(!roy&&delta)delayEditSlot55=WRAP23(delayEditSlot55,delta,0,4);if(!roy&&click){delayTargetsButton55(4);return;}delayTargetsMenu55();return;}
 if(click&&roy==0){delayEditDetail55=false;++delayEditSlot55;delayTargetsMenu55();return;}if(!delta)return;
 auto d=delayGet907(modOwnerTrack805*16+delaySlot924*256);auto&t=d.targets55[delayEditSlot55];uint8_t track=delayOutputTrack56(d);
 if(!roy){const uint8_t kinds[]={7,8,0,4,5,6,9};int k=0;while(k<6&&kinds[k]!=t.kind)++k;int step=t.step;uint8_t xp=t.exp;t={};t.step=step;t.exp=xp;t.kind=kinds[WRAP23(k,delta,0,6)];t.track=track;if(t.kind==8)t.number=modPatchNext810(track,255,1);if(t.kind==5||t.kind==6){bool found=false;for(unsigned key=0;key<64;++key)if(remoteOwn54(key)){t.identity=key;found=true;break;}if(!found)return;}}
 else if(t.kind==8){if(roy==1)t.number=modPatchNext810(track,t.number,delta);}
 else if(t.kind==0){if(roy==1)t.number=WRAP23(t.number,delta,0,127);if(roy==3)t.base=clampInt((int)t.base+delta,0,127);}
 else if(t.kind==4){if(roy==1)t.number=WRAP23(t.number,delta,0,3);if(roy==3)t.base=clampInt((int)t.base+delta,0,4095);}
 else if(t.kind==5||t.kind==6){if(roy==1){int key=t.identity,step=delta>0?1:-1;for(int n=0;n<64;++n){key=(key+step+64)%64;if(remoteOwn54(key)){t.identity=key;t.slot=0;break;}}}if(roy==2)t.slot=t.kind==5?WRAP23(t.slot,delta,0,max(0,(int)modParamCount811(t.identity)-1)):modExistingOutput811(t.identity,t.slot,delta);}
 if(delayTargetStore55(d))delayTargetsMenu55();
}
FLASHMEM void delayTargetsSelect55(){auto d=delayGet907(modOwnerTrack805*16+delaySlot924*256);auto&current=delayTargetSelected55[modOwnerTrack805][delaySlot924];for(unsigned n=1;n<=5;++n){unsigned i=(current+n)%5;if((i==4&&d.scdo!=-128)||(i<4&&d.targets55[i].kind!=255)){current=i;break;}}delayXp56=false;displayValueDirty=modDisplayDirty810=delayMenuDirty817=true;}
FLASHMEM void delayTargetTurn56(int delta){auto d=delayGet907(modOwnerTrack805*16+delaySlot924*256);auto i=delayFocus55();if(i==4){if(d.scdo==-128)return;if(delayXp56)d.oxp=clampInt((int)d.oxp+delta,0,100);else d.scdo=ZCLAMP57(d.scdo,delta,-127,127);}else{auto&t=d.targets55[i];if(t.kind==255)return;if(delayXp56)t.exp=clampInt((int)t.exp+delta,0,100);else if(t.kind==7)d.note=ZCLAMP57(d.note,delta,-127,127);else t.step=ZCLAMP57(t.step,delta,-32767,32767);}delayTargetStore55(d);delayMenuDirty817=true;}
FLASHMEM bool delayTargetsKy55(uint8_t ky,int){return ky==6||ky==7||ky==9||ky==11;}
