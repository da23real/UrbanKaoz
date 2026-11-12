#pragma once
extern bool remixScheduleDirty911;
static RollNode1000* rollStage1000=nullptr;
FLASHMEM void rollClear1000(RollNode1000*&head){while(head){auto*n=head;head=n->next;extmem_free(n);}}
FLASHMEM void rollDefault15(uint8_t track){
 auto**at=&rollNodes1000;while(*at&&(*at)->track!=track)at=&(*at)->next;
 if(*at){auto*old=*at;*at=old->next;extmem_free(old);}
 if(expDefault15[0]==33)return;
 auto*n=(RollNode1000*)extmem_malloc(sizeof(RollNode1000));if(!n)return;
 *n={rollNodes1000,track,{}};n->value.v[0]=expDefault15[0];rollNodes1000=n;
}
FLASHMEM void rollReset1000(){for(auto&w:rollWindows44)w={};rollHoldUs44=0;rollClear1000(rollNodes1000);for(uint8_t t=0;t<16;t++)rollDefault15(t);}
FLASHMEM int rollMin1000(uint8_t f){return f==2||f==3||f==5?-127:0;}
FLASHMEM int rollStop09(const TrackRollSettings&r,const RollExtra1000&x){return clampInt(x.v[6],0,max(0,(int)r.start-1));}
FLASHMEM int rollMax1000(uint8_t f){return f==7?2:f==6?1535:f==0?102:f==1||f==4?1:127;}
FLASHMEM RollExtra1000 rollExtra1000(uint8_t t,uint8_t p,int32_t tick,uint8_t note){
 Scope830 rollMeasure30(P830_ROLL30);RollExtra1000 x;for(auto*n=rollNodes1000;n;n=n->next)if(n->track==t){x=n->value;break;}
 for(uint8_t f=0;f<8;++f)x.v[f]=indBase50(t,p<16?p:indRuntimePattern50(t),LOCK_ROLL1000,t,f,x.v[f]);
 if(p<16&&tick>=-48&&note<128&&playbackLocks828)for(uint8_t f=0;f<8;++f)x.v[f]=lockValue817(LOCK_ROLL1000,t,f,x.v[f]);
 if(p<16&&tick>=-48&&note<128&&playbackLocks828)for(int i=lockEventFirst30(t,p,tick,note);i>=0;i=lockEventNext30(i)){auto&l=lockPlaybackEntry39(i);if(l.kind==LOCK_ROLL1000&&l.track==t&&l.pattern==p&&l.tick==tick&&l.note==note&&l.parameter<8)x.v[l.parameter]=l.value;}
 return x;
}
FLASHMEM int rollVelocity1000(const TrackRollSettings&r,const RollExtra1000&x,uint8_t original,uint16_t index){
 int32_t velocity=(x.v[1]?clampInt((int)original+x.v[2],0,127):r.level)*256,delta=r.curve*256;
 if(x.v[0]>=101){unsigned cycles=index/8,remain=index%8;int32_t sum=0,lo=0,hi=127*256;for(unsigned i=0;i<8;++i){int32_t d=rollWave60(delta,x.v[0],i);sum+=d;lo=clampInt(lo+d,0,127*256);hi=clampInt(hi+d,0,127*256);}if(cycles){int32_t low=sum>0?min(hi,lo+(int32_t)(cycles-1)*sum):lo,high=sum<0?max(lo,hi+(int32_t)(cycles-1)*sum):hi;velocity=clampInt(velocity+(int32_t)cycles*sum,low,high);}for(unsigned i=0;i<remain;++i)velocity=clampInt(velocity+rollWave60(delta,x.v[0],i),0,127*256);}
 else for(uint16_t i=0;i<index;++i){velocity=clampInt(velocity+delta,0,127*256);if((velocity==0&&delta<=0)||(velocity==127*256&&delta>=0))break;int64_t mag=abs(delta);mag=min((int64_t)127*256,(mag*(expFactor14(x.v[0]))+128)/256);delta=delta<0?-mag:mag;}

 return clampInt((velocity+128)/256,0,127);
}
FLASHMEM int rollPitch1000(const RollExtra1000&x,uint8_t original,uint16_t index){return clampInt(x.v[4]?x.v[3]:(int)original+x.v[3]+index*x.v[5],0,127);}
FLASHMEM void rollEdit1000(uint8_t f,int delta){
 if(f>=8)return;uint8_t t=selectedTrack;auto x=rollExtra1000(t);int lo=rollMin1000(f),hi=f==6?max(0,(int)displayedRollForTrack(t).start-1):rollMax1000(f);if(f==3&&lockDisplay817(LOCK_ROLL1000,t,4,x.v[4]))lo=0;
 if(f==0&&!rollModeChange60){int current=lockDisplay817(LOCK_ROLL1000,t,0,x.v[0]);if(current>=101)delta=(current==101?102:101)-current;}
 if(!lockEdit817(LOCK_ROLL1000,t,f,x.v[f],delta,lo,hi)){
  x.v[f]=WRAP23(x.v[f],delta,lo,hi);auto**at=&rollNodes1000;while(*at&&(*at)->track!=t)at=&(*at)->next;
  bool used=false;RollExtra1000 defaults;for(uint8_t i=0;i<8;++i)used|=x.v[i]!=defaults.v[i];
  if(!used){if(*at){auto*n=*at;*at=n->next;extmem_free(n);}}
  else{if(!*at){*at=(RollNode1000*)extmem_malloc(sizeof(RollNode1000));if(!*at){DISPLAY_SERIAL.println("MSG|ROLL MEMORY FULL");return;}**at={nullptr,t,{}};}(*at)->value=x;}
 }
 mod805Invalidate();displayValueDirty=true;
}
FLASHMEM void rollClick1000(uint8_t f){auto x=rollExtra1000(selectedTrack);int current=lockDisplay817(LOCK_ROLL1000,selectedTrack,f,x.v[f]);if(f==4&&!current){int pitch=lockDisplay817(LOCK_ROLL1000,selectedTrack,3,x.v[3]);if(pitch<0)rollEdit1000(3,-pitch);}rollEdit1000(f,current?-1:1);}
FLASHMEM bool rollWrite1000(File&f){uint8_t count=192;for(auto*n=rollNodes1000;n;n=n->next)++count;if(f.write(&count,1)!=1)return false;for(auto*n=rollNodes1000;n;n=n->next)if(f.write(&n->track,1)!=1||f.write((uint8_t*)&n->value,sizeof(n->value))!=sizeof(n->value))return false;return true;}
FLASHMEM bool rollRead1000(File&f,bool present){rollClear1000(rollStage1000);
if(!present)return true;
uint8_t count=0;
uint16_t seen=0;
if(f.read(&count,1)!=1)return false;bool extended=count&128;bool version44=count&64;count&=63;if(count>16)return false;
while(count--){uint8_t t;
RollExtra1000 x;
if(f.read(&t,1)!=1||t>=16||(seen&(1u<<t))||f.read((uint8_t*)&x,version44?sizeof(x):extended?14:12)!=(version44?sizeof(x):extended?14:12))return false;
seen|=1u<<t;
for(uint8_t i=0;
i<8;
++i)if(x.v[i]<rollMin1000(i)||x.v[i]>rollMax1000(i))return false;
auto*n=(RollNode1000*)extmem_malloc(sizeof(RollNode1000));
if(!n)return false;
*n={rollStage1000,t,x};
rollStage1000=n;
}return true;
}
FLASHMEM void rollInstall1000(){for(auto&w:rollWindows44)w={};rollHoldUs44=0;rollClear1000(rollNodes1000);rollNodes1000=rollStage1000;rollStage1000=nullptr;}
FLASHMEM bool rollQuickEdit1000(uint8_t ky,int delta){
 if(ky==1){int current=lockDisplay817(LOCK_ROLL1000,selectedTrack,8,rollDir939[selectedTrack]);uint8_t order[]={0,2,1};int at=current==0?0:current==2?1:2,next=order[WRAP23(at,delta,0,2)];if(!lockEdit817(LOCK_ROLL1000,selectedTrack,8,rollDir939[selectedTrack],0,next,next))rollDir939[selectedTrack]=next;mod805Invalidate();remixScheduleDirty911=true;neoDisplayDirty=displayValueDirty=true;return true;}
 if(ky==3){rollEdit1000(7,delta);return true;}
 if(ky==7){rollEdit1000(6,delta);return true;}
 if(ky==12){rollEdit1000(0,delta);return true;}
 if(ky==13)return true;
 if(ky==14){rollEdit1000(5,delta);return true;}
 if(ky==15){rollEdit1000(3,delta);return true;}
 if(ky==11){rollEdit1000(1,delta>0?1:-1);return true;}
 if(ky==10&&lockDisplay817(LOCK_ROLL1000,selectedTrack,1,rollExtra1000(selectedTrack).v[1])){rollEdit1000(2,delta);return true;}
 return false;
}
FLASHMEM void rollQuickDisplay1000(){
 auto r=displayedRollForTrack(selectedTrack);auto x=rollExtra1000(selectedTrack);
 for(uint8_t f=0;f<8;++f)x.v[f]=lockDisplay817(LOCK_ROLL1000,selectedTrack,f,x.v[f]);
 const char*labels[16]={"RST","DIR","RSP","PLX","RAC","TIM","SEX","RTP","RCV","VEL","RVL","TYP","VEX","","ROT","NOS"};
 for(uint8_t screen=0;screen<4;++screen){char a[17],b[17];memset(a,' ',16);memset(b,' ',16);a[16]=b[16]=0;
  for(uint8_t col=0;col<4;++col){uint8_t k=rollKey26(screen*4+col);char value[12]={};memcpy(a+col*4,labels[k],strlen(labels[k]));
   if(k<10&&!(k&1))snprintf(value,sizeof(value),"%d",rollFieldValue(r,k/2));
   else if(k==3)strcpy(value,x.v[7]==2?"MOD":x.v[7]==1?"ON":"OFF");
   else if(k==5||k==9)snprintf(value,sizeof(value),"%d",blockNoteDisplay21(k==5?3:1));
   else if(k==1)strcpy(value,lockDisplay817(LOCK_ROLL1000,selectedTrack,8,rollDir939[selectedTrack])==0?"Pre":lockDisplay817(LOCK_ROLL1000,selectedTrack,8,rollDir939[selectedTrack])==1?"Post":"Sym");
   else if(k==7)snprintf(value,sizeof(value),"%d",rollStop09(r,x));
   else if(k==10)snprintf(value,sizeof(value),x.v[1]?"%+d":"%d",x.v[1]?x.v[2]:r.level);
   else if(k==11)strcpy(value,x.v[1]?"+/-":"Fix");
   else if(k==12)snprintf(value,sizeof(value),"%d",x.v[0]);
   else if(k==13)value[0]=0;
   else if(k==14)snprintf(value,sizeof(value),"%+d",x.v[5]);
   else if(k==15)snprintf(value,sizeof(value),x.v[4]?"%d":"%+d",x.v[3]);
   if(k==0||k==2)musical937(value,rollFieldValue(r,k/2));if(k==0)a[col*4+3]=rollFine60?'f':'s';if((k==6&&r.expo>=101)||(k==12&&x.v[0]>=101))strcpy(value,(k==6?r.expo:x.v[0])==101?"/v/v":"/_/_");
   memcpy(b+col*4,value,min((size_t)4,strlen(value)));
  }DISPLAY_SERIAL.printf("MODLCD|%u|%s|%s\n",screen,a,b);
  uint16_t mask=0;auto base=rollExtra1000(selectedTrack);const int8_t field[16]={-1,-1,-1,7,-1,-1,-1,6,-1,-1,2,1,0,-1,5,3};for(uint8_t col=0;col<4;++col){int f=field[rollKey26(screen*4+col)];if(f>=0&&lockDifferent11(LOCK_ROLL1000,selectedTrack,f,base.v[f]))mask|=15u<<(col*4);}lockBlinkSend11(screen,mask);
  if(screen==1&&x.v[0]>=101)DISPLAY_SERIAL.printf("GRAPH36,1,4,4,%d\n",x.v[0]-101);if(screen==2&&r.expo>=101)DISPLAY_SERIAL.printf("GRAPH36,2,8,4,%d\n",r.expo-101);
 }
}



