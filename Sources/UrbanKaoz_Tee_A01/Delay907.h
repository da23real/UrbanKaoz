#pragma once
int delayField925(const DelayValues907&,uint8_t,uint8_t);
FLASHMEM DelayValues907 delayFactory932(uint8_t slot){
 DelayValues907 d;d.length=96u<<min(slot,(uint8_t)3);d.wait=slot==0?192:slot==1?96:0;
 d.accel=-1;d.scdo=-5;d.axp=d.oxp=d.gex08=expDefault15[1];d.velocity=d.relativeVelocity926;return d;
}
FLASHMEM DelayValues907 delayDefault928(uint8_t slot){return slot<4&&(delayDefaultsMask932&(1u<<slot))?delayDefaults932[slot]:delayFactory932(slot);}
FLASHMEM DelayValues907 delayGet907(uint16_t key){uint8_t t=(key%256)/16,s=key/256;t=linkOwner12(t,LINK_DLY12);if(s>=4)return {};auto*n=delayIndex925[t][s];return n?n->value:delayDefault928(s);}
FLASHMEM void delayClear907(DelayNode907*&head){while(head){auto*n=head;head=n->next;extmem_free(n);}if(&head==&delayNodes907){memset(delayIndex925,0,sizeof(delayIndex925));memset(delayMask925,0,sizeof(delayMask925));memset(delayWorkedMask930,0,sizeof(delayWorkedMask930));neoDisplayDirty=true;}}
FLASHMEM bool delayPresent925(uint8_t t,uint8_t s){return t<16&&s<4&&delayIndex925[linkOwner12(t,LINK_DLY12)][s];}
FLASHMEM void delayRemove907(uint16_t key){uint8_t t=(key%256)/16,s=key/256;t=linkOwner12(t,LINK_DLY12);if(s>=4)return;key=t*16+s*256;auto**n=&delayNodes907;while(*n){if((*n)->key==key){auto*old=*n;*n=old->next;extmem_free(old);break;}n=&(*n)->next;}delayIndex925[t][s]=nullptr;delayMask925[t]&=~(1u<<s);delayWorkedMask930[t]&=~(1u<<s);neoDisplayDirty=true;delayChanged925(t,s,255,0);}
FLASHMEM bool delaySet907(uint16_t key,DelayValues907 v){
 for(auto&a:v.targets55)if(!delayTargetValid55(a))return false;
 uint8_t t=linkOwner12((key%256)/16,LINK_DLY12),s=key/256;
 if(key>=1024||v.gat08==-128||v.gex08>100||v.edited930>1||v.fixedVelocity926<0||v.relativeVelocity926==-128||v.velocity==-128||v.note==-128||v.mode>2||v.accel==-128||v.axp>100||v.oxp>100||!v.length||v.length>3072||v.wait>3072||v.repeats>16||v.output>16||v.block21>2||v.rolls>1||v.enabled>1||v.source>1)return false;
 v.chain&=(1u<<s)-1;if(!s)v.source=1;auto*n=delayIndex925[t][s];bool created=!n;DelayValues907 old=n?n->value:DelayValues907{};
 // Compare only on an actual edit, never in the render or playback loop.
 DelayValues907 baseline=created?delayDefault928(s):old, candidate=v;
 baseline.enabled=candidate.enabled;baseline.edited930=candidate.edited930;
 if(!delayImportPending925&&memcmp(&baseline,&candidate,sizeof(v)))v.edited930=1;
 if(!created)v.edited930|=old.edited930;
 if(!n){n=(DelayNode907*)extmem_malloc(sizeof(DelayNode907));if(!n)return false;*n={delayNodes907,(uint16_t)(t*16+s*256),v};delayNodes907=n;delayIndex925[t][s]=n;}else n->value=v;
 if(v.edited930)delayWorkedMask930[t]|=1u<<s;else delayWorkedMask930[t]&=~(1u<<s);neoDisplayDirty=true;
 if(v.enabled)delayMask925[t]|=1u<<s;else delayMask925[t]&=~(1u<<s);
 if(!s)for(uint8_t pattern=0;pattern<16;++pattern){delays805[t][pattern]={v.length,v.repeats,127,v.enabled};delayExtra817[t][pattern]={v.wait,v.output,v.rolls};}
 delayNotify925(t,s,old,created);return true;
}
// Pattern operations must never copy/delete track-wide Delay configuration.
FLASHMEM bool delayCopy907(uint16_t,uint16_t){return true;}
FLASHMEM int delayMin907(uint8_t f){return f==1?1:(f==3||f==6||f==7||f==9||f==15)?-127:0;}
FLASHMEM int delayMax907(uint8_t f){return f==17?2:f==10||f==11||f==16?100:f<2?3072:f==2?16:f==4?15:(f==5||f==8)?1:127;}
FLASHMEM int delayBase907(uint8_t t,uint8_t p,uint8_t f){return delayBaseSlot924(t,p,0,f);}
FLASHMEM int delayValue907(uint8_t t,uint8_t p,uint8_t f,bool display){uint8_t s=display?delaySlot924:0;int base=display?delayField925(delayGet907(t*16+s*256),t,f):delayBaseSlot924(t,p,s,f);uint16_t key=delayKey924(t,p,s);int v=display?lockDisplay817(LOCK_DELAY817,key,f,base):base;int lo=delayMin907(f);if(f==3&&delayBaseSlot924(t,p,s,8)==1)lo=0;return clampInt(v,lo,f==8?2:delayMax907(f));}
FLASHMEM void delayMode907(){uint8_t t=modOwnerTrack805,p=modOwnerPattern805;int old=delayValue907(t,p,8,true),next=old==0?1:0;if(!lockEdit817(LOCK_DELAY817,delayKey924(t,p,delaySlot924),8,old,next-old,0,1))delayAssign924(t,p,delaySlot924,8,next);++modEpoch805;delayMenuDirty817=modDisplayDirty810=true;}
const char* delayLabel926(uint8_t f,int v,const char* fallback){return f==9?(v<0?"Accl":v>0?"Dccl":"Rglr"):f==7?(v<0?"Dndo":v>0?"Cndo":"Rglr"):fallback;}
FLASHMEM void delayMenu907(char*a,char*b,char*c,char*d){
 memset(a,' ',20);memset(b,' ',20);memset(c,' ',20);memset(d,' ',20);a[20]=b[20]=c[20]=d[20]=0;uint8_t t=modOwnerTrack805,p=modOwnerPattern805;
 const char*units[]={"fin","\x14","\x13","\x12"};
 for(uint8_t i=0;i<4;++i){if(i==1)continue;uint8_t slot=i*2+delayToggle907[i],f=delayRoyField21(i);char value[12];int v=delayValue907(t,p,f,true);
  if(f<2&&(delayIncrementShown925&(1u<<f)))snprintf(value,sizeof(value),"%s",units[delayTimeMode817[f]%4]);else snprintf(value,sizeof(value),(f==9||f==7||(f==3&&delayValue907(t,p,8,true)==0))?"%+d":"%d",f==17?v+1:v);
  const char*label=delayLabel926(f,v,f==17?"BLK":delayRoyLabels923[slot]);memcpy(a+i*5,label,strlen(label));memcpy(b+i*5,value,min((size_t)5,strlen(value)));}
 char cells[8][12];snprintf(cells[0],12,"V:%s",delayValue907(t,p,8,true)==1?"Fix":"+/-");strcpy(cells[1],"Wait");strcpy(cells[2],"TRK");strcpy(cells[3],"EDIT");
 snprintf(cells[4],12,"R:%s",delayValue907(t,p,5,true)?"On":"Off");strcpy(cells[5],"Leng");snprintf(cells[6],12,"%02d",delayValue907(t,p,4,true)+1);strcpy(cells[7],"SEL");
 for(uint8_t i=0;i<8;++i)memcpy((i<4?c:d)+(i%4)*5,cells[i],min((size_t)5,strlen(cells[i])));
 // Both OB values stay visible. Existing B20/B19 choices and OB increment click remain.
 memcpy(a+5,"Wait ",5);memcpy(c+5,"Leng ",5);memset(b+5,' ',5);memset(d+5,' ',5);
 for(uint8_t field=0;field<2;++field){char value[12];
  if(delayIncrementShown925&(1u<<field))snprintf(value,sizeof(value),"%s",units[delayTimeMode817[field]%4]);
  else snprintf(value,sizeof(value),"%d",delayValue907(t,p,field,true));
  if(!(delayIncrementShown925&(1u<<field)))musical937(value,delayValue907(t,p,field,true));
  char* row=field?d:b;memcpy(row+5,value,min((size_t)5,strlen(value)));
  if(field==(delayToggle907[1]?0:1)){uint8_t ch=row[5];
   if(ch>='0'&&ch<='9')row[5]=(char)(0x80+ch-'0');
   else if(ch=='f')row[5]=(char)0x8A;
   else if(ch>=0x12&&ch<=0x14)row[5]=(char)(0x90+ch-0x12);
  }
 }
 delayTargetMain56(a,b);
}
