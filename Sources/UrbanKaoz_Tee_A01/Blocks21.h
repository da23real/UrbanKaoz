#pragma once
FLASHMEM void blockReset21(){for(uint8_t t=0;t<16;++t)for(uint8_t b=0;b<3;++b)blocks21[t][b]={0,(uint8_t)(b?0:1),t,0};memset(trackLists44,0,sizeof(trackLists44));memset(trackListCounts44,0,sizeof(trackListCounts44));blockListEdit21=patchBlock21=quickPatchBlock21=0;blockPlaying21=-1;blockResolvedValid21=false;}
FLASHMEM uint8_t blockMask21(uint8_t t){if(t>=16)return 0;auto b=blockResolvedValid21?blockResolved21:blockAtNote21(t,blockIndex21(),255,-1,255);return b.port&&b.port<=8?1u<<(b.port-1):0;}
FLASHMEM uint8_t blockChannel21(uint8_t t){if(t>=16)return 0;return (blockResolvedValid21?blockResolved21:blockAtNote21(t,blockIndex21(),255,-1,255)).channel&15;}
// Sparse fixed hash: no full lock-registry scan for every candidate voice.
EXTMEM int16_t blockLockIndex21[4096];uint32_t blockLockEpoch21=UINT32_MAX;uint16_t blockLockCount21=UINT16_MAX;
FLASHMEM uint32_t blockLockHash21(uint8_t t,uint8_t p,int32_t tick,uint8_t note,uint16_t field){return ((uint32_t)tick*2654435761u+t*1619u+p*31337u+note*6971u+field*1013u)&4095;}
FLASHMEM int blockLockFind21(uint8_t t,uint8_t p,int32_t tick,uint8_t note,uint16_t field){
 if(!lockPlaybackCount39())return -1;
 if(blockLockEpoch21!=registryEpoch31()||blockLockCount21!=lockPlaybackCount39()){memset(blockLockIndex21,255,sizeof(blockLockIndex21));for(uint16_t i=0;i<lockPlaybackCount39();++i){auto&l=lockPlaybackEntry39(i);if(l.kind!=LOCK_BLOCK21)continue;uint32_t h=blockLockHash21(l.track,l.pattern,l.tick,l.note,l.parameter);while(blockLockIndex21[h]>=0)h=(h+1)&4095;blockLockIndex21[h]=i;}blockLockEpoch21=registryEpoch31();blockLockCount21=lockPlaybackCount39();}
 uint32_t h=blockLockHash21(t,p,tick,note,field);while(blockLockIndex21[h]>=0){int i=blockLockIndex21[h];auto&l=lockPlaybackEntry39(i);if(l.track==t&&l.pattern==p&&l.tick==tick&&l.note==note&&l.parameter==field&&l.target==t)return i;h=(h+1)&4095;}return -1;
}
EXTMEM int16_t blockLeaseIndex21[16][13];uint32_t blockLeaseEpoch21=UINT32_MAX;uint16_t blockLeaseCount21=UINT16_MAX;
FLASHMEM int blockLeaseValue21(uint8_t t,uint8_t field,int base){if(!playbackLocks828||!lockLeaseCount817)return base;if(blockLeaseEpoch21!=modEpoch805||blockLeaseCount21!=lockLeaseCount817){memset(blockLeaseIndex21,255,sizeof(blockLeaseIndex21));for(uint16_t i=0;i<256;++i){auto&l=lockLeases817[i];if(!l.used||l.kind!=LOCK_BLOCK21||l.target>=16||l.parameter>=13)continue;auto&at=blockLeaseIndex21[l.target][l.parameter];if(at<0||(int32_t)(l.serial-lockLeases817[at].serial)>0)at=i;}blockLeaseEpoch21=modEpoch805;blockLeaseCount21=lockLeaseCount817;}int at=blockLeaseIndex21[t][field];return at<0?base:lockLeases817[at].value;}
FLASHMEM Block21 blockAtNote21(uint8_t t,uint8_t b,uint8_t pattern,int32_t tick,uint8_t note){Scope830 addressMeasure30(P830_ADDR30);auto r=blocks21[linkOwner12(t,LINK_MOUT12)][b];uint16_t fields=blockTargeted30?blockNoteFields30(t):4095;int values[4]={r.port,r.channel,r.tim,r.transpose};for(uint8_t f=0;f<4;++f)values[f]=autoParameter46(LOCK_BLOCK21,t,b*4+f,indBase50(t,pattern<16?pattern:indRuntimePattern50(t),LOCK_BLOCK21,t,b*4+f,values[f]));for(uint8_t f=0;f<4;++f){if(lockLeaseCount817)values[f]=blockLeaseValue21(t,b*4+f,values[f]);if(playbackLocks828&&pattern<16&&(fields&(1u<<(b*4+f)))){int i=blockLockFind21(t,pattern,tick,note,b*4+f);if(i>=0)values[f]=lockPlaybackEntry39(i).value;}}r.port=clampInt(values[0],0,8);r.channel=clampInt(values[1],0,15);r.tim=clampInt(values[2],-96,96);r.transpose=clampInt(values[3],-127,127);int g=autoParameter46(LOCK_BLOCK21,t,12,indBase50(t,pattern<16?pattern:indRuntimePattern50(t),LOCK_BLOCK21,t,12,blocks21[t][0].groove));
 if(lockLeaseCount817)g=blockLeaseValue21(t,12,g);if(playbackLocks828&&pattern<16){int i=blockLockFind21(t,pattern,tick,note,12);if(i>=0)g=lockPlaybackEntry39(i).value;}r.groove=clampInt(g,0,listCount44(t));return r;}
