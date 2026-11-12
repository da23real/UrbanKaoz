#pragma once
// No project data, EEPROM or SD writes. Transport stays stopped after leaving.
FLASHMEM void pinTestStatus33(){Serial.printf("PINTEST33,ACTIVE=%u,OUT=%u,PIN=%u,GP=%u,HIGH=%u,TRAIL=%u\n",pinTestActive33,pinTestPort33+1,pinTestPort33<4?pinTestPort33+4:pinTestPort33+5,pinTestPort33+2,pinTestHigh33,headTrail25);}
FLASHMEM void pinTestRender33(){char title[21],line[21];snprintf(title,sizeof(title),"PIN TEST OUT %u",pinTestPort33+1);snprintf(line,sizeof(line),"%u (GP%u) CMD %s",pinTestPort33<4?pinTestPort33+4:pinTestPort33+5,pinTestPort33+2,pinTestHigh33?"HAUT":"BAS");sendMenuScreen(title,line,"OA:PORT  BAS/HAUT 3s","MIDI DEBRANCHE J1:FIN");}
FLASHMEM bool pinTestSend33(uint8_t port,bool high){uint8_t data[]={port,(uint8_t)high};return pidiPacket827(pidi827::PINTEST33,data,sizeof(data));}
FLASHMEM void pinTestStart33(){
 if(pinTestActive33)return;
 playing=internalClockRunning=false;noInterrupts();pendingClockTicks=0;interrupts();
 allNotesOff();pidiCancelDelays924();remixAudition911.active=false;
 pinTestPort33=0;pinTestHigh33=true;pinTestPhase33=millis();pinTestSent33=millis()-250;
 pinTestActive33=true;settingsOpen=true;settingsPage=4;settingsMenuLevel=SETTINGS_ITEMS;settingsPageRetryPending=false;
 pidiEnsure827();pinTestService33();pinTestRender33();
}
FLASHMEM void pinTestStop33(){
 if(!pinTestActive33)return;
 pinTestSend33(255,true);pinTestActive33=false;pinTestHigh33=true;
 // Re-send runtime configuration, without loading or restoring a project.
 pidiReset827=true;pidiEnsure827();sendSettingsPage();
}
FLASHMEM void pinTestTurn33(int32_t delta){
 uint8_t next=clampInt((int)pinTestPort33+delta,0,7);if(next==pinTestPort33)return;
 pinTestPort33=next;pinTestHigh33=true;pinTestPhase33=millis();pinTestSent33=millis()-250;
 pinTestService33();pinTestRender33();
}
FLASHMEM void pinTestService33(){
 if(!pinTestActive33)return;
 if(!settingsOpen){pinTestStop33();return;}
 uint32_t now=millis();if((uint32_t)(now-pinTestPhase33)>=3000){pinTestPhase33=now;pinTestHigh33=!pinTestHigh33;pinTestRender33();}
 if((uint32_t)(now-pinTestSent33)>=250){pidiEnsure827();if(pinTestSend33(pinTestPort33,pinTestHigh33))pinTestSent33=now;}
}
