#pragma once
FLASHMEM uint8_t indFocus59(uint8_t t){if(patternMode&&lastPatternValid&&lastPatternTrack==t)return lastPatternIndex;return activePattern[t];}
FLASHMEM bool indEnabled59(uint8_t t,uint8_t p){return t<16&&p<16&&(ind50.flags[t*16+p]&128);}
FLASHMEM bool indEditing59(){return indEnabled59(selectedTrack,indFocus59(selectedTrack));}
FLASHMEM int indFamily50(uint8_t kind,uint16_t target,uint16_t param){
 if(kind==IND_LIST50)return IND_OOV50;if(kind==IND_NOTE50)return IND_BAS50;if(kind==IND_ROLL50)return IND_ROL50;
 if(kind==LOCK_BLOCK21)return param==12?IND_OOV50:IND_BAS50;
 if(kind==LOCK_ROLL1000)return IND_ROL50;
 if(kind==LOCK_DELAY817)return IND_DLY50;
 if(kind==LOCK_CC817||kind==LOCK_NRPN7817||kind==LOCK_NRPN14817||kind==LOCK_PROGRAM26)return IND_PATCH50;
 if(kind==LOCK_MOD817||kind==LOCK_EXTRA817||kind==LOCK_DEPTH817||kind==LOCK_RAT817)return target<64?(modKind931(target)?IND_LFO50:IND_ENV50):-1;
 return -1;
}
FLASHMEM uint16_t indTarget50(uint8_t kind,uint16_t target){return kind==LOCK_DELAY817?target/256:kind==IND_LIST50||kind==IND_NOTE50||kind==IND_ROLL50||kind==LOCK_BLOCK21||kind==LOCK_ROLL1000||lockMidiKind817(kind)?0:target;}
FLASHMEM int indBase50(uint8_t t,uint8_t p,uint8_t kind,uint16_t target,uint16_t param,int base){int f=indFamily50(kind,target,param);if(t>=16||p>=16||f<0)return base;return ind50.value(t*16+p,f,kind,indTarget50(kind,target),param,base);}
FLASHMEM uint8_t indRuntimePattern50(uint8_t t,uint16_t key){
 int64_t now=queryTick50>=0?queryTick50:(int64_t)(songEngineActive?songTick:transportTickAbsolute);uint8_t p=patch808PlayingPattern(t);
 auto due=[&](uint8_t candidate,int64_t start){if(now>=start)return true;if(key>=64||modKind931(key))return false;int h=indBase50(t,candidate,LOCK_MOD817,key,0,holdBase50(key,modUnits805[key]));return h<0&&now>=start+h;};
 if(songEngineActive){for(auto&c:songClips)if(c.used&&c.track==t&&now<(int64_t)(c.position+c.length)*96&&due(c.pattern,(int64_t)c.position*96))p=c.pattern;}
 else if(grooveBridgeReady[t]&&!grooveBridgeSwitched[t]&&pendingPattern[t]>=0&&due(pendingPattern[t],grooveBridgeAt[t]))p=pendingPattern[t];
 return p;
}
FLASHMEM int indLive50(uint8_t kind,uint16_t target,uint16_t param,int base){
 bool mod=kind==LOCK_MOD817||kind==LOCK_EXTRA817||kind==LOCK_DEPTH817||kind==LOCK_RAT817;
 int t=indContext50;if(t<0){if(kind==LOCK_DELAY817)t=(target%256)/16;else if(kind==LOCK_BLOCK21||kind==LOCK_ROLL1000||lockMidiKind817(kind))t=target;else if(target<64){for(int i=0;i<16;++i)if(modAssociated12(target,i)){t=i;break;}}}
 if(mod&&t>=0&&!modAssociated12(target,t))return base;
 return t>=0&&t<16?indBase50(t,indRuntimePattern50(t,mod?target:65535),kind,target,param,base):base;
}
static EXTMEM uint64_t indPatternMods50[256];
FLASHMEM uint16_t indModOwners50(uint16_t key){uint16_t owners=0;bool split=false;for(uint8_t t=0;t<16;++t)if(modAssociated12(key,t)){owners|=1u<<t;uint8_t p=indRuntimePattern50(t,key),o=t*16+p;int f=modKind931(key)?IND_LFO50:IND_ENV50;split|=ind50.active(o,f)&&(indPatternMods50[o]&(1ULL<<key));}return split?owners:0;}
FLASHMEM int indDisplay50(uint8_t kind,uint16_t target,uint16_t param,int base){if(!indEditing59())return base;uint8_t t=selectedTrack,p=activePattern[t];if(patternMode&&lastPatternValid&&lastPatternTrack==t)p=lastPatternIndex;else if(indEditTrack50==t&&patMenu46==5)p=indEditPattern50;return indBase50(t,p,kind,target,param,base);}
FLASHMEM void indChanged50(){memset(indPatternMods50,0,sizeof(indPatternMods50));for(auto&e:ind50.entries)if(e.state==1&&(e.family==IND_ENV50||e.family==IND_LFO50)&&e.target<64)indPatternMods50[e.owner]|=1ULL<<e.target;storageDirty931=true;neoDisplayDirty=displayValueDirty=modDisplayDirty810=true;remixScheduleDirty911=true;mod805Invalidate();invalidateStepTriggers();}
FLASHMEM void indPatchRefresh51(uint8_t t,int parameter){
 if(t>=16)return;BlockScope21 block(patchBlockIndex21());const auto&c=patch808Context(t,patch808PlayingPattern(t));MidiScope902 route(outMask902(t),t);
 for(auto*q=patchNext819(c,nullptr);q;q=patchNext819(c,q))if(parameter<0||q->number==parameter){uint16_t v=0;if(patchValue819(c,*q,v))patch808SendParam(patch808Channel(t,c),*q,v);}
}
FLASHMEM bool indEdit50(uint8_t kind,uint16_t target,uint16_t param,int base,int delta,int lo,int hi){
 if((!patternMode&&lockWantsEdit817())||(!indEditing59()&&!(patternMode&&selectedPatternCount())))return false;
 uint8_t t=selectedTrack,p=activePattern[t];if(patternMode&&lastPatternValid&&lastPatternTrack==t)p=lastPatternIndex;else if(indEditTrack50==t&&patMenu46==5)p=indEditPattern50;
 int family=indFamily50(kind,target,param);if(family<0)return false;
 if(patternMode&&selectedPatternCount()){
   unsigned needed=0,selected=0;bool def=false;uint16_t dest=indTarget50(kind,target);
   for(uint8_t q=0;q<16;++q)if(patternSelected[t][q]){++selected;needed+=indEnabled59(t,q)&&ind50.find(t*16+q,family,kind,dest,param)<0;}
   if(selected){
     if(!deleteButtonActive&&ind50.count+needed>IndStore50::CAPACITY){DISPLAY_SERIAL.println("MSG|IND MEMORY FULL");return true;}
     for(uint8_t q=0;q<16;++q)if(patternSelected[t][q]){
       if(!indEnabled59(t,q)){def=true;continue;}
       if(deleteButtonActive)ind50.remove(t*16+q,family,kind,dest,param);
       else {int old=ind50.value(t*16+q,family,kind,dest,param,base);ind50.set(t*16+q,family,kind,dest,param,ZCLAMP57(old,delta,lo,hi));}
     }
     indChanged50();if(family==IND_PATCH50)indPatchRefresh51(t,param);return !def; // Caller updates the shared DEF exactly once.
   }
 }

 if(!indEnabled59(t,p))return false;
 uint16_t dest=indTarget50(kind,target);int old=ind50.value(t*16+p,family,kind,dest,param,base);
 int next=ZCLAMP57(old,delta,lo,hi);
 if(deleteButtonActive)ind50.remove(t*16+p,family,kind,dest,param);
 else if(!ind50.set(t*16+p,family,kind,dest,param,next)){DISPLAY_SERIAL.println("MSG|IND MEMORY FULL");return true;}
 indChanged50();if(family==IND_PATCH50)indPatchRefresh51(t,param);return true;
}
FLASHMEM void indRender50(){
 char rows[4][21];for(auto&r:rows){memset(r,' ',20);r[20]=0;}
 const char*names[]={"BAS","ROL","DLY","ENV","Patch","OOV","LFO"};const uint8_t row[]={0,0,0,0,1,1,1},col[]={0,5,10,15,0,10,15};
 uint8_t flags=ind50.flags[indEditTrack50*16+indEditPattern50];
 for(uint8_t i=0;i<7;++i){if(flags&(1u<<i))memcpy(rows[row[i]]+col[i],names[i],strlen(names[i]));memcpy(rows[row[i]+2]+col[i],names[i],strlen(names[i]));}
 sendMenuScreen(rows[0],rows[1],rows[2],rows[3]);
}
FLASHMEM void indButton50(uint8_t i){static const int8_t map[]={0,1,2,3,4,-1,5,6};if(i>=8||map[i]<0)return;uint8_t owner=indEditTrack50*16+indEditPattern50,f=map[i];uint8_t before=programValue939(indEditTrack50,patch808PlayingPattern(indEditTrack50));ind50.useFamily(owner,f,!(ind50.flags[owner]&(1u<<f)));if(f==IND_OOV50&&ind50.active(owner,f))patternGroove[indEditTrack50][indEditPattern50]=0;indChanged50();if(f==IND_PATCH50){if(before!=programValue939(indEditTrack50,patch808PlayingPattern(indEditTrack50)))programSend939(indEditTrack50,patch808PlayingPattern(indEditTrack50));for(uint8_t b=0;b<3;++b){BlockScope21 block(b);indPatchRefresh51(indEditTrack50);}}indRender50();}
FLASHMEM bool indPad50(uint8_t n,uint8_t pad,uint8_t edge){
 if(indEditMode52&&patternMode&&!grooveEditMode){uint8_t t=n*4+pad%4,p=patternAtRow51(t,patternRowOffset+pad/4);if(t>=16||p>=16)return true;neoPadDown[n][pad]=edge==SEESAW_KEYPAD_EDGE_RISING;if(edge==SEESAW_KEYPAD_EDGE_RISING){selectedTrack=t;lastPatternTrack=t;lastPatternIndex=p;lastPatternValid=true;indEditTrack50=t;indEditPattern50=p;displayedPattern=p+1;clearPatternSelection();patternSelected[t][p]=true;ind50.flags[t*16+p]^=128;indChanged50();if(p==patch808PlayingPattern(t)){programSend939(t,p);indPatchRefresh51(t);}DISPLAY_SERIAL.printf("MSG|P%02u %s\n",p+1,indEnabled59(t,p)?"IND":"DEF");}return true;}
 if(patMenu46!=4&&patMenu46!=5)return false;uint8_t t=n*4+pad%4,p=patternAtRow51(t,patternRowOffset+pad/4);if(t>=16||p>=16)return true;
 neoPadDown[n][pad]=edge==SEESAW_KEYPAD_EDGE_RISING;
 if(edge==SEESAW_KEYPAD_EDGE_RISING){indEditTrack50=selectedTrack=t;indEditPattern50=p;lastPatternTrack=t;lastPatternIndex=p;lastPatternValid=true;
 uint8_t before=programValue939(t,patch808PlayingPattern(t));if(patMenu46==4){ind50.useDef(t*16+p);patternGroove[t][p]=0;}else ind50.flags[t*16+p]|=0x80;indChanged50();if(before!=programValue939(t,patch808PlayingPattern(t)))programSend939(t,patch808PlayingPattern(t));for(uint8_t b=0;b<3;++b){BlockScope21 block(b);indPatchRefresh51(t);}if(patMenu46==5)indRender50();}
 return true;
}
FLASHMEM void indReset50(){orderReset51();indClocksReset50();ind50.clear();indStage50.clear();memset(envEarly50,0,sizeof(envEarly50));memset(cvShift50,0,sizeof(cvShift50));indContext50=-1;}
FLASHMEM bool indWrite50(File&f){uint32_t magic=0x50353049;uint16_t count=ind50.count;bool ok=f.write((uint8_t*)&magic,4)==4&&f.write(ind50.flags,256)==256&&f.write((uint8_t*)&count,2)==2;
 for(const auto&e:ind50.entries)if(ok&&e.state==1){auto out=e;if(modLock931(e.kind)){if(e.target>=64||!modUnits805[e.target].enabled)return false;out.target=modDiskKey931(e.target);}ok=f.write((uint8_t*)&out,sizeof(out))==sizeof(out);}if(ok){for(unsigned disk=0;disk<512;++disk){uint16_t key=disk/2;int16_t v=key<64&&modUnits805[key].enabled&&modKind931(key)==disk%2?envEarly50[key]:0;if(f.write((uint8_t*)&v,2)!=2)return false;}ok=f.write((uint8_t*)cvShift50,sizeof(cvShift50))==sizeof(cvShift50);}return ok;
}
static EXTMEM int16_t fullEarly50[64],fullCvShift50[4];
FLASHMEM bool indRead50(File&f,bool present){memset(fullEarly50,0,sizeof(fullEarly50));memset(fullCvShift50,0,sizeof(fullCvShift50));indStage50.clear();if(!present)return true;uint32_t magic=0;uint16_t count=0;if(f.read((uint8_t*)&magic,4)!=4||magic!=0x50353049||f.read(indStage50.flags,256)!=256||f.read((uint8_t*)&count,2)!=2||count>IndStore50::CAPACITY)return false;
 uint8_t oldFlags52[256];memcpy(oldFlags52,indStage50.flags,256);memset(indStage50.flags,0,256);
 for(unsigned i=0;i<count;++i){IndEntry50 e{};if(f.read((uint8_t*)&e,sizeof(e))!=sizeof(e)||e.state!=1||e.family>=7||(e.kind>LOCK_BLOCK21&&e.kind!=IND_NOTE50&&e.kind!=IND_ROLL50&&e.kind!=IND_LIST50))return false;if(modLock931(e.kind)){if(e.target>=512||fullMap931[e.target]<0)return false;e.target=fullMap931[e.target];}if(!indEntryValid50(e)||indStage50.find(e.owner,e.family,e.kind,e.target,e.parameter)>=0||!indStage50.set(e.owner,e.family,e.kind,e.target,e.parameter,e.value))return false;}
 memcpy(indStage50.flags,oldFlags52,256);
 for(unsigned disk=0;disk<512;++disk){int16_t v;if(f.read((uint8_t*)&v,2)!=2||v>0||v< -384)return false;if(fullMap931[disk]>=0)fullEarly50[fullMap931[disk]]=v;}if(f.read((uint8_t*)fullCvShift50,sizeof(fullCvShift50))!=sizeof(fullCvShift50))return false;for(auto v:fullCvShift50)if(v< -384||v>384)return false;return true;
}
FLASHMEM void indInstall50(){ind50=indStage50;memcpy(envEarly50,fullEarly50,sizeof(envEarly50));memcpy(cvShift50,fullCvShift50,sizeof(cvShift50));indChanged50();}

