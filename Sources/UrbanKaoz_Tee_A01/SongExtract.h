#pragma once

FLASHMEM bool songExtractCommand(char *line){
  unsigned t,id;unsigned long long begin,end;char trailing;
  if(!strcmp(line,"EXTRACT,CANCEL")){songExportArmed=false;songSend("EXPORT,CANCELLED");return true;}
  if(sscanf(line,"SOURCECLIP,%u%c",&id,&trailing)==1){
    if(!songMode||id>=SONG_CLIP_CAPACITY||!songClips[id].used){songError("UNKNOWN_CLIP");return true;}
    const auto &c=songClips[id];songExportClip=id;songExportTrack=c.track;songExportBegin=(uint64_t)c.position*96;songExportEnd=songExportBegin+(uint64_t)c.length*96;
  }else if(sscanf(line,"EXTRACT,%u,%llu,%llu%c",&t,&begin,&end,&trailing)==3){
    if(!songMode||!selectButtonActive||t>=16||end<=begin||end-begin>64*1536||end>(uint64_t)SONG_MAX_STEP*96){songError("INVALID_SELECTION");return true;}
    songExportClip=-1;songExportTrack=t;songExportBegin=begin;songExportEnd=end;
  }else return false;
  songExportArmed=true;songExportRevision=songRevision;songSend("EXPORT,SELECT_DESTINATION");DISPLAY_SERIAL.println("MSG|SELECT DEST PATTERN");return true;
}
FLASHMEM bool songExportTo(uint8_t track,uint8_t pattern){
  if(!songExportArmed)return false;
  if(songExportRevision!=songRevision){songExportArmed=false;songError("SOURCE_CHANGED_SELECT_AGAIN");return true;}
  if(playing||songRecord||!songWorkspaceReady||!storageWriteAvailable||pendingSelectionEdit.active||songDeleteArmed){songError("STOP_AND_FINISH_EDIT_FIRST");return true;}
  if(songPatternPresent(track,pattern)){songError("DEST_PATTERN_NOT_EMPTY");return true;}
  uint64_t begin=songExportBegin,end=songExportEnd;
  if(end<=begin||end-begin>64*1536){songError("SELECTION_EXCEEDS_64_BARS");return true;}
  rmx911::Model exportNotes;
  bool ok=true;
  auto append=[&](uint64_t tick,uint32_t gate,uint8_t note,uint8_t vel){
    if(exportNotes.notes.size>=65536||tick<begin||tick>=end){ok=false;return;}
    int32_t relative=tick-begin;
    exportNotes.mask|=1u<<track;
    if(!exportNotes.notes.add({exportNotes.nextId++,relative,(uint16_t)min(gate,(uint32_t)GATE_TICK_MAX),0,track,note,vel,0}))ok=false;
  };
  if(songExportClip>=0){
    if(songExportClip>=SONG_CLIP_CAPACITY||!songClips[songExportClip].used)ok=false;
    else {const SongClip c=songClips[songExportClip];
      if((uint64_t)c.position*96!=begin||((uint64_t)c.position+c.length)*96!=end||c.track!=songExportTrack)ok=false;
      if(ok)ok=songPrepare();
      if(ok){const auto &cache=songCache[c.track][c.pattern];if(cache.hasRoll)ok=false;
        for(uint32_t i=0;ok&&i<cache.count;++i){const auto &n=songNotes[cache.first+i];if(n.logical<c.offset*96||n.logical>=(c.offset+c.length)*96)continue;
          int32_t at=n.tick;int note=n.note,vel=n.velocity;uint32_t gate=n.gate;int16_t bend=0;songApplyGroove(c.track,c.pattern,n.logical,at,note,vel,gate,bend);
          int64_t tick=freeZoneTick(c.position,c.offset,at);if(bend||tick<(int64_t)begin||tick>=(int64_t)end){ok=false;break;}
          append(tick,min((uint64_t)gate,end-tick),note,vel);
        }
      }
    }
  }else for(uint32_t i=0;ok&&i<zlCount;++i){const auto &e=zlEvents[i];if(!songSliceContains(e.track,e.tick,songExportTrack,begin,end))continue;
    if(e.kind!=0){ok=false;break;}append(e.tick,e.gate,e.a,e.b);
  }
  if(ok&&exportNotes.notes.size){
    for(uint8_t t=0;t<16;++t){exportNotes.tracks[t].bars=(end-begin+1535)/1536;exportNotes.tracks[t].roll[1]=24;exportNotes.tracks[t].delay[1]=96;exportNotes.tracks[t].delay[4]=t;}
    ok=songExportNotes911(exportNotes,pattern,(end-begin+1535)/1536);
  }else {songError("EMPTY_OR_UNREPRESENTABLE_SELECTION");ok=false;}
  if(ok){songExportArmed=false;songSelect(track,pattern);songSend("EXPORT,DONE,%u,%u",track,pattern);}
  return true;
}
