#pragma once
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <new>
#ifndef RMX_ALLOC
#define RMX_ALLOC malloc
#define RMX_FREE free
#endif
// Only actual events and property exceptions allocate memory. No per-tick storage.
namespace rmx911 {
constexpr int Step=96, Bar=16*Step;
inline int wrap(int64_t x,int period){int r=x%period;return r<0?r+period:r;}
inline int bound(int64_t v,int lo,int hi){return v<lo?lo:v>hi?hi:(int)v;}
template<class T> struct List {
 T* data=nullptr;uint32_t size=0,capacity=0;
 List()=default;List(const List&)=delete;List&operator=(const List&)=delete;
 ~List(){clear();}
 T&operator[](uint32_t i){return data[i];}const T&operator[](uint32_t i)const{return data[i];}
 void clear(){RMX_FREE(data);data=nullptr;size=capacity=0;}
 bool reserve(uint32_t n){if(n<=capacity)return true;if(n>65536)return false;
  uint32_t cap=(n+15)&~15u;if(cap<capacity+capacity/2)cap=capacity+capacity/2;if(cap>65536)cap=65536;auto*p=(T*)RMX_ALLOC(cap*sizeof(T));if(!p)return false;
  if(size)memcpy(p,data,size*sizeof(T));RMX_FREE(data);data=p;capacity=cap;return true;}
 bool add(const T&v){if(!reserve(size+1))return false;data[size++]=v;return true;}
 bool copy(const List&o){if(!reserve(o.size))return false;if(o.size)memcpy(data,o.data,o.size*sizeof(T));size=o.size;return true;}
 void swap(List&o){auto*p=data;data=o.data;o.data=p;auto n=size;size=o.size;o.size=n;n=capacity;capacity=o.capacity;o.capacity=n;}
};
enum Kind:uint8_t {Roll=0,Delay=1,CC=2,NRPN7=3,NRPN14=4,Block21=5};
struct Note {uint32_t id;int32_t tick;uint16_t gate;int16_t time;uint8_t track,pitch,velocity,reserved=0;};
struct Property {uint32_t id;int32_t value;uint16_t parameter;uint8_t kind,reserved=0;};
struct Track {int16_t roll[6]={};int16_t delay[9]={};uint16_t bars=1;uint8_t groove=0;uint8_t sheets[16]={};uint16_t repeats[16]={};};
struct Model {
 List<Note> notes;List<Property> properties;Track tracks[16];uint32_t nextId=1;uint16_t mask=0;
 bool clone(const Model&o){if(!notes.copy(o.notes)||!properties.copy(o.properties))return false;memcpy(tracks,o.tracks,sizeof(tracks));nextId=o.nextId;mask=o.mask;return true;}
 void swap(Model&o){notes.swap(o.notes);properties.swap(o.properties);for(int t=0;t<16;++t){auto a=tracks[t];tracks[t]=o.tracks[t];o.tracks[t]=a;}auto n=nextId;nextId=o.nextId;o.nextId=n;auto m=mask;mask=o.mask;o.mask=m;}
 int find(uint32_t id,uint8_t kind,uint16_t param)const{for(uint32_t i=0;i<properties.size;++i){auto&p=properties[i];if(p.id==id&&p.kind==kind&&p.parameter==param)return i;}return -1;}
 int value(const Note&n,uint8_t kind,uint16_t param)const{int i=find(n.id,kind,param);if(i>=0)return properties[i].value;return kind==Roll&&param<6?tracks[n.track].roll[param]:kind==Delay&&param<9?tracks[n.track].delay[param]:kind==Roll&&param==6?33:kind==Roll&&param==8?-60:0;}
 bool set(const Note&n,uint8_t kind,uint16_t param,int value){int i=find(n.id,kind,param);int base=kind==Roll&&param<6?tracks[n.track].roll[param]:kind==Delay&&param<9?tracks[n.track].delay[param]:kind==Roll&&param==6?33:kind==Roll&&param==8?-60:0;
  if(kind<=Delay&&value==base){if(i>=0)properties[i]=properties[--properties.size];return true;}
  if(i>=0){properties[i].value=value;return true;}return properties.add({n.id,value,param,kind,0});}
 bool append(const Model&src,const Note&original,uint8_t track,int tick,int period){
  if(nextId==UINT32_MAX)return false;Note n=original;n.id=nextId++;n.track=track;n.tick=wrap(tick,period);
  if(!notes.add(n))return false;mask|=1u<<track;
  for(uint8_t k=0;k<2;++k)for(uint8_t p=0;p<(k==Roll?12:13);++p)if(!set(n,k,p,src.value(original,k,p)))return false;
  for(uint32_t i=0;i<src.properties.size;++i){auto p=src.properties[i];if(p.id==original.id&&(p.kind>=CC||(p.kind==Delay&&p.parameter>=13)||(p.kind==Roll&&p.parameter>=12))){p.id=n.id;if(!properties.add(p))return false;}}return true;
 }
 void remove(uint32_t i){auto id=notes[i].id;notes[i]=notes[--notes.size];for(uint32_t p=0;p<properties.size;)if(properties[p].id==id)properties[p]=properties[--properties.size];else ++p;}
};
// Common bar for HEAR, independent of each source pattern's length.
inline bool targetBar912(uint32_t tick,uint16_t commonBars,uint16_t hear){(void)commonBars;return hear&&(tick/Bar)%hear==hear-1;}
inline uint8_t targetStep912(int32_t tick){return wrap(tick+Step/2,Bar)/Step;}
inline bool export912(Model&out,const Model&draft,uint16_t enabled){
 if(!out.clone(draft))return false;out.mask=draft.mask&enabled;
 for(uint32_t i=0;i<out.notes.size;)if(!(out.mask&(1u<<out.notes[i].track)))out.remove(i);else ++i;
 return true;
}
struct Selection {int8_t role=-1;uint16_t steps=0;uint16_t tracks=0xFFFF;uint16_t bar=0,exiled=0,edited=0;
 bool choose(uint8_t r,uint8_t step){if(steps&&role!=r)return false;role=r;uint16_t bit=1u<<step;if(steps&bit){if(edited&bit)exiled|=bit;}else exiled&=~bit;steps^=bit;if(!steps)role=-1;return true;}
 bool contains(const Note&n,const Model&m,bool source)const{if(!(tracks&(1u<<n.track)))return false;int origin=source?(bar%m.tracks[n.track].bars)*Bar:bar*Bar;int offset=wrap(n.tick-origin,m.tracks[n.track].bars*Bar);return offset<Bar&&(steps&(1u<<(source?offset/Step:targetStep912(n.tick))));}
 void clear(){role=-1;steps=exiled=edited=0;}
};
// The caller stages the entire operation, so any capacity failure leaves the draft intact.
inline bool paste(Model&dst,const Model&src,uint16_t sourceMask,uint16_t destMask,int bar,int anchor,int length,int destination,int latency,int period,const Selection*selection=nullptr){
 uint8_t from[16],to[16],nf=0,nt=0;for(uint8_t t=0;t<16;++t){if(sourceMask&(1u<<t))from[nf++]=t;if(destMask&(1u<<t))to[nt++]=t;}if(nf!=nt||!nf||length<1)return false;
 dst.mask|=destMask;
 // GhostR replaces the pasted interval; notes outside it remain untouched.
 for(uint32_t i=0;i<dst.notes.size;){const auto&n=dst.notes[i];int offset=wrap(n.tick-destination-latency,period);bool erase=(destMask&(1u<<n.track))&&offset<length;
  if(selection){int step=wrap(offset+anchor,Bar)/Step;erase=(destMask&(1u<<n.track))&&offset<Bar&&(selection->steps&(1u<<step));}
  if(erase)dst.remove(i);else ++i;
 }
 for(uint32_t i=0;i<src.notes.size;++i){auto&n=src.notes[i];int k=0;while(k<nf&&from[k]!=n.track)++k;if(k==nf)continue;
  int sourcePeriod=src.tracks[n.track].bars*Bar;int origin=(bar%src.tracks[n.track].bars)*Bar+anchor+latency;
  int offset=wrap(n.tick-origin,sourcePeriod);
  if(selection){if(!selection->contains(n,src,true))continue;offset=wrap(n.tick-((bar%src.tracks[n.track].bars)*Bar+anchor),sourcePeriod);}
  else if(offset>=length)continue;
  if(!dst.append(src,n,to[k],destination+latency+offset,period))return false;
 }return true;
}
#include "RemixHistory915.h"
}
