#pragma once
void nameDeleteR8();
// Small LCD protocol already supported by Picran; no display firmware change.
FLASHMEM bool nameDisplayR8(){
  static bool previous=false;static char cache[4][2][17]={};
  bool typing=fileBrowserMode==FILE_BROWSER_SAVE||patchUi808==4;
  if(loadTypeMenuActive||fileBrowserMode==FILE_BROWSER_LOAD)return true;
  if(!typing){if(previous){previous=false;memset(cache,0,sizeof(cache));DISPLAY_SERIAL.println("CUSTOM,OFF");displayValueDirty=true;}return false;}
  previous=true;
  const char* top[]={"A   B   C   D","E   F   G   H","I   J   K   L","M   N   O   P"};
  const char* bottom[]={"Q   R   S   T","U   V   W   X","Y   Z   _   -","SPACE   DELETE"};
  for(uint8_t i=0;i<4;++i){char rows[2][17];snprintf(rows[0],17,"%-16.16s",top[i]);snprintf(rows[1],17,"%-16.16s",bottom[i]);if(!strcmp(cache[i][0],rows[0])&&!strcmp(cache[i][1],rows[1]))continue;memcpy(cache[i],rows,sizeof(rows));DISPLAY_SERIAL.printf("MODLCD|%u|%s|%s\n",i==2?3:i==3?2:i,rows[0],rows[1]);}
  return true;
}
FLASHMEM bool namePadR8(uint8_t neo,uint8_t pad,uint8_t edge){
  static bool consumed[4][16]={};
  if(edge==SEESAW_KEYPAD_EDGE_FALLING&&consumed[neo][pad]){consumed[neo][pad]=false;return true;}
  if(fileBrowserMode!=FILE_BROWSER_SAVE&&patchUi808!=4)return false;
  if(edge!=SEESAW_KEYPAD_EDGE_RISING)return true;
  consumed[neo][pad]=true;uint8_t row=pad/4,col=neo*4+pad%4;char key=0;
  if(row==1&&col>=14){if(fileBrowserMode==FILE_BROWSER_SAVE)storageNameDelete();else nameDeleteR8();return true;}
  if(row==0)key='A'+col;
  else if(row==1)key=col<10?'Q'+col:col==10?'_':col==11?'-':' ';
  else if(row==2&&col<10)key=col==9?'0':'1'+col;
  if(!key)return true;
  if(fileBrowserMode==FILE_BROWSER_SAVE){storageNameKey(key);return true;}
  uint8_t limit=nameLimit07(patchNameTarget808);size_t n=strlen(patchNameDraft808);if(patchNamePos808>=limit)return true;
  patchNamePos808=min((size_t)patchNamePos808,n);if(patchNamePos808==n)patchNameDraft808[n+1]=0;patchNameDraft808[patchNamePos808++]=key;patch808Render();return true;
}
FLASHMEM void nameDeleteR8(){size_t n=strlen(patchNameDraft808);if(patchNamePos808<n)memmove(patchNameDraft808+patchNamePos808,patchNameDraft808+patchNamePos808+1,n-patchNamePos808);else if(n){patchNameDraft808[n-1]=0;patchNamePos808=n-1;}patch808Render();}

FLASHMEM uint32_t nameColor54(uint8_t neo,uint8_t pad){
 uint8_t row=pad/4,col=neo*4+pad%4;
 if(row==1&&col>=14)return colorFor(neo,100,0,0);
 if(row==1&&col>=12)return colorFor(neo,80,80,80);
 if(row==2&&col<10)return colorFor(neo,45,0,70);
 char key=row==0?'A'+col:row==1&&col<10?'Q'+col:0;
 if(key)return strchr("AEIOUY",key)?colorFor(neo,0,70,55):colorFor(neo,0,20,85);
 if(row==1&&(col==10||col==11))return colorFor(neo,80,45,0);
 return 0;
}
FLASHMEM bool namePixels54(){
 static bool previous=false;bool typing=fileBrowserMode==FILE_BROWSER_SAVE||patchUi808==4;
 if(!typing){if(previous)neoDisplayDirty=true;previous=false;return false;}
 previous=true;for(uint8_t n=0;n<4;++n){if(!(neoAvailable[n]||pc831.active))continue;for(uint8_t p=0;p<16;++p)neoPixel821(n,p,nameColor54(n,p));diagShowNeo814(n);}return true;
}