FLASHMEM int holdBase50(uint16_t k,const ModUnit805&m){return envEarly50[k]<0?envEarly50[k]:m.hold;}
FLASHMEM int holdValue50(uint16_t k,const ModUnit805&m){return lockValue817(LOCK_MOD817,k,0,holdBase50(k,m));}
FLASHMEM void holdSet50(uint16_t k,int v){v=clampInt(v,-384,65535);envEarly50[k]=min(0,v);modUnits805[k].hold=max(0,v);}
FLASHMEM void cvTimingRender50(){char a[21],b[21];snprintf(a,21,"%+d  %+d",cvShift50[0],cvShift50[1]);snprintf(b,21,"%+d  %+d",cvShift50[2],cvShift50[3]);patch808Frame("CV1  CV2 (-:advance)",a,"CV3  CV4 (+:delay)",b);}
FLASHMEM bool cvTimingRoy50(uint8_t roy,int32_t delta,bool click){if(patchUi808!=16)return false;if(roy<4&&delta){cvShift50[roy]=ZCLAMP57(cvShift50[roy],delta,-384,384);indChanged50();}if(click&&!roy){patch808Back();return true;}cvTimingRender50();return true;}

struct UpcomingNote50 {int32_t tick;uint32_t gate;int32_t identity;uint8_t note;};
struct UpcomingCache50 {UpcomingNote50 notes[4224];uint16_t count;uint8_t pattern;uint32_t epoch;uint32_t length;};
static EXTMEM UpcomingCache50 upcoming50[16];
FLASHMEM bool upcomingSource50(uint8_t t,int64_t at,ModSourceCache805&result){
 auto&begin=result.begin;auto&gate=result.gate;auto&next=result.next;
 if(songEngineActive||!playing||!grooveBridgeReady[t]||grooveBridgeSwitched[t]||at<(int64_t)grooveBridgeAt[t])return false;
 uint8_t p=pendingPattern[t];if(p>=16)return false;auto&c=upcoming50[t];
 if(c.epoch!=modEpoch805||c.pattern!=p){c.count=0;c.pattern=p;c.epoch=modEpoch805;
  if(!patterns906[t][p]||!unpackPattern906(patterns906[t][p],patternWork906))return false;
  auto&image=patternWork906;c.length=image.bars*1536;
  auto append=[&](uint16_t step,uint8_t note,uint8_t vel,int micro,int logical,uint32_t duration){if(!vel||c.count>=4224)return;auto b=blockAtNote21(t,0,p,step*96+logical,note);if(!b.port)return;int32_t head=((int64_t)step*96+image.mov[0])%c.length;if(head<0)head+=c.length;uint8_t sheet=grooveSheet40(t,p,b.groove,head/1536);int shift=micro+logical+b.tim, g=duration;if(!grooveBypass23&&sheet>0&&sheet<=GROOVE_COUNT){shift+=grooveTime16(sheet,step*96+micro+logical);g=(int64_t)g*grooveGate51(sheet,step)/100;}c.notes[c.count++]={head+shift,(uint32_t)clampInt(g,GATE_TICK_MIN,GATE_TICK_MAX),(int32_t)step*96+logical,note};};
  for(unsigned step=0;step<image.bars*16;++step)if(image.active[step]){append(step,image.note[step],image.vel[step],image.micro[step],image.logical[step],image.gate[step]);if(trackPlayMode[linkOwner12(t,LINK_POLY12)]!=TRACK_PLAY_MONO)for(unsigned v=0;v<image.extraCount[step]&&v<3;++v){auto&e=image.extra14[step][v];append(step,image.extraNote[step][v],e.vel,e.micro,image.logical[step],e.gate);}}
  for(auto&e:image.fine)if(e.active)append(e.step,e.note,e.vel,e.micro,e.logicalMicro,e.gate);
 }
 auto previous=mod805Source(t,(int64_t)grooveBridgeAt[t]-1);begin=previous.begin;gate=previous.gate;result.pattern=previous.pattern;result.identity=previous.identity;result.note=previous.note;next=INT64_MAX/2;
 for(unsigned i=0;i<c.count;++i){int64_t tick=(int64_t)grooveBridgeAt[t]+c.notes[i].tick;if(tick<=at&&tick>=begin){gate=tick==begin?max(gate,c.notes[i].gate):c.notes[i].gate;begin=tick;result.pattern=p;result.identity=c.notes[i].identity;result.note=c.notes[i].note;}else if(tick>at)next=min(next,tick);}return true;
}

