#pragma once
FLASHMEM uint16_t patternLinkId911(){for(uint16_t id=1;id<65535;++id){bool used=false;for(auto&row:patternLinks911)for(auto v:row)used|=v==id;if(!used)return id;}return 0;}
static uint8_t linkPage55=0;
FLASHMEM uint16_t linkGroup55(){return patternLinks911[selectedTrack][patternMode&&lastPatternValid&&lastPatternTrack==selectedTrack?lastPatternIndex:activePattern[selectedTrack]];}
FLASHMEM void linkRender55(){
 char a[21]="Link patterns",b[21]={},c[21]={};uint16_t group=linkGroup55();unsigned count=0;
 for(uint8_t t=0;t<16;++t)for(uint8_t p=0;p<16;++p)if(group&&patternLinks911[t][p]==group)++count;
 if(linkPage55>=(count+1)/2)linkPage55=0;unsigned at=0;
 for(uint8_t t=0;t<16;++t)for(uint8_t p=0;p<16;++p)if(group&&patternLinks911[t][p]==group){if(at/2==linkPage55){char cell[11];snprintf(cell,11,"T%02u P%02u",t+1,p+1);memcpy(b+(at%2)*10,cell,8);if(!(at%2))b[8]=b[9]=' ';}++at;}
 if(!count)strcpy(b,"No linked patterns");
 uint8_t color=0;for(uint8_t t=0;t<16;++t)for(uint8_t p=0;p<16;++p)if(group&&patternLinks911[t][p]==group)color=patternLinkColor55[t][p];
 snprintf(c,21,"OA list OB color %02u",color);sendMenuScreen(a,b,c,"Link      Unlink    ");
}
FLASHMEM void linkButton55(uint8_t i){
 if(i!=4&&i!=6)return;uint16_t mask=0;unsigned count=0;
 for(uint8_t t=0;t<16;++t)for(uint8_t p=0;p<16;++p)if(patternSelected[t][p]){if(i==4&&(mask&(1u<<t))){remixError911("ONE PATTERN PER TRACK");return;}mask|=1u<<t;++count;}
 uint16_t group=linkGroup55();
 if(i==6){for(uint8_t t=0;t<16;++t)for(uint8_t p=0;p<16;++p)if(count?patternSelected[t][p]:(group&&patternLinks911[t][p]==group))patternLinks911[t][p]=0;}
 else{if(count<2){remixError911("SELECT 2+ PATTERNS");return;}uint16_t id=patternLinkId911();if(!id)return;uint8_t color=1+(id-1)%(NEO_PALETTE_COUNT-1);for(uint8_t t=0;t<16;++t)for(uint8_t p=0;p<16;++p)if(patternSelected[t][p]){patternLinks911[t][p]=id;patternLinkColor55[t][p]=color;}}
 storageDirty931=true;neoDisplayDirty=true;linkRender55();
}
FLASHMEM void linkRoy55(uint8_t roy,int delta,bool){
 if(!delta)return;if(!roy)linkPage55=clampInt((int)linkPage55+delta,0,7);
 else if(roy==1){uint16_t group=linkGroup55();int color=-1;for(uint8_t t=0;t<16;++t)for(uint8_t p=0;p<16;++p)if(group&&patternLinks911[t][p]==group){if(color<0)color=WRAP23(patternLinkColor55[t][p],delta,0,NEO_PALETTE_COUNT-1);patternLinkColor55[t][p]=color;}storageDirty931=true;neoDisplayDirty=true;}linkRender55();
}
FLASHMEM bool patternLinkButton911(uint8_t bit){if(bit!=53)return false;patMenu46=6;linkPage55=0;linkRender55();return true;}
FLASHMEM bool songLinkedPlace911(uint8_t track,uint8_t pattern,uint32_t position){
 uint16_t group=patternLinks911[track][pattern];if(!group)return false;SongClip candidates[16];uint16_t slots[16];uint8_t count=0;
 for(uint8_t t=0;t<16;++t)for(uint8_t p=0;p<16;++p)if(patternLinks911[t][p]==group){
  if(count>=16||!songPatternPresent(t,p)||(playing&&!songCache[t][p].valid)){songError("LINK_PATTERN_NOT_READY");return true;}
  auto&c=candidates[count++];c={position,0,songPatternSteps(t,p),t,p,true};if(!songClipValid(c,songPatternSteps(t,p))){songError("INVALID_LINK_POSITION");return true;}
  for(auto&old:songClips)if(songClipsOverlap(c,old)){songError("LINK_TRACK_OVERLAP");return true;}
 }
 uint8_t free=0;for(uint16_t i=0;i<SONG_CLIP_CAPACITY&&free<count;++i)if(!songClips[i].used)slots[free++]=i;if(free<count){songError("CLIP_CAPACITY");return true;}
 for(uint8_t i=0;i<count;++i)if(!songCachePattern(candidates[i].track,candidates[i].pattern))return true;
 for(uint8_t i=0;i<count;++i){songClips[slots[i]]=candidates[i];songEmitClip(slots[i]);}++songRevision;songState();return true;
}
FLASHMEM bool remixImageNote911(PatternImage805&a,const rmx911::Model&m,const rmx911::Note&n,int tick,int32_t*identity=nullptr){
 int step=tick/96,logical=tick%96,micro=n.time;if(logical>48){micro+=logical-48;logical=48;}
 if(micro< -96||micro>96||logical+micro< -96||logical+micro>96){int at=rmx911::wrap((int64_t)tick+n.time,a.bars*1536);step=(at+48)/96;logical=at-step*96;step%=a.bars*16;micro=0;}
 if(identity)*identity=step*96+logical;
 TrackRollSettings roll{};uint8_t mask=m.value(n,rmx911::Roll,12)?(1u<<5):0;for(uint8_t f=0;f<6;++f){int value=m.value(n,rmx911::Roll,f);setRollFieldValue(roll,f,value);if(value!=rollFieldValue(trackRoll[n.track],f))mask|=1u<<f;}
 if(!a.active[step]){a.active[step]=true;a.note[step]=n.pitch;a.vel[step]=n.velocity;a.gate[step]=n.gate;a.logical[step]=logical;a.micro[step]=micro;a.lockPitch[step]=a.lockVel[step]=a.lockGate[step]=a.lockTime[step]=true;a.roll[step]=roll;a.rollMask[step]=mask;return true;}
 bool same=a.logical[step]==logical&&a.micro[step]==micro&&a.vel[step]==n.velocity&&a.gate[step]==n.gate&&a.rollMask[step]==mask;
 for(uint8_t f=0;f<6;++f)if(mask&(1u<<f))same&=rollFieldValue(a.roll[step],f)==rollFieldValue(roll,f);
 if(same&&a.extraCount[step]<3){a.extra14[step][a.extraCount[step]]={n.gate,micro,n.velocity};a.extraNote[step][a.extraCount[step]++]=n.pitch;return true;}
 for(auto&f:a.fine)if(!f.active){f={};f.active=true;f.step=step;f.logicalMicro=logical;f.micro=micro;f.note=n.pitch;f.vel=n.velocity;f.gate=n.gate;f.roll=roll;f.pxMask=PX_PITCH|PX_VEL|PX_GATE|PX_TIME|((uint16_t)mask<<PX_ROLL_SHIFT);return true;}
 return false;
}
// Writes are staged before publishing. No transport reader runs during commit;
// clock interrupts continue to enqueue dates. Service playback between tracks.
FLASHMEM bool remixInstall911(const rmx911::Model&m,bool current,uint8_t row,uint8_t pre,uint8_t copies,uint8_t post,bool overwrite,bool dryRun,bool rangeOnly42){
 uint16_t mask=m.mask;if(!mask){remixError911("NO EXPORT TRACK");return false;}
 unsigned span=rmx916::cycle(pre,copies,remixBars911,post);
 if(!copies||row>=16||!span||span>64){remixError911("MAX 64 BARS");return false;}
 if(!current&&!overwrite)for(uint8_t t=0;t<16;++t)if((mask&(1u<<t))&&songPatternPresent(t,row)){remixError911("DESTINATION OCCUPIED");return false;}
 struct Stage {
  PackedPattern906*patterns[16]={};bool reserved[16][64]={};rmx911::List<NoteLock817> locks;bool committed=false;
  PatternImage805*image=(PatternImage805*)extmem_malloc(sizeof(PatternImage805));
  PatternImage805*original=(PatternImage805*)extmem_malloc(sizeof(PatternImage805));
  ~Stage(){extmem_free(image);extmem_free(original);for(auto*p:patterns)if(p)extmem_free(p);if(!committed)for(uint8_t t=0;t<16;++t)for(uint8_t p=0;p<64;++p)if(reserved[t][p]){extmem_free(rollPages906[t][p]);rollPages906[t][p]=nullptr;rollBytes906-=sizeof(RollPage906);}}
 } stage;
 if(!stage.image||!stage.original){remixError911("PATTERN MEMORY FULL");return false;}
 auto addLock=[&](const NoteLock817&l){if(stage.locks.size>=LOCK_CAPACITY817||!stage.locks.add(l)){remixError911("P.LOCK CAPACITY");return false;}return true;};
 // Preserve locks outside every destination pattern.
 for(uint16_t i=0;i<lockCount817;++i){const auto&l=noteLocks817[i];if(!(mask&(1u<<l.track))||l.pattern!=(current?activePattern[l.track]:row))if(!addLock(l))return false;}
 for(uint8_t t=0;t<16;++t){if(!(mask&(1u<<t)))continue;
  uint8_t p=current?activePattern[t]:row;auto&a=*stage.image;auto&original=*stage.original;
  memset(&a,0,sizeof(a));
  if(current){pattern805CaptureLive(t,original);a.mov[0]=original.mov[0];a.bars=rangeOnly42?max((unsigned)original.bars,span):rmx916::length(original.bars,span);}else a.bars=span;
  if(a.bars>64){remixError911("MAX 64 BARS");return false;}
  uint16_t fineCount=0;
  for(unsigned bar=0;bar<a.bars;++bar){int target=rangeOnly42?(bar>=pre&&bar<pre+remixBars911?int(bar-pre):-1):rmx916::target(bar,pre,copies,remixBars911,post);
   if(target<0){
    if(!current||(rangeOnly42&&bar>=original.bars))continue;
    unsigned sourceBar=bar%original.bars;
    for(unsigned i=0;i<16;++i){unsigned dst=bar*16+i,src=sourceBar*16+i;
     #define COPY916(field) memcpy(&a.field[dst],&original.field[src],sizeof(a.field[dst]));
     COPY916(active) COPY916(vel) COPY916(note) COPY916(gate) COPY916(micro)
     COPY916(lockVel) COPY916(lockPitch) COPY916(lockGate) COPY916(lockTime)
     COPY916(roll) COPY916(rollMask) COPY916(extraCount) COPY916(extraNote) COPY916(extra14) COPY916(logical)
     #undef COPY916
    }
    for(const auto&f:original.fine)if(f.active&&f.step/16==sourceBar){
     if(fineCount>=128){remixError911("PATTERN NOTE CAPACITY");return false;}
     a.fine[fineCount]=f;a.fine[fineCount++].step=bar*16+f.step%16;
    }
    for(uint16_t i=0;i<lockCount817;++i){auto l=noteLocks817[i];if(l.track!=t||l.pattern!=p)continue;
     // Match the owning step (including negative micro-position at bar edges).
     bool belongs=false;
     for(unsigned st=sourceBar*16;st<(sourceBar+1)*16&&!belongs;++st)if(original.active[st]&&(int32_t)(st*96+original.logical[st])==l.tick){
      belongs=original.note[st]==l.note;for(uint8_t voice=0;voice<original.extraCount[st]&&!belongs;++voice)belongs=original.extraNote[st][voice]==l.note;
     }
     for(const auto&f:original.fine)if(f.active&&f.step/16==sourceBar&&(int32_t)(f.step*96+f.logicalMicro)==l.tick&&f.note==l.note)belongs=true;
     if(!belongs)continue;
     l.tick+=(int(bar)-int(sourceBar))*1536;if(!addLock(l))return false;
    }
    continue;
   }
   for(uint32_t i=0;i<m.notes.size;++i){const auto&n=m.notes[i];if(n.track!=t||n.tick/1536!=target)continue;
    int tick=bar*1536+n.tick%1536;int32_t identity=0;
    if(!remixImageNote911(a,m,n,tick,&identity)){remixError911("PATTERN NOTE CAPACITY");return false;}
    for(uint8_t f=0;f<12&&m.value(n,rmx911::Delay,12)<1024;++f){int value=m.value(n,rmx911::Delay,f);int base=f==2&&!delays805[t][p].enabled?0:delayBase907(t,p,f);if(value==base)continue;
     NoteLock817 lock{identity,value,(uint16_t)(t*16+p),f,t,p,n.pitch,LOCK_DELAY817};if(!addLock(lock))return false;}
    auto rollBase=rollExtra1000(t);for(uint8_t f=0;f<8;++f){int value=m.value(n,rmx911::Roll,f>=6?7+f:6+f);if(value==rollBase.v[f])continue;NoteLock817 lock{identity,value,t,f,t,p,n.pitch,LOCK_ROLL1000};if(!addLock(lock))return false;}
    for(uint32_t j=0;j<m.properties.size;++j){const auto&v=m.properties[j];if(v.id!=n.id)continue;
     if(v.kind==rmx911::Delay&&v.parameter>=100&&v.parameter<228){uint16_t slot=(v.parameter-100)/32,f=(v.parameter-100)%32;NoteLock817 lock{identity,v.value-4096,(uint16_t)(slot*256+t*16+p),f,t,p,n.pitch,LOCK_DELAY817};if(!addLock(lock))return false;continue;}
     if(v.kind==rmx911::Delay&&v.parameter>=13&&v.parameter<100){uint16_t slot=1+(v.parameter-13)/12,f=(v.parameter-13)%12;NoteLock817 lock{identity,v.value-4096,(uint16_t)(slot*256+t*16+p),f,t,p,n.pitch,LOCK_DELAY817};if(!addLock(lock))return false;continue;}
     if(v.kind==rmx911::Roll&&v.parameter>=6)continue;
     if(v.kind<rmx911::CC)continue;
     if(v.kind==rmx911::Block21){NoteLock817 lock{identity,v.value,t,v.parameter,t,p,n.pitch,LOCK_BLOCK21};if(!addLock(lock))return false;continue;}
     uint8_t kind=v.kind==rmx911::CC?LOCK_CC817:v.kind==rmx911::NRPN7?LOCK_NRPN7817:LOCK_NRPN14817;
     NoteLock817 lock{identity,v.value,t,v.parameter,t,p,n.pitch,kind};if(!addLock(lock))return false;}
   }
   // RMX can have populated fine slots before later preserved bars.
   fineCount=0;for(unsigned f=0;f<128;++f)if(a.fine[f].active)fineCount=f+1;
  }
  for(auto&f:a.fine)if(f.active&&!a.active[f.step]){auto step=f.step;a.active[step]=true;a.note[step]=f.note;a.vel[step]=f.vel;a.gate[step]=f.gate;a.micro[step]=f.micro;a.logical[step]=f.logicalMicro;a.lockPitch[step]=f.pxMask&PX_PITCH;a.lockVel[step]=f.pxMask&PX_VEL;a.lockGate[step]=f.pxMask&PX_GATE;a.lockTime[step]=f.pxMask&PX_TIME;a.roll[step]=f.roll;a.rollMask[step]=f.pxMask>>PX_ROLL_SHIFT;a.extraCount[step]=0;f.active=false;}
  stage.patterns[t]=packPattern906(a);if(!stage.patterns[t]){remixError911("PATTERN MEMORY FULL");return false;}
  if(p==activePattern[t])for(uint8_t page=0;page<64;++page){bool required=false;for(uint8_t st=0;st<16;++st)required|=a.rollMask[page*16+st]!=0;if(required&&!rollPages906[t][page]){if(!rollReserve906(t,page)){remixError911("ROLL MEMORY FULL");return false;}stage.reserved[t][page]=true;}}
  serviceRealtimeBetweenNeoPanels();pidiService827();
 }
 if(dryRun)return true;
 uint16_t group=remixLink911&&!current?patternLinkId911():0;
 if(remixLink911&&!current&&!group){remixError911("LINK CAPACITY");return false;}
 // No updateTransport call here: readers see all old or all new patterns.
 for(uint8_t t=0;t<16;++t){if(!stage.patterns[t])continue;uint8_t p=current?activePattern[t]:row;auto*v=stage.patterns[t];installPattern906(t,p,v);stage.patterns[t]=nullptr;patternDirty805[t][p]=true;
  unpackPattern906(v,*stage.image);auto&a=*stage.image;patternBank[t][p]={true,a.bars,false};for(auto active:a.active)patternBank[t][p].hasNotes|=active;
  if(p==activePattern[t]){pattern805ApplyLive(t,a);trackLoopStartStep[t]=1;trackLoopEndStep[t]=trackBars[t]*16;}
  if(!current){patternGroove[t][p]=m.tracks[t].groove;for(uint8_t f=0;f<GROOVE_LIST_SIZE;++f)grooveLists[t][p][f]={m.tracks[t].sheets[f],(uint8_t)m.tracks[t].repeats[f]};patternLinks911[t][p]=group;}
  songCache[t][p].valid=false;
  if(!delays805[t][p].enabled){delays805[t][p].repeats=0;for(uint32_t i=0;i<m.notes.size;++i)if(m.notes[i].track==t&&m.value(m.notes[i],rmx911::Delay,2)>0){delays805[t][p].enabled=1;break;}}
 }
 loopEndStep=longestTrackStepCount();sequenceBars=loopEndStep/16;
 stage.committed=true;lockCount817=stage.locks.size;if(lockCount817)memcpy(noteLocks817,stage.locks.data,lockCount817*sizeof(NoteLock817));
 invalidateStepTriggers();mod805Invalidate();++songRevision;songSnapshot();remixError911("REMIX WRITTEN");return true;
}
struct PLinkHeader911 {uint32_t magic=0x314B4C50,version=4,notes=0,properties=0,nextId=1,hash=0;uint16_t mask=0,bars=1;uint8_t linked=0,reserved[3]={};};
FLASHMEM uint32_t plinkHash911(PLinkHeader911 h,const rmx911::Model&m){h.hash=0;uint32_t hash=songHash(2166136261u,&h,sizeof(h));hash=songHash(hash,m.tracks,sizeof(m.tracks));hash=songHash(hash,m.notes.data,m.notes.size*sizeof(rmx911::Note));return songHash(hash,m.properties.data,m.properties.size*sizeof(rmx911::Property));}
FLASHMEM bool plinkRead911(const char*path,rmx911::Model&m,PLinkHeader911&h){File f=SD.open(path,FILE_READ);if(!f)return false;
 bool ok=f.read((uint8_t*)&h,sizeof(h))==sizeof(h)&&h.magic==0x314B4C50&&h.version==4&&h.notes<=65536&&h.properties<=65536&&h.bars>=1&&h.bars<=64&&h.linked<2&&h.nextId>0;
 if(ok)ok=f.size()==sizeof(h)+sizeof(m.tracks)+h.notes*sizeof(rmx911::Note)+h.properties*sizeof(rmx911::Property)&&m.notes.reserve(h.notes)&&m.properties.reserve(h.properties);
 if(ok)ok=f.read((uint8_t*)m.tracks,sizeof(m.tracks))==sizeof(m.tracks)&&f.read((uint8_t*)m.notes.data,h.notes*sizeof(rmx911::Note))==h.notes*sizeof(rmx911::Note)&&f.read((uint8_t*)m.properties.data,h.properties*sizeof(rmx911::Property))==h.properties*sizeof(rmx911::Property);f.close();if(!ok)return false;
 m.notes.size=h.notes;m.properties.size=h.properties;m.mask=h.mask;m.nextId=h.nextId;if(h.hash!=plinkHash911(h,m))return false;
 for(uint8_t t=0;t<16;++t){auto&b=m.tracks[t];if(b.bars!=h.bars||b.groove>GROOVE_USE_LIST)return false;for(uint8_t i=0;i<GROOVE_LIST_SIZE;++i)if(b.sheets[i]>GROOVE_COUNT||b.repeats[i]>255)return false;for(uint8_t i=0;i<6;++i)if(b.roll[i]!=clampRollFieldValue(i,b.roll[i]))return false;for(uint8_t i=0;i<9;++i)if(b.delay[i]<delayMin907(i)||b.delay[i]>(i==8?2:delayMax907(i)))return false;}
 for(uint32_t i=0;i<m.notes.size;++i){auto&n=m.notes[i];if(!n.id||n.id>=m.nextId||n.track>=16||!(m.mask&(1u<<n.track))||n.pitch>127||!n.velocity||n.velocity>127||n.gate<GATE_TICK_MIN||n.gate>GATE_TICK_MAX||n.tick<0||n.tick>=h.bars*1536||n.time< -96||n.time>96)return false;for(uint32_t j=0;j<i;++j)if(m.notes[j].id==n.id)return false;}
 for(uint32_t i=0;i<m.properties.size;++i){auto&p=m.properties[i];bool found=false;for(uint32_t j=0;j<m.notes.size;++j)found|=p.id==m.notes[j].id;if(!found||p.kind>rmx911::Block21)return false;
  if(p.kind==rmx911::Block21){NoteLock817 l{};l.track=0;l.pattern=0;l.note=60;l.target=0;l.kind=LOCK_BLOCK21;l.parameter=p.parameter;l.value=p.value;if(!lockValid817(l))return false;continue;}
  if(p.kind==rmx911::Roll){if(p.parameter==12){if(p.value!=1)return false;continue;}if(p.parameter==13){if(p.value<0||p.value>1535)return false;continue;}if(p.parameter==14){if(p.value<0||p.value>2)return false;continue;}if(p.parameter>14)return false;if(p.parameter<6){if(p.value!=clampRollFieldValue(p.parameter,p.value))return false;}else if(p.value<rollMin1000(p.parameter-6)||p.value>rollMax1000(p.parameter-6))return false;}
  if(p.kind==rmx911::Delay&&p.parameter>=13&&p.parameter!=300){uint16_t f=p.parameter>=100?(p.parameter-100)%32:(p.parameter-13)%12;int value=p.value-4096;if((p.parameter>227||(p.parameter>96&&p.parameter<100))||value<delayMin907(f)||value>(f==8?2:delayMax907(f)))return false;}
  if(p.kind==rmx911::Delay&&p.parameter<13&&((p.parameter==12?(p.value<0||(p.value>256&&(p.value<1024||p.value>1039))):(p.value<delayMin907(p.parameter)||p.value>(p.parameter==8?2:delayMax907(p.parameter))))))return false;
  if(p.kind==rmx911::Delay&&p.parameter==300&&(p.value<0||p.value>1))return false;
  if(p.kind>=rmx911::CC&&p.kind<=rmx911::NRPN14&&(p.parameter>(p.kind==rmx911::CC?127:16383)||p.value<0||p.value>(p.kind==rmx911::NRPN14?16383:127)))return false;
 }return true;
}
FLASHMEM void remixBrowserOpen911(bool save){if(!sdAvailable||(save&&!storageWriteAvailable)){remixError911("SD UNAVAILABLE");return;}remixAudition911.active=false;remixBrowser911=true;remixLoading911=false;fileBrowserMode=save?FILE_BROWSER_SAVE:FILE_BROWSER_LOAD;activeStorageType=STORAGE_PATTERN;saveMenuActive=loadTypeMenuActive=patternLoadConfirmActive=false;pendingSaveClipboard=false;
 copyField(fileBrowserPath,sizeof(fileBrowserPath),"/URBANKAOZ/PLink");copyField(fileBrowserRootPath,sizeof(fileBrowserRootPath),fileBrowserPath);copyField(fileBrowserExtension,sizeof(fileBrowserExtension),"PLK");copyField(fileBrowserName,sizeof(fileBrowserName),"REMIX");
 if(save)SD.mkdir(fileBrowserPath);storageNameCursor=0;fileBrowserEntryIndex=0;fileBrowserDirectoryPage=0;selectBrowserFile(0);renderFileBrowser();storageSync(true);
}
FLASHMEM void remixSave911(){rmx911::Model m;if(!rmx911::export912(m,remixDraft911,remixTargetMask911)||!m.mask){char path[80]={};finishSave905(false,path,sizeof(path));remixError911("NO TRACK OR MEMORY");return;}PLinkHeader911 h;h.notes=m.notes.size;h.properties=m.properties.size;h.nextId=m.nextId;h.mask=m.mask;h.bars=remixBars911;h.linked=remixLink911;h.hash=plinkHash911(h,m);
 File f=SD.open(saveWork905,FILE_WRITE);bool ok=f&&f.write((uint8_t*)&h,sizeof(h))==sizeof(h)&&f.write((uint8_t*)m.tracks,sizeof(m.tracks))==sizeof(m.tracks)&&f.write((uint8_t*)m.notes.data,m.notes.size*sizeof(rmx911::Note))==m.notes.size*sizeof(rmx911::Note)&&f.write((uint8_t*)m.properties.data,m.properties.size*sizeof(rmx911::Property))==m.properties.size*sizeof(rmx911::Property);if(f){f.flush();if(f.getWriteError())ok=false;f.close();}
 rmx911::Model verify;PLinkHeader911 vh;if(ok)ok=plinkRead911(saveWork905,verify,vh);char path[80]={};ok=finishSave905(ok,path,sizeof(path));remixBrowser911=false;closeFileBrowser();remixRefresh911();sendSaveResult(path,ok);
}
FLASHMEM bool remixLoadChoice911(){
 if(!remixBrowser911&&!remixLoading911)return false;
 if(!remixLoading911){if(!selectedBrowserFilePath(remixLoadPath911,sizeof(remixLoadPath911)))return true;remixBrowser911=false;fileBrowserMode=FILE_BROWSER_NONE;remixLoading911=remixWriteView911=true;remixWriteCurrent911=false;remixLoadSource42=false;remixLoadBar42=0;remixConfirm911=false;remixDisplay911(true);return true;}
 rmx911::Model loaded;PLinkHeader911 h;if(!plinkRead911(remixLoadPath911,loaded,h)){remixError911("INVALID PLINK FILE");return true;}
 if(remixLoadSource42&&remixLoadBar42+h.bars>64){remixError911("MAX 64 BARS");return true;}
 if(remixLoadSource42&&!remixConfirm911){remixConfirm911=true;remixDisplay911(true);mod806LiveFrame("SOURCE REPLACE","Sur duree du Remix","","Remplacer       Back");return true;}
 uint16_t oldBars=remixBars911;bool oldLink=remixLink911;remixBars911=h.bars;remixLink911=h.linked;
 bool ok=false;
 if(remixLoadSource42){ok=remixInstall911(loaded,true,0,remixLoadBar42,1,0,true,false,true);remixBars911=oldBars;remixLink911=oldLink;if(ok)remixReady911=false;}
 else {remixAudition911.active=false;remixFinishSelection911(false);remixFinishTake913();remixFreeEdit911();remixHistory911.clear();remixDraft911.swap(loaded);remixDraftInitialized915=true;remixTargetMask911=h.mask;remixTargetBar911=0;remixSelection911.clear();remixRecorded913.clear();remixScheduleDirty911=true;ok=true;}
 if(ok){remixWriteView911=remixLoading911=remixConfirm911=false;remixMode=true;remixEnter911();showGameScreen();remixRefresh911();}return true;
}
FLASHMEM bool songExportNotes911(const rmx911::Model&m,uint8_t row,uint16_t bars){uint16_t previousBars=remixBars911;bool previousLink=remixLink911;remixBars911=bars;remixLink911=false;bool ok=remixInstall911(m,false,row,0,1,0,false);remixBars911=previousBars;remixLink911=previousLink;return ok;}
