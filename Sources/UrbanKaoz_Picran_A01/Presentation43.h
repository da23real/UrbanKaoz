#pragma once
// Seven CGRAM slots, no framebuffer or heap allocation. Never delays transport.
struct PresentationState43{uint32_t started=0,beat=0,lastFrame=UINT32_MAX;uint8_t mode=0;bool glyphs=false;};
static PresentationState43 presentation43;
static const uint8_t titleGlyph43[7][8]={
 {15,14,12,30,24,28,30,0}, // E, slanted heavy capitals
 {15,13,13,30,28,26,25,0}, // R
 {15,14,12,14,7,7,30,0},  // S
 {3,7,6,6,12,12,30,0},   // 1
 {15,14,12,30,7,7,30,0}, // 5
 {2,3,2,2,14,30,12,0},   // note
 {6,7,5,5,13,29,27,0}    // paired notes
};
bool presentationActive43(){return presentation43.mode!=0;}
const uint8_t* presentationGlyph43(uint8_t ch){return ch>=1&&ch<=7?titleGlyph43[ch-1]:nullptr;}
void presentationEnd43(){if(!presentation43.mode)return;presentation43.mode=0;presentation43.glyphs=false;screenMapHardware();renderMask931=31;displayDirty=true;}
void presentationStart43(uint8_t mode){presentation43={millis(),millis(),UINT32_MAX,mode,false};for(uint8_t i=0;i<4;++i)invalidateSmallScreenCache(i);}
void presentationBeat43(){if(presentation43.mode){presentation43.beat=millis();presentation43.lastFrame=UINT32_MAX;}}
void presentationReset43(){presentationEnd43();recoverMainLcd(true);for(uint8_t p=0;p<4;++p){screenPhysicalPresent[p]=i2cResponds(Wire1,LCD_ADDRESS[p]);if(screenPhysicalPresent[p]){LcdScreen hw={};hw.bus=&Wire1;hw.address=LCD_ADDRESS[p];hw.present=true;lcdInit(hw);}}screenMapHardware();renderDisplays(true);}
bool presentationCommand43(const char*line){if(!strncmp(line,"SCRMAP,",7)&&presentation43.mode)presentation43.glyphs=false;if(!strcmp(line,"LCDRESET43")){presentationReset43();return true;}if(!strcmp(line,"PRESENT43,0")){presentationEnd43();return true;}if(!strcmp(line,"PRESENT43,2")){presentationStart43(2);return true;}return false;}
void presentationService43(){
 auto&s=presentation43;if(!s.mode)return;
 if(saveAlert08||busyVisible910){presentationEnd43();return;}
 uint32_t now=millis(),age=now-s.started;if(s.mode==1&&age>=6000){presentationEnd43();return;}
 uint32_t tempo=constrain(gridValue[7],40,300),period=30000/tempo;
 uint32_t frame=(now-s.beat)/period;if(frame==s.lastFrame)return;s.lastFrame=frame;
 if(!s.glyphs){for(auto&lcd:screens)if(lcd.present){for(uint8_t n=0;n<7;++n){lcdCommand(lcd,0x40|(n+1)*8);for(uint8_t y=0;y<8;++y)lcdSend(lcd,titleGlyph43[n][y],true);}lcdCommand(lcd,0x80);}s.glyphs=true;}
 const char*title[]={"   URBAN KAOZ   "," DIY SEQUENCER  ","    VA01.46     ","    \1\2\3 \4\4\5     "};
 for(uint8_t i=0;i<4;++i){char top[17],bottom[17];memcpy(top,title[i],16);top[16]=0;memset(bottom,' ',16);bottom[16]=0;
  if(s.mode==1&&age<2000){unsigned shown=age/100;for(unsigned c=shown;c<16;++c)top[c]=' ';}
  for(uint8_t n=0;n<4;++n){uint8_t pos=(frame+n*16)%64;if(pos/16==i)bottom[pos%16]=n%2?7:6;}
  lcdPrintLine(i,0,top);lcdPrintLine(i,1,bottom);
 }
}
