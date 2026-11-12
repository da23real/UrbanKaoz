#pragma once
FLASHMEM void fxSync32(){
 if(fxDirty32)for(uint8_t t=0;t<16;++t)if(fxDirty32&(1u<<t)){harmony32::Packet v{t,fxConfig32[t]};if(pidiPacket827(pidi827::FX32,&v,sizeof(v)))fxDirty32&=~(1u<<t);}
 static harmony32::Clock prev{};harmony32::Clock v{uint32_t(max(100.0f,tickDurationMs()*1000.0f)),prev.audible,uint8_t(playing||remixMode)};if(!fxClockValid32||pidiStateDirty931){v.audible=0;for(uint8_t t=0;t<16;++t)if(!trackSilent818(t))v.audible|=1u<<t;}
 if(!fxClockValid32||memcmp(&v,&prev,sizeof(v)))if(pidiPacket827(pidi827::CLOCK32,&v,sizeof(v))){prev=v;fxClockValid32=true;}
}
FLASHMEM void chordName32(uint8_t id,char*out){if(!id){strcpy(out,"Off");return;}snprintf(out,20,"%s %s",harmony32::roots[(id-1)%12],harmony32::qualities[(id-1)/12]);}
// Detection uses source note classes in the active pattern, including the edit draft.
FLASHMEM uint16_t chordNotes35(){uint16_t mask=0;for(uint16_t s=0;s<trackStepCount(selectedTrack);++s)for(uint8_t v=0;v<noteCountAtStep(selectedTrack,s);++v)mask|=1u<<(visualNoteAtVoice(selectedTrack,s,v)%12);return mask;}
FLASHMEM unsigned chordList35(uint8_t*ids){unsigned n=1;ids[0]=0;uint16_t notes=chordNotes35();
 for(unsigned a=0;a<12;++a){unsigned root=chordHarmonic32?(a*7)%12:a;for(unsigned q=0;q<12;++q){uint8_t id=1+q*12+root;uint16_t mask=harmony32::mask(id);if(!chordDetect35||(notes&&(mask&notes)==notes))ids[n++]=id;}}
 if(!chordHarmonic32)for(unsigned i=2;i<n;++i){uint8_t id=ids[i];char a[20],b[20];chordName32(id,a);unsigned j=i;while(j>1){chordName32(ids[j-1],b);if(strcmp(a,b)>=0)break;ids[j]=ids[j-1];--j;}ids[j]=id;}
 return n;
}
FLASHMEM uint8_t chordAt32(int index){uint8_t ids[145];unsigned n=chordList35(ids);if(index<0){for(unsigned i=0;i<n;++i)if(ids[i]==chordDraft32)return i;return 0;}return ids[index%n];}
FLASHMEM void chordRender35(){auto&c=fxConfig32[selectedTrack];char a[21],b[21],name[20];chordName32(c.chord,name);uint16_t notes=chordNotes35(),mask=harmony32::mask(c.chord);char mark=c.chord&&notes&&mask==notes?'*':c.chord&&notes&&(mask&notes)==notes?'+':' ';
 const char*modes[]={"Ignor","Near","Next","Prev"};snprintf(a,21,"%-5.5s%-5.5s%-5.5s%-5.5s",name,modes[c.policy],chordDetect35?"Detec":"All",chordHarmonic32?"Fifth":"A-Z");snprintf(b,21,"%c%-13.13s T%02u",mark,name,selectedTrack+1);sendMenuScreen("ChordMode List Sort ",a,b,"Off  TRA- TRA+ Back ");
}
FLASHMEM void chordTurn35(uint8_t roy,int delta,bool click){auto c=fxConfig32[selectedTrack];
 if(roy==0&&delta){uint8_t ids[145];unsigned n=chordList35(ids);int pos=0;for(unsigned i=0;i<n;++i)if(ids[i]==c.chord){pos=i;break;}pos=((pos+delta)%(int)n+n)%n;c.chord=chordDraft32=ids[pos];fxSet32(selectedTrack,c);storageDirty931=true;fxSync32();}
 else if(roy==1&&(delta||click)){c.policy=((c.policy+(delta?delta:1))%4+4)%4;fxSet32(selectedTrack,c);storageDirty931=true;fxSync32();}
 else if(roy==2&&(delta||click))chordDetect35=delta?delta>0:!chordDetect35;
 else if(roy==3&&(delta||click))chordHarmonic32=delta?delta>0:!chordHarmonic32;
 chordRender35();
}
FLASHMEM void fxRender32(){char title[21],a[21],b[21];auto&c=fxConfig32[selectedTrack];snprintf(title,21,"TRACK %02u %s",selectedTrack+1,trackPage933==5?"CHORD":"ARP");
 if(trackPage933==5){chordRender35();}

 else{static const char* modes[]={"Off","Up","Down","UpDown","Played","Random"};static const char* rates[]={"1/2","1/4","1/8","1/16","1/32","1/8T","1/16T","1/8D"};snprintf(a,21,"%s %s Oct%u",modes[c.arp],rates[c.rate],c.octaves);snprintf(b,21,"Gate%u Hold:%s",c.gate,c.latch?"On":"Off");sendMenuScreen(title,a,b,arpField32?"OA Oct OB Gate OC Hold":"OA Mode OB Rate OC >");}
}
FLASHMEM void fxOpen32(uint8_t page){trackPage933=page;chordDraft32=fxConfig32[selectedTrack].chord;arpField32=0;fxRender32();}
FLASHMEM void fxTurn32(uint8_t roy,int delta,bool click){auto c=fxConfig32[selectedTrack];
 if(trackPage933==5){chordTurn35(roy,delta,click);return;}
 else {if(!arpField32){if(!roy)c.arp=(c.arp+(delta?delta:click?1:0)%6+6)%6;else if(roy==1)c.rate=(c.rate+(delta?delta:click?1:0)%8+8)%8;else if(click||delta)arpField32=1;}else{if(!roy)c.octaves=clampInt(c.octaves+delta,1,4);else if(roy==1)c.gate=clampInt(c.gate+delta,1,100);else if(click)c.latch=!c.latch;else if(delta)arpField32=0;}fxSet32(selectedTrack,c);storageDirty931=true;}
 fxRender32();
}
FLASHMEM void fxButton32(uint8_t i){if(trackPage933==5){
 if(i<4){chordTurn35(i,0,true);return;}
 if(i==4){auto c=fxConfig32[selectedTrack];c.chord=chordDraft32=0;fxSet32(selectedTrack,c);storageDirty931=true;fxSync32();}
 else if(i==5||i==6){selectEditingTrack(clampInt((int)selectedTrack+(i==5?-1:1),0,15));chordDraft32=fxConfig32[selectedTrack].chord;}
 else {trackPage933=0;sendTrackMenu();return;}chordRender35();
 }else{arpField32^=1;fxRender32();}}
