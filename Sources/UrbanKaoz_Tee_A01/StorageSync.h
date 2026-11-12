#pragma once
FLASHMEM bool storageNameValid(const char *s){
  if(strlen(s)>16)return false;
  for(;*s;++s)if(!((*s>='A'&&*s<='Z')||(*s>='a'&&*s<='z')||(*s>='0'&&*s<='9')||*s=='_'||*s=='-'||*s==' '))return false;
  return true;
}
FLASHMEM const char *storageTypeLabel(){
  if(itemKind809)return itemLabel809();
  if(fileBrowserMode==FILE_BROWSER_SAVE)return pendingSaveCandidate==SAVE_CANDIDATE_FULL?"FULL":fileBrowserExtension;
  return activeStorageType==STORAGE_FULL?"FULL":activeStorageType==STORAGE_PATTERN?"PATTERN":activeStorageType==STORAGE_SEQUENCE?"SEQUENCE":"NOTE";
}
FLASHMEM void storageSync(bool force){
  static uint8_t prior=255;static char previous[150]="",priorType[20]="",priorPath[64]="";static uint32_t last=0,blink=0;
  uint8_t mode=fileBrowserMode;
  if(mode==FILE_BROWSER_SAVE&&millis()-blink>400){blink=millis();storageBlink931=true;renderFileBrowser();storageBlink931=false;}
  if(!force&&!storageDirty931)return;storageDirty931=false;
  const char *type=mode?storageTypeLabel():"",*path=mode?fileBrowserPath:"";
  if(prior!=mode||strcmp(priorType,type)||strcmp(priorPath,path)){++storageSession;prior=mode;copyField(priorType,sizeof(priorType),type);copyField(priorPath,sizeof(priorPath),path);}
  storageNameCursor=min((size_t)storageNameCursor,strlen(fileBrowserName));
  char line[150];snprintf(line,sizeof(line),"STORAGE,%lu,%u,%s,%s,%u,%s,%s,%u",(unsigned long)storageSession,mode,mode?storageTypeLabel():"",mode==FILE_BROWSER_SAVE?fileBrowserName:"",storageNameCursor,mode?fileBrowserPath:"",mode?fileBrowserEntry:"",patternLoadConfirmActive);
  if(force||strcmp(previous,line)){copyField(previous,sizeof(previous),line);songSend("%s",line);last=millis();}

}
FLASHMEM void storageNameKey(char key){if(saveCollision905)return;
  if(!((key>='0'&&key<='9')||(key>='A'&&key<='Z')||key=='_'||key=='-'||key==' '))return;
  nameLetter12(key);size_t n=strlen(fileBrowserName);storageNameCursor=min((size_t)storageNameCursor,n);
  if(storageNameCursor>=16)return;if(storageNameCursor==n)fileBrowserName[n+1]=0;
  fileBrowserName[storageNameCursor++]=key;renderFileBrowser();storageSync(true);
}
FLASHMEM void storageNameDelete(){if(saveCollision905)return;nameProposed12=false;
  size_t n=strlen(fileBrowserName);if(storageNameCursor<n)memmove(fileBrowserName+storageNameCursor,fileBrowserName+storageNameCursor+1,n-storageNameCursor);
  else if(n){fileBrowserName[n-1]=0;storageNameCursor=n-1;}
  renderFileBrowser();storageSync(true);
}
FLASHMEM void storageNameTurn(uint8_t encoder,int delta){if(saveCollision905)return;
  const char chars[]="ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-_ ";
  size_t n=strlen(fileBrowserName);
  if(encoder==1){cycleBrowserFile(delta>0?1:-1);if(fileBrowserEntry[0]){char proposed[17];copyField(proposed,sizeof(proposed),fileBrowserEntry);char*dot=strrchr(proposed,'.');if(dot)*dot=0;namePropose12(proposed);}renderFileBrowser();storageSync(true);return;}
  if(encoder==0)storageNameCursor=clampInt((int)storageNameCursor+delta,0,n);
  else if(storageNameCursor<16){const char *found=storageNameCursor<n?strchr(chars,fileBrowserName[storageNameCursor]):nullptr;
    int index=found?found-chars:(int)sizeof(chars)-2;const int count=sizeof(chars)-1;index=clampInt(index+delta,0,count-1);
    nameLetter12(chars[index]);n=strlen(fileBrowserName);
    fileBrowserName[storageNameCursor]=chars[index];if(storageNameCursor==n)fileBrowserName[n+1]=0;
  }
  renderFileBrowser();storageSync(true);
}
FLASHMEM bool storageCommand(char *line){
  if(!strcmp(line,"STORAGE?")){storageSync(true);return true;}
  if(strncmp(line,"STORAGE,",8))return false;
  storageSync(false); // Refresh the context before accepting a queued command.
  char *action=strtok(line+8,","),*sid=strtok(nullptr,",");
  if(!action||!sid||!fileBrowserMode||strtoul(sid,nullptr,10)!=storageSession){songError("STALE_STORAGE_CONTEXT");storageSync(true);return true;}
  if(!strcmp(action,"NAME")&&fileBrowserMode==FILE_BROWSER_SAVE){if(saveCollision905)return true;char *cursor=strtok(nullptr,","),*name=strtok(nullptr,",");if(!name)name=(char*)"";
    if(!cursor||!storageNameValid(name)){songError("INVALID_FILE_NAME");return true;}
    nameProposed12=false;copyField(fileBrowserName,sizeof(fileBrowserName),name);storageNameCursor=min(strlen(name),(size_t)strtoul(cursor,nullptr,10));renderFileBrowser();
  }else if(!strcmp(action,"PICK")&&fileBrowserMode==FILE_BROWSER_LOAD){char *direction=strtok(nullptr,",");cycleBrowserFile(direction&&atoi(direction)<0?-1:1);renderFileBrowser();
  }else if(!strcmp(action,"COMMIT")){
    if(playing&&!itemKind809){songError("STOP_BEFORE_STORAGE");return true;}
    if(fileBrowserMode==FILE_BROWSER_SAVE){if(!fileBrowserName[0]){songError("FILE_NAME_REQUIRED");return true;}executeBrowserSave();}
    else if(patternLoadConfirmActive)answerPatternLoadConfirmation(true);
    else handleFileBrowserScreenButton(fileBrowserDirectoryPage>0?1:0);
  }else if(!strcmp(action,"CANCEL")){patternLoadConfirmActive=false;closeFileBrowser();}
  else songError("INVALID_STORAGE_ACTION");
  storageSync(true);return true;
}
