#pragma once
FLASHMEM void autoReindex46(){
 memset(autoSentValid46,0,sizeof(autoSentValid46));autoKinds46=0;autoIoMask46=0;autoRoute46=autoCvPresent46=false;uint16_t tail[AUTO_LANES46];for(auto&v:tail)v=AUTO_NONE46;for(auto&l:autoLanes46)if(l.used){if(l.type==1)autoKinds46|=1u<<l.kind;else if(l.type==2&&l.target<64)autoIoMask46|=1ULL<<l.target;else if(!l.type){autoRoute46=true;autoCvPresent46|=l.route.kind==MOD_CV46;}}
 for(uint16_t i=0;i<AUTO_LANES46;++i){autoHead46[i]=autoCursor46[i]=AUTO_NONE46;autoLastTick46[i]=UINT32_MAX;}
 for(uint16_t i=0;i<autoCount46;++i){auto&p=autoPoints46[i];if(p.lane>=AUTO_LANES46||!autoLanes46[p.lane].used)continue;if(tail[p.lane]!=AUTO_NONE46&&autoPoints46[tail[p.lane]].tick<=p.tick){autoPoints46[tail[p.lane]].next=i;p.next=AUTO_NONE46;tail[p.lane]=i;continue;}uint16_t*link=&autoHead46[p.lane];while(*link!=AUTO_NONE46&&autoPoints46[*link].tick<=p.tick)link=&autoPoints46[*link].next;p.next=*link;*link=i;if(p.next==AUTO_NONE46)tail[p.lane]=i;}
}
FLASHMEM void autoReset46(){autoHistoryClear46();memset(autoCaptureValid46,0,sizeof(autoCaptureValid46));memset(autoLanes46,0,sizeof(autoLanes46));autoCount46=autoMuteCount46=0;autoReindex46();}
FLASHMEM int autoLane46(uint8_t t,uint8_t p,uint8_t kind,uint16_t target,uint16_t parameter){
 for(uint16_t i=0;i<AUTO_LANES46;++i){auto&l=autoLanes46[i];if(l.used&&l.track==t&&l.pattern==p&&l.type==1&&l.kind==kind&&l.target==target&&l.parameter==parameter)return i;}
 for(uint16_t i=0;i<AUTO_LANES46;++i)if(!autoLanes46[i].used){auto&l=autoLanes46[i];l={};autoCaptureValid46[i]=false;l.used=1;l.track=t;l.pattern=p;l.type=1;l.kind=kind;l.target=target;l.parameter=parameter;snprintf(l.name,8,kind==LOCK_CC817?"CC%03u":"Auto%u",parameter);return i;}return -1;
}
FLASHMEM bool autoPut46(uint16_t lane,uint32_t tick,uint32_t end,float value){
 if(lane>=AUTO_LANES46||end<=tick||!isfinite(value))return false;
 // Point replacement is transactional with respect to capacity. Recording a
 // new pass replaces the covered interval, preserving the outside portions.
 unsigned splits=0;for(uint16_t i=0;i<autoCount46;++i){auto&p=autoPoints46[i];if(p.lane==lane&&p.tick<tick&&p.end>end)++splits;}
 if(autoCount46+splits+1>AUTO_POINTS46)return false;
 for(uint16_t i=0;i<autoCount46;++i){auto&p=autoPoints46[i];if(p.lane!=lane||p.end<=tick||p.tick>=end)continue;
  if(p.tick<tick&&p.end>end){auto q=p;q.tick=end;p.end=tick;autoPoints46[autoCount46++]=q;}
  else if(p.tick<tick)p.end=tick;else if(p.end>end)p.tick=end;else{p=autoPoints46[--autoCount46];--i;}}
 autoPoints46[autoCount46++]={tick,end,value,lane,AUTO_NONE46};autoReindex46();return true;
}
FLASHMEM bool autoLocal46(const AutoLane46&l,uint32_t&tick){
 if(!playing||trackSilent818(l.track))return false;
 int64_t now=queryTick50>=0?queryTick50:(int64_t)(songEngineActive?songTick:transportTickAbsolute);
 if(!songEngineActive){uint8_t p=activePattern[l.track];int64_t origin=now-(int64_t)((bridgeMasterTickAt(max((int64_t)0,now))+trackSequenceTicks(l.track)-grooveTrackPhase[l.track]%trackSequenceTicks(l.track))%trackSequenceTicks(l.track));if(grooveBridgeReady[l.track]&&!grooveBridgeSwitched[l.track]&&now>=(int64_t)grooveBridgeAt[l.track]&&pendingPattern[l.track]>=0){p=pendingPattern[l.track];origin=grooveBridgeAt[l.track];}if(l.pattern!=p)return false;tick=now-origin;return true;}
 for(const auto&c:songClips)if(c.used&&c.track==l.track&&c.pattern==l.pattern&&now>=(int64_t)c.position*96&&now<(int64_t)(c.position+c.length)*96){tick=(now-(int64_t)c.position*96)+(uint32_t)c.offset*96;return true;}return false;
}
FLASHMEM bool autoLaneValue46(uint16_t lane,float&value){
 auto&l=autoLanes46[lane];uint32_t tick;if(!l.used||!autoLocal46(l,tick))return false;
 uint16_t&cursor=autoCursor46[lane];if(cursor==AUTO_NONE46||tick<autoLastTick46[lane])cursor=autoHead46[lane];autoLastTick46[lane]=tick;
 while(cursor!=AUTO_NONE46&&autoPoints46[cursor].next!=AUTO_NONE46&&autoPoints46[autoPoints46[cursor].next].tick<=tick)cursor=autoPoints46[cursor].next;
 if(cursor==AUTO_NONE46)return false;const auto&p=autoPoints46[cursor];if(tick<p.tick||tick>=p.end)return false;value=p.value;return true;
}
FLASHMEM int autoParameter46(uint8_t kind,uint16_t target,uint16_t param,int base){
 if(autoBusy46||!playing||!(autoKinds46&(1u<<kind)))return base;float v;for(uint16_t i=0;i<AUTO_LANES46;++i){auto&l=autoLanes46[i];if(l.used&&(indContext50<0||l.track==indContext50)&&l.type==1&&l.kind==kind&&l.target==target&&l.parameter==param&&autoLaneValue46(i,v))base=lroundf(v);}return base;
}
FLASHMEM void autoManual46(uint8_t t,uint8_t kind,uint16_t target,uint16_t param,int value){
 if(!playing||!recordingArmed||!autoRecord50||songEngineActive||autoPlayback46||autoBusy46||t>=16)return;
 int lane=autoLane46(t,activePattern[t],kind,target,param);if(lane<0){DISPLAY_SERIAL.println("MSG|AUTO LIST FULL");return;}
 if(autoCaptureValid46[lane]&&autoCaptured46[lane]==value)return;autoCaptured46[lane]=value;autoCaptureValid46[lane]=true;
 uint32_t tick=localTickForTrack(t),end=trackSequenceTicks(t);if(!autoPut46(lane,tick,end,value)){DISPLAY_SERIAL.println("MSG|AUTO POINTS FULL");return;}storageDirty931=true;
}
FLASHMEM void autoMidi46(int64_t){
 if(autoBusy46||!playing||!autoKinds46)return;autoPlayback46=true;
 for(uint16_t i=0;i<AUTO_LANES46;++i){auto&l=autoLanes46[i];float v;if(!l.used||l.type!=1||!autoLaneValue46(i,v)){autoSentValid46[i]=false;continue;}
  uint8_t t=l.track,ch=midiChannelForTrack(t);MidiScope902 scope(outMask902(t),t);
  if(l.kind==LOCK_CC817)midiOffer810(ch,l.parameter,clampInt(lroundf(v),0,127),false);
  else if(l.kind==LOCK_PROGRAM26){uint16_t value=clampInt(lroundf(v),0,128),address=(outMask902(t)<<4)|ch;if(!autoSentValid46[i]||autoSent46[i]!=value||autoSentAddress46[i]!=address){if(value)midiRaw810(0xC0|ch,value-1,0);autoSent46[i]=value;autoSentAddress46[i]=address;autoSentValid46[i]=true;}}
  else if(l.kind==LOCK_NRPN7817||l.kind==LOCK_NRPN14817){int value=lroundf(v);uint16_t address=(outMask902(t)<<4)|ch;if(!autoSentValid46[i]||autoSent46[i]!=value||autoSentAddress46[i]!=address){midiNrpn810(ch,l.parameter,l.kind==LOCK_NRPN14817?14:7,value);autoSent46[i]=value;autoSentAddress46[i]=address;autoSentValid46[i]=true;}}
 }
 autoPlayback46=false;
}
// Frozen CV contributions are aggregated in AutoPlayback46.h.
FLASHMEM void autoClearPattern46(uint8_t t,uint8_t p){
 for(auto&l:autoLanes46)if(l.used&&l.track==t&&l.pattern==p)l.used=0;
 for(uint16_t i=0;i<autoCount46;)if(!autoLanes46[autoPoints46[i].lane].used)autoPoints46[i]=autoPoints46[--autoCount46];else ++i;
 for(uint16_t i=0;i<autoMuteCount46;)if(autoMutes46[i].track==t&&autoMutes46[i].pattern==p)autoMutes46[i]=autoMutes46[--autoMuteCount46];else ++i;autoReindex46();
}
FLASHMEM bool autoCopyPattern46(uint8_t ft,uint8_t fp,uint8_t tt,uint8_t tp){
 if(ft==tt&&fp==tp)return true;unsigned lanes=0,points=0,mutes=0,free=0,replacedPoints=0,replacedMutes=0;for(auto&l:autoLanes46){if(l.used&&l.track==ft&&l.pattern==fp)++lanes;if(!l.used||(l.track==tt&&l.pattern==tp))++free;}
 for(uint16_t i=0;i<autoCount46;++i){auto&l=autoLanes46[autoPoints46[i].lane];if(l.track==ft&&l.pattern==fp)++points;if(l.track==tt&&l.pattern==tp)++replacedPoints;}
 for(uint16_t i=0;i<autoMuteCount46;++i){if(autoMutes46[i].track==ft&&autoMutes46[i].pattern==fp)++mutes;if(autoMutes46[i].track==tt&&autoMutes46[i].pattern==tp)++replacedMutes;}
 if(lanes>free||points+autoCount46-replacedPoints>AUTO_POINTS46||mutes+autoMuteCount46-replacedMutes>AUTO_MUTES46)return false;autoClearPattern46(tt,tp);
 uint16_t map[AUTO_LANES46];for(auto&v:map)v=AUTO_NONE46;
 for(uint16_t i=0;i<AUTO_LANES46;++i)if(autoLanes46[i].used&&autoLanes46[i].track==ft&&autoLanes46[i].pattern==fp){for(uint16_t j=0;j<AUTO_LANES46;++j)if(!autoLanes46[j].used){autoLanes46[j]=autoLanes46[i];autoLanes46[j].track=tt;autoLanes46[j].pattern=tp;auto&copy=autoLanes46[j];if(copy.type==1&&(lockMidiKind817(copy.kind)||copy.kind==LOCK_BLOCK21||copy.kind==LOCK_ROLL1000)&&copy.target==ft)copy.target=tt;if(!copy.type&&(copy.route.kind==MOD_TRACK21||copy.route.kind==MOD_PATCH810||modTrackMidi43(copy.route))&&copy.route.track==ft)copy.route.track=tt;map[i]=j;break;}}
 uint16_t n=autoCount46;for(uint16_t i=0;i<n;++i)if(map[autoPoints46[i].lane]!=AUTO_NONE46){auto q=autoPoints46[i];q.lane=map[q.lane];autoPoints46[autoCount46++]=q;}
 n=autoMuteCount46;for(uint16_t i=0;i<n;++i)if(autoMutes46[i].track==ft&&autoMutes46[i].pattern==fp){auto q=autoMutes46[i];q.track=tt;q.pattern=tp;autoMutes46[autoMuteCount46++]=q;}autoReindex46();return true;
}
FLASHMEM bool autoWrite46(File&f){uint32_t magic=0x46305441;
 if(f.write((uint8_t*)&magic,4)!=4||f.write((uint8_t*)&autoCount46,2)!=2||f.write((uint8_t*)&autoMuteCount46,2)!=2)return false;
 for(auto l:autoLanes46){if(l.used){if(l.type==2||(l.type==1&&modLock931(l.kind))){if(l.target>=64||!modUnits805[l.target].enabled)return false;l.target=modDiskKey931(l.target);}if(!l.type&&modIsTarget811(l.route)){uint16_t k=modTarget810(l.route);if(k>=64||!modUnits805[k].enabled)return false;modRouteKey931(l.route,modDiskKey931(k));}}if(f.write((uint8_t*)&l,sizeof(l))!=sizeof(l))return false;}
 if(f.write((uint8_t*)autoPoints46,autoCount46*sizeof(AutoPoint46))!=autoCount46*sizeof(AutoPoint46))return false;
 for(uint16_t i=0;i<autoMuteCount46;++i){auto m=autoMutes46[i];if(m.key>=64||!modUnits805[m.key].enabled)return false;m.key=modDiskKey931(m.key);if(f.write((uint8_t*)&m,sizeof(m))!=sizeof(m))return false;}return true;
}
FLASHMEM bool autoRead46(File&f,bool present){
 autoStageCount46=autoStageMuteCount46=0;memset(autoStageLanes46,0,sizeof(autoStageLanes46));if(!present)return true;
 uint32_t magic;if(f.read((uint8_t*)&magic,4)!=4||magic!=0x46305441||f.read((uint8_t*)&autoStageCount46,2)!=2||autoStageCount46>AUTO_POINTS46||f.read((uint8_t*)&autoStageMuteCount46,2)!=2||autoStageMuteCount46>AUTO_MUTES46)return false;
 if(f.read((uint8_t*)autoStageLanes46,sizeof(autoStageLanes46))!=sizeof(autoStageLanes46)||f.read((uint8_t*)autoStagePoints46,autoStageCount46*sizeof(AutoPoint46))!=autoStageCount46*sizeof(AutoPoint46)||f.read((uint8_t*)autoStageMutes46,autoStageMuteCount46*sizeof(AutoMute46))!=autoStageMuteCount46*sizeof(AutoMute46))return false;
 for(auto&l:autoStageLanes46)if(l.used){if(l.used>1||l.track>15||l.pattern>15||l.type>2||l.kind>LOCK_BLOCK21||l.name[7])return false;
  if(l.type==2||(l.type==1&&modLock931(l.kind))){if(l.target>=512||fullMap931[l.target]<0)return false;l.target=fullMap931[l.target];}
  if(!l.type){auto&q=l.route;if(q.enabled!=1||q.track>15||q.channel>15||q.port>8||(q.kind>MOD_TRACK21&&q.kind!=MOD_CV46)||(q.kind==MOD_CV46&&q.track>3))return false;if((q.kind==MOD_CC||q.kind==MOD_PATCH810)&&q.number>127)return false;if(q.reference<(q.kind==MOD_MIDI_BEND806?-8192:0)||q.reference>(q.kind==MOD_MIDI_BEND806?8191:q.kind==MOD_CV46?4095:127))return false;if(modIsTarget811(q)){uint16_t k=modTarget810(q);if(k>=512||fullMap931[k]<0)return false;modRouteKey931(q,fullMap931[k]);}}
 }
 for(uint16_t i=0;i<autoStageCount46;++i){auto&p=autoStagePoints46[i];if(p.lane>=AUTO_LANES46||!autoStageLanes46[p.lane].used||p.end<=p.tick||!isfinite(p.value)||fabsf(p.value)>10000000)return false;auto&l=autoStageLanes46[p.lane];if(l.type==1){NoteLock817 check={0,(int32_t)lroundf(p.value),l.target,l.parameter,l.track,l.pattern,128,l.kind,1};if(!lockValid817(check))return false;}if(l.type==2&&(p.value<0||p.value>127))return false;}
 for(uint16_t i=0;i<autoStageMuteCount46;++i){auto&m=autoStageMutes46[i];if(m.track>15||m.pattern>15||m.key>=512||fullMap931[m.key]<0||m.slot>=8||m.end<=m.tick||m.note>128)return false;m.key=fullMap931[m.key];}return true;
}
FLASHMEM void autoInstall46(){autoHistoryClear46();memcpy(autoLanes46,autoStageLanes46,sizeof(autoLanes46));autoCount46=autoStageCount46;autoMuteCount46=autoStageMuteCount46;memcpy(autoPoints46,autoStagePoints46,autoCount46*sizeof(AutoPoint46));memcpy(autoMutes46,autoStageMutes46,autoMuteCount46*sizeof(AutoMute46));autoReindex46();}
FLASHMEM void patRender46(){
 char title[21],line[21];snprintf(title,21,"PAT T%02u P%02u",selectedTrack+1,activePattern[selectedTrack]+1);

 if(patMenu46==6){linkRender55();return;}
 if(patMenu46==1){sendMenuScreen(title,"Pattern settings",indEditing59()?"PC   Auto Link IND ":"PC   Auto Link DEF ","");return;}
 if(patMenu46==2){uint8_t p=programFocus51(selectedTrack),v=indEditing59()?programValue939(selectedTrack,p):programDefault939[selectedTrack];snprintf(title,21,"PAT T%02u P%02u",selectedTrack+1,p+1);snprintf(line,21,"PRG %03u %s",v,indEditing59()&&ind50.find(selectedTrack*16+p,IND_PATCH50,LOCK_PROGRAM26,0,0)>=0&&ind50.active(selectedTrack*16+p,IND_PATCH50)?"IND":"DEF");char recall[21];snprintf(recall,21,"OB PC>PATCH %s",patchRecallMask54&(1u<<selectedTrack)?"ON":"OFF");sendMenuScreen(title,line,recall,"Inh  Off  Patch     ");return;}
 uint16_t count=0;bool valid=false;for(uint16_t i=0;i<AUTO_LANES46;++i){auto&l=autoLanes46[i];if(l.used&&l.track==selectedTrack&&l.pattern==activePattern[selectedTrack]){++count;if(i==autoSelected46)valid=true;}}
 if(!valid){autoSelected46=AUTO_NONE46;for(uint16_t i=0;i<AUTO_LANES46;++i)if(autoLanes46[i].used&&autoLanes46[i].track==selectedTrack&&autoLanes46[i].pattern==activePattern[selectedTrack]){autoSelected46=i;break;}}
 char rows[4][21];for(auto&row:rows){memset(row,' ',20);row[20]=0;}uint16_t position=0,selected=0;
 for(uint16_t i=0;i<AUTO_LANES46;++i){auto&l=autoLanes46[i];if(l.used&&l.track==selectedTrack&&l.pattern==activePattern[selectedTrack]){if(i==autoSelected46)selected=position;++position;}}
 uint16_t first=(selected/6)*6;position=0;for(uint16_t i=0;i<AUTO_LANES46;++i){auto&l=autoLanes46[i];if(!l.used||l.track!=selectedTrack||l.pattern!=activePattern[selectedTrack])continue;if(position>=first&&position<first+6){uint8_t slot=position-first;char name[10];snprintf(name,10,"%c%.7s",i==autoSelected46?'>':' ',l.name);memcpy(rows[slot/2]+(slot%2)*10,name,strlen(name));}++position;}
 if(!count)memcpy(rows[0],"No automation",13);memcpy(rows[3],"Add  Edit Rem  Ren  ",20);sendMenuScreen(rows[0],rows[1],rows[2],rows[3]);
}
FLASHMEM void patButton46(uint8_t i){
 if(patMenu46==6){linkButton55(i);return;}
 if(patMenu46==1){if(i==0)patMenu46=2;else if(i==1)patMenu46=3;else if(i==2)patMenu46=6;else if(i==3){indToggle52();return;}patRender46();return;}

 if(patMenu46==2){if(i==6)patchRecallToggle54();if(i==4)programEdit939(0,true);if(i==5){programEdit939(0,false,true);}patRender46();return;}
 if(i==6&&autoSelected46!=AUTO_NONE46){autoRemove46(autoSelected46);patRender46();}
 if(i==7&&autoSelected46!=AUTO_NONE46){patchNameTarget808=5;patchNamePos808=0;++nameSession809;snprintf(patchNameDraft808,sizeof(patchNameDraft808),"%s",autoLanes46[autoSelected46].name);patchUi808=4;patch808Render();}
 // Add and Edit intentionally remain inert until curve editing is specified.
}
FLASHMEM void patRoy46(uint8_t roy,int delta,bool click){if(patMenu46==6){linkRoy55(roy,delta,click);return;}if(patMenu46==2&&roy==1&&click){patchRecallToggle54();patRender46();return;}if(roy||!delta)return;if(patMenu46==2){programEdit939(delta,false);patRender46();return;}if(patMenu46!=3)return;int step=delta>0?1:-1;for(int i=(autoSelected46==AUTO_NONE46?(step>0?-1:AUTO_LANES46):(int)autoSelected46)+step;i>=0&&i<AUTO_LANES46;i+=step){auto&l=autoLanes46[i];if(l.used&&l.track==selectedTrack&&l.pattern==activePattern[selectedTrack]){autoSelected46=i;break;}}patRender46();}
FLASHMEM void patBack46(){if(patMenu46>1){patMenu46=1;patRender46();}else{patMenu46=0;showGameScreen();}}
FLASHMEM void autoNameDone46(bool commit){if(commit&&autoSelected46<AUTO_LANES46){snprintf(autoLanes46[autoSelected46].name,8,"%.7s",patchNameDraft808);storageDirty931=true;}patchUi808=0;patRender46();}
static bool recHeld46=false;static uint32_t recAt46=0;
FLASHMEM void recordButton46(bool pressed){
 if(settingsOpen){recHeld46=false;return;}
 if(!pressed){if(recHeld46){recHeld46=false;autoRecord50=false;recordToggle820();}return;}
 if(recordingArmed||songRecord||remixRecord913){recHeld46=false;autoRecord50=false;recordToggle820();return;}
 recAt46=millis();recHeld46=true;
}
FLASHMEM void recordService46(){if(recHeld46&&(uint32_t)(millis()-recAt46)>=2000){recHeld46=false;autoRecord50=true;memset(autoCaptureValid46,0,sizeof(autoCaptureValid46));recordToggle820();}}

FLASHMEM int autoMidiBase46(uint8_t port,uint8_t channel,uint8_t cc,int base){if(autoBusy46||!playing||!(autoKinds46&(1u<<LOCK_CC817)))return base;float v;for(uint16_t i=0;i<AUTO_LANES46;++i){auto&l=autoLanes46[i];if(l.used&&l.type==1&&l.kind==LOCK_CC817&&l.parameter==cc&&midiChannelForTrack(l.track)==channel&&(outMask902(l.track)&(1u<<port))&&autoLaneValue46(i,v))base=clampInt(lroundf(v),0,127);}return base;}
