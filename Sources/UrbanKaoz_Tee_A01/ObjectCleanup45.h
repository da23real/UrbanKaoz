#pragma once
// Two sparse deletion histories. Rotation swaps indices, never copies a project.
struct RemovedRoute45 {ModRoute805 route;uint8_t source,slot;};
struct RemovedDelay55 {delay55::Target target;uint16_t key;uint8_t slot;};
struct RemovedZone45 {KbZoneR10 zone;uint8_t track,slot;};
struct ModRemoved45 {
 RemovedDelay55 delays55[272];uint16_t delayCount55=0;
 IndEntry50 ind[IndStore50::CAPACITY];uint16_t indCount=0;
 NoteLock817 locks[LOCK_CAPACITY817];RemovedRoute45 routes[512];RemovedZone45 zones[64];
 uint16_t count=0,routeCount=0;uint8_t zoneCount=0;
};
static EXTMEM ModRemoved45 removed45[2];
static uint8_t historyMap45[2]={0,1};
FLASHMEM void modHistoryPrepare45(uint8_t count){
 if(count==2){uint8_t old=historyMap45[0];historyMap45[0]=historyMap45[1];historyMap45[1]=old;count=1;}
 autoModHistoryReset46(historyMap45[count]);auto&h=removed45[historyMap45[count]];h.count=h.routeCount=h.zoneCount=h.indCount=h.delayCount55=0;
}
FLASHMEM bool lockTargets45(const NoteLock817&l,uint16_t key){return modLock931(l.kind)&&l.target==key;}
FLASHMEM uint16_t pruneLocks45(NoteLock817*locks,uint16_t count,uint16_t key){
 uint16_t n=0;for(uint16_t i=0;i<count;++i)if(!lockTargets45(locks[i],key))locks[n++]=locks[i];return n;
}
FLASHMEM void modDetach45(uint16_t key,uint8_t history){
 if(key>=64)return;if(history<2)autoModCapture46(key,historyMap45[history]);autoDetach46(key);
 if(modUnits805[key].enabled)delayDetachPending55|=1ULL<<key;
 ModRemoved45*h=history<2?&removed45[historyMap45[history]]:nullptr;
 auto detachDelay=[&](DelayValues907&v,uint16_t id){bool changed=false;for(uint8_t i=0;i<4;++i){auto&a=v.targets55[i];if((a.kind==5||a.kind==6)&&a.identity==key){if(h&&h->delayCount55<272)h->delays55[h->delayCount55++]={a,id,i};a={};changed=true;}}return changed;};
 for(auto*n=delayNodes907;n;n=n->next)if(detachDelay(n->value,n->key))delayChanged925((n->key%256)/16,n->key/256,255,0);
 for(uint8_t i=0;i<4;++i)detachDelay(delayDefaults932[i],2048+i);
 for(auto&e:ind50.entries)if(e.state==1&&modLock931(e.kind)&&e.target==key){if(h)h->ind[h->indCount++]=e;e.state=2;--ind50.count;}envEarly50[key]=0;indChanged50();
 if(h)for(uint16_t i=0;i<lockCount817;++i)if(lockTargets45(noteLocks817[i],key))h->locks[h->count++]=noteLocks817[i];
 lockCount817=pruneLocks45(noteLocks817,lockCount817,key);
 for(uint8_t i=0;i<2;++i)lockUndoCount817[i]=pruneLocks45(lockUndo817[i],lockUndoCount817[i],key);
 uint16_t n=0;for(uint16_t i=0;i<freeDraftCount25;++i)if(!lockTargets45(freeDraft25[i].lock,key))freeDraft25[n++]=freeDraft25[i];freeDraftCount25=n;
 for(uint16_t i=0;i<256;++i)if(lockLeases817[i].used&&modLock931(lockLeases817[i].kind)&&lockLeases817[i].target==key)lockDrop817(i);
 for(auto&bank:clips15)for(auto&c:bank)c.count=pruneLocks45(c.locks,c.count,key);
 for(auto&c:noteClips18)c.count=pruneLocks45(c.locks,c.count,key);
 previewContent18.count=pruneLocks45(previewContent18.locks,previewContent18.count,key);
 moveBefore939.lockCount=pruneLocks45(moveBefore939.locks,moveBefore939.lockCount,key);
 moveAfter939.lockCount=pruneLocks45(moveAfter939.locks,moveAfter939.lockCount,key);for(auto*s:{&moveBefore939,&moveAfter939})for(uint16_t i=0;i<s->muteCount;)if(s->mutes[i].key==key)s->mutes[i]=s->mutes[--s->muteCount];else ++i;
 for(auto&p:recordPads820)p.kbCount=pruneLocks45(p.kbLocks,p.kbCount,key);
 for(uint8_t k=0;k<64;++k)for(uint8_t j=0;j<8;++j){auto&r=modUnits805[k].outputs[j];if(r.enabled&&modIsTarget811(r)&&modTarget810(r)==key){if(h)h->routes[h->routeCount++]={r,k,j};r.enabled=0;}}
 for(uint8_t t=0;t<16;++t)for(uint8_t j=0;j<4;++j){auto&z=kbTracksR10[t].zone[j];if(z.type==KB_MOD_R10&&z.target==key){if(h)h->zones[h->zoneCount++]={z,t,j};z.target=65535;z.valid=0;}}
 freeInvalidate11();mod805Invalidate();neoDisplayDirty=displayValueDirty=true;
}
FLASHMEM bool modHistoryReady45(uint8_t index){
 auto&h=removed45[historyMap45[index]];auto&u=modHistory807[index];
 if(ind50.count+removed45[historyMap45[index]].indCount>IndStore50::CAPACITY){DISPLAY_SERIAL.println("MSG|IND UNDO CAPACITY");return false;}
 if(!autoModRoom46(historyMap45[index])){DISPLAY_SERIAL.println("MSG|AUTO UNDO CAPACITY");return false;}
 if(lockCount817+h.count>LOCK_CAPACITY817){DISPLAY_SERIAL.println("MSG|P.LOCK UNDO CAPACITY");return false;}
 if(!h.routeCount)return true;
 // Validate the complete candidate graph before touching active routes.
 memcpy(fullMods805,modUnits805,sizeof(fullMods805));fullMods805[u.key]=u.unit;
 for(uint16_t i=0;i<h.routeCount;++i){auto&r=h.routes[i];auto&dest=fullMods805[r.source].outputs[r.slot];if(!dest.enabled)dest=r.route;}
 bool ok=modGraphValid810(fullMods805);graphDirty810=true;
 if(!ok)DISPLAY_SERIAL.println("MSG|MOD LOOP REFUSED");return ok;
}
FLASHMEM void modHistoryRestore45(uint8_t index){
 auto&h=removed45[historyMap45[index]];autoModRestore46(historyMap45[index]);for(unsigned i=0;i<h.indCount;++i){auto&e=h.ind[i];ind50.set(e.owner,e.family,e.kind,e.target,e.parameter,e.value);}indChanged50();
 for(uint16_t i=0;i<h.count;++i){auto&l=h.locks[i];int at=lockFind817(l.track,l.pattern,l.tick,l.note,l.kind,l.target,l.parameter);if(at<0)noteLocks817[lockCount817++]=l;}
 for(uint16_t i=0;i<h.routeCount;++i){auto&r=h.routes[i];auto&dest=modUnits805[r.source].outputs[r.slot];if(!dest.enabled)dest=r.route;}
 for(uint8_t i=0;i<h.zoneCount;++i){auto&z=h.zones[i];auto&dest=kbTracksR10[z.track].zone[z.slot];if(dest.type==KB_MOD_R10&&dest.target==65535)dest=z.zone;}
 for(uint16_t i=0;i<h.delayCount55;++i){auto&a=h.delays55[i];if(a.key>=2048){auto&v=delayDefaults932[a.key-2048].targets55[a.slot];if(v.kind==255)v=a.target;}else{for(auto*n=delayNodes907;n;n=n->next)if(n->key==a.key&&n->value.targets55[a.slot].kind==255){n->value.targets55[a.slot]=a.target;delayChanged925((n->key%256)/16,n->key/256,255,0);break;}}}
 h.count=h.routeCount=h.zoneCount=h.indCount=h.delayCount55=0;freeInvalidate11();
}
FLASHMEM void lockDiagnostic45(const char*reason,uint16_t index,const NoteLock817&l){
 Serial.printf("FULL,PLOCK_DETAIL,WHY=%s,INDEX=%u,T=%u,P=%u,TICK=%ld,NOTE=%u,KIND=%u,TARGET=%u,PARAM=%u,VALUE=%ld,DURATION=%lu\n",reason,index,l.track,l.pattern,(long)l.tick,l.note,l.kind,l.target,l.parameter,(long)l.value,(unsigned long)l.duration);
}
FLASHMEM bool saveCheck45(bool clean){
 bool ok=lockCount817<=LOCK_CAPACITY817;uint16_t orphan=0;
 if(!ok){Serial.println("FULL,INVALID,PLOCK_COUNT");return false;}
 // Invalid values are diagnosed, never silently clamped. Missing objects are a
 // distinct legacy deletion case; only their references can be discarded.
 for(uint16_t i=0;i<lockCount817;++i){auto&l=noteLocks817[i];if(!lockValid817(l)){lockDiagnostic45("LIVE_VALUE",i,l);ok=false;}else if(modLock931(l.kind)&&!modUnits805[l.target].enabled){++orphan;lockDiagnostic45("ORPHAN",i,l);}}
 for(uint8_t k=0;k<64;++k)if(modUnits805[k].enabled)for(auto&r:modUnits805[k].outputs)if(r.enabled&&modIsTarget811(r)&&modTarget810(r)>=64){Serial.printf("FULL,INVALID,MOD_ROUTE,SOURCE=%u,TARGET=%u\n",k,modTarget810(r));ok=false;}
 if(clean&&ok)for(uint8_t k=0;k<64;++k)if(!modUnits805[k].enabled)modDetach45(k);
 Serial.printf("FULL,PREFLIGHT,VALID=%u,ORPHANS=%u,CLEANED=%u\n",ok,orphan,clean&&ok?orphan:0);
 return ok;
}
