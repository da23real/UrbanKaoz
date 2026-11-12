#pragma once
#include "ScreenMapCore.h"
// Dedicated preferences, 3900..3907, outside existing settings records.
bool screenMapMenu=false;
uint8_t screenMapRoles[4]={2,1,3,4},screenMapOld[4],screenMapSelected=0;
void sendSettingsPage();
FLASHMEM void screenMapSend(){DISPLAY_SERIAL.printf("SCRMAP,%u,%u,%u,%u\n",screenMapRoles[0],screenMapRoles[1],screenMapRoles[2],screenMapRoles[3]);}
FLASHMEM void screenMapLoad(){uint8_t b[8];EEPROM.get(3900,b);if(b[0]==0x53&&b[1]==0x4D&&b[7]==1&&scrmap::valid(b+2)&&b[6]==scrmap::checksum(b+2))memcpy(screenMapRoles,b+2,4);}
FLASHMEM void screenMapSave(){uint8_t b[8]={0x53,0x4D,screenMapRoles[0],screenMapRoles[1],screenMapRoles[2],screenMapRoles[3],scrmap::checksum(screenMapRoles),1};EEPROM.put(3900,b);}
FLASHMEM void screenMapRender(){char title[21],value[21],role[4];uint8_t r=screenMapRoles[screenMapSelected];if(r)snprintf(role,sizeof(role),"E%u",r);else strcpy(role,"OFF");snprintf(title,sizeof(title),"SCR > MAP %s",scrmap::anyDuplicate(screenMapRoles)?"!":"");snprintf(value,sizeof(value),"I2C 0x%02X -> %s%s",scrmap::address[screenMapSelected],role,scrmap::duplicate(screenMapRoles,screenMapSelected)?" !":"");sendMenuScreen(title,value,"OA:I2C OC:E1..4/OFF","Clic OC OK J1 annul");DISPLAY_SERIAL.printf("SCRIDENT,%u\n",screenMapSelected);settingsPageLastSendMs=millis();}
FLASHMEM void screenMapOpen(){memcpy(screenMapOld,screenMapRoles,4);screenMapSelected=0;screenMapMenu=true;screenMapSend();screenMapRender();}
FLASHMEM void screenMapClose(bool commit){if(commit)screenMapSave();else memcpy(screenMapRoles,screenMapOld,4);screenMapMenu=false;DISPLAY_SERIAL.println("SCRIDENT,255");screenMapSend();sendSettingsPage();}
FLASHMEM void screenMapTurn(uint8_t roy,int delta){if(!delta)return;int d=delta>0?1:-1;if(roy==0)screenMapSelected=(screenMapSelected+4+d)%4;else if(roy==2){screenMapRoles[screenMapSelected]=(screenMapRoles[screenMapSelected]+5+d)%5;screenMapSend();}else return;screenMapRender();}
