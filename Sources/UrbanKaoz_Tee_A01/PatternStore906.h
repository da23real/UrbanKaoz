#pragma once
#include "Compact906.h"
struct PackedPattern906 {uint32_t length;uint8_t data[1];};
static PackedPattern906* patterns906[16][16]={};
static PackedPattern906* staged906[16][16]={};
EXTMEM PatternImage805 patternWork906;
static uint32_t patternBytes906=0,patternCount906=0;
static uint32_t patternGeneration906=0;
static int32_t allocationLimit906=-1; // failure injection, disabled in normal use
static bool readPatternPayload906(File&f,uint32_t magic,uint16_t bars,PatternImage805&image){
 if((magic!=0x554B4236UL&&magic!=0x554B4235UL)||bars<1||bars>64)return false;
 memset(&image,0,sizeof(image));image.bars=bars;
 #define READ906(field) if(f.read((uint8_t*)image.field,sizeof(image.field))!=sizeof(image.field))return false;
 READ906(active) READ906(vel) READ906(note) READ906(gate) READ906(micro)
 READ906(lockVel) READ906(lockPitch) READ906(lockGate) READ906(lockTime) READ906(roll) READ906(rollMask)
 if(magic>=0x554B4233UL){READ906(extraCount) READ906(extraNote)}
 if(magic>=0x554B4234UL){READ906(logical) READ906(fine)}
 if(magic>=0x554B4235UL){READ906(extra14) if(magic>=0x554B4236UL){READ906(mov)}}else for(uint16_t i=0;i<1024;++i)if(image.active[i])for(uint8_t v=0;v<image.extraCount[i]&&v<3;++v)image.extra14[i][v]={image.gate[i],image.micro[i],image.vel[i]};
 #undef READ906
 return image.mov[0]>=-98303&&image.mov[0]<=98303;
}
FLASHMEM PackedPattern906* packPattern906(const PatternImage805& image){
 size_t n=compactEncode906((const uint8_t*)&image,sizeof(image),nullptr);
 if(allocationLimit906==0)return nullptr;
 if(allocationLimit906>0)--allocationLimit906;
 auto*p=(PackedPattern906*)extmem_malloc(sizeof(uint32_t)+n);if(!p)return nullptr;
 p->length=n;compactEncode906((const uint8_t*)&image,sizeof(image),p->data);return p;
}
FLASHMEM bool unpackPattern906(const PackedPattern906*p,PatternImage805& image){
 return p&&compactDecode906(p->data,p->length,(uint8_t*)&image,sizeof(image));
}
FLASHMEM void releasePattern906(uint8_t t,uint8_t p){
 auto*&v=patterns906[t][p];if(v){patternBytes906-=v->length+4;--patternCount906;extmem_free(v);v=nullptr;}
}
FLASHMEM void installPattern906(uint8_t t,uint8_t p,PackedPattern906* v){
 releasePattern906(t,p);patterns906[t][p]=v;if(v){patternBytes906+=v->length+4;++patternCount906;}
}
bool storePattern906(uint8_t,uint8_t,const PatternImage805&);
FLASHMEM bool storedMatches59(uint8_t t,uint8_t p,const PatternImage805&image){
 auto*v=patterns906[t][p];if(!v)return false;auto*data=(const uint8_t*)&image;size_t at=0,out=0;
 while(at<v->length){uint8_t tag=v->data[at++];size_t n=(tag&127)+1;if(out+n>sizeof(image))return false;
  if(tag&128){if(at>=v->length)return false;uint8_t value=v->data[at++];for(size_t j=0;j<n;++j)if(data[out+j]!=value)return false;}
  else {if(at+n>v->length||memcmp(data+out,v->data+at,n))return false;at+=n;}out+=n;
 }return out==sizeof(image);
}
// Real-time switch uses literal packets of the existing codec, avoiding two
// byte-by-byte compression passes at the note boundary. Allocation failure
// falls back to the compact representation; content and disk format are identical.
FLASHMEM bool storeTransition59(uint8_t t,uint8_t p,const PatternImage805&image){
 const size_t size=sizeof(image),n=size+(size+127)/128;
 if(allocationLimit906==0)return false;
 if(allocationLimit906>0)--allocationLimit906;
 auto*v=(PackedPattern906*)extmem_malloc(n+4);
 if(!v)return storePattern906(t,p,image);
 v->length=n;auto*src=(const uint8_t*)&image;size_t at=0,out=0;
 while(at<size){size_t count=min((size_t)128,size-at);v->data[out++]=count-1;memcpy(v->data+out,src+at,count);at+=count;out+=count;}
 installPattern906(t,p,v);return true;
}
FLASHMEM bool storePattern906(uint8_t t,uint8_t p,const PatternImage805& image){
 auto*v=packPattern906(image);if(!v)return false;installPattern906(t,p,v);return true;
}
FLASHMEM void resetPatterns906(){++patternGeneration906;for(uint8_t t=0;t<16;++t)for(uint8_t p=0;p<16;++p){releasePattern906(t,p);if(staged906[t][p]){extmem_free(staged906[t][p]);staged906[t][p]=nullptr;}}}


FLASHMEM void compactIdle59(){
 static uint8_t cursor=0;uint8_t t=cursor/16,p=cursor%16;++cursor;auto*v=patterns906[t][p];
 const size_t literal=sizeof(PatternImage805)+(sizeof(PatternImage805)+127)/128;
 if(!v||v->length!=literal||!unpackPattern906(v,patternWork906))return;
 auto*packed=packPattern906(patternWork906);if(packed)installPattern906(t,p,packed);
}

