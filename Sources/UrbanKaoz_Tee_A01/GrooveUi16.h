#pragma once
FLASHMEM void grooveEditAssign26(){
 // Editing a sheet never assigns a list implicitly.
 storageDirty931=true;
}
FLASHMEM uint16_t grooveUsers26(){uint16_t mask=0;for(uint8_t t=0;t<16;++t)for(uint8_t b=0;b<3;++b){auto v=blocks21[t][b];if(!v.groove||v.groove>listCount44(t))continue;for(auto&e:listBank44(t)[v.groove-1]){if(!e.sheet)break;if(e.sheet==editedGroove)mask|=1u<<t;}}return mask;}
FLASHMEM void grooveFirstNote26(uint8_t t,uint16_t step){
 bool assigned=false;for(auto&b:blocks21[t])assigned|=b.groove&&b.groove<=listCount44(t);if(!assigned)return;
 for(uint16_t s=0;s<trackStepCount(t);++s)if(noteCountAtStep(t,s))return;
 grooveNoticeAt26=millis();grooveNoticeDuration26=max(16UL,(uint32_t)(tickDurationMs()*1536));selectionIndicatorsPending931=true;
}
FLASHMEM void grooveNoticeService26(){if(!grooveNoticeDuration26)return;if(millis()-grooveNoticeAt26>=grooveNoticeDuration26)grooveNoticeDuration26=0;sendSelectionIndicatorsToPicran();}

FLASHMEM void sendGrooveVisual16(){
 DISPLAY_SERIAL.printf("GUS26,%u\n",grooveUsers26());
 if(grooveEditMode)DISPLAY_SERIAL.printf("GTRACK35,%u,%u\n",selectedTrack+1,blockListEdit21+1);
 DISPLAY_SERIAL.printf("OOV16,%u,%u,%u,%u,%u,%u",grooveEditMode&&!patternMode,grooveView16||oovMenu16,grooveShape16[editedGroove-1][0],grooveShape16[editedGroove-1][1],oovLow16,oovHigh16);
 for(uint8_t i=0;i<16;++i)DISPLAY_SERIAL.printf(",%d",grooveOffset[editedGroove-1][i][3]);
 DISPLAY_SERIAL.println();
}
FLASHMEM void oovRender16(){
 char value[21];snprintf(value,sizeof(value),"OA >=%02u  OB >=%02u",oovLow16,oovHigh16);
 sendMenuScreen("SCR > OOV (ticks)",value,"Moyenne     Forte","Clic Roy OK J1 annul");
 sendGrooveVisual16();
}
FLASHMEM void oovClose16(bool commit){
 if(commit)oovSave16();else{oovLow16=oovOld16[0];oovHigh16=oovOld16[1];}
 oovMenu16=false;oovRoy16=-1;sendSettingsPage();sendGrooveVisual16();
}
FLASHMEM void oovTurn16(uint8_t roy,int delta){
 if(roy>1)return;
 if(!roy)oovLow16=clampInt((int)oovLow16+delta,1,(int)oovHigh16-1);
 else oovHigh16=clampInt((int)oovHigh16+delta,(int)oovLow16+1,99);
 oovRoy16=roy;oovRender16();
}

FLASHMEM uint8_t grooveSheet35(uint8_t t){uint8_t id=blocks21[t][0].groove;return id&&id<=listCount44(t)&&listBank44(t)[id-1][0].sheet?listBank44(t)[id-1][0].sheet:grooveEditTrack23[t];}
FLASHMEM void grooveChooseSheet35(uint8_t t,uint8_t sheet){
 if(t>=16||!sheet||sheet>GROOVE_COUNT)return;
 uint8_t id=0;for(uint8_t i=0;i<listCount44(t);++i)if(listBank44(t)[i][0].sheet==sheet&&!listBank44(t)[i][1].sheet){id=i+1;break;}
 if(!id){if(listCount44(t)>=BLOCK_LISTS21){DISPLAY_SERIAL.println("MSG|9 GROOVE LISTS MAX");return;}id=++listCount44(t);memset(listBank44(t)[id-1],0,sizeof(listBank44(t)[0]));listBank44(t)[id-1][0]={sheet,1};}
 auto&bs=blocks21[t];bs[0].groove=id;bs[1].groove=bs[2].groove=0;grooveEditTrack23[t]=sheet;blockListEdit21=id-1;++modEpoch805;storageDirty931=true;
}
FLASHMEM void grooveTrack35(uint8_t t){grooveFinish60(true);if(t>=16)return;selectEditingTrack(t);editedGroove=grooveSheet35(t);uint8_t id=blocks21[t][0].groove;blockListEdit21=id?id-1:0;grooveStepMask=0;sendGroovePacket();displayValueDirty=neoDisplayDirty=true;}
