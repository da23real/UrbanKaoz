#pragma once
// One directory snapshot: encoder movement never opens the SD card.
struct PatchEntry819 {PatchEntry819*next;uint32_t id;char name[49],path[96];bool directory;};
char patchDirectory07[96]="/URBANKAOZ/PATCH";
PatchEntry819*patchEntries819=nullptr;
uint32_t patchWanted819=0,patchDeleteId819=0;
char patchDeletePath819[96]={},patchDeleteName819[49]={};
uint8_t patchScroll819=0;
uint32_t patchScrollAt819=0,patchScrollCursor819=UINT32_MAX;
FLASHMEM uint32_t patchListCount819(){uint32_t n=1;for(auto*e=patchEntries819;e;e=e->next)++n;return n;}
FLASHMEM PatchEntry819* patchEntry819(uint32_t index){if(!index)return nullptr;auto*e=patchEntries819;while(e&&--index)e=e->next;return e;}
FLASHMEM void patchSelectId819(uint32_t id){patchWanted819=id;uint32_t n=1;for(auto*e=patchEntries819;e;e=e->next,++n)if(e->id==id){patchCursor808=n;return;}}
FLASHMEM void patchShortName20(char*name,const char*directory){
  const char*folder=strrchr(directory,'/');folder=folder?folder+1:directory;
  const char*prefix=nullptr;
  if(!strcasecmp(folder,"AR"))prefix="Analog Rytm ";
  else if(!strcasecmp(folder,"A4"))prefix="Analog Four ";
  else if(!strcasecmp(folder,"AIRBASE"))prefix="AirBase 99 ";
  else if(!strcasecmp(folder,"BASSST"))prefix="BassStation Rack ";
  else if(!strcasecmp(folder,"PULSE2"))prefix="Pulse 2 ";
  if(prefix){size_t n=strlen(prefix);if(!strncasecmp(name,prefix,n)&&name[n])memmove(name,name+n,strlen(name+n)+1);}
  if(!strcasecmp(folder,"AR")){
    const char*codes[]={"bd","sd","rs","cp","sy","bt","lt","mt","ht","ch","oh","hh","cy","cb","ut"};
    for(auto code:codes)if(!strncasecmp(name,code,2)&&name[2]==' '){name[0]=toupper(name[0]);name[1]=toupper(name[1]);break;}
  }
}
FLASHMEM void patchEntryAdd819(PatchEntry819*e){if(!e->directory)patchShortName20(e->name,patchDirectory07);auto**tail=&patchEntries819;while(*tail&&(!strcmp((*tail)->name,"parent")||((*tail)->directory>e->directory||((*tail)->directory==e->directory&&strcasecmp((*tail)->name,e->name)<=0))))tail=&(*tail)->next;e->next=*tail;*tail=e;}
FLASHMEM void patchList819(){
 while(patchEntries819){auto*e=patchEntries819;patchEntries819=e->next;extmem_free(e);}
 bool failed=false;
 if(strcmp(patchDirectory07,"/URBANKAOZ/PATCH")){auto*e=patch808Allocate<PatchEntry819>();if(e){e->directory=true;strcpy(e->name,"parent");strcpy(e->path,patchDirectory07);char*slash=strrchr(e->path,'/');if(slash)*slash=0;patchEntries819=e;}else failed=true;}
 if(sdAvailable){File dir=SD.open(patchDirectory07);
  if(dir&&dir.isDirectory())while(true){File f=dir.openNextFile();if(!f)break;
   char path[96];int length=snprintf(path,sizeof(path),"%s/%s",patchDirectory07,f.name());const char*ext=strrchr(path,'.');
   if(length<0||(size_t)length>=sizeof(path)||f.name()[0]=='.'){f.close();continue;}
   if(f.isDirectory()){
    auto*e=patch808Allocate<PatchEntry819>();if(!e){failed=true;f.close();break;}
    e->directory=true;snprintf(e->name,sizeof(e->name),"%.47s/",f.name());strcpy(e->path,path);patchEntryAdd819(e);
   }else if(ext&&!strcasecmp(ext,PSTR(".UKH"))){
    Item809 item;if(itemRead809(f,item,ITEM_PATCH809)){
     auto*e=patch808Allocate<PatchEntry819>();if(!e){failed=true;itemFree809(item);f.close();break;}
     strcpy(e->name,item.def->name);strcpy(e->path,path);
     for(auto*d=patchDb808.defs;d;d=d->next){
      bool same=itemSame809(d,item.def);const char*base=strrchr(path,'/');base=base?base+1:path;size_t n=strlen(base)-4;
      if(!same&&strlen(d->name)==n&&!strncasecmp(d->name,base,n)){PatchDef808 renamed=*d;strcpy(renamed.name,item.def->name);same=itemSame809(&renamed,item.def);if(same)strcpy(d->name,item.def->name);}
      if(!strcasecmp(d->source819,path)||same){e->id=d->id;strcpy(d->source819,path);strcpy(e->name,d->name);break;}
     }
     patchEntryAdd819(e);
    }itemFree809(item);
   }f.close();serviceNoteOffs();
  }dir.close();
 }
 for(auto*d=patchDb808.defs;d;d=d->next){bool found=false;for(auto*e=patchEntries819;e;e=e->next)if(e->id==d->id){found=true;break;}if(found)continue;
  if(d->source819[0]){const char*slash=strrchr(d->source819,'/');size_t n=slash?(size_t)(slash-d->source819):0;if(n!=strlen(patchDirectory07)||strncmp(d->source819,patchDirectory07,n))continue;}
  else if(strcmp(patchDirectory07,"/URBANKAOZ/PATCH"))continue;
  auto*e=patch808Allocate<PatchEntry819>();if(!e){failed=true;break;}e->id=d->id;strcpy(e->name,d->name);patchEntryAdd819(e);
 }
 if(patchWanted819)patchSelectId819(patchWanted819);
 patchCursor808=min(patchCursor808,patchListCount819()-1);patchScroll819=0;patchScrollCursor819=UINT32_MAX;
 if(failed)DISPLAY_SERIAL.println(PSTR("MSG|PATCH LIST MEMORY FULL"));
}
FLASHMEM bool patchEnter07(){auto*e=patchEntry819(patchCursor808);if(!e||!e->directory)return false;strcpy(patchDirectory07,e->path);patchCursor808=patchWanted819=0;patchList819();patch808Render();return true;}
FLASHMEM bool patchParent07(){if(patchUi808!=1)return false;char*slash=strrchr(patchDirectory07,'/');if(slash&&slash>patchDirectory07+10){*slash=0;patchCursor808=patchWanted819=0;patchList819();patch808Render();return true;}return false;}
FLASHMEM PatchDef808* patchSelected819(bool load){
 auto*e=patchEntry819(patchCursor808);if(!e||e->directory)return nullptr;
 if(e->id){auto*d=patch808Find(patchDb808,e->id);if(d)return d;e->id=0;}
 if(!load||!e->path[0])return nullptr;
 File f=SD.open(e->path,FILE_READ);Item809 item;bool ok=f&&itemRead809(f,item,ITEM_PATCH809);f.close();
 if(ok){auto*d=patchDb808.defs;while(d&&strcasecmp(d->source819,e->path)&&!itemSame809(d,item.def))d=d->next;
  if(!d){uint32_t count=patch808ParamCount(item.def);if(patchDb808.nextPatch==UINT32_MAX||count>=UINT32_MAX-patchDb808.nextParam)ok=false;
   else {d=item.def;item.def=nullptr;d->id=patchDb808.nextPatch++;for(auto*q=d->params;q;q=q->next)q->id=patchDb808.nextParam++;auto**tail=&patchDb808.defs;while(*tail)tail=&(*tail)->next;*tail=d;}
  }if(ok){e->id=d->id;strcpy(d->source819,e->path);}
 }itemFree809(item);
 if(ok)patchResolve10(patchDb808);
 if(!ok)DISPLAY_SERIAL.println(PSTR("MSG|PATCH READ ERROR"));return ok?patch808Find(patchDb808,e->id):nullptr;
}
FLASHMEM void patchListRender819(char (*rows)[21]){
 if(!rows){char a[21],b[21];snprintf(a,sizeof(a),PSTR("%.20s"),patchDeleteName819);snprintf(b,sizeof(b),PSTR("%.20s"),patchDeleteName819+min((size_t)20,strlen(patchDeleteName819)));
  patch808Frame(PSTR("DELETE PATCH / SD ?"),a,b,PSTR("Cancel       DELETE"));return;
 }
 uint32_t count=patchListCount819();patchCursor808=min(patchCursor808,count-1);
 if(patchScrollCursor819!=patchCursor808){patchScrollCursor819=patchCursor808;patchScroll819=0;patchScrollAt819=millis();}
 uint32_t first=patchCursor808?patchCursor808-1:0;
 for(uint8_t row=0;row<3&&first+row<count;++row){uint32_t index=first+row;auto*e=patchEntry819(index);const char*name=e?e->name:PSTR("NONE");
  bool current=index==patchCursor808;uint8_t offset=current?min((size_t)patchScroll819,strlen(name)):0;
  snprintf(rows[row],21,PSTR("%c%c%-18.18s"),current?'>':' ',patchAssigned811(e?e->id:0)&&(!e||e->id)?'*':' ',name+offset);
 }memcpy(rows[3],PSTR("New           Edit"),18);
}
FLASHMEM bool patchDelete819(){
 if(patchUi808==12)return true;
 if(patchUi808!=1)return false;
 auto*e=patchEntry819(patchCursor808);if(!e||e->directory){return true;}
 patchDeleteId819=e->id;strcpy(patchDeletePath819,e->path);strcpy(patchDeleteName819,e->name);patchUi808=12;patch808Render();return true;
}
FLASHMEM void patchDeleteConfirm819(bool confirm){
 if(confirm){
  if(patchDeletePath819[0]&&(!storageWriteAvailable||strncmp(patchDeletePath819,PSTR("/URBANKAOZ/PATCH/"),17)||strstr(patchDeletePath819,PSTR(".."))||!SD.remove(patchDeletePath819))){DISPLAY_SERIAL.println(PSTR("MSG|DELETE SD ERROR"));return;}
  auto*d=patch808Find(patchDb808,patchDeleteId819);
  if(d){for(auto&track:patchDb808.context)for(auto&c:track)if(patchHas819(c,d->id))patchAttach819(c,d->id,false);
   for(auto&slot:patchClipboard808)for(auto&c:slot)if(patchHas819(c,d->id))patchAttach819(c,d->id,false);
   auto**link=&patchDb808.defs;while(*link&&*link!=d)link=&(*link)->next;if(*link)*link=d->next;
   while(d->params){auto*q=d->params;d->params=q->next;extmem_free(q);}extmem_free(d);
  }patchEditing808=nullptr;patchParam808=nullptr;patchWanted819=0;patchPage808=0;++modEpoch805;patchList819();displayValueDirty=true;
 }
 patchUi808=1;patchDeleteId819=0;patchDeletePath819[0]=0;patch808Render();
}
FLASHMEM void patchListService819(){
 if(patchUi808!=1)return;auto*e=patchEntry819(patchCursor808);if(!e||strlen(e->name)<=18)return;
 if(millis()-patchScrollAt819<(patchScroll819?450u:1200u))return;
 patchScrollAt819=millis();patchScroll819=patchScroll819+18>=strlen(e->name)?0:patchScroll819+1;patch808Render();
}