FLASHMEM int blockNoteDisplay21(uint8_t field){if(remixMode&&remixSelection911.steps){auto&m=remixSelection911.role==0?remixSource911:remixDraft911;for(uint32_t i=0;i<m.notes.size;++i){auto&n=m.notes[i];if(remixSelection911.contains(n,m,remixSelection911.role==0))return field==0?n.pitch:field==1?n.velocity:field==2?n.gate:n.time;}}EditMode old=editMode;editMode=field==0?EDIT_PITCH:field==1?EDIT_VEL:field==2?EDIT_GATE:EDIT_TIME;int value=getCurrentEditedValue();editMode=old;return value;}
FLASHMEM void blockDisplay21(){
 const char*label[16]={"OOV","VEL","GAT","PBN","MOT","CHA","TIM","NOT","MOT","CHA","TIM","NOT","MOT","CHA","TIM","NOT"};
 for(uint8_t screen=0;screen<4;++screen){char a[17]="                ",b[17]="                ";uint16_t blink=0;
 for(uint8_t col=0;col<4;++col){uint8_t k=screen*4+col;char v[12];memcpy(a+col*4,label[k],3);
 if(k==7)noteLabel37(v,sizeof(v),quickTrack903,blockNoteDisplay21(0));else if(k==6)snprintf(v,sizeof(v),"%d",blockNoteDisplay21(3));else if(k==0){int g=lockDisplay817(LOCK_BLOCK21,quickTrack903,12,blocks21[quickTrack903][0].groove);if(!g)strcpy(v,"OFF");else snprintf(v,sizeof(v),"%d",g);}else if(k==1)snprintf(v,sizeof(v),"%u",blockNoteDisplay21(1));else if(k==2){if(gateIncrementShown15)snprintf(v,sizeof(v),"%s",gateEditMode==GATE_EDIT_1T?"fin":"\x14");else snprintf(v,sizeof(v),"%u",blockNoteDisplay21(2));}else if(k==3){if(pitchBendStepShown44)snprintf(v,sizeof(v),"%s",pitchBendStep44==0?"fin":pitchBendStep44==1?"+10":"+100");else snprintf(v,sizeof(v),"%d",trackPitchBend[quickTrack903]);}else{uint8_t blk=(k-4)/4,f=(k-4)%4;auto&r=blocks21[linkOwner12(quickTrack903,LINK_MOUT12)][blk];int base=f==0?r.port:f==1?r.channel:f==2?r.tim:r.transpose;int value=lockDisplay817(LOCK_BLOCK21,quickTrack903,blk*4+f,base);if(value!=base)blink|=15u<<(col*4);if(f==0&&!value)strcpy(v,"OFF");else if(f==3)snprintf(v,sizeof(v),"%+d",value);else snprintf(v,sizeof(v),"%d",value+(f==1));}memcpy(b+col*4,v,min((size_t)4,strlen(v)));}
 lockBlinkSend11(screen,blink);DISPLAY_SERIAL.printf("MODLCD|%u|%s|%s\n",screen,a,b);}
}
FLASHMEM bool blockKy21(uint8_t k,int32_t delta){if(k>=16)return false;uint8_t t=quickTrack903;
 if(k==7||k==6){
  if(remixMode&&remixSelection911.steps)remixEditDelta911(k==7?0:3,delta);
  else {BlockScope21 scope(0);stopNoteAuditions();applyNoteVelGateTimeDelta(k==7?2:5,delta);}
  displayValueDirty=true;return true;
 }
 if(k==0){auto&r=blocks21[t][0];if(remixMode&&remixSelection911.steps)remixEditDelta911(44,delta);else if(!lockEdit817(LOCK_BLOCK21,t,12,r.groove,delta,0,listCount44(t))){r.groove=WRAP23(r.groove,delta,0,listCount44(t));for(uint8_t b=1;b<3;++b)blocks21[t][b].groove=0;storageDirty931=true;++modEpoch805;remixScheduleDirty911=true;}displayValueDirty=true;return true;}
 if(remixMode&&remixSelection911.steps&&k!=3){remixEditDelta911(k<3?k:32+k-4,k==0?-delta:k==2?delta*(gateEditMode==GATE_EDIT_1T?1:96):delta);return true;}
 if(k<4){if(k==2&&delta)gateIncrementShown15=false;uint8_t legacy=k==3?6:k+2;if(k==3)changePitchBendFromKy(delta);else applyNoteVelGateTimeDelta(legacy,delta);displayValueDirty=true;return true;}
 uint8_t b=(k-4)/4,f=(k-4)%4;auto&r=blocks21[linkOwner12(t,LINK_MOUT12)][b];int value=f==0?r.port:f==1?r.channel:f==2?r.tim:r.transpose;int lo=f==2?-96:f==3?-127:0,hi=f==0?8:f==1?15:f==2?96:127;
 if(!lockEdit817(LOCK_BLOCK21,t,b*4+f,value,delta,lo,hi)){value=WRAP23(value,delta,lo,hi);if(f==0)r.port=value;if(f==1)r.channel=value;if(f==2)r.tim=value;if(f==3)r.transpose=value;pidiRoutesDirty931|=1u<<t;storageDirty931=true;remixScheduleDirty911=true;++modEpoch805;}
 if(f==3&&delta&&notePreviewForTrack(t))blockAudition34(t,b);
 displayValueDirty=true;return true;
}
FLASHMEM bool blockClick21(uint8_t k,bool down){if(k>=16)return false;bool click=!down&&kyBtnDown[k]&&!kyRotatedWhileDown[k]&&!kyHelpShown[k]&&!numericEntryActive;uint32_t held=millis()-kyBtnDownAtMs[k];if(down&&!kyBtnDown[k]){kyBtnDownAtMs[k]=millis();kyRotatedWhileDown[k]=kyHelpShown[k]=false;}kyBtnDown[k]=down;if(!down){if(kyHelpShown[k])sendKyHelp(k,false);kyHelpShown[k]=false;gateHoldValueShown=false;}if(!click)return true;
 if(k==11||k==15){trackNotePreviewMode[quickTrack903]=notePreviewForTrack(quickTrack903)?0:1;return true;}
 if((k>0&&k<4)||k==6||k==7){QuickContext903 context;modUi805=0;patchBank808=false;toggleEditFastFromKy(k==7?2:k==6?5:k==3?6:k+2,held);}
 else if(k==0){auto&r=blocks21[quickTrack903][0];if(!r.groove||r.groove>listCount44(quickTrack903)){if(listCount44(quickTrack903)>=BLOCK_LISTS21){DISPLAY_SERIAL.println("MSG|9 GROOVE LISTS MAX");return true;}r.groove=++listCount44(quickTrack903);memset(listBank44(quickTrack903)[r.groove-1],0,sizeof(listBank44(quickTrack903)[0]));}blockListEdit21=r.groove-1;quickView903=0;grooveEditMode=patternMode=true;grooveListRepeatEditing=grooveListSelection805=0;grooveListFocus40(selectedTrack,activePattern[selectedTrack]);blockListDisplay21();storageDirty931=true;}
 displayValueDirty=true;return true;
}
FLASHMEM void blockListDisplay21(){if(grooveEditMode&&patternMode){grooveListDisplay40();return;}DISPLAY_SERIAL.printf("GTRACK35,%u,%u\n",selectedTrack+1,blockListEdit21+1);DISPLAY_SERIAL.printf("MSG|GROOVE LIST %u\n",blockListEdit21+1);DISPLAY_SERIAL.printf("GLSEL,%u\n",grooveListSelection805);DISPLAY_SERIAL.print("GLIST,");DISPLAY_SERIAL.print(grooveListRepeatEditing);for(auto&e:listBank44(selectedTrack)[blockListEdit21]){DISPLAY_SERIAL.print(',');DISPLAY_SERIAL.print(e.sheet);DISPLAY_SERIAL.print(',');DISPLAY_SERIAL.print(max((uint8_t)1,e.repeats));}DISPLAY_SERIAL.println();DISPLAY_SERIAL.printf("FIELD,14,T,List\nFIELD,14,V,%u\nFIELD,15,T,Add\n",blockListEdit21+1);}
FLASHMEM bool blockListEditValue21(uint8_t k,int32_t delta){if(k==14){blockListEdit21=WRAP23(blockListEdit21,delta,0,max(0,(int)listCount44(selectedTrack)-1));if(listCount44(selectedTrack)){blocks21[selectedTrack][0].groove=blockListEdit21+1;editedGroove=grooveSheet35(selectedTrack);++modEpoch805;storageDirty931=true;}blockListDisplay21();return true;}if(k>=14)return true;if(!listCount44(selectedTrack))listCount44(selectedTrack)=1;auto&e=listBank44(selectedTrack)[blockListEdit21][k];if(grooveListRepeatEditing&(1u<<k))e.repeats=WRAP23(max((uint8_t)1,e.repeats),delta,1,99);else{e.sheet=WRAP23(e.sheet,delta,0,GROOVE_COUNT);if(!e.repeats)e.repeats=1;}++modEpoch805;storageDirty931=true;remixScheduleDirty911=true;blockListDisplay21();return true;}

