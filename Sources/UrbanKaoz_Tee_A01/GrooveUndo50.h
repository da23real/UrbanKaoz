#pragma once
struct GrooveEdit50 {
 uint32_t generation,indPresent=0;bool list,whole,ind=false;uint8_t track,id,pattern=0;uint16_t mask;
 int8_t values[16][5];uint8_t shape[2];GrooveListEntry entries[14];
};
EXTMEM GrooveEdit50 grooveHistory50[2];
static uint8_t grooveHistoryCount50=0,grooveHistoryAt50=0;
FLASHMEM void grooveRefresh50(){storageDirty931=true;remixScheduleDirty911=true;mod805Invalidate();invalidateStepTriggers();neoDisplayDirty=displayValueDirty=true;sendGroovePacket();}
FLASHMEM bool grooveDelete50(uint8_t action){
 if(!grooveEditMode||debugTestMode!=DEBUG_TEST_NONE)return false;
 if(action!=EDIT_BUTTON_DELETE&&action!=EDIT_BUTTON_UNDO&&action!=EDIT_BUTTON_REDO)return false;
 if(grooveHistoryCount50&&grooveHistory50[0].generation!=patternGeneration906)grooveHistoryCount50=grooveHistoryAt50=0;
 if(action==EDIT_BUTTON_DELETE){
  bool list=patternMode;uint8_t t=listTrack40,id=list?listEditId52(t,listPattern40):editedGroove;
  if(list&&!id)id=listEnsure40();if(!id)return true;
  if(list&&!listRoom50(t,listPattern40,grooveListSelection805?grooveListSelection805:0x3fff))return true;
  grooveHistoryCount50=grooveHistoryAt50;
  if(grooveHistoryCount50==2){grooveHistory50[0]=grooveHistory50[1];--grooveHistoryCount50;}
  auto&h=grooveHistory50[grooveHistoryCount50++];h={};h.generation=patternGeneration906;h.list=list;h.track=t;h.id=id;h.whole=list?!grooveListSelection805:!grooveStepMask;
  h.mask=list?(grooveListSelection805?grooveListSelection805:0x3fff):(grooveStepMask?grooveStepMask:0xffff);
  if(list){h.pattern=listPattern40;h.ind=indEditing59();memcpy(h.entries,listEditRead52(t,h.pattern),sizeof(h.entries));if(h.ind)for(unsigned f=0;f<28;++f)if(ind50.find(t*16+h.pattern,IND_OOV50,IND_LIST50,0,f)>=0)h.indPresent|=1u<<f;for(unsigned i=0;i<14;++i)if(h.mask&(1u<<i))listEntry50(t,h.pattern,id,i,{});}
  else {memcpy(h.values,grooveOffset[id-1],sizeof(h.values));memcpy(h.shape,grooveShape16[id-1],2);for(unsigned i=0;i<16;++i)if(h.mask&(1u<<i)){memset(grooveOffset[id-1][i],0,5);grooveGateSet51(id,i,100);}if(h.whole)memset(grooveShape16[id-1],50,2);}
  grooveHistoryAt50=grooveHistoryCount50;
 }else{
  bool redo=action==EDIT_BUTTON_REDO;if(redo?grooveHistoryAt50>=grooveHistoryCount50:!grooveHistoryAt50)return true;
  auto&h=grooveHistory50[redo?grooveHistoryAt50:grooveHistoryAt50-1];
  if(h.list&&h.id>listCount44(h.track))return true;
  if(redo)++grooveHistoryAt50;else --grooveHistoryAt50;
  if(h.list){if(h.id>listCount44(h.track))return true;if(h.ind){uint8_t owner=h.track*16+h.pattern;for(unsigned i=0;i<14;++i)if(h.mask&(1u<<i))for(unsigned f=0;f<2;++f){unsigned param=i*2+f;int at=ind50.find(owner,IND_OOV50,IND_LIST50,0,param);bool present=at>=0;int value=present?ind50.entries[at].value:0;auto&stored=f?h.entries[i].repeats:h.entries[i].sheet;if(h.indPresent&(1u<<param))ind50.set(owner,IND_OOV50,IND_LIST50,0,param,stored);else ind50.remove(owner,IND_OOV50,IND_LIST50,0,param);stored=value;if(present)h.indPresent|=1u<<param;else h.indPresent&=~(1u<<param);}}else for(unsigned i=0;i<14;++i)if(h.mask&(1u<<i)){GrooveListEntry old;memcpy(&old,&listBank44(h.track)[h.id-1][i],sizeof(old));memcpy(&listBank44(h.track)[h.id-1][i],&h.entries[i],sizeof(old));h.entries[i]=old;}}
  else {for(unsigned i=0;i<16;++i)if(h.mask&(1u<<i))for(unsigned f=0;f<5;++f){auto old=grooveOffset[h.id-1][i][f];grooveOffset[h.id-1][i][f]=h.values[i][f];h.values[i][f]=old;}if(h.whole)for(unsigned f=0;f<2;++f){auto old=grooveShape16[h.id-1][f];grooveShape16[h.id-1][f]=h.shape[f];h.shape[f]=old;}}
 }
 indChanged50();grooveRefresh50();return true;
}
