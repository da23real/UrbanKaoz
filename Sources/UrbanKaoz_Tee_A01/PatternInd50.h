#pragma once
// Sparse absolute replacements. DEF values remain in their existing owners;
// this store contains only explicit pattern edits, including dormant families.
enum IndFamily50 : uint8_t { IND_BAS50,IND_ROL50,IND_DLY50,IND_ENV50,IND_PATCH50,IND_OOV50,IND_LFO50 };
struct IndEntry50 {
 int32_t value;uint16_t target,parameter;uint8_t owner,family,kind,state;
};
struct IndStore50 {
 static constexpr unsigned SLOTS=8192,CAPACITY=4096;
 IndEntry50 entries[SLOTS];uint8_t flags[256];uint16_t count;
 static unsigned hash(uint8_t owner,uint8_t family,uint8_t kind,uint16_t target,uint16_t parameter){return (owner*131071u+family*8191u+kind*1619u+target*31337u+parameter*6971u)&(SLOTS-1);}
 void clear(){memset(entries,0,sizeof(entries));memset(flags,0,sizeof(flags));count=0;}
 int find(uint8_t owner,uint8_t family,uint8_t kind,uint16_t target,uint16_t parameter,bool insertion=false)const{
  unsigned h=hash(owner,family,kind,target,parameter);int tomb=-1;
  for(unsigned n=0;n<SLOTS;++n){unsigned i=(h+n)&(SLOTS-1);const auto&e=entries[i];
   if(!e.state)return insertion?(tomb>=0?tomb:(int)i):-1;
   if(e.state==2){if(tomb<0)tomb=i;continue;}
   if(e.owner==owner&&e.family==family&&e.kind==kind&&e.target==target&&e.parameter==parameter)return i;
  }return insertion?tomb:-1;
 }
 bool active(uint8_t owner,uint8_t family)const{return family<7&&(flags[owner]&0x80)&&(flags[owner]&(1u<<family));}
 int32_t value(uint8_t owner,uint8_t family,uint8_t kind,uint16_t target,uint16_t parameter,int32_t fallback)const{
  if(!active(owner,family))return fallback;int i=find(owner,family,kind,target,parameter);return i<0?fallback:entries[i].value;
 }
 bool set(uint8_t owner,uint8_t family,uint8_t kind,uint16_t target,uint16_t parameter,int32_t value){
  if(family>=7)return false;int i=find(owner,family,kind,target,parameter,true);if(i<0)return false;
  if(entries[i].state!=1){if(count>=CAPACITY)return false;++count;}
  entries[i]={value,target,parameter,owner,family,kind,1};flags[owner]|=0x80|(1u<<family);return true;
 }
 bool remove(uint8_t owner,uint8_t family,uint8_t kind,uint16_t target,uint16_t parameter){int i=find(owner,family,kind,target,parameter);if(i<0)return false;entries[i].state=2;--count;uint8_t enabled=flags[owner]&0x80;flags[owner]=enabled;for(const auto&e:entries)if(e.state==1&&e.owner==owner)flags[owner]|=1u<<e.family;return true;}
 void useFamily(uint8_t owner,uint8_t family,bool enabled){if(family>=7)return;flags[owner]|=0x80;if(enabled)flags[owner]|=1u<<family;else flags[owner]&=~(1u<<family);}
 void useDef(uint8_t owner){for(auto&e:entries)if(e.state==1&&e.owner==owner){e.state=2;--count;}flags[owner]=0;}
 bool copy(uint8_t source,uint8_t destination){
  if(source==destination)return true;unsigned from=0,to=0;for(const auto&e:entries)if(e.state==1){from+=e.owner==source;to+=e.owner==destination;}
  if(count-to+from>CAPACITY)return false;useDef(destination);
  for(const auto&e:entries)if(e.state==1&&e.owner==source){IndEntry50 v=e;set(destination,v.family,v.kind,v.target,v.parameter,v.value);}flags[destination]=flags[source];return true;
 }
};
static_assert(sizeof(IndEntry50)==12,"sparse pattern entry layout");
