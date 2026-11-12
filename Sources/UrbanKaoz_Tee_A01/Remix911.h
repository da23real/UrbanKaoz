#pragma once
// A seek changes the phase origin, not the transport or the saved source.
rmx913::Phase remixPhase917(uint64_t tick){
 if(remixChainRole916>=0&&tick/1536==remixChainBar917)return remixChainRole916?rmx913::Target:rmx913::Source;
 if(!remixHearBar911)return rmx913::Source;
 uint64_t bar=(tick/1536+remixHearBar911+remixPhaseOffset917)%remixHearBar911;
 return rmx913::phase(bar*1536+tick%1536,1,remixHearBar911,remixWait913);
}
struct RemixDue911 {int32_t tick;uint32_t index;uint8_t velocity;uint8_t pitch=255;uint8_t block21=0;int16_t rollOffset44=0;};
rmx911::List<RemixDue911> remixDue911,remixAuditionDue911;

bool remixScheduleDirty911=true;
static char remixPrevious915[4][2][17]={};
FLASHMEM void remixInvalidateDisplay915(){memset(remixPrevious915,0,sizeof(remixPrevious915));if(remixMode)remixDisplay911(true);}
uint16_t remixSourcePads915[16]={},remixTargetPads915=0;
FLASHMEM void remixIndexPads915(){memset(remixSourcePads915,0,sizeof(remixSourcePads915));for(uint32_t i=0;i<remixSource911.notes.size;++i){const auto&n=remixSource911.notes[i];int bar=remixSourceBar911%remixSource911.tracks[n.track].bars;if(n.tick/1536==bar)remixSourcePads915[n.track]|=1u<<((n.tick/96)%16);}}

FLASHMEM void remixError911(const char*message){DISPLAY_SERIAL.print("MSG|");DISPLAY_SERIAL.println(message);}
FLASHMEM uint16_t remixMaxBars911(){uint16_t bars=1;for(uint8_t t=0;t<16;++t)bars=max(bars,trackBars[t]);return bars;}
FLASHMEM void remixFreeEdit911(){if(remixEditBefore911){remixEditBefore911->~Model();extmem_free(remixEditBefore911);remixEditBefore911=nullptr;}}
FLASHMEM rmx911::Model& remixEditModel911(){return remixSelection911.role==0?remixSource911:remixDraft911;}