// VA01.30: classify PLX dependencies once per agenda slice/registry epoch.
// MIDI-only locks and modulations do not change note scheduling. BLOCK/ROLL
// locks remain conservative for their source AND destination tracks.

static uint32_t blockDependencyEpoch30=UINT32_MAX,blockDependencySlice30=UINT32_MAX;
static uint16_t blockDependencyCount30=UINT16_MAX,blockDependencyLease30=UINT16_MAX;
static bool blockDependencyPlayback30=false;
static uint16_t blockTimingTracks30=0;
static uint8_t blockPortLocks30[16]={};
static uint16_t blockNoteFieldMasks30[16]={};
static uint32_t staticDepsEpoch31=UINT32_MAX;
static uint16_t staticDepsCount31=UINT16_MAX,staticTiming31=0,staticFields31[16];
static bool staticPlayback31=false;static uint8_t staticPorts31[16];
FLASHMEM void blockDependencies30(){
 if(blockDependencyEpoch30==modEpoch805&&blockDependencyCount30==lockPlaybackCount39()&&blockDependencyLease30==lockLeaseCount817&&blockDependencyPlayback30==playbackLocks828)return;
 Scope830 depsMeasure30(P830_DEPS30);blockTimingTracks30=0;memset(blockPortLocks30,0,sizeof(blockPortLocks30));memset(blockNoteFieldMasks30,0,sizeof(blockNoteFieldMasks30));
 if(playbackLocks828){
  bool staticSame31=(optim31&2)&&staticDepsEpoch31==registryEpoch31()&&staticDepsCount31==lockPlaybackCount39()&&staticPlayback31==playbackLocks828;
  if(staticSame31){blockTimingTracks30=staticTiming31;memcpy(blockPortLocks30,staticPorts31,sizeof(staticPorts31));memcpy(blockNoteFieldMasks30,staticFields31,sizeof(staticFields31));}
  else {
  for(uint16_t i=0;i<lockPlaybackCount39();++i){auto&l=lockPlaybackEntry39(i);
   if(l.kind!=LOCK_BLOCK21&&l.kind!=LOCK_ROLL1000)continue;
   if(l.kind==LOCK_BLOCK21&&l.track<16&&l.target==l.track&&l.parameter<13)blockNoteFieldMasks30[l.track]|=1u<<l.parameter;
   if(l.track<16)blockTimingTracks30|=1u<<l.track;
   if(l.target<16)blockTimingTracks30|=1u<<l.target;
   if(l.kind==LOCK_BLOCK21&&l.target<16&&l.parameter<12&&l.parameter%4==0&&l.value>0)blockPortLocks30[l.target]|=1u<<(l.parameter/4);
  }
  staticTiming31=blockTimingTracks30;memcpy(staticPorts31,blockPortLocks30,sizeof(staticPorts31));memcpy(staticFields31,blockNoteFieldMasks30,sizeof(staticFields31));
  staticDepsEpoch31=registryEpoch31();staticDepsCount31=lockPlaybackCount39();staticPlayback31=playbackLocks828;
  }
  for(auto&l:lockLeases817){if(!l.used||(l.kind!=LOCK_BLOCK21&&l.kind!=LOCK_ROLL1000)||l.target>=16)continue;
   blockTimingTracks30|=1u<<l.target;
   if(l.kind==LOCK_BLOCK21&&l.parameter<12&&l.parameter%4==0&&l.value>0)blockPortLocks30[l.target]|=1u<<(l.parameter/4);
  }
 }
 if(autoKinds46&((1u<<LOCK_BLOCK21)|(1u<<LOCK_ROLL1000)))for(const auto&l:autoLanes46)if(l.used&&l.type==1&&(l.kind==LOCK_BLOCK21||l.kind==LOCK_ROLL1000)){blockTimingTracks30|=1u<<l.track;if(l.kind==LOCK_BLOCK21&&l.parameter<13){blockNoteFieldMasks30[l.track]|=1u<<l.parameter;if(l.parameter<12&&l.parameter%4==0)blockPortLocks30[l.track]|=1u<<(l.parameter/4);}}
 blockDependencyEpoch30=modEpoch805;blockDependencySlice30=agendaSlice830;blockDependencyCount30=lockPlaybackCount39();blockDependencyLease30=lockLeaseCount817;blockDependencyPlayback30=playbackLocks828;
}
FLASHMEM uint16_t blockNoteFields30(uint8_t t){blockDependencies30();return blockNoteFieldMasks30[t];}
FLASHMEM uint8_t blockActiveMask30(uint8_t t){
 blockDependencies30();uint8_t mask=blockPortLocks30[t];
 for(uint8_t b=0;b<3;++b)if(blocks21[linkOwner12(t,LINK_MOUT12)][b].port)mask|=1u<<b;
 return mask;
}

