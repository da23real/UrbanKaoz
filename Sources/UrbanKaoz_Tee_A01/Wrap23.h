#pragma once
struct WrapState23 {uint32_t event=0,edgeAt=0,burstAt=0;int value=0,result=0,lo=0,hi=0;uint8_t held=0,track=255;int8_t direction=0;bool armed=false;uint32_t zeroAt57=0;bool zero57=false;};
static uint32_t wrapEvent23=1;
static uint8_t wrapDepth23=0;
struct WrapInput23 {WrapInput23(){if(!wrapDepth23++)++wrapEvent23;}~WrapInput23(){--wrapDepth23;}};
FLASHMEM int wrapValue23(int value,int delta,int lo,int hi,WrapState23&s){
 if(!delta||hi<=lo)return max(lo,min(hi,value));
 if(s.event==wrapEvent23&&s.value==value&&s.lo==lo&&s.hi==hi&&s.track==selectedTrack)return s.result;
 int direction=delta>0?1:-1;bool edge=direction>0?value>=hi:value<=lo;
 uint32_t now=millis(),twoBeats=max(1u,(uint32_t)(120000.0f/max(1.0f,bpm)));
 if(s.track!=selectedTrack||s.direction!=direction||s.lo!=lo||s.hi!=hi||!edge){s.armed=false;s.held=0;}
 if(s.track!=selectedTrack||s.lo!=lo||s.hi!=hi)s.zero57=false;
 int result=max(lo,min(hi,value+delta));
 if(lo<0&&hi>0){
  if(value&&((value<0&&result>=0)||(value>0&&result<=0))){result=0;s.zeroAt57=now;s.zero57=true;}
  else if(!value&&s.zero57){if((uint32_t)(now-s.zeroAt57)<150)result=0;else s.zero57=false;}
  else if((uint32_t)(now-s.zeroAt57)>=150)s.zero57=false;
 }else s.zero57=false;
 if(edge){
  if(!s.armed){s.armed=true;s.edgeAt=now;s.held=0;}
  else if((uint32_t)(now-s.edgeAt)>=twoBeats){
   if(!s.held||(uint32_t)(now-s.burstAt)>=twoBeats){s.held=0;s.burstAt=now;}
   if(++s.held==3){result=direction>0?lo:hi;s.armed=false;s.held=0;}
  }
 }else if(result==(direction>0?hi:lo)){s.armed=true;s.edgeAt=now;}
 s.event=wrapEvent23;s.value=value;s.result=result;s.lo=lo;s.hi=hi;s.track=selectedTrack;s.direction=direction;return result;
}
#define WRAP23(v,d,l,h) ([&](){static EXTMEM WrapState23 state;return wrapValue23((v),(d),(l),(h),state);}())

FLASHMEM int zeroClamp57(int value,int delta,int lo,int hi,WrapState23&s){
 if(lo>=0||hi<=0||!delta)return max(lo,min(hi,value+delta));
 bool edge=(delta>0?value>=hi:value<=lo);
 int result=wrapValue23(value,delta,lo,hi,s);
 return edge?(delta>0?hi:lo):result;
}
#define ZCLAMP57(v,d,l,h) ([&](){static EXTMEM WrapState23 state;return zeroClamp57((v),(d),(l),(h),state);}())
