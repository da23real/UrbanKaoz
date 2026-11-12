#pragma once
#include <stdlib.h>
#include <errno.h>

FLASHMEM void controllerEmit940(const char*kind,int id,int value){if(controller940&&pc831.active&&Serial&&Serial.availableForWrite()>=48)Serial.printf("CTL940,%s,%d,%d\n",kind,id,value);}
FLASHMEM void controllerSet940(bool active){if(active!=controller940){if(active){playing=false;internalClockRunning=false;pidiStateDirty931=true;noInterrupts();pendingClockTicks=0;interrupts();synkozStop933();stopClipboardPreview(true,true);bool soft=softTransport903;softTransport903=false;allNotesOff();softTransport903=soft;modStop811();remixAudition911.active=false;stopHeld931=stopSent931=false;songHeldNeo=-1;}controller940=active;pidiStateDirty931=true;playHeld923=false;memset(clipboardPasteDown,0,sizeof(clipboardPasteDown));memset(controllerLeds940,0,sizeof(controllerLeds940));memset(kyBtnDown,0,sizeof(kyBtnDown));memset(kyHelpShown,0,sizeof(kyHelpShown));memset(neoPadDown,0,sizeof(neoPadDown));memset(selectPadPending,0,sizeof(selectPadPending));memset(transportMeasureWasPlaying,0,sizeof(transportMeasureWasPlaying));memset(transportMeasureRepeatAtMs,0,sizeof(transportMeasureRepeatAtMs));neoDisplayDirty=true;displayValueDirty=true;DISPLAY_SERIAL.println(active?"MSG|CONTROLEUR MONTAGE":"MSG|CONTROLEUR OFF");}if(Serial)Serial.printf("CTL940,MODE,%u,0\n",controller940);}

static uint32_t usbNotes901=0,usbOffs901=0,usbCC901=0;
static uint8_t usbCCChannel901=0,usbCCNumber901=0,usbCCValue901=0;