// Bounded neighbourhood: independent of track length. Includes negative TIM and roll anticipation.
static uint32_t blockDueSlice29[16],blockDueEpoch29[16];
static int64_t blockDue29[16];
static uint32_t blockBridgeEpoch49[16];
static uint8_t blockPattern49[16];
FLASHMEM void blockProcessTrack21(uint8_t t,uint32_t local){
 const int64_t now=transportTickAbsolute;const uint32_t length=trackSequenceTicks(t);if(!length)return;const int32_t mov41=defaultMov41[t][0];if(mov41)local=((int64_t)local-mov41%((int32_t)length)+length)%length;
 // Input edits are outside the slice; epoch guards cover live lease changes.
 // Tracks with timing PLX keep the complete scheduler; other tracks cache.
 const uint8_t activeMask=blockTargeted30?blockActiveMask30(t):7;
 // A queued transition does not change the current notes on every tick.
 // Re-evaluate when its boundary/phase/state changes, rather than disabling
 // the next-event cache for the entire waiting period and first incoming cycle.
 bool cache=blockOptimize29&&agendaActive822&&(blockTargeted30?!(blockTimingTracks30&(1u<<t)):(!modEnabledCount805&&!lockPlaybackCount39()&&!lockLeaseCount817));
 if(cache&&blockDueSlice29[t]==agendaSlice830&&blockDueEpoch29[t]==modEpoch805&&blockBridgeEpoch49[t]==bridgeEpoch49[t]&&blockPattern49[t]==activePattern[t]&&now<blockDue29[t])return;
 blockCollectDue29=cache;blockNextDue29=now+96-local%96;
 uint16_t cachedStep[14];int64_t cachedHead[14];uint8_t cachedCount[14]={};
 if(blockOptimize29)for(int i=0;i<14;++i){int64_t head=now-local%96+(i-5)*96;cachedHead[i]=head;cachedStep[i]=65535;if(head<0)continue;uint32_t logical=mov41?((int64_t)local-local%96+(i-5)*96+length)%length:(bridgeMasterTickAt(head)+length-grooveTrackPhase[t]%length)%length;if(!(logical%96)&&logical/96<trackStepCount(t))cachedStep[i]=logical/96;}
 if(blockOptimize29){
  for(uint8_t i=0;i<14;++i)if(cachedStep[i]!=65535)cachedCount[i]=baseVoiceCountAtStep(t,cachedStep[i]);
  // Count the sparse fine-note registry once, rather than scan it for every
  // candidate, every loop condition and each of the three blocks.
  for(uint8_t f=0;f<FINE_EVENTS_PER_TRACK;++f)if(fineNote[t][f].active)for(uint8_t i=0;i<14;++i)if(cachedStep[i]==fineNote[t][f].step&&cachedCount[i]<255)++cachedCount[i];
 }
 for(uint8_t b=0;b<3;++b){
 // A port PLX or a lease may activate a base-OFF block: retain the full path then.
 if(blockOptimize29&&(blockTargeted30?!(activeMask&(1u<<b)):(!lockPlaybackCount39()&&!lockLeaseCount817&&!blocks21[linkOwner12(t,LINK_MOUT12)][b].port)))continue;
 BlockScope21 scope(b);Block21 cachedAddress31;uint32_t cachedAddressEpoch31=0;bool cachedAddressValid31=false;
 for(int offset=-5;offset<=8;++offset){int64_t head;uint16_t step;
 if(blockOptimize29){head=cachedHead[offset+5];step=cachedStep[offset+5];if(step==65535)continue;}
 else{head=now-local%96+offset*96;if(head<0)continue;uint32_t master=bridgeMasterTickAt(head);uint32_t logical=mov41?((int64_t)local-local%96+offset*96+length)%length:(master+length-grooveTrackPhase[t]%length)%length;if(logical%96)continue;step=logical/96;if(step>=trackStepCount(t))continue;}
 const uint8_t count=blockOptimize29?cachedCount[offset+5]:noteCountAtStep(t,step);
 for(uint8_t v=0;v<count;++v){if(pendingDeleteNote(t,step,v)||previewSuppressesVoice(t,step,v))continue;if(v&&v<baseVoiceCountAtStep(t,step)&&trackPlayMode[linkOwner12(t,LINK_POLY12)]==TRACK_PLAY_MONO)continue;
 int32_t identity=step*96+logicalMicroTick(t,step,v);uint8_t pitch=noteAtVoice(t,step,v);
 bool addressConstant31=(optim31&4)&&(!playbackLocks828||!(blockNoteFields30(t)&((15u<<(b*4))|4096u)));
 if(addressConstant31&&cachedAddressValid31&&cachedAddressEpoch31==modEpoch805){blockResolved21=cachedAddress31;++sharedHits31;}
 else{blockResolved21=blockAtNote21(t,b,activePattern[t],identity,pitch);if(addressConstant31){cachedAddress31=blockResolved21;cachedAddressEpoch31=modEpoch805;cachedAddressValid31=true;}}
 blockResolvedValid21=true;if(!blockResolved21.port)continue;
 grooveEventTrack=t;grooveEventSheet=grooveSheet40(t,activePattern[t],blockResolved21.groove,head>=grooveListOrigin[t]?(head-grooveListOrigin[t])/1536:0);modLogicalTick805=head;
 int64_t target=head+audibleMicroTick(t,step,v)-mov41+blockResolved21.tim;
 if(grooveBridgeReady[t]&&!grooveBridgeSwitched[t]&&head>=(int64_t)grooveBridgeAt[t])continue;if(grooveBridgeSwitched[t]&&head<(int64_t)grooveBridgeAt[t])continue;
 blockObserveDue29(target,now);playListRoll(t,step,v,target,now);if(target==now)triggerFineEvent(t,step,v);
 }}}
 grooveEventTrack=255;grooveEventSheet=-1;modLogicalTick805=-1;
 blockCollectDue29=false;if(cache){blockDueSlice29[t]=agendaSlice830;blockDueEpoch29[t]=modEpoch805;blockBridgeEpoch49[t]=bridgeEpoch49[t];blockPattern49[t]=activePattern[t];blockDue29[t]=blockNextDue29;}
}

