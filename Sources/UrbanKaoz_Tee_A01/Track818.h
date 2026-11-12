#pragma once
static bool trackTest818=false;static uint8_t trackPreviewCapture818=255;
FLASHMEM bool trackSilent818(uint8_t t){return t>=16||trackMuted[t]||(trackSolo818&&!(trackSolo818&(1u<<t)));}
FLASHMEM void trackAction818(uint8_t row,uint8_t t){
 if(t>=16||row>3)return;
 if(modMixer939){modMixerToggle28(t,row);return;}row=mixerFunction938(t,row);if(row>=4){mixerToggle938(t,row-4);return;}
 if(row==0){selectEditingTrack(t);if(songEngineActive)songSelect(t,songSelectedPattern);followTrack817();if(trackMenuActive){if(linkMenu12){linkDraft12=linkMask12[t]?linkSource12[t]:(t?0:1);linkRender12();}else if(trackMidiMenu902){trackMidiDraft902=outMask902(t);sendTrackMidi902();}else if(kbMenuR10)kbRenderMenuR10();else sendTrackMenu();}}
 else if(row==1){if(trackTest818)trackPreviewCapture818=t;else {NoteLock817 l[4];uint8_t count=kbSnapshotR10(t,l);kbAuditionR10(t,defaultMidiNote[t],l,count);}}
 else {if(row==2)(pidiStateDirty931=true, trackMuted[t]=!trackMuted[t]);else (pidiStateDirty931=true, trackSolo818^=1u<<t);
   for(uint8_t i=0;i<16;++i)if(trackSilent818(i))stopActiveNotesForTrack(i);
   ++modEpoch805;
 }
 neoDisplayDirty=displayValueDirty=true;
}
FLASHMEM void trackVolumeTurn818(int32_t delta){
 uint8_t t=selectedTrack;int next=clampInt((int)trackVolume818[t]+delta,0,127);
 if(next==trackVolume818[t])return;trackVolume818[t]=next;
 auto&ctx=patch808Context(t,activePattern[t]);
 // Keep an explicitly assigned CC07 baseline aligned with the direct volume control.
 for(auto*q=patchNext819(ctx,nullptr);q;q=patchNext819(ctx,q))if(!q->kind&&q->number==7)patch808Set(ctx,q->id,next);
 {MidiScope902 route(outMask902(t),t);midiOffer810(patch808Channel(t,ctx),7,next);}displayValueDirty=true;
}
FLASHMEM void trackReset818(){for(auto&c:fxConfig32)c=harmony32::Config{};fxRebuild32();blockReset21();midiInRelease17();memset(midiInChannel17,0,16);linkReset12();kbResetR10();programReset939();mixerReset938();memset(trackVolume818,90,sizeof(trackVolume818));(pidiStateDirty931=true, trackSolo818=0);trackChord815=false;memset(trackPads815,0,sizeof(trackPads815));}
