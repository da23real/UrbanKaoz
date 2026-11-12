#pragma once
// No configuration reconstruction, hash, or periodic configuration resend at a note.
static bool manualSource1000=false;
static uint8_t delayDirty925[16]={};static uint16_t delayTracksDirty931=0;
static uint32_t delaySources924=0,delayRejected924=0,delayUpdates925=0;
static pidi827::Lock925 delayNoteLocks925[32];static uint8_t delayNoteLockCount925=0;
FLASHMEM void pidiProject925(){memset(delayDirty925,15,sizeof(delayDirty925));delayTracksDirty931=0xffff;}
FLASHMEM void pidiInvalidate924(){pidiProject925();}
FLASHMEM void pidiCancelDelays924(){pidiEnsure827();pidiPacket827(pidi827::CANCEL,nullptr,0);}
FLASHMEM void pidiDelayStatus924(){Serial.printf("DELAY925,ENGINE=PIDI,SOURCES=%lu,REJECTED=%lu,UPDATES=%lu,MAX_PER_TRACK=4\n",delaySources924,delayRejected924,delayUpdates925);for(uint8_t t=0;t<16;++t){Serial.printf("DLYTRACK09,T=%u,MASK=%u,MIXER=%u\n",t+1,delayMask925[t],mixerOn938(t,1));for(uint8_t slot=0;slot<4;++slot)if(delayPresent925(t,slot)){auto d=delayGet907(t*16+slot*256);Serial.printf("DLYCONFIG09,T=%u,S=%u,ON=%u,SOURCE=%u,CHAIN=%u,REP=%u,OUT=%u\n",t+1,slot,d.enabled,d.source,d.chain,d.repeats,d.output);}}}
FLASHMEM bool pidiObject925(uint8_t t,uint8_t s){
 pidi827::Object925 o{};o.track=t;o.slot=s;auto&c=o.config;c.length=1;
 if(delayPresent925(t,s)){auto d=delayGet907(t*16+s*256);c.wait=d.wait;c.length=d.length;c.repeats=d.repeats;c.velocity=d.velocity;c.track=(d.output==16?t:d.output)*3+d.block21;c.drum=trackPlayMode[linkOwner12(t,LINK_POLY12)]==TRACK_PLAY_DRUM;c.rolls=d.rolls;c.note=d.note;c.scdo=d.scdo==-128?0:d.scdo;c.mode=d.mode;c.accel=d.accel;c.axp=d.axp;c.oxp=d.oxp;c.gat08=d.gat08;c.gex08=d.gex08;c.enabled=d.enabled&&mixerOn938(t,1);c.source=!s||d.source;c.chain=d.chain&((1u<<s)-1);}
 delayDirty925[t]&=~(1u<<s);if(!delayDirty925[t])delayTracksDirty931&=~(1u<<t);
 if(!delayTargetsSend55(t,s)||!pidiPacket827(pidi827::OBJECT925,&o,sizeof(o))){delayDirty925[t]|=1u<<s;delayTracksDirty931|=1u<<t;++delayRejected924;return false;}++delayUpdates925;return true;
}
FLASHMEM void delayChanged925(uint8_t t,uint8_t s,uint8_t f,int value){
 for(uint8_t c=0;c<16;++c)if(c!=t&&linkOwner12(c,LINK_DLY12)==t){delayDirty925[c]|=1u<<s;delayTracksDirty931|=1u<<c;}

 if(!pidiStarted827||pidiReset827||delayImportPending925){delayDirty925[t]|=1u<<s;delayTracksDirty931|=1u<<t;return;}
 if(!mixerOn938(t,1)||(f==4&&value==16))f=255;
 if(f==4||f==17||f>=254||(delayDirty925[t]&(1u<<s))){delayDirty925[t]|=1u<<s;delayTracksDirty931|=1u<<t;pidiObject925(t,s);return;}
 pidi827::Value925 v{(int16_t)value,t,s,f};if(pidiPacket827(pidi827::VALUE925,&v,sizeof(v)))++delayUpdates925;else ++delayRejected924;
}
FLASHMEM void pidiSync925(){if(delayDetachPending55){uint8_t key=__builtin_ctzll(delayDetachPending55);if(pidiPacket827(50,&key,1))delayDetachPending55&=~(1ULL<<key);return;}if(delayImportPending925||!delayTracksDirty931)return;for(uint8_t t=0;t<16;++t)if(delayTracksDirty931&(1u<<t)){for(uint8_t s=0;s<4;++s)if(delayDirty925[t]&(1u<<s)){pidiObject925(t,s);return;}}}
FLASHMEM void pidiRoutes925(){
 if(!pidiRoutesDirty931)return;
 for(uint8_t c=0;c<16;++c)if(linkMask12[c]&((1u<<LINK_POLY12)|(1u<<LINK_MOUT12)))if(pidiRoutesDirty931&(1u<<linkSource12[c]))pidiRoutesDirty931|=1u<<c;
 for(uint8_t t=0;t<16;++t)if(pidiRoutesDirty931&(1u<<t)){
  pidiRoutesDirty931&=~(1u<<t);
  for(uint8_t owner=0;owner<16;++owner)for(uint8_t slot=0;slot<4;++slot)if(delayPresent925(owner,slot)){auto d=delayGet907(owner*16+slot*256);if((d.output<16?d.output:owner)==t){delayDirty925[owner]|=1u<<slot;delayTracksDirty931|=1u<<owner;}}
  for(uint8_t b=0;b<3;++b){BlockScope21 scope(b);pidi827::Route925 v{(uint8_t)(t*3+b),midiChannelForTrack(t),outMask902(t),0};pidiPacket827(pidi827::ROUTE925,&v,sizeof(v));}
 }
}
static bool stopHeld931=false,stopSent931=false;static uint32_t stopAt931=0;
FLASHMEM void pidiStopEdge931(bool pressed){if(pressed&&!stopHeld931){stopAt931=millis();stopSent931=false;}stopHeld931=pressed;}
FLASHMEM void pidiManual931(){pidiHealthy827=true;pidiProject925();pidiRoutesDirty931=0xffff;pidiStateValid827=false;pidiStateDirty931=true;}
FLASHMEM void pidiStopService931(){if(stopHeld931&&!stopSent931&&(uint32_t)(millis()-stopAt931)>=1200){stopSent931=true;pidiManual931();}}
FLASHMEM bool freeDelayHas11(uint8_t a){for(uint8_t i=0;i<delayNoteLockCount925;++i)if(delayNoteLocks925[i].address==a)return true;return false;}
FLASHMEM void delayNoteLocksReset925(){delayNoteLockCount925=0;}
FLASHMEM void delayNoteLock925(uint8_t s,uint8_t f,int value){if(s>=4||(f>=12&&f!=15&&f!=16&&f!=17))return;uint8_t address=s*32+f;for(uint8_t i=0;i<delayNoteLockCount925;++i)if(delayNoteLocks925[i].address==address){delayNoteLocks925[i].value=value;return;}if(delayNoteLockCount925<32)delayNoteLocks925[delayNoteLockCount925++]={(int16_t)value,address};else ++delayRejected924;}
FLASHMEM void pidiNote925(uint8_t t,uint8_t note,uint8_t vel,uint32_t gate,bool roll){
 if(controller940||t>=16||!mixerOn938(t,1)||!delayMask925[linkOwner12(t,LINK_DLY12)]){delayNoteLockCount925=0;return;}pidiEnsure827();pidiRoutes925();pidiStateSend827();
 for(uint8_t s=0;s<4;++s)if(delayDirty925[t]&(1u<<s))if(!pidiObject925(t,s)){delayNoteLockCount925=0;return;}
 freeDelaySnapshot11(t);
 for(uint8_t offset=0;offset<delayNoteLockCount925;){uint8_t bytes[62];uint8_t n=min((uint8_t)20,(uint8_t)(delayNoteLockCount925-offset));bytes[0]=offset;bytes[1]=n;memcpy(bytes+2,delayNoteLocks925+offset,n*sizeof(pidi827::Lock925));if(!pidiPacket827(pidi827::LOCKS925,bytes,2+n*sizeof(pidi827::Lock925))){delayNoteLockCount925=0;++delayRejected924;return;}offset+=n;}
 pidi827::Note925 n{gate,(uint32_t)max(1.0f,tickDurationMs()*1000.0f),t,note,vel,(uint8_t)((roll?1:0)|(manualSource1000?2:0)|((delayCut12&(1u<<linkOwner12(t,LINK_DLY12)))?4:0)),delayNoteLockCount925};delayNoteLockCount925=0;
 if(pidiPacket827(pidi827::NOTE925,&n,sizeof(n)))++delaySources924;else ++delayRejected924;
}

FLASHMEM void manualDelayPrepared06(uint8_t track,uint8_t note,uint8_t vel,uint32_t gate){manualSource1000=true;pidiNote925(track,note,vel,gate,false);manualSource1000=false;}
FLASHMEM void manualDelay1000(uint8_t track,uint8_t note,uint8_t vel,uint32_t gate){delayNoteLocksReset925();manualDelayPrepared06(track,note,vel,gate);}
