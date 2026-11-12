#pragma once
template<class T> T* patch808Allocate(){auto p=(T*)extmem_malloc(sizeof(T));if(p)memset(p,0,sizeof(T));return p;}
FLASHMEM void patch808FreeValues(PatchValue808*&v){while(v){auto n=v->next;extmem_free(v);v=n;}}
FLASHMEM void patch808Free(PatchDb808&db){
  while(db.defs){auto d=db.defs;db.defs=d->next;while(d->params){auto q=d->params;d->params=q->next;extmem_free(q);}extmem_free(d);}
  for(auto &track:db.context)for(auto &c:track){patch808FreeValues(c.values);c={0,-1,nullptr};}
  db.nextPatch=db.nextParam=1;
}
FLASHMEM void patch808ClearClipboard(){for(uint8_t s=0;s<4;++s)for(uint8_t i=0;i<16;++i){patch808FreeValues(patchClipboard808[s][i].values);patchClipboard808[s][i]={0,-1,nullptr};patchClipboardValid808[s][i]=false;}}
FLASHMEM void patch808Init(){static bool initialized=false;if(!initialized){memset(&patchDb808,0,sizeof(patchDb808));memset(&patchStage808,0,sizeof(patchStage808));memset(patchClipboard808,0,sizeof(patchClipboard808));initialized=true;}patch808ClearClipboard();midiAdvance808=0;patch808Free(patchDb808);patch808Free(patchStage808);for(auto&r:modRat808)r=8;patchUi808=0;patchBank808=false;patchPage808=0;}
// Correct only the exact erroneous factory signature; retain IDs, CCs and user values.
FLASHMEM void patchPlastic937(PatchDef808*d){
 if(strcmp(d->name,"Analog Rytm Bd Plastic")&&strcmp(d->name,"ARBDPLAS"))return;
 const char*old[]={"LEV","TUN","DEC","SWD","SWT","HLD","CLK","DST"};
 auto*q=d->params;for(uint8_t i=0;i<8;++i){if(!q||q->kind||q->bits!=7||q->number!=16+i||strcmp(q->shortName,old[i]))return;q=q->next;}if(q)return;
 const char*names[]={"Type","Mod","Tick"},*shorts[]={"TYP","MOD","TIC"};q=d->params;for(uint8_t i=0;i<8;++i,q=q->next)if(i>=5){strcpy(q->name,names[i-5]);strcpy(q->shortName,shorts[i-5]);}
}
FLASHMEM PatchDef808* patch808Find(PatchDb808&db,uint32_t id){for(auto d=db.defs;d;d=d->next)if(d->id==id){patchPlastic937(d);return d;}return nullptr;}
FLASHMEM PatchDef808* patch808At(uint32_t index){auto d=patchDb808.defs;while(d&&index--)d=d->next;return d;}
FLASHMEM uint32_t patch808Count(){uint32_t n=0;for(auto d=patchDb808.defs;d;d=d->next)++n;return n;}
FLASHMEM PatchParam808* patch808ParamAt(PatchDef808*d,uint32_t index){auto q=d?d->params:nullptr;while(q&&index--)q=q->next;return q;}
FLASHMEM uint32_t patch808ParamCount(PatchDef808*d){uint32_t n=0;for(auto q=d?d->params:nullptr;q;q=q->next)++n;return n;}
FLASHMEM PatchParam808* patchEditableAt44(PatchDef808*d,uint32_t at){for(auto*q=d?d->params:nullptr;q;q=q->next)if(q->kind!=2){if(!at--)return q;}return nullptr;}
FLASHMEM uint32_t patchEditableCount44(PatchDef808*d){uint32_t n=0;for(auto*q=d?d->params:nullptr;q;q=q->next)if(q->kind!=2)++n;return n;}
FLASHMEM PatchDef808* patch808New(){
  if(patchDb808.nextPatch==UINT32_MAX)return nullptr;
  auto d=patch808Allocate<PatchDef808>();if(!d)return nullptr;
  d->id=patchDb808.nextPatch++;snprintf(d->name,sizeof(d->name),"Patch%lu",(unsigned long)d->id);
  auto tail=&patchDb808.defs;while(*tail)tail=&(*tail)->next;*tail=d;return d;
}
FLASHMEM PatchParam808* patch808Add(PatchDef808*d){
  if(!d||patchDb808.nextParam==UINT32_MAX)return nullptr;
  auto q=patch808Allocate<PatchParam808>();if(!q)return nullptr;
  q->id=patchDb808.nextParam++;q->bits=7;strcpy(q->name,"Param");strcpy(q->shortName,"PAR");
  auto tail=&d->params;while(*tail)tail=&(*tail)->next;*tail=q;return q;
}
FLASHMEM PatchContext808& patch808Context(uint8_t t,uint8_t p){t=linkOwner12(t,LINK_PATCH12);
  auto &own=patchDb808.context[t][patchContextIndex21(p+1)];return (own.patch||own.explicit819)?own:patchDb808.context[t][patchContextIndex21(0)];
}
FLASHMEM bool patch808Set(PatchContext808&c,uint32_t id,uint16_t value){
  for(auto v=c.values;v;v=v->next)if(v->parameter==id){v->value=value;return true;}
  auto v=patch808Allocate<PatchValue808>();if(!v)return false;v->parameter=id;v->value=value;v->next=c.values;c.values=v;return true;
}
FLASHMEM bool patch808Get(const PatchContext808&c,uint32_t id,uint16_t &value){for(auto v=c.values;v;v=v->next)if(v->parameter==id){value=v->value;return true;}return false;}
FLASHMEM bool patch808Clone(const PatchContext808&src,PatchContext808&dst){
  if(&src==&dst)return true;PatchContext808 copy=src;copy.values=nullptr;
  for(auto v=src.values;v;v=v->next)if(!patch808Set(copy,v->parameter,v->value)){patch808FreeValues(copy.values);return false;}
  patch808FreeValues(dst.values);dst=copy;return true;
}

