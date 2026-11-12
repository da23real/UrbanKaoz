#pragma once
// SD library and musical project have separate lifetimes. No writes in this file.
EXTMEM PatchDb808 patchCatalog10={},patchCatalogStage10={};
uint32_t patchCatalogFiles10=0,patchCatalogErrors10=0,patchCatalogScans10=0;
FLASHMEM void patchGeneric10(PatchParam808&q){
 strcpy(q.name,"MIDI");if(q.number<1000)snprintf(q.shortName,4,"%03u",q.number);else strcpy(q.shortName,"NRP");
}
FLASHMEM PatchDef808* patchLibraryFile10(const char*path){
 if(!path[0])return nullptr;
 for(auto*d=patchCatalog10.defs;d;d=d->next)if(!strcasecmp(d->source819,path))return d;
 return nullptr;
}
FLASHMEM void patchResolve10(PatchDb808&db){
 for(auto*d=db.defs;d;d=d->next){
  // New, unsaved library edits stay editable during this session. Imported
  // project labels (including legacy embedded definitions) are never trusted.
  if(!d->source819[0] && &db!=&patchStage808)continue;
  auto*lib=patchLibraryFile10(d->source819);strcpy(d->name,lib?lib->name:"MIDI");
  for(auto*q=d->params;q;q=q->next){patchGeneric10(*q);for(auto*p=lib?lib->params:nullptr;p;p=p->next)if(patchSameAddress819(*q,*p)){strcpy(q->name,p->name);strcpy(q->shortName,p->shortName);break;}}
  // Additional destinations in a newer external patch are available immediately;
  // existing IDs, values and MIDI addresses remain stable.
  for(auto*p=lib?lib->params:nullptr;p;p=p->next){bool found=false;auto**tail=&d->params;
   while(*tail){if(patchSameAddress819(**tail,*p))found=true;tail=&(*tail)->next;}
   if(!found&&db.nextParam!=UINT32_MAX){auto*q=patch808Allocate<PatchParam808>();if(!q){++patchCatalogErrors10;break;}*q=*p;q->next=nullptr;q->id=db.nextParam++;*tail=q;}
  }
 }
}
FLASHMEM void patchCatalogDirectory10(PatchDb808&stage,const char*path,uint8_t depth){
 File dir=SD.open(path,FILE_READ);if(!dir||!dir.isDirectory()){++patchCatalogErrors10;return;}
 while(true){File f=dir.openNextFile();if(!f)break;char full[96];int n=snprintf(full,sizeof(full),"%s/%s",path,f.name());
  if(f.name()[0]=='.'){f.close();continue;}
  if(n<0||n>=(int)sizeof(full)){++patchCatalogErrors10;f.close();continue;}
  if(f.isDirectory()){f.close();if(depth<12)patchCatalogDirectory10(stage,full,depth+1);else ++patchCatalogErrors10;continue;}
  const char*ext=strrchr(full,'.');
  if(ext&&!strcasecmp(ext,".UKH")){Item809 item;if(itemRead809(f,item,ITEM_PATCH809)){
    auto*d=item.def;item.def=nullptr;strcpy(d->source819,full);d->next=stage.defs;stage.defs=d;++patchCatalogFiles10;
   }else ++patchCatalogErrors10;itemFree809(item);}
  f.close();serviceNoteOffs();
 }dir.close();
}
FLASHMEM void patchCatalogScan10(){
 // Teensy EXTMEM is NOLOAD: its zero initializer is not a boot initialization.
 static bool initialized=false;if(!initialized){memset(&patchCatalog10,0,sizeof(patchCatalog10));memset(&patchCatalogStage10,0,sizeof(patchCatalogStage10));initialized=true;}
 ++patchCatalogScans10;patchCatalogFiles10=patchCatalogErrors10=0;
 // Only one pointer is required for the stage: avoid a large PatchDb on the stack.
 auto&stage=patchCatalogStage10;patch808Free(stage);
 if(sdAvailable)patchCatalogDirectory10(stage,"/URBANKAOZ/PATCH",0);
 patch808Free(patchCatalog10);patchCatalog10.defs=stage.defs;stage.defs=nullptr;
 if(patchCatalogErrors10)DISPLAY_SERIAL.println("MSG|PATCH READ ERROR");
}

FLASHMEM void patchStats10(){
 Serial.printf("PATCH10,FILES=%lu,ERRORS=%lu,SCANS=%lu,DEFS=%lu\n",(unsigned long)patchCatalogFiles10,(unsigned long)patchCatalogErrors10,(unsigned long)patchCatalogScans10,(unsigned long)patch808Count());
 for(auto*d=patchDb808.defs;d;d=d->next)Serial.printf("PATCHREF10,ID=%lu,FILE=%s,NAME=%s\n",(unsigned long)d->id,d->source819,d->name);
}