struct BlockDisk21 {uint32_t magic=0x3400424c;uint8_t count=0;Block21 blocks[16][3];BlockListEntry21 lists[64][14];};
EXTMEM BlockDisk21 blockStage21;
EXTMEM ListBank44 listStage44[16];
static uint8_t listStageCounts44[16];static bool legacyLists44=false;
uint8_t blockStageCount40(uint8_t t){return t<16?listStageCounts44[t]:0;}
FLASHMEM bool blockWrite21(File&f){
 uint32_t magic=0x5800424c;
 if(f.write((uint8_t*)&magic,4)!=4||f.write((uint8_t*)blocks21,sizeof(blocks21))!=sizeof(blocks21)||f.write(trackListCounts44,16)!=16)return false;
 for(uint8_t t=0;t<16;++t){size_t n=listCount44(t)*sizeof(listBank44(t)[0]);if(f.write((uint8_t*)listBank44(t),n)!=n)return false;}return true;
}
FLASHMEM bool blockRead21(File&f,bool install){
 auto&d=blockStage21;uint32_t at=f.position(),magic=0;if(f.read((uint8_t*)&magic,4)!=4)return false;
 memset(listStage44,0,sizeof(listStage44));memset(listStageCounts44,0,16);legacyLists44=magic==0x3400424c;
 if(legacyLists44)return false;
 if(magic==0x5800424c){if(f.read((uint8_t*)d.blocks,sizeof(d.blocks))!=sizeof(d.blocks)||f.read(listStageCounts44,16)!=16)return false;for(uint8_t t=0;t<16;++t){if(listStageCounts44[t]>BLOCK_LISTS21)return false;size_t n=listStageCounts44[t]*sizeof(listStage44[t][0]);if(f.read((uint8_t*)listStage44[t],n)!=(int)n)return false;}}
 else return false;
 for(uint8_t t=0;t<16;++t){for(auto&b:d.blocks[t])if(b.transpose< -127||b.port>8||b.channel>15||b.tim< -96||b.tim>96||b.groove>listStageCounts44[t])return false;for(auto&l:listStage44[t])for(auto&e:l)if(e.sheet>GROOVE_COUNT||e.repeats>99)return false;}
 if(install)blockInstall21();return true;
}
FLASHMEM void blockInstall21(){memcpy(blocks21,blockStage21.blocks,sizeof(blocks21));memcpy(trackLists44,listStage44,sizeof(trackLists44));memcpy(trackListCounts44,listStageCounts44,16);blockListEdit21=0;pidiRoutesDirty931=0xffff;}
FLASHMEM void listMigrate44(){
 if(!legacyLists44)return;legacyLists44=false;
 uint8_t maps[16][65]={};bool used[65]={};
 for(uint8_t t=0;t<16;++t){uint8_t next=0;auto map=[&](uint8_t old){if(old&&old<=listStageCounts44[t]&&!maps[t][old]){maps[t][old]=++next;used[old]=true;}return maps[t][old<=64?old:0];};
  for(uint8_t b=0;b<3;++b)blocks21[t][b].groove=map(blocks21[t][b].groove);
  for(uint8_t p=0;p<16;++p)if(patternGroove[t][p]==(GROOVE_COUNT+2))grooveLists[t][p][0].sheet=map(grooveLists[t][p][0].sheet);
  for(uint16_t i=0;i<lockCount817;++i){auto&l=noteLocks817[i];if(l.kind==LOCK_BLOCK21&&l.parameter==12&&l.target==t)l.value=map(clampInt(l.value,0,64));}
  listCount44(t)=next;
 }
 // Preserve formerly unassigned lists on track 1 instead of silently deleting them.
 for(uint8_t old=1;old<=listStageCounts44[0];++old)if(!used[old])maps[0][old]=++listCount44(0);
 memset(trackLists44,0,sizeof(trackLists44));for(uint8_t t=0;t<16;++t)for(uint8_t old=1;old<=BLOCK_LISTS21;++old)if(maps[t][old])memcpy(listBank44(t)[maps[t][old]-1],listStage44[t][old-1],sizeof(listBank44(t)[0]));
}