FLASHMEM void pcHc831(unsigned bit,bool down){
  uint64_t mask=1ULL<<bit;
  if(((pc831.hcDown&mask)!=0)==down)return;
  if(down)pc831.hcDown|=mask;else pc831.hcDown&=~mask;
  if(assignOpen932&&!controller940){if(down){for(uint8_t i=0;i<8;++i)if(SCREEN_BUTTON_BITS[i]==bit){if(i>=4)assignChoice932(i>=6);return;}if(!functionAllowed932(bit))return;assignButton932=bit;assignFunction932=buttonMap932[bit];assignLed932=buttonLed932[bit];assignConflict932=-1;assignRender932();}return;}
  bit=buttonMap932[bit];if(bit>=64)return;selectionIndicatorsPending931=true;
  hc165ProbeStable[bit]=hc165ProbeLastRaw[bit]=down;hc165ProbeRawChangedMs[bit]=millis();
  if(debugTestMode==DEBUG_TEST_BUTTONS)reportDebugButton(bit,down);
  else if(buttonSettingsLiveTest())reportLiveButtonSetting(bit,down);
  else if(down)dispatchStableButtonPress(bit);
  for(unsigned i=0;i<TRANSPORT_BUTTON_COUNT;++i)if(TRANSPORT_BUTTON_BITS[i]==bit){
    transportButtonStable[i]=transportButtonLastRaw[i]=down;transportButtonRawChangedMs[i]=millis();
    handleTransportButtonEdge(i,down);
  }
  for(unsigned i=0;i<CLIPBOARD_BUTTON_COUNT;++i)if(CLIPBOARD_BUTTON_BITS[i]==bit){
    clipboardButtonStable[i]=clipboardButtonLastRaw[i]=down;clipboardButtonRawChangedMs[i]=millis();
    handleClipboardButtonEdge(i,down);
  }
}
FLASHMEM void pcKy831(unsigned id,bool button,int value){
  uint16_t mask=1u<<id;
  if(button){if(((pc831.kyDown&mask)!=0)==(value!=0))return;if(value)pc831.kyDown|=mask;else pc831.kyDown&=~mask;}
  unsigned pico=id/8+1,index=id%8;
  auto saved=picoRxStats[pico-1]; // Preserve absolute-position recovery of physical encoders.
  picoRxStats[pico-1].valueKnown[index]=false;
  char packet[40];if(button)snprintf(packet,sizeof(packet),"P%u,B,%u,%d",pico,index,value);
  else snprintf(packet,sizeof(packet),"P%u,E,%u,0,%d",pico,index,value);
  pc831.injecting=true;handlePicoLine(pico,packet);pc831.injecting=false;
  picoRxStats[pico-1]=saved;
}
FLASHMEM void pcNeo831(unsigned id,bool down){
  uint64_t bit=1ULL<<id;if(((pc831.neoDown&bit)!=0)==down)return;
  if(down)pc831.neoDown|=bit;else pc831.neoDown&=~bit;
  pc831.injecting=true;onPadEdge(id/16,id%16,down?SEESAW_KEYPAD_EDGE_RISING:SEESAW_KEYPAD_EDGE_FALLING);pc831.injecting=false;
}
FLASHMEM void pcJoy831(){
  char packet[90];snprintf(packet,sizeof(packet),"DATA,Y1,0,0,%d,%d,%d,Y2,0,0,%d,%d,%d",pc831.joy[0],pc831.joy[1],pc831.joy[2],pc831.joy[3],pc831.joy[4],pc831.joy[5]);
  pc831.injecting=true;handleP23Line(packet);pc831.injecting=false;
}
FLASHMEM void pcRelease831(){
  numericId917=-1;
  for(uint8_t c=0;c<16;++c)if(modMidiHeld806[c])mod806UsbNoteOff(c+1,modMidiNote806[c],0);
  for(unsigned i=0;i<HC165_BIT_COUNT;++i)if(pc831.hcDown&(1ULL<<i))pcHc831(i,false);
  for(unsigned i=0;i<16;++i)if(pc831.kyDown&(1u<<i)){kyRotatedWhileDown[i]=true;pcKy831(i,true,0);}
  for(unsigned i=0;i<64;++i)if(pc831.neoDown&(1ULL<<i)){
    // A lost mouse-up cancels a pending short tap rather than creating a note.
    neoPadEdited[i/16][i%16]=true;selectPadPending[i/16][i%16]=false;pcNeo831(i,false);
  }
  for(unsigned i=0;i<4;++i)if(pc831.royDown&(1u<<i))royEdge831(3-i,false);
  pc831.royDown=0;memset(pc831.joy,0,sizeof(pc831.joy));pcJoy831();
}
FLASHMEM bool pcNumber831(const char*s,long&v){
  if(!s||!*s)return false;char*end;errno=0;v=strtol(s,&end,10);return !errno&&!*end;
}
FLASHMEM void pcAck831(uint32_t seq,const char*status){if(Serial&&Serial.availableForWrite()>=48)Serial.printf("PC1,ACK,%lu,%s\n",(unsigned long)seq,status);}
FLASHMEM bool pcCommand831(char*line){
  if(!strcmp(line,"USBSTAT")){
    uint16_t held=0;for(uint8_t c=0;c<16;++c)if(modMidiHeld806[c])held|=1u<<c;
    Serial.printf("USB901,NOTES=%lu,OFFS=%lu,CC=%lu,HELD=%u,CC_CHANNEL=%u,CC_NUMBER=%u,CC_VALUE=%u\n",usbNotes901,usbOffs901,usbCC901,held,usbCCChannel901,usbCCNumber901,usbCCValue901);return true;
  }
  if(strncmp(line,"PC1,",4))return false;
  if(!strcmp(line,"PC1,HELLO")){
    if(pc831.active)pcRelease831();controllerSet940(false);memset(&pc831,0,sizeof(pc831));numericId917=-1;pc831.active=true;pcNeoDirty931=15;pc831.seen=millis();
    Serial.printf("PC1,HELLO,%s,1\n",APP_VERSION);songSnapshot();neoDisplayDirty=true;return true;
  }
  char*parts[7];unsigned count=0;char*save=nullptr;
  for(char*t=strtok_r(line+4,",",&save);t&&count<7;t=strtok_r(nullptr,",",&save))parts[count++]=t;
  long seq,a=0,b=0,c=0;bool valid=count>=2&&pcNumber831(parts[0],seq)&&seq>0;
  if(!valid){pcAck831(0,"INVALID");return true;}
  if(!pc831.active){pcAck831(seq,"NO_SESSION");return true;}
  pc831.seen=millis();
  if((uint32_t)seq==pc831.lastSeq){pcAck831(seq,"OK");return true;}
  if((uint32_t)seq!=pc831.lastSeq+1){pcAck831(seq,"ORDER");return true;}
  const char*op=parts[1];valid=false;
  if(sdBusy905()&&strcmp(op,"PING")&&strcmp(op,"RELEASE")&&strcmp(op,"BYE")){pc831.lastSeq=seq;pcAck831(seq,"OK");return true;}
  if(controller940&&strcmp(op,"CTL940")&&strcmp(op,"CLED940")&&strcmp(op,"PING")&&strcmp(op,"RELEASE")&&strcmp(op,"BYE")&&strcmp(op,"HC")&&strcmp(op,"NEO")&&strcmp(op,"KT")&&strcmp(op,"KB")&&strcmp(op,"RT")&&strcmp(op,"RB")&&strcmp(op,"JOY")&&strcmp(op,"MASK")){pc831.lastSeq=seq;pcAck831(seq,"OK");return true;}
  if(count==3&&!strcmp(op,"CTL940")&&pcNumber831(parts[2],a)&&(a==0||a==1)){controllerSet940(a!=0);valid=true;}
  else if(count==4&&!strcmp(op,"CLED940")&&pcNumber831(parts[2],a)&&pcNumber831(parts[3],b)&&a>=0&&a<16&&b>=0&&b<=15){controllerLeds940[a]=b;neoDisplayDirty=true;valid=true;}
  else if(!strcmp(op,"MA28")&&count==3){char cmd[24];snprintf(cmd,sizeof(cmd),"MA28,%s",parts[2]);valid=analyzerCommand28(cmd);}else if(synkozCommand933(op,count,count>2?parts[2]:nullptr))valid=true;
  else if(count==2&&!strcmp(op,"PING"))valid=true;
  else if(count==3&&!strcmp(op,"VGET")&&pcNumber831(parts[2],a)&&a>=0&&a<20){numericRead917(a);valid=true;}
  else if(count==5&&!strcmp(op,"VSET")&&pcNumber831(parts[2],a)&&pcNumber831(parts[3],b)&&pcNumber831(parts[4],c)&&a>=0&&a<20&&b>=-3599999&&b<=3599999&&c>0){numericWrite917(a,b,c);valid=true;}
  else if(count==5&&pcNumber831(parts[2],a)&&pcNumber831(parts[3],b)&&pcNumber831(parts[4],c)
          &&a>=1&&a<=16&&b>=0&&b<=127&&c>=0&&c<=127){
    if(!strcmp(op,"NOTE")){midiInOn17(a,b,c);++usbNotes901;valid=true;}
    else if(!strcmp(op,"OFF")){midiInOff17(a,b,c);++usbOffs901;valid=true;}
    else if(!strcmp(op,"CC")){++usbCC901;usbCCChannel901=a;usbCCNumber901=b;usbCCValue901=c;{MidiInputScope53 scope53(1);ccReceive941(a-1,b,c);}valid=true;}
  }
  else if(count==2&&(!strcmp(op,"RELEASE")||!strcmp(op,"BYE"))){pcRelease831();valid=true;if(!strcmp(op,"BYE")){controllerSet940(false);pc831.active=false;pc831.hcMask=pc831.neoMask=0;pc831.kyMask=pc831.royMask=pc831.joyMask=0;pc831.keyMask=false;}}
  else if(count==4&&pcNumber831(parts[2],a)&&pcNumber831(parts[3],b)){
    if(!strcmp(op,"HC")&&a>=0&&a<HC165_BIT_COUNT&&(b==0||b==1)){pcHc831(a,b);valid=true;}
    else if(!strcmp(op,"NEO")&&a>=0&&a<64&&(b==0||b==1)){pcNeo831(a,b);valid=true;}
    else if(!strcmp(op,"KB")&&a>=0&&a<16&&(b==0||b==1)){pcKy831(a,true,b);valid=true;}
    else if(!strcmp(op,"KT")&&a>=0&&a<16&&b>=-16&&b<=16&&b){pcKy831(a,false,b);valid=true;}
    else if(!strcmp(op,"RB")&&a>=0&&a<4&&(b==0||b==1)){
      uint8_t mask=1u<<a;if(((pc831.royDown&mask)!=0)!=(b!=0)){if(b)pc831.royDown|=mask;else pc831.royDown&=~mask;royEdge831(3-a,b);}valid=true;
    }else if(!strcmp(op,"RT")&&a>=0&&a<4&&b>=-16&&b<=16&&b){royTurn831(3-a,b,0,b);valid=true;}
  }else if(count==3&&!strcmp(op,"KEY")&&strlen(parts[2])==1&&strchr("0123456789ABCD*#",parts[2][0])){
    char key[6]={'K','E','Y',',',parts[2][0],0};pc831.injecting=true;handleDisplayInputLine(key);pc831.injecting=false;valid=true;
  }else if(count==5&&!strcmp(op,"JOY")&&pcNumber831(parts[2],a)&&pcNumber831(parts[3],b)&&pcNumber831(parts[4],c)&&a>=0&&a<2&&b>=0&&b<3&&c>=-512&&c<=512&&(b!=2||c==0||c==1)){
    pc831.joy[a*3+b]=c;pcJoy831();valid=true;
  }else if(count==5&&!strcmp(op,"MASK")&&pcNumber831(parts[3],a)&&pcNumber831(parts[4],b)&&a>=0&&(b==0||b==1)){
    const char*kind=parts[2];
    if(!strcmp(kind,"HC")&&a<HC165_BIT_COUNT){uint64_t m=1ULL<<a;pc831.hcMask=b?pc831.hcMask|m:pc831.hcMask&~m;valid=true;}
    else if(!strcmp(kind,"NEO")&&a<64){uint64_t m=1ULL<<a;
      if(b&&neoPadDown[a/16][a%16]&&!(pc831.neoDown&m)){neoPadEdited[a/16][a%16]=true;selectPadPending[a/16][a%16]=false;pc831.injecting=true;onPadEdge(a/16,a%16,SEESAW_KEYPAD_EDGE_FALLING);pc831.injecting=false;}
      pc831.neoMask=b?pc831.neoMask|m:pc831.neoMask&~m;valid=true;}
    else if(!strcmp(kind,"KY")&&a<16){uint16_t m=1u<<a;
      if(b&&kyBtnDown[a]&&!(pc831.kyDown&m)){kyBtnDown[a]=false;kyRotatedWhileDown[a]=kyHelpShown[a]=false;sendKyHelp(a,false);}
      pc831.kyMask=b?pc831.kyMask|m:pc831.kyMask&~m;valid=true;}
    else if(!strcmp(kind,"ROY")&&a<4){uint8_t m=1u<<a;pc831.royMask=b?pc831.royMask|m:pc831.royMask&~m;valid=true;}
    else if(!strcmp(kind,"JOY")&&a<2){uint8_t m=1u<<a;pc831.joyMask=b?pc831.joyMask|m:pc831.joyMask&~m;valid=true;}
    else if(!strcmp(kind,"KEY")&&a==0){pc831.keyMask=b;valid=true;}
  }
  if(valid)pc831.lastSeq=seq;pcAck831(seq,valid?"OK":"INVALID");return true;
}
FLASHMEM void pcService831(){
  if(!pc831.active)return;
  uint32_t now=millis();
  if(!Serial||now-pc831.seen>2500){pcRelease831();controllerSet940(false);memset(&pc831,0,sizeof(pc831));return;}
  if(now-pc831.joyAt>=35){pc831.joyAt=now;if(pc831.joy[0]||pc831.joy[1]||pc831.joy[2]||pc831.joy[3]||pc831.joy[4]||pc831.joy[5])pcJoy831();}
  // One panel per 40 ms, whole packets only; no wait for USB buffer space.
  if(!pcNeoDirty931||now-pc831.telemetryAt<40||Serial.availableForWrite()<150)return;
  pc831.telemetryAt=now;unsigned n=0;while(!(pcNeoDirty931&(1u<<n)))++n;pcNeoDirty931&=~(1u<<n);
  char out[140];int at=snprintf(out,sizeof(out),"PC1,LED,%u",n);
  for(unsigned p=0;p<16;++p)at+=snprintf(out+at,sizeof(out)-at,",%06lX",(unsigned long)(neoColors821[n][p]&0xFFFFFF));
  out[at++]='\n';Serial.write((const uint8_t*)out,at);
}
