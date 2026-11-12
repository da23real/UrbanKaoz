#pragma once
struct ColorPrefs11 {uint32_t magic;uint8_t brightness[8][4],plx[2],plxBrightness[2];uint16_t crc;};
FLASHMEM uint32_t colorScale11(uint32_t c,uint8_t level){uint32_t r=((c>>16)&255)*level/100,g=((c>>8)&255)*level/100,b=(c&255)*level/100;return (r<<16)|(g<<8)|b;}
FLASHMEM void colorsSave11(){ColorPrefs11 p{};p.magic=0x11C010A;p.crc=0;memcpy(p.brightness,colorBrightness11,sizeof(p.brightness));memcpy(p.plx,plxColors11,2);memcpy(p.plxBrightness,plxBrightness11,2);p.crc=pidi827::crc((uint8_t*)&p,offsetof(ColorPrefs11,crc));EEPROM.put(3800,p);}
FLASHMEM void colorsLoad11(){ColorPrefs11 p{};EEPROM.get(3800,p);if(p.magic!=0x11C010A||p.crc!=pidi827::crc((uint8_t*)&p,offsetof(ColorPrefs11,crc)))return;for(auto&row:p.brightness)for(auto b:row)if(b>100)return;for(auto c:p.plx)if(c>=NEO_PALETTE_COUNT)return;for(auto b:p.plxBrightness)if(b>100)return;memcpy(colorBrightness11,p.brightness,sizeof(p.brightness));memcpy(plxColors11,p.plx,2);memcpy(plxBrightness11,p.plxBrightness,2);}
FLASHMEM uint8_t colorRoleBrightness11(uint8_t role){switch(role){case 0:return colorBrightness11[0][0];case 1:return colorBrightness11[0][1];case 4:return colorBrightness11[0][2];case 5:return colorBrightness11[0][3];case 2:return colorBrightness11[5][0];case 3:return colorBrightness11[5][1];default:return colorBrightness11[5][2];}}
FLASHMEM uint32_t plxColor11(uint8_t neo,bool note,bool selected){return colorScale11(palette932(neo,plxColors11[note?1:0]),!note&&selected?colorBrightness11[0][3]:plxBrightness11[note?1:0]);}
FLASHMEM void colorsClick11(uint8_t roy){if(colorGroup932==1){indColorsPage51=!indColorsPage51;colorsRender932();neoDisplayDirty=true;return;}if(colorGroup932==5&&roy==2){headTrail25=(headTrail25+1)%3;colorsRender932();neoDisplayDirty=true;return;}if(colorGroup932==5&&roy>=2)return;colorLast11=min(roy,(uint8_t)3);if(!colorGroup932&&roy<2)colorPlxRow11=!colorPlxRow11;else colorPlxRow11=false;colorsRender932();neoDisplayDirty=true;}
FLASHMEM void colorsBrightness11(int delta){auto&b=(!colorGroup932&&colorPlxRow11&&colorLast11<2)?plxBrightness11[colorLast11]:colorBrightness11[colorGroup932][colorGroup932==5&&colorLast11==1?2:colorLast11];b=clampInt((int)b+delta,0,100);colorsRender932();neoDisplayDirty=true;}
FLASHMEM bool colorsPreview11(){
 if(!settingsOpen||settingsMenuLevel!=SETTINGS_COLOR_EDIT)return false;
 for(uint8_t n=0;n<4;++n){if(!(neoAvailable[n]||pc831.active))continue;for(uint8_t p=0;p<16;++p){uint8_t role=p/4;uint32_t c=0;
  if(!colorGroup932){bool selected=role>=2;uint8_t col=p%4;if(col==0)c=colorForRole(n,selected?NEO_COLOR_SELECTED_EMPTY:NEO_COLOR_EMPTY);else if(col==1)c=selected?colorForRole(n,NEO_COLOR_SELECTED_NOTE):plxColor11(n,true,false);else if(col==2)c=colorScale11(colorForRole(n,selected?NEO_COLOR_SELECTED_NOTE:NEO_COLOR_NOTE),50);else c=plxColor11(n,false,selected);}
  else{uint8_t saved=colorGroup932;uint8_t idx=min(role,(uint8_t)(saved==5?1:3));c=colorScale11(palette932(n,colorRef932(idx)),colorBrightness11[saved][saved==5&&idx==1?2:idx]);if((p%4)>1)c=colorScale11(c,40);}
  neoPixel821(n,p,c);
 }diagShowNeo814(n);serviceRealtimeBetweenNeoPanels();}return true;
}