FLASHMEM void remixRefresh911(){neoDisplayDirty=displayValueDirty=true;remixDisplay911(true);}
FLASHMEM bool remixSchedule911(const rmx911::Model&,rmx911::List<RemixDue911>&,bool);
FLASHMEM bool remixCapture911(){
 rmx911::Model next;
 for(uint8_t t=0;t<16;++t){auto&base=next.tracks[t];uint8_t p=activePattern[t];base.bars=trackBars[t];base.groove=patternGroove[t][p];
  for(uint8_t f=0;f<6;++f)base.roll[f]=rollFieldValue(trackRoll[t],f);
  base.delay[1]=1; // defaults only; live track Delay stays on Pidi
  for(uint8_t f=0;f<GROOVE_LIST_SIZE;++f){base.sheets[f]=grooveLists[t][p][f].sheet;base.repeats[f]=grooveLists[t][p][f].repeats;}if(base.groove==GROOVE_COUNT+2){base.groove=GROOVE_USE_LIST;const auto*list=listRead40(t,p);for(uint8_t f=0;f<14;++f){base.sheets[f]=list[f].sheet;base.repeats[f]=list[f].repeats;}}
  for(uint16_t step=0;step<trackStepCount(t);++step){uint8_t voices=noteCountAtStep(t,step);for(uint8_t voice=0;voice<voices;++voice){
   rmx911::Note n{next.nextId++,rmx911::wrap((int32_t)step*96+logicalMicroTick(t,step,voice)+defaultMov41[t][0],base.bars*1536),effectiveGateTicks(t,step,voice),effectiveMicroTick(t,step,voice),t,noteAtVoice(t,step,voice),effectiveVel(t,step,voice),0};
   n.time=clampInt(logicalMicroTick(t,step,voice)+n.time,-96,96)-logicalMicroTick(t,step,voice);
   if(!next.notes.add(n))return false;next.mask|=1u<<t;
   auto roll=configuredRollSettings(t,step,voice);for(uint8_t f=0;f<6;++f)if(!next.set(n,rmx911::Roll,f,rollFieldValue(roll,f)))return false;
   
   if(rvlLock938(t,p,(int32_t)step*96+logicalMicroTick(t,step,voice),n.pitch,rvlFixedLock938(t,step,voice))&&!next.set(n,rmx911::Roll,12,1))return false;
   if(!next.set(n,rmx911::Roll,15,rollDirection60(t,p,(int32_t)step*96+logicalMicroTick(t,step,voice),n.pitch)))return false;
   auto rx=rollExtra1000(t,p,(int32_t)step*96+logicalMicroTick(t,step,voice),n.pitch);if(!next.set(n,rmx911::Roll,13,rx.v[6])||!next.set(n,rmx911::Roll,14,rx.v[7]))return false;for(uint8_t f=0;f<6;++f)if(!next.set(n,rmx911::Roll,6+f,rx.v[f]))return false;
   if(!next.set(n,rmx911::Delay,12,1024+t))return false;
   int32_t original=(int32_t)step*96+logicalMicroTick(t,step,voice);
   for(uint16_t i=0;i<lockCount817;++i){const auto&l=noteLocks817[i];if(l.track!=t||l.pattern!=p||l.tick!=original||l.note!=n.pitch)continue;
    uint8_t kind=l.kind==LOCK_BLOCK21?rmx911::Block21:l.kind==LOCK_DELAY817?rmx911::Delay:l.kind==LOCK_CC817?rmx911::CC:l.kind==LOCK_NRPN7817?rmx911::NRPN7:l.kind==LOCK_NRPN14817?rmx911::NRPN14:255;
    if(kind==rmx911::Delay){if(l.target/256<4&&!next.set(n,kind,100+(l.target/256)*32+l.parameter,l.value+4096))return false;continue;}
    if(kind!=255&&!next.set(n,kind,l.parameter,l.value))return false;
   }
  }
  if((step&15)==15){serviceRealtimeBetweenNeoPanels();pidiService827();}}
 }
 rmx911::List<RemixDue911> schedule;if(!remixSchedule911(next,schedule,true))return false;
 ++remixEpoch913;remixSource911.swap(next);remixAuditionDue911.swap(schedule);return true;
}
FLASHMEM bool remixEnter911(){
 if(remixReady911)return true;
 if(!remixCapture911()){remixError911("REMIX MEMORY FULL");return false;}
 if(!remixDraftInitialized915){for(uint8_t t=0;t<16;++t){remixDraft911.tracks[t]=remixSource911.tracks[t];remixDraft911.tracks[t].bars=remixBars911;}remixDraftInitialized915=true;}
 remixCopySelection911.clear();remixAnchor911=-1;quickView903=0;patternMode=grooveEditMode=false;remixReady911=true;remixSourceBar911=min(remixSourceBar911,(uint16_t)(remixMaxBars911()-1));
 remixPendingSource915=remixSourceBar911;remixIndexPads915();remixScheduleDirty911=true;remixSelection911.clear();selectButtonActive=selectionMode=false;remixRefresh911();return true;
}
FLASHMEM void remixLeave911(){remixFinishSelection911(false);
remixFinishTake913();
remixRecord913=false;
remixClearAsk913=false;
if(remixDelete913){remixDelete913=deleteButtonActive=false;
}remixCancelPreview913();
remixAudition911.active=false;
remixChainRole916=-1;
remixPhaseOffset917=0;
remixReady911=false;
remixWriteView911=remixLoading911=remixConfirm911=false;
remixFreeEdit911();
remixSelection911.clear();
remixSource911.notes.clear();
remixSource911.properties.clear();
remixAuditionDue911.clear();
remixDue911.clear();
remixRecorded913.clear();
remixDisplay911(true);
}
FLASHMEM int remixSheet911(const rmx911::Track&t,uint16_t bar){if(t.groove!=GROOVE_USE_LIST)return t.groove;uint16_t total=0;for(uint8_t i=0;i<GROOVE_LIST_SIZE&&t.sheets[i];++i)total+=max((uint16_t)1,t.repeats[i]);if(!total)return 0;int position=bar%total;for(uint8_t i=0;i<GROOVE_LIST_SIZE&&t.sheets[i];++i){int n=max((uint16_t)1,t.repeats[i]);if(position<n)return t.sheets[i];position-=n;}return 0;}
FLASHMEM Block21 remixBlock21(const rmx911::Model&m,const rmx911::Note&n,uint8_t b){auto r=blocks21[linkOwner12(n.track,LINK_MOUT12)][b];r.groove=blocks21[n.track][0].groove;if(!playbackLocks828)return r;int values[4]={r.port,r.channel,r.tim,r.transpose};for(uint8_t f=0;f<4;++f){int i=m.find(n.id,rmx911::Block21,b*4+f);if(i>=0)values[f]=m.properties[i].value;}r.port=clampInt(values[0],0,8);r.channel=clampInt(values[1],0,15);r.tim=clampInt(values[2],-96,96);r.transpose=clampInt(values[3],-127,127);int g=m.find(n.id,rmx911::Block21,12);r.groove=g>=0?clampInt(m.properties[g].value,0,listCount44(n.track)):blocks21[n.track][0].groove;return r;}
FLASHMEM int remixGroove911(const rmx911::Model&m,const rmx911::Note&n,uint8_t field){int bar=(&m==&remixDraft911&&remixHearBar911)?(remixHearBar911-1)%trackBars[n.track]:n.tick/1536;auto b=remixBlock21(m,n,blockIndex21());int sheet=m.tracks[n.track].groove==GROOVE_USE_LIST?remixSheet911(m.tracks[n.track],bar):b.groove&&b.groove<=listCount44(n.track)?listSheetAtBar(reinterpret_cast<const GrooveListEntry*>(listBank44(n.track)[b.groove-1]),bar):0;if(grooveBypass23||grooveAllOff59||(grooveTrackOff59&(1u<<n.track)))sheet=0;if(field==4)return 0;if(field==3)return grooveTime16(sheet,n.tick+n.time);return sheet>0&&sheet<=GROOVE_COUNT?grooveOffset[sheet-1][(n.tick/96)%16][field]:0;}
FLASHMEM bool remixSchedule911(const rmx911::Model&m,rmx911::List<RemixDue911>&out,bool source){
 rmx911::List<RemixDue911> next;
 for(uint32_t i=0;i<m.notes.size;++i){const auto&n=m.notes[i];int period=source?m.tracks[n.track].bars*1536:remixBars911*1536;
  for(uint8_t blk=0;blk<3;++blk){BlockScope21 block(blk);auto route=remixBlock21(m,n,blk);if(!route.port)continue;
  int at=n.tick+n.time+route.tim+remixGroove911(m,n,3);
  if(!next.add({rmx911::wrap(at,period),i,0,255,blk}))return false;
  TrackRollSettings r{};for(uint8_t f=0;f<6;++f)setRollFieldValue(r,f,m.value(n,rmx911::Roll,f));
  RollExtra1000 rx;for(uint8_t f=0;f<6;++f)rx.v[f]=m.value(n,rmx911::Roll,6+f);
  rx.v[6]=m.value(n,rmx911::Roll,13);if((!r.level&&!rx.v[1])||!r.start)continue;const int origin38=at;int pos=at-r.start;at-=rollStop09(r,rx);int32_t spacing=clampInt(r.speed,1,96),acc=r.accel*256;
  for(uint16_t repeat=0;repeat<MAX_PREROLL_NOTES&&pos<at;++repeat){int rv=rollVelocity1000(r,rx,n.velocity,repeat);if(rv&&m.value(n,rmx911::Roll,15)!=1&&!next.add({rmx911::wrap(pos,period),i,(uint8_t)rv,(uint8_t)rollPitch1000(rx,n.pitch,repeat),blk,(int16_t)(pos-origin38)}))return false;if(rv&&m.value(n,rmx911::Roll,15)!=0&&!next.add({rmx911::wrap(2*origin38-pos,period),i,(uint8_t)rv,(uint8_t)rollPitch1000(rx,n.pitch,repeat),blk,(int16_t)(origin38-pos)}))return false;if(spacing>=at-pos)break;pos+=spacing;
   rollAdvance60(spacing,acc,r.expo,repeat);
  }
 }
 }
 if(next.size)qsort(next.data,next.size,sizeof(RemixDue911),[](const void*a,const void*b){auto&x=*(const RemixDue911*)a;auto&y=*(const RemixDue911*)b;return x.tick<y.tick?-1:x.tick>y.tick?1:x.index<y.index?-1:x.index>y.index?1:0;});out.swap(next);if(source)++remixSourceBuilds915;else{++remixTargetBuilds915;remixTargetPads915=0;for(uint32_t i=0;i<m.notes.size;++i)remixTargetPads915|=1u<<rmx911::targetStep912(m.notes[i].tick);}return true;
}
FLASHMEM bool remixDelay911(uint8_t track,uint8_t field,int&value){if(!remixEmittingModel911||!remixEmittingNote911||remixEmittingNote911->track!=track)return false;value=remixEmittingModel911->value(*remixEmittingNote911,rmx911::Delay,field);return true;}
FLASHMEM bool remixNoteDelay911(uint8_t track,uint8_t note,uint8_t velocity,uint32_t gate){
 if(!remixEmittingModel911||!remixEmittingNote911)return false;auto&m=*remixEmittingModel911;auto&n=*remixEmittingNote911;delayNoteLocksReset925();
 int origin=m.value(n,rmx911::Delay,12);bool modern=origin>=1024;uint8_t source=modern?origin-1024:origin?(origin-1)/16:track;
 for(uint32_t i=0;i<m.properties.size;++i){const auto&p=m.properties[i];if(p.id!=n.id||p.kind!=rmx911::Delay)continue;
  if(p.parameter>=100&&p.parameter<228)delayNoteLock925((p.parameter-100)/32,(p.parameter-100)%32,p.value-4096);
  else if(!modern&&p.parameter<12)delayNoteLock925(0,p.parameter,p.value);
  else if(!modern&&p.parameter>=13&&p.parameter<49)delayNoteLock925(1+(p.parameter-13)/12,(p.parameter-13)%12,p.value-4096);
 }pidiNote925(source,note,velocity,gate,rollTrigger817||m.value(n,rmx911::Delay,300));return true;
}
FLASHMEM bool remixLocks911(uint8_t track,uint8_t channel,uint8_t pitch,uint32_t gate){
 if(!remixEmittingModel911||!remixEmittingNote911)return false;if(!playbackLocks828)return true;auto&m=*remixEmittingModel911;auto&n=*remixEmittingNote911;
 if(rollHoldUs44)gate=rollHoldUs44;lockService817();uint16_t needed=0;for(uint32_t i=0;i<m.properties.size;++i)if(m.properties[i].id==n.id&&m.properties[i].kind>=rmx911::CC&&m.properties[i].kind<=rmx911::NRPN14)++needed;
 if(lockLeaseCount817+needed>256){remixError911("P.LOCK VOICE LIMIT");return true;}
 MidiScope902 route(outMask902(track),track);uint32_t now=micros();auto&ctx=patch808Context(track,activePattern[track]);uint8_t target=patch808Channel(track,ctx);
 for(uint32_t i=0;i<m.properties.size;++i){const auto&p=m.properties[i];if(p.id!=n.id||p.kind<rmx911::CC||p.kind>rmx911::NRPN14)continue;
  uint8_t kind=p.kind==rmx911::CC?LOCK_CC817:p.kind==rmx911::NRPN7?LOCK_NRPN7817:LOCK_NRPN14817;int old=0,top=lockTop817(kind,target,p.parameter);
  if(rollHoldUs44){bool found=false;for(auto&l:lockLeases817)if(l.used&&l.rollHold44&&l.rollIdentity44==rollIdentity44&&l.kind==kind&&l.target==target&&l.parameter==p.parameter&&l.outputMask==outMask902(track)&&l.value==p.value){found=true;break;}if(found)continue;}
  if(top>=0)old=lockLeases817[top].previous;else if(kind==LOCK_CC817&&midiPending810[firstOut12(track)*16+target][p.parameter].known)old=midiPending810[firstOut12(track)*16+target][p.parameter].last;
  else for(auto*q=patchNext819(ctx,nullptr);q;q=patchNext819(ctx,q))if(q->number==p.parameter&&q->kind==(kind!=LOCK_CC817)){uint16_t v=0;patchValue819(ctx,*q,v);old=v;break;}
  for(auto&lease:lockLeases817)if(!lease.used){lease={now+gate,0,++lockSerial817,p.value,old,target,p.parameter,kind,channel,pitch,true,false,outMask902(track)};lease.rollHold44=rollHoldUs44!=0;lease.rollIdentity44=rollIdentity44;++lockLeaseCount817;break;}
  lockSend817(kind,target,p.parameter,p.value);
 }return true;
}
FLASHMEM void remixEmit911(const rmx911::Model&m,const RemixDue911&due){const auto&n=m.notes[due.index];BlockScope21 block(due.block21);blockResolved21=remixBlock21(m,n,due.block21);blockResolvedValid21=true;
 if(trackSilent818(n.track))return;
 if(due.velocity&&!mixerOn938(n.track,0)&&(!playbackLocks828||!(m.value(n,rmx911::Roll,12)||m.find(n.id,rmx911::Roll,5)>=0)))return;
 if(&m==&remixDraft911&&remixRecordCycle913==transportTickAbsolute/1536)for(uint32_t i=0;i<remixRecorded913.size;++i)if(remixRecorded913[i]==n.id)return;
 int pitch=clampInt((due.pitch<128?due.pitch:n.pitch)+remixGroove911(m,n,0),0,127),vel=clampInt((due.velocity?due.velocity:n.velocity)+remixGroove911(m,n,1),0,127);
 uint32_t gate=clampInt(n.gate+remixGroove911(m,n,2),GATE_TICK_MIN,GATE_TICK_MAX);
 remixEmittingModel911=&m;remixEmittingNote911=&n;remixVoiceSent913=false;
 bool previousRoll=rollTrigger817;rollTrigger817=due.velocity!=0;
 if(rollTrigger817){TrackRollSettings roll{};for(uint8_t f=0;f<6;++f)setRollFieldValue(roll,f,m.value(n,rmx911::Roll,f));RollExtra1000 extra;for(uint8_t f=0;f<6;++f)extra.v[f]=m.value(n,rmx911::Roll,6+f);extra.v[6]=m.value(n,rmx911::Roll,13);extra.v[7]=m.value(n,rmx911::Roll,14);rollEvent44(n.track,roll,extra,due.rollOffset44,n.id,gate,m.value(n,rmx911::Roll,15));}
 triggerTrackNoteWithFixedGate(n.track,pitch,vel,max(1000UL,(uint32_t)(gate*tickDurationMs()*1000.0f)),remixGroove911(m,n,4)*100);
 rollTrigger817=previousRoll;rollHoldUs44=0;
 remixEmittingModel911=nullptr;remixEmittingNote911=nullptr;
 if(remixVoiceSent913)remixRecordNote913(m,due.index,due.velocity,due.pitch);
}
FLASHMEM void remixRunDue911(const rmx911::Model&m,const rmx911::List<RemixDue911>&list,uint8_t track,int tick){uint32_t lo=0,hi=list.size;while(lo<hi){uint32_t mid=(lo+hi)/2;if(list[mid].tick<tick)lo=mid+1;else hi=mid;}for(;lo<list.size&&list[lo].tick==tick;++lo)if(m.notes[list[lo].index].track==track)remixEmit911(m,list[lo]);}
bool remixTrack911(uint8_t track,uint32_t tick){
 if(!remixMode)return false;uint64_t commonTick=transportTickAbsolute;auto phase=remixPhase917(commonTick);

 if(rmx916::replace(remixAudition911.active,remixAudition911.tracks,track,remixHit916))return true;
 if(phase==rmx913::Wait)return true;
 if(phase==rmx913::Source)return false;
 if(trackSilent818(track)||tickExpired830())return true;
 // The common target bar replaces the source even on a muted target track.
 if((remixTargetMask911&(1u<<track))&&!remixScheduleDirty911)remixRunDue911(remixDraft911,remixDue911,track,commonTick%1536);return true;
}
FLASHMEM void remixStartAudition911(uint8_t pad,uint8_t role=0){
 if(!remixFinishTake913())return;remixQuantShift913=rmx913::quantShift(globalLoopedTick(),remixQuantTicks913[remixQuan913]);
 remixChainRole916=-1;
 remixCopySelection911.clear();remixAnchor911=pad*96;remixAudition911={true,true,pad,micros(),micros(),0,(uint32_t)remixLength911,remixSourceMask911,pad*96+remixLatency911};
 remixAudition911.role=role;
 if(role){remixAudition911.tracks=remixTargetMask911;}else remixStoreClipboard915();
 if(remixScheduleDirty911){if(!remixSchedule911(remixDraft911,remixDue911,false)){remixAudition911.active=false;remixError911("REMIX MEMORY FULL");return;}remixScheduleDirty911=false;}
 remixAudition911.at=remixAudition911.next=micros();remixService911();
}
void remixService911(){
 if(!remixMode)return;remixSessionService913();
 if(remixScheduleDirty911){if(remixSchedule911(remixDraft911,remixDue911,false))remixScheduleDirty911=false;else{remixHearBar911=0;remixError911("REMIX MEMORY FULL");}}
 auto&a=remixAudition911;if(!a.active){remixFinishTake913();return;}uint32_t now=micros(),period=max(1UL,internalClockPeriodUs());uint8_t budget=8;
 // Skip expired attacks without freezing musical position; note-offs remain serviced.
 int32_t late=(int32_t)(now-a.next);if(late>(int32_t)LATE_LIMIT830){uint32_t skip=((uint32_t)late-LATE_LIMIT830+period-1)/period;a.next+=skip*period;a.ticks+=skip;
  if(a.ticks>=a.length&&!(remixHold911&&a.held)){if(remixLoop911)a.ticks%=a.length;else a.active=false;}
 }
 while(a.active&&(int32_t)(now-a.next)>=0&&budget--){
  for(uint8_t t=0;t<16;++t)if(a.tracks&(1u<<t)){
   const auto&m=a.role?remixDraft911:remixSource911;const auto&due=a.role?remixDue911:remixAuditionDue911;
   int periodTicks=a.role?remixBars911*1536:m.tracks[t].bars*1536;
   int at=(a.role?remixTargetBar911:remixSourceBar911%m.tracks[t].bars)*1536+a.start+a.ticks;
   remixRunDue911(m,due,t,rmx911::wrap(at,periodTicks));
  }
  ++a.ticks;a.next+=period;
  if(a.ticks>=a.length&&!(remixHold911&&a.held)){if(remixLoop911)a.ticks=0;else a.active=false;}
 }

 if(!a.active)remixFinishTake913();
}
FLASHMEM bool remixCommitStage911(rmx911::Model&stage,rmx911::Model&model,rmx911::History&history){if(!history.push(model,stage)){remixError911("UNDO MEMORY FULL");return false;}model.swap(stage);remixScheduleDirty911=true;remixRefresh911();return true;}
FLASHMEM bool remixPaste911(uint8_t pad){if(!remixFinishTake913())return false;return remixPasteClipboard915(pad);}
FLASHMEM bool remixPad(uint8_t n,uint8_t p,uint8_t edge){
 if(!remixMode)return false;if(n>=4||p>=16)return true;bool down=edge==SEESAW_KEYPAD_EDGE_RISING;uint8_t raw=p;neoPadDown[n][raw]=down;if(n<2){if(down)remixHeldMap913[n][raw]=rmx913::step(raw,remixLayout913);p=remixHeldMap913[n][raw];}else p=rmx913::step(raw,remixChannels916);
 if(remixWriteView911||remixBrowser911||menuNavigationOnly())return true;
 if(remixHit916==rmx916::Chain&&n<2&&!selectButtonActive&&!remixDelete913){if(playing){if(down)remixPosition916(n,p);return true;}if(down){remixStartAudition911(p,n);neoDisplayDirty=true;return true;}}
 if(!down){if(n==remixAudition911.role&&remixAudition911.pad==p&&remixAudition911.held){remixAudition911.held=false;if(remixHold911){remixLength911=clampInt((micros()-remixAudition911.at)/max(1UL,internalClockPeriodUs()),1,remixBars911*1536);remixAudition911.length=remixLength911;remixAudition911.active=remixLoop911;if(!remixAudition911.role)remixStoreClipboard915();remixRefresh911();}}return true;}
 if(n>=2){uint16_t&mask=n==2?remixSourceMask911:remixTargetMask911;mask^=1u<<p;selectedTrack=p;
 if(n==3&&!(mask&(1u<<p))&&remixPhase917(transportTickAbsolute)==rmx913::Target)stopActiveNotesForTrack(p);if(n==2&&remixSelection911.role==0)remixSelection911.tracks=mask;remixRefresh911();return true;}
 if(remixDelete913){if(n==1)remixDeleteStep913(p);return true;}
 if(selectButtonActive){if(n!=1)return true;
int8_t previousRole=remixSelection911.role;
if(remixEditBefore911&&previousRole!=n){remixError911("VALIDATE OR CANCEL SEL");
return true;
}if(!remixSelection911.choose(n,p)){remixError911("SRC OR RMX ONLY");
return true;
}if(remixEditBefore911&&remixSelection911.role<0)remixSelection911.role=previousRole;
remixSelection911.tracks=n?0xFFFF:remixSourceMask911;
remixSelection911.bar=n?remixTargetBar911:remixSourceBar911;
remixRefresh911();
return true;
}
 if(n==0)remixStartAudition911(p);else remixPaste911(p);neoDisplayDirty=true;return true;
}
FLASHMEM bool remixRender(){if(!remixMode)return false;for(uint8_t n=0;n<4;++n){if(!(neoAvailable[n]||pc831.active))continue;for(uint8_t raw=0;raw<16;++raw){uint8_t p=rmx913::step(raw,n<2?remixLayout913:remixChannels916);uint32_t color=0;
 if(n==2)color=(remixSourceMask911&(1u<<p))?colorFor(n,0,55,0):0;
 else if(n==3)color=(remixTargetMask911&(1u<<p))?colorFor(n,0,25,75):0;
 else {color=colorFor(n,n?10:25,n?15:0,n?45:25);uint16_t occupied=remixTargetPads915;if(!n){occupied=0;for(uint8_t t=0;t<16;++t)if(remixSourceMask911&(1u<<t))occupied|=remixSourcePads915[t];}if(occupied&(1u<<p))color=colorFor(n,n?0:30,n?35:40,n?75:0);
  if(n==1&&(remixSelection911.exiled&(1u<<p)))color=colorFor(n,45,25,0);if(remixSelection911.role==n&&(remixSelection911.steps&(1u<<p)))color=colorFor(n,0,90,90);if(playing){auto phase=remixPhase917(transportTickAbsolute);if(((n==0&&phase==rmx913::Source)||(n==1&&phase==rmx913::Target))&&p==(globalLoopedTick()/96)%16)color=colorForRole(n,(occupied&(1u<<p))?NEO_COLOR_NOTE_PLAYHEAD:NEO_COLOR_PLAYHEAD);}if(neoPadDown[n][raw])color=colorFor(n,80,80,80);}
 neoPixel821(n,raw,color);}diagShowNeo814(n);serviceRealtimeBetweenNeoPanels();}return true;}
