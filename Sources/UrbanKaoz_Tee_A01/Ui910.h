#pragma once
FLASHMEM bool beginFullBusy910(bool loading){
 if(fullBusy910)return false;
 fullBusy910=true;busyAck910=0;if(!++busyToken910)++busyToken910;
 DISPLAY_SERIAL.printf("BUSY910,START,%lu,%s\n",(unsigned long)busyToken910,loading?"LOAD":"SAVE");
 uint32_t at=millis();
 while(busyAck910!=busyToken910&&millis()-at<2500){display821.service();serviceDisplayInput();serviceNoteOffs();pidiService827();}
 if(busyAck910==busyToken910)return true;
 endFullBusy910();return false;
}
FLASHMEM void endFullBusy910(){
 if(!fullBusy910)return;
 DISPLAY_SERIAL.printf("BUSY910,END,%lu\n",(unsigned long)busyToken910);
 fullBusy910=false;
}