FLASHMEM void blockPatchChoose21(){uint32_t ids[48];uint8_t bs[48],count=0,old=patchBlock21;for(uint8_t b=0;b<3;++b){patchBlock21=b;auto&c=patch808Context(selectedTrack,activePattern[selectedTrack]);for(uint8_t slot=0;slot<16;++slot){uint32_t id=patchId819(c,slot);if(id&&patch808Find(patchDb808,id)){ids[count]=id;bs[count++]=b;}}}patchBlock21=old;uint8_t at=0;if(quickView903==6&&quickTrack903==selectedTrack){while(at<count&&(ids[at]!=quickItem903||bs[at]!=quickPatchBlock21))++at;at=count?(at+1)%count:0;}quickView903=6;quickTrack903=selectedTrack;quickPattern903=activePattern[selectedTrack];quickItem903=count?ids[at]:UINT32_MAX;quickPatchBlock21=count?bs[at]:0;quickPatchPage903=0;kbZoneViewR11=false;displayValueDirty=true;quickDisplay903();}

FLASHMEM bool blockImportTrack21(uint8_t t,const Block21*bs,const BlockListEntry21 (*lists)[14],uint8_t count,bool commit){if(t>=16||count>BLOCK_LISTS21)return false;for(uint8_t l=0;l<count;++l)for(auto&e:lists[l])if(e.sheet>GROOVE_COUNT||e.repeats>99)return false;uint8_t map[65]={},needed=0;for(uint8_t b=0;b<3;++b){auto&v=bs[b];if(v.transpose< -127||v.port>8||v.channel>15||v.tim< -96||v.tim>96||v.groove>count)return false;uint8_t id=v.groove;if(!id||map[id])continue;for(uint8_t l=0;l<listCount44(t);++l)if(!memcmp(lists[id-1],listBank44(t)[l],sizeof(listBank44(t)[0]))){map[id]=l+1;break;}if(!map[id]){if(listCount44(t)+needed>=BLOCK_LISTS21)return false;map[id]=listCount44(t)+(++needed);}}if(!commit)return true;for(uint8_t id=1;id<=count;++id)if(map[id]>listCount44(t))memcpy(listBank44(t)[map[id]-1],lists[id-1],sizeof(listBank44(t)[0]));listCount44(t)+=needed;for(uint8_t b=0;b<3;++b){blocks21[t][b]=bs[b];blocks21[t][b].groove=map[bs[b].groove];}pidiRoutesDirty931|=1u<<t;return true;}