FLASHMEM bool remixFinishSelection911(bool accept){
 if(accept&&remixSelection911.role==0&&remixSelection911.steps){remixCopySelection911=remixSelection911;if(!remixStoreClipboard915())return false;}
 if(remixEditBefore911){if(accept){remixUndoRole911=remixSelection911.role;if(!remixHistory911.push(*remixEditBefore911,remixDraft911)){remixError911("UNDO MEMORY FULL");return false;}}else remixEditModel911().swap(*remixEditBefore911);remixFreeEdit911();remixScheduleDirty911=true;}
 remixSelection911.clear();selectButtonActive=selectionMode=false;remixRefresh911();sendSelectionIndicatorsToPicran();return true;
}
FLASHMEM bool remixButton911(uint8_t bit){if(!remixMode)return false;
 if(bit==11)return true;
 if(remixHit916==rmx916::Chain&&(bit==EDIT_BUTTON_BITS[EDIT_BUTTON_SELECT]||bit==EDIT_BUTTON_BITS[EDIT_BUTTON_DELETE]))return true;
 if(bit==12||bit==13){if(remixFinishSelection911(true)&&remixFinishTake913())remixBrowserOpen911(bit==12);return true;}
 if(fileBrowserMode!=FILE_BROWSER_NONE)return false;
 if(bit==EDIT_BUTTON_BITS[EDIT_BUTTON_SELECT]){if(!remixFinishTake913())return true;if(remixDelete913){remixDelete913=deleteButtonActive=false;}if(selectButtonActive)remixFinishSelection911(true);else{selectButtonActive=selectionMode=true;remixSelection911.clear();sendSelectionIndicatorsToPicran();}return true;}
 if(bit==EDIT_BUTTON_BITS[EDIT_BUTTON_UNDO]||bit==EDIT_BUTTON_BITS[EDIT_BUTTON_REDO]){if(remixEditBefore911){remixError911("VALIDATE OR CANCEL SEL");return true;}if(!remixFinishTake913())return true;if(remixHistory911.move(remixDraft911,bit==EDIT_BUTTON_BITS[EDIT_BUTTON_UNDO]?-1:1))remixScheduleDirty911=true;remixRefresh911();return true;}
 if(bit==EDIT_BUTTON_BITS[EDIT_BUTTON_DELETE]){
  if(remixSelection911.steps){
   if(remixSelection911.role==0){remixError911("SOURCE READ ONLY");return true;}
   auto selection=remixSelection911;if(!remixFinishSelection911(true)||!remixFinishTake913())return true;
   rmx911::Model stage;if(!stage.clone(remixDraft911)){remixError911("DELETE MEMORY FULL");return true;}
   for(uint32_t i=0;i<stage.notes.size;)if(selection.contains(stage.notes[i],stage,false))stage.remove(i);else ++i;
   remixUndoRole911=1;if(remixCommitStage911(stage,remixDraft911,remixHistory911)){remixDelete913=deleteButtonActive=false;remixRefresh911();sendSelectionIndicatorsToPicran();}return true;
  }
  if(!remixFinishSelection911(true)||!remixFinishTake913())return true;
  remixDelete913=!remixDelete913;deleteButtonActive=remixDelete913;remixClearAsk913=false;
  sendSelectionIndicatorsToPicran();remixDisplay911(true);return true;
 }

 return false;
}
FLASHMEM bool remixBack911(){if(!remixMode)return false;
if(remixPendingSource915!=remixSourceBar911){remixPendingSource915=remixSourceBar911;
remixDisplay911(true);
return true;
}if(remixLayoutPreview913){remixCancelPreview913();
return true;
}if(remixClearAsk913){remixClearAsk913=false;
remixDisplay911(true);
return true;
}if(remixDelete913){remixDelete913=deleteButtonActive=false;
sendSelectionIndicatorsToPicran();
remixDisplay911(true);
return true;
}if(remixBrowser911){remixBrowser911=false;
closeFileBrowser();
remixRefresh911();
return true;
}if(remixWriteView911){remixWriteView911=remixLoading911=remixConfirm911=false;
remixRefresh911();
showGameScreen();
return true;
}if(selectButtonActive||remixEditBefore911){remixFinishSelection911(false);
return true;
}remixAudition911.active=false;
quickView903=0;
remixRefresh911();
return true;
}
FLASHMEM bool remixQv911(uint8_t view){if(!remixMode)return false;remixClearAsk913=false;remixCancelPreview913();display821.invalidate();memset(modPreviousLcd806,0,sizeof(modPreviousLcd806));memset(remixPrevious915,0,sizeof(remixPrevious915));if(view==4||view==5)return false;quickView903=view==6?0:view;remixDisplay911(true);return true;}
FLASHMEM void remixEditDelta911(uint8_t field,int delta,bool absolute=false){if(remixSelection911.role!=1||!remixSelection911.steps){remixError911(remixSelection911.role==0?"SOURCE READ ONLY":"SELECT RMX STEP");
return;
}auto&m=remixEditModel911();
bool found=false;
for(uint32_t i=0;
i<m.notes.size;
++i)found|=remixSelection911.contains(m.notes[i],m,false);
if(!found){remixError911("RMX STEP EMPTY");
return;
}rmx911::Model stage;
if(!stage.clone(m)){remixError911("REMIX MEMORY FULL");
return;
}
 for(uint32_t i=0;i<stage.notes.size;++i){auto&n=stage.notes[i];if(!remixSelection911.contains(n,stage,remixSelection911.role==0))continue;
  if((field==9||field==23||field==24)&&!stage.set(n,rmx911::Roll,12,1)){remixError911("P.LOCK MEMORY FULL");return;}
  if(field==0)n.pitch=clampInt((absolute?delta:n.pitch+delta),0,127);else if(field==1)n.velocity=clampInt((absolute?delta:n.velocity+delta),1,127);else if(field==2)n.gate=clampInt((absolute?delta:n.gate+delta),GATE_TICK_MIN,GATE_TICK_MAX);else if(field==3)n.time=(absolute?clampInt(delta,-96,96):ZCLAMP57(n.time,delta,-96,96));
  else if(field==44){int i=stage.find(n.id,rmx911::Block21,12);int base=i>=0?stage.properties[i].value:blocks21[n.track][0].groove;if(!stage.set(n,rmx911::Block21,12,clampInt(absolute?delta:base+delta,0,listCount44(n.track)))){remixError911("P.LOCK MEMORY FULL");return;}}
  else if(field>=32&&field<44){uint8_t f=(field-32)%4,b=(field-32)/4;auto route=remixBlock21(stage,n,b);int base=f==0?route.port:f==1?route.channel:f==2?route.tim:route.transpose;int v=clampInt(absolute?delta:base+delta,f==2?-96:f==3?-127:0,f==0?8:f==1?15:f==2?96:127);if(!stage.set(n,rmx911::Block21,field-32,v)){remixError911("P.LOCK MEMORY FULL");return;}}
  else if(field==28){int v=clampInt(absolute?delta:stage.value(n,rmx911::Roll,13)+delta,0,max(0,stage.value(n,rmx911::Roll,0)-1));if(!stage.set(n,rmx911::Roll,13,v)){remixError911("P.LOCK MEMORY FULL");return;}}
  else if(field>=22&&field<28){uint8_t f=field-22;int v=absolute?delta:stage.value(n,rmx911::Roll,6+f)+delta;v=clampInt(v,(f==3&&stage.value(n,rmx911::Roll,10))?0:rollMin1000(f),rollMax1000(f));if(f==4&&v&&stage.value(n,rmx911::Roll,9)<0&&!stage.set(n,rmx911::Roll,9,0)){remixError911("P.LOCK MEMORY FULL");return;}if(!stage.set(n,rmx911::Roll,6+f,v)){remixError911("P.LOCK MEMORY FULL");return;}}
  else {uint8_t kind=field<10?rmx911::Roll:rmx911::Delay,param=field<10?field-4:field-10;
uint16_t stored=kind==rmx911::Delay?100+delaySlot924*32+param:param;
int at=stage.find(n.id,kind,stored);
int old=kind==rmx911::Delay?(at>=0?stage.properties[at].value-4096:delayBaseSlot924(n.track,0,delaySlot924,param)):stage.value(n,kind,param);
int v=absolute?delta:old+delta;
v=kind==rmx911::Roll?clampRollFieldValue(param,v):clampInt(v,delayMin907(param),param==8?2:delayMax907(param));
if(!stage.set(n,kind,stored,kind==rmx911::Delay?v+4096:v)){remixError911("P.LOCK MEMORY FULL");
return;
}}
 }
 if(!remixEditBefore911)remixEditBefore911=rmx911::History::copy(m);if(!remixEditBefore911){remixError911("UNDO MEMORY FULL");return;}m.swap(stage);if(field>=10&&field<22)for(uint32_t i=0;i<m.notes.size;++i)if(remixSelection911.contains(m.notes[i],m,false))delayMarkWorked930(m.notes[i].track,delaySlot924);remixSelection911.edited|=remixSelection911.steps;remixScheduleDirty911=true;remixRefresh911();
}
FLASHMEM int remixRollValue1000(uint8_t field){auto&m=remixEditModel911();for(uint32_t i=0;i<m.notes.size;++i)if(remixSelection911.contains(m.notes[i],m,false))return m.value(m.notes[i],rmx911::Roll,field);return 0;}
FLASHMEM bool remixKy911(uint8_t ky,int32_t delta){if(!delta)return true;
 if(remixWriteView911){remixConfirm911=false;
if(remixLoading911){if(ky==0)remixLoadSource42=delta>0;else if(ky==1)remixLoadBar42=clampInt(remixLoadBar42+delta,0,63);remixConfirm911=false;remixDisplay911(true);return true;}
if(ky==0)remixWriteCurrent911=delta<0;
else if(ky==1&&!remixWriteCurrent911)remixWriteRow911=clampInt(remixWriteRow911+delta,remixLoading911?0:remixWriteMinimum911,15);
else if(ky==2&&!remixLoading911)remixPre911=clampInt(remixPre911+delta,0,63);
else if(ky==3&&!remixLoading911)remixCopies911=clampInt(remixCopies911+delta,1,64);
else if(ky==4&&!remixLoading911)remixPost911=clampInt(remixPost911+delta,0,63);
else if(ky==5)remixLink911=delta>0;
remixDisplay911(true);
return true;
}
 if(quickView903==0){if(remixClearAsk913&&ky==10){remixClearYes913=delta>0;remixDisplay911(true);return true;}remixClearAsk913=false;
 switch(ky){
 case 0:if(!selectButtonActive)remixPendingSource915=clampInt(remixPendingSource915+delta,0,remixMaxBars911()-1);break;
 case 1:remixPhaseOffset917=0;remixChainRole916=-1;remixHearBar911=clampInt(remixHearBar911+delta,0,MAX_BARS);remixWait913=min((int)remixWait913,max(0,(int)remixHearBar911-1));remixScheduleDirty911=true;break;
 case 2:remixPhaseOffset917=0;remixChainRole916=-1;remixWait913=clampInt(remixWait913+delta,0,max(0,(int)remixHearBar911-1));break;
 case 3:if(!remixFinishTake913())return true;remixHit916=clampInt(remixHit916+delta,0,2);remixChain911=remixHit916==rmx916::Chain;remixAudition911.active=false;remixChainRole916=-1;if(remixChain911){remixFinishSelection911(false);remixDelete913=deleteButtonActive=false;remixRecord913=false;sendSelectionIndicatorsToPicran();}break;
 case 4:remixLatency911=ZCLAMP57(remixLatency911,delta,-1536,1536);break;
 case 5:remixLen916=clampInt(remixLen916+delta,0,2);remixHold911=remixLen916==rmx916::Hold;remixLength911=remixLen916==rmx916::Pad?96:remixFixed916;break;
 case 6:if(remixLen916==rmx916::Fixed){remixFixed916=rmx916::aligned(remixFixed916,delta,(const int[]){1,192,384}[remixQuantum916]);remixLength911=remixFixed916;}break;
 case 7:remixLoop911=delta>0;break;
 case 8:remixQuan913=clampInt(remixQuan913+delta,0,4);break;
 case 12:remixChannelsPreview917=false;remixLayout913=rmx911::wrap(remixLayout913+delta,4);savePrefs932();remixLayoutPreview913=true;remixLayoutUntil913=millis()+(uint32_t)(240000.0f/bpm);remixLayoutEp913();break;
 case 13:remixChannelsPreview917=true;remixChannels916=rmx911::wrap(remixChannels916+delta,4);savePrefs932();remixLayoutPreview913=true;remixLayoutUntil913=millis()+(uint32_t)(240000.0f/bpm);remixLayoutEp913();break;
 }if((ky==4||ky==5||ky==6)&&remixAnchor911>=0&&remixHit916!=rmx916::Chain)remixStoreClipboard915();remixRefresh911();return true;}
 if(quickView903==1){if(ky>=2&&ky<=5)remixEditDelta911(ky-2,ky==4?delta*(gateEditMode==GATE_EDIT_1T?1:96):delta);else if(ky==8)remixEditDelta911(remixRollValue1000(7)?24:9,delta);else if(ky==15)remixEditDelta911(22,delta);else if(ky>=10&&ky<=14)remixEditDelta911(ky-6,delta);return true;}
 if(quickView903==2){if(ky==7){remixEditDelta911(28,delta);return true;}if(ky==10&&remixRollValue1000(7)){remixEditDelta911(24,delta);return true;}if(ky==11){remixEditDelta911(23,delta>0?1:0,true);return true;}if(ky==12||ky==14||ky==15){remixEditDelta911(ky==12?22:ky==14?27:25,delta);return true;}if(!(ky%2)&&ky<12)remixEditDelta911(4+ky/2,delta);return true;}
 if(quickView903==3){if(!(ky%2))remixEditDelta911(10+delayFields907[ky/2],delta);return true;}
 // ENV/LFO keep their existing editors; Patch cannot enter this path.
 if(quickView903==4||quickView903==5)return quickKy903(ky,delta);return true;
}
FLASHMEM void remixKey911(uint8_t ky,bool pressed){if(ky>=16)return;uint16_t bit=1u<<ky;bool was=remixKeys911&bit;if(pressed)remixKeys911|=bit;else remixKeys911&=~bit;if(!pressed||was||!remixMode)return;
 if(quickView903==1&&ky==4){toggleTimeUnit38();remixDisplay911(true);return;}
 if((quickView903==1&&ky==8)||(quickView903==2&&ky==10)){remixEditDelta911(23,!remixRollValue1000(7),true);return;}
 if(quickView903==2&&ky==15){remixEditDelta911(26,!remixRollValue1000(10),true);return;}
 if(!quickView903&&!remixWriteView911&&ky==0&&remixPendingSource915!=remixSourceBar911){if(!remixFinishSelection911(true)||!remixFinishTake913())return;
remixAudition911.active=false;
if(remixCapture911()){remixSourceBar911=remixPendingSource915;
remixIndexPads915();
remixAnchor911=-1;
remixCopySelection911.clear();
}else remixError911("SOURCE MEMORY FULL");
remixRefresh911();
return;
}if(!quickView903&&!remixWriteView911&&ky==10){if(!remixClearAsk913){remixClearAsk913=true;
remixClearYes913=false;
}else{bool yes=remixClearYes913;
remixClearAsk913=false;
if(yes)remixClearDraft913();
}remixDisplay911(true);
return;
}if(!quickView903&&!remixWriteView911&&ky==6&&remixLen916==rmx916::Fixed){remixQuantum916=(remixQuantum916+1)%3;
remixFine911=remixQuantum916==0;
remixError911((const char*[]){"FIX: LIBRE","FIX: CROCHE","FIX: NOIRE"}[remixQuantum916]);
remixDisplay911(true);
}}
FLASHMEM void remixOpenWrite(){if(!remixFinishSelection911(true)||!remixFinishTake913())return;
remixCancelPreview913();
remixClearAsk913=false;
remixWriteMinimum911=0;
for(uint8_t t=0;
t<16;
++t)for(uint8_t p=0;
p<16;
++p)if(songPatternPresent(t,p))remixWriteMinimum911=max(remixWriteMinimum911,(uint8_t)(p+1));
remixWriteRow911=min(remixWriteMinimum911,(uint8_t)15);
remixWriteView911=true;
remixWriteCurrent911=false;
remixConfirm911=false;
remixDisplay911(true);
mod806LiveFrame("WRITE REMIX","Reglages E1-E4","","Valider         Back");
}
FLASHMEM bool remixScreen(uint8_t index){if(!remixMode||!remixWriteView911)return false;if(index==7){remixBack911();return true;}if(index!=4&&index!=0)return true;
 if(!remixWriteCurrent911&&!remixLoading911&&remixWriteMinimum911>=16){remixError911("NO FREE PATTERN ROW");return true;}
 if(remixLoading911){if(!remixLoadChoice911())return true;return true;}
 rmx911::Model output;if(!rmx911::export912(output,remixDraft911,remixTargetMask911)){remixError911("EXPORT MEMORY FULL");return true;}if(!output.mask){remixError911("NO ACTIVE RMX TRACK");return true;}
 if(remixInstall911(output,remixWriteCurrent911,remixWriteRow911,remixPre911,remixCopies911,remixPost911,false)){remixWriteView911=false;remixRefresh911();showGameScreen();}return true;}
