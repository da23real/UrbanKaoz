#pragma once
#include "KeyboardParams06.h"
// Four bounded values per track; held notes snapshot these on their rising edge.
FLASHMEM bool kbActiveZoneR10(uint8_t t,uint8_t z){auto s=kbSettings13(t).split;return z==0||(s==1&&z==2)||s==2;}
FLASHMEM void kbResetR10(){for(auto&t:kbTracksR10){t=KbTrackR10{};for(uint8_t z=1;z<4;++z){t.zone[z].type=KB_CC_R10;t.zone[z].center=64;t.zone[z].value=64;t.zone[z].target=z;}}}
FLASHMEM uint8_t kbZoneForR10(uint8_t t,uint8_t n,uint8_t p){auto&k=kbSettings13(t);uint8_t axis=k.panels?n:p/4;return k.split==0?0:k.split==1?(axis<2?0:2):axis;}
FLASHMEM uint8_t kbPositionR10(uint8_t t,uint8_t n,uint8_t p){auto&k=kbSettings13(t);if(!k.split)return (3-p/4)*16+n*4+p%4;return k.panels?(k.split==1?(3-p/4)*8+(n%2)*4+p%4:(3-p/4)*4+p%4):(k.split==1?(1-p/4%2)*16+n*4+p%4:n*4+p%4);}
FLASHMEM PatchParam808* kbPatchR10(uint8_t t,uint32_t id){auto&ctx=patch808Context(t,activePattern[t]);for(auto*q=patchNext819(ctx,nullptr);q;q=patchNext819(ctx,q))if(q->id==id)return q;return nullptr;}
FLASHMEM void kbBoundsR10(uint8_t t,const KbZoneR10&z,int&lo,int&hi){lo=0;hi=127;if(z.type==KB_AH_R10){auto*q=kbPatchR10(t,z.target);if(q&&q->kind&&q->bits==14)hi=16383;}if(z.type==KB_MOD_R10&&z.target<64){kbModBounds06(z.target,z.parameter,lo,hi);}if(z.type==KB_DLY_R10){lo=delayMin907(z.parameter);hi=z.parameter==4?16:delayMax907(z.parameter);}}
FLASHMEM bool kbLockR10(uint8_t t,const KbZoneR10&z,NoteLock817&l){
 l={};l.track=t;l.pattern=activePattern[t];l.value=z.value;l.parameter=z.parameter;l.target=z.target;
 if(!z.valid||z.type==KB_NOT_R10)return false;
 if(z.type==KB_CC_R10){l.kind=LOCK_CC817;l.target=t;l.parameter=z.target;}
 else if(z.type==KB_AH_R10){auto*q=kbPatchR10(t,z.target);if(!q)return false;l.kind=q->kind?(q->bits==14?LOCK_NRPN14817:LOCK_NRPN7817):LOCK_CC817;l.target=t;l.parameter=q->number;}
 else if(z.type==KB_MOD_R10){if(z.target>=64||!mod806Unit(z.target).enabled)return false;l.kind=z.parameter>=8?LOCK_DEPTH817:(modKind931(z.target)&&z.parameter>=2?LOCK_EXTRA817:LOCK_MOD817);l.parameter=z.parameter>=8?z.parameter-8:modKind931(z.target)&&z.parameter>=2?(z.parameter==2?4:3):z.parameter;if(l.kind==LOCK_DEPTH817&&!mod806Unit(z.target).outputs[l.parameter].enabled)return false;}
 else {if(z.target>=4)return false;l.kind=LOCK_DELAY817;l.target=z.target*256+t*16+activePattern[t];}
 return lockValid817(l);
}
FLASHMEM uint8_t kbSnapshotR10(uint8_t t,NoteLock817*out){uint8_t count=0;for(uint8_t z=0;z<4;++z)if(kbActiveZoneR10(t,z)){NoteLock817 l;if(!kbLockR10(t,kbSettings13(t).zone[z],l))continue;uint8_t i=0;while(i<count&&(out[i].kind!=l.kind||out[i].target!=l.target||out[i].parameter!=l.parameter))++i;out[i]=l;if(i==count)++count;}return count;}
FLASHMEM bool kbStoreR10(uint8_t t,uint8_t p,int32_t tick,uint8_t note,const NoteLock817*locks,uint8_t count){
 uint16_t needed=0,existing=0;for(uint16_t i=0;i<lockCount817;++i){auto&l=noteLocks817[i];if(l.track==t&&l.pattern==p&&l.tick==tick&&l.note==note)++existing;}
 for(uint8_t j=0;j<count;++j){auto l=locks[j];uint16_t target=lockMidiKind817(l.kind)?t:l.kind==LOCK_DELAY817?(l.target/256)*256+t*16+(p<16?p:activePattern[t]):l.target;if(lockFind817(t,p,tick,note,l.kind,target,l.parameter)<0)++needed;}
 if(lockCount817+needed>LOCK_CAPACITY817||existing+needed>32){DISPLAY_SERIAL.println("MSG|KB P.LOCK CAPACITY");return false;}
 for(uint8_t j=0;j<count;++j){auto l=locks[j];l.track=t;l.pattern=p;l.tick=tick;l.note=note;if(lockMidiKind817(l.kind))l.target=t;else if(l.kind==LOCK_DELAY817)l.target=(l.target/256)*256+t*16+(p<16?p:activePattern[t]);int i=lockFind817(t,p,tick,note,l.kind,l.target,l.parameter);if(i<0)i=lockCount817++;noteLocks817[i]=l;}++modEpoch805;return true;
}
FLASHMEM void kbStepR10(uint8_t t,uint16_t s,uint8_t v){NoteLock817 l[4];uint8_t count=kbSnapshotR10(t,l);kbStoreR10(t,activePattern[t],lockTick817(t,s,v),noteAtVoice(t,s,v),l,count);if(keyboardEnabled823&&notePreviewForTrack(t))triggerNoteAudition(t,noteAtVoice(t,s,v));}
FLASHMEM void kbLiveR10(uint8_t t,uint8_t note,uint8_t channel,const NoteLock817*locks,uint8_t count){
 delayNoteLocksReset925();uint32_t gate=1800000000UL; // Held KB leases release with their Note Off.
 for(uint8_t j=0;j<count;++j){auto l=locks[j];if(l.kind==LOCK_DELAY817){delayNoteLock925(l.target/256,l.parameter,l.value);continue;}
  uint16_t target=lockMidiKind817(l.kind)?patch808Channel(t,patch808Context(t,activePattern[t])):l.target;
  if(lockMidiKind817(l.kind)){autoManual46(t,l.kind,t,l.parameter,l.value);lockSend817(l.kind,target,l.parameter,l.value);}
  {bool placed=false;for(auto&lease:lockLeases817)if(!lease.used){lease={micros()+gate,0,++lockSerial817,l.value,lockMidiKind817(l.kind)?l.value:0,target,l.parameter,l.kind,channel,note,true,false,outMask902(t)};++lockLeaseCount817;placed=true;break;}if(!placed)DISPLAY_SERIAL.println("MSG|KB VOICE LIMIT");}
 }++modEpoch805;
}
FLASHMEM void kbAuditionR10(uint8_t t,uint8_t note,const NoteLock817*l,uint8_t count){MidiScope902 route(outMask902(t),t);uint8_t ch=midiChannelForTrack(t);uint32_t gate=clampInt((int)(defaultGateTicks[t]*tickDurationMs()*1000.0f),1000,20000000);if(scheduleNoteOffAtUs(ch,note,micros()+gate,true,true,t)){kbLiveR10(t,note,ch,l,count);if(noteOnNow(ch,note,defaultVel[t])){manualModStart06(t,255,gate);manualDelayPrepared06(t,note,defaultVel[t],gate);}}}
FLASHMEM bool kbPressR10(uint8_t n,uint8_t p,uint8_t&note,NoteLock817*locks,uint8_t&count){uint8_t t=keyboardTrack823(),id=kbZoneForR10(t,n,p);auto&z=kbSettings13(t).zone[id];int lo,hi;kbBoundsR10(t,z,lo,hi);int pads=kbSettings13(t).split==0?64:kbSettings13(t).split==1?32:16;int pos=kbPositionR10(t,n,p);z.value=clampInt(kbCenterFor12(t,id)+(kbSettings13(t).split&&!kbSettings13(t).panels?(pads/2-pos):(pos-pads/2))*z.range,lo,hi);z.valid=1;
 if(z.type==KB_NOT_R10){note=z.value;if(!recordSuspended823()&&!selectionHasActiveNotes())defaultMidiNote[t]=note;}else note=defaultMidiNote[t];count=kbSnapshotR10(t,locks);displayValueDirty=true;return !z.mute;}
