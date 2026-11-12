#pragma once
FLASHMEM bool songExtraCommand(char*line){
  unsigned id,t,kind,a,b,gate,v;unsigned long long tick;
  if(sscanf(line,"VIEW,%u",&v)==1){if(v>2)songError("INVALID_VIEW");else{songView=v;songExtraState();}return true;}
  if(!strcmp(line,"ENGINE,0")){(pidiStateDirty931=true, playing=false);internalClockRunning=false;allNotesOff();songEngineActive=false;songMode=false;songRecord=false;songState();return true;}
  if(!strcmp(line,"SAVE")){songSaveWorkspace();return true;}if(!strcmp(line,"LOAD")){songLoadWorkspace();return true;}
  if(sscanf(line,"RECORD,%u",&v)==1){songRecord=!remixMode&&v==1;songExtraState();return true;}
  if(sscanf(line,"ZADD,%u,%u,%llu,%u,%u,%u",&kind,&t,&tick,&gate,&a,&b)==6){
    if(t>=16||kind>1||a>127||b>127){songError("INVALID_ZL");return true;}zlInsert({tick,0,gate,(uint8_t)t,(uint8_t)kind,(uint8_t)a,(uint8_t)b});return true;}
  if(sscanf(line,"ZDELETE,%u",&id)==1){if(!songDeleteArmed){songError("ENABLE_DELETE_ON_SEQUENCER");return true;}
    for(uint32_t i=0;i<zlCount;++i)if(zlEvents[i].id==id){kbSongRemoveR10(id);memmove(zlEvents+i,zlEvents+i+1,(zlCount-i-1)*sizeof(ZLEvent));--zlCount;++songRevision;allNotesOff();songSend("ZREMOVE,%u",id);return true;}songError("UNKNOWN_ZL");return true;}
  if(sscanf(line,"ZEDIT,%u,%llu,%u,%u,%u",&id,&tick,&gate,&a,&b)==5){
    for(uint32_t i=0;i<zlCount;++i)if(zlEvents[i].id==id){auto e=zlEvents[i];e.tick=tick;e.gate=gate;if(a>127||b>127){songError("INVALID_ZL");return true;}e.a=a;e.b=b;if(!zlValid(e)){songError("INVALID_ZL");return true;}
      kbSongPitchR10(id,e.a);memmove(zlEvents+i,zlEvents+i+1,(zlCount-i-1)*sizeof(ZLEvent));--zlCount;uint32_t j=zlCount;while(j&&zlEvents[j-1].tick>e.tick){zlEvents[j]=zlEvents[j-1];--j;}zlEvents[j]=e;++zlCount;++songRevision;songEmitZL(j);return true;}songError("UNKNOWN_ZL");return true;}
  return false;
}
