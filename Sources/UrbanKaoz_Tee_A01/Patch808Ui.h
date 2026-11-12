#pragma once
uint32_t patchCursor808=0,patchParamCursor808=0;
bool patchNotes44=false;
FLASHMEM uint32_t patchVisibleCount44(PatchDef808*d){uint32_t n=0;for(auto*q=d?d->params:nullptr;q;q=q->next)if((q->kind==2)==patchNotes44)++n;return n;}
FLASHMEM PatchParam808* patchVisibleAt44(PatchDef808*d,uint32_t at){for(auto*q=d?d->params:nullptr;q;q=q->next)if((q->kind==2)==patchNotes44){if(!at--)return q;}return nullptr;}
int16_t midiDraft808=0;
PatchDef808* patchEditing808=nullptr;
PatchParam808* patchParam808=nullptr;
PatchParam808 patchDraft808={};
uint8_t patchDirty808=0,patchNamePos808=0,patchNameTarget808=0;
char patchNameDraft808[49]={};
uint32_t patchBlinkAt808=0;bool patchBlink808=false;
// UI states: 1 list, 2 definition parameters, 3 parameter, 4 text editor.
FLASHMEM void patch808Frame(const char*a,const char*b,const char*c,const char*d){mod806LiveFrame(a,b,c,d);}
FLASHMEM void patch808Render(){nameDirty931=true;neoDisplayDirty=true;
  if(!quickWorking903&&(patchUi808==1||patchUi808==2))quickPatchEp903();
  clearMenuBlink821();
  if(patchUi808==12){patchListRender819(nullptr);return;}
  if(patchUi808>=7){settingsRender811();return;}
  if(patchUi808==6){midiConfigRender810();return;}
  char rows[4][21];for(auto&r:rows){memset(r,' ',20);r[20]=0;}
  if(patchUi808==5){strcpy(rows[0],PSTR("CC timing (ticks)"));snprintf(rows[1],21,PSTR("%+d"),midiDraft808);strcpy(rows[2],PSTR("validate to hear"));strcpy(rows[3],PSTR("OA edit / click OK"));}
  else if(patchUi808==1){
    patchListRender819(rows);snprintf(rows[3],21,"New   Edit    BLK %u",patchBlock21+1);
  }else if(patchUi808==2){
    snprintf(rows[0],21,"%s %.13s",patchNotes44?"Notes":"CC",patchEditing808->name);
    uint32_t count=patchVisibleCount44(patchEditing808);patchParamCursor808=min(patchParamCursor808,count);
    uint32_t first=patchParamCursor808>=3?patchParamCursor808-2:0;
    for(uint8_t i=0;i<3&&first+i<=count;++i){auto q=patchVisibleAt44(patchEditing808,first+i);snprintf(rows[i+1],21,PSTR("%c%-6.6s %s %u"),first+i==patchParamCursor808?'>':' ',q?q->name:PSTR("Add"),q?(q->kind==2?PSTR("NOTE"):q->kind?PSTR("NRPN"):PSTR("CC")):PSTR(""),q?q->number:0);if(!q)snprintf(rows[i+1],21,PSTR(">Add"));}
  }else if(patchUi808==3){
    snprintf(rows[0],21,PSTR("%-6.6s  %-3.3s"),patchParam808->name,patchParam808->shortName);
    strcpy(rows[1],PSTR("Type Number Bits"));
    char fields[3][8];strcpy(fields[0],patchDraft808.kind==2?PSTR("NOTE"):patchDraft808.kind?PSTR("NRPN"):PSTR("CC"));snprintf(fields[1],8,PSTR("%u"),patchDraft808.number);snprintf(fields[2],8,PSTR("%u"),patchDraft808.bits);
    for(uint8_t i=0;i<3;++i){memcpy(rows[2]+i*6,fields[i],strlen(fields[i]));if(patchDirty808&(1u<<i))menuBlink821[2]|=((1UL<<strlen(fields[i]))-1)<<(i*6);}
    memcpy(rows[3],PSTR("Name"),4);memcpy(rows[3]+10,PSTR("Short name"),10);
  }else if(patchUi808==4){
    strcpy(rows[0],patchNameTarget808==5?"Automation name":patchNameTarget808==3?"Track name":patchNameTarget808==4?"Modulation name":patchNameTarget808==0?PSTR("Patch name"):patchNameTarget808==1?PSTR("Name (6 max)"):PSTR("Short name (3)"));
    uint8_t start=patchNamePos808/20*20;snprintf(rows[1],21,PSTR("%.20s"),patchNameDraft808+min((size_t)start,strlen(patchNameDraft808)));memset(rows[2],' ',20);rows[2][patchNamePos808-start]='^';
    strcpy(rows[3],PSTR("Neo: type / EP: OK"));
  }
  patch808Frame(rows[0],rows[1],rows[2],rows[3]);nameDisplayR8();
  if(patchUi808==2){char values[17];snprintf(values,sizeof(values),"        %-4s%+4d",patchNotes44?"NOT":"CC",patchEditing808->noteOffset44);DISPLAY_SERIAL.printf("MODLCD|3|        TypeNAM |%s\n",values);}
}
FLASHMEM bool patchNoteOffsetEdit44(uint8_t k,int32_t delta){if(patchUi808!=2||!patchEditing808)return false;if(k==14){if(delta){patchNotes44=delta>0;patchParamCursor808=0;}patch808Render();return true;}if(k!=15)return false;patchEditing808->noteOffset44=ZCLAMP57(patchEditing808->noteOffset44,delta,-127,127);storageDirty931=true;patch808Render();displayValueDirty=true;return true;}
FLASHMEM void patch808Open(){modUi805=0;patchUi808=1;patchCursor808=0;patchDirty808=0;patchCatalogScan10();patchResolve10(patchDb808);patchList819();patch808Render();}
FLASHMEM void patch808Name(uint8_t target){
  ++nameSession809;
  patchNameTarget808=target;patchNamePos808=0;memset(patchNameDraft808,0,sizeof(patchNameDraft808));
  strncpy(patchNameDraft808,target==0?patchEditing808->name:target==1?patchParam808->name:patchParam808->shortName,target==0?48:target==1?6:3);
  patchUi808=4;patch808Render();
}
FLASHMEM bool patch808Back(){
  if(!patchUi808)return false;
  if(patchUi808==4&&patchNameTarget808>=3){objectNameDone07(false);return true;}
  if(patchUi808==12){patchDeleteConfirm819(false);return true;}
  if(patchUi808==5||patchUi808==6){patchUi808=0;patchDirty808=0;settingsOpen=true;settingsPage=4;settingsMenuLevel=SETTINGS_ITEMS;settingsEditingRoy=-1;sendSettingsPage();return true;}
  if(patchUi808>=5){patchUi808=0;sendSettingsPage();return true;}
  if(patchUi808==1){patchUi808=0;showGameScreen();return true;}
  if(patchUi808==4)patchUi808=patchNameTarget808==0?2:3;
  else if(patchUi808==3){patchDirty808=0;patchUi808=2;}
  else {patchUi808=1;patchList819();}
  patch808Render();return true;
}
FLASHMEM void patch808Assign(){
 if(patchEnter07())return;
 auto*d=patchSelected819(true);if(patchCursor808&&!d)return;uint32_t id=d?d->id:0;
 bool enable=!patchAssigned811(id),any=false,ok=true;uint16_t visited52=0;
 struct Change819{Change819*next;PatchContext808*dst;PatchContext808 copy;};Change819*changes=nullptr;
 auto assign=[&](uint8_t t,int p){p=-1;if(visited52&(1u<<t))return;visited52|=1u<<t;if(!ok)return;auto&dst=patchDb808.context[linkOwner12(t,LINK_PATCH12)][patchContextIndex21(p+1)];auto*n=patch808Allocate<Change819>();if(!n){ok=false;return;}n->next=changes;changes=n;n->dst=&dst;
  const auto&src=p<0?dst:patch808Context(t,p);ok=patch808Clone(src,n->copy)&&patchAttach819(n->copy,id,enable);};
 if(patternMode)for(uint8_t t=0;t<16;++t)for(uint8_t p=0;p<16;++p)if(patternSelected[t][p]){assign(t,p);any=true;}
 if(!any)assign(selectedTrack,patternMode?activePattern[selectedTrack]:-1);
 while(changes){auto*n=changes;changes=n->next;if(ok){patch808FreeValues(n->dst->values);*n->dst=n->copy;}else patch808FreeValues(n->copy.values);extmem_free(n);}
 patchPage808=0;++modEpoch805;DISPLAY_SERIAL.println(!ok?PSTR("MSG|PATCH MEMORY / 16 MAX"):!id?PSTR("MSG|PATCH NONE"):enable?PSTR("MSG|PATCH ADDED"):PSTR("MSG|PATCH REMOVED"));displayValueDirty=true;
}
FLASHMEM bool patch808Screen(uint8_t i){
  if(!patchUi808)return false;
  if(patchUi808==12){if(i>=4)patchDeleteConfirm819(i>=6);return true;}
  if(patchUi808==13){if(i>=6){patchUi808=11;settingsRender811();}else{uint8_t k=i/2;bool old=k==0?playbackLocks828:k==1?playbackEnv828:playbackLfo828;playbackSet828(k,!old);playbackRender828();}return true;}
  if(patchUi808==11){if(i<4){playbackOpen828();}else if(i>=6){patchUi808=10;configDraft811[0]=lockRelease817;settingsRender811();}else if(i>=4){patchUi808=0;settingsItemSelection=3;settingsDraftValue=settingsValue[5][3];settingsEditingRoy=-1;settingsMenuLevel=SETTINGS_EDIT;sendSettingsPage();}return true;}
  if(patchUi808==1){
    if(i==4||i==5){patchEditing808=patch808New();if(!patchEditing808){DISPLAY_SERIAL.println(PSTR("MSG|PATCH MEMORY FULL"));return true;}patchList819();patchSelectId819(patchEditing808->id);patchParamCursor808=0;patch808Name(0);return true;}
    if(i>=6){patchEditing808=patchSelected819(true);if(patchEditing808){patchUi808=2;patchParamCursor808=0;}}
  }else if(patchUi808==2&&i==1){patch808Name(0);return true;}
  else if(patchUi808==3&&i>=4){patch808Name(i<6?1:2);return true;}
  patch808Render();return true;
}
FLASHMEM bool patch808Roy(uint8_t roy,int32_t delta,bool click){
  if(!patchUi808)return false;
  if(patchUi808==12){if(click&&roy==0)patchDeleteConfirm819(false);return true;}
  if(patchUi808==1&&roy==3){if(delta)patchBlock21=clampInt(patchBlock21+delta,0,2);patch808Render();return true;}
  if(settingsRoy811(roy,delta,click))return true;
  if(midiConfigRoy810(roy,delta,click))return true;
  if(patchUi808==5&&roy==0){if(delta)midiDraft808=WRAP23(midiDraft808,delta,-96,96);if(click){midiAdvance808=midiDraft808;++modEpoch805;patch808Render();return true;}}
  else if(patchUi808==1&&roy==0){uint32_t n=patchListCount819();if(delta&&n)patchCursor808=clampInt((int)patchCursor808+delta,0,n-1);if(click)patch808Assign();}
  else if(patchUi808==2&&roy==1&&click){patch808Name(0);return true;}
  else if(patchUi808==2&&roy==0){
    uint32_t n=patchVisibleCount44(patchEditing808);patchParamCursor808=clampInt((int)patchParamCursor808+delta,0,n);
    if(click){patchParam808=patchVisibleAt44(patchEditing808,patchParamCursor808);if(!patchParam808){patchParam808=patch808Add(patchEditing808);if(patchParam808)patchParam808->kind=patchNotes44?2:0;}
      if(!patchParam808){DISPLAY_SERIAL.println(PSTR("MSG|PATCH MEMORY FULL"));return true;}patchDraft808=*patchParam808;patchDirty808=0;patchUi808=3;}
  }else if(patchUi808==3&&roy<3){
    if(click){
      if(roy==0){patchParam808->kind=patchDraft808.kind;if(patchParam808->kind!=1){patchParam808->bits=7;patchParam808->number=min(patchParam808->number,(uint16_t)127);}}
      if(roy==1)patchParam808->number=min(patchDraft808.number,(uint16_t)(patchParam808->kind==1?16383:127));
      if(roy==2)patchParam808->bits=patchParam808->kind?patchDraft808.bits:7;
      patchDirty808&=~(1u<<roy);
      for(auto&track:patchDb808.context)for(auto&ctx:track)for(auto v=ctx.values;v;v=v->next)if(v->parameter==patchParam808->id)v->value=min(v->value,(uint16_t)(patchParam808->bits==14?16383:127));
      if(!(patchDirty808&1))patchDraft808.kind=patchParam808->kind;
      if(!(patchDirty808&2))patchDraft808.number=patchParam808->number;
      if(!(patchDirty808&4))patchDraft808.bits=patchParam808->bits;
    }else if(delta){
      if(roy==0){patchDraft808.kind=WRAP23(patchDraft808.kind,delta,0,2);if(patchDraft808.kind!=1){patchDraft808.bits=7;patchDraft808.number=min(patchDraft808.number,(uint16_t)127);}}
      if(roy==1)patchDraft808.number=WRAP23(patchDraft808.number,delta,0,patchDraft808.kind==1?16383:127);
      if(roy==2)patchDraft808.bits=patchDraft808.kind==1&&delta>0?14:7;
      patchDirty808|=1u<<roy;
    }
  }else if(patchUi808==4){
    uint8_t len=nameLimit07(patchNameTarget808);
    if(click&&roy==0){
      size_t n=strlen(patchNameDraft808);while(n&&patchNameDraft808[n-1]==' ')patchNameDraft808[--n]=0;
      if(!n||(patchNameTarget808==2&&n!=3)){DISPLAY_SERIAL.println(PSTR("MSG|NAME LENGTH"));return true;}
      if(patchNameTarget808>=3){objectNameDone07(true);return true;}
      char*dst=patchNameTarget808==0?patchEditing808->name:patchNameTarget808==1?patchParam808->name:patchParam808->shortName;
      memcpy(dst,patchNameDraft808,len);dst[len]=0;patchUi808=patchNameTarget808==0?2:3;
    }else if(delta){
      if(roy==0)patchNamePos808=clampInt((int)patchNamePos808+delta,0,len-1);
      if(roy==1){const char chars[]="ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-_ ";auto found=patchNameDraft808[patchNamePos808]?strchr(chars,patchNameDraft808[patchNamePos808]):nullptr;int pos=found?found-chars:(int)sizeof(chars)-2;int total=sizeof(chars)-1;pos=clampInt(pos+delta,0,total-1);
        size_t n=strlen(patchNameDraft808);while(n<=patchNamePos808)patchNameDraft808[n++]=' ';patchNameDraft808[n]=0;patchNameDraft808[patchNamePos808]=chars[pos];if(chars[pos]==' '&&patchNamePos808+1>=n)patchNameDraft808[patchNamePos808]=0;}
    }
  }
  patchBlink808=false;patch808Render();return true;
}
FLASHMEM bool patch808Display(){
  if(!patchBank808||patchUi808)return false;
  auto &c=patchDb808.context[linkOwner12(selectedTrack,LINK_PATCH12)][patchContextIndex21(0)];
  for(uint8_t screen=0;screen<4;++screen){
    uint16_t blink11=0;char labels[17],values[17];memset(labels,' ',16);memset(values,' ',16);labels[16]=values[16]=0;
    for(uint8_t i=0;i<4;++i){auto q=patchParam819(c,patchPage808*16+screen*4+i);if(!q)continue;memcpy(labels+i*4,patchLabel10(c,*q,true),3);
      uint16_t v=0;char txt[8];bool known=patchRawValue50(c,*q,v);int shown=lockDisplay817(q->kind?(q->bits==14?LOCK_NRPN14817:LOCK_NRPN7817):LOCK_CC817,selectedTrack,q->number,v);if(shown!=v)blink11|=15u<<(i*4);if(known||shown!=v){v=shown;if(v==65535)strcpy(txt,"-");else snprintf(txt,8,PSTR("%u"),v);}else strcpy(txt,PSTR("--"));
      // 14-bit values use scrolling-independent 4-character coarse display on LCD;
      // exact edited value is also shown on main display as a transient message.
      if(strlen(txt)>4)snprintf(txt,8,PSTR("%uk"),v/1000);
      memcpy(values+i*4,txt,min((size_t)4,strlen(txt)));
    }
    lockBlinkSend11(screen,blink11);
    if(!strcmp(modPreviousLcd806[screen][0],labels)&&!strcmp(modPreviousLcd806[screen][1],values))continue;
    strcpy(modPreviousLcd806[screen][0],labels);strcpy(modPreviousLcd806[screen][1],values);
    DISPLAY_SERIAL.print(PSTR("MODLCD|"));DISPLAY_SERIAL.print(screen);DISPLAY_SERIAL.print('|');DISPLAY_SERIAL.print(labels);DISPLAY_SERIAL.print('|');DISPLAY_SERIAL.println(values);
  }return true;
}
FLASHMEM void patch808Toggle(int direction){
  if(patchUi808){while(patchUi808)patch808Back();}
  auto &c=patchDb808.context[linkOwner12(selectedTrack,LINK_PATCH12)][patchContextIndex21(0)];uint32_t n=patchCount819(c);
  uint32_t pages=max((uint32_t)1,(n+15)/16);
  if(!patchBank808){patchBank808=true;patchPage808=direction>0?0:pages-1;}
  else if(direction>0&&patchPage808+1<pages)++patchPage808;
  else if(direction<0&&patchPage808>0)--patchPage808;
  else {patchBank808=false;DISPLAY_SERIAL.println(PSTR("CUSTOM,OFF"));}
  memset(modPreviousLcd806,0,sizeof(modPreviousLcd806));displayValueDirty=true;if(patchBank808)patch808Display();
}
FLASHMEM bool patch808Ky(uint8_t ky,int32_t delta){
  if(itemKind809||itemChoosing809)return true;
  if(!patchBank808||patchUi808)return false;
  if(!delta)return true;
  bool any=false;uint16_t visited52=0;
  auto edit=[&](uint8_t t,int p){
    if(visited52&(1u<<t))return;visited52|=1u<<t;p=-1;
    struct TrackEdit52 {uint8_t previous;TrackEdit52(uint8_t t):previous(selectedTrack){selectedTrack=t;}~TrackEdit52(){selectedTrack=previous;}} trackEdit52(t);
    auto &c=patchDb808.context[linkOwner12(t,LINK_PATCH12)][patchContextIndex21(p<0?0:p+1)];
    if(p>=0&&!c.patch&&!c.explicit819&&!patch808Clone(patchDb808.context[linkOwner12(t,LINK_PATCH12)][patchContextIndex21(0)],c)){DISPLAY_SERIAL.println(PSTR("MSG|PATCH MEMORY FULL"));return;}
    auto q=patchParam819(c,patchPage808*16+ky);if(!q)return;
    uint16_t old=0;bool defined=patchRawValue50(c,*q,old);if(lockWantsEdit817()&&indBase50(t,activePattern[t],q->kind?(q->bits==14?LOCK_NRPN14817:LOCK_NRPN7817):LOCK_CC817,t,q->number,defined?old:65535)==65535){lockBlinkSend11(ky/4,15u<<((ky%4)*4));DISPLAY_SERIAL.println("MSG|SET PATCH BASE FIRST");return;}if(old==65535)old=0;uint16_t v=WRAP23(old,delta,0,q->bits==14?16383:127);
    // Selected-note locks require a defined effective base.
    if(lockEdit817(q->kind?(q->bits==14?LOCK_NRPN14817:LOCK_NRPN7817):LOCK_CC817,t,q->number,old,delta,0,q->bits==14?16383:127))return;
    if(defined&&v==old)return;
    if(!patchSetDefault59(c,*q,v)){DISPLAY_SERIAL.println(PSTR("MSG|PATCH MEMORY FULL"));return;}
    if(p<0||p==activePattern[t]){autoManual46(t,q->kind?(q->bits==14?LOCK_NRPN14817:LOCK_NRPN7817):LOCK_CC817,t,q->number,v);patchChanged15(t,c,*q,v);}
    ++modEpoch805; // Re-aggregate with the new base once, before servicing the MIDI queue.
  };
  if(patternMode)for(uint8_t t=0;t<16;++t)for(uint8_t p=0;p<16;++p)if(patternSelected[t][p]){edit(t,p);any=true;}
  if(!any)edit(selectedTrack,patternMode?activePattern[selectedTrack]:-1);
  patchDisplayDirty810=true;return true;
}
FLASHMEM void patch808Service(){
  patchListService819();
  // Blink is autonomous on Picran.
}
FLASHMEM void midi808Open(){midiDraft808=midiAdvance808;patchUi808=5;settingsPageRetryPending=false;patch808Render();}