FLASHMEM bool kbSelectR10(uint8_t t,bool pitch,uint8_t note,const NoteLock817*l,uint8_t count){
 if(!selectionHasActiveNotes())return false;

 // Reserve an upper bound before changing any selected note.
 uint16_t notes=0;for(uint8_t tr=0;tr<16;++tr)for(uint16_t s=0;s<trackStepCount(tr);++s)for(uint8_t v=0;v<noteCountAtStep(tr,s);++v)if(selectedNoteObject(tr,s,v))++notes;
 if(lockCount817+(uint32_t)notes*count>LOCK_CAPACITY817){DISPLAY_SERIAL.println("MSG|KB P.LOCK CAPACITY");return true;}
 if(pitch)stageSelectedParamValue(EDIT_PITCH,note,false);
 for(uint8_t tr=0;tr<16;++tr)for(uint16_t s=0;s<trackStepCount(tr);++s)for(uint8_t v=0;v<noteCountAtStep(tr,s);++v){if(!selectedNoteObject(tr,s,v))continue;kbStoreR10(tr,activePattern[tr],lockTick817(tr,s,v),noteAtVoice(tr,s,v),l,count);}
 mod805Invalidate();displayValueDirty=neoDisplayDirty=true;return true;
}
FLASHMEM void kbSongRemoveR10(uint32_t id){for(uint16_t i=0;i<lockCount817;)if(noteLocks817[i].pattern==254&&(!id||(uint32_t)noteLocks817[i].tick==id))noteLocks817[i]=noteLocks817[--lockCount817];else ++i;}
FLASHMEM void kbSongPitchR10(uint32_t id,uint8_t note){lockEventInvalidate30();for(uint16_t i=0;i<lockCount817;++i)if(noteLocks817[i].pattern==254&&(uint32_t)noteLocks817[i].tick==id)noteLocks817[i].note=note;}


#include "KeyboardZones05.h"
#include "KeyboardStorage05.h"
