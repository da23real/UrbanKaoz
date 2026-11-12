#pragma once
#include <stdarg.h>

FLASHMEM void songSend(const char *format, ...) {
  char text[150]; va_list args; va_start(args, format);
  vsnprintf(text, sizeof(text), format, args); va_end(args);
  uint8_t next=(songOutWrite+1)%64;if(next==songOutRead){songOutOverflow=true;return;}
  memcpy(songOutbox[songOutWrite],text,strlen(text)+1);songOutWrite=next;
}
void songFlush(){
  if(songOutRead==songOutWrite)return;
  const char*text=songOutbox[songOutRead];size_t n=strlen(text);
  // A disconnected Y5 or a closed/replaced editor must not receive stale names.
  if(!strncmp(text,"NAMEEDIT,",9)&&!nameQueuedValid813(text)){songOutRead=(songOutRead+1)%64;return;}
  if(Serial&&Serial.availableForWrite()<(int)n+8)return;
  if(songMidi){uint8_t packet[160]={0xF0,0x7D,'U','K','S',1};memcpy(packet+6,text,n);packet[n+6]=0xF7;songMidi.sendSysEx(n+7,packet,true);}
  if(Serial){Serial.print("SONG,");Serial.println(text);}
  songOutRead=(songOutRead+1)%64;
  if(songOutOverflow&&(songOutWrite+1)%64!=songOutRead){songOutOverflow=false;songSend("ERROR,RESYNC_REQUIRED");}
}

