#pragma once
struct WriteNote46 {int32_t tick,identity;uint32_t gate,scope;uint8_t track,note;};
static EXTMEM WriteNote46 writeNotes46[16384];static uint16_t writeNoteCount46=0;
static bool writeSelection46=false,writeSelectionError46=false;static uint16_t writeFirstPoint46=0;
static uint16_t writeLaneMap46[AUTO_LANES46],writeLastPoint46[AUTO_LANES46];
static const char*writeAutoError46="WRITE AUTO CAPACITY";
FLASHMEM void writeSelect46(){
 writeNoteCount46=0;writeSelectionError46=false;writeSelection46=selectionHasActiveNotes();
 // Only note identities and rendering times are retained, not a project copy.
 for(uint8_t t=0;t<16;++t)for(uint16_t s=0;s<trackStepCount(t);++s)for(uint8_t v=0;v<noteCountAtStep(t,s);++v){
  if(writeSelection46&&!selectedNoteObject(t,s,v))continue;if(writeNoteCount46==16384){writeSelectionError46=true;return;}
  int32_t len=trackSequenceTicks(t),tick=(int32_t)s*96+audibleMicroTick(t,s,v);
  tick=(tick%len+len)%len;uint32_t gate=audibleGateTicks(t,s,v),release=0;
  if(lockRelease817)for(uint16_t k=0;k<64;++k)if(modUnits805[k].enabled&&!modKind931(k)&&!ioKey38(k)&&(modUnits805[k].inputs&(1u<<t)))release=max(release,(uint32_t)modUnits805[k].release);
  writeNotes46[writeNoteCount46++]={tick,lockTick817(t,s,v),gate,gate+release,t,noteAtVoice(t,s,v)};
 }
}
FLASHMEM bool writeScope46(uint8_t t,uint32_t tick){if(!writeSelection46)return true;uint32_t len=trackSequenceTicks(t);for(uint16_t i=0;i<writeNoteCount46;++i){auto&n=writeNotes46[i];if(n.track==t&&(tick+len-n.tick)%len<n.scope)return true;}return false;}
FLASHMEM int writeLockValue46(uint8_t kind,uint16_t target,uint16_t param,int base){
 if(autoRenderTrack46<0)return base;int64_t latest=INT64_MIN;uint8_t t=autoRenderTrack46;uint32_t len=trackSequenceTicks(t);
 for(uint16_t i=0;i<lockPlaybackCount39();++i){const auto&l=lockPlaybackEntry39(i);if(l.track!=t||l.pattern!=activePattern[t]||l.kind!=kind||l.target!=target||l.parameter!=param)continue;
  if(l.note>=128){uint32_t age=(autoRenderTick46+len-(l.tick%len))%len;if(age<l.duration&&autoRenderTick46-age>latest){latest=autoRenderTick46-age;base=l.value;}continue;}
  for(uint16_t j=0;j<writeNoteCount46;++j){auto&n=writeNotes46[j];if(n.track!=t||n.identity!=l.tick||n.note!=l.note)continue;uint32_t age=(autoRenderTick46+len-n.tick)%len;if(age<n.scope&&autoRenderTick46-age>latest){latest=autoRenderTick46-age;base=l.value;}}
 }return base;
}
FLASHMEM float writeValue46(const ModUnit805&m,uint8_t kind,int64_t at,uint16_t key){
 int64_t latest=-1;uint32_t gate=0;uint8_t t=autoRenderTrack46;uint32_t len=trackSequenceTicks(t);
 if(m.inputs&(1u<<t))for(uint16_t i=0;i<writeNoteCount46;++i){auto&n=writeNotes46[i];if(n.track!=t)continue;int64_t tick=n.tick;if(tick>at)tick-=len;if(tick>=latest){gate=tick==latest?max(gate,n.gate):n.gate;latest=tick;}}
 return modValue810(m,kind,at,key,latest,gate,false);
}
FLASHMEM bool writeStagePoint46(uint16_t lane,uint32_t tick,uint32_t end,float value){
 uint16_t last=writeLastPoint46[lane];if(last!=AUTO_NONE46){auto&p=autoStagePoints46[last];if(p.end==tick&&p.value==value){p.end=end;return true;}}
 if(autoStageCount46+autoCount46>=AUTO_POINTS46)return false;writeLastPoint46[lane]=autoStageCount46;autoStagePoints46[autoStageCount46++]={tick,end,value,lane,AUTO_NONE46};return true;
}
FLASHMEM int writeStageLane46(uint8_t t,uint8_t type,uint8_t kind,uint16_t target,uint16_t parameter,const ModRoute805&route,const char*name){
 for(uint16_t i=0;i<AUTO_LANES46;++i)if(!autoStageLanes46[i].used){auto&l=autoStageLanes46[i];l={};l.used=1;l.track=t;l.pattern=activePattern[t];l.type=type;l.kind=kind;l.target=target;l.parameter=parameter;l.route=route;snprintf(l.name,8,"%.7s",name);return i;}return -1;
}
FLASHMEM bool writeStageMute46(uint8_t t,uint16_t k,uint8_t slot,uint32_t tick,uint32_t end,int32_t identity=-1,uint8_t note=128){
 if(autoStageMuteCount46+autoMuteCount46>=AUTO_MUTES46)return false;autoStageMutes46[autoStageMuteCount46++]={tick,end,k,t,activePattern[t],slot,identity,note};return true;
}
FLASHMEM void writeFree46(){autoRenderTrack46=-1;autoBusy46=false;mod805Invalidate();}
FLASHMEM float writeDepth46(uint16_t key,uint8_t slot,uint8_t track,uint32_t tick,float base){int32_t identity=-1;uint8_t note=128;int64_t latest=-1;for(uint16_t i=0;i<writeNoteCount46;++i){auto&n=writeNotes46[i];if(n.track==track&&n.tick<=(int32_t)tick&&n.tick>=latest){latest=n.tick;identity=n.identity;note=n.note;}}return autoScopedDepth46(track,activePattern[track],key,slot,identity,note,lroundf(base));}
FLASHMEM bool writePrepare46(){
 if(!(writeMask41&112))return true;
 if(playing){DISPLAY_SERIAL.println("MSG|STOP BEFORE WRITE AUTO");return false;}
 if(writeSelectionError46){DISPLAY_SERIAL.println("MSG|WRITE NOTE CAPACITY");return false;}
 autoStageCount46=autoStageMuteCount46=0;for(auto&v:writeLastPoint46)v=AUTO_NONE46;memset(autoStageLanes46,0,sizeof(autoStageLanes46));autoBusy46=true;bool ok=true;
 for(uint8_t t=0;t<16&&ok;++t)if(writeTracks41&(1u<<t)){
  autoRenderTrack46=t;uint32_t length=trackSequenceTicks(t);
  int16_t lanes[64][8];memset(lanes,255,sizeof(lanes));
  for(uint16_t k=0;k<64&&ok;++k){const auto&m=modUnits805[k];if(!m.enabled||ioKey38(k)||!modAssociated12(k,t)||!(writeMask41&(modKind931(k)?32:16)))continue;
   for(uint8_t j=0;j<8&&ok;++j){auto route=m.outputs[j];if(!route.enabled)continue;bool already=false;for(uint16_t n=0;n<autoMuteCount46;++n){auto&q=autoMutes46[n];if(q.key==k&&q.slot==j&&q.track==t&&q.pattern==activePattern[t]&&q.note==128&&q.tick==0&&q.end==UINT32_MAX&&q.depth==0)already=true;}if(already)continue;
    char name[8];snprintf(name,8,"%s%02u.%u",modKind931(k)?"LF":"EN",k+1,j+1);int lane=writeStageLane46(t,0,0,k,j,route,name);if(lane<0){ok=false;break;}lanes[k][j]=lane;
    if(!writeSelection46)ok=writeStageMute46(t,k,j,0,UINT32_MAX);
    else for(uint16_t n=0;n<writeNoteCount46&&ok;++n){auto&v=writeNotes46[n];if(v.track!=t)continue;uint32_t end=v.tick+v.scope;ok=writeStageMute46(t,k,j,v.tick,min(end,length),v.identity,v.note);if(end>length)ok=ok&&writeStageMute46(t,k,j,0,min(end-length,length),v.identity,v.note);}
   }
  }
  // One graph evaluation per tick and track, regardless of route count.
  for(uint32_t tick=0;tick<length&&ok;++tick){if(!writeScope46(t,tick))continue;autoRenderTick46=tick;++modEpoch805;modFrame810(tick);
   for(uint16_t k=0;k<64&&ok;++k)for(uint8_t j=0;j<8&&ok;++j)if(lanes[k][j]>=0){bool muted=false;for(uint16_t n=0;n<autoMuteCount46;++n){auto&q=autoMutes46[n];if(q.key==k&&q.slot==j&&q.track==t&&q.pattern==activePattern[t]&&autoContains49(q,tick,length)&&q.depth==0){muted=true;break;}}if(!muted)ok=writeStagePoint46(lanes[k][j],tick,tick+1,modLevelAt811(k,tick)*writeDepth46(k,j,t,tick,modDepth810(k,j,tick)));}
  }
  if(writeMask41&64)for(uint16_t i=0;i<lockPlaybackCount39()&&ok;++i){auto l=lockPlaybackEntry39(i);if(l.track!=t||l.pattern!=activePattern[t])continue;
   uint32_t tick=l.tick,end=tick+l.duration;bool match=l.note>=128&&!writeSelection46;
   for(uint16_t n=0;n<writeNoteCount46&&!match;++n){auto&v=writeNotes46[n];if(v.track==t&&v.identity==l.tick&&v.note==l.note){tick=v.tick;end=tick+v.scope;match=true;}}
   if(!match)continue;char name[8];snprintf(name,8,"PLX%03u",i+1);int lane=writeStageLane46(t,1,l.kind,l.target,l.parameter,{},name);if(lane<0){ok=false;break;}
   writeFirstPoint46=autoStageCount46;ok=writeStagePoint46(lane,tick,min(end,length),l.value);if(end>length)ok=ok&&writeStagePoint46(lane,0,min(end-length,length),l.value);
  }
 }
 for(uint16_t i=0;i<AUTO_LANES46;++i)if(autoStageLanes46[i].used&&writeLastPoint46[i]==AUTO_NONE46)autoStageLanes46[i].used=0;
 uint16_t next=0;for(uint16_t i=0;i<AUTO_LANES46&&ok;++i)if(autoStageLanes46[i].used){while(next<AUTO_LANES46&&autoLanes46[next].used)++next;if(next==AUTO_LANES46){ok=false;break;}writeLaneMap46[i]=next++;}
 writeFree46();if(!ok){DISPLAY_SERIAL.print("MSG|");DISPLAY_SERIAL.println(writeAutoError46);}return ok;
}
FLASHMEM void writeCommit46(){
 if(!(writeMask41&112))return;
 for(uint16_t i=0;i<AUTO_LANES46;++i)if(autoStageLanes46[i].used)autoLanes46[writeLaneMap46[i]]=autoStageLanes46[i];
 for(uint16_t i=0;i<autoStageCount46;++i){auto q=autoStagePoints46[i];q.lane=writeLaneMap46[q.lane];autoPoints46[autoCount46++]=q;}
 for(uint16_t i=0;i<autoStageMuteCount46;++i)autoMutes46[autoMuteCount46++]=autoStageMutes46[i];
 if(writeMask41&64){for(uint16_t i=0;i<lockCount817;){auto&l=noteLocks817[i];bool remove=false;if((writeTracks41&(1u<<l.track))&&l.pattern==activePattern[l.track]){if(!writeSelection46)remove=true;else for(uint16_t n=0;n<writeNoteCount46;++n){auto&v=writeNotes46[n];if(v.track==l.track&&v.identity==l.tick&&v.note==l.note){remove=true;break;}}}if(remove)noteLocks817[i]=noteLocks817[--lockCount817];else ++i;}}
 writeDraftClear46();
 autoReindex46();freeInvalidate11();lockStop817();mod805Invalidate();storageDirty931=true;
}