FLASHMEM int indNote50(uint8_t t,uint8_t field,bool display){int base=field==0?defaultMidiNote[t]:field==1?defaultVel[t]:field==2?defaultGateTicks[t]:field==3?defaultMicroTick[t]:trackPitchBend[t];return display?indDisplay50(IND_NOTE50,t,field,base):indBase50(t,indRuntimePattern50(t),IND_NOTE50,t,field,base);}
FLASHMEM bool indNoteEdit50(uint8_t field,int delta){uint8_t t=selectedTrack;int base=field==0?defaultMidiNote[t]:field==1?defaultVel[t]:field==2?defaultGateTicks[t]:field==3?defaultMicroTick[t]:trackPitchBend[t];return indEdit50(IND_NOTE50,t,field,base,delta,field==0?0:field==1?1:field==2?GATE_TICK_MIN:field==3?-96:-8192,field<2?127:field==2?GATE_TICK_MAX:field==3?96:8191);}
FLASHMEM bool indPatternEdit50(uint8_t ky,int32_t delta,int8_t only){if(!selectedPatternCount()||!delta||ky==6)return false;int field=ky>=2&&ky<=5?ky-2:ky==8?5:ky>=10&&ky<=14?ky-10:-1;if(field<0)return false;bool roll=ky>=8;uint16_t defDone=0;unsigned needed50=0;for(uint8_t t=0;t<16;++t)for(uint8_t p=0;p<16;++p)if((only<0||only==t)&&patternSelected[t][p]&&indEnabled59(t,p)&&ind50.find(t*16+p,roll?IND_ROL50:IND_BAS50,roll?IND_ROLL50:IND_NOTE50,0,field)<0)++needed50;if(ind50.count+needed50>IndStore50::CAPACITY){DISPLAY_SERIAL.println("MSG|IND MEMORY FULL");return true;}
 int bases51[16];for(uint8_t t=0;t<16;++t)bases51[t]=roll?rollFieldValue(trackRoll[t],field):field==0?defaultMidiNote[t]:field==1?defaultVel[t]:field==2?defaultGateTicks[t]:defaultMicroTick[t];
 for(uint8_t t=0;t<16;++t)for(uint8_t p=0;p<16;++p)if((only<0||only==t)&&patternSelected[t][p]){uint8_t family=roll?IND_ROL50:IND_BAS50,kind=roll?IND_ROLL50:IND_NOTE50;int base=bases51[t];int lo=roll?clampRollFieldValue(field,-32768):field==1?1:field==2?GATE_TICK_MIN:field==3?-96:0,hi=roll?clampRollFieldValue(field,32767):field<2?127:field==2?GATE_TICK_MAX:96;int old=indEnabled59(t,p)?indBase50(t,p,kind,t,field,base):base,v=ZCLAMP57(old,delta,lo,hi);
  if(indEnabled59(t,p)){if(!ind50.set(t*16+p,family,kind,0,field,v)){DISPLAY_SERIAL.println("MSG|IND MEMORY FULL");return true;}}
  else if(!(defDone&(1u<<t))){defDone|=1u<<t;if(roll)setRollFieldValue(trackRoll[t],field,v);else if(field==0)defaultMidiNote[t]=v;else if(field==1)defaultVel[t]=v;else if(field==2)defaultGateTicks[t]=v;else defaultMicroTick[t]=v;}
 }indChanged50();return true;
}
FLASHMEM int indPatchOwner50(const PatchContext808&c){uintptr_t p=(uintptr_t)&c,begin=(uintptr_t)&patchDb808.context[0][0];if(p<begin||p>=begin+sizeof(patchDb808.context))return -1;return (p-begin)/(sizeof(PatchContext808)*51);}
FLASHMEM int indPatchValue50(const PatchContext808&c,const PatchParam808&q,int base){int t=indPatchOwner50(c);if(t<0)return base;uint8_t kind=q.kind?(q.bits==14?LOCK_NRPN14817:LOCK_NRPN7817):LOCK_CC817;return indBase50(t,indRuntimePattern50(t),kind,t,q.number,base);}
FLASHMEM bool indPatchEdit50(PatchContext808&c,const PatchParam808&q,uint16_t value){int t=indPatchOwner50(c);if(t!=selectedTrack||t<0)return false;uint16_t base=0;patchRawValue50(c,q,base);uint8_t kind=q.kind?(q.bits==14?LOCK_NRPN14817:LOCK_NRPN7817):LOCK_CC817;return indEdit50(kind,t,q.number,base,0,value,value);}