FLASHMEM void rollEvent44(uint8_t t,const TrackRollSettings&r,const RollExtra1000&x,int32_t offset,int32_t identity,uint32_t gate,uint8_t direction){
 rollHoldUs44=0;if(!x.v[7]||!r.start)return;
 int32_t last=-r.start,pos=last,spacing=clampInt(r.speed,1,96),accel=(int32_t)r.accel*256;
 for(uint16_t i=0;i<MAX_PREROLL_NOTES&&pos< -rollStop09(r,x);++i){last=pos;if(spacing>=-rollStop09(r,x)-pos)break;pos+=spacing;rollAdvance60(spacing,accel,r.expo,i);}
 if(direction>2)direction=rollDir939[t];int32_t first=direction==1?-last:-r.start,finish=direction==0?last:r.start;
 int64_t now=songEngineActive?songTick:transportTickAbsolute,begin=now-offset+first,end=now-offset+finish+gate;
 rollIdentity44=((uint64_t)begin<<4)|t;auto&w=rollWindows44[t];if(x.v[7]==2&&(w.begin!=begin||w.end!=end||w.identity!=(uint32_t)identity)){w={begin,end,(uint32_t)identity};mod805Invalidate();}
 if(x.v[7]==1)rollHoldUs44=max(1000UL,(uint32_t)(max((int64_t)1,end-now)*tickDurationMs()*1000.0f));
}