FLASHMEM void songError(const char *why) { songSend("ERROR,%s", why); DISPLAY_SERIAL.print("MSG|");DISPLAY_SERIAL.println(why); }
FLASHMEM uint16_t songPatternSteps(uint8_t t, uint8_t p) {
  if (p == activePattern[t]) return trackBars[t] * STEPS_PER_BAR;
  patternHasNotes(t, p); // also discovers the stored metadata
  return patternBank[t][p].initialized ? patternBank[t][p].bars * STEPS_PER_BAR : 0;
}
FLASHMEM bool songPatternPresent(uint8_t t, uint8_t p) {
  if (mod805OwnerPresent(t,p) || patternHasNotes(t,p)) return true;
  if (p == activePattern[t]) {
    for (uint8_t f=0; f<FINE_EVENTS_PER_TRACK; ++f) if (fineNote[t][f].active) return true;
  }
  return false;
}
FLASHMEM void songPosition931() {
  songSend("STATE,%u,%u,%llu,%u,%lu,%lu,%u,%u,%u,%u", songMode, playing,
    (unsigned long long)songDisplayTick40(), songLoopEnabled, (unsigned long)songStartBar,
    (unsigned long)songEndBar, (unsigned)(bpm*100), sdAvailable, songNoteCount, songDeleteArmed);
}
FLASHMEM void songState() {
  selectionIndicatorsPending931=true;
  songStateDirty931=false;
  songPosition931();
  songExtraState();
  songSend("SELECTMODE,%u",selectButtonActive);
  selection805SongState();
}
FLASHMEM void songEmitClip(uint16_t id) {
  const SongClip &c=songClips[id];
  if (c.used) songSend("CLIP,%u,%u,%u,%lu,%u,%u", id,c.track,c.pattern,
    (unsigned long)c.position,c.offset,c.length);
  else songSend("REMOVE,%u",id);
}
FLASHMEM void songSnapshot() {
  songSnapshotRevision=songRevision;
  songSend("BEGIN,2,16,16,96,16,128,65536,32768");
  storageDirty931=nameDirty931=true;songCatalogCursor=0; songClipCursor=-1; zlSendCursor=-1; songState();
}
FLASHMEM bool songAppendNote(int32_t tick,uint16_t gate,uint8_t note,uint8_t vel,int32_t logical,const TrackRollSettings &roll,int32_t sourceTick=INT32_MIN,bool fixedRvl=false) {
  if (songNoteCount >= SONG_NOTE_CAPACITY) return false;
  // Cache-only expo bit7 preserves explicit RVL provenance; cleared before roll evaluation.
  songRoll805[songNoteCount]=roll;if(fixedRvl)songRoll805[songNoteCount].expo|=0x80;songLockTick817[songNoteCount]=sourceTick==INT32_MIN?logical:sourceTick;
  SongNote &n=songNotes[songNoteCount++];
  n.tick=tick; n.logical=logical; n.gate=gate; n.note=note; n.velocity=vel; return true;
}
FLASHMEM bool songRead(File &f,uint32_t offset,void *dst,size_t size) {
  return f.seek(offset) && f.read((uint8_t*)dst,size)==size;
}
// Build a compact immutable note cache. Reading inactive patterns does NOT
// replace the active editing arrays, touch defaults, or write pattern files.
FLASHMEM bool songCachePattern(uint8_t t,uint8_t p) {
  SongPatternCache &cache=songCache[t][p]; if(cache.valid) return true;
  uint32_t begin=songNoteCount; bool ok=true; cache.hasRoll=false;
  if(p==activePattern[t]) {
    for(uint16_t s=0;s<trackStepCount(t) && ok;++s) {
      uint8_t count=noteCountAtStep(t,s);
      for(uint8_t v=0;v<count && ok;++v) {
        if(configuredRollSettings(t,s,v).level||rollExtra1000(t,p,(int32_t)s*96+logicalMicroTick(t,s,v),noteAtVoice(t,s,v)).v[1]) cache.hasRoll=true;
        int off=clampInt((int)logicalMicroTick(t,s,v)+effectiveMicroTick(t,s,v),-96,96);
        ok=songAppendNote((int32_t)s*96+off+defaultMov41[t][0],effectiveGateTicks(t,s,v),noteAtVoice(t,s,v),effectiveVel(t,s,v),(int32_t)s*96,configuredRollSettings(t,s,v),(int32_t)s*96+logicalMicroTick(t,s,v),rvlFixedLock938(t,s,v));
      }
    }
  } else {
    char path[40]; internalPatternPath(t,p,path); File f=SD.open(path,FILE_READ);
    uint32_t magic=0; uint16_t bars=0;
    ok=f && songRead(f,0,&magic,4) && songRead(f,4,&bars,2) &&
      (magic==0x554B4236UL||magic==0x554B4235UL) && bars>=1 && bars<=MAX_BARS;
    // B4 SoA layout, sizeof ties offsets to the unchanged source format.
    uint32_t oActive=7,oVel=oActive+sizeof(stepActive[t]),oNote=oVel+sizeof(stepVel[t]);
    uint32_t oGate=oNote+sizeof(stepMidiNote[t]),oMicro=oGate+sizeof(stepGateTicks[t]);
    uint32_t oLV=oMicro+sizeof(stepMicroTick[t]),oLP=oLV+sizeof(stepLockVel[t]);
    uint32_t oLG=oLP+sizeof(stepLockPitch[t]),oLT=oLG+sizeof(stepLockGate[t]);
    uint32_t oRoll=oLT+sizeof(stepLockTime[t]),oMask=oRoll+ROLL_ROW_BYTES906;
    uint32_t oExtra=oMask+sizeof(stepRollLockMask[t]),oExtraNote=oExtra+sizeof(stepExtraNoteCount[t]);
    uint32_t oLogical=oExtraNote+sizeof(stepExtraMidiNote[t]),oFine=oLogical+sizeof(stepLogicalMicro[t]);
    int32_t mov41=0;if(ok&&magic>=0x554B4236UL)ok=songRead(f,oFine+sizeof(fineNote[t])+sizeof(stepExtra14[t]),&mov41,4);
    if(ok && f.size()<oFine+sizeof(fineNote[t])) ok=false;
    for(uint16_t s=0;s<bars*16 && ok;++s) {
      uint8_t active=0; ok=songRead(f,oActive+s,&active,1); if(!active || !ok) continue;
      uint8_t v=0,n=0,locks[4]={},extras=0,en[3]={}; uint16_t gate=0; int16_t micro=0; int8_t logical=0;
      ok=songRead(f,oVel+s,&v,1)&&songRead(f,oNote+s,&n,1)&&songRead(f,oGate+2*s,&gate,2)&&
        songRead(f,oMicro+2*s,&micro,2)&&songRead(f,oLV+s,locks,1)&&songRead(f,oLP+s,locks+1,1)&&
        songRead(f,oLG+s,locks+2,1)&&songRead(f,oLT+s,locks+3,1)&&songRead(f,oExtra+s,&extras,1)&&
        songRead(f,oExtraNote+3*s,en,3)&&songRead(f,oLogical+s,&logical,1);
      if(!ok) break;
      TrackRollSettings roll={};uint8_t rollMask=0;
      if(!songRead(f,oRoll+s*sizeof(roll),&roll,sizeof(roll))||!songRead(f,oMask+s,&rollMask,1)){ok=false;break;}
      for(uint8_t field=0;field<6;++field)if(!(rollMask&(1u<<field)))setRollFieldValue(roll,field,rollFieldValue(trackRoll[t],field));
      if(roll.level||rollExtra1000(t,p).v[1])cache.hasRoll=true;

      const int32_t tick=mov41+(int32_t)s*96+clampInt((int)logical+micro,-96,96);
      ok=songAppendNote(tick,gate,n,v,(int32_t)s*96,roll,(int32_t)s*96+logical,(rollMask&(1u<<5))!=0);
      for(uint8_t e=0;e<min((uint8_t)3,extras)&&ok;++e){ExtraNote14 x;ok=songRead(f,oFine+sizeof(fineNote[t])+(s*3+e)*sizeof(x),&x,sizeof(x));if(ok)ok=songAppendNote(mov41+(int32_t)s*96+clampInt((int)logical+x.micro,-96,96),x.gate,en[e],x.vel,(int32_t)s*96,roll,(int32_t)s*96+logical,(rollMask&(1u<<5))!=0);}
    }
    for(uint8_t i=0;i<FINE_EVENTS_PER_TRACK&&ok;++i) {
      FineNoteEvent n; ok=songRead(f,oFine+i*sizeof(n),&n,sizeof(n));
      if(!ok || !n.active || n.step>=bars*16) continue;
      for(uint8_t field=0;field<6;++field)if(!(n.pxMask&(1u<<(PX_ROLL_SHIFT+field))))setRollFieldValue(n.roll,field,rollFieldValue(trackRoll[t],field));
      if(n.roll.level||rollExtra1000(t,p,(int32_t)n.step*96+n.logicalMicro,n.note).v[1])cache.hasRoll=true;
      ok=songAppendNote(mov41+(int32_t)n.step*96+clampInt((int)n.logicalMicro+
          (n.micro),-96,96),
        n.gate,
        n.note,n.vel,(int32_t)n.step*96,n.roll,(int32_t)n.step*96+n.logicalMicro,(n.pxMask&(1u<<(PX_ROLL_SHIFT+5)))!=0);
    }
    if(f) f.close();
  }
  if(!ok) { songNoteCount=begin; songError("CACHE_FULL_OR_PATTERN_READ_FAILED"); return false; }
  cache.first=begin; cache.count=songNoteCount-begin; cache.valid=true; return true;
}
FLASHMEM bool songPrepare() {
  if (!pattern805Flush()) return false;
  if(!songWorkspaceReady){songError("PSRAM_REQUIRED");return false;}
  songNoteCount=0; memset(songCache,0,sizeof(songCache));
  for(uint16_t i=0;i<SONG_CLIP_CAPACITY;++i) if(songClips[i].used) {
    const SongClip &c=songClips[i];
    if(!songClipValid(c,songPatternSteps(c.track,c.pattern))) { songError("SOURCE_LENGTH_CHANGED"); return false; }
    if(!songCachePattern(c.track,c.pattern)) return false;
  }
  return true;
}
FLASHMEM void songSeek(uint64_t tick) {
  allNotesOff(); songTick=tick; evaluateCurrentTickOnNextClock=true;
  noInterrupts(); pendingClockTicks=0; interrupts(); songState();
}
FLASHMEM void songSetMode(bool enabled) {
  if(enabled&&!songScreen932()){songMissing932();return;}
  if(enabled==songMode) { songState(); return; }
  if(enabled && menuNavigationOnly()) {
    songError("FINISH_EDIT_FIRST"); return;
  }
  if(!enabled){songLeaveSurface();neoDisplayDirty=true;sendSelectionIndicatorsToPicran();return;}
  leavePadModesA01(false);
  if(!songEngineActive){if(!songPrepare())return;(pidiStateDirty931=true, playing=false);internalClockRunning=false;allNotesOff();songTick=0;evaluateCurrentTickOnNextClock=true;}
  songEngineActive=true;songMode=true;songDeleteArmed=false;(pidiStateDirty931=true, remixMode=false);grooveEditMode=false;patternMode=true;
  remixLeave911();clearPatternSelection();cancelStepSelection();
  songHeldNeo=songHeldPad=-1;
  neoDisplayDirty=displayBpmDirty=displayValueDirty=true;
  sendSelectionIndicatorsToPicran(); songSnapshot();
  DISPLAY_SERIAL.println(enabled?F("MSG|SONG"):F("MSG|PATTERN"));
}
FLASHMEM void songSelect(uint8_t t,uint8_t p) {
  if(t>=16||p>=16) return;
  if(t!=selectedTrack)recordExitA01();
  songSelectedTrack=t; songSelectedPattern=p; selectedTrack=t; displayedPattern=p+1;
  clearPatternSelection(); patternSelected[t][p]=true; neoDisplayDirty=true;
  songSend("SELECT,%u,%u",t,p);
}
FLASHMEM bool songPad(uint8_t neoIndex,uint8_t pad,uint8_t edge) {
  if(!songMode) return false;
  const uint8_t t=neoIndex*4+pad%4,p=patternRowOffset+pad/4;
  if(edge==SEESAW_KEYPAD_EDGE_RISING) {
    if(songExportArmed){songExportTo(t,p);songHeldNeo=songHeldPad=-1;return true;}
    neoPadDown[neoIndex][pad]=true; songHeldNeo=neoIndex; songHeldPad=pad;
    songHeldSince=millis(); songHeldSent=false;
  } else {
    neoPadDown[neoIndex][pad]=false;
    if(songHeldNeo==neoIndex && songHeldPad==pad) {
      if(!songHeldSent) songSelect(t,p);
      songHeldNeo=songHeldPad=-1;
    }
  }
  return true;
}
FLASHMEM bool songTransport(uint8_t index,bool pressed) {
  TransportScope903 scope(index);
  if(index==2&&pressed)autosave805Request();
  if(index>=6)return false; // B30/B31 are reserved for the transport options.
  if(!songEngineActive) return false;
  if(index==6)return false;
  if(index==4||index==5)transportMeasureRepeatAtMs[index-4]=pressed?millis()+transportTicksToMs(transportRepeatWaitTicks):0;
  if(!pressed) return true;
  if(index==0){recordToggle820();return true;}
  const uint64_t barTicks=1536;
  if(index==1) {
    if(!playing && !songPrepare()) return true;
    if(!playing)patch808Play();
    (pidiStateDirty931=true, playing=!playing); internalClockRunning=playing;
    noInterrupts(); pendingClockTicks=0; interrupts();
    if(!playing) allNotesOff();
  } else if(index==2) {
    transportMeasureRepeatAtMs[0]=transportMeasureRepeatAtMs[1]=0;
    (pidiStateDirty931=true, playing=false); internalClockRunning=false; songSeek((uint64_t)(songStartBar-1)*barTicks);
  } else if(index==3) songSeek((uint64_t)(songStartBar-1)*barTicks);
  else if(index==4) songSeek(songTick>=barTicks?(songTick/barTicks-1)*barTicks:0);
  else if(index==5) songSeek((songTick/barTicks+1)*barTicks);
  songState(); return true;
}
FLASHMEM void songRoy(uint8_t encoder,int delta) {
  if(encoder==0) { patternRowOffset=(uint8_t)clampInt((int)patternRowOffset+delta,0,12); neoDisplayDirty=true; }
  else if(encoder==1) songSelect((uint8_t)clampInt((int)songSelectedTrack+delta,0,15),songSelectedPattern);
  else if(encoder==2) {
    if(loopEditStart) songStartBar=(uint32_t)max((int64_t)1,min((int64_t)songEndBar,(int64_t)songStartBar+delta));
    else songEndBar=(uint32_t)max((int64_t)songStartBar,min((int64_t)62500000,(int64_t)songEndBar+delta));
    songState();
  } else if(encoder==3) songSelect(songSelectedTrack,(uint8_t)clampInt((int)songSelectedPattern+delta,0,15));
}
void songApplyGroove(uint8_t t,uint8_t p,int32_t logical,int32_t &at,int &note,int &vel,uint32_t &gate,int16_t &bend){
  auto b=blockResolvedValid21?blockResolved21:blocks21[t][blockIndex21()];at+=b.tim;uint8_t g=grooveSheet40(t,p,b.groove,max(0,logical)/1536);
  if(!grooveBypass23&&g&&g<=GROOVE_COUNT){const auto&d=grooveOffset[g-1][(max(0,logical)/96)%GROOVE_STEPS];
    at+=grooveTime16(g,at);note=clampInt(note+d[0],0,127);vel=clampInt(vel+d[1],0,127);gate=clampInt((int64_t)gate*grooveGate51(g,max(0,logical)/96)/100,GATE_TICK_MIN,GATE_TICK_MAX);bend=0;}
}
void songClockTick() {
  Scope830 measured(P830_SONG);
  if(!playing) return;
  if(evaluateCurrentTickOnNextClock) evaluateCurrentTickOnNextClock=false; else ++songTick;
  uint64_t next=songLoopPosition(songTick,songLoopEnabled,songStartBar,songEndBar,1536);
  if(next!=songTick) { allNotesOff(); songTick=next; }
  recordErase820();
  songPlayZL();
  for(uint16_t i=0;i<SONG_CLIP_CAPACITY;++i) {
    const SongClip &c=songClips[i]; if(!c.used || trackSilent818(c.track)) continue;
    const uint64_t begin=(uint64_t)c.position*96,end=begin+(uint64_t)c.length*96;
    if(songTick==begin){programSend939(c.track,c.pattern);patch808PatternAt(c.track,c.pattern);}
    if(tickExpired830()||songTick+544<begin||songTick>=end) continue;
    const int32_t local=(int32_t)((int64_t)songTick-(int64_t)begin)+(int32_t)c.offset*96;
    if(local>=0&&songTick>=begin)freePlayTrack11(c.track,local,c.pattern);
    for(uint8_t blk=0;blk<3;++blk){BlockScope21 block(blk);
    if(c.pattern==activePattern[c.track]){
      uint8_t t=c.track;
      // Base TIM plus logical offset is bounded to 96 ticks; Groove is int8.
      // Seven nearby steps cover every possible onset without scanning 64 bars.
      int32_t source41=local-defaultMov41[t][0];int firstStep=max(0,source41/96-5),lastStep=min((int)trackStepCount(t)-1,source41/96+8);
      for(int stepIndex=firstStep;stepIndex<=lastStep;++stepIndex)for(uint8_t voice=0;voice<noteCountAtStep(t,stepIndex);++voice){
        if(pendingDeleteNote(t,stepIndex,voice))continue;
        blockResolved21=blockAtNote21(t,blk,c.pattern,stepIndex*96+logicalMicroTick(t,stepIndex,voice),noteAtVoice(t,stepIndex,voice));blockResolvedValid21=true;if(!blockResolved21.port)continue;
        int32_t at=defaultMov41[t][0]+(int32_t)stepIndex*96+logicalMicroTick(t,stepIndex,voice)+pendingEditedValue(t,stepIndex,voice,EDIT_TIME,effectiveMicroTick(t,stepIndex,voice));
        if(stepIndex<c.offset||stepIndex>=c.offset+c.length)continue;
        uint32_t gate=pendingEditedValue(t,stepIndex,voice,EDIT_GATE,effectiveGateTicks(t,stepIndex,voice));int note=visualNoteAtVoice(t,stepIndex,voice),vel=pendingEditedValue(t,stepIndex,voice,EDIT_VEL,effectiveVel(t,stepIndex,voice));int16_t bend=0;
        songApplyGroove(t,c.pattern,stepIndex*96,at,note,vel,gate,bend);
mod805SongApply(t,(int64_t)begin+(stepIndex-(int32_t)c.offset)*96,at,note,vel,gate);
mod805SongRoll(t,(int64_t)begin+(stepIndex-(int32_t)c.offset)*96,(int64_t)begin-(int64_t)c.offset*96+at,gate,note,configuredRollSettings(t,stepIndex,voice),end,(int32_t)stepIndex*96+logicalMicroTick(t,stepIndex,voice),noteAtVoice(t,stepIndex,voice),c.pattern,vel,rvlFixedLock938(t,stepIndex,voice));
if(at!=local)continue;
gate=min(gate,(uint32_t)(end-songTick));

        if(songLoopEnabled)gate=min((uint64_t)gate,(uint64_t)songEndBar*1536-songTick);
        triggerTrackNoteWithFixedGate(t,note,vel,max(1000UL,(uint32_t)(gate*tickDurationMs()*1000.0f)),bend,(int32_t)stepIndex*96+logicalMicroTick(t,stepIndex,voice),noteAtVoice(t,stepIndex,voice),c.pattern);
      }continue;
    }
    const SongPatternCache &cache=songCache[c.track][c.pattern]; if(!cache.valid) continue;
    for(uint32_t j=0;j<cache.count;++j) {
      const SongNote &n=songNotes[cache.first+j];if(n.logical<c.offset*96||n.logical>=(c.offset+c.length)*96)continue;
      blockResolved21=blockAtNote21(c.track,blk,c.pattern,songLockTick817[cache.first+j],n.note);blockResolvedValid21=true;if(!blockResolved21.port)continue;
      int32_t at=n.tick;
int note=n.note,vel=n.velocity;
uint32_t gate=n.gate;
int16_t bend=0;
songApplyGroove(c.track,c.pattern,n.logical,at,note,vel,gate,bend);
mod805SongApply(c.track,(int64_t)begin+n.logical-(int64_t)c.offset*96,at,note,vel,gate);
mod805SongRoll(c.track,(int64_t)begin+n.logical-(int64_t)c.offset*96,(int64_t)begin-(int64_t)c.offset*96+at,gate,note,songRoll805[cache.first+j],end,songLockTick817[cache.first+j],n.note,c.pattern,vel);
if(at!=local)continue;
gate=min(gate,(uint32_t)(end-songTick));

      if(songLoopEnabled && songTick<(uint64_t)songEndBar*1536)
        gate=min(gate,(uint32_t)min((uint64_t)UINT32_MAX,(uint64_t)songEndBar*1536-songTick));
      if(gate) triggerTrackNoteWithFixedGate(c.track,note,vel,
        max((uint32_t)1000,(uint32_t)(gate*tickDurationMs()*1000.0f)),bend,songLockTick817[cache.first+j],n.note,c.pattern);
    }
  }
}
}
#include "SongWorkspaceEngine.h"
#include "SongStorage.h"
#include "SongConversion.h"
#include "SdBrowse.h"
#include "StorageSync.h"
#include "SongExtract.h"
#include "SongProtocol.h"
#include "SongChecks.h"
FLASHMEM void songCommand(char *line) {
  mod805Invalidate();
  unsigned t,p,id,offset,length,enabled,first,last; unsigned long pos;
  if(nameCommand809(line)||storageCommand(line))return;
  if(songExtractCommand(line))return;
  if(!strcmp(line,"CHECK")) { songSelfCheck(); return; }
  if(!strcmp(line,"HELLO")||!strcmp(line,"SYNC")) { songSnapshot(); return; }
  if(sdBrowseCommand(line)||songConversionCommand(line))return;
  if(sscanf(line,"MODE,%u",&enabled)==1 && enabled<=1) { songSetMode(enabled); return; }
  if(songExtraCommand(line))return;
  if(!songEngineActive) { songError("ENTER_SONG_FIRST"); return; }
  if(sscanf(line,"SELECT,%u,%u",&t,&p)==2 && t<16&&p<16) { songSelect(t,p); return; }
  if(sscanf(line,"LOOP,%u,%u,%u",&enabled,&first,&last)==3) {
    if(enabled>1||first<1||last<first||last>62500000) { songError("INVALID_LOOP"); return; }
    songLoopEnabled=enabled; songStartBar=first; songEndBar=last; songState(); return;
  }
  if(!strcmp(line,"PLAY")) { songTransport(1,true); return; }
  if(!strcmp(line,"STOP")) { songTransport(2,true); return; }
  if(!strcmp(line,"PREV")) { songTransport(4,true); return; }
  if(!strcmp(line,"NEXT")) { songTransport(5,true); return; }
  if(!strcmp(line,"HOME")) { songTransport(3,true); return; }
  // Editing during playback is supported for cached sources; file IO is never
  // initiated while playing. A newly selected source must first be prepared.
  bool add=sscanf(line,"ADD,%u,%u,%lu",&t,&p,&pos)==3;
  if(add) {
    if(songDeleteArmed) { songError("DELETE_ARMED"); return; }
    if(t>=16||p>=16) { songError("INVALID_PATTERN"); return; }
    if(playing && !songCache[t][p].valid) { songError("PAUSE_TO_PREPARE_PATTERN"); return; }
    if(songLinkedPlace911(t,p,pos))return;
    uint16_t source=songPatternSteps(t,p);
    if(!source||!songPatternPresent(t,p)) { songError("EMPTY_PATTERN"); return; }
    id=0; while(id<128&&songClips[id].used) ++id;
    if(id==128) { songError("CLIP_CAPACITY"); return; }
    offset=0; length=source;
  } else if(sscanf(line,"EDIT,%u,%lu,%u,%u",&id,&pos,&offset,&length)==4) {
    if(songDeleteArmed) { songError("DELETE_ARMED"); return; }
    if(id>=128||!songClips[id].used) { songError("UNKNOWN_CLIP"); return; }
    t=songClips[id].track; p=songClips[id].pattern;
    const SongClip &old=songClips[id];
    if(offset<old.offset || (uint64_t)offset+length>(uint64_t)old.offset+old.length) {
      songError("TRIM_ONLY"); return;
    }
  } else if(sscanf(line,"DELETE,%u",&id)==1) {
    if(!songDeleteArmed) { songError("ENABLE_DELETE_ON_SEQUENCER"); return; }
    if(id>=128) { songError("UNKNOWN_CLIP"); return; }
    allNotesOff(); songClips[id].used=false; ++songRevision; songEmitClip(id); return;
  } else { songError("UNKNOWN_COMMAND"); return; }
  if(offset>65535||length>65535) { songError("INVALID_TRIM"); return; }
  SongClip candidate={(uint32_t)pos,(uint16_t)offset,(uint16_t)length,(uint8_t)t,(uint8_t)p,true};
  if(!songClipValid(candidate,songPatternSteps(t,p))) { songError("INVALID_TRIM"); return; }
  for(uint16_t i=0;i<128;++i) if(i!=id&&songClipsOverlap(candidate,songClips[i])) {
    songError("TRACK_OVERLAP"); return;
  }
  if(!songCachePattern(t,p)) return;
  if(!add) allNotesOff(); songClips[id]=candidate; ++songRevision; songEmitClip(id); songState();
}
FLASHMEM void songMidiMessage(uint8_t *data,unsigned int length) {
  if(controller940)return;
  if(length<8||length>156||data[0]!=0xF0||data[1]!=0x7D||data[2]!='U'||data[3]!='K'||
     data[4]!='S'||data[5]!=1||data[length-1]!=0xF7) return;
  char line[150]; memcpy(line,data+6,length-7); line[length-7]=0; songCommand(line);
}
FLASHMEM void songInit() {
  if (!full805Restored) songWorkspaceInit();
  songMidi.setHandleNoteOn(midiInOn17);songMidi.setHandleNoteOff(midiInOff17);
  songMidi.setHandleControlChange([](uint8_t channel,uint8_t number,uint8_t value){MidiInputScope53 scope53(1);if(channel>=1&&channel<=16){ccReceive941(channel-1,number,value);if(number==120||number==123)midiInRelease17(255,channel-1);}});
  songMidi.setHandleSystemExclusive(songMidiMessage); songUsb.begin();
}
void songService() {
  storageSync();
  songFlush();
  songUsb.Task(); for(uint8_t i=0;i<8&&songMidi.read();++i) {}
  midiInService17();
  bool connected=(bool)songMidi;
  if(connected!=songUsbConnected) {
    songUsbConnected=connected;if(!connected)midiInRelease17();
    Serial.println(connected?"SONG,USB_CONNECTED":"SONG,USB_DISCONNECTED");
    if(connected) songSnapshot();
  }
  if(!controller940&&songMode&&songHeldNeo>=0&&!songHeldSent&&millis()-songHeldSince>=neoSelectionHoldMs) {
    songHeldSent=true; songSelect(songHeldNeo*4+songHeldPad%4,patternRowOffset+songHeldPad/4);
  }
  if((connected||Serial)&&((songOutWrite+64-songOutRead)%64)<48&&millis()-songLastSendMs>=(zlSendCursor>=0?1:8)) {
    songLastSendMs=millis();
    if(songCatalogCursor>=0) {
      uint8_t t=songCatalogCursor/16,p=songCatalogCursor%16;
      bool present=songPatternPresent(t,p); uint16_t steps=songPatternSteps(t,p);
      songSend("PAT,%u,%u,%u,%u",t,p,present,steps);
      if(++songCatalogCursor==256) { songCatalogCursor=-1; songClipCursor=0; }
    } else if(songClipCursor>=0) {
      if(songClips[songClipCursor].used) songEmitClip(songClipCursor);
      if(++songClipCursor==128) { songClipCursor=-1; zlSendCursor=0; }
    } else if(zlSendCursor>=0){if((uint32_t)zlSendCursor<zlCount)songEmitZL(zlSendCursor++);else {zlSendCursor=-1;songSend("END");}}
  }
  if((connected||pc831.active)&&songStateDirty931) songState();
  if(songMode&&playing&&(connected||pc831.active)&&((songOutWrite+64-songOutRead)%64)<48&&millis()-songLastStateMs>=150u) { songLastStateMs=millis(); songPosition931(); }
}