FLASHMEM bool indEntryValid50(const IndEntry50&e){if(e.state!=1||e.family>=7||indFamily50(e.kind,e.target,e.parameter)!=e.family)return false;
 if(e.kind==LOCK_MOD817&&e.target<64&&modKind931(e.target)&&e.parameter>=5)return e.parameter==5?(e.value>=1&&e.value<=99):e.parameter==6&&(e.value==80||(e.value>=119&&e.value<=126));
 if(e.family==IND_PATCH50&&e.value==65535&&(e.kind==LOCK_CC817||e.kind==LOCK_NRPN7817||e.kind==LOCK_NRPN14817))return !e.target&&e.parameter<(e.kind==LOCK_CC817?128:16384);
 if(e.kind==IND_LIST50)return !e.target&&e.parameter<28&&e.value>=0&&e.value<=(e.parameter%2?99:GROOVE_COUNT);
 if(e.kind==IND_NOTE50){if(e.target||e.parameter>4)return false;int lo=e.parameter<2?0:e.parameter==2?GATE_TICK_MIN:e.parameter==3?-96:-8192,hi=e.parameter<2?127:e.parameter==2?GATE_TICK_MAX:e.parameter==3?96:8191;return e.value>=lo&&e.value<=hi;}
 if(e.kind==IND_ROLL50)return !e.target&&e.parameter<6&&clampRollFieldValue(e.parameter,e.value)==e.value;
 uint16_t target=e.kind==LOCK_DELAY817?e.target*256+(e.owner/16)*16+e.owner%16:e.target;NoteLock817 l={0,e.value,target,e.parameter,(uint8_t)(e.owner/16),(uint8_t)(e.owner%16),128,e.kind,1};return lockValid817(l);
}

