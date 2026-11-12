#pragma once
static ModRoute805 modNewBefore50;

// Menu contract V8.08: live navigation must not stop transport.
// OA selects sources/list items; OC selects destinations. Selection uses a fixed >.
// Blink means an uncommitted Roy value, never selection and never audible preview.
// J1 goes back and discards only uncommitted fields. Curves/depth/RAT stay on KY40.
// Add is a list item, absent during Delete; Delete+Roy removes once and exits.
// Small pagination indicators must not consume full button slots in new menus.
uint16_t modCursor807=0;
uint8_t modSourceCursor807=0,modDestCursor807=0,modFocus807=0;
struct ModUndo807 {int16_t early50; char name07[8];CcEnvelope941 cc; ModUnit805 unit; ModMeta806 meta; ModExtra810 extra; uint16_t key,rat; uint8_t kind,index,type; };
ModUndo807 modHistory807[2];
uint8_t modHistoryCount807=0;
uint8_t modSavedTrigger807=0,modDraftMode810=0;
FLASHMEM void mod807ClearHistory(){modHistoryCount807=0;}
FLASHMEM void mod807Remember(uint8_t kind=0,uint8_t index=0) {
  modHistoryPrepare45(modHistoryCount807);
  if(modHistoryCount807==2){memmove(modHistory807,modHistory807+1,sizeof(ModUndo807));--modHistoryCount807;}
  auto &u=modHistory807[modHistoryCount807++];u.key=modSelected806;
  u.early50=envEarly50[u.key];u.type=modKinds931[u.key];u.kind=kind;u.index=index;u.rat=modRat808[u.key];
  memcpy(u.name07,modNames07[u.key],8);u.cc=ccEnv941[u.key];u.extra=modExtra810[u.key];u.unit=mod806Unit(u.key);u.meta=modMeta806[u.key];
}
FLASHMEM bool mod807Undo() {
  if(!modHistoryCount807)return false;
  if(modDirty806){DISPLAY_SERIAL.println("MSG|FINISH EDIT FIRST");return true;}
  if(!modHistoryReady45(modHistoryCount807-1))return true;
  auto &u=modHistory807[--modHistoryCount807];
  int16_t beforeEarly50=envEarly50[u.key];char beforeName07[8];memcpy(beforeName07,modNames07[u.key],8);auto beforeCc=ccEnv941[u.key];bool beforeSeen=ccSeen941[u.key];uint8_t beforeKind=modKinds931[u.key];auto before=mod806Unit(u.key);auto beforeMeta=modMeta806[u.key];auto beforeExtra=modExtra810[u.key];auto beforeRat=modRat808[u.key];
  if(u.kind==0){envEarly50[u.key]=u.early50;memcpy(modNames07[u.key],u.name07,8);ccEnv941[u.key]=u.cc;ccSeen941[u.key]=false;modKinds931[u.key]=u.type;mod806Unit(u.key)=u.unit;modMeta806[u.key]=u.meta;modRat808[u.key]=u.rat;modExtra810[u.key]=u.extra;}
  else if(u.kind==1){uint16_t &mask=u.index<16?mod806Unit(u.key).inputs:modMeta806[u.key].midiInputs;mask|=1u<<(u.index%16);}
  else mod806Unit(u.key).outputs[u.index]=u.unit.outputs[u.index];
  if(!modGraphValid810(modUnits805)){envEarly50[u.key]=beforeEarly50;memcpy(modNames07[u.key],beforeName07,8);ccEnv941[u.key]=beforeCc;ccSeen941[u.key]=beforeSeen;modKinds931[u.key]=beforeKind;mod806Unit(u.key)=before;modMeta806[u.key]=beforeMeta;modExtra810[u.key]=beforeExtra;modRat808[u.key]=beforeRat;++modHistoryCount807;graphDirty810=true;DISPLAY_SERIAL.println("MSG|MOD LOOP REFUSED");return true;}
  modHistoryRestore45(modHistoryCount807);
  modRemove806=false;deleteButtonActive=false;sendSelectionIndicatorsToPicran();
  mod805Invalidate();if(modUi805){mod805Menu();mod805Display();}
  DISPLAY_SERIAL.println("MSG|MOD RESTORED");return true;
}
FLASHMEM void mod807DeleteDone() {
  modRemove806=false;deleteButtonActive=false;
  sendSelectionIndicatorsToPicran();mod805Invalidate();mod805Menu();mod805Display();
}
char modPreviousLcd806[4][2][17]={};
EXTMEM uint16_t modVisible806[64];
uint16_t modVisibleCount806=0,modObjectPage806=0;
int modFields806[4]={},modSavedFields806[4]={};
bool modAddressPage19=false;
uint8_t modConfirmKind806=0,modConfirmIndex806=0;
int8_t modDepthSlot806=-1;
Delay805 modSavedDelay806={};
FLASHMEM const char *mod806TargetName(uint8_t kind) {
  static const char *names[]={"Note","Velocity","Gate","Timing","Pitch Bend",
    "Roll Start","Roll Speed","Roll Accel","Roll Expo","Roll Curve","Roll Level",
    "CC","Virtual bus","Pitch Bend"};
  return kind==MOD_CV46?"CV output":kind<=MOD_RLV?names[kind]:kind==MOD_PATCH810?"Patch":kind==MOD_DEPTH810?"Mod Depth":kind<=MOD_MIDI_BEND806?names[kind]:"?";
}
FLASHMEM const char *mod806ShortTarget(uint8_t kind) {
  static const char *names[]={"NOT","VEL","GAT","TIM","PBN","RST","RSP","RAC","REX","RCV","RLV","CC","BUS","BEND"};
  return kind==MOD_CV46?"CV":kind<=MOD_MIDI_BEND806?names[kind]:"?";
}
FLASHMEM bool mod806Visible(uint16_t key) {
  const auto&m=mod806Unit(key);
  if(!m.enabled||modKind931(key)!=modType805||ioKey38(key)!=ioMenu38)return false;
  if(modAssociated12(key,modOwnerTrack805))return true;
  if(m.inputs&(1u<<modOwnerTrack805))return true;
  for(const auto&r:m.outputs)if(r.enabled&&!mod806MidiTarget(r)&&r.kind!=MOD_VIRTUAL&&!modIsTarget811(r)&&r.track==modOwnerTrack805)return true;
  return false;
}
FLASHMEM ModRoute805 mod806Canonical(ModRoute805 r) {
  r=modResolve810(r);
  if(r.kind==MOD_PBN){r.kind=MOD_MIDI_BEND806;r.track=midiChannelForTrack(r.track);}
  return r;
}
FLASHMEM bool cvShared50(uint8_t ch,uint16_t skip,uint8_t slot){
 for(uint16_t k=0;k<64;++k)if(modUnits805[k].enabled)for(uint8_t j=0;j<8;++j){const auto&q=modUnits805[k].outputs[j];if((k!=skip||j!=slot)&&q.enabled&&q.kind==MOD_CV46&&q.track==ch)return true;}return false;
}
FLASHMEM bool mod806Conflict(const ModRoute805 &route) {
  uint16_t count=0;auto target=mod806Canonical(route);
  for(uint16_t key=0;key<64;++key)if(mod806Unit(key).enabled)
    for(const auto&r:mod806Unit(key).outputs)
      if(mod806SameTarget(target,mod806Canonical(r))&&++count>1)return true;
  return false;
}
FLASHMEM uint16_t mod806ListCount() {
  modVisibleCount806=0;
  if(modUi805==MOD_LIST806||modUi805==MOD_REMOVE806) {
    for(uint16_t key=0;key<64;++key)if(mod806Visible(key))modVisible806[modVisibleCount806++]=key;
    return modVisibleCount806+2;
  }
  if(modUi805==MOD_OBJECT806)return 6;
  if(modUi805==MOD_FX806){uint32_t list[128];modVisibleCount806=delayList924(modOwnerTrack805,modOwnerPattern805,list,128);for(uint16_t i=0;i<modVisibleCount806;++i)modVisible806[i]=list[i];return modVisibleCount806+2;}
  if(modUi805==MOD_SOURCES806) {
    for(uint8_t i=0;i<32;++i) {
      uint16_t mask=i<16?mod806Unit(modSelected806).inputs:modMeta806[modSelected806].midiInputs;
      if(mask&(1u<<(i%16)))modVisible806[modVisibleCount806++]=i;
    }
  }
  if(modUi805==MOD_DESTS806)for(uint8_t i=0;i<8;++i)
    if(mod806Unit(modSelected806).outputs[i].enabled)modVisible806[modVisibleCount806++]=i;
  return modVisibleCount806;
}
FLASHMEM uint16_t mod806PageStart(uint16_t page){return page?3+(page-1)*2:0;}
FLASHMEM int mod806PageItem(uint8_t slot,uint16_t count) {
  uint16_t start=mod806PageStart(modPage806);
  bool next=start+(modPage806?3:4)<count;
  if((modPage806&&slot==0)||(next&&slot==3))return -1;
  uint16_t item=start+slot-(modPage806?1:0);
  return item<count?item:-1;
}
FLASHMEM void mod806LiveFrame(const char *a,const char*b,const char*c,const char*d) {
  settingsPageRetryPending=false;
  const char* rows[]={a,b,c,d};
  for(uint8_t i=0;i<4;++i){strncpy(menuScreenRaw[i],rows[i],95);menuScreenRaw[i][95]=0;}
  menuScrollLine=255;menuScrollOffset=0;
  renderMenuScreenRaw();
}
FLASHMEM void mod806Label(uint16_t item,char out[11]) {
  out[0]=0;
  if(modUi805==MOD_LIST806){
    if(item<2){strcpy(out,item?"Remove":"Add");return;}
    modName07(modVisible806[item-2],out,11);
  }else if(modUi805==MOD_CC941){strcpy(out,"CC input");
  }else if(modUi805==MOD_OBJECT806){
    const char* labels[]={"Add source","Del source","Add output","Del output","Sources","Outputs"};
    strncpy(out,labels[item],10);out[10]=0;
  }else if(modUi805==MOD_SOURCES806){
    uint8_t i=modVisible806[item];snprintf(out,11,"%s %02u",i<16?"Track":"USB",i%16+1);
  }else if(modUi805==MOD_DESTS806){
    const auto&r=mod806Unit(modSelected806).outputs[modVisible806[item]];
    snprintf(out,11,"%u %.3s %02u%s",modVisible806[item]+1,mod806ShortTarget(r.kind),r.track+1,mod806Conflict(r)?"!":"");
  }else if(modUi805==MOD_FX806){if(item<2)strcpy(out,item==0?"Add Delay":"Remove");else {uint16_t key=modVisible806[item-2];snprintf(out,11,"D%02u",key/256+1);}}
}
FLASHMEM void mod806SetFields(const ModRoute805&r) {
  modAddressPage19=false;
  modFields806[0]=r.kind==MOD_PATCH810?2:modIsTarget811(r)?3:mod806MidiTarget(r)?1:0;
  modFields806[1]=modIsTarget811(r)?modTarget810(r):(r.port&&r.kind!=MOD_PATCH810&&!modTrackMidi43(r)?r.channel:r.track);
  modFields806[2]=r.kind==MOD_PATCH810?r.number:modIsTarget811(r)?(r.kind==MOD_DEPTH810?1:0):r.kind;
  modFields806[3]=modIsTarget811(r)?r.number%8:r.number;
  if(r.kind==MOD_TRACK21)modTrackFields21(r,modFields806);if(r.kind==MOD_CV46){modFields806[0]=5;modFields806[1]=r.track;modFields806[2]=modFields806[3]=0;}
  memcpy(modSavedFields806,modFields806,sizeof(modFields806));
}
FLASHMEM void mod806ApplyFields() { /* V8.10: routing drafts never touch the engine. */ }
FLASHMEM void mod806SourcePreview(uint8_t kind,uint8_t index) {
  auto&m=mod806Unit(modSelected806);auto&meta=modMeta806[modSelected806];
  uint16_t &oldMask=modSourceKind806?meta.midiInputs:m.inputs;
  oldMask&=~(1u<<modSourceIndex806);
  modSourceKind806=kind;modSourceIndex806=index;
  uint16_t &mask=kind?meta.midiInputs:m.inputs;
  mask|=1u<<index;mod805Invalidate();
}
FLASHMEM void mod806CancelDraft() {
  if(modUi805==MOD_DEST806){
    memcpy(modFields806,modSavedFields806,sizeof(modFields806));mod806ApplyFields();
    if(modAdding806){mod806Unit(modSelected806).outputs[modOutputCursor805]=modNewBefore50;modAdding806=false;mod805Invalidate();}
  }else if(modUi805==MOD_SOURCE806){
    if(modDirty806&4)modDraftMode810=modSavedTrigger807;
    mod806SourcePreview(modCommittedSourceKind806,modCommittedSourceIndex806);
    if(modAdding806&&modDirty806==3){
      uint16_t &mask=modSourceKind806?modMeta806[modSelected806].midiInputs:mod806Unit(modSelected806).inputs;
      mask&=~(1u<<modSourceIndex806);
    }
  }else if(modUi805==MOD_DELAY806){
    auto &d=delays805[modOwnerTrack805][modOwnerPattern805];
    if(modDirty806&1)d.ticks=modSavedDelay806.ticks;
    if(modDirty806&2)d.repeats=modSavedDelay806.repeats;
    if(modDirty806&4)d.velocity=modSavedDelay806.velocity;
  }
  modDirty806=0;modAdding806=false;mod805Invalidate();
}
FLASHMEM void mod805Menu() {
 if(delayTargetsOpen55){delayTargetsMenu55();return;}
  if(!quickWorking903)quickFromEp903();
  clearMenuBlink821();if(ioMenuRender38())return;
  char a[21]={},b[21]={},c[21]={},d[21]={};
  if(modUi805==MOD_LIST806||modUi805==MOD_REMOVE806) {
    mod806ListCount();
    if(modVisibleCount806)modCursor807=min(modCursor807,(uint16_t)(modVisibleCount806-1));else modCursor807=0;
    char rows[4][21];for(auto &row:rows){memset(row,' ',20);row[20]=0;}
    uint16_t first=(modCursor807/3)*3;
    const char*flags58[]={"OFF","ON ","ONLY"};
    for(uint8_t slot=0;slot<3&&first+slot<modVisibleCount806;++slot){
      uint16_t pos=first+slot,key=modVisible806[pos];char label[10];modName07(key,label,10);
      rows[slot][0]=pos==modCursor807?'>':' ';memcpy(rows[slot]+1,label,min((size_t)8,strlen(label)));
      auto&m=mod806Unit(key);char flags[6];snprintf(flags,6,"R%s",flags58[min((unsigned)m.roll58,2u)]);memcpy(rows[slot]+10,flags,strlen(flags));
      snprintf(flags,6,"D%s",remoteOwn54(key)?flags58[min((unsigned)m.delay58,2u)]:"---");memcpy(rows[slot]+15,flags,strlen(flags));
    }
    if(modUi805==MOD_REMOVE806)memcpy(rows[3],"Remove modulation",17);
    else {memcpy(rows[3],"Add  Del  Ren.      ",20);}
    mod806LiveFrame(rows[0],rows[1],rows[2],rows[3]);return;
  }else if(modUi805==MOD_CC941){auto c941=ccEnv941[modSelected806];snprintf(a,21,"CC IN CH  CC   INV");snprintf(b,21,"%-5s%02u  %03u  %s",c941.enabled?"ON":"OFF",c941.channel+1,c941.number,c941.invert?"YES":"NO");strcpy(c,"Depth 1 = CC x1");strcpy(d,"OA click: back");
  }else if(modUi805==MOD_OBJECT806){
    uint8_t sources[32],outs[8],ns=0,no=0;
    auto &m=mod806Unit(modSelected806);
    for(uint8_t i=0;i<32;++i)if((i<16?m.inputs:modMeta806[modSelected806].midiInputs)&(1u<<(i%16)))sources[ns++]=i;
    for(uint8_t i=0;i<8;++i)if(m.outputs[i].enabled)outs[no++]=i;
    modSourceCursor807=min(modSourceCursor807,(uint8_t)(modRemove806?(ns?ns-1:0):ns));modDestCursor807=min(modDestCursor807,(uint8_t)(modRemove806?(no?no-1:0):no));
    modDepthSlot806=modDestCursor807<no?outs[modDestCursor807]:-1;
    char rows[4][21];for(auto &row:rows){memset(row,' ',20);row[20]=0;}
    for(uint8_t col=0;col<2;++col){
      uint8_t cursor=col?modDestCursor807:modSourceCursor807,count=col?no:ns;
      uint8_t top=count<3?1:0,slots=3-top,total=count+(modRemove806?0:1);
      if(top){const char*title=col?(modRemove806?"Del output":"Outputs"):(modRemove806?"Del source":"Sources");memcpy(rows[0]+col*10,title,strlen(title));}
      uint8_t first=cursor>=slots?cursor-slots+1:0;
      for(uint8_t line=0;line<slots&&first+line<total;++line){
        uint8_t pos=first+line;char label[11]={};
        if(pos==count)strcpy(label,"Add");
        else if(!col){uint8_t src=sources[pos];if(ioKey38(modSelected806))snprintf(label,11,"%s %02u",ioSourceName38(m.hold),m.attack+1);else snprintf(label,11,"%s %02u",src<16?"TRK":"USB",src%16+1);}
        else {const auto&r=m.outputs[outs[pos]];modRouteLabel810(r,label);}
        if(pos==cursor){if(pos==count)strcpy(label,">Add");else {memmove(label+1,label,8);label[0]='>';label[9]=0;}}
        memcpy(rows[line+top]+col*10,label,strlen(label));
      }
    }
    memcpy(rows[3],modRemove806?"Cancel    Rename    ":"Delete    Rename    ",20);mod806LiveFrame(rows[0],rows[1],rows[2],rows[3]);return;
  }else if(modUi805==MOD_SOURCE806||modUi805==MOD_DEST806) {
    char fields[4][12]={};
    if(modUi805==MOD_SOURCE806){
      strcpy(a,"Type Track Trigger");
      sourcePulse909.watch(sourceWatchKey909());a[18]=' ';a[19]=sourcePulse909.dark(millis())?' ':'o';a[20]=0;
      strcpy(fields[0],modSourceKind806?"USB":"TRK");snprintf(fields[1],12,"%02u",modSourceIndex806+1);
      if(modType805&&modDraftMode810==3){memcpy(a+15,"Bars ",5);snprintf(fields[3],12,"%u",max((uint16_t)1,mod806Unit(modSelected806).hold));}
      const char*mode=modModeName07(modDraftMode810);
      strcpy(fields[2],modDraftMode810==5?"CC":mode);snprintf(c,21,"%s",mode);if(modDraftMode810==5){strcpy(a,"Chan CC   Trigger");snprintf(fields[0],12,"%02u",ccEnv941[modSelected806].channel+1);snprintf(fields[1],12,"%03u",ccEnv941[modSelected806].number);}
    }else{
      modRouteRender810(a,fields,c);
      if(modAddressPage19){auto&r=mod806Unit(modSelected806).outputs[modOutputCursor805];int reference19=0;const char*label19="";modTargetBase06(r,0,reference19,label19);strcpy(a,modTrackMidi43(r)?"Track BLK1 Ref      ":"Port Chan Ref       ");snprintf(fields[0],12,"%u",r.kind==MOD_PATCH810?firstOut12(r.track)+1:r.port?r.port:firstOut12(modOwnerTrack805)+1);snprintf(fields[1],12,"%u",r.kind==MOD_PATCH810?modResolve810(r).track+1:r.port?r.channel+1:modResolve810(r).track+1);snprintf(fields[2],12,"%d",reference19);if(ioVirtual53(r)){strcpy(fields[0],"USB V");snprintf(fields[1],12,"V%u",r.channel+17);}strcpy(fields[3],"Back");strcpy(c,"OA/OB/OC edit");if(modTrackMidi43(r)){snprintf(fields[0],12,"%u",r.track+1);strcpy(fields[1],"1");strcpy(c,"OA track OC ref");}}
    }
    memset(b,' ',20);b[20]=0;
    uint8_t position=0;
    for(uint8_t i=0;i<4;++i){
      uint8_t width=modUi805==MOD_DEST806&&modFields806[0]==3?(i==0?3:i==1?7:i==2?4:6):5;
      memcpy(b+position,fields[i],min((size_t)width,strlen(fields[i])));
      if(modDirty806)menuBlink821[1]=0xFFFFF;
      position+=width;
    }
    if(modUi805==MOD_DEST806&&(modFields806[0]==1||modFields806[0]==2)){strcpy(d,modAddressPage19?"          < Route   ":"          Port/Chan ");}
    else snprintf(d,21,"%s %03u %s",modType805?"LFO":"ENV",modSelected806+1,modUi805==MOD_SOURCE806?"SOURCE":"DEST");memcpy(d,"Delete    ",10);
  }else if(modUi805==MOD_FX806){
    delayHome925(a,b,c,d);
  }else if(modUi805==MOD_DELAY806){
    delayMenu907(a,b,c,d);
  }else if(modUi805==MOD_CONFIRM806){
    strcpy(a,"Confirm removal?");
    strcpy(b,modConfirmKind806==0?"Track association":modConfirmKind806==1?"Source connection":modConfirmKind806==3?"Delay":"Destination");
    strcpy(c,"Confirm   Cancel");
  }else{
    uint16_t count=mod806ListCount();
    while(modPage806&&mod806PageStart(modPage806)>=count)--modPage806;
    snprintf(a,21,"%s T%02u",modUi805==MOD_FX806?"DLY":modType805?"LFO":"ENV",modOwnerTrack805+1);
    if(modSelected806<64&&modUi805!=MOD_LIST806&&modUi805!=MOD_FX806)
      snprintf(b,21,"%03u %s",modSelected806+1,modRemove806?"Remove link":modUi805==MOD_SOURCES806?"Sources":modUi805==MOD_DESTS806?"Destinations":"Connections");
    else strcpy(b,modRemove806?"Remove association":"");
    memset(c,' ',20);memset(d,' ',20);c[20]=d[20]=0;
    for(uint8_t slot=0;slot<4;++slot){
      char text[11]={};int item=mod806PageItem(slot,count);
      if(item>=0)mod806Label(item,text);
      else if(slot==0&&modPage806)strcpy(text,"<");
      else if(slot==3&&mod806PageStart(modPage806)+(modPage806?3:4)<count)strcpy(text,">");
      memcpy((slot<2?c:d)+(slot%2)*10,text,strlen(text));
    }
  }
  mod806LiveFrame(a,b,c,d);
}
FLASHMEM void mod806Small(uint8_t screen,const char*left,const char*lv,const char*right,const char*rv) {
  
  char a[17],b[17];snprintf(a,17,"%-8.8s%-8.8s",left,right);snprintf(b,17,"%-8.8s%-8.8s",lv,rv);
  if(!strcmp(modPreviousLcd806[screen][0],a)&&!strcmp(modPreviousLcd806[screen][1],b))return;
  strcpy(modPreviousLcd806[screen][0],a);strcpy(modPreviousLcd806[screen][1],b);
  DISPLAY_SERIAL.print("MODLCD|");DISPLAY_SERIAL.print(screen);DISPLAY_SERIAL.print('|');
  DISPLAY_SERIAL.print(a);DISPLAY_SERIAL.print('|');DISPLAY_SERIAL.println(b);
}
FLASHMEM bool mod805Display() {if(!quickWorking903&&quickView903)return quickDisplay903();if(!quickWorking903)return false;return modDisplay810();}
FLASHMEM void mod805Open(uint8_t type) {
  ioMenu38=type==4;if(ioMenu38)type=0;
  memset(modPreviousLcd806,0,sizeof(modPreviousLcd806));
  modOwnerTrack805=selectedTrack;modOwnerPattern805=activePattern[selectedTrack];
  if(patternMode&&lastPatternValid&&patternSelected[lastPatternTrack][lastPatternIndex]){
    modOwnerTrack805=lastPatternTrack;modOwnerPattern805=lastPatternIndex;
  }
  modCursor807=modSourceCursor807=modDestCursor807=modFocus807=0;
  delaySlot924=delayChosen925[modOwnerTrack805];delayChainView924=-1;modType805=min(type,(uint8_t)2);modUi805=type>=2?MOD_FX806:MOD_LIST806;
  modSelected806=UINT16_MAX;modPage806=modListPage806=modObjectPage806=0;modDepthSlot806=-1;
  modRemove806=modAdding806=false;modDirty806=0;
  mod805Menu();mod805Display();
}
FLASHMEM bool mod806Navigate(int x) {
  if(!modUi805)return false;
  if(modUi805==MOD_LIST806||modUi805==MOD_OBJECT806||modUi805==MOD_SOURCES806||modUi805==MOD_DESTS806||modUi805==MOD_FX806){
    uint16_t count=mod806ListCount();
    if(x>0&&mod806PageStart(modPage806)+(modPage806?3:4)<count)++modPage806;
    else if(x<0&&modPage806)--modPage806;
    mod805Menu();
  }
  return true;
}
FLASHMEM bool mod805Back() {
 if(delayTargetsOpen55){delayEditDetail55=false;delayTargetsOpen55=false;mod805Menu();return true;}
  if(!modUi805)return false;
  uint8_t page=modUi805;mod806CancelDraft();
  modHistoryRestore45(modHistoryCount807);
  modRemove806=false;deleteButtonActive=false;sendSelectionIndicatorsToPicran();
  if(page==MOD_REMOVE806){modUi805=MOD_LIST806;modRemove806=false;}
  else if(page==MOD_LIST806||page==MOD_FX806){ioMenu38=false;
    modUi805=0;modSelected806=UINT16_MAX;showGameScreen();
  }else if(page==MOD_OBJECT806){
    modUi805=MOD_LIST806;modPage806=modListPage806;modSelected806=UINT16_MAX;modDepthSlot806=-1;modRemove806=false;
  }else if(page==MOD_SOURCE806||page==MOD_DEST806){
    modUi805=modParent806;modPage806=modReturnPage806;
  }else if(page==MOD_CONFIRM806){modUi805=modConfirmParent806;}
  else if(page==MOD_DELAY806){modUi805=MOD_FX806;modPage806=0;}
  else {modUi805=MOD_OBJECT806;modPage806=modObjectPage806;modRemove806=false;}
  if(modUi805){mod805Menu();mod805Display();}return true;
}
FLASHMEM void mod806Confirm(uint8_t kind,uint8_t index) {
  modConfirmKind806=kind;modConfirmIndex806=index;modConfirmParent806=modUi805;
  modUi805=MOD_CONFIRM806;mod805Menu();
}
FLASHMEM bool mod806Delete(){
  if(!modUi805)return false;
  if(modUi805==MOD_DELAY806){mod806CancelDraft();mod806Confirm(3,delaySlot924);return true;}
  if(modUi805==MOD_SOURCE806||modUi805==MOD_DEST806)return mod805Screen(4);
  if(modUi805==MOD_OBJECT806||modUi805==MOD_LIST806||modUi805==MOD_FX806) {
    modRemove806=!modRemove806;deleteButtonActive=modRemove806;
    sendSelectionIndicatorsToPicran();mod805Menu();
  }
  return true;
}
FLASHMEM void mod806EditSource(uint8_t index,bool adding) {
  modSavedTrigger807=modDraftMode810=modExtra810[modSelected806].mode;
  modParent806=modUi805;modReturnPage806=modPage806;
  modSourceKind806=modCommittedSourceKind806=index/16;
  modSourceIndex806=modCommittedSourceIndex806=index%16;
  modSourceCursor807=0;
  for(uint8_t i=0;i<index;++i)if((i<16?mod806Unit(modSelected806).inputs:modMeta806[modSelected806].midiInputs)&(1u<<(i%16)))++modSourceCursor807;
  modAdding806=false;modDirty806=0;
  if(adding){mod807ClearHistory();uint16_t&mask=index<16?mod806Unit(modSelected806).inputs:modMeta806[modSelected806].midiInputs;mask|=1u<<(index%16);}
  modUi805=MOD_SOURCE806;mod805Invalidate();mod805Menu();mod805Display();
}
FLASHMEM void mod806EditDest(uint8_t index,bool adding) {
  modDestCursor807=0;for(uint8_t i=0;i<index;++i)if(mod806Unit(modSelected806).outputs[i].enabled)++modDestCursor807;
  modParent806=modUi805;modReturnPage806=modPage806;modOutputCursor805=index;modDepthSlot806=index;
  auto&r=mod806Unit(modSelected806).outputs[index];
  if(adding){modNewBefore50=r;mod807ClearHistory();r={MOD_PATCH810,modOwnerTrack805,modPatchNext810(modOwnerTrack805,255,1),0,0};if(ccEnv941[modSelected806].enabled)r.depth=0;}
  mod806SetFields(r);modAdding806=adding;modDirty806=0;
  modUi805=MOD_DEST806;mod805Invalidate();mod805Menu();mod805Display();
}
FLASHMEM bool mod805Screen(uint8_t index) {
 if(delayTargetsOpen55){delayTargetsButton55(index);return true;}
  if(!modUi805)return false;
  if(modUi805==MOD_FX806){delayHomeButton925(index);return true;}
  if(modUi805==MOD_DELAY806){
    if(index==0)delayMode907();
    if(index==1)delayToggle907[1]=1;
    if(index==5)delayToggle907[1]=0;
    if(index==2||index==6)delayEdit817(4,index==2?1:-1);
    if(index==3){delayEditSlot55=(delayTargetSelected55[modOwnerTrack805][delaySlot924]+1)%5;delayEditDetail55=false;delayTargetsOpen55=true;delayTargetsMenu55();}if(index==7)delayTargetsSelect55();
    if(index==4)delayEdit817(5,delayValue907(modOwnerTrack805,modOwnerPattern805,5,true)?-1:1);
    delayMenuDirty817=true;modDisplayDirty810=true;return true;
  }
  if(modUi805==MOD_CONFIRM806){
    if(index<2){
      if(modConfirmKind806==0){
        mod806Associate(modMeta806[modSelected806],modOwnerTrack805,modOwnerPattern805,false);
        modUi805=MOD_LIST806;modPage806=modListPage806;modSelected806=UINT16_MAX;modDepthSlot806=-1;
      }else if(modConfirmKind806==1){
        uint16_t&mask=modConfirmIndex806<16?mod806Unit(modSelected806).inputs:modMeta806[modSelected806].midiInputs;
        mask&=~(1u<<(modConfirmIndex806%16));modUi805=MOD_SOURCES806;
      }else if(modConfirmKind806==3){delayRemove924(modOwnerTrack805,modOwnerPattern805,modConfirmIndex806);modUi805=MOD_FX806;
      }else {mod806Unit(modSelected806).outputs[modConfirmIndex806].enabled=0;modUi805=MOD_DESTS806;modDepthSlot806=-1;}
      modRemove806=false;modDirty806=0;modAdding806=false;mod805Invalidate();mod805Menu();mod805Display();
    }else if(index<4)mod805Back();
    return true;
  }
  if((modUi805==MOD_SOURCE806||modUi805==MOD_DEST806)&&(index==4||index==5)){bool dest=modUi805==MOD_DEST806;uint8_t item=dest?modOutputCursor805:modSourceKind806*16+modSourceIndex806;mod806CancelDraft();mod807Remember(dest?2:1,item);if(dest)mod806Unit(modSelected806).outputs[item].enabled=0;else{uint16_t &mask=item<16?mod806Unit(modSelected806).inputs:modMeta806[modSelected806].midiInputs;mask&=~(1u<<(item%16));}modUi805=MOD_OBJECT806;mod807DeleteDone();return true;}if(modUi805==MOD_DEST806&&index>=6){auto&r=mod806Unit(modSelected806).outputs[modOutputCursor805];if(r.kind==MOD_CC||r.kind==MOD_PATCH810||r.kind==MOD_MIDI_BEND806){if(modDirty806){DISPLAY_SERIAL.println("MSG|FINISH EDIT FIRST");return true;}modAddressPage19=!modAddressPage19;mod805Menu();}return true;}
  if(modUi805==MOD_SOURCE806||modUi805==MOD_DEST806||modUi805==MOD_DELAY806)return true;
  if(modUi805==MOD_LIST806){
    if(index==5){modUi805=MOD_REMOVE806;modRemove806=true;mod805Menu();return true;}if(index==7)return true;if(index==4){
      if(uint16_t key=modFreeSlot931();key<64){modDetach45(key);modKinds931[key]=modType805;modOff28&=~(1ULL<<key);
        auto&m=mod806Unit(key);memset(&m,0,sizeof(m));memset(&modMeta806[key],0,sizeof(ModMeta806));
        modExtraReset810(key);
        for(uint16_t other=0;other<64;++other)for(auto&r:mod806Unit(other).outputs)
          if(modIsTarget811(r)&&modTarget810(r)==key)r.enabled=0;
        modNames07[key][0]=0;modPresetDirty07[key]=false;m.enabled=1;m.attack=1;m.decay=100;m.sustain=80;m.release=100;m.rate=384;m.inputs=1u<<modOwnerTrack805;modRat808[key]=8;
        if(ioMenu38){m.shape=4;m.curve=2;m.hold=0;m.attack=modOwnerTrack805;m.decay=0;m.release=127;m.sustain=0;ioSeen38[key]=false;snprintf(modNames07[key],8,"IO%02u",key+1);}
        mod806Associate(modMeta806[key],modOwnerTrack805,modOwnerPattern805,true);
        modHistoryCount807=0; // Do not restore a deleted identity over a newly allocated one.
        modSelected806=key;modSourceCursor807=modDestCursor807=0;modDepthSlot806=-1;
        modUi805=MOD_OBJECT806;modRemove806=false;mod805Invalidate();mod805Menu();mod805Display();return true;
      }
      DISPLAY_SERIAL.println("MSG|MOD CAPACITY");return true;
    }
    if(index==6){mod806ListCount();if(modVisibleCount806){modSelected806=modVisible806[min(modCursor807,(uint16_t)(modVisibleCount806-1))];objectName07(4);}return true;}
    return true;
  }
  if(modUi805==MOD_OBJECT806){if(index==4||index==5){mod807Remember();modDetach45(modSelected806,modHistoryCount807-1);mod806Unit(modSelected806).enabled=0;memset(&modMeta806[modSelected806],0,sizeof(ModMeta806));modSelected806=UINT16_MAX;modUi805=MOD_LIST806;modDepthSlot806=-1;mod807DeleteDone();}else if(index>=6)objectName07(4);return true;}if(modUi805==MOD_REMOVE806)return true;
  uint16_t count=mod806ListCount();int item=mod806PageItem(index/2,count);if(item<0)return true;
  if(false){
  }else if(modUi805==MOD_SOURCES806){
    uint8_t i=modVisible806[item];if(modRemove806)mod806Confirm(1,i);else {mod806EditSource(i,false);return true;}
  }else if(modUi805==MOD_DESTS806){
    uint8_t i=modVisible806[item];if(modRemove806)mod806Confirm(2,i);else {mod806EditDest(i,false);return true;}
  }else if(modUi805==MOD_FX806){
    if(item==1){modRemove806=!modRemove806;mod805Menu();return true;}
    if(item==0){if(modRemove806||!delayAdd924(modOwnerTrack805,modOwnerPattern805))return true;}
    else {delaySelect924(modVisible806[item-2]);if(modRemove806){mod806Confirm(3,delaySlot924);return true;}}
    modSavedDelay806=delays805[modOwnerTrack805][modOwnerPattern805];neoDisplayDirty=true;modUi805=MOD_DELAY806;modDirty806=0;delayEntry933();
  }
  mod805Menu();mod805Display();return true;
}
FLASHMEM bool mod806Roy(uint8_t roy,int32_t delta,bool click) {
 if(delayTargetsOpen55){delayTargetsRoy55(roy,delta,click);return true;}
  if(!modUi805)return false;
  if(modUi805==MOD_FX806){delayHomeRoy925(roy,delta,click);return true;}
  if(modUi805==MOD_LIST806||modUi805==MOD_REMOVE806){
    mod806ListCount();
    if(roy==2||roy==3){if(modVisibleCount806&&(delta||click)){uint16_t k=modVisible806[min(modCursor807,(uint16_t)(modVisibleCount806-1))];if(roy==3&&!remoteOwn54(k)){mod805Menu();return true;}auto&m=mod806Unit(k);auto&v=roy==2?m.roll58:m.delay58;v=WRAP23(v,delta?delta:1,0,2);mod805Invalidate();storageDirty931=true;mod805Menu();}return true;}
    if(roy!=0)return true;
    if(delta&&modVisibleCount806)modCursor807=WRAP23(modCursor807,delta,0,modVisibleCount806-1);
    if(click&&modVisibleCount806){
      modSelected806=modVisible806[min(modCursor807,(uint16_t)(modVisibleCount806-1))];
      if(modUi805==MOD_REMOVE806||modRemove806){
        mod807Remember();modDetach45(modSelected806,modHistoryCount807-1);mod806Unit(modSelected806).enabled=0;memset(&modMeta806[modSelected806],0,sizeof(ModMeta806));
        modSelected806=UINT16_MAX;modUi805=MOD_LIST806;modDepthSlot806=-1;mod807DeleteDone();
      }else {modUi805=MOD_OBJECT806;modSourceCursor807=modDestCursor807=0;modFocus807=0;}
    }
    modBlink806=false;mod805Menu();mod805Display();return true;
  }
  if(ioRoy38(roy,delta,click))return true;
  if(modUi805==MOD_CC941){auto &c=ccEnv941[modSelected806];if(click&&roy==0){modUi805=MOD_OBJECT806;mod805Menu();return true;}if(delta){mod807Remember();if(roy==0){bool was=c.enabled;c.enabled=WRAP23(c.enabled,delta,0,1);if(c.enabled&&!was)for(auto&r:mod806Unit(modSelected806).outputs)if(r.enabled)r.depth=1;}if(roy==1)c.channel=clampInt((c.channel)+(delta),0,15);if(roy==2)c.number=WRAP23(c.number,delta,0,127);if(roy==3)c.invert=WRAP23(c.invert,delta,0,1);ccSeen941[modSelected806]=false;mod805Invalidate();}mod805Menu();return true;}
  if(modUi805==MOD_OBJECT806){
    
    if(roy!=0&&roy!=2)return true;
    bool output=modRemove806&&click&&roy==0?modFocus807:roy==2;if(delta)modFocus807=output;
    if(modRemove806&&click&&roy==2){modFocus807=1;mod805Menu();return true;}
    uint8_t items[32],count=0;auto&m=mod806Unit(modSelected806);
    if(output){for(uint8_t i=0;i<8;++i)if(m.outputs[i].enabled)items[count++]=i;}
    else for(uint8_t i=0;i<32;++i)if((i<16?m.inputs:modMeta806[modSelected806].midiInputs)&(1u<<(i%16)))items[count++]=i;
    uint8_t &cursor=output?modDestCursor807:modSourceCursor807;
    cursor=WRAP23(cursor,delta,0,modRemove806?(count?count-1:0):count);
    if(modRemove806&&!count){mod805Menu();return true;}
    if(click){
      if(cursor<count){
        if(modRemove806){
          mod807Remember(output?2:1,items[cursor]);
          if(output)m.outputs[items[cursor]].enabled=0;
          else {uint8_t src=items[cursor];uint16_t &mask=src<16?m.inputs:modMeta806[modSelected806].midiInputs;mask&=~(1u<<(src%16));}
          mod807DeleteDone();
        }else {if(output)mod806EditDest(items[cursor],false);else mod806EditSource(items[cursor],false);return true;}
      }else if(modRemove806){DISPLAY_SERIAL.println("MSG|SELECT A CONNECTION");}
      else if(output){
        for(uint8_t i=0;i<8;++i)if(!m.outputs[i].enabled){mod806EditDest(i,true);return true;}
        DISPLAY_SERIAL.println("MSG|OUTPUT CAPACITY");
      }else{
        uint32_t mask=(uint32_t)m.inputs|((uint32_t)modMeta806[modSelected806].midiInputs<<16);
        for(uint8_t n=0;n<32;++n){uint8_t i=(modOwnerTrack805+n)%32;if(!(mask&(1UL<<i))){mod806EditSource(i,true);return true;}}
        DISPLAY_SERIAL.println("MSG|SOURCE CAPACITY");
      }
    }
    modBlink806=false;mod805Menu();mod805Display();return true;
  }
  if(modUi805==MOD_CONFIRM806){if(click&&roy==0)mod805Screen(0);return true;}
  if(modUi805==MOD_DELAY806){
    if(roy>=4)return true;
    if(roy==3){if(click)delayXp56=!delayXp56;if(delta)delayTargetTurn56(delta);delayMenuDirty817=modDisplayDirty810=true;return true;}
    if(click){if(roy==1)delayIncrement925(delayToggle907[1]?0:1);else if(roy==2)delayToggle907[roy]=(delayToggle907[roy]+1)%3;else delayToggle907[roy]^=1;}
    if(delta)delayEdit817(delayRoyField21(roy),delta);
    delayMenuDirty817=true;modDisplayDirty810=true;return true;
  }else if(modUi805==MOD_DEST806){
    if(modAddressPage19){auto&r=mod806Unit(modSelected806).outputs[modOutputCursor805];
      if(roy==3&&click){modAddressPage19=false;}
      else if(delta&&roy<3){if(modTrackMidi43(r)&&!(ioKey38(modSelected806)&&roy==0)){if(roy==0){r.track=clampInt((int)r.track+delta,0,15);modFields806[1]=modSavedFields806[1]=r.track;}else if(roy==2){int value;const char*label="";modBaseEdit43(r,delta,value,label);}storageDirty931=true;mod805Invalidate();mod805Menu();return true;}if(r.kind==MOD_PATCH810&&roy<2){mod805Menu();return true;}if(!r.port){int base=0;const char*label="";modTargetBase06(r,0,base,label);r.reference=base;r.channel=modResolve810(r).track;r.port=firstOut12(r.kind==MOD_PATCH810?r.track:modOwnerTrack805)+1;}
       if(roy==0){int port=clampInt((ioVirtual53(r)?9:r.port)+delta,1,ioKey38(modSelected806)&&r.kind==MOD_CC?9:8);r.block21=port==9?4:0;r.port=port==9?1:port;}else if(roy==1)r.channel=clampInt((r.channel)+(delta),0,15);else {int value;const char*label="";modTargetBase06(r,delta,value,label);}
       if(r.kind!=MOD_PATCH810){r.track=r.channel;modFields806[1]=modSavedFields806[1]=r.channel;}storageDirty931=true;mod805Invalidate();}
    }else modRouteFields810(click,roy,delta);
  }else if(modUi805==MOD_SOURCE806){
    if(roy==3&&modType805&&modDraftMode810==3&&!click){if(delta){mod807Remember();auto&m=mod806Unit(modSelected806);m.hold=WRAP23(max((uint16_t)1,m.hold),delta,1,999);modPresetDirty07[modSelected806]=true;mod805Invalidate();}mod805Menu();return true;}
    if(roy>2&&!(roy==3&&click&&modType805&&modDraftMode810==3))return true;
    if(click){
      modCommittedSourceKind806=modSourceKind806;modCommittedSourceIndex806=modSourceIndex806;
      modSetMode07(modSelected806,modDraftMode810);modSavedTrigger807=modDraftMode810;
      modDirty806=0;modAdding806=false;
    }else if(roy==2){
      if(delta){modDraftMode810=modNextMode07(modDraftMode810,delta,modType805);modDirty806|=4;}
    }else if(delta&&modDraftMode810==5){auto&cc=ccEnv941[modSelected806];if(roy==0)cc.channel=clampInt((cc.channel)+(delta),0,15);else cc.number=WRAP23(cc.number,delta,0,127);ccSeen941[modSelected806]=false;modPresetDirty07[modSelected806]=true;
    }else if(delta){
      uint8_t kind=roy==0?WRAP23(modSourceKind806,delta,0,1):modSourceKind806;
      uint8_t n=roy==1?(modSourceKind806?clampInt(modSourceIndex806+delta,0,15):WRAP23(modSourceIndex806,delta,0,15)):modSourceIndex806;
      uint16_t mask=kind?modMeta806[modSelected806].midiInputs:mod806Unit(modSelected806).inputs;
      if(roy==1){
        int step=delta>0?1:-1;
        while(n<16&&(mask&(1u<<n))&&n!=modSourceIndex806){
          int next=(int)n+step;if(next<0||next>15){n=modSourceIndex806;break;}n=next;
        }
      }
      if((kind==modSourceKind806&&n==modSourceIndex806)||!(mask&(1u<<n))){
        mod806SourcePreview(kind,n);modDirty806|=1u<<roy;
      }
    }
  }else return true;
  if(click)mod805Invalidate();
  modBlink806=false;mod805Menu();mod805Display();return true;
}
FLASHMEM bool mod805Ky(uint8_t ky,int32_t delta) {return modKy810(ky,delta);}
FLASHMEM void mod806UiService() {
  if(!modUi805&&quickView903!=4&&quickView903!=5)return;
  static bool previousDark909=false;
  sourcePulse909.watch(sourceWatchKey909());
  bool dark909=sourcePulse909.dark(millis());
  if(previousDark909!=dark909){previousDark909=dark909;if(modUi805==MOD_SOURCE806)mod805Menu();}

}
FLASHMEM ModUnit805 mod808EffectiveUnit(uint16_t key){
  auto m=mod806Unit(key);
  if(modKind931(key)){m.rate=lockValue817(LOCK_MOD817,key,0,m.rate);m.shape=lockValue817(LOCK_MOD817,key,1,m.shape);m.decay=lockValue817(LOCK_MOD817,key,2,m.decay);m.attack=lockValue817(LOCK_MOD817,key,3,m.attack);m.release=lockValue817(LOCK_MOD817,key,4,m.release);m.hold=indLive50(LOCK_MOD817,key,5,m.hold);m.sustain=indLive50(LOCK_MOD817,key,6,m.sustain);}
  else {m.hold=max(0,holdValue50(key,m));m.attack=lockValue817(LOCK_MOD817,key,1,m.attack);m.decay=lockValue817(LOCK_MOD817,key,2,m.decay);m.sustain=lockValue817(LOCK_MOD817,key,3,m.sustain);m.release=lockValue817(LOCK_MOD817,key,4,m.release);}
  for(uint8_t i=0;i<8;++i)m.outputs[i].depth=lockValue817(LOCK_DEPTH817,key,i,m.outputs[i].depth);
  if(key!=modSelected806||!modDirty806)return m;
  if(modUi805==MOD_SOURCE806){
    if(!modSourceKind806)m.inputs&=~(1u<<modSourceIndex806);
    if(!modCommittedSourceKind806)m.inputs|=1u<<modCommittedSourceIndex806;

  }
  return m;
}
FLASHMEM ModMeta806 mod808EffectiveMeta(uint16_t key){
  auto m=modMeta806[key];
  if(key==modSelected806&&modDirty806&&modUi805==MOD_SOURCE806){
    if(modSourceKind806)m.midiInputs&=~(1u<<modSourceIndex806);
    if(modCommittedSourceKind806)m.midiInputs|=1u<<modCommittedSourceIndex806;
  }
  return m;
}
FLASHMEM void delayEdit817(uint8_t field,int32_t delta){
 if((field>11&&field!=15&&field!=16)||!delta)return;delayIncrementShown925=0;uint8_t t=modOwnerTrack805,p=modOwnerPattern805;uint16_t key=delayKey924(t,p,delaySlot924);
 int quantum=field<2?(delayTimeMode817[field]?96:1):((field==10||field==11||field==16)&&!curveFine41?10:1);
 int change=clampInt(delta,-3072,3072)*quantum,base=delayBaseSlot924(t,p,delaySlot924,field);
 int lo=delayMin907(field),hi=delayMax907(field);if(field==3&&delayValue907(t,p,8,true)==1){lo=0;base=max(0,base);}
 if(!lockEdit817(LOCK_DELAY817,key,field,base,change,lo,hi)){
  int v=(field<=2||field==4)?clampInt(base+change,lo,hi):WRAP23(base,change,lo,hi);if(!delayAssign924(t,p,delaySlot924,field,v))DISPLAY_SERIAL.println("MSG|DELAY MEMORY FULL");
 }
 modSavedDelay806=delays805[t][p];modDirty806=0;++modEpoch805;delayMenuDirty817=modDisplayDirty810=true;
}
FLASHMEM void followTrack817(){
 static uint8_t previous=0;bool changed=previous!=selectedTrack;previous=selectedTrack;
 if(changed&&modUi805&&modOwnerTrack805!=selectedTrack&&!itemKind809&&!itemChoosing809){
  uint8_t type=modType805;mod806CancelDraft();
  bool pattern=patternMode;patternMode=false;mod805Open(type);patternMode=pattern;
 }
 if(changed){if(patchBank808){patchPage808=0;memset(modPreviousLcd806,0,sizeof(modPreviousLcd806));patchDisplayDirty810=true;}if(patchUi808==1)patch808Render();}
}