FLASHMEM uint32_t patchId819(const PatchContext808&c,uint8_t slot){return slot?c.extra[slot-1]:c.patch;}
FLASHMEM bool patchHas819(const PatchContext808&c,uint32_t id){if(!id)return !c.patch;for(uint8_t i=0;i<16;++i)if(patchId819(c,i)==id)return true;return false;}
FLASHMEM PatchParam808* patchNextDb819(PatchDb808&db,const PatchContext808&c,PatchParam808*q){
 if(q&&q->next)return q->next;
 bool ready=!q;
 for(uint8_t i=0;i<16;++i){auto*d=patch808Find(db,patchId819(c,i));if(!d)continue;
  if(ready&&d->params)return d->params;
  for(auto*x=d->params;x;x=x->next)if(x==q){ready=true;break;}
 }return nullptr;
}
FLASHMEM PatchParam808* patchNext819(const PatchContext808&c,PatchParam808*q){do{q=patchNextDb819(patchDb808,c,q);}while(q&&q->kind==2);return q;}
FLASHMEM PatchParam808* patchParam819(const PatchContext808&c,uint32_t index){auto*q=patchNext819(c,nullptr);while(q&&index--)q=patchNext819(c,q);return q;}
FLASHMEM uint32_t patchCount819(const PatchContext808&c){uint32_t n=0;for(auto*q=patchNext819(c,nullptr);q;q=patchNext819(c,q))++n;return n;}
FLASHMEM bool patchSameAddress819(const PatchParam808&a,const PatchParam808&b){return a.kind==b.kind&&a.number==b.number&&a.bits==b.bits;}
// Alphabetic priority uses patch names, then full paths as a stable tie-break.
FLASHMEM bool patchBefore10(const PatchDef808&a,const PatchDef808&b){int c=strcasecmp(a.name,b.name);return c<0||(!c&&strcasecmp(a.source819,b.source819)<0);}
FLASHMEM PatchParam808* patchAddress10(const PatchContext808&c,uint8_t kind,uint16_t number,uint8_t bits){
 PatchDef808*owner=nullptr;PatchParam808*best=nullptr;
 for(uint8_t slot=0;slot<16;++slot){auto*d=patch808Find(patchDb808,patchId819(c,slot));
  for(auto*q=d?d->params:nullptr;q;q=q->next)if(q->kind==kind&&q->number==number&&q->bits==bits&&strcmp(q->name,"MIDI")){
   if(!owner||patchBefore10(*d,*owner)){owner=d;best=q;}break;
  }
 }return best;
}
FLASHMEM const char* patchLabel10(const PatchContext808&c,const PatchParam808&q,bool shortName){auto*p=patchAddress10(c,q.kind,q.number,q.bits);if(!p)p=&const_cast<PatchParam808&>(q);return shortName?p->shortName:p->name;}
FLASHMEM bool patchRawValue50(const PatchContext808&c,const PatchParam808&q,uint16_t&value){
 int owner51=indPatchOwner50(c);if(owner51>=0){unsigned index51=((uintptr_t)&c-(uintptr_t)&patchDb808.context[owner51][0])/sizeof(PatchContext808);auto&base51=patchDb808.context[owner51][(index51/17)*17];if(&base51!=&c&&patchRawValue50(base51,q,value))return true;}
 auto*first=patchAddress10(c,q.kind,q.number,q.bits);if(first&&patch808Get(c,first->id,value))return true;
 if(patch808Get(c,q.id,value))return true;
 for(auto*x=patchNext819(c,nullptr);x;x=patchNext819(c,x))if(patchSameAddress819(*x,q)&&patch808Get(c,x->id,value))return true;return false;
}
FLASHMEM bool patchValue819(const PatchContext808&c,const PatchParam808&q,uint16_t&value){bool ok=patchRawValue50(c,q,value);if(ok)value=indPatchValue50(c,q,value);if(value==65535){value=0;return false;}return ok;}
FLASHMEM bool patchSetDefault59(PatchContext808&c,const PatchParam808&q,uint16_t value){
 int owner55=indPatchOwner50(c);
 if(owner55>=0){unsigned index55=(&c-&patchDb808.context[owner55][0]);auto&base55=patchDb808.context[owner55][(index55/17)*17];
  if(&base55!=&c){bool ok55=true,matched55=false;for(auto*x=patchNext819(base55,nullptr);x;x=patchNext819(base55,x))if(patchSameAddress819(*x,q)){matched55=true;ok55=patch808Set(base55,x->id,value)&&ok55;}if(matched55)return ok55;}
 }
 for(auto*x=patchNext819(c,nullptr);x;x=patchNext819(c,x))if(patchSameAddress819(*x,q)&&!patch808Set(c,x->id,value))return false;return true;
}
FLASHMEM bool patchSetValue819(PatchContext808&c,const PatchParam808&q,uint16_t value){if(indPatchEdit50(c,q,value))return true;return patchSetDefault59(c,q,value);}
FLASHMEM void patchRemoveValues819(PatchContext808&c,PatchDef808*d){
 auto**v=&c.values;while(*v){bool remove=false;for(auto*q=d?d->params:nullptr;q;q=q->next)if(q->id==(*v)->parameter)remove=true;
  if(remove){auto*old=*v;*v=old->next;extmem_free(old);}else v=&(*v)->next;
 }
}
FLASHMEM bool patchAttach819(PatchContext808&c,uint32_t id,bool enabled){
 c.explicit819=true;
 if(!id){patch808FreeValues(c.values);c.patch=0;memset(c.extra,0,sizeof(c.extra));return true;}
 if(patchHas819(c,id)==enabled)return true;
 if(enabled){if(!c.patch)c.patch=id;else {uint8_t i=0;while(i<15&&c.extra[i])++i;if(i==15)return false;c.extra[i]=id;}
  auto*d=patch808Find(patchDb808,id);for(auto*q=d?d->params:nullptr;q;q=q->next){uint16_t v;if(patchValue819(c,*q,v)&&!patch808Set(c,q->id,v))return false;}
 }else {
  patchRemoveValues819(c,patch808Find(patchDb808,id));uint32_t ids[16]={};uint8_t n=0;for(uint8_t i=0;i<16;++i){auto v=patchId819(c,i);if(v&&v!=id)ids[n++]=v;}c.patch=ids[0];memcpy(c.extra,ids+1,sizeof(c.extra));
 }return true;
}
FLASHMEM void patch808Copy(uint8_t ft,uint8_t fp,uint8_t tt,uint8_t tp){
  if(!patch808Clone(patch808Context(ft,fp),patchDb808.context[tt][patchContextIndex21(tp+1)]))DISPLAY_SERIAL.println("MSG|PATCH MEMORY FULL");
}
FLASHMEM bool patch808CaptureClipboard(uint8_t s,uint8_t i,uint8_t t,uint8_t p){
  if(s>=4||i>=16)return false;
  patchClipboardValid808[s][i]=patch808Clone(patch808Context(t,p),patchClipboard808[s][i]);
  return patchClipboardValid808[s][i];
}
FLASHMEM bool patch808PasteClipboard(uint8_t s,uint8_t i,uint8_t t,uint8_t p){
  if(s>=4||i>=16||!patchClipboardValid808[s][i])return true;
  return patch808Clone(patchClipboard808[s][i],patchDb808.context[t][patchContextIndex21(p+1)]);
}
FLASHMEM uint8_t patch808Channel(uint8_t t,const PatchContext808&c){return blocks21[t][patchBlockIndex21()].channel;}
FLASHMEM uint8_t patch808PlayingPattern(uint8_t t){
  if(songEngineActive)for(const auto&clip:songClips)if(clip.used&&clip.track==t&&songTick>=(uint64_t)clip.position*96&&songTick<(uint64_t)(clip.position+clip.length)*96)return clip.pattern;
  return activePattern[t];
}
FLASHMEM bool patch808Base(uint8_t channel,uint8_t cc,int &value){
  for(uint8_t t=0;t<16;++t){
    if(midiTrack940<16&&t!=midiTrack940)continue;
    const auto&c=patch808Context(t,patch808PlayingPattern(t));if(!c.patch||patch808Channel(t,c)!=channel)continue;
    for(auto q=patchNext819(c,nullptr);q;q=patchNext819(c,q))if(q->kind==0&&q->number==cc){uint16_t v;if(patchValue819(c,*q,v)){value=min((int)v,127);return true;}}
  }return false;
}
FLASHMEM void patch808SendParam(uint8_t channel,const PatchParam808&q,uint16_t value){
 if(value==65535)return;
  if(!q.kind)midiOffer810(channel,q.number,min((uint16_t)127,value));
  else if(q.kind==1)midiNrpn810(channel,q.number,q.bits,value);
}
FLASHMEM void patchChanged15(uint8_t edited,PatchContext808&context,const PatchParam808&q,uint16_t value){
 BlockScope21 block(patchBlockIndex21());uint8_t owner=linkOwner12(edited,LINK_PATCH12);
 for(uint8_t t=0;t<16;++t)if(linkOwner12(t,LINK_PATCH12)==owner){uint8_t p=patch808PlayingPattern(t);auto&playingContext=patch808Context(t,p);if(&playingContext!=&context&&&context!=&patchDb808.context[owner][patchContextIndex21(0)])continue;if(!patchAddress10(playingContext,q.kind,q.number,q.bits))continue;MidiScope902 route(outMask902(t),t);uint8_t kind=q.kind?(q.bits==14?LOCK_NRPN14817:LOCK_NRPN7817):LOCK_CC817;patch808SendParam(patch808Channel(t,playingContext),q,indBase50(t,p,kind,t,q.number,value));}
 mod805Invalidate();
}
static bool patchProgramsSent51=false;
FLASHMEM void patch808PatternAt(uint8_t t,uint8_t pattern){
  if(!patchProgramsSent51)programSend939(t,pattern); // Program first; subsequent Patch values customize it.
  modRecall937(t);
  modPatternEvent810(t,pattern,songEngineActive?songTick:transportTickAbsolute);
  mod806CollectMidi(max((int64_t)0,(int64_t)(songEngineActive?songTick:transportTickAbsolute)-midiAdvance808));
  for(uint8_t b=0;b<3;++b){BlockScope21 block(b);
  MidiScope902 route(outMask902(t),t);
  const auto&c=patch808Context(t,pattern);
  for(auto q=patchNext819(c,nullptr);q;q=patchNext819(c,q)){uint16_t value;if(!patchValue819(c,*q,value))continue;
    // Recall the stored base at this event; ongoing modulation is evaluated separately.
    if(!q->kind)midiForce810(patch808Channel(t,c),q->number);
    patch808SendParam(patch808Channel(t,c),*q,value);
  }
}
}
FLASHMEM void patch808Pattern(uint8_t t){pidiRoutesDirty931|=1u<<t;patch808PatternAt(t,patch808PlayingPattern(t));}
FLASHMEM void patch808Play(){for(uint8_t t=0;t<16;++t)programSend939(t,patch808PlayingPattern(t));patchProgramsSent51=true;modRecall937();mod806CollectMidi(songEngineActive?songTick:transportTickAbsolute);for(uint8_t t=0;t<16;++t)patch808Pattern(t);patchProgramsSent51=false;}
FLASHMEM bool patchStored10(uint32_t id){for(auto&track:patchDb808.context)for(auto&c:track)if(patchHas819(c,id))return true;return false;}
FLASHMEM uint32_t patchStoredCount10(){uint32_t n=0;for(auto*d=patchDb808.defs;d;d=d->next)if(patchStored10(d->id))++n;return n;}
FLASHMEM uint32_t patch808DiskSize(){
  uint32_t n=16+512*sizeof(uint16_t)+2+4;for(uint8_t t=0;t<16;++t)if(trackCcMask933&(1u<<t))n+=2;
  for(auto d=patchDb808.defs;d;d=d->next){if(!patchStored10(d->id))continue;n+=104;for(auto q=d->params;q;q=q->next)n+=8;}
  for(auto &track:patchDb808.context)for(auto &c:track){n+=70;for(auto v=c.values;v;v=v->next)n+=6;}
  return n;
}
FLASHMEM bool patch808Write(File&f){
  bool ok=true;auto put=[&](const void*p,size_t n){if(ok)ok=f.write((const uint8_t*)p,n)==n;};
  uint32_t magic=0xA215043u;put(&magic,4);uint32_t count=patchStoredCount10();put(&count,4);put(&patchDb808.nextPatch,4);put(&patchDb808.nextParam,4);
  for(auto d=patchDb808.defs;d;d=d->next){if(!patchStored10(d->id))continue;put(&d->id,4);put(d->source819,95);int8_t offset44=d->noteOffset44;put(&offset44,1);uint32_t n=patch808ParamCount(d);put(&n,4);
    for(auto q=d->params;q;q=q->next){put(&q->id,4);put(&q->number,2);put(&q->kind,1);put(&q->bits,1);}
  }
  for(auto&track:patchDb808.context)for(auto&c:track){put(&c.patch,4);put(&c.channel,1);put(c.extra,60);uint8_t explicitValue=c.explicit819;put(&explicitValue,1);uint32_t n=0;for(auto v=c.values;v;v=v->next)++n;put(&n,4);
    for(auto v=c.values;v;v=v->next){uint16_t value=v->value;for(auto*q=patchNext819(c,nullptr);q;q=patchNext819(c,q))if(q->id==v->parameter){patchValue819(c,*q,value);break;}put(&v->parameter,4);put(&value,2);}
  }
  for(uint16_t old=0;old<512;++old){uint16_t rat=8,key=old/2;if(key<64&&modUnits805[key].enabled&&modKinds931[key]==(old%2))rat=modRat808[key];put(&rat,2);}put(&midiAdvance808,2);put(&delayMute933,2);put(&trackCcMask933,2);for(uint8_t t=0;t<16;++t)if(trackCcMask933&(1u<<t))put(&trackCc933[t],2);return ok;
}
FLASHMEM bool patch808Read(File&f,bool install){
  patch808Free(patchStage808);bool ok=true;
  auto get=[&](void*p,size_t n){if(ok)ok=f.read((uint8_t*)p,n)==n;};
  uint32_t count=0;get(&count,4);bool refs10=count==0xA215043u;bool modern933=refs10||count==0x9335043u;bool modern819=modern933||count==0x8195043u;if(modern819)get(&count,4);get(&patchStage808.nextPatch,4);get(&patchStage808.nextParam,4);
  if(!patchStage808.nextPatch||!patchStage808.nextParam||count>(uint32_t)f.available()/18)return false;
  auto tail=&patchStage808.defs;
  for(uint32_t i=0;i<count&&ok;++i){auto d=patch808Allocate<PatchDef808>();if(!d){ok=false;break;}*tail=d;tail=&d->next;
    uint32_t n=0;get(&d->id,4);if(!refs10)get(d->name,modern819?48:10);if(modern819){get(d->source819,95);get(&d->noteOffset44,1);}get(&n,4);if(!d->id||d->id>=patchStage808.nextPatch||n>(uint32_t)f.available()/(refs10?8:18)){ok=false;break;}
    for(auto prev=patchStage808.defs;prev!=d;prev=prev->next)if(prev->id==d->id)ok=false;
    auto qt=&d->params;
    for(uint32_t j=0;j<n&&ok;++j){auto q=patch808Allocate<PatchParam808>();if(!q){ok=false;break;}*qt=q;qt=&q->next;
      uint8_t reserved;get(&q->id,4);get(&q->number,2);get(&q->kind,1);get(&q->bits,1);if(!refs10){get(q->name,6);get(q->shortName,3);get(&reserved,1);}
      if(!q->id||q->id>=patchStage808.nextParam||q->kind>2||(q->bits!=7&&q->bits!=14)||(q->kind!=1&&(q->number>127||q->bits!=7))||q->number>16383)ok=false;
      for(auto def=patchStage808.defs;def;def=def->next)for(auto prev=def->params;prev;prev=prev->next)if(prev!=q&&prev->id==q->id)ok=false;
    }
  }
  for(auto&track:patchStage808.context)for(auto&c:track){if(!ok)break;uint32_t n=0;get(&c.patch,4);get(&c.channel,1);if(modern819){get(c.extra,60);uint8_t e=0;get(&e,1);if(e>1)ok=false;c.explicit819=e;}get(&n,4);
    bool gap=!c.patch;for(uint8_t slot=1;slot<16;++slot){uint32_t id=patchId819(c,slot);if(!id){gap=true;continue;}if(gap||!patch808Find(patchStage808,id))ok=false;for(uint8_t prev=0;prev<slot;++prev)if(patchId819(c,prev)==id)ok=false;}
    if(c.channel<-1||c.channel>15||(c.patch&&!patch808Find(patchStage808,c.patch))||n>(uint32_t)f.available()/6){ok=false;break;}
    auto valueTail=&c.values;
    for(uint32_t i=0;i<n&&ok;++i){uint32_t id=0;uint16_t value=0;get(&id,4);get(&value,2);auto q=patchNextDb819(patchStage808,c,nullptr);while(q&&q->id!=id)q=patchNextDb819(patchStage808,c,q);
      uint16_t duplicate;if(!q||patch808Get(c,id,duplicate)||(value!=65535&&value>(q->bits==14?16383:127))){ok=false;break;}
      // Preserve disk order as well as values, so a read/write is deterministic.
      auto v=patch808Allocate<PatchValue808>();if(!v){ok=false;break;}v->parameter=id;v->value=value;*valueTail=v;valueTail=&v->next;
    }
  }
  for(auto&r:fullRat808)r=8;for(uint16_t old=0;old<512;++old){uint16_t rat=8;get(&rat,2);if(rat<1||rat>3072)ok=false;int key=modStaging931?fullMap931[old]:((old/2<64&&modKinds931[old/2]==old%2)?old/2:-1);if(key>=0)fullRat808[key]=rat;}get(&fullMidiAdvance808,2);
  fullDelayMute933=fullCcMask933=0;memset(fullCc933,0,sizeof(fullCc933));if(modern933){get(&fullDelayMute933,2);get(&fullCcMask933,2);for(uint8_t t=0;t<16;++t)if(fullCcMask933&(1u<<t)){get(&fullCc933[t],2);if(fullCc933[t]<-96||fullCc933[t]>96)ok=false;}}else fullMidiAdvance808=-fullMidiAdvance808;
  for(auto r:fullRat808)if(r<1||r>3072)ok=false;
  if(fullMidiAdvance808<-96||fullMidiAdvance808>96)ok=false;
  if(!ok){patch808Free(patchStage808);return false;}
  if(install){patchResolve10(patchStage808);patch808Free(patchDb808);patchDb808=patchStage808;memset(&patchStage808,0,sizeof(patchStage808));memcpy(modRat808,fullRat808,sizeof(modRat808));midiAdvance808=fullMidiAdvance808;installPrefs933();}
  return true;
}
FLASHMEM uint32_t patch808Hash(){
  uint32_t h=2166136261u;auto add=[&](const void*p,size_t n){auto b=(const uint8_t*)p;while(n--){h^=*b++;h*=16777619u;}};
  for(auto d=patchDb808.defs;d;d=d->next){if(!patchStored10(d->id))continue;add(&d->id,4);add(d->source819,96);add(&d->noteOffset44,1);for(auto q=d->params;q;q=q->next){add(&q->id,4);add(&q->number,2);add(&q->kind,1);add(&q->bits,1);}}
  for(auto&track:patchDb808.context)for(auto&c:track){add(&c.patch,4);add(&c.channel,1);add(c.extra,60);add(&c.explicit819,1);for(auto v=c.values;v;v=v->next){uint16_t value=v->value;for(auto*q=patchNext819(c,nullptr);q;q=patchNext819(c,q))if(q->id==v->parameter){patchValue819(c,*q,value);break;}add(&v->parameter,4);add(&value,2);}}
  add(modRat808,sizeof(modRat808));add(&midiAdvance808,2);add(&delayMute933,2);add(&trackCcMask933,2);for(uint8_t t=0;t<16;++t)if(trackCcMask933&(1u<<t))add(&trackCc933[t],2);return h;
}

