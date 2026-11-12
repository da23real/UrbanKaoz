#pragma once
// Lossless run/literal codec: no musical value (including zero) is a sentinel.
// Portable for the host validation harness. Caller owns all storage.
#include <stdint.h>
#include <stddef.h>
#include <string.h>
static size_t compactEncode906(const uint8_t* src,size_t size,uint8_t* dst){
 size_t at=0,out=0;
 while(at<size){
  size_t run=1;while(at+run<size&&run<128&&src[at+run]==src[at])++run;
  if(run>=3){if(dst){dst[out]=0x80|(run-1);dst[out+1]=src[at];}out+=2;at+=run;}
  else {size_t start=at;at+=run;
   while(at<size&&at-start<128){run=1;while(at+run<size&&run<3&&src[at+run]==src[at])++run;if(run>=3)break;++at;}
   size_t n=at-start;if(dst){dst[out]=n-1;memcpy(dst+out+1,src+start,n);}out+=n+1;
  }
 }return out;
}
static bool compactDecode906(const uint8_t* src,size_t size,uint8_t* dst,size_t capacity){
 size_t at=0,out=0;
 while(at<size){uint8_t tag=src[at++];size_t n=(tag&127)+1;
  if(n>capacity-out)return false;
  if(tag&128){if(at>=size)return false;memset(dst+out,src[at++],n);}
  else {if(n>size-at)return false;memcpy(dst+out,src+at,n);at+=n;}
  out+=n;
 }return out==capacity;
}
