#pragma once
#include "ConversionRules.h"
// Reserved existing system locations cannot be renamed/deleted by the browser.
FLASHMEM bool sdProtected(const char *path) {
  const char *reserved[]={"/URBANKAOZ","/URBANKAOZ/PRJ","/URBANKAOZ/PAT","/URBANKAOZ/SEQ","/URBANKAOZ/NPX","/URBANKAOZ/NPL","/URBANKAOZ/CFG","/URBANKAOZ/SONG","/URBANKAOZ/PATCH","/URBANKAOZ/PRESET","/URBANKAOZ/ENV","/URBANKAOZ/LFO","/URBANKAOZ/FX"};
  for(auto p:reserved)if(!strcasecmp(path,p))return true;
  return false;
}
FLASHMEM bool sdBrowseCommand(char *line) {
  if(strncmp(line,"FS,",3))return false;
  if(!sdAvailable||(playing&&!itemKind809)){songError("STOP_AND_CHECK_SD");return true;}
  char *action=strtok(line+3,","),*path=strtok(nullptr,","),*arg=strtok(nullptr,",");
  if(!action||!path||!sdFolderPathValid(path)){songError("INVALID_FOLDER_PATH");return true;}
  File dir=SD.open(path,FILE_READ);
  if(!dir||!dir.isDirectory()){if(dir)dir.close();songError("FOLDER_NOT_FOUND");return true;}
  if(!strcmp(action,"LIST")) {
    unsigned skip=arg?strtoul(arg,nullptr,10):0,index=0,sent=0;bool more=false;
    songSend("FSBEGIN,%s,%u",path,skip);
    while(true){File f=dir.openNextFile();if(!f)break;
      if(f.isDirectory()) {const char *name=f.name();const char *slash=strrchr(name,'/');if(slash)name=slash+1;
        char full[64];snprintf(full,sizeof(full),"%s/%s",path,name);
        if(sdFolderPathValid(full)){if(index++>=skip){if(sent==12){more=true;f.close();break;}songSend("FSDIR,%s",name);++sent;}}}
      f.close();
    }
    dir.close();songSend("FSEND,%u",more?skip+sent:0);return true;
  }
  dir.close();
  if(!strcmp(action,"SELECT")) {
    copyField(selectedSdFolder,sizeof(selectedSdFolder),path);copyField(songFolder,sizeof(songFolder),path);songDiskGeneration=0;
    if(fileBrowserMode!=FILE_BROWSER_NONE){copyField(fileBrowserPath,sizeof(fileBrowserPath),path);copyField(fileBrowserRootPath,sizeof(fileBrowserRootPath),path);fileBrowserEntryIndex=0;selectBrowserFile(0);renderFileBrowser();}
    songSend("FSSELECTED,%s",path);return true;
  }
  if(!storageWriteAvailable){songError("SD_NOT_WRITABLE");return true;}
  if(!strcmp(action,"MKDIR")) {
    if(!arg||!*arg||strchr(arg,'/')){songError("INVALID_FOLDER_NAME");return true;}
    char next[80];snprintf(next,sizeof(next),"%s/%s",path,arg);
    if(!sdFolderPathValid(next)||SD.exists(next)||!SD.mkdir(next))songError("CREATE_FOLDER_FAILED");else songSend("FSOK,CREATED");return true;
  }
  // Never move a system directory, or the current destination or its ancestors.
  size_t plen=strlen(path);
  bool selectedInside=!strncasecmp(path,selectedSdFolder,plen)&&(selectedSdFolder[plen]==0||selectedSdFolder[plen]=='/');
  bool songInside=!strncasecmp(path,songFolder,plen)&&(songFolder[plen]==0||songFolder[plen]=='/');
  if(sdProtected(path)||selectedInside||songInside){songError("PROTECTED_FOLDER");return true;}
  if(!strcmp(action,"REMOVE")){
    dir=SD.open(path);File child=dir.openNextFile();bool empty=!child;if(child)child.close();dir.close();
    if(!empty){songError("FOLDER_NOT_EMPTY");return true;}
    if(SD.rmdir(path))songSend("FSOK,REMOVED");else songError("REMOVE_FOLDER_FAILED");return true;
  }
  if(!strcmp(action,"RENAME")) {
    if(!arg||!*arg||strchr(arg,'/')){songError("INVALID_FOLDER_NAME");return true;}
    char next[80];copyField(next,sizeof(next),path);char *last=strrchr(next,'/');if(!last){songError("PROTECTED_FOLDER");return true;}*(last+1)=0;
    if(strlen(next)+strlen(arg)>=sizeof(next)){songError("INVALID_FOLDER_NAME");return true;}strcat(next,arg);
    if(!sdFolderPathValid(next)||SD.exists(next)||!SD.rename(path,next))songError("RENAME_FOLDER_FAILED");else songSend("FSOK,RENAMED");return true;
  }
  songError("UNKNOWN_FS_COMMAND");return true;
}
