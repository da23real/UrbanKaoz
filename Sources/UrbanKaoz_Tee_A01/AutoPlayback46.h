#pragma once
FLASHMEM bool autoMuted46(uint8_t t,uint8_t p,uint16_t key,uint8_t slot,uint32_t tick){
 for(uint16_t i=0;i<autoMuteCount46;++i){auto&m=autoMutes46[i];if(m.track==t&&m.pattern==p&&m.key==key&&m.slot==slot&&autoContains49(m,tick,trackSequenceTicks(t)))return true;}return false;
}
FLASHMEM int autoScopedDepth46(uint8_t t,uint8_t p,uint16_t key,uint8_t slot,int32_t identity,uint8_t note,int base){
 int value=base;for(uint16_t i=0;i<autoMuteCount46;++i){auto&q=autoMutes46[i];if(q.track==t&&q.pattern==p&&q.key==key&&q.slot==slot&&q.note==128&&((playing?localTickForTrack(t):(uint32_t)max(0,(int)identity))>=q.tick)&&((playing?localTickForTrack(t):(uint32_t)max(0,(int)identity))<q.end))value=q.depth;}
 for(uint16_t i=0;i<autoMuteCount46;++i){auto&q=autoMutes46[i];if(q.track==t&&q.pattern==p&&q.key==key&&q.slot==slot&&q.note==note&&q.identity==identity)value=q.depth;}return value;
}
FLASHMEM bool autoLive46(uint16_t key,uint8_t slot,int64_t at,const ModUnit805&m,float&value,float&depth){
 if(autoRenderTrack46>=0||!playing||!autoMuteCount46)return true;
 uint16_t owners=0,scoped=0;for(uint8_t t=0;t<16;++t)if((indContext50<0||indContext50==t)&&modAssociated12(key,t)&&!trackSilent818(t)){owners|=1u<<t;for(uint16_t i=0;i<autoMuteCount46;++i){auto&q=autoMutes46[i];if(q.track==t&&q.pattern==patch808PlayingPattern(t)&&q.key==key&&q.slot==slot)scoped|=1u<<t;}}
 if(!scoped)return true;
 uint8_t mode=modMode810(key);if(mode==1||mode==2||mode==3||mode==4){float sum=0;uint8_t count=0;for(uint8_t t=0;t<16;++t)if(owners&(1u<<t)){int d=autoScopedDepth46(t,patch808PlayingPattern(t),key,slot,-1,128,lroundf(depth));uint32_t local=localTickForTrack(t);for(uint16_t i=0;i<autoMuteCount46;++i){auto&q=autoMutes46[i];if(q.track==t&&q.pattern==patch808PlayingPattern(t)&&q.key==key&&q.slot==slot&&q.note<128&&autoContains49(q,local,trackSequenceTicks(t)))d=q.depth;}if(d){sum+=d;++count;}}depth=count?sum/count:0;return depth!=0;}
 int64_t latest=INT64_MIN/2;uint32_t gate=0;float chosen=depth;
 for(uint8_t t=0;t<16;++t)if((owners&m.inputs)&(1u<<t)){
  auto source=mod805Source(t,at);float candidate=depth;
  if(scoped&(1u<<t)){
   source.begin=INT64_MIN/2;source.gate=0;uint32_t len=trackSequenceTicks(t);int64_t origin=at-localTickForTrack(t);
   for(uint16_t s=0;s<trackStepCount(t);++s)for(uint8_t v=0;v<noteCountAtStep(t,s);++v){int d=autoScopedDepth46(t,patch808PlayingPattern(t),key,slot,lockTick817(t,s,v),noteAtVoice(t,s,v),lroundf(depth));if(!d)continue;
    int32_t placed=((int32_t)s*96+audibleMicroTick(t,s,v))%len;if(placed<0)placed+=len;int64_t onset=origin+placed;if(onset>at)onset-=len;if(onset<0)continue;uint32_t g=audibleGateTicks(t,s,v);if(onset>=source.begin){source.gate=onset==source.begin?max(source.gate,g):g;source.begin=onset;candidate=d;}
   }
  }
  if(source.begin>latest){latest=source.begin;gate=source.gate;chosen=candidate;}else if(source.begin==latest)gate=max(gate,source.gate);
 }
 if(latest<0){value=0;return false;}depth=chosen;value=modValue810(m,modKind931(key),at,key,latest,gate,false);return true;
}
FLASHMEM int autoDepthDisplay46(uint16_t key,uint8_t slot,int base){
 int32_t tick=-1;uint8_t note=128;if(selectedStep>=0&&selectedVoice<noteCountAtStep(selectedTrack,selectedStep)){tick=lockTick817(selectedTrack,selectedStep,selectedVoice);note=noteAtVoice(selectedTrack,selectedStep,selectedVoice);}else for(uint16_t s=0;s<trackStepCount(selectedTrack)&&note==128;++s)for(uint8_t v=0;v<noteCountAtStep(selectedTrack,s);++v)if(selectedNoteObject(selectedTrack,s,v)){tick=lockTick817(selectedTrack,s,v);note=noteAtVoice(selectedTrack,s,v);break;}
 return autoScopedDepth46(selectedTrack,activePattern[selectedTrack],key,slot,tick,note,base);
}
FLASHMEM bool autoDepthEdit46(uint16_t key,uint8_t slot,int delta,int lo,int hi){
 bool has=false;for(uint16_t i=0;i<autoMuteCount46;++i){auto&q=autoMutes46[i];if(q.track==selectedTrack&&q.pattern==activePattern[selectedTrack]&&q.key==key&&q.slot==slot)has=true;}if(!has)return false;
 if(lockWantsEdit817()){
  unsigned need=0;auto selected=[&](uint8_t t,uint16_t s,uint8_t v){return selectedStep>=0?t==selectedTrack&&s==selectedStep&&v==selectedVoice:selectedNoteObject(t,s,v);};
  for(uint8_t t=0;t<16;++t)for(uint16_t s=0;s<trackStepCount(t);++s)for(uint8_t v=0;v<noteCountAtStep(t,s);++v)if(selected(t,s,v))++need;
  if(autoMuteCount46+need>AUTO_MUTES46){DISPLAY_SERIAL.println("MSG|DEPTH SCOPE CAPACITY");return true;}
  for(uint8_t t=0;t<16;++t)for(uint16_t s=0;s<trackStepCount(t);++s)for(uint8_t v=0;v<noteCountAtStep(t,s);++v)if(selected(t,s,v)){int32_t id=lockTick817(t,s,v);uint8_t note=noteAtVoice(t,s,v);int old=autoScopedDepth46(t,activePattern[t],key,slot,id,note,modUnits805[key].outputs[slot].depth);int next=clampInt(old+delta,lo,hi);bool found=false;for(uint16_t i=0;i<autoMuteCount46;++i){auto&q=autoMutes46[i];if(q.track==t&&q.pattern==activePattern[t]&&q.key==key&&q.slot==slot&&q.identity==id&&q.note==note){q.depth=next;found=true;}}if(!found){uint32_t tick=max(0,(int)((int32_t)s*96+audibleMicroTick(t,s,v)));autoMutes46[autoMuteCount46++]={tick,tick+audibleGateTicks(t,s,v),key,t,activePattern[t],slot,id,note,(int16_t)next};}}
 }else{int next=clampInt(autoDepthDisplay46(key,slot,modUnits805[key].outputs[slot].depth)+delta,lo,hi);bool full=false;for(uint16_t i=0;i<autoMuteCount46;++i){auto&q=autoMutes46[i];if(q.track==selectedTrack&&q.pattern==activePattern[selectedTrack]&&q.key==key&&q.slot==slot){q.depth=next;full|=q.note==128;}}if(!full){if(autoMuteCount46==AUTO_MUTES46){DISPLAY_SERIAL.println("MSG|DEPTH SCOPE CAPACITY");return true;}autoMutes46[autoMuteCount46++]={0,UINT32_MAX,key,selectedTrack,activePattern[selectedTrack],slot,-1,128,(int16_t)next};}}
 ++modEpoch805;storageDirty931=true;modDisplayDirty810=displayValueDirty=true;return true;
}
FLASHMEM void autoGraph46(){
 if(autoBusy46||!autoRoute46)return;for(uint16_t i=0;i<AUTO_LANES46;++i){auto&l=autoLanes46[i];float v;if(!l.used||l.type||!modIsTarget811(l.route)||!autoLaneValue46(i,v))continue;uint16_t key=modTarget810(l.route);uint8_t p=l.route.number%8;if(key>=64||!modUnits805[key].enabled)continue;if(l.route.kind==MOD_DEPTH810){modSums810[key][p]+=v;++modCounts810[key][p];}else if(p<5){modParamSums811[key][p]+=v;++modParamCounts811[key][p];}}
}
FLASHMEM void autoMix46(){
 if(autoBusy46||!autoRoute46)return;for(uint16_t i=0;i<AUTO_LANES46;++i){auto&l=autoLanes46[i];float v;if(!l.used||l.type||!autoLaneValue46(i,v))continue;auto&r=l.route;if(ioVirtual53(r)){ioVirtualAdd53(r.channel,r.number,r.reference+v);continue;}auto dest=modResolve810(r);if(!dest.enabled||!mod806MidiTarget(dest))continue;
  uint8_t mask=r.kind==MOD_PATCH810?outMask902(r.track):dest.port?1u<<(dest.port-1):channelMask902(dest.track);int base=modReference43(r);
  modMidiPort19(dest.track,dest.kind==MOD_MIDI_BEND806?128:dest.number,v,mask,base);
 }
}
FLASHMEM void autoNrpn46(){
 if(autoBusy46||!autoRoute46)return;for(uint16_t i=0;i<AUTO_LANES46;++i){auto&l=autoLanes46[i];float v;if(!l.used||l.type||l.route.kind!=MOD_TRACK21||!autoLaneValue46(i,v))continue;auto&r=l.route;auto*q=modTrackParam21(r);if(!q||!q->kind)continue;auto b=blockAtNote21(r.track,r.block21,255,-1,255);if(!b.port)continue;uint16_t base=0;patchValue819(blockPatchContext21(r.track,r.block21),*q,base);
  uint16_t j=0;while(j<modMixCount21){auto&a=modMix21[j];if(a.port==b.port&&a.channel==b.channel&&a.number==q->number&&a.kind==q->kind&&a.bits==q->bits)break;++j;}if(j==modMixCount21){if(j==512)continue;modMix21[modMixCount21++]={0,q->number,base,0,b.port,b.channel,q->kind,q->bits};}modMix21[j].sum+=v;++modMix21[j].count;
 }
}
FLASHMEM void autoCv46(float*sum,uint16_t*count,uint16_t*base){if(autoBusy46||!autoRoute46)return;for(uint16_t i=0;i<AUTO_LANES46;++i){auto&l=autoLanes46[i];float v;if(l.used&&!l.type&&l.route.kind==MOD_CV46&&l.route.track<4&&autoLaneValue46(i,v)){uint8_t ch=l.route.track;sum[ch]+=v;++count[ch];base[ch]=l.route.reference;}}}
FLASHMEM void autoCcInput46(uint8_t channel,uint8_t number,uint8_t value){for(uint8_t t=0;t<16;++t)if(midiInChannel17[t]==channel+1)autoManual46(t,LOCK_CC817,t,number,value);}
FLASHMEM void autoIoInput46(uint16_t key,uint8_t value){if(!playing||!recordingArmed||!autoRecord50||songEngineActive||autoBusy46)return;for(uint8_t t=0;t<16;++t)if(modAssociated12(key,t)){
 int lane=-1;for(uint16_t i=0;i<AUTO_LANES46;++i){auto&l=autoLanes46[i];if(l.used&&l.type==2&&l.track==t&&l.pattern==activePattern[t]&&l.target==key){lane=i;break;}}
 if(lane<0)for(uint16_t i=0;i<AUTO_LANES46;++i)if(!autoLanes46[i].used){auto&l=autoLanes46[i];l={};autoCaptureValid46[i]=false;l.used=1;l.type=2;l.track=t;l.pattern=activePattern[t];l.target=key;snprintf(l.name,8,"J2 IO%u",key+1);lane=i;break;}
 if(lane<0||!autoPut46(lane,localTickForTrack(t),trackSequenceTicks(t),value))DISPLAY_SERIAL.println("MSG|AUTO CAPACITY");else storageDirty931=true;
}}
FLASHMEM float autoIoValue46(uint16_t key,float sample){if(autoBusy46||key>=64||!(autoIoMask46&(1ULL<<key)))return sample;float v;for(uint16_t i=0;i<AUTO_LANES46;++i){auto&l=autoLanes46[i];if(l.used&&l.type==2&&l.target==key&&autoLaneValue46(i,v))sample=v;}return sample;}
FLASHMEM void autoDetach46(uint16_t key){
 autoUndoClear46();autoClipDetach46(key);
 for(auto&l:autoLanes46)if(l.used&&((l.type==2&&l.target==key)||(l.type==1&&modLock931(l.kind)&&l.target==key)||(!l.type&&modIsTarget811(l.route)&&modTarget810(l.route)==key)))l.used=0;
 for(uint16_t i=0;i<autoMuteCount46;)if(autoMutes46[i].key==key)autoMutes46[i]=autoMutes46[--autoMuteCount46];else ++i;
 for(uint16_t i=0;i<autoCount46;)if(!autoLanes46[autoPoints46[i].lane].used)autoPoints46[i]=autoPoints46[--autoCount46];else ++i;autoReindex46();
}
