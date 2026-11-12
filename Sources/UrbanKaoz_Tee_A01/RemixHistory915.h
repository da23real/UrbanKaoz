#pragma once
// Included inside rmx911. Only changed array entries are retained. This also
// preserves the exact ordering used by swap-removal and simultaneous attacks.
template<class T> struct Change915 {uint32_t index;T before,after;};
template<class T> struct ArrayDelta915 {
 List<Change915<T>> changes;uint32_t before=0,after=0;
 bool capture(const List<T>&a,const List<T>&b){before=a.size;after=b.size;
  for(uint32_t i=0;i<(before>after?before:after);++i){
   if(i<before&&i<after&&!memcmp(&a[i],&b[i],sizeof(T)))continue;
   Change915<T> c{};c.index=i;if(i<before)c.before=a[i];if(i<after)c.after=b[i];if(!changes.add(c))return false;
  }return true;
 }
 bool reserve(List<T>&out,bool forward)const{return out.reserve(forward?after:before);}
 void apply(List<T>&out,bool forward)const{out.size=forward?after:before;for(uint32_t i=0;i<changes.size;++i){const auto&c=changes[i];if(c.index<out.size)out[c.index]=forward?c.after:c.before;}}
};
struct Delta915 {
 ArrayDelta915<Note> notes;ArrayDelta915<Property> properties;
 List<Change915<Track>> tracks;uint32_t beforeId=1,afterId=1;uint16_t beforeMask=0,afterMask=0;
 bool capture(const Model&a,const Model&b){
  beforeId=a.nextId;afterId=b.nextId;beforeMask=a.mask;afterMask=b.mask;
  if(!notes.capture(a.notes,b.notes)||!properties.capture(a.properties,b.properties))return false;
  for(uint32_t t=0;t<16;++t)if(memcmp(&a.tracks[t],&b.tracks[t],sizeof(Track))&&!tracks.add({t,a.tracks[t],b.tracks[t]}))return false;
  return true;
 }
 bool empty()const{return !notes.changes.size&&!properties.changes.size&&!tracks.size&&beforeId==afterId&&beforeMask==afterMask;}
 bool apply(Model&m,bool forward)const{
  // Reserve everything before the first semantic mutation: failure is atomic.
  if(!notes.reserve(m.notes,forward)||!properties.reserve(m.properties,forward))return false;
  notes.apply(m.notes,forward);properties.apply(m.properties,forward);
  for(uint32_t i=0;i<tracks.size;++i)m.tracks[tracks[i].index]=forward?tracks[i].after:tracks[i].before;
  m.nextId=forward?afterId:beforeId;m.mask=forward?afterMask:beforeMask;return true;
 }
};
struct History {
 static constexpr uint8_t Depth=16;
 Delta915* entries[Depth]={};uint8_t count=0,cursor=0;
 ~History(){clear();}
 static void dispose(Delta915*p){if(p){p->~Delta915();RMX_FREE(p);}}
 void clear(){for(auto*&p:entries){dispose(p);p=nullptr;}count=cursor=0;}
 // Temporary transaction rollback only; never stored in the history ring.
 static Model* copy(const Model&m){void*memory=RMX_ALLOC(sizeof(Model));if(!memory)return nullptr;auto*p=new(memory)Model;if(!p->clone(m)){p->~Model();RMX_FREE(p);return nullptr;}return p;}
 bool push(const Model&before,const Model&after){
  void*memory=RMX_ALLOC(sizeof(Delta915));if(!memory)return false;auto*p=new(memory)Delta915;
  if(!p->capture(before,after)){dispose(p);return false;}if(p->empty()){dispose(p);return true;}
  for(uint8_t i=cursor;i<count;++i){dispose(entries[i]);entries[i]=nullptr;}count=cursor;
  if(count==Depth){dispose(entries[0]);memmove(entries,entries+1,(Depth-1)*sizeof(Delta915*));--count;--cursor;}
  entries[count++]=p;cursor=count;return true;
 }
 bool move(Model&m,int direction){bool forward=direction>0;if(forward?cursor>=count:cursor==0)return false;
  auto*p=entries[forward?cursor:cursor-1];if(!p->apply(m,forward))return false;cursor+=forward?1:-1;return true;
 }
};