FLASHMEM int forecastValue50(uint8_t kind,uint16_t target,uint16_t param,int base,bool&found){found=false;if(target>=64||!modLock931(kind)||!forecast50[target].valid||!playbackLocks828)return base;auto&f=forecast50[target];for(int i=lockEventFirst30(f.track,f.pattern,f.identity,f.note);i>=0;i=lockEventNext30(i)){auto&l=lockPlaybackEntry39(i);if(l.track==f.track&&l.pattern==f.pattern&&l.tick==f.identity&&l.note==f.note&&l.kind==kind&&l.target==target&&l.parameter==param){found=true;return l.value;}}return base;}
FLASHMEM void prepareForecast50(uint16_t key,int64_t at){
 forecast50[key].valid=false;if(modKind931(key)||ioKey38(key)||!playing)return;
 static uint32_t epoch=UINT32_MAX;static EXTMEM uint16_t advance[64];if(epoch!=registryEpoch31()){epoch=registryEpoch31();memset(advance,0,sizeof(advance));for(unsigned i=0;i<lockPlaybackCount39();++i){auto&l=lockPlaybackEntry39(i);if(l.kind==LOCK_MOD817&&!l.parameter&&l.target<64&&l.value<0)advance[l.target]=max(advance[l.target],(uint16_t)-l.value);}}
 int early=max((int)advance[key],-min(0,holdBase50(key,modUnits805[key])));for(uint8_t t=0;t<16;++t)if(modAssociated12(key,t)){int h=indBase50(t,indRuntimePattern50(t,key),LOCK_MOD817,key,0,holdBase50(key,modUnits805[key]));early=max(early,-min(0,h));}
 if(!early&&!queryOffset50&&indContext50<0)return;
 int64_t best=INT64_MIN/2;Forecast50 chosen;for(uint8_t t=0;t<16;++t)if((modUnits805[key].inputs&(1u<<t))&&(indContext50<0||t==indContext50)){
  int64_t search=at+early;for(unsigned attempt=0;attempt<385;++attempt){auto c=mod805Source(t,search);if(c.begin<0||c.pattern>15||c.note>127)break;forecast50[key]={c.identity,t,c.pattern,c.note,true};bool found=false;int h=forecastValue50(LOCK_MOD817,key,0,indBase50(t,c.pattern,LOCK_MOD817,key,0,holdBase50(key,modUnits805[key])),found);
   if(c.begin+min(0,h)<=at){if(c.begin>=best){best=c.begin;chosen=forecast50[key];}break;}search=c.begin-1;
  }
 }forecast50[key]=chosen;
}
FLASHMEM void indClocksReset50(int key){if(key<0){memset(indFreeClock50,0,sizeof(indFreeClock50));memset(indFreeSamples50,0,sizeof(indFreeSamples50));}else for(unsigned t=0;t<16;++t){indFreeClock50[t][key]={};indFreeSamples50[t][key]={};}}

