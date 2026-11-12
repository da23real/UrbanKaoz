#pragma once
const char*const mixerNames938[]={"Sel","Prev","Mute","Solo","Roll","Dely","Env","Lfo"};
FLASHMEM void mixerReset938(){modOff28=0;memset(mixerMap938,0,sizeof(mixerMap938));memset(mixerOff938,0,sizeof(mixerOff938));}
FLASHMEM void mixerRender938(){char title[21],names[21],rows[21];snprintf(title,21,"TRACK %02u MIXER",selectedTrack+1);memset(names,' ',20);memset(rows,' ',20);names[20]=rows[20]=0;for(uint8_t i=0;i<4;++i){const char*n=mixerNames938[mixerFunction938(selectedTrack,i)];memcpy(names+i*5,n,strlen(n));rows[i*5]='1'+i;}sendMenuScreen(title,"OA   OB   OC   OD",rows,names);}
FLASHMEM void mixerTurn938(uint8_t row,int delta){if(row>3||!delta)return;uint16_t shift=row*3,mask=7u<<shift;int f=clampInt(mixerFunction938(selectedTrack,row)+(delta>0?1:-1),0,7);uint16_t map=mixerMap938[selectedTrack]^MIX_DEFAULT938;map=(map&~mask)|(f<<shift);mixerMap938[selectedTrack]=map^MIX_DEFAULT938;neoDisplayDirty=true;mixerRender938();}
FLASHMEM bool mixerMod938(uint16_t key){if(key>=64||(modOff28&(1ULL<<key)))return false;uint16_t owners=modMeta806[key].tracks;uint16_t off=mixerOff938[modKind931(key)?3:2];return !owners||(owners&~off);}
FLASHMEM void mixerToggle938(uint8_t t,uint8_t kind){if(t>=16||kind>=4)return;mixerOff938[kind]^=1u<<t;
 if(kind==1)for(uint8_t s=0;s<4;++s)delayChanged925(t,s,255,0);
 if(kind>=2){mod805Invalidate();modRecall937(t);}else ++modEpoch805;
 neoDisplayDirty=displayValueDirty=true;
 char msg[40];snprintf(msg,sizeof(msg),"MSG|T%02u %s %s",t+1,mixerNames938[kind+4],mixerOn938(t,kind)?"ON":"OFF");DISPLAY_SERIAL.println(msg);
}
FLASHMEM bool mixerActive938(uint8_t t,uint8_t row,bool preview){if(modMixer939){uint16_t key=modMixerKey28(t,row);return key<64&&!(modOff28&(1ULL<<key));}switch(mixerFunction938(t,row)){case MIX_SELECT938:return t==selectedTrack;case MIX_PREV938:return preview;case MIX_MUTE938:return trackMuted[t];case MIX_SOLO938:return trackSolo818&(1u<<t);default:return mixerOn938(t,mixerFunction938(t,row)-4);}}
FLASHMEM bool rvlFixedLock938(uint8_t t,uint16_t s,uint8_t v){if(pendingSelectionEdit.active&&pendingSelectionEdit.roll[5].touched&&selectedNoteObject(t,s,v))return true;int f=fineIndexForVoice(t,s,v);return f>=0?(fineNote[t][f].pxMask&(1u<<(PX_ROLL_SHIFT+5))):(stepRollLockMask[t][s]&(1u<<5));}
FLASHMEM bool rvlLock938(uint8_t t,uint8_t p,int32_t tick,uint8_t note,bool fixed){Scope830 rvlMeasure30(P830_RVL30);if(!playbackLocks828)return false;if(fixed)return true;for(int i=lockEventFirst30(t,p,tick,note);i>=0;i=lockEventNext30(i)){const auto&l=lockPlaybackEntry39(i);if(l.kind==LOCK_ROLL1000&&l.track==t&&l.pattern==p&&l.tick==tick&&l.note==note&&(l.parameter==1||l.parameter==2))return true;}return false;}
FLASHMEM bool rollAllowed938(uint8_t t,uint8_t p,int32_t tick,uint8_t note,bool fixed){return mixerOn938(t,0)||rvlLock938(t,p,tick,note,fixed);}
FLASHMEM bool rollNoteAllowed938(uint8_t t,uint16_t s,uint8_t v){if(mixerOn938(t,0))return true;return rollAllowed938(t,activePattern[t],(int32_t)s*96+logicalMicroTick(t,s,v),noteAtVoice(t,s,v),rvlFixedLock938(t,s,v));}
static uint16_t mixerStageMap938[16],mixerStageOff938[4];
FLASHMEM bool mixerWrite938(File&f){uint8_t count=0;for(uint8_t t=0;t<16;++t){uint8_t off=0;for(uint8_t k=0;k<4;++k)off|=(!mixerOn938(t,k))<<k;if(mixerMap938[t]||off)++count;}if(f.write(&count,1)!=1)return false;for(uint8_t t=0;t<16;++t){uint8_t off=0;for(uint8_t k=0;k<4;++k)off|=(!mixerOn938(t,k))<<k;if(!mixerMap938[t]&&!off)continue;if(f.write(&t,1)!=1||f.write((uint8_t*)&mixerMap938[t],2)!=2||f.write(&off,1)!=1)return false;}return true;}
FLASHMEM bool mixerRead938(File&f,bool present){memset(mixerStageMap938,0,sizeof(mixerStageMap938));memset(mixerStageOff938,0,sizeof(mixerStageOff938));if(!present)return true;uint8_t count;if(f.read(&count,1)!=1||count>16)return false;uint16_t seen=0;while(count--){uint8_t t,off;uint16_t map;if(f.read(&t,1)!=1||t>=16||(seen&(1u<<t))||f.read(&map,2)!=2||map>4095||f.read(&off,1)!=1||off>15)return false;seen|=1u<<t;mixerStageMap938[t]=map;for(uint8_t k=0;k<4;++k)if(off&(1u<<k))mixerStageOff938[k]|=1u<<t;}return true;}
FLASHMEM void mixerInstall938(){modOff28=0;memcpy(mixerMap938,mixerStageMap938,sizeof(mixerMap938));memcpy(mixerOff938,mixerStageOff938,sizeof(mixerOff938));}

FLASHMEM uint16_t modMixerKey28(uint8_t t,uint8_t row){
 if(t>=16||row>=4)return 65535;
 for(uint16_t key=0;key<64;++key){const auto&m=mod806Unit(key);if(!m.enabled)continue;bool visible=modAssociated12(key,t)||(m.inputs&(1u<<t));
 for(const auto&r:m.outputs)if(r.enabled&&!mod806MidiTarget(r)&&r.kind!=MOD_VIRTUAL&&!modIsTarget811(r)&&r.track==t)visible=true;
 if(visible){if(!row)return key;--row;}}
 return 65535;
}
FLASHMEM void modMixerToggle28(uint8_t t,uint8_t row){uint16_t key=modMixerKey28(t,row);if(key>=64)return;modOff28^=1ULL<<key;mod805Invalidate();modRecall937();neoDisplayDirty=displayValueDirty=true;char msg[50];snprintf(msg,sizeof(msg),"MSG|T%02u MOD%02u %s",t+1,key+1,(modOff28&(1ULL<<key))?"OFF":"ON");DISPLAY_SERIAL.println(msg);}

