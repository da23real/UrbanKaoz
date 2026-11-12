#pragma once
#include <stdint.h>
// Fixed point, shared verbatim with Picran. One sheet is always periodic.
namespace groove16 {
inline int wrap(int t){t%=1536;return t<0?t+1536:t;}
inline int shape(int q,int smo){
 if(q<=0)return 0;if(q>=9600)return 9600;
 if(smo<=0)return 0;if(smo>=100)return 9600;
 return (int)((int64_t)9600*smo*q/((100-smo)*(9600-q)+smo*q));
}
inline int q8(const int8_t* values,int stride,int tick,int smo,int width){
 int t=wrap(tick),i=t/96,u=t%96,a=values[i*stride],b=values[((i+1)%16)*stride];
 if(!u)return a*256;
 if(a&&b){ // B: never return to zero between two active neighbouring centres.
  int mid=(a+b)*128;
  if(u<=48)return mid+(int)((int64_t)(a*256-mid)*shape(9600-u*200,smo)/9600);
  return mid+(int)((int64_t)(b*256-mid)*shape((u-48)*200,smo)/9600);
 }
 if(width<=0)return 0;
 int distance=a?u:96-u,value=a?a:b;
 int q=9600-distance*10000/width;
 return (int)((int64_t)value*256*shape(q,smo)/9600);
}
inline int ticks(const int8_t*v,int stride,int tick,int smo,int width){
 int q=q8(v,stride,tick,smo,width);return q<0?-((-q+128)/256):(q+128)/256;
}
inline int strength(int q,int low,int high){int a=q<0?-q:q;return !a?0:a<low*256?1:a<high*256?2:3;}
}
