#pragma once
#include "ConversionRules.h"
FLASHMEM int convertedEventOrder(const void *a,const void *b){
  const auto &x=*(const ZLEvent*)a,&y=*(const ZLEvent*)b;
  if(x.tick!=y.tick)return x.tick<y.tick?-1:1;
  return x.id==y.id?0:(x.id<y.id?-1:1);
}

FLASHMEM bool songConvert(const SongClip &clip, int removeId) {
  if(!songEngineActive || !songWorkspaceReady || playing || songRecord || songDeleteArmed ||
      pendingSelectionEdit.active || deleteButtonActive || selectButtonActive) {
    songError("STOP_AND_FINISH_EDIT_FIRST"); return false;
  }
  if(!songClipValid(clip,songPatternSteps(clip.track,clip.pattern))) {songError("SOURCE_LENGTH_CHANGED");return false;}
  // Refresh active sources as well as inactive caches; never modify a pattern.
  if(!songPrepare() || !songCachePattern(clip.track,clip.pattern)) return false;
  uint32_t count=0;
  const auto &cache=songCache[clip.track][clip.pattern];
  if(cache.hasRoll){songError("CONVERT_ROLL_UNSUPPORTED_SOURCE_KEPT");return false;}
  const int64_t end=((int64_t)clip.position+clip.length)*96;
  for(uint32_t i=0;i<cache.count;++i) {
    const auto &n=songNotes[cache.first+i];
    if(n.logical<(int32_t)clip.offset*96 || n.logical>=(int32_t)(clip.offset+clip.length)*96) continue;
    int32_t at=n.tick;int note=n.note,vel=n.velocity;uint32_t gate=n.gate;int16_t bend=0;
    songApplyGroove(clip.track,clip.pattern,n.logical,at,note,vel,gate,bend);
    if(bend) {songError("CONVERT_PITCH_BEND_UNSUPPORTED");return false;}
    int64_t tick=freeZoneTick(clip.position,clip.offset,at);
    // A negative onset cannot be represented: do not silently move/drop it.
    if(tick<0 || tick>=end) {songError("CONVERT_ONSET_OUTSIDE_SONG_OR_CLIP");return false;}
    gate=(uint32_t)min((uint64_t)gate,(uint64_t)(end-tick));
    if(count>=ZL_CAPACITY-zlCount || (uint64_t)zlNextId+count>=UINT32_MAX) {songError("ZL_CAPACITY_OR_RANGE");return false;}
    ZLEvent e={(uint64_t)tick,zlNextId+count,gate,clip.track,0,(uint8_t)note,(uint8_t)vel};
    if(!zlValid(e)) {songError("INVALID_ZL");return false;}
    zlStaging[count++]=e;
  }
  if(!count) {songError("CONVERT_EMPTY_CLIP");return false;}
  // All checks precede the only commit. Snapshot streams the result without flooding the outbox.
  qsort(zlStaging,count,sizeof(ZLEvent),convertedEventOrder);
  uint32_t a=zlCount,b=count,out=zlCount+count;
  while(b){if(a&&zlEvents[a-1].tick>zlStaging[b-1].tick)zlEvents[--out]=zlEvents[--a];else zlEvents[--out]=zlStaging[--b];}
  zlCount+=count;
  zlNextId+=count;
  if(removeId>=0) songClips[removeId].used=false;
  ++songRevision;songView=2;songSnapshot();songSend("CONVERTED,%d,%lu",removeId,(unsigned long)count);
  return true;
}
FLASHMEM bool songConversionCommand(char *line) {
  unsigned id,t,p;unsigned long pos;char extra;
  if(sscanf(line,"CONVERT,%u%c",&id,&extra)==1) {
    if(id>=SONG_CLIP_CAPACITY||!songClips[id].used)songError("UNKNOWN_CLIP");
    else songConvert(songClips[id],id);
    return true;
  }
  if(sscanf(line,"ZPATTERN,%u,%u,%lu%c",&t,&p,&pos,&extra)==3) {
    if(t>=16||p>=16||pos>SONG_MAX_STEP){songError("INVALID_PATTERN");return true;}
    SongClip c={(uint32_t)pos,0,songPatternSteps(t,p),(uint8_t)t,(uint8_t)p,true};
    songConvert(c,-1);return true;
  }
  return false;
}
