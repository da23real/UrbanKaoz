#pragma once
// Slot 4 owns this typed payload just as the other slots own notes/patterns.
// Every ordinary overwrite releases it; there is no hidden Remix clipboard.
FLASHMEM void releaseRemixClipboard915(uint8_t slot){
 if(slot>=CLIPBOARD_COUNT)return;contentClearClip18(slot);auto&c=clipboardSlots[slot];
 if(c.remix){c.remix->~Model();extmem_free(c.remix);c.remix=nullptr;}
}
FLASHMEM bool remixStoreClipboard915(){
 if(remixAnchor911<0&&!remixCopySelection911.steps)return false;
 const auto&src=remixSource911;rmx911::Model fragment;memcpy(fragment.tracks,src.tracks,sizeof(fragment.tracks));
 uint16_t mask=remixCopySelection911.steps?remixCopySelection911.tracks:remixSourceMask911;
 int anchor=remixAnchor911<0?0:remixAnchor911;uint16_t selected=0;
 if(remixCopySelection911.steps){anchor=0;while(!(remixCopySelection911.steps&(1u<<(anchor/96))))anchor+=96;
  for(unsigned i=0;i<16;++i)if(remixCopySelection911.steps&(1u<<i))selected|=1u<<rmx911::wrap(i-anchor/96,16);
 }
 for(uint32_t i=0;i<src.notes.size;++i){const auto&n=src.notes[i];if(!(mask&(1u<<n.track)))continue;
  int bar=remixCopySelection911.steps?remixCopySelection911.bar:remixSourceBar911;
  int origin=(bar%src.tracks[n.track].bars)*1536+anchor+(selected?0:remixLatency911);
  int offset=rmx911::wrap(n.tick-origin,src.tracks[n.track].bars*1536);
  if(selected?!remixCopySelection911.contains(n,src,true):offset>=remixLength911)continue;
  if(!fragment.append(src,n,n.track,offset,1536)){remixError911("CLIP4 MEMORY FULL");return false;}
 }
 for(auto&t:fragment.tracks)t.bars=1;fragment.mask=mask;
 auto*payload=rmx911::History::copy(fragment);if(!payload){remixError911("CLIP4 MEMORY FULL");return false;}
 releaseRemixClipboard915(3);auto&clip=clipboardSlots[3];memset(&clip,0,sizeof(clip));clip.remix=payload;clip.remixSteps=selected;clip.remixTicks=remixLength911;clip.type=CLIP_TYPE_REMIX;clip.valid=true;clip.lengthSteps=16;clip.sourceTrack=selectedTrack;
 clipboardFineCount[3]=0;patternClipboard[3].valid=false;return true;
}
FLASHMEM bool remixPasteClipboard915(uint8_t pad){
 const auto&clip=clipboardSlots[3];if(!clip.valid||clip.type!=CLIP_TYPE_REMIX||!clip.remix){remixError911("CHOOSE SOURCE / CLIP4");return false;}
 rmx911::Model stage;if(!stage.clone(remixDraft911)){remixError911("REMIX MEMORY FULL");return false;}
 const auto&src=*clip.remix;int destination=pad*96+remixLatency911;
 for(uint32_t i=0;i<stage.notes.size;){const auto&n=stage.notes[i];int offset=rmx911::wrap(n.tick-destination,1536);
  bool erase=(src.mask&(1u<<n.track))&&(clip.remixSteps?(clip.remixSteps&(1u<<(offset/96))):offset<clip.remixTicks);
  if(erase)stage.remove(i);else ++i;
 }
 stage.mask|=src.mask;for(uint32_t i=0;i<src.notes.size;++i)if(!stage.append(src,src.notes[i],src.notes[i].track,destination+src.notes[i].tick,1536)){remixError911("REMIX MEMORY FULL");return false;}
 return remixCommitStage911(stage,remixDraft911,remixHistory911);
}
FLASHMEM void remixSeekPad915(uint8_t pad){remixPosition916(0,pad);}
FLASHMEM void remixPosition916(uint8_t role,uint8_t pad){
 if(!remixFinishTake913())return;remixRecord913=false;remixAudition911.active=false;remixChainRole916=role;
 resetGrooveBridges();allNotesOff();
 noInterrupts();transportTickAbsolute=(uint64_t)(role?remixTargetBar911:remixSourceBar911)*1536+pad*96;pendingClockTicks=0;interrupts();
 remixChainBar917=transportTickAbsolute/1536;remixPhaseOffset917=remixHearBar911?(int)(role?remixHearBar911-1:0)-(int)(remixChainBar917%remixHearBar911):0;
 currentStep=globalLoopedTick()/96;measureTick=localTickForTrack(selectedTrack);lastDisplayedStep=0xFFFF;evaluateCurrentTickOnNextClock=true;invalidateStepTriggers();neoDisplayDirty=true;
}
