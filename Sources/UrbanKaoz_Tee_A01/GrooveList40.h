#pragma once
extern bool remixScheduleDirty911;
// Shared list references use assignment 66 and the existing per-pattern payload.
// The format version prevents older readers from misinterpreting this assignment.
static uint8_t listTrack40=0,listPattern40=0;static bool listFocused40=false;
static GrooveListEntry listView40[14];
FLASHMEM uint8_t listId40(uint8_t t,uint8_t p){
 if(ind50.active(t*16+p,IND_OOV50)){int at=ind50.find(t*16+p,IND_OOV50,LOCK_BLOCK21,0,12);if(at>=0){uint8_t id=ind50.entries[at].value;return id<=listCount44(t)?id:0;}}
 if(patternGroove[t][p]==(GROOVE_COUNT+2)){uint8_t id=grooveLists[t][p][0].sheet;return id<=listCount44(t)?id:0;}
 if(patternGroove[t][p])return 0;
 uint8_t id=blocks21[t][0].groove;return id<=listCount44(t)?id:0;
}
FLASHMEM const GrooveListEntry* listRead40(uint8_t t,uint8_t p){
 uint8_t id=listId40(t,p);if(id)memcpy(listView40,listBank44(t)[id-1],sizeof(listView40));
 else if(patternGroove[t][p]==GROOVE_USE_LIST)memcpy(listView40,grooveLists[t][p],sizeof(listView40));
 else {memset(listView40,0,sizeof(listView40));if(patternGroove[t][p]&&patternGroove[t][p]<=GROOVE_COUNT)listView40[0]={patternGroove[t][p],1};}
 for(unsigned i=0;i<14;++i){listView40[i].sheet=indBase50(t,p,IND_LIST50,0,i*2,listView40[i].sheet);listView40[i].repeats=indBase50(t,p,IND_LIST50,0,i*2+1,listView40[i].repeats);}return listView40;
}
FLASHMEM uint8_t listEditId52(uint8_t t,uint8_t p){return indEditing59()?listId40(t,p):blocks21[t][0].groove;}
FLASHMEM const GrooveListEntry* listEditRead52(uint8_t t,uint8_t p){if(indEditing59())return listRead40(t,p);uint8_t id=blocks21[t][0].groove;if(id&&id<=listCount44(t))memcpy(listView40,listBank44(t)[id-1],sizeof(listView40));else memset(listView40,0,sizeof(listView40));return listView40;}
FLASHMEM bool listRoom50(uint8_t t,uint8_t p,uint16_t mask){if(!indEditing59())return true;unsigned need=0;for(unsigned i=0;i<14;++i)if(mask&(1u<<i))for(unsigned f=0;f<2;++f)need+=ind50.find(t*16+p,IND_OOV50,IND_LIST50,0,i*2+f)<0;if(ind50.count+need>IndStore50::CAPACITY){DISPLAY_SERIAL.println("MSG|IND MEMORY FULL");return false;}return true;}
FLASHMEM void listEntry50(uint8_t t,uint8_t p,uint8_t id,uint8_t slot,GrooveListEntry e){if(indEditing59()){auto old=listRead40(t,p)[slot];if(old.sheet!=e.sheet)ind50.set(t*16+p,IND_OOV50,IND_LIST50,0,slot*2,e.sheet);if(old.repeats!=e.repeats)ind50.set(t*16+p,IND_OOV50,IND_LIST50,0,slot*2+1,e.repeats);}else if(id&&id<=listCount44(t)){listBank44(t)[id-1][slot]={e.sheet,e.repeats};}}
FLASHMEM void listAssign40(uint8_t t,uint8_t p,uint8_t id){if(indEditing59()){if(!ind50.set(t*16+p,IND_OOV50,LOCK_BLOCK21,0,12,id)){DISPLAY_SERIAL.println("MSG|IND MEMORY FULL");return;}}else{blocks21[t][0].groove=id;patternGroove[t][p]=0;}indChanged50();}
FLASHMEM uint8_t listEnsure40(){
 uint8_t id=listEditId52(listTrack40,listPattern40);if(id&&id<=listCount44(listTrack40))return id;
 if(listCount44(listTrack40)>=BLOCK_LISTS21){DISPLAY_SERIAL.println("MSG|9 GROOVE LISTS MAX");return 0;}
 const auto*src=listEditRead52(listTrack40,listPattern40);id=++listCount44(listTrack40);memcpy(listBank44(listTrack40)[id-1],src,sizeof(listBank44(listTrack40)[0]));listAssign40(listTrack40,listPattern40,id);return id;
}
FLASHMEM void grooveListFocus40(uint8_t t,uint8_t p){
 grooveFinish60(true);
 listTrack40=t;listPattern40=p;indEditTrack50=t;indEditPattern50=p;listFocused40=true;selectedTrack=t;displayedPattern=p+1;
 uint8_t id=listId40(t,p);blockListEdit21=id?id-1:0;grooveListSelection805=grooveListRepeatEditing=0;
 neoDisplayDirty=displayValueDirty=true;sendGroovePacket();
}
FLASHMEM void grooveListPad40(uint8_t t,uint8_t p){
 lastPatternTrack=t;lastPatternIndex=p;lastPatternValid=true;
 if(selectButtonActive){patternSelected[t][p]=!patternSelected[t][p];for(int row=15;row>=0;--row)for(uint8_t col=0;col<16;++col)if(patternSelected[col][row]){grooveListFocus40(col,row);return;}}
 grooveListFocus40(t,p);
}
FLASHMEM void grooveListDisplay40(){
 if(!listFocused40){listTrack40=selectedTrack;listPattern40=activePattern[selectedTrack];listFocused40=true;}
 const auto*list=listEditRead52(listTrack40,listPattern40);uint8_t id=listEditId52(listTrack40,listPattern40);
 DISPLAY_SERIAL.printf("GTRACK35,%u,%u\nGLSEL,%u\nGLIST,%u",listTrack40+1,id,grooveListSelection805,grooveListRepeatEditing);
 for(uint8_t i=0;i<14;++i)DISPLAY_SERIAL.printf(",%u,%u",list[i].sheet,max((uint8_t)1,list[i].repeats));
 DISPLAY_SERIAL.printf("\nFIELD,14,T,List\nFIELD,14,V,%u\nFIELD,15,T,ALL\nFIELD,15,V,%s\n",id,grooveAllOff59?"OFF":"ON");
 DISPLAY_SERIAL.printf("FIELD,14,T,List\nFIELD,14,V,%u%s\n",id,(grooveTrackOff59&(1u<<listTrack40))?"off":"on");
}
FLASHMEM bool grooveListEdit40(uint8_t k,int32_t delta){
 if(!listFocused40)grooveListFocus40(selectedTrack,activePattern[selectedTrack]);
 if(k<14&&delta)grooveBegin60();if(k==14)grooveFinish60(true);
 uint8_t id=listEnsure40();if(!id)return true;
 if(k==14){id=WRAP23(id,delta,1,listCount44(listTrack40));blockListEdit21=id-1;listAssign40(listTrack40,listPattern40,id);}
 else if(k<14){if(!listRoom50(listTrack40,listPattern40,1u<<k))return true;auto e=listEditRead52(listTrack40,listPattern40)[k];if(grooveListRepeatEditing&(1u<<k))e.repeats=WRAP23(max((uint8_t)1,e.repeats),delta,1,99);else{e.sheet=WRAP23(e.sheet,delta,0,GROOVE_COUNT);if(!e.repeats)e.repeats=1;}listEntry50(listTrack40,listPattern40,id,k,e);indChanged50();}
 blockListEdit21=id-1;grooveListDisplay40();return true;
}
FLASHMEM void grooveListFinish40(){ /* No automatic assignment on selection exit. */ }
FLASHMEM void grooveListClipboard40(uint8_t button,bool down){
 if(!down||button>=CLIPBOARD_BUTTON_COUNT)return;
 static GrooveListEntry clips[CLIPBOARD_COUNT][14];static bool valid[CLIPBOARD_COUNT]={};
 uint8_t slot=button/CLIPBOARD_ACTION_COUNT,action=button%CLIPBOARD_ACTION_COUNT;
 uint8_t t=lastPatternValid?lastPatternTrack:selectedTrack,p=lastPatternValid?lastPatternIndex:activePattern[t];
 if(action==CLIP_ACTION_COPY){memcpy(clips[slot],listEditRead52(t,p),sizeof(clips[slot]));valid[slot]=true;DISPLAY_SERIAL.println("MSG|LIST COPIED");}
 if(action==CLIP_ACTION_PASTE&&valid[slot]){grooveListFocus40(t,p);uint8_t id=listEnsure40();if(id&&listRoom50(t,p,0x3fff)){for(unsigned i=0;i<14;++i)listEntry50(t,p,id,i,clips[slot][i]);indChanged50();grooveListDisplay40();DISPLAY_SERIAL.println("MSG|LIST PASTED");}}
}
FLASHMEM uint8_t grooveSheet40(uint8_t t,uint8_t p,uint8_t blockId,uint64_t bar){
 if(grooveBypass23||grooveAllOff59||(grooveTrackOff59&(1u<<t)))return 0;
 if(ind50.active(t*16+p,IND_OOV50))return listSheetAtBar(listRead40(t,p),bar);
 uint8_t assignment=patternGroove[t][p];
 if(assignment==GROOVE_COUNT+2){uint8_t id=listId40(t,p);return id?listSheetAtBar(reinterpret_cast<const GrooveListEntry*>(listBank44(t)[id-1]),bar):0;}
 if(assignment==GROOVE_USE_LIST)return listSheetAtBar(grooveLists[t][p],bar);
 return blockId&&blockId<=listCount44(t)?listSheetAtBar(reinterpret_cast<const GrooveListEntry*>(listBank44(t)[blockId-1]),bar):0;
}
