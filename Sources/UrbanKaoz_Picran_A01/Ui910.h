#pragma once
// Dedicated small-screen overlay wins over every cached Global/QV renderer.
bool remixVisible910=false,busyVisible910=false,busyLoading17=false;
char remixRows910[4][2][17]={};
uint32_t busyToken910=0,busyAt910=0,busyFrameAt910=0;
uint8_t busyFrame910=0;
bool renderBusy910(bool force){
 if(!busyVisible910)return false;
 mainPrintLine(0,busyLoading17?"CHARGEMENT FULL":"SAUVEGARDE FULL",force);
 mainPrintLine(1,busyLoading17?"Je sors le bazar...":"Je range le bazar...",force);
 const char*faces[]={"    \\(^_^)/", "     (^_^)>", "    /(^_^)\\", "    <(^_^)"};
 mainPrintLine(2,faces[busyFrame910%4],force);
 mainPrintLine(3,"Un instant, maestro!",force);
 return true;
}
void serviceBusy910(){
 if(!busyVisible910)return;
 if(millis()-busyFrameAt910>=220){busyFrameAt910=millis();++busyFrame910;renderMask931|=16;displayDirty=true;}
}
bool parseUi910(char*line){
 if(!strncmp(line,"HELLO,",6)){busyVisible910=remixVisible910=false;displayDirty=true;return false;}
 if(!strcmp(line,"RMX910,OFF")){remixVisible910=false;displayDirty=true;return true;}
 if(!strncmp(line,"RMX910|",7)){
  if(strlen(line)!=42||line[7]<'0'||line[7]>'3'||line[8]!='|'||line[25]!='|')return true;
  uint8_t i=line[7]-'0';memcpy(remixRows910[i][0],line+9,16);memcpy(remixRows910[i][1],line+26,16);remixRows910[i][0][16]=remixRows910[i][1][16]=0;
  remixVisible910=true;displayDirty=true;return true;
 }
 if(!strncmp(line,"BUSY910,START,",14)){
  char* mode=nullptr;busyToken910=strtoul(line+14,&mode,10);busyLoading17=mode&&!strcmp(mode,",LOAD");busyAt910=busyFrameAt910=millis();busyFrame910=0;busyVisible910=true;
  renderBusy910(true); // Physical LCD writes complete before the acknowledgement.
  if(mainLcdPresent)TEE_SERIAL.printf("BUSY910,READY,%lu\n",(unsigned long)busyToken910);
  return true;
 }
 if(!strncmp(line,"BUSY910,END,",12)){
  if(strtoul(line+12,nullptr,10)==busyToken910){busyVisible910=false;displayDirty=true;}
  return true;
 }
 return false;
}
