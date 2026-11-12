#pragma once

FLASHMEM void trackInitRender57(){
 const char*names[]={"MOT","Patch","ENV","LFO","I/O","R/D","Lists","PAT"};
 char rows[4][21];for(auto&r:rows){memset(r,' ',20);r[20]=0;}
 for(unsigned i=0;i<8;++i){unsigned row=i<4?0:2,col=(i%4)*5;memcpy(rows[row]+col,names[i],strlen(names[i]));memcpy(rows[row+1]+col,(trackInitMask57&(1u<<i))?"[x]":"[ ]",3);}
 sendMenuScreen(rows[0],rows[1],rows[2],rows[3]);
}
FLASHMEM bool trackInitApply57(){
 uint8_t t=selectedTrack,mask=trackInitMask57;if(t>=16||!mask)return true;
 // Preserve tracks which inherited a group owned by this track before resetting it.
 uint8_t groups=0;if(mask&1)groups|=(1u<<LINK_MOUT12)|(1u<<LINK_VIEW12)|(1u<<LINK_POLY12)|(1u<<LINK_KB13);
 if(mask&2)groups|=1u<<LINK_PATCH12;if(mask&4)groups|=1u<<LINK_ENV12;if(mask&8)groups|=1u<<LINK_LFO12;if(mask&32)groups|=1u<<LINK_DLY12;
 for(uint8_t child=0;child<16;++child)if(child!=t&&linkSource12[child]==t){
  for(uint8_t g=0;g<8;++g)if((groups&linkMask12[child])&(1u<<g)){
   if(g==LINK_ENV12||g==LINK_LFO12){for(unsigned k=0;k<64;++k)if(modUnits805[k].enabled&&modKind931(k)==(g==LINK_LFO12)&&(modMeta806[k].tracks&(1u<<t)))modMeta806[k].tracks|=1u<<child;linkMask12[child]&=~(1u<<g);}
   else {selectedTrack=child;bool ok=linkToggle12(g);selectedTrack=t;if(!ok){trackInitRender57();return false;}}
  }
 }
 linkMask12[t]&=~groups;
 if(mask&128){
  clearPatternSelection();for(uint8_t p=0;p<16;++p)patternSelected[t][p]=true;
  deletePatternContext=true;deleteButtonActive=true;deleteNumeric55=false;deleteLocksMode15=false;
  if(!finishDeleteMode(true)){trackInitRender57();return false;}
  for(uint8_t p=0;p<16;++p){patternLinks911[t][p]=0;programPattern939[t][p]=0;}
  clearTrackPatternData(t);activePattern[t]=0;pendingPattern[t]=-1;clearGrooveBridge(t);
 }
 if(mask&1){
  fxSet32(t,harmony32::Config{});defaultVel[t]=100;defaultMidiNote[t]=36+t;defaultGateTicks[t]=48;defaultMicroTick[t]=0;trackPitchBend[t]=0;
  midiInRelease17(t);midiInChannel17[t]=0;trackMidiChannel[t]=t;trackOut902[t]=1;
  for(auto&b:blocks21[t]){uint8_t groove=b.groove;b={};b.groove=groove;}
  trackNoteViewMode[t]=-1;trackPlayMode[t]=TRACK_PLAY_MULTI;trackAutoSelectMode[t]=trackMultiEditMode[t]=trackNotePreviewMode[t]=-1;
  kbTracksR10[t]={};trackCcMask933&=~(1u<<t);trackCc933[t]=0;trackVolume818[t]=90;
 }
 if(mask&2){for(auto&c:patchDb808.context[t]){patch808FreeValues(c.values);c={};}programDefault939[t]=0;}
 for(unsigned k=0;k<64;++k){auto&m=modUnits805[k];if(!m.enabled)continue;unsigned family=ioKey38(k)?4:modKind931(k)?3:2;if(!(mask&(1u<<family)))continue;
  auto&meta=modMeta806[k];bool changed=(meta.tracks|m.inputs)&(1u<<t);meta.tracks&=~(1u<<t);m.inputs&=~(1u<<t);
  for(auto&r:m.outputs)if(r.enabled&&!modIsTarget811(r)&&r.kind!=MOD_VIRTUAL&&(!mod806MidiTarget(r)||modTrackMidi43(r))&&r.track==t){r.enabled=0;changed=true;}
  bool connected=meta.tracks||m.inputs||meta.midiInputs;for(auto&r:m.outputs)connected|=r.enabled;
  if(changed&&!connected){modDetach45(k);m.enabled=0;meta={};}
 }
 if(mask&32){for(uint8_t s=0;s<4;++s)delayRemove907(t*16+s*256);rollDefault15(t);trackRoll[t]={};trackRoll[t].start=trackRoll[t].speed=24;trackRoll[t].expo=expDefault15[0];rollMod827[t]=0;rollModAt827[t]=-1;rollDir939[t]=0;delayCut12&=~(1u<<t);delayMute933&=~(1u<<t);}
 if(mask&64){memset(trackLists44[t],0,sizeof(trackLists44[t]));trackListCounts44[t]=0;memset(patternGroove[t],0,sizeof(patternGroove[t]));memset(grooveLists[t],0,sizeof(grooveLists[t]));for(auto&b:blocks21[t])b.groove=0;}
 // Reset only the selected track's family overrides; note-level locks remain unless PAT is checked.
 uint8_t families=(mask&1?1u<<IND_BAS50:0)|(mask&2?1u<<IND_PATCH50:0)|(mask&4?1u<<IND_ENV50:0)|(mask&8?1u<<IND_LFO50:0)|(mask&32?(1u<<IND_ROL50)|(1u<<IND_DLY50):0)|(mask&64?1u<<IND_OOV50:0);
 for(auto&e:ind50.entries)if(e.state==1&&e.owner/16==t&&(families&(1u<<e.family))){e.state=2;--ind50.count;}
 for(unsigned p=0;p<16;++p){auto&f=ind50.flags[t*16+p];f=0;for(auto&e:ind50.entries)if(e.state==1&&e.owner==t*16+p)f|=128|(1u<<e.family);}
 mod807ClearHistory();mod805Invalidate();indChanged50();pidiRoutesDirty931=0xffff;pidiProject925();ensureSelectedTrackVisible();
 storageDirty931=neoDisplayDirty=displayValueDirty=true;return true;
}
