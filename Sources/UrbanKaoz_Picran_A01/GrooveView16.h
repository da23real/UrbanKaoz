#pragma once
#include "GrooveMath16.h"
bool grooveVisible16=false,grooveView16=false;
uint8_t grooveSmo16=50,grooveWidth16=50,oovLow16=2,oovHigh16=5;
int8_t grooveTim16[16]={};bool grooveGlyphs16[4]={};
// Three strengths, mirrored for anticipation/retardation. Six CGRAM slots.
const uint8_t oovGlyph16[3][8]={{0,0,4,2,31,2,4,0},{0,4,6,31,31,31,6,4},{4,6,31,31,31,31,6,4}};
void handleOov16(char* line){
 int v[22];char *t=strtok(line,",");
 for(uint8_t i=0;i<22;++i){t=strtok(nullptr,",");if(!t||!*t)return;char*end;long n=strtol(t,&end,10);if(*end||n< -99||n>100)return;v[i]=n;}
 if(strtok(nullptr,",")||v[0]<0||v[0]>1||v[1]<0||v[1]>1||v[2]<0||v[3]<0||v[4]<1||v[5]<=v[4]||v[5]>99)return;
 for(uint8_t i=6;i<22;++i)if(v[i]>99)return;
 grooveVisible16=v[0];grooveView16=v[1];grooveSmo16=v[2];grooveWidth16=v[3];oovLow16=v[4];oovHigh16=v[5];
 for(uint8_t i=0;i<16;++i)grooveTim16[i]=v[6+i];displayDirty=true;
}
void oovGlyphMode16(uint8_t index,bool on){
 if(grooveGlyphs16[index]==on&&!on)return;
 auto&s=screens[index];if(!s.present)return;
 if(on){for(uint8_t side=0;side<2;++side)for(uint8_t strength=0;strength<3;++strength){
  lcdCommand(s,0x40|(1+side*3+strength)*8);
  for(uint8_t y=0;y<8;++y){uint8_t row=oovGlyph16[strength][y];if(!side){uint8_t mirror=0;for(uint8_t x=0;x<5;++x)mirror|=((row>>x)&1)<<(4-x);row=mirror;}lcdSend(s,row,true);}
 }}else{for(uint8_t n=0;n<3;++n){lcdCommand(s,0x40|noteSlot927(0x12+n)*8);for(uint8_t y=0;y<8;++y)lcdSend(s,noteGlyph927[n][y],true);}smallInverse902[index]=0;cursorCode932[index]=0;presetInverse07=0;}
 grooveGlyphs16[index]=on;s.cacheValid[0]=s.cacheValid[1]=false;
}
bool renderOov16(uint8_t index){
 static char name51[4]={};
 bool active=grooveVisible16&&grooveEditing&&!grooveListVisible;
 if(index==0&&name51[0]&&(!active||grooveView16||!grooveActive51)){
   auto&s=screens[0];if(s.present)for(uint8_t n=0;n<3;++n){lcdCommand(s,0x40|noteSlot927(0x12+n)*8);for(uint8_t y=0;y<8;++y)lcdSend(s,noteGlyph927[n][y],true);}
   name51[0]=0;smallInverse902[0]=0;cursorCode932[0]=0;memset(indGlyph50[0],0,8);screens[0].cacheValid[0]=screens[0].cacheValid[1]=false;
 }
 oovGlyphMode16(index,active&&grooveView16);
 if(!active)return false;
 char top[17],bottom[17];memset(top,' ',16);memset(bottom,' ',16);top[16]=bottom[16]=0;
 const char*names[16]={"Oov","","","","TRA","","","","Not","Vel","Gt%","Tim","SMO","Wid","","Off"};
 for(uint8_t k=0;k<4;++k){uint8_t field=index*4+k;char v[5]="";
  memcpy(top+4*k,names[field],min((size_t)3,strlen(names[field])));
  if(field==0)snprintf(v,5,"%c%u ",'A'+(grooveSheet-1)/9,(grooveSheet-1)%9+1);
  else if(field==4)snprintf(v,5,"%02u",grooveTrack35);
  else if(field>=8&&field<=11){int f=field-8;if(grooveCommon[f]){if(f==2)snprintf(v,5,"%u",grooveValues[f]);else snprintf(v,5,"%+03d",grooveValues[f]);}}
  else if(field==12)snprintf(v,5,"%03u",grooveSmo16);
  else if(field==13)snprintf(v,5,"%03u",grooveWidth16);
  else if(field==15)strcpy(v,"Viw");
  memcpy(bottom+4*k,v,min((size_t)(field==10?4:3),strlen(v)));
 }
 if(index<2){char tracks[49]={};uint8_t count=0;for(uint8_t t=0;t<16;++t)if(grooveUsers26&(1u<<t)){snprintf(tracks+count*3,sizeof(tracks)-count*3,"%02u,",t+1);++count;}if(count)tracks[count*3-1]=0;else strcpy(tracks,"None");
  uint8_t page=count>8?(millis()/2000)%2:0;char window[25];memset(window,' ',24);window[24]=0;size_t start=page*24,len=strlen(tracks);if(start<len)memcpy(window,tracks+start,min((size_t)24,len-start));if(index==0)memcpy(bottom+4,window,12);else memcpy(bottom+4,window+12,12);
 }
 if(grooveView16){for(uint8_t c=0;c<16;++c){int tick=(index*16+c)*24;int q=groove16::q8(grooveTim16,1,tick,grooveSmo16,grooveWidth16);int force=groove16::strength(q,oovLow16,oovHigh16);top[c]=force?(char)(force+(q>0?3:0)):'-';}}
 if(index==0&&grooveActive51)bottom[3]='*';
 lcdPrintLine(index,0,top);lcdPrintLine(index,1,bottom);return true;
}
