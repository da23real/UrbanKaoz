#pragma once
static EXTMEM mod54::Config remoteConfig54[64],remoteSentConfig54[64];
static EXTMEM mod54::Source remoteSources54[64],remoteSentSources54[64];
static EXTMEM mod54::Route remoteRoutes54[512],remoteSentRoutes54[512];
static EXTMEM int8_t remoteMap54[64][17];static uint64_t remoteMask54=0;
static uint16_t remoteRouteCount54=0,remoteSentCount54=65535;static uint8_t remoteSentNodes54=255;
static bool remoteUploading54=false;static uint64_t remotePlanMask54=0;static uint16_t remoteStage54=0,remoteCursor54=0;
static uint32_t remoteEpoch54=0,remoteWall54=0;static int64_t remoteTick54=-1;static bool remoteBuilding54=false;
FLASHMEM bool remoteOwn54(uint16_t k){return autoRenderTrack46<0&&!remoteBuilding54&&k<64&&(remoteMask54&(1ULL<<k));}
FLASHMEM void remoteInvalidate54(){remoteUploading54=false;remoteSentCount54=65535;remoteEpoch54=0;remoteTick54=-1;remoteMask54=0;}
FLASHMEM bool remoteAddress54(uint8_t kind,uint8_t port,uint8_t ch,uint16_t number){if(autoRenderTrack46>=0)return false;for(unsigned i=0;i<(remoteUploading54?(remoteSentCount54==65535?0:remoteSentCount54):remoteRouteCount54);++i){auto&r=remoteUploading54?remoteSentRoutes54[i]:remoteRoutes54[i];if(r.enabled&&r.kind==kind&&r.port==port&&r.channel==ch&&r.number==number)return true;}return false;}
FLASHMEM bool remoteBase54(uint8_t kind,uint8_t ch,uint16_t number,int value,uint8_t mask,bool lock,bool active){
 uint8_t owned=0;for(unsigned port=0;port<8;++port)if((mask&(1u<<port))&&remoteAddress54(kind,port,ch,number))owned|=1u<<port;
 if(!owned)return false;mod54::Lock packet{{number,(int16_t)value,kind,owned,ch},(uint8_t)active};pidiPacket827(lock?mod54::LOCK:mod54::BASE,&packet,lock?sizeof(packet):sizeof(packet.base));return owned==mask;
}
FLASHMEM void remoteLock54(uint8_t kind,uint8_t ch,uint16_t number,int value){if(!lockMidiKind817(kind)||kind==LOCK_PROGRAM26)return;int top=lockTop817(kind,ch,number);remoteBase54(kind==LOCK_CC817?0:kind==LOCK_NRPN14817?3:2,ch,number,value,midiMask902(ch),true,top>=0);}
// One resolved endpoint; mask expansion happens after the whole dependency group fits.
FLASHMEM bool remoteRoute54(uint16_t key,uint8_t slot,mod54::Route&d,uint8_t&mask){
 auto original=mod808EffectiveUnit(key).outputs[slot];d={};mask=0;d.sourceSlot=slot;d.enabled=original.enabled;d.depth=original.depth;d.limit=modLimit810(original.kind);d.track=255;if(!original.enabled)return true;
 if(modIsTarget811(original)){d.kind=original.kind==MOD_PARAM811?5:6;d.target=modTarget810(original);d.slot=original.number%8;mask=1;return d.target<64;}
 if(ioVirtual53(original))return false;
 if(original.kind==MOD_CV46){if(original.track>=4||cvShift50[original.track]||debugTestMode==DEBUG_TEST_CV47)return false;d.kind=4;d.number=original.track;d.base=original.reference;mask=1;return true;}
 if(original.kind==MOD_TRACK21){auto*q=modTrackParam21(original);if(!q)return false;auto b=blockAtNote21(original.track,original.block21,255,-1,255);if(!b.port){d.enabled=0;return true;}uint16_t base=0;patchValue819(blockPatchContext21(original.track,original.block21),*q,base);d.kind=q->kind?(q->bits==14?3:2):0;d.number=q->number;d.base=base;d.channel=b.channel;d.track=original.track;mask=1u<<(b.port-1);return true;}
 auto r=modResolve810(original);if(!r.enabled){d.enabled=0;return true;}if(r.kind!=MOD_CC&&r.kind!=MOD_MIDI_BEND806)return false;
 d.kind=r.kind==MOD_CC?0:1;d.number=d.kind?0:r.number;d.channel=r.track;d.base=modReference43(original);
 if(modTrackMidi43(original)){d.track=original.track;mask=1u<<(r.port-1);}
 else if(original.kind==MOD_PATCH810){d.track=original.track;mask=outMask902(original.track);for(unsigned t=0;t<16;++t)if(t!=original.track&&linkOwner12(t,modKind931(key)?LINK_LFO12:LINK_ENV12)==original.track&&modAssociated12(key,t))return false;}
 else mask=original.port?1u<<(original.port-1):midiMask902(r.track);
 return true;
}
FLASHMEM mod54::Source remoteSource54(uint16_t k,uint8_t id,int64_t at,const ModUnit805&m){
 mod54::Source s{};s.id=id;s.begin=s.noteBegin=-1;s.active=playbackMod828(k)&&((playing&&mod806Playing(k,at))||(modKind931(k)&&modMode810(k)==2)||ccActive941(k));
 uint8_t modeEarly=modMode810(k);if((!modKind931(k)||lfoEnvMode36(m)!=2)&&(modeEarly==1||modeEarly==2||modeEarly==3||modeEarly==4)){s.begin=modeEarly==1?modPatternStart810(k,at):0;if(!modKind931(k))s.gate=max(1,(int)m.hold+(int)m.attack+(int)m.decay);return s;}
 auto offer=[&](int64_t begin,uint32_t gate){if(begin>s.noteBegin){s.noteBegin=begin;s.gate=gate;}else if(begin==s.noteBegin)s.gate=max(s.gate,gate);};
 for(unsigned t=0;t<16;++t)if(m.inputs&(1u<<t)){if(playing&&m.roll58!=2&&m.delay58!=2){auto c=mod805Source(t,at);offer(c.begin,c.gate);}if(m.roll58&&rollModAt827[t]>=0&&rollModAt827[t]<=at)offer(rollModAt827[t],rollModGate827[t]);}
 for(unsigned ch=0;ch<16;++ch)if(mod808EffectiveMeta(k).midiInputs&(1u<<ch))offer(modMidiBegin806[ch],modMidiHeld806[ch]?UINT32_MAX:modMidiGate806[ch]);
 s.begin=s.noteBegin;uint8_t mode=modMode810(k);
 if(mode==1&&playing){s.begin=modPatternStart810(k,at);if(!modKind931(k))s.gate=max(1,(int)m.hold+(int)m.attack+(int)m.decay);}
 if(mode==6){s.begin=-1;s.gate=0;for(unsigned t=0;t<16;++t)if(m.inputs&(1u<<t)){auto&w=rollWindows44[t];if(w.begin>=0&&w.begin<=at&&w.begin>s.begin){s.begin=w.begin;s.gate=max((int64_t)0,w.end-w.begin);}}}
 return s;
}
FLASHMEM FreeClock48& remoteMirror54(uint16_t key,int owner,const mod54::Params&p,uint32_t now){
 auto&clock=owner<16?indFreeClock50[owner][key]:freeClock48[key];if(!p.lfo||p.mode!=2)return clock;uint32_t sample=now-now%p.rat;float period=max(.001f,p.cycle*(p.ms?1.f:tickDurationMs()));
 if(!clock.valid){clock.phase=fmod((double)sample/period,1.);clock.at=sample;clock.valid=true;}else{uint32_t elapsed=sample-(uint32_t)clock.at;clock.phase=fmod(clock.phase+elapsed/clock.period,1.);clock.at+=elapsed;}clock.period=period;return clock;
}
FLASHMEM void remoteCopySent54(){memcpy(remoteSentConfig54,remoteConfig54,remoteNodes54*sizeof(mod54::Config));memcpy(remoteSentSources54,remoteSources54,remoteNodes54*sizeof(mod54::Source));memcpy(remoteSentRoutes54,remoteRoutes54,remoteRouteCount54*sizeof(mod54::Route));remoteSentCount54=remoteRouteCount54;remoteSentNodes54=remoteNodes54;}
FLASHMEM void remotePump54(){
 for(unsigned budget=0;budget<2&&pidiCount827<256&&pidiHealthy827;++budget){
  if(remoteStage54==0){pidiPacket827(mod54::CLEAR,nullptr,0);remoteStage54=1;remoteCursor54=0;}
  else if(remoteStage54==1){if(remoteCursor54>=remoteNodes54*2){remoteStage54=2;remoteCursor54=0;continue;}unsigned i=remoteCursor54/2;if(remoteCursor54&1)pidiPacket827(mod54::SOURCE,&remoteSources54[i],sizeof(mod54::Source));else pidiPacket827(mod54::CONFIG,&remoteConfig54[i],sizeof(mod54::Config));++remoteCursor54;}
  else if(remoteStage54==2){if(remoteCursor54>=remoteRouteCount54){remoteStage54=3;continue;}pidiPacket827(mod54::ROUTE,&remoteRoutes54[remoteCursor54],sizeof(mod54::Route));++remoteCursor54;}
  else if(remoteStage54==3){mod54::Clock c{songEngineActive?songTick:transportTickAbsolute,millis(),(uint32_t)(tickDurationMs()*1000),uint8_t(playing)};pidiPacket827(mod54::CLOCK,&c,sizeof(c));remoteStage54=4;}
  else {pidiPacket827(mod54::COMMIT,nullptr,0);remoteMask54=remotePlanMask54;modDisplayDirty810=true;if(modUi805==MOD_LIST806)mod805Menu();remoteCopySent54();pidiProject925();remoteUploading54=false;remoteEpoch54=0;
   for(unsigned i=0;i<remoteRouteCount54;++i){auto&r=remoteRoutes54[i];if(r.kind<2){auto&p=midiPending810[r.port*16+r.channel][r.kind==1?128:r.number];if(p.pending){p.pending=false;--midiCounters810.pending;}p.known=false;}else if(r.kind==2||r.kind==3){auto link=&nrpnPending810;while(*link){auto*q=*link;if(q->output==r.port&&q->channel==r.channel&&q->number==r.number){*link=q->next;extmem_free(q);}else link=&q->next;}}}return;
  }
 }
}
FLASHMEM void remoteRefresh54(uint32_t now){ // Refresh one definition at a time: Pidi can recover after an independent restart,
 // without a periodic full upload delaying musical messages.
 static uint32_t refreshAt=0;static unsigned refresh=0;
 if(now-refreshAt>=20&&pidiCount827<128){refreshAt=now;unsigned total=remoteNodes54*2+remoteRouteCount54;if(total){refresh%=total;if(!refresh)pidiPacket827(mod54::SYNC,nullptr,0);if(refresh<remoteNodes54*2){unsigned i=refresh/2;if(refresh&1)pidiPacket827(mod54::SOURCE,&remoteSources54[i],sizeof(mod54::Source));else pidiPacket827(mod54::CONFIG,&remoteConfig54[i],sizeof(mod54::Config));}else{unsigned i=refresh-remoteNodes54*2;pidiPacket827(mod54::ROUTE,&remoteRoutes54[i],sizeof(mod54::Route));}if(++refresh==total)pidiPacket827(mod54::COMMIT,nullptr,0);}}
}
FLASHMEM void remoteClock54(int64_t at,uint32_t now){static uint32_t last=0;static bool running=false;if(now-last>=20||running!=playing){mod54::Clock clock{(uint64_t)max((int64_t)0,at),now,(uint32_t)(tickDurationMs()*1000),uint8_t(playing)};pidiPacket827(mod54::CLOCK,&clock,sizeof(clock));last=now;running=playing;}}
FLASHMEM void remotePrepare54(int64_t at){
 BlockScope21 destinationScope55(blockIndex21());
 if(!remoteEnabled54){remoteMask54=0;remoteRouteCount54=0;return;}
 if(remoteBuilding54||autoRenderTrack46>=0||queryOffset50||!pidiStarted827||!pidiHealthy827)return;if(remoteUploading54){remotePump54();return;}at=songEngineActive?songTick:transportTickAbsolute;
 uint32_t now=millis();if(remoteEpoch54==modEpoch805&&remoteTick54==at&&remoteWall54==now)return;
 uint64_t previousMask=remoteMask54;IndContext50 defaultContext(-1);remoteBuilding54=true;bool unchanged=remoteEpoch54==modEpoch805&&remoteSentCount54!=65535;remoteEpoch54=modEpoch805;remoteTick54=at;remoteWall54=now;
 if(unchanged){for(unsigned id=0;id<remoteNodes54;++id){unsigned identity=remoteConfig54[id].identity,k=identity/17,owner=identity%17;IndContext50 context(owner<16?owner:-1);auto m=mod808EffectiveUnit(k);remoteMirror54(k,owner,remoteConfig54[id].p,now);auto source=remoteSource54(k,id,at,m);remoteSources54[id]=source;if(memcmp(&source,&remoteSentSources54[id],sizeof(source))&&pidiCount827<256&&pidiPacket827(mod54::SOURCE,&source,sizeof(source)))remoteSentSources54[id]=source;}if(pidiCount827<256){remoteClock54(at,now);remoteRefresh54(now);}remoteBuilding54=false;return;}

 uint8_t parent[64];bool bad[64]={};uint16_t owners[64]={};for(unsigned k=0;k<64;++k)parent[k]=k;
 auto root=[&](unsigned k){while(parent[k]!=k)k=parent[k];return k;};auto join=[&](unsigned a,unsigned b){a=root(a);b=root(b);if(a!=b)parent[b]=a;};
 // Share the entire graph and every common physical destination, never split its mean.
 struct End {uint16_t number;uint8_t key,kind,ch,mask;};static EXTMEM End ends[512];unsigned endCount=0;
 for(unsigned k=0;k<64;++k)if(mod806Unit(k).enabled){owners[k]=indModOwners50(k);bad[k]=ioKey38(k)||ccEnv941[k].enabled||manualModActive06(k)||holdValue50(k,mod806Unit(k))<0||midiAdvance808||autoCount46||autoMuteCount46||controller940;
  for(unsigned j=0;j<8;++j){mod54::Route d{};uint8_t mask=0;if(!remoteRoute54(k,j,d,mask))bad[k]=true;if(!d.enabled)continue;if(d.kind>=5){if(d.target<64){join(k,d.target);if(d.kind==5&&d.slot==0&&modKind931(d.target)&&modMode810(d.target)==2)bad[k]=true;}else bad[k]=true;continue;}if(d.track<16&&(trackCcMask933&(1u<<d.track))&&trackCc933[d.track])bad[k]=true;
   for(unsigned e=0;e<endCount;++e){auto&x=ends[e];if(x.kind==d.kind&&x.ch==d.channel&&x.number==d.number&&(x.mask&mask))join(k,x.key);}if(endCount<512)ends[endCount++]={d.number,(uint8_t)k,d.kind,d.channel,mask};
  }
 }
 for(unsigned k=0;k<64;++k)if(bad[k])bad[root(k)]=true;
 memset(remoteMap54,-1,sizeof(remoteMap54));remoteNodes54=0;remoteRouteCount54=0;remoteMask54=0;remoteFallback54=remoteCapacity54=0;
 for(unsigned group=0;group<64;++group)if(root(group)==group){bool any=false;unsigned need=0,routeNeed=0;uint16_t contexts=0;for(unsigned k=0;k<64;++k)if(mod806Unit(k).enabled&&root(k)==group){any=true;contexts|=owners[k];}
  if(!any)continue;unsigned instances=contexts?__builtin_popcount(contexts):1;
  for(unsigned k=0;k<64;++k)if(mod806Unit(k).enabled&&root(k)==group){need+=instances;for(unsigned j=0;j<8;++j){mod54::Route d{};uint8_t mask=0;remoteRoute54(k,j,d,mask);if(d.enabled)routeNeed+=instances*__builtin_popcount(mask);}}
  if(bad[group]){++remoteFallback54;continue;}if(remoteNodes54+need>64||remoteRouteCount54+routeNeed>512){++remoteCapacity54;continue;}
  unsigned first=remoteNodes54,firstRoute=remoteRouteCount54;bool success=true;
  for(unsigned owner=0;owner<17;++owner)if(contexts?(owner<16&&(contexts&(1u<<owner))):owner==16)for(unsigned k=0;k<64;++k)if(mod806Unit(k).enabled&&root(k)==group)remoteMap54[k][owner]=remoteNodes54++;
  for(unsigned owner=0;owner<17;++owner)if(contexts?(owner<16&&(contexts&(1u<<owner))):owner==16){IndContext50 context(owner<16?owner:-1);
   for(unsigned k=0;k<64;++k)if(remoteMap54[k][owner]>=0&&root(k)==group){unsigned id=remoteMap54[k][owner];auto m=mod808EffectiveUnit(k);mod54::Config c{};c.id=id;c.identity=k*17+owner;auto&p=c.p;p.cycle=max((uint32_t)1,m.rate);p.hold=m.hold;p.attack=m.attack;p.decay=m.decay;p.release=m.release;p.rat=max(1,lockValue817(LOCK_RAT817,k,0,modRat808[k]));p.interval=max((uint16_t)1,m.hold);p.phase=lockValue817(LOCK_EXTRA817,k,4,modExtra810[k].phase);p.ac=lockValue817(LOCK_EXTRA817,k,0,modExtra810[k].attackCurve);p.dc=lockValue817(LOCK_EXTRA817,k,1,modExtra810[k].decayCurve);p.rc=lockValue817(LOCK_EXTRA817,k,2,modExtra810[k].releaseCurve);p.sustain=m.sustain;p.shape=m.shape;p.lfo=modKind931(k);p.mode=modMode810(k);p.env=p.lfo?lfoEnvMode36(m):0;p.ms=m.curve==2||(m.curve==0&&p.mode==2);p.maximum=expEnvMaximum15;p.enabled=1;p.inputs58=m.inputs;p.delay58=m.delay58;auto&phase=remoteMirror54(k,owner,p,now);if(p.lfo&&p.mode==2&&phase.valid){c.seedValid=1;c.phaseSeed=phase.phase;c.wallSeed=phase.at;c.periodSeed=phase.period;}if(id<remoteSentNodes54&&remoteSentNodes54!=255&&remoteSentConfig54[id].identity==c.identity){c.seedValid=remoteSentConfig54[id].seedValid;c.phaseSeed=remoteSentConfig54[id].phaseSeed;c.wallSeed=remoteSentConfig54[id].wallSeed;c.periodSeed=remoteSentConfig54[id].periodSeed;}remoteConfig54[id]=c;remoteSources54[id]=remoteSource54(k,id,at,m);
    for(unsigned j=0;j<8;++j){mod54::Route d{};uint8_t mask=0;if(!remoteRoute54(k,j,d,mask)){success=false;break;}if(!d.enabled)continue;if(d.kind<5&&(owners[k]?!(owners[k]&(1u<<owner)):(contexts&&owner!=(unsigned)__builtin_ctz(contexts))))continue;d.source=id;if(d.kind>=5){int dest=remoteMap54[d.target][owner];if(dest<0){success=false;break;}d.target=dest;}
     for(unsigned port=0;port<8;++port)if(mask&(1u<<port)){if(remoteRouteCount54>=512){success=false;break;}d.port=port;if(d.kind==0||d.kind==2||d.kind==3){IndContext50 global(-1);MidiScope902 scope(1u<<port);uint8_t kind=d.kind==0?LOCK_CC817:d.kind==3?LOCK_NRPN14817:LOCK_NRPN7817;int top=lockTop817(kind,d.channel,d.number);d.locked=top>=0;d.lockValue=top>=0?lockLeases817[top].value:0;}d.id=remoteRouteCount54;remoteRoutes54[remoteRouteCount54++]=d;}
    }
   }
  }
  if(!success){remoteNodes54=first;remoteRouteCount54=firstRoute;++remoteFallback54;for(unsigned k=0;k<64;++k)if(root(k)==group)memset(remoteMap54[k],-1,17);}
  else for(unsigned k=0;k<64;++k)if(mod806Unit(k).enabled&&root(k)==group)remoteMask54|=1ULL<<k;
 }
 bool structural=remoteSentCount54!=remoteRouteCount54||remoteSentNodes54!=remoteNodes54;
 for(unsigned i=0;i<remoteNodes54&&!structural;++i)structural=remoteConfig54[i].identity!=remoteSentConfig54[i].identity;
 for(unsigned i=0;i<remoteRouteCount54&&!structural;++i){auto a=remoteRoutes54[i],b=remoteSentRoutes54[i];a.base=b.base=0;a.depth=b.depth=0;a.lockValue=b.lockValue=0;a.locked=b.locked=0;structural=memcmp(&a,&b,sizeof(a))!=0;}
 // Reserve a complete configuration transaction before making TEE skip this graph.
 unsigned reserve=32;for(unsigned i=0;i<remoteNodes54;++i){if(structural||memcmp(&remoteConfig54[i],&remoteSentConfig54[i],sizeof(mod54::Config)))reserve+=120;if(structural||memcmp(&remoteSources54[i],&remoteSentSources54[i],sizeof(mod54::Source)))reserve+=72;}for(unsigned i=0;i<remoteRouteCount54;++i)if(structural||memcmp(&remoteRoutes54[i],&remoteSentRoutes54[i],sizeof(mod54::Route)))reserve+=60;
 if(structural||reserve>512){remotePlanMask54=remoteMask54;remoteMask54=previousMask;remoteUploading54=true;remoteStage54=remoteCursor54=0;remoteBuilding54=false;remotePump54();return;}
 if(reserve>sizeof(pidiQueue827)-pidiCount827){remoteMask54=previousMask;remoteRouteCount54=remoteSentCount54==65535?0:remoteSentCount54;remoteNodes54=remoteSentNodes54==255?0:remoteSentNodes54;memcpy(remoteRoutes54,remoteSentRoutes54,remoteRouteCount54*sizeof(mod54::Route));remoteBuilding54=false;return;}
 if(structural)pidiPacket827(mod54::CLEAR,nullptr,0);
 mod54::Clock clock{(uint64_t)max((int64_t)0,at),now,(uint32_t)(tickDurationMs()*1000),uint8_t(playing)};static uint32_t lastClock=0;static bool lastPlaying=false;if(structural||now-lastClock>=20||lastPlaying!=playing){pidiPacket827(mod54::CLOCK,&clock,sizeof(clock));lastClock=now;lastPlaying=playing;}
 for(unsigned i=0;i<remoteNodes54;++i){if(structural||memcmp(&remoteConfig54[i],&remoteSentConfig54[i],sizeof(mod54::Config)))pidiPacket827(mod54::CONFIG,&remoteConfig54[i],sizeof(mod54::Config));if(structural||memcmp(&remoteSources54[i],&remoteSentSources54[i],sizeof(mod54::Source)))pidiPacket827(mod54::SOURCE,&remoteSources54[i],sizeof(mod54::Source));}
 for(unsigned i=0;i<remoteRouteCount54;++i)if(structural||memcmp(&remoteRoutes54[i],&remoteSentRoutes54[i],sizeof(mod54::Route)))pidiPacket827(mod54::ROUTE,&remoteRoutes54[i],sizeof(mod54::Route));
 if(structural)pidiPacket827(mod54::COMMIT,nullptr,0);
 remoteCopySent54();
 remoteRefresh54(now);
 remoteBuilding54=false;
}
