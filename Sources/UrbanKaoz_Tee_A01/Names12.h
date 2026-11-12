#pragma once
FLASHMEM void namePropose12(const char* text){if(text!=fileBrowserName)copyField(fileBrowserName,sizeof(fileBrowserName),text&&text[0]?text:"default");storageNameCursor=strlen(fileBrowserName);nameProposed12=true;}
FLASHMEM void nameLetter12(char c){(void)c;nameProposed12=false;}