FLASHMEM void rollAdvance60(int32_t&spacing,int32_t&acceleration,uint8_t mode,uint16_t index){
 int32_t d=rollWave60(acceleration,mode,index);spacing=clampInt(spacing+(d>=0?(d+128)/256:-((-d+128)/256)),1,96);
 if(mode>0&&mode<=100&&acceleration){int32_t mag=min((int64_t)32768,((int64_t)abs(acceleration)*expFactor14(mode)+128)/256);acceleration=acceleration<0?-mag:mag;}
}
FLASHMEM bool rollClick60(uint8_t key,bool down){
 if(grooveEditMode||patternMode||menuNavigationOnly()||patchUi808||modUi805||keyboardEnabled823||(quickView903&&quickView903!=2))return false;
 uint8_t k=quickView903==2?rollKey26(key):key==10?0:key==13?6:key==15?12:255;if(k!=0&&k!=6&&k!=12)return false;
 bool click=!down&&kyBtnDown[key]&&!kyRotatedWhileDown[key];kyBtnDown[key]=down;if(down){kyRotatedWhileDown[key]=false;kyBtnDownAtMs[key]=millis();}
 if(click){rollModeChange60=true;if(k==0)rollFine60=!rollFine60;else if(k==6){int v=displayedRollForTrack(selectedTrack).expo,next=v<=100?101:v==101?102:33;bool fine=curveFine41;curveFine41=true;editTrackRollFromKy(3,next-v);curveFine41=fine;}else{int v=lockDisplay817(LOCK_ROLL1000,selectedTrack,0,rollExtra1000(selectedTrack).v[0]);rollEdit1000(0,(v<=100?101:v==101?102:33)-v);}neoDisplayDirty=displayValueDirty=true;rollModeChange60=false;}return true;
}

FLASHMEM int rollDirection60(uint8_t t,uint8_t p,int32_t tick,uint8_t note,bool draft){
 int base=rollDir939[t];if(p>=16||note>=128)return base;
 if(draft)return lockDraftValue26({tick,base,t,8,t,p,note,LOCK_ROLL1000},base);
 if(playbackLocks828)for(int i=lockEventFirst30(t,p,tick,note);i>=0;i=lockEventNext30(i)){auto&l=lockPlaybackEntry39(i);if(l.track==t&&l.pattern==p&&l.tick==tick&&l.note==note&&l.kind==LOCK_ROLL1000&&l.parameter==8)return l.value;}return base;
}