FLASHMEM void remixDisplay911(bool force){static bool visible=false;static uint32_t at=0;auto&previous=remixPrevious915;
 if(!remixMode){if(visible||force){DISPLAY_SERIAL.println("RMX910,OFF");visible=false;memset(previous,0,sizeof(previous));memset(modPreviousLcd806,0,sizeof(modPreviousLcd806));displayValueDirty=true;}return;}
 if(!force&&(!remixMode||remixWriteView911||quickView903||remixPendingSource915==remixSourceBar911))return;
 if(!force&&millis()-at<120)return;at=millis();
 if(quickView903>=3&&!remixWriteView911){if(visible){DISPLAY_SERIAL.println("RMX910,OFF");visible=false;memset(previous,0,sizeof(previous));}quickDisplay903();return;}
 visible=true;char rows[4][2][17]={};auto cell=[&](uint8_t ky,const char*label,const char*value){uint8_t screen=ky/4,col=(ky%4)*4;memcpy(rows[screen][0]+col,label,min((size_t)4,strlen(label)));memcpy(rows[screen][1]+col,value,min((size_t)4,strlen(value)));};for(auto&r:rows)for(auto&line:r){memset(line,' ',16);line[16]=0;}
 auto number=[&](uint8_t ky,const char*label,int n){char v[16];snprintf(v,sizeof(v),"%d",n);cell(ky,label,v);};
 if(remixWriteView911&&remixLoading911){cell(0,"Dest",remixLoadSource42?"Src":"Rmx");if(remixLoadSource42)number(1,"Bar",remixLoadBar42+1);snprintf(rows[2][0],17,"LOAD REMIX");snprintf(rows[2][1],17,"%s",remixConfirm911?"REPLACE? EP OK":"CONFIRM ON EP");}
 else if(remixWriteView911){cell(0,"Dest",remixWriteCurrent911?"Curr":"New");number(1,"Row",remixWriteRow911+1);number(2,remixWriteCurrent911?"Wait":"Pre",remixPre911);number(3,"Copy",remixCopies911);number(4,"Post",remixPost911);cell(5,"Link",remixLink911?"ON":"OFF");snprintf(rows[2][0],17,"%s",remixLoading911?"LOAD PLINK":"WRITE REMIX");snprintf(rows[2][1],17,"%s",remixConfirm911?"REPLACE? EP OK":"CONFIRM ON EP");}
 else if(!quickView903){
 if(remixPendingSource915==remixSourceBar911||millis()%600<300)number(0,"Src",remixPendingSource915+1);else cell(0,"Src","");
 if(remixHearBar911)number(1,"Hear",remixHearBar911);else cell(1,"Hear","OFF");number(2,"Wait",remixWait913);cell(3,"Hit",(const char*[]){"PadO","PadR","CHN"}[remixHit916]);
 number(4,"LAT",remixLatency911);cell(5,"Len",(const char*[]){"PAD","FIX","HOLD"}[remixLen916]);
 if(remixLen916==rmx916::Fixed)number(6,"Fix",remixFixed916);cell(7,"Loop",remixLoop911?"ON":"OFF");
 cell(8,"Quan",(const char*[]){"OFF","1/32","1/16","1/8","1/4"}[remixQuan913]);cell(10,remixClearAsk913?"SURE":"CLR",remixClearAsk913?(remixClearYes913?"YES":"NO"):"PUSH");number(12,"Trix",remixLayout913+1);number(13,"ChVw",remixChannels916+1);
 }

 else {auto&m=remixEditModel911();const rmx911::Note*chosen=nullptr;for(uint32_t i=0;i<m.notes.size;++i)if(remixSelection911.contains(m.notes[i],m,remixSelection911.role==0)){chosen=&m.notes[i];break;}
  if(quickView903==1){if(chosen){cell(0,remixSelection911.role==0?"SRC":"RMX",remixSelection911.role==0?"READ":"EDIT");
number(2,"NOT",chosen->pitch);
number(3,"VEL",chosen->velocity);
number(4,"GAT",chosen->gate);
number(5,"TIM",chosen->time);
number(8,"RVL",m.value(*chosen,rmx911::Roll,m.value(*chosen,rmx911::Roll,7)?8:5));
number(15,"Vex",m.value(*chosen,rmx911::Roll,6));
for(uint8_t f=0;
f<5;
++f)number(10+f,(const char*[]){"RST","RSP","RAC","Sex","RCV"}[f],m.value(*chosen,rmx911::Roll,f));
}else snprintf(rows[0][0],17,"%s",remixSelection911.role==0?"SOURCE READ ONLY":remixSelection911.steps?"RMX STEP EMPTY":"SEL RMX STEP");
}
  else if(quickView903==2){
   const char*labels[]={"RST","","RSP","","RAC","","Sex","RTP","RCV","","RVL","Mode","Vex","","ROT","NOS"};
   for(uint8_t k=0;k<16;++k)if(*labels[k]){if(!chosen){cell(k,labels[k],"--");continue;}
    int f=k==7?13:k<10?k/2:k==10?(m.value(*chosen,rmx911::Roll,7)?8:5):k==11?7:k==12?6:k==14?11:9;
    if(k==11)cell(k,labels[k],m.value(*chosen,rmx911::Roll,7)?"+/-":"Fix");else number(k,labels[k],m.value(*chosen,rmx911::Roll,f));
   }
  }
  else for(uint8_t f=0;
f<(quickView903==2?6:8);
++f){const char*label=quickView903==2?(const char*[]){"Start","Speed","Accel","Sex","Curve","RVL"}[f]:delayLabels907[f];
uint8_t ky=f*2;
char value[12];
if(chosen)snprintf(value,sizeof(value),"%d",m.value(*chosen,quickView903==2?rmx911::Roll:rmx911::Delay,quickView903==2?f:delayFields907[f]));
else strcpy(value,"--");
uint8_t screen=ky/4,col=(ky%4)*4;
memcpy(rows[screen][0]+col,label,min((size_t)8,strlen(label)));
memcpy(rows[screen][1]+col,value,min((size_t)8,strlen(value)));
}
 }
 for(uint8_t i=0;i<4;++i){char a[17],b[17];snprintf(a,17,"%-16.16s",rows[i][0]);snprintf(b,17,"%-16.16s",rows[i][1]);if(memcmp(previous[i][0],a,17)||memcmp(previous[i][1],b,17)){memcpy(previous[i][0],a,17);memcpy(previous[i][1],b,17);DISPLAY_SERIAL.printf("RMX910|%u|%s|%s\n",i,a,b);}}
}