FLASHMEM PatchContext808& blockPatchContext21(uint8_t t,uint8_t b){uint8_t p=patch808PlayingPattern(t);uint8_t owner=linkOwner12(t,LINK_PATCH12);auto&own=patchDb808.context[owner][b*17+p+1];return own.patch||own.explicit819?own:patchDb808.context[owner][b*17];}
FLASHMEM PatchDef808* blockPatchAt21(uint8_t t,int index,uint8_t&b){int at=0;for(b=0;b<3;++b){auto&c=blockPatchContext21(t,b);for(uint8_t slot=0;slot<16;++slot){auto*d=patch808Find(patchDb808,patchId819(c,slot));if(d&&at++==index)return d;}}b=0;return nullptr;}
FLASHMEM PatchParam808* modTrackParam21(const ModRoute805&r){if(r.track>=16||r.block21>=3)return nullptr;auto&c=blockPatchContext21(r.track,r.block21);if(!patchHas819(c,r.patch21))return nullptr;auto*d=patch808Find(patchDb808,r.patch21);for(auto*q=d?d->params:nullptr;q;q=q->next)if(q->id==r.param21&&q->kind!=2)return q;return nullptr;}
FLASHMEM ModRoute805 modTrackDraft21(const int*f,const ModRoute805&base){auto r=base;r.kind=MOD_TRACK21;r.track=clampInt(f[1],0,15);r.port=0;r.patch21=r.param21=0;auto*d=blockPatchAt21(r.track,f[2],r.block21);auto*q=patchEditableAt44(d,f[3]);if(d&&q){r.patch21=d->id;r.param21=q->id;r.number=0;}r.depth=clampInt(r.depth,-16383,16383);return r;}
FLASHMEM void modTrackFields21(const ModRoute805&r,int*f){f[0]=4;f[1]=r.track;f[2]=f[3]=0;for(int i=0;i<48;++i){uint8_t b;auto*d=blockPatchAt21(r.track,i,b);if(!d)break;if(d->id==r.patch21&&b==r.block21){f[2]=i;for(auto*q=d->params;q&&q->id!=r.param21;q=q->next)if(q->kind!=2)++f[3];break;}}}
FLASHMEM void modTrackRender21(const int*f,char fields[4][12],char*c){uint8_t b;auto*d=blockPatchAt21(f[1],f[2],b);auto*q=patchEditableAt44(d,f[3]);strcpy(fields[0],"Piste");snprintf(fields[1],12,"%02u",f[1]+1);snprintf(fields[2],12,"%.5s",d?d->name:"None");snprintf(fields[3],12,"%.5s",q?q->name:"None");snprintf(c,21,"BLK %u %.13s",b+1,d?d->name:"Assign a patch");}
FLASHMEM bool modTrackBase21(const ModRoute805&r,int delta,int&value,const char*&label){auto*q=modTrackParam21(r);if(!q)return false;auto&c=blockPatchContext21(r.track,r.block21);uint16_t base=0;patchValue819(c,*q,base);value=clampInt(base+delta,0,q->bits==14?16383:127);label=q->shortName;if(lockWantsEdit817()){uint8_t kind=q->kind?(q->bits==14?LOCK_NRPN14817:LOCK_NRPN7817):LOCK_CC817;if(delta)lockEdit817(kind,r.track,q->number,base,delta,0,q->bits==14?16383:127);value=lockDisplay817(kind,r.track,q->number,base);targetLocked11=value!=base;return true;}if(delta){BlockScope21 scope(r.block21);if(!patchSetValue819(c,*q,value))return false;MidiScope902 route(outMask902(r.track),r.track);patch808SendParam(midiChannelForTrack(r.track),*q,value);storageDirty931=true;++modEpoch805;}return true;}
FLASHMEM ModRoute805 modTrackResolve21(ModRoute805 r){auto*q=modTrackParam21(r);if(!q||q->kind){r.enabled=0;return r;}auto b=blockAtNote21(r.track,r.block21,255,-1,255);uint16_t value=0;patchValue819(blockPatchContext21(r.track,r.block21),*q,value);r.enabled=r.enabled&&b.port;r.kind=MOD_CC;r.number=q->number;r.port=b.port;r.channel=r.track=b.channel;r.reference=value;return r;}
struct ModMix21 {float sum=0;uint16_t number=0,base=0,count=0;uint8_t port=0,channel=0,kind=0,bits=7;};
EXTMEM ModMix21 modMix21[512],modMixPrevious21[512];uint16_t modMixPreviousCount21=0;uint16_t modMixCount21=0;
FLASHMEM int nrpnBase54(uint8_t port,uint8_t ch,uint16_t number,uint8_t bits,int fallback){for(unsigned i=0;i<modMixCount21;++i){auto&a=modMix21[i];if(a.port==port+1&&a.channel==ch&&a.number==number&&a.bits==bits)return a.base;}return fallback;}
FLASHMEM int nrpnOffset54(uint8_t port,uint8_t ch,uint16_t number,uint8_t bits){for(unsigned i=0;i<modMixCount21;++i){auto&a=modMix21[i];if(a.port==port+1&&a.channel==ch&&a.number==number&&a.bits==bits&&a.count)return lroundf(a.sum/a.count);}return 0;}
FLASHMEM void modTrackCollect21(int64_t at){modMixCount21=0;for(uint16_t n=0;n<modEnabledCount805;++n){uint16_t key=modEnabled805[n];if(remoteOwn54(key))continue;uint16_t owners50=indModOwners50(key);for(unsigned pass50=0;pass50<17;++pass50){if(owners50?(pass50==16||!(owners50&(1u<<pass50))):pass50!=16)continue;IndContext50 context50(pass50<16?pass50:-1);if(!playbackMod828(key)||((!playing||!mod806Playing(key,at))&&!ccActive941(key)&&!ioActive38(key)&&!(modKind931(key)&&modMode810(key)==2)&&!manualModActive06(key)))continue;auto m=mod808EffectiveUnit(key);for(uint8_t j=0;j<8;++j){auto&r=m.outputs[j];if(!r.enabled||r.kind!=MOD_TRACK21)continue;auto*q=modTrackParam21(r);if(!q||!q->kind)continue;auto b=blockAtNote21(r.track,r.block21,255,-1,255);if(!b.port)continue;uint16_t base=0;patchValue819(blockPatchContext21(r.track,r.block21),*q,base);float depth=modDepth810(key,j,at);if(!depth)continue;uint16_t i=0;while(i<modMixCount21){auto&a=modMix21[i];if(a.port==b.port&&a.channel==b.channel&&a.number==q->number&&a.kind==q->kind&&a.bits==q->bits)break;++i;}if(i==modMixCount21){if(i>=512)continue;modMix21[modMixCount21++]={0,q->number,base,0,b.port,b.channel,q->kind,q->bits};}auto&a=modMix21[i];a.sum+=modRouteValues46[key][j]*depth;++a.count;}}}
 autoNrpn46();
 for(uint16_t i=0;i<modMixPreviousCount21;++i){auto&a=modMixPrevious21[i];bool found=false;for(uint16_t j=0;j<modMixCount21;++j){auto&b=modMix21[j];if(a.port==b.port&&a.channel==b.channel&&a.number==b.number&&a.kind==b.kind&&a.bits==b.bits){found=true;break;}}if(!found){MidiScope902 route(1u<<(a.port-1));if(a.kind)midiNrpn810(a.channel,a.number,a.bits,a.base);else midiOffer810(a.channel,a.number,a.base,true);}}
 modMixPreviousCount21=modMixCount21;memcpy(modMixPrevious21,modMix21,modMixCount21*sizeof(ModMix21));
 for(uint16_t i=0;i<modMixCount21;++i){auto&a=modMix21[i];int v=clampInt(a.base+lroundf(a.sum/max((uint16_t)1,a.count)),0,a.bits==14?16383:127);MidiScope902 route(1u<<(a.port-1));if(a.kind){nrpnIsMod54=true;midiNrpn810(a.channel,a.number,a.bits,v);nrpnIsMod54=false;}else midiOffer810(a.channel,a.number,v,true);}}

