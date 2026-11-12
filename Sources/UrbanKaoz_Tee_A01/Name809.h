#pragma once
// A session identifies one name edit. Queued phone edits cannot target a new field.
FLASHMEM bool nameQueuedValid813(const char*line){
  if(!songMidi&&!pc831.active)return false;
  unsigned long session=0;unsigned open=0;
  return sscanf(line,"NAMEEDIT,%lu,%u",&session,&open)==2&&session==nameSession809&&open==(patchUi808==4?1u:0u);
}
FLASHMEM void nameSync809(bool force){
  static bool wasOpen=false,announced=false,requested=false,sent=false;
  static uint32_t last=0;static char previous[100]="";
  bool open=patchUi808==4;
  if(force||open!=wasOpen)nameDirty931=true;
  if(!nameDirty931)return;
  if(wasOpen&&!open)++nameSession809;wasOpen=open;
  // USB-host MIDI connection is the existing Y5 link, not the PC debug port.
  if(!songMidi&&!pc831.active){announced=requested=sent=false;previous[0]=0;return;}
  if(!open&&!announced){requested=false;return;}
  requested|=force;
  uint32_t now=millis();
  // Never queue a history of characters or compete with the normal outbox.
  // Force requests obey the same cadence; the newest draft is read when ready.
  if((sent&&(uint32_t)(now-last)<250)||songOutRead!=songOutWrite)return;
  nameDirty931=false;
  uint8_t limit=nameLimit07(patchNameTarget808);
  char line[100];snprintf(line,sizeof(line),"NAMEEDIT,%lu,%u,%s,%u,%u,%u,%s",(unsigned long)nameSession809,open?1:0,
    patchNameTarget808==3?"Track":patchNameTarget808==4?"Modulation":patchNameTarget808==0?"Patch":patchNameTarget808==1?"Parameter":"Short",limit,patchNameTarget808==2?3:1,patchNamePos808,open?patchNameDraft808:"");
  if(requested||strcmp(line,previous)){
    songSend("%s",line);copyField(previous,sizeof(previous),line);last=now;sent=true;announced=open;requested=false;
  }
}
FLASHMEM bool nameCommand809(char*line){
  if(!strcmp(line,"NAMEEDIT?")){nameSync809(true);return true;}
  if(strncmp(line,"NAMEEDIT,",9))return false;
  nameSync809();char*fields[5];uint8_t n=itemFields809(line+9,fields,5);uint32_t sid=0,pos=0;
  if(n<2||!itemNumber809(fields[1],UINT32_MAX,sid)||sid!=nameSession809||patchUi808!=4){songError("STALE_NAME_CONTEXT");nameSync809(true);return true;}
  if(!strcmp(fields[0],"SET")&&n==4){
    nameDirty931=false;
  uint8_t limit=nameLimit07(patchNameTarget808);
    if(!itemNumber809(fields[2],limit,pos)||!itemName809(fields[3],limit,0)){songError("INVALID_NAME");return true;}
    copyField(patchNameDraft808,sizeof(patchNameDraft808),fields[3]);patchNamePos808=min((uint32_t)(limit-1),min(pos,(uint32_t)strlen(fields[3])));patch808Render();
  }else if(!strcmp(fields[0],"COMMIT")&&n==2)patch808Roy(0,0,true);
  else if(!strcmp(fields[0],"CANCEL")&&n==2)patch808Back();
  else songError("INVALID_NAME_ACTION");
  nameSync809(true);return true;
}
