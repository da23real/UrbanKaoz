#pragma once
struct Settings816 {uint32_t magic;uint16_t ky23,checksum;};
static const int settingsAddress816=settingsAddress811+sizeof(Settings811);
FLASHMEM void settingsSave816(){Settings816 s={0x8164254u,ky23Debounce816,(uint16_t)(ky23Debounce816^0x816)};EEPROM.put(settingsAddress816,s);}
FLASHMEM void settingsLoad816(){
 Settings816 s={};EEPROM.get(settingsAddress816,s);
 if(s.magic==0x8164254u&&s.ky23<=120&&s.checksum==(uint16_t)(s.ky23^0x816)){ky23Debounce816=s.ky23;return;}
 // One-time migration: requested 15 ms defaults, preserve unrelated settings.
 hc165PressDebounceMs=hc165ReleaseDebounceMs=roySwitchDebounceMs=picoButtonDebounceMs=ky23Debounce816=15;
 settingsValue[0][1]=settingsValue[0][2]=15;
 saveSettingsToEEPROM();settingsSave816();
}
FLASHMEM void settingsOpenKy23816(){patchUi808=9;settingsPageRetryPending=false;configDraft811[0]=ky23Debounce816;settingsRender811();}
FLASHMEM void rememberFileName816(const char*path){
 const char*base=strrchr(path,'/');base=base?base+1:path;
 size_t length=strlen(base);const char*dot=strrchr(base,'.');if(dot)length=dot-base;
 length=min(length,sizeof(lastFileName816)-1);memcpy(lastFileName816,base,length);lastFileName816[length]=0;
}
