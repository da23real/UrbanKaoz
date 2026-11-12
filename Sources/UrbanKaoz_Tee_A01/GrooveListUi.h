#pragma once
FLASHMEM void sendGrooveListPacket(){if(grooveEditMode&&patternMode)grooveListDisplay40();}
FLASHMEM bool editGrooveListSingle805(uint8_t index,int32_t delta){return grooveListEdit40(index,delta);}
FLASHMEM bool editGrooveList(uint8_t index,int32_t delta){
  grooveListFocus805=index;
  if(!grooveListSelection805)return editGrooveListSingle805(index,delta);
  bool ok=false;for(uint8_t i=0;i<GROOVE_LIST_SIZE;++i)if(grooveListSelection805&(1u<<i))ok=editGrooveListSingle805(i,delta)||ok;return ok;
}