FLASHMEM void indMarks50(){uint16_t masks[4]={};uint8_t t=selectedTrack,p=activePattern[t];if(patternMode&&lastPatternValid&&lastPatternTrack==t)p=lastPatternIndex;else if(indEditTrack50==t&&patMenu46==5)p=indEditPattern50;
 auto mark=[&](uint8_t screen,uint8_t col,uint8_t kind,uint16_t target,uint16_t param){int f=indFamily50(kind,target,param);if(indEditing59()&&f>=0&&ind50.active(t*16+p,f)&&ind50.find(t*16+p,f,kind,indTarget50(kind,target),param)>=0)masks[screen]|=1u<<col;};
 if(modUi805==MOD_DELAY806){for(int k=0;k<16;++k)if(k!=14)mark(k/4,k%4*4,LOCK_DELAY817,t*16+delaySlot924*256,delayQvFields925[k]);}
 else if(modUi805&&modSelected806<64){uint16_t k=modSelected806;
 if(modKind931(k)){mark(0,0,LOCK_MOD817,k,0);mark(0,12,LOCK_MOD817,k,1);mark(1,0,LOCK_EXTRA817,k,4);mark(1,4,LOCK_EXTRA817,k,3);mark(1,8,LOCK_MOD817,k,5);mark(1,12,LOCK_MOD817,k,6);for(int i=0;i<3;++i)mark(3,i*4,LOCK_MOD817,k,i+2);mark(3,8,LOCK_EXTRA817,k,2);}
 else{mark(0,0,LOCK_MOD817,k,0);mark(0,8,LOCK_MOD817,k,1);mark(0,8,LOCK_EXTRA817,k,0);mark(1,0,LOCK_MOD817,k,2);mark(1,0,LOCK_EXTRA817,k,1);mark(1,8,LOCK_MOD817,k,3);mark(3,0,LOCK_MOD817,k,4);mark(3,0,LOCK_EXTRA817,k,2);mark(3,8,LOCK_RAT817,k,0);}
 if(modDepthSlot806>=0)mark(2,0,LOCK_DEPTH817,k,modDepthSlot806);
 }
 static uint16_t previous[4]={65535,65535,65535,65535};for(int i=0;i<4;++i)if(previous[i]!=masks[i]){previous[i]=masks[i];DISPLAY_SERIAL.printf("IND50,%d,%u\n",i,masks[i]);}
}

FLASHMEM void indToggle52(){indEditMode52=!indEditMode52;if(indEditMode52){leavePadModesA01(true);patternMode=true;grooveEditMode=false;keyboardEnabled823=false;}lockHoldCancel26();selectionIndicatorsPending931=true;displayValueDirty=modDisplayDirty810=neoDisplayDirty=true;DISPLAY_SERIAL.println(indEditMode52?"MSG|PAT IND TOGGLE":"MSG|PAT IND END");if(patMenu46)patRender46();}