FLASHMEM void delayHome925(char*a,char*b,char*c,char*d){
 memset(a,' ',20);memset(b,' ',20);memset(c,' ',20);memset(d,' ',20);a[20]=b[20]=c[20]=d[20]=0;
 uint8_t t=modOwnerTrack805,s=delayChosen925[t];auto v=delayGet907(t*16+s*256);
 for(uint8_t i=0;i<4;++i){char text[6];if(i==3){snprintf(text,6,"D%u %s",s+1,v.source?"TD":"DD");memcpy(a+15,text,5);}else if(i<s){const char*label=v.chain&(1u<<i)?"In":"Off";memcpy(a+i*5,label,strlen(label));}
  snprintf(text,6,"D%02u",i+1);memcpy(b+i*5,text,3);const char*on=delayEnabled924(t,0,i)?"On":"Off";memcpy(c+i*5,on,strlen(on));memcpy(d+i*5,modRemove806?"Del":"Edit",modRemove806?3:4);
 }
}
FLASHMEM void delayHomeRoy925(uint8_t roy,int32_t delta,bool click){uint8_t t=modOwnerTrack805,s=delayChosen925[t];
 if(roy==3&&delta){s=clampInt((int)s+(delta>0?1:-1),0,3);delayChosen925[t]=delaySlot924=s;}
 else if(roy==3&&click){delaySourceToggle930(t,s);}
 else if(roy<s&&(delta||click)&&delayPresent925(t,s)){auto d=delayGet907(t*16+s*256);if(click)d.chain^=1u<<roy;else if(delta>0)d.chain|=1u<<roy;else d.chain&=~(1u<<roy);delaySet907(t*16+s*256,d);}
 mod805Menu();mod805Display();
}
FLASHMEM void delayHomeButton925(uint8_t index){if(index>=8)return;uint8_t t=modOwnerTrack805,s=index%4;
 if(index<4){delayToggle925(t,s);mod805Menu();mod805Display();return;}
 delayChosen925[t]=delaySlot924=s;
 if(modRemove806){if(delayPresent925(t,s))mod806Confirm(3,s);return;}
 if(!delayPresent925(t,s)){DelayValues907 d=delayDefault928(s);d.enabled=0;if(!delaySet907(t*16+s*256,d)){DISPLAY_SERIAL.println("MSG|DELAY MEMORY FULL");return;}}
 neoDisplayDirty=true;modUi805=MOD_DELAY806;modDirty806=0;delayEntry933();modSelected806=UINT16_MAX;mod805Menu();mod805Display();
}


