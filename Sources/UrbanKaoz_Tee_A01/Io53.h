#pragma once
FLASHMEM void ioLearn53(uint8_t channel,uint8_t number,uint8_t value,bool cc){
 if(ioLearnKey53<0||!ioKey38(ioLearnKey53)||channel>15||number>127||(!cc&&!value)||(ioLearnPort53&&midiInputPort53!=ioLearnPort53))return;
 auto k=ioLearnKey53;ioLearnKey53=-1;mod807Remember();auto&m=modUnits805[k];m.attack=channel;m.hold=cc?6:3;m.sustain=cc?number:m.sustain;m.rate=100001+ioLearnPort53;ioSeen38[k]=false;modPresetDirty07[k]=storageDirty931=true;mod805Invalidate();displayValueDirty=true;DISPLAY_SERIAL.println("MSG|INPUT LEARNED");
}
FLASHMEM void ioVirtualSend53(uint8_t channel,uint8_t number,uint8_t value){
 if(channel>15||number>127||!synkozEnabled933||!Serial)return;
 if(ioVirtualLast53[channel][number]==value)return;
 uint32_t seq=++synkozSequence933;if(synkozCount933==64){++synkozDrops933;return;}
 auto&e=synkozQueue933[(synkozHead933+synkozCount933)%64];e={seq,micros(),synkozTick933,6,number,value,channel};e.channel=channel+16;++synkozCount933;ioVirtualLast53[channel][number]=value;
}
