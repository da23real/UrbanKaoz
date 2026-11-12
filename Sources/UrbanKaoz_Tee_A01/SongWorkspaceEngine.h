#pragma once
FLASHMEM void songWorkspaceInit(){
  songWorkspaceReady=external_psram_size>=8;
  // EXTMEM has no static initialization guarantee. Only committed counts expose data.
  zlCount=0;
}
FLASHMEM void songExtraState(){
  songSend("EXTRA,%u,%u,%u,%lu,%u,%u,%u,%lu",songEngineActive,songRecord,songView,
    (unsigned long)zlCount,external_psram_size,0,0,(unsigned long)songRevision);
}
FLASHMEM void songLeaveSurface(){songMode=false;songDeleteArmed=false;songExportArmed=false;selectButtonActive=selectionMode=shiftButtonActive=columnSelectionMode=false;selection805Reset();patternMode=false;(pidiStateDirty931=true, remixMode=false);grooveEditMode=false;remixLeave911();songState();}
FLASHMEM void songEmitZL(uint32_t i){const auto&e=zlEvents[i];songSend("ZL,%lu,%u,%u,%llu,%lu,%u,%u",(unsigned long)e.id,e.track,e.kind,(unsigned long long)e.tick,(unsigned long)e.gate,e.a,e.b);}
FLASHMEM bool zlValid(const ZLEvent&e){return e.track<16&&e.kind<2&&e.a<128&&e.b<128&&e.gate>0&&e.tick<=((uint64_t)SONG_MAX_STEP*96)&&e.gate<=((uint64_t)SONG_MAX_STEP*96-e.tick)&&(e.kind||e.b>0);}
FLASHMEM bool zlInsert(ZLEvent e){
  if(!songWorkspaceReady){songError("PSRAM_REQUIRED");return false;}
  if(!zlValid(e)||zlCount>=ZL_CAPACITY||zlNextId==UINT32_MAX){songError("ZL_CAPACITY_OR_RANGE");return false;}
  e.id=zlNextId++;uint32_t i=zlCount;while(i&&zlEvents[i-1].tick>e.tick){zlEvents[i]=zlEvents[i-1];--i;}
  zlEvents[i]=e;++zlCount;++songRevision;songEmitZL(i);return true;
}
void songPlayZL(){
  if(!songWorkspaceReady)return;
  uint64_t earliest=songTick>192?songTick-192:0,latest=songTick+192;
  uint32_t lo=0,hi=zlCount;while(lo<hi){uint32_t m=(lo+hi)/2;if(zlEvents[m].tick<earliest)lo=m+1;else hi=m;}
  for(uint32_t i=lo;i<zlCount&&zlEvents[i].tick<=latest;++i){const auto&e=zlEvents[i];if(trackMuted[e.track])continue;
    for(uint8_t blk=0;blk<3;++blk){BlockScope21 block(blk);if(!outMask902(e.track))continue;
    if(e.kind){MidiScope902 route(outMask902(e.track),e.track);if(e.tick==songTick){midiForce810(midiChannelForTrack(e.track),e.a);sendMidi3(0xB0|midiChannelForTrack(e.track),e.a,e.b);}continue;}
    if(tickExpired830())continue;
    int offset=clampInt(mod805Offset(e.track,MOD_TIM,e.tick),-96,96)+blocks21[e.track][blk].tim;if((int64_t)e.tick+offset!=(int64_t)songTick)continue;
    uint32_t gate=max((int64_t)1,(int64_t)e.gate+mod805Offset(e.track,MOD_GAT,e.tick));
    if(songLoopEnabled)gate=min((uint64_t)gate,(uint64_t)songEndBar*1536-songTick);
    uint8_t note=clampInt((int)e.a+mod805Offset(e.track,MOD_NOT,e.tick),0,127),vel=clampInt((int)e.b+mod805Offset(e.track,MOD_VEL,e.tick),1,127);
    triggerTrackNoteWithFixedGate(e.track,note,vel,max(1000UL,(uint32_t)(gate*tickDurationMs()*1000.0f)),0,(int32_t)e.id,e.a,254);
    }
  }
}