FLASHMEM int remixPatchValue937(const PatchParam808&q,int base){
 auto&m=remixDraft911;uint8_t kind=q.kind?(q.bits==14?rmx911::NRPN14:rmx911::NRPN7):rmx911::CC;
 for(uint32_t i=0;i<m.notes.size;++i)if(remixSelection911.contains(m.notes[i],m,false)){int at=m.find(m.notes[i].id,kind,q.number);return at<0?base:m.properties[at].value;}return base;
}
FLASHMEM void remixPatchEdit937(const PatchParam808&q,int base,int delta){
 if(remixSelection911.role!=1||!remixSelection911.steps){remixError911("SELECT RMX STEP");return;}
 auto&m=remixDraft911;rmx911::Model stage;if(!stage.clone(m)){remixError911("P.LOCK MEMORY FULL");return;}
 uint8_t kind=q.kind?(q.bits==14?rmx911::NRPN14:rmx911::NRPN7):rmx911::CC;
 for(uint32_t i=0;i<stage.notes.size;++i){auto&n=stage.notes[i];if(!remixSelection911.contains(n,stage,false))continue;int at=stage.find(n.id,kind,q.number);int value=at<0?base:stage.properties[at].value;
  if(!stage.set(n,kind,q.number,clampInt(value+delta,0,q.bits==14?16383:127))){remixError911("P.LOCK MEMORY FULL");return;}}
 if(!remixEditBefore911)remixEditBefore911=rmx911::History::copy(m);if(!remixEditBefore911){remixError911("UNDO MEMORY FULL");return;}
 m.swap(stage);remixSelection911.edited|=remixSelection911.steps;remixScheduleDirty911=true;remixRefresh911();
}
