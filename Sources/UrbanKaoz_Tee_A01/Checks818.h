#pragma once
FLASHMEM void checks818(){
 if(playing||!trackContext815()||kyBtnDown[0]||selectionHasActiveNotes()||selectedStep>=0||lockLeaseCount817){Serial.println("CHECK818,BUSY");return;}
 for(const auto&n:activeNotes)if(n.active){Serial.println("CHECK818,BUSY_NOTES");return;}
 bool oldMute[16];uint8_t oldVolume[16];memcpy(oldMute,trackMuted,16);memcpy(oldVolume,trackVolume818,16);
 uint16_t oldSolo=trackSolo818;uint8_t oldTrack=selectedTrack;trackTest818=true;trackReset818();
 uint16_t pass=0,fail=0;auto check=[&](bool ok,const char*name){if(ok)++pass;else ++fail;Serial.printf("CHECK818,%s,%s\n",ok?"OK":"FAIL",name);};
 auto ky=[](bool down){static bool held=false;if(down&&!held)dispatchStableButtonPress(52);held=down;};
 auto pad=[](uint8_t row,uint8_t t,bool down){onPadEdge(t/4,row*4+t%4,down?SEESAW_KEYPAD_EDGE_RISING:SEESAW_KEYPAD_EDGE_FALLING);};
 bool defaults=true;for(auto v:trackVolume818)defaults&=v==90;check(defaults,"volume_default_90");
 memset(trackMuted,0,16);bool select=true,preview=true,mute=true,solo=true;
 ky(true);check(trackChord815&&neoDisplayDirty,"press_enables_grid");
 ky(true);check(trackChord815,"repeated_down_does_not_toggle");
 ky(false);check(trackChord815,"release_keeps_grid_active");
 for(uint8_t t=0;t<16;++t){pad(0,t,true);pad(0,t,false);select&=selectedTrack==t;
   uint8_t before=selectedTrack;trackPreviewCapture818=255;pad(1,t,true);pad(1,t,false);preview&=trackPreviewCapture818==t&&selectedTrack==before;
   pad(2,t,true);pad(2,t,false);mute&=trackMuted[t];pad(2,t,true);pad(2,t,false);mute&=!trackMuted[t];
   pad(3,t,true);pad(3,t,false);solo&=trackSolo818==(1u<<t);pad(3,t,true);pad(3,t,false);solo&=!trackSolo818;
 }
 check(select,"row1_select_all_16_columns");check(preview,"row2_preview_without_select");check(mute,"row3_independent_mute");check(solo,"row4_toggle_solo");
 trackMuted[1]=true;pad(3,0,true);pad(3,0,false);check(!trackSilent818(0)&&trackSilent818(1)&&trackSilent818(2),"solo_filters_other_tracks");
 pad(3,2,true);pad(3,2,false);check(!trackSilent818(0)&&!trackSilent818(2)&&trackMuted[1],"multiple_solos_preserve_mutes");
 pad(3,0,true);pad(3,0,false);pad(3,2,true);pad(3,2,false);check(!trackSolo818&&trackMuted[1]&&!trackSilent818(2),"exit_solo_restores_mutes");
 // Release order must not turn the consumed chord into a musical note.
 pad(2,3,true);ky(true);ky(false);pad(2,3,false);check(!trackChord815&&!trackPads815[0][11]&&trackMuted[3],"toggle_off_before_pad_release");
 uint8_t t=selectedTrack;auto&ctx=patch808Context(t,activePattern[t]);uint32_t oldPatch=ctx.patch;int8_t oldChannel=ctx.channel;ctx.patch=0;ctx.channel=-1;
 uint8_t ch=midiChannelForTrack(t);auto pending=midiPending810[ch][7];auto counters=midiCounters810;
 applyParamDeltaFromKy(0,12);check(trackVolume818[t]==102&&midiPending810[ch][7].pending&&midiPending810[ch][7].value==102&&selectedTrack==t,"CC07_without_patch_grouped_delta");
 applyParamDeltaFromKy(0,1000);check(trackVolume818[t]==127,"volume_upper_bound");applyParamDeltaFromKy(0,-1000);check(trackVolume818[t]==0,"volume_lower_bound");
 midiPending810[ch][7]=pending;midiCounters810=counters;ctx.patch=oldPatch;ctx.channel=oldChannel;
 memcpy(trackMuted,oldMute,16);memcpy(trackVolume818,oldVolume,16);trackSolo818=oldSolo;trackTest818=false;
 selectEditingTrack(oldTrack);neoDisplayDirty=displayValueDirty=true;Serial.printf("CHECK818,%s,PASS=%u,FAIL=%u\n",fail?"FAIL":"PASS",pass,fail);
}
