#pragma once
// EP alone selects the storage scope. No QV or Neo mode inference.
FLASHMEM bool scopeOpen18(bool save){
 uint8_t kind=0;
 if(kbMenuR10&&kbMenuTrackR10)kind=ITEM_KB18;
 else if(modUi805==MOD_LIST806||modUi805==MOD_REMOVE806)kind=modType805==2?ITEM_DELAYS18:modType805?ITEM_LFOS18:ITEM_ENVS18;
 else if(modUi805==MOD_FX806)kind=ITEM_DELAYS18;
 if(!kind)return false;
 if(!sdAvailable||(save&&!storageWriteAvailable)){DISPLAY_SERIAL.println("MSG|SD UNAVAILABLE");return true;}
 if(modDirty806){DISPLAY_SERIAL.println("MSG|FINISH EDIT FIRST");return true;}
 itemSave809=save;itemTrack809=kind==ITEM_KB18?selectedTrack:modOwnerTrack805;itemPattern809=kind==ITEM_KB18?activePattern[selectedTrack]:modOwnerPattern805;
 itemReturnMod809=modUi805;itemReturnPatch809=patchUi808;itemReturnKb18=kbMenuR10;modUi805=patchUi808=kbMenuR10=0;
 itemBrowser809(kind);return true;
}
FLASHMEM const char* storageDescription18(){
 static char label[21];
 const char* names[]={"","Patch definition","Patch preset","ENV preset","LFO preset","Delay preset","Track parameters","KB configuration","ENV configuration","LFO configuration","Delay configuration","Groove sheet"};
 if(itemKind809){if(itemKind809==ITEM_FX809){snprintf(label,21,"Delay %u preset",delaySlot924+1);return label;}return names[itemKind809];}
 if(remixBrowser911)return "Remix / Pattern Link";
 if(fileBrowserMode==FILE_BROWSER_LOAD)return activeStorageType==STORAGE_FULL?"Full project":activeStorageType==STORAGE_PATTERN?"Pattern + PLX":activeStorageType==STORAGE_SEQUENCE?"Sequence + PLX":"Note + PLX";
 if(pendingSaveClipboard){snprintf(label,21,"C%u %s",pendingSaveClipboardSlot+1,clipKinds18[pendingSaveClipboardSlot]==4?"Track parameters":clipKinds18[pendingSaveClipboardSlot]==5?"Remix":clipKinds18[pendingSaveClipboardSlot]==6?"PLX":clipKinds18[pendingSaveClipboardSlot]==3?"Patterns + PLX":clipKinds18[pendingSaveClipboardSlot]==1?"Note + PLX":"Sequence + PLX");return label;}
 switch(pendingSaveCandidate){case SAVE_CANDIDATE_FULL:return "Full project";case SAVE_CANDIDATE_PATTERN:return "Pattern + PLX";case SAVE_CANDIDATE_SETTINGS:return "General parameters";case SAVE_CANDIDATE_SELECTION:return "Selection + PLX";case SAVE_CANDIDATE_NOTE:return "Note + PLX";default:return "Sequence + PLX";}
}
FLASHMEM void clipMenuEnter18(){
 saveMenuClipboardMode=true;saveMenuSelection=0;
 for(uint8_t i=0;i<4;++i){clipKinds18[i]=contentClipKind18(i);if(!clipKinds18[i])clipKinds18[i]=clipboardSaveKind(i);if(clipboardSlots[i].valid&&!patternClipboard[i].valid){if(clipboardSlots[i].type==CLIP_TYPE_TRACK12)clipKinds18[i]=4;else if(clipboardSlots[i].type==CLIP_TYPE_REMIX)clipKinds18[i]=5;else if(clipboardSlots[i].type==CLIP_TYPE_EFFECT)clipKinds18[i]=6;}}
}

