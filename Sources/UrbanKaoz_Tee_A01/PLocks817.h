#pragma once
#include "LockEvent30.h"
struct LockLease817 {
  uint32_t until,releaseUs,serial;
  int32_t value,previous;
  uint16_t target,parameter;
  uint8_t kind,channel,note;
  bool used,released;uint8_t outputMask=1;bool provisional39=false;bool rollHold44=false;uint64_t rollIdentity44=0;uint8_t owner50=255;
};
EXTMEM LockLease817 lockLeases817[256];
uint32_t lockSerial817=0;
uint16_t lockLeaseCount817=0;
EXTMEM NoteLock817 lockUndo817[2][LOCK_CAPACITY817];
uint16_t lockUndoCount817[2]={};
bool lockTest817=false;
uint32_t lockTestSends817=0;int lockTestLast817=0;
FLASHMEM bool lockMidiKind817(uint8_t kind){return kind==LOCK_CC817||kind==LOCK_NRPN7817||kind==LOCK_NRPN14817||kind==LOCK_PROGRAM26;}
FLASHMEM bool lockWantsEdit817(){return !patternMode&&((selectedStep>=0&&selectedVoice<noteCountAtStep(selectedTrack,selectedStep))||(explicitNoteSelection&&(selectionHasActiveNotes()||selectedEmptyCount)));}
FLASHMEM bool lockValid817(const NoteLock817&l){
 if(l.track>=16||(l.pattern>=16&&l.pattern!=254)||((l.note>=128)&&(l.duration<1||l.duration>65535))||(l.pattern!=254&&(l.tick< -48||l.tick>=1024*96+48))||l.kind>LOCK_BLOCK21)return false;
 if(l.kind==LOCK_BLOCK21)return l.target<16&&l.parameter<13&&l.value>=(l.parameter==12?0:l.parameter%4==2?-96:l.parameter%4==3?-127:0)&&l.value<=(l.parameter==12?64:l.parameter%4==0?8:l.parameter%4==1?15:l.parameter%4==2?96:127);
 if(l.kind==LOCK_PROGRAM26)return l.target<16&&l.parameter==0&&l.value>=0&&l.value<=128;
 if(l.kind==LOCK_CC817)return l.target<16&&l.parameter<128&&l.value>=0&&l.value<=127;
 if(l.kind==LOCK_ROLL1000)return l.target<16&&l.parameter<9&&l.value>=((l.parameter==2||l.parameter==3||l.parameter==5)?-127:0)&&l.value<=(l.parameter>=7?2:l.parameter==6?1535:l.parameter==0?102:(l.parameter==1||l.parameter==4)?1:127);
 if(l.kind==LOCK_NRPN7817||l.kind==LOCK_NRPN14817)return l.target<16&&l.parameter<16384&&l.value>=0&&l.value<=(l.kind==LOCK_NRPN14817?16383:127);
 if(l.kind==LOCK_DELAY817)return l.target<2048&&(l.parameter<12||l.parameter==15||l.parameter==16||l.parameter==17)&&l.value>=delayMin907(l.parameter)&&l.value<=(l.parameter==4?16:delayMax907(l.parameter));
 if(l.target>=64)return false;
 if(l.kind==LOCK_MOD817)return l.parameter<5&&l.value>=(modKind931(l.target)&&l.parameter==0?1:!modKind931(l.target)&&l.parameter==0?-384:0)&&l.value<=(modKind931(l.target)&&l.parameter==0?LFO_MAX42:modKind931(l.target)&&l.parameter==1?4:!(modKind931(l.target))&&l.parameter==3?127:65535);
 if(l.kind==LOCK_DEPTH817)return l.parameter<9&&l.value>=-32767&&l.value<=32767;
 if(l.kind==LOCK_EXTRA817)return l.parameter<5&&l.value>=(l.parameter<3?-100:0)&&l.value<=(l.parameter<3?100:l.parameter==3?6:360);
 return l.kind==LOCK_RAT817&&l.parameter==0&&l.value>=1&&l.value<=3072;
}
FLASHMEM int lockFind817(uint8_t t,uint8_t p,int32_t tick,uint8_t note,uint8_t kind,uint16_t target,uint16_t parameter){
 for(uint16_t i=0;i<lockCount817;++i){const auto&l=noteLocks817[i];if(l.track==t&&l.pattern==p&&l.tick==tick&&l.note==note&&l.kind==kind&&l.target==target&&l.parameter==parameter)return i;}return -1;
}
FLASHMEM int32_t lockTick817(uint8_t t,uint16_t s,uint8_t v){return (int32_t)s*96+logicalMicroTick(t,s,v);}
FLASHMEM bool lockEdit817(uint8_t kind,uint16_t target,uint16_t parameter,int base,int delta,int minimum,int maximum){
 if(indEdit50(kind,target,parameter,base,delta,minimum,maximum))return true;
 if(lockWantsEdit817()&&lockMidiKind817(kind)&&kind!=LOCK_PROGRAM26&&!deleteButtonActive){
  for(uint8_t t=0;t<16;++t){bool selected=selectedStep>=0?t==selectedTrack:false;if(explicitNoteSelection){for(uint16_t step=0;step<trackStepCount(t)&&!selected;++step)for(uint8_t v=0;v<noteCountAtStep(t,step);++v)selected|=selectedNoteObject(t,step,v);for(uint16_t i=0;i<selectedEmptyCount;++i)selected|=selectedEmptyCells[i].track==t;}if(!selected)continue;
   auto&c=patchDb808.context[linkOwner12(t,LINK_PATCH12)][patchContextIndex21(0)];auto*q=patchAddress10(c,kind==LOCK_CC817?0:1,parameter,kind==LOCK_NRPN14817?14:7);if(!q)continue;uint16_t v=65535;patchRawValue50(c,*q,v);if(indBase50(t,activePattern[t],kind,t,parameter,v)==65535){for(uint8_t screen=0;screen<4;++screen)lockBlinkSend11(screen,65535);DISPLAY_SERIAL.println("MSG|SET PATCH BASE FIRST");return true;}
  }
 }

 if(kind==LOCK_DEPTH817&&delta&&autoDepthEdit46(target,parameter,delta,minimum,maximum))return true;
 bool freeEdited=freeEdit11(kind,target,parameter,base,delta,minimum,maximum);
 bool held=selectedStep>=0&&selectedVoice<noteCountAtStep(selectedTrack,selectedStep);
 bool selected=explicitNoteSelection&&selectionHasActiveNotes();
 if(patternMode||(!held&&!selected))return freeEdited;
 uint16_t required=0;
 // Reserve all slots first: an edit of several notes is never half applied.
 for(uint8_t t=0;t<16;++t)for(uint16_t s=0;s<trackStepCount(t);++s)for(uint8_t v=0;v<noteCountAtStep(t,s);++v){
  if(!(held?(t==selectedTrack&&s==selectedStep&&v==selectedVoice):selectedNoteObject(t,s,v)))continue;
  uint16_t dest=(lockMidiKind817(kind)||kind==LOCK_ROLL1000||kind==LOCK_BLOCK21)?t:kind==LOCK_DELAY817?(target/256)*256+t*16+activePattern[t]:target;
  if(lockFind817(t,activePattern[t],lockTick817(t,s,v),noteAtVoice(t,s,v),kind,dest,parameter)<0){
   uint8_t count=0;for(uint16_t i=0;i<lockCount817;++i){const auto&l=noteLocks817[i];if(l.track==t&&l.pattern==activePattern[t]&&l.tick==lockTick817(t,s,v)&&l.note==noteAtVoice(t,s,v))++count;}
   if(count>=32&&!deleteButtonActive){DISPLAY_SERIAL.println("MSG|32 P.LOCKS PER NOTE");return true;}++required;
  }
 }
 if(!deleteButtonActive&&!lockDraftRoom26(required)){DISPLAY_SERIAL.println("MSG|P.LOCK CAPACITY");return true;}
 uint16_t edited=0;
 for(uint8_t t=0;t<16;++t)for(uint16_t s=0;s<trackStepCount(t);++s)for(uint8_t v=0;v<noteCountAtStep(t,s);++v){
  if(!(held?(t==selectedTrack&&s==selectedStep&&v==selectedVoice):selectedNoteObject(t,s,v)))continue;
  uint16_t dest=(lockMidiKind817(kind)||kind==LOCK_ROLL1000||kind==LOCK_BLOCK21)?t:kind==LOCK_DELAY817?(target/256)*256+t*16+activePattern[t]:target;
  int32_t tick=lockTick817(t,s,v);uint8_t note=noteAtVoice(t,s,v);
  int base59=indBase50(t,activePattern[t],kind,dest,parameter,base);
  NoteLock817 entry={tick,base59,dest,parameter,t,activePattern[t],note,kind};
  int old=lockDraftValue26(entry,base59);
  int next=(kind==LOCK_DELAY817&&(parameter<=2||parameter==4))?clampInt(old+delta,minimum,maximum):WRAP23(old,delta,minimum,maximum);
  entry.value=next;
  if(lockDraftPut26(entry,deleteButtonActive||next==base59))++edited;
  if(kind==LOCK_DELAY817)delayMarkWorked930(t,target/256);
 }
 if(held)markSelectedHeldPadEdited();
 if(edited){++modEpoch805;char msg[40];snprintf(msg,sizeof(msg),"MSG|P.LOCK %s %u",deleteButtonActive?"REMOVED":"SET",edited);DISPLAY_SERIAL.println(msg);}
 return true;
}
FLASHMEM int lockDisplay817(uint8_t kind,uint16_t target,uint16_t parameter,int base){
 base=indDisplay50(kind,target,parameter,base);
 if(kind==LOCK_DEPTH817)base=autoDepthDisplay46(target,parameter,base);
 if(selectedEmptyCount&&!selectionHasActiveNotes())return freeDisplay11(kind,target,parameter,base);
 uint8_t t=selectedTrack;uint16_t dest=(lockMidiKind817(kind)||kind==LOCK_ROLL1000||kind==LOCK_BLOCK21)?t:kind==LOCK_DELAY817?(target/256)*256+t*16+activePattern[t]:target;
 if(selectedStep>=0)return lockDraftValue26({lockTick817(t,selectedStep,selectedVoice),base,dest,parameter,t,activePattern[t],noteAtVoice(t,selectedStep,selectedVoice),kind},base);
 for(uint16_t s=0;s<trackStepCount(t);++s)for(uint8_t v=0;v<noteCountAtStep(t,s);++v)if(selectedNoteObject(t,s,v))return lockDraftValue26({lockTick817(t,s,v),base,dest,parameter,t,activePattern[t],noteAtVoice(t,s,v),kind},base);
 return selectedEmptyCount?freeDisplay11(kind,target,parameter,base):base;
}
// Active leases have a separate address index: note PLX releases never
// require scanning 256 unrelated leases for every roll parameter.
EXTMEM int16_t lockLeaseHeads30[512],lockLeaseLinks30[256];
static uint32_t lockLeaseEpoch30=UINT32_MAX,lockLeaseSerial30=UINT32_MAX;
static uint16_t lockLeaseSize30=UINT16_MAX;
FLASHMEM unsigned lockLeaseHash30(uint8_t kind,uint16_t target,uint16_t parameter){return (kind*1619u+target*31337u+parameter*6971u)&511;}
FLASHMEM int lockLeaseFirst30(uint8_t kind,uint16_t target,uint16_t parameter){
 if(lockLeaseEpoch30!=modEpoch805||lockLeaseSize30!=lockLeaseCount817||lockLeaseSerial30!=lockSerial817){
  memset(lockLeaseHeads30,255,sizeof(lockLeaseHeads30));
  for(int i=255;i>=0;--i){auto&l=lockLeases817[i];if(!l.used)continue;unsigned h=lockLeaseHash30(l.kind,l.target,l.parameter);lockLeaseLinks30[i]=lockLeaseHeads30[h];lockLeaseHeads30[h]=i;}
  lockLeaseEpoch30=modEpoch805;lockLeaseSize30=lockLeaseCount817;lockLeaseSerial30=lockSerial817;
 }
 return lockLeaseHeads30[lockLeaseHash30(kind,target,parameter)];
}
FLASHMEM int lockTop817(uint8_t kind,uint16_t target,uint16_t parameter){
 int found=-1;if(!playbackLocks828||!lockLeaseCount817)return found;
 for(int i=blockTargeted30?lockLeaseFirst30(kind,target,parameter):0;i>=0&&i<256;i=blockTargeted30?lockLeaseLinks30[i]:i+1){const auto&l=lockLeases817[i];if(l.used&&l.kind==kind&&l.target==target&&l.parameter==parameter&&(indContext50<0||l.owner50==255||l.owner50==indContext50)&&(!lockMidiKind817(kind)||(l.outputMask&midiMask902(target)))&&(found<0||(int32_t)(l.serial-lockLeases817[found].serial)>0))found=i;}return found;
}
FLASHMEM int lockValue817(uint8_t kind,uint16_t target,uint16_t parameter,int base){base=indLive50(kind,target,parameter,base);bool forecast=false;int scheduled=forecastValue50(kind,target,parameter,base,forecast);if(forecast)return scheduled;if(autoRenderTrack46>=0)return writeLockValue46(kind,target,parameter,base);int i=lockTop817(kind,target,parameter);return i<0?autoParameter46(kind,target,parameter,base):lockLeases817[i].value;}
FLASHMEM int lockOriginal11(uint8_t kind,uint16_t target,uint16_t parameter,int base){int i=lockTop817(kind,target,parameter);return i<0?base:lockLeases817[i].previous;}
FLASHMEM void nrpnPrevious54(uint8_t kind,uint8_t ch,uint16_t number,uint8_t port,int base){for(auto&l:lockLeases817)if(l.used&&l.kind==kind&&l.target==ch&&l.parameter==number&&(l.outputMask&(1u<<port)))l.previous=base;}
FLASHMEM int lockMidiOffer817(uint8_t channel,uint16_t object,int value){
 int i=lockTop817(LOCK_CC817,channel,object);if(i<0)return value;
 // Track the current underlying automation without overwriting the lock.
 for(auto&l:lockLeases817)if(l.used&&l.kind==LOCK_CC817&&l.target==channel&&l.parameter==object&&(l.outputMask&midiMask902(channel)))l.previous=value;
 return lockLeases817[i].value;
}
FLASHMEM void lockSendLocal54(uint8_t kind,uint8_t channel,uint16_t parameter,int value){
 if(lockTest817){++lockTestSends817;lockTestLast817=value;return;}
 if(kind==LOCK_PROGRAM26){if(value>0)midiRaw810(0xC0|channel,value-1,0);return;}
 uint8_t port54=0;while(port54<7&&!(midiMask902(channel)&(1u<<port54)))++port54;
 if(kind==LOCK_CC817){value=clampInt(value+(modMidiMix806[port54*16+channel][parameter].count?(int)lroundf(modMidiMix806[port54*16+channel][parameter].value()):0),0,127);
  uint8_t changed59=0;
  for(uint8_t port=0;port<8;++port)if(midiMask902(channel)&(1u<<port)){auto&p=midiPending810[port*16+channel][parameter];if(!p.known||p.last!=value)changed59|=1u<<port;if(p.pending){p.pending=false;--midiCounters810.pending;}p.last=value;p.known=true;p.sent=micros();}
  if(changed59){MidiScope902 changedRoute59(changed59);sendMidi3(0xB0|channel,parameter,value);}
 }else{
  value=clampInt(value+nrpnOffset54(port54,channel,parameter,kind==LOCK_NRPN14817?14:7),0,kind==LOCK_NRPN14817?16383:127);
  // Remove a stale queued NRPN for this same address before the atomic burst.
  auto link=&nrpnPending810;while(*link){auto*p=*link;if(p->channel==channel&&p->number==parameter&&(midiMask902(channel)&(1u<<p->output))){*link=p->next;extmem_free(p);}else link=&p->next;}
  sendMidi3(0xB0|channel,99,parameter>>7);sendMidi3(0xB0|channel,98,parameter&127);
  sendMidi3(0xB0|channel,6,kind==LOCK_NRPN14817?value>>7:value&127);
  if(kind==LOCK_NRPN14817)sendMidi3(0xB0|channel,38,value&127);
  sendMidi3(0xB0|channel,101,127);sendMidi3(0xB0|channel,100,127);
 }
}
FLASHMEM void lockSend817(uint8_t kind,uint8_t channel,uint16_t parameter,int value){
 if(lockTest817||kind==LOCK_PROGRAM26){lockSendLocal54(kind,channel,parameter,value);return;}uint8_t mask=midiMask902(channel);for(unsigned port=0;port<8;++port)if(mask&(1u<<port)){MidiScope902 scope(1u<<port);if(remoteAddress54(kind==LOCK_CC817?0:kind==LOCK_NRPN14817?3:2,port,channel,parameter))remoteLock54(kind,channel,parameter,value);else lockSendLocal54(kind,channel,parameter,value);}
}
FLASHMEM void lockDrop817(uint16_t index){
 auto old=lockLeases817[index];MidiScope902 route(old.outputMask,old.owner50);bool top=lockTop817(old.kind,old.target,old.parameter)==index;
 lockLeases817[index].used=false;--lockLeaseCount817;if(!lockMidiKind817(old.kind))leaseChanged31();if(old.kind==LOCK_BLOCK21)pidiRoutesDirty931|=1u<<old.target;
 if(top&&lockMidiKind817(old.kind))lockSend817(old.kind,old.target,old.parameter,lockValue817(old.kind,old.target,old.parameter,old.previous));
}
FLASHMEM void lockClearDraft39(){for(uint16_t i=0;i<256;++i)if(lockLeases817[i].used&&lockLeases817[i].provisional39)lockDrop817(i);}
FLASHMEM void lockService817(){
 if(!lockLeaseCount817)return;uint32_t now=micros();
 for(uint16_t i=0;i<256;++i)if(lockLeases817[i].used&&(int32_t)(now-lockLeases817[i].until)>=0)lockDrop817(i);
}
FLASHMEM void lockOff817(uint8_t channel,uint8_t note){
 if(!lockLeaseCount817)return;uint32_t now=micros();
 for(uint16_t i=0;i<256;++i){auto&l=lockLeases817[i];if(l.used&&!l.rollHold44&&!l.released&&l.channel==channel&&l.note==note&&(l.outputMask&midiMask902(channel))){l.released=true;l.until=now+l.releaseUs;if(!l.releaseUs)lockDrop817(i);}}
}
FLASHMEM void lockStop817(){if(!lockLeaseCount817)return;for(uint16_t i=0;i<256;++i)if(lockLeases817[i].used)lockDrop817(i);}
FLASHMEM void lockReset817(){freeDraftFinish25(false);freeInvalidate11();lockStop817();memset(lockLeases817,0,sizeof(lockLeases817));lockLeaseCount817=0;lockSerial817=0;lockCount817=0;memset(noteLocks817,0,sizeof(noteLocks817));memset(lockUndoCount817,0,sizeof(lockUndoCount817));}
FLASHMEM void lockBegin817(uint8_t t,uint8_t p,int32_t tick,uint8_t sourceNote,uint8_t channel,uint8_t note,uint32_t gateUs){
 if(sourceNote<128){delayNoteLocksReset925();if(remixLocks911(t,channel,note,gateUs))return;}
 if(!playbackLocks828||!lockPlaybackCount39()||(p!=254&&tick< -48))return;lockService817();
 uint16_t needed=0;for(int i=lockEventFirst30(t,p,tick,sourceNote);i>=0;i=lockEventNext30(i)){const auto&l=lockPlaybackEntry39(i);if((l.kind!=LOCK_BLOCK21||sourceNote>=128||(l.parameter==12||l.parameter/4==blockIndex21()))&&(l.kind!=LOCK_ROLL1000||sourceNote>=128)&&l.track==t&&l.pattern==p&&l.tick==tick&&l.note==sourceNote)++needed;}
 if(!needed)return;
 const bool hold44=rollHoldUs44!=0;if(hold44)gateUs=rollHoldUs44;
 if(lockLeaseCount817+needed>256){DISPLAY_SERIAL.println("MSG|P.LOCK VOICE LIMIT");return;}
 uint32_t now=micros(),release=0;bool modulationChanged59=false;
 if(lockRelease817&&sourceNote<128&&!hold44){uint16_t longest=0;for(uint16_t n=0;n<modEnabledCount805;++n){uint16_t key=modEnabled805[n];if(modKind931(key)||ioKey38(key))continue;auto m=mod808EffectiveUnit(key);if((m.inputs&(1u<<t))&&mod806Playing(key,songEngineActive?songTick:transportTickAbsolute))longest=max(longest,m.release);}release=(uint32_t)min(1800000000.0f,longest*tickDurationMs()*1000.0f);}
 for(int i=lockEventFirst30(t,p,tick,sourceNote);i>=0;i=lockEventNext30(i)){const auto&l=lockPlaybackEntry39(i);if(l.track!=t||l.pattern!=p||l.tick!=tick||l.note!=sourceNote)continue;
  if(l.kind==LOCK_BLOCK21&&sourceNote<128&&l.parameter!=12&&l.parameter/4!=blockIndex21())continue;
  if(l.kind==LOCK_ROLL1000&&sourceNote<128)continue; // Applied to its own roll, never leased to other notes.
  if(l.kind==LOCK_DELAY817&&sourceNote<128){delayNoteLock925(l.target/256,l.parameter,l.value);continue;}
  uint16_t target=l.target;int previous=0;
  if(hold44){bool exists=false;for(auto&lease:lockLeases817)if(lease.used&&lease.rollHold44&&lease.kind==l.kind&&lease.parameter==l.parameter&&lease.channel==channel&&lease.rollIdentity44==rollIdentity44&&lease.value==l.value&&lease.outputMask==outMask902(t)){exists=true;break;}if(exists)continue;}
  if(lockMidiKind817(l.kind)){
   auto&ctx=patch808Context(t,p<16?p:activePattern[t]);target=patch808Channel(t,ctx);
   int top=lockTop817(l.kind,target,l.parameter);
   if(top>=0)previous=lockLeases817[top].previous;
   else if(l.kind==LOCK_PROGRAM26)previous=programValue939(t,p<16?p:activePattern[t]);
   else if(l.kind==LOCK_CC817&&midiPending810[firstOut12(t)*16+target][l.parameter].known)previous=modMidiMix806[firstOut12(t)*16+target][l.parameter].count?modBase19[firstOut12(t)*16+target][l.parameter]:midiPending810[firstOut12(t)*16+target][l.parameter].last;
   else {for(auto*q=patchNext819(ctx,nullptr);q;q=patchNext819(ctx,q))if(q->number==l.parameter&&q->kind==(l.kind!=LOCK_CC817)){uint16_t value=0;patchValue819(ctx,*q,value);previous=value;break;}}
  }
  for(auto&lease:lockLeases817)if(!lease.used){lease={now+gateUs+release,release,++lockSerial817,l.value,previous,target,l.parameter,l.kind,channel,note,true,false,outMask902(t),lockPlaybackDraft39(i)};lease.owner50=t;lease.rollHold44=hold44;lease.rollIdentity44=rollIdentity44;++lockLeaseCount817;break;}
  if(!lockMidiKind817(l.kind))modulationChanged59=true;
  if(l.kind==LOCK_BLOCK21)pidiRoutesDirty931|=1u<<l.target;
  if(lockMidiKind817(l.kind))lockSend817(l.kind,target,l.parameter,l.value);
 }
 // MIDI leases replace the outgoing base only; count/serial invalidate the lease index.
 // Internal parameter leases also change the modulation graph and must invalidate it.
 if(modulationChanged59)leaseChanged31();
 // A release P.lock itself is included in the captured note lifetime.
 if(lockRelease817&&sourceNote<128&&!hold44){uint16_t longest=0;for(uint16_t n=0;n<modEnabledCount805;++n){uint16_t key=modEnabled805[n];if(modKind931(key)||ioKey38(key))continue;auto m=mod808EffectiveUnit(key);if((m.inputs&(1u<<t))&&mod806Playing(key,songEngineActive?songTick:transportTickAbsolute))longest=max(longest,m.release);}release=(uint32_t)min(1800000000.0f,longest*tickDurationMs()*1000.0f);
  for(auto&l:lockLeases817)if(l.used&&!l.rollHold44&&!l.released&&l.channel==channel&&l.note==note&&(l.outputMask&midiMask902(channel))){l.releaseUs=release;l.until=now+gateUs+release;}}
}
FLASHMEM void lockBeforeNote14(uint8_t channel,uint8_t note){
 if(!lockLeaseCount817)return;
 for(uint16_t i=0;i<256;++i){const auto&l=lockLeases817[i];if(!l.used||l.released||l.channel!=channel||l.note!=note||!lockMidiKind817(l.kind)||!(l.outputMask&midiMask902(channel)))continue;bool newer=false;for(uint16_t j=0;j<256;++j){const auto&x=lockLeases817[j];if(x.used&&!x.released&&x.channel==channel&&x.note==note&&x.kind==l.kind&&x.target==l.target&&x.parameter==l.parameter&&(x.outputMask&l.outputMask)&&x.serial>l.serial)newer=true;}if(!newer)lockSend817(l.kind,l.target,l.parameter,l.value);}
}
FLASHMEM void lockClearPattern817(uint8_t t,uint8_t p){freeInvalidate11();for(uint16_t i=0;i<lockCount817;)if(noteLocks817[i].track==t&&noteLocks817[i].pattern==p)noteLocks817[i]=noteLocks817[--lockCount817];else ++i;}
FLASHMEM void lockCopyPattern817(uint8_t ft,uint8_t fp,uint8_t tt,uint8_t tp){
 if(ft==tt&&fp==tp)return;uint16_t added=0,removed=0;
 for(uint16_t i=0;i<lockCount817;++i){const auto&l=noteLocks817[i];if(l.track==ft&&l.pattern==fp)++added;if(l.track==tt&&l.pattern==tp)++removed;}
 if(lockCount817-removed+added>LOCK_CAPACITY817){DISPLAY_SERIAL.println("MSG|P.LOCK COPY LIMIT");return;}
 lockClearPattern817(tt,tp);uint16_t count=lockCount817;
 for(uint16_t i=0;i<count;++i)if(noteLocks817[i].track==ft&&noteLocks817[i].pattern==fp){auto l=noteLocks817[i];l.track=tt;l.pattern=tp;if(lockMidiKind817(l.kind)||l.kind==LOCK_ROLL1000||l.kind==LOCK_BLOCK21)l.target=tt;if(l.kind==LOCK_DELAY817)l.target=(l.target/256)*256+tt*16+tp;noteLocks817[lockCount817++]=l;}
}
FLASHMEM void lockPrune817(uint8_t t,uint8_t p){
 for(uint16_t i=0;i<lockCount817;){auto&l=noteLocks817[i];if(l.track!=t||l.pattern!=p||l.note>=128){++i;continue;}bool found=false;
  int center=max(0,(l.tick+48)/96);for(int s=max(0,center-1);s<=min((int)trackStepCount(t)-1,center+1)&&!found;++s)for(uint8_t v=0;v<noteCountAtStep(t,s);++v)if(noteAtVoice(t,s,v)==l.note&&lockTick817(t,s,v)==l.tick){found=true;break;}
  if(found)++i;else l=noteLocks817[--lockCount817];
 }
}
FLASHMEM void lockRemoveNote817(uint8_t t,uint16_t step,uint8_t voice){
 if(lockMoving817)return;int32_t tick=lockTick817(t,step,voice);uint8_t note=noteAtVoice(t,step,voice);autoNoteRemove46(t,tick,note);if(!lockCount817)return;
 for(uint16_t i=0;i<lockCount817;)if(noteLocks817[i].track==t&&noteLocks817[i].pattern==activePattern[t]&&noteLocks817[i].tick==tick&&noteLocks817[i].note==note)noteLocks817[i]=noteLocks817[--lockCount817];else ++i;
}
FLASHMEM void lockPitch817(uint8_t t,uint16_t step,uint8_t voice,uint8_t pitch){lockEventInvalidate30();
 int32_t tick=lockTick817(t,step,voice);uint8_t old=noteAtVoice(t,step,voice);autoNotePitch46(t,tick,old,pitch);if(!lockCount817)return;
 for(uint16_t i=0;i<lockCount817;++i){auto&l=noteLocks817[i];if(l.track==t&&l.pattern==activePattern[t]&&l.tick==tick&&l.note==old)l.note=pitch;}
}
FLASHMEM void lockMove817(uint8_t t,int32_t tick,uint8_t pitch,int32_t destination){lockEventInvalidate30();autoNoteMove46(t,tick,pitch,destination);
 for(uint16_t i=0;i<lockCount817;++i){auto&l=noteLocks817[i];if(l.track==t&&l.pattern==activePattern[t]&&l.tick==tick&&l.note==pitch)l.tick=destination;}
}
FLASHMEM void lockUndoCapture817(uint8_t index,uint8_t t,uint16_t s,uint8_t v){
 lockUndoCount817[index]=0;int32_t tick=lockTick817(t,s,v);uint8_t note=noteAtVoice(t,s,v);autoNoteUndoCapture46(index,t,tick,note);
 for(uint16_t i=0;i<lockCount817;++i){auto&l=noteLocks817[i];if(l.track==t&&l.pattern==activePattern[t]&&l.tick==tick&&l.note==note&&lockUndoCount817[index]<32)lockUndo817[index][lockUndoCount817[index]++]=l;}
}
FLASHMEM bool lockUndoRoom817(uint8_t index){return autoNoteUndoRoom46(index)&&lockCount817+lockUndoCount817[index]<=LOCK_CAPACITY817;}
FLASHMEM void lockUndoInstall817(uint8_t index){autoNoteUndoInstall46(index);for(uint16_t i=0;i<lockUndoCount817[index];++i)noteLocks817[lockCount817++]=lockUndo817[index][i];}
FLASHMEM void lockUndoShift817(){autoNoteUndoShift46();deleteShift23();memmove(lockUndo817,lockUndo817+1,sizeof(lockUndo817[0]));memmove(lockUndoCount817,lockUndoCount817+1,sizeof(lockUndoCount817[0]));}