FLASHMEM bool modTrackPending21(){return modMixPreviousCount21!=0;}
FLASHMEM void modTrackStop21(){for(uint16_t i=0;i<modMixPreviousCount21;++i){auto&a=modMixPrevious21[i];MidiScope902 route(1u<<(a.port-1));midiNrpn810(a.channel,a.number,a.bits,a.base);}modMixPreviousCount21=0;}




FLASHMEM int blockPitch34(uint8_t t,uint8_t note){auto b=blockResolvedValid21?blockResolved21:blockAtNote21(t,blockIndex21(),255,-1,255);return clampInt((int)note+b.transpose,0,127);}
FLASHMEM void blockAudition34(uint8_t t,uint8_t b){stopNoteAuditions();BlockScope21 scope(b);blockResolved21=blockAtNote21(t,b,255,-1,255);blockResolved21.transpose=lockDisplay817(LOCK_BLOCK21,t,b*4+3,blockResolved21.transpose);blockResolvedValid21=true;if(blockResolved21.port)triggerNoteAudition(t,(uint8_t)blockNoteDisplay21(0));}

FLASHMEM bool blockMusicalField35(const NoteLock817&l){return l.kind==LOCK_BLOCK21&&l.parameter<12&&(l.parameter%4==2||l.parameter%4==3);}
FLASHMEM bool noteHasPlx35(uint8_t t,uint16_t s,uint8_t v){for(int i=lockEventFirst30(t,activePattern[t],lockTick817(t,s,v),noteAtVoice(t,s,v));i>=0;i=lockEventNext30(i))if(lockEventSame30(lockPlaybackEntry39(i),t,activePattern[t],lockTick817(t,s,v),noteAtVoice(t,s,v))&&!blockMusicalField35(lockPlaybackEntry39(i)))return true;return false;}