// Scope files carry only the requested configuration. Modulation routes retain
// all their fields; internal modulation identities are remapped on import.
struct ScopeMod18 {int16_t early50=0;ModUnit805 unit{};ModMeta806 meta{};ModExtra810 extra{};CcEnvelope941 cc{};uint16_t rat=1;uint8_t kind=0;char name[8]{};};
struct ScopeData18 {
 uint32_t magic=0x58004355;uint8_t kind=0,origin=0,present=0,reserved=0;
 uint64_t roots=0;KbTrackR10 kb{};int32_t center=36;
 uint16_t mixer=0;uint8_t cut=0,mute=0,mixerOff=0;
 DelayValues907 delays[4]{};ScopeMod18 mods[64]{};
};
FLASHMEM bool scopeValid18(const ScopeData18&d,uint8_t kind){
 if((d.magic!=0x58004355)||d.kind!=kind||d.origin>=16||d.present>15||kind<ITEM_KB18||kind>ITEM_DELAYS18)return false;
 if(d.kb.split>2||d.kb.panels>1||d.center< -32767||d.center>65535||d.cut>1||d.mute>1||d.mixerOff>15)return false;
 for(const auto&z:d.kb.zone)if(z.type>4||z.mute>1||z.valid>1||z.range< -65535||z.range>65535||z.center< -32767||z.center>65535)return false;
 for(uint8_t i=0;i<4;++i)if(d.present&(1u<<i)){const auto&v=d.delays[i];if(!v.length||v.length>3072||v.wait>3072||v.repeats>16||v.output>16||v.mode>2||v.enabled>1||v.source>1||v.rolls>1||v.gex08>100||v.axp>100||v.oxp>100||v.chain&~((1u<<i)-1)||v.gat08==-128||v.edited930>1||v.fixedVelocity926<0||v.relativeVelocity926==-128||v.velocity==-128||v.note==-128||v.accel==-128)return false;}
 for(uint8_t i=0;i<4;++i)if(d.present&(1u<<i))for(auto&a:d.delays[i].targets55){if(a.kind==8){if(a.track>=16||a.number>127)return false;}else if(!delay55::valid(a))return false;if((a.kind==5||a.kind==6)&&(a.identity>=64||!d.mods[a.identity].unit.enabled))return false;}
 uint8_t degree[64]={},total=0,done=0;bool seen[64]={};
 for(uint8_t i=0;i<64;++i){auto&s=d.mods[i];auto&m=s.unit;if(s.early50>0||s.early50< -384||m.enabled>1||m.roll58>2||m.delay58>2||s.kind>1||s.name[7])return false;if(!m.enabled){if(d.roots&(1ULL<<i))return false;continue;}++total;
  if(!m.rate||m.rate>LFO_MAX42||m.sustain>127||m.shape>4||!s.rat||s.rat>3072||s.extra.mode>6||s.extra.phase>360||s.cc.enabled>1||s.cc.channel>15||s.cc.number>127||s.cc.invert>1)return false;
  for(auto&r:m.outputs)if(r.enabled){if((r.block21>2&&!modTrackMidi43(r)&&!ioVirtual53(r))||(r.kind==MOD_CV46&&r.track>3)||r.port>8||r.channel>15||r.reference<(r.kind==MOD_MIDI_BEND806?-8192:0)||r.reference>(r.kind==MOD_MIDI_BEND806?8191:r.kind==MOD_CV46?4095:127)||r.enabled>1||r.track>=16||(r.kind>MOD_TRACK21&&r.kind!=MOD_CV46))return false;if(modIsTarget811(r)){auto j=modTarget810(r);if(j>=64||!d.mods[j].unit.enabled)return false;++degree[j];}}
 }
 for(uint8_t pass=0;pass<64;++pass){bool more=false;for(uint8_t i=0;i<64;++i)if(d.mods[i].unit.enabled&&!seen[i]&&!degree[i]){seen[i]=true;more=true;++done;for(auto&r:d.mods[i].unit.outputs)if(r.enabled&&modIsTarget811(r))--degree[modTarget810(r)];}if(!more)break;}
 return done==total;
}
FLASHMEM bool scopeCapture18(ScopeData18&d){
 d.kind=itemKind809;d.origin=itemTrack809;
 if(d.kind==ITEM_KB18){d.kb=kbSettings13(d.origin);d.center=kbCenter12;}
 if(d.kind==ITEM_DELAYS18){d.cut=(delayCut12>>linkOwner12(d.origin,LINK_DLY12))&1;d.mute=(delayMute933>>d.origin)&1;d.mixerOff=((mixerOff938[1]>>d.origin)&1)<<1;for(uint8_t i=0;i<4;++i){if(delayPresent925(d.origin,i)){d.present|=1u<<i;d.delays[i]=delayGet907(delayKey924(d.origin,0,i));}}}
 if(d.kind==ITEM_ENVS18||d.kind==ITEM_LFOS18){
  uint8_t family=d.kind==ITEM_LFOS18?3:2;d.mixerOff=((mixerOff938[family]>>d.origin)&1)<<family;
  uint8_t oldType=modType805,oldTrack=modOwnerTrack805;modType805=d.kind==ITEM_LFOS18;modOwnerTrack805=d.origin;
  for(uint8_t i=0;i<64;++i)if(mod806Visible(i))d.roots|=1ULL<<i;
  modType805=oldType;modOwnerTrack805=oldTrack;
 }
 if(d.kind==ITEM_DELAYS18)for(uint8_t i=0;i<4;++i)if(d.present&(1u<<i))for(auto&a:d.delays[i].targets55)if(a.kind==5||a.kind==6){if(a.identity>=64||!modUnits805[a.identity].enabled)return false;d.roots|=1ULL<<a.identity;}
 uint64_t included=d.roots;
 for(uint8_t pass=0;pass<64;++pass){auto before=included;for(uint8_t i=0;i<64;++i)if(included&(1ULL<<i))for(auto&r:modUnits805[i].outputs)if(r.enabled&&modIsTarget811(r)){auto k=modTarget810(r);if(k>=64||!modUnits805[k].enabled)return false;included|=1ULL<<k;}if(before==included)break;}
 for(uint8_t i=0;i<64;++i)if(included&(1ULL<<i)){auto&s=d.mods[i];s.early50=envEarly50[i];s.unit=modUnits805[i];s.meta=modMeta806[i];s.extra=modExtra810[i];s.cc=ccEnv941[i];s.rat=modRat808[i];s.kind=modKinds931[i];memcpy(s.name,modNames07[i],8);uint8_t owner=linkOwner12(d.origin,s.kind?LINK_LFO12:LINK_ENV12);if(owner!=d.origin){if(s.unit.inputs&(1u<<owner))s.unit.inputs=(s.unit.inputs&~(1u<<owner))|(1u<<d.origin);if(s.meta.tracks&(1u<<owner))s.meta.tracks=(s.meta.tracks&~(1u<<owner))|(1u<<d.origin);for(auto&r:s.unit.outputs)if(r.enabled&&!modIsTarget811(r)&&(!mod806MidiTarget(r)||modTrackMidi43(r))&&r.kind!=MOD_VIRTUAL&&r.track==owner)r.track=d.origin;}}
 return scopeValid18(d,d.kind);
}
FLASHMEM uint32_t scopeHash18(const ScopeData18&d){return ~crc905(0xffffffffu,(const uint8_t*)&d,sizeof(d));}
FLASHMEM bool scopeWrite18(File&f,const ScopeData18&d){uint32_t hash=scopeHash18(d);return f.write((const uint8_t*)&d,sizeof(d))==sizeof(d)&&f.write((uint8_t*)&hash,4)==4;}
FLASHMEM bool scopeRead18(File&f,ScopeData18&d,uint8_t kind){uint32_t hash=0;bool ok=f.read((uint8_t*)&d,sizeof(d))==sizeof(d)&&f.read((uint8_t*)&hash,4)==4&&!f.available()&&hash==scopeHash18(d);return ok&&scopeValid18(d,kind);}
FLASHMEM bool scopeInstall18(const ScopeData18&d,uint8_t t){
 if(t>=16||!scopeValid18(d,itemKind809))return false;
 if(d.kind==ITEM_KB18){
  // Assignments refer to the destination project's existing objects. Refuse
  // unresolved assignments rather than silently dropping a keyboard zone.
  for(auto&z:d.kb.zone)if(z.valid){if(z.type==KB_MOD_R10&&(z.target>=64||!modUnits805[z.target].enabled))return false;if(z.type==KB_DLY_R10&&(z.target>=4||!delayPresent925(t,z.target)))return false;if(z.type==KB_AH_R10&&!kbPatchR10(t,z.target))return false;}
  kbSettings13(t)=d.kb;kbCenter12=d.center;
 }else if(d.kind==ITEM_DELAYS18){
  int16_t map[64];for(auto&v:map)v=-1;bool reserved[64]={};
  for(unsigned i=0;i<64;++i)if(d.mods[i].unit.enabled){for(unsigned j=0;j<64;++j)if(!reserved[j]&&!modUnits805[j].enabled){map[i]=j;reserved[j]=true;break;}if(map[i]<0)return false;}
  DelayNode907*nodes[4]={};for(uint8_t i=0;i<4;++i)if(d.present&(1u<<i)){nodes[i]=(DelayNode907*)extmem_malloc(sizeof(DelayNode907));if(!nodes[i]){for(auto*p:nodes)if(p)extmem_free(p);return false;}}
  for(unsigned i=0;i<64;++i)if(map[i]>=0){unsigned j=map[i];modDetach45(j);auto&v=d.mods[i];modUnits805[j]=v.unit;modMeta806[j]=v.meta;modKinds931[j]=v.kind;modExtra810[j]=v.extra;envEarly50[j]=v.early50;ccEnv941[j]=v.cc;ccSeen941[j]=false;modRat808[j]=v.rat;memcpy(modNames07[j],v.name,8);
   auto&m=modUnits805[j];if(m.inputs&(1u<<d.origin))m.inputs=(m.inputs&~(1u<<d.origin))|(1u<<t);modMeta806[j].tracks=(modMeta806[j].tracks&~(1u<<d.origin))|(1u<<t);
   for(auto&r:m.outputs)if(r.enabled){if(modIsTarget811(r))modRouteKey931(r,map[modTarget810(r)]);else if((!mod806MidiTarget(r)||modTrackMidi43(r))&&r.kind!=MOD_VIRTUAL&&r.track==d.origin)r.track=t;}}
  uint8_t owner=linkOwner12(t,LINK_DLY12);
  for(uint8_t i=0;i<4;++i){delayRemove907(owner*16+i*256);if(auto*n=nodes[i]){n->key=owner*16+i*256;n->value=d.delays[i];for(auto&a:n->value.targets55){if(a.kind==5||a.kind==6)a.identity=map[a.identity];if(a.kind==8&&a.track==d.origin)a.track=t;}if(n->value.output==d.origin)n->value.output=t;n->next=delayNodes907;delayNodes907=n;delayIndex925[owner][i]=n;delaySet907(n->key,n->value);}}
  delayCut12=(delayCut12&~(1u<<owner))|(d.cut<<owner);delayMute933=(delayMute933&~(1u<<t))|(d.mute<<t);mixerOff938[1]=(mixerOff938[1]&~(1u<<t))|(((d.mixerOff>>1)&1)<<t);
 }else{
  bool replace[64]={};int16_t map[64];for(auto&v:map)v=-1;
  uint8_t kind=d.kind==ITEM_LFOS18,owner=linkOwner12(t,kind?LINK_LFO12:LINK_ENV12);
  // Only exclusively owned members are recycled; shared objects survive for
  // other tracks. Dependencies are imported into their own available slots.
  for(uint8_t i=0;i<64;++i)replace[i]=modUnits805[i].enabled&&modKinds931[i]==kind&&modMeta806[i].tracks==(1u<<owner);
  // An external modulation or keyboard may still point to an old slot.
  // Never recycle that identity into an unrelated imported object.
  bool pinned[64]={};for(uint8_t k=0;k<64;++k)if(modUnits805[k].enabled&&!replace[k])for(auto&r:modUnits805[k].outputs)if(r.enabled&&modIsTarget811(r)&&modTarget810(r)<64)pinned[modTarget810(r)]=true;
  for(uint8_t track=0;track<16;++track)if(track!=t)for(auto&z:kbSettings13(track).zone)if(z.type==KB_MOD_R10&&z.target<64)pinned[z.target]=true;
  bool used[64]={};for(uint8_t i=0;i<64;++i)if(d.mods[i].unit.enabled){if(!modUnits805[i].enabled||(replace[i]&&d.origin==t)){map[i]=i;used[i]=true;}}
  for(uint8_t i=0;i<64;++i)if(d.mods[i].unit.enabled&&map[i]<0){for(uint8_t j=0;j<64;++j)if(!used[j]&&(!modUnits805[j].enabled||(replace[j]&&!pinned[j]))){map[i]=j;used[j]=true;break;}if(map[i]<0)return false;}
  for(uint8_t i=0;i<64;++i)if(modKinds931[i]==kind){auto&m=modUnits805[i];modMeta806[i].tracks&=~(1u<<owner);m.inputs&=~(1u<<owner);if(!replace[i]||pinned[i])for(auto&r:m.outputs)if(r.enabled&&!modIsTarget811(r)&&(!mod806MidiTarget(r)||modTrackMidi43(r))&&r.kind!=MOD_VIRTUAL&&r.track==t)r.enabled=0;}
  for(uint8_t i=0;i<64;++i)if(replace[i]&&!pinned[i]){modDetach45(i);modUnits805[i].enabled=0;modMeta806[i]={};}
  for(uint8_t i=0;i<64;++i)if(map[i]>=0){uint8_t j=map[i];if(!modUnits805[j].enabled)modDetach45(j);auto&s=d.mods[i];auto&m=modUnits805[j];m=s.unit;envEarly50[j]=s.early50;modMeta806[j]=s.meta;modKinds931[j]=s.kind;modExtra810[j]=s.extra;ccEnv941[j]=s.cc;ccSeen941[j]=false;modRat808[j]=s.rat;memcpy(modNames07[j],s.name,8);modPresetDirty07[j]=false;
   if(m.inputs&(1u<<d.origin))m.inputs=(m.inputs&~(1u<<d.origin))|(1u<<owner);
   auto&tracks=modMeta806[j].tracks;tracks=(tracks&~(1u<<d.origin))|((d.roots&(1ULL<<i))?(1u<<owner):0);
   for(auto&r:m.outputs)if(r.enabled){if(modIsTarget811(r))modRouteKey931(r,map[modTarget810(r)]);else if((!mod806MidiTarget(r)||modTrackMidi43(r))&&r.kind!=MOD_VIRTUAL&&r.track==d.origin)r.track=t;}
  }
  uint8_t family=kind?3:2;mixerOff938[family]=(mixerOff938[family]&~(1u<<t))|(((d.mixerOff>>family)&1)<<t);mod807ClearHistory();
 }
 mod805Invalidate();freeInvalidate11();neoDisplayDirty=displayValueDirty=true;return true;
}
FLASHMEM void scopeSave18(){
 auto*d=(ScopeData18*)extmem_malloc(sizeof(ScopeData18));if(!d){DISPLAY_SERIAL.println("MSG|CONFIG MEMORY");return;}new(d)ScopeData18{};
 char path[96]={};bool ok=scopeCapture18(*d)&&makeUniqueSavePath(itemLabel809(),itemLabel809(),itemExtension809(),path,sizeof(path));
 if(ok){File f=SD.open(path,FILE_WRITE_BEGIN);ok=f&&f.truncate(0)&&scopeWrite18(f,*d);f.close();if(ok){f=SD.open(path,FILE_READ);ok=f&&scopeRead18(f,*d,itemKind809);f.close();}}
 extmem_free(d);ok=finishSave905(ok,path,sizeof(path));if(ok){rememberFileName816(path);itemClose809();DISPLAY_SERIAL.println("MSG|CONFIG SAVED");}else DISPLAY_SERIAL.println("MSG|CONFIG SAVE ERROR");
}
FLASHMEM void scopeLoad18(){
 char path[96];snprintf(path,sizeof(path),"%s/%s",fileBrowserPath,fileBrowserEntry);
 auto*d=(ScopeData18*)extmem_malloc(sizeof(ScopeData18));if(!d){DISPLAY_SERIAL.println("MSG|CONFIG MEMORY");return;}File f=SD.open(path,FILE_READ);bool ok=f&&scopeRead18(f,*d,itemKind809);f.close();if(ok)ok=scopeInstall18(*d,itemTrack809);extmem_free(d);
 if(ok){rememberFileName816(path);itemClose809();DISPLAY_SERIAL.println("MSG|CONFIG LOADED");}else DISPLAY_SERIAL.println("MSG|INVALID CONFIG / TARGET");
}


