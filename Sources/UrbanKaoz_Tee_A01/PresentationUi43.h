#pragma once
// Dedicated preference3920..3923, separate from project data and note scale3912.
static uint8_t sleepMinutes43=2,sleepDraft43=2;
static bool sleepMenu43=false,sleepActive43=false;
static uint32_t sleepActivity43=0;
FLASHMEM void presentationTouch43(){sleepActivity43=millis();if(sleepActive43||millis()<7000){sleepActive43=false;DISPLAY_SERIAL.println("PRESENT43,0");}}
FLASHMEM void sleepLoad43(){uint32_t v=0;EEPROM.get(3920,v);if((v&0xffffff00)==0x43010000&&(v&255)<=60)sleepMinutes43=v&255;sleepActivity43=millis();}
FLASHMEM void sleepSave43(){sleepMinutes43=sleepDraft43;uint32_t v=0x43010000|sleepMinutes43;EEPROM.put(3920,v);sleepActivity43=millis();}
FLASHMEM void sleepRender43(){char line[21];if(sleepDraft43)snprintf(line,sizeof(line),"Inactivity %u min",sleepDraft43);else strcpy(line,"Automatic OFF");sendMenuScreen("SCR > SLP",line,"OK                  ","BACK            SHOW");settingsPageLastSendMs=millis();}
FLASHMEM void sleepShow43(){sleepActive43=true;DISPLAY_SERIAL.println("PRESENT43,2");}
FLASHMEM void sleepService43(){if(!sleepMinutes43||sleepActive43||sleepMenu43||fileBrowserMode!=FILE_BROWSER_NONE||saveMenuActive||loadTypeMenuActive)return;if((uint32_t)(millis()-sleepActivity43)>=(uint32_t)sleepMinutes43*60000)sleepShow43();}
