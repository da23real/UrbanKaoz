#pragma once
FLASHMEM void linkReset12(){memset(linkMask12,0,sizeof(linkMask12));memset(linkSource12,0,sizeof(linkSource12));delayCut12=0;kbCenter12=36;projectName12[0]=0;}
FLASHMEM bool modAssociated12(uint16_t key,uint8_t t){if(t>=16||key>=64)return false;uint8_t owner=linkOwner12(t,modKind931(key)?LINK_LFO12:LINK_ENV12);return modMeta806[key].tracks&((1u<<t)|(1u<<owner));}
FLASHMEM bool modSourceAudible12(uint8_t t){if(!trackSilent818(t))return true;for(uint8_t c=0;c<16;++c)if(!trackSilent818(c)&&linkSource12[c]==t&&(linkMask12[c]&((1u<<LINK_ENV12)|(1u<<LINK_LFO12))))return true;return false;}
FLASHMEM void linkRender12(){char a[21],b[21],c[21],d[21],labels[8][6];const char*names[]={"Pch","ENV","LFO","DLY","View","Poly","BLK","KB"};const char*ptr[8];for(uint8_t i=0;i<8;++i){snprintf(labels[i],6,"%s%s",names[i],i<8?(linkMask12[selectedTrack]&(1u<<i)?"+":"-"):"");ptr[i]=labels[i];}snprintf(a,21,"%02u   Track %02u Link",linkDraft12+1,selectedTrack+1);b[0]=0;memset(c,' ',20);memset(d,' ',20);c[20]=d[20]=0;for(uint8_t i=0;i<8;++i)memcpy((i<4?c:d)+(i%4)*5,ptr[i],min((size_t)5,strlen(ptr[i])));sendMenuScreen(a,b,c,d);}
FLASHMEM bool linkToggle12(uint8_t g){if(g>=8||selectedTrack>=16||linkDraft12>=16)return false;uint8_t t=selectedTrack,from=linkSource12[t];bool off=linkMask12[t]&(1u<<g);
 if(!off){from=linkDraft12;if(from==t||linkMask12[from]){DISPLAY_SERIAL.println("MSG|LINK SOURCE REFUSED");return false;}for(uint8_t i=0;i<16;++i)if(linkMask12[i]&&linkSource12[i]==t){DISPLAY_SERIAL.println("MSG|TRACK IS A REFERENCE");return false;}linkSource12[t]=from;linkMask12[t]|=1u<<g;}
 else {
  if(g==LINK_ENV12||g==LINK_LFO12){uint8_t type=g==LINK_LFO12;uint8_t needed=0,free=0;for(uint16_t k=0;k<64;++k){free+=!modUnits805[k].enabled;if(modUnits805[k].enabled&&modKind931(k)==type&&(modMeta806[k].tracks&(1u<<from)))++needed;}if(needed>free){DISPLAY_SERIAL.println("MSG|64 MODS LIMIT");return false;}
   int16_t map[64];memset(map,255,sizeof(map));for(uint16_t k=0;k<64;++k)if(modUnits805[k].enabled&&modKind931(k)==type&&(modMeta806[k].tracks&(1u<<from))){uint16_t dst=modFreeSlot931();modDetach45(dst);mod807ClearHistory();map[k]=dst;modUnits805[dst]=modUnits805[k];modKinds931[dst]=type;modMeta806[dst]={uint16_t(1u<<t),modMeta806[k].midiInputs};modExtra810[dst]=modExtra810[k];modRat808[dst]=modRat808[k];ccEnv941[dst]=ccEnv941[k];ccSeen941[dst]=false;memcpy(modNames07[dst],modNames07[k],8);auto&m=modUnits805[dst];if(m.inputs&(1u<<from))m.inputs=(m.inputs&~(1u<<from))|(1u<<t);for(auto&r:m.outputs)if(r.kind==MOD_PATCH810&&r.track==from)r.track=t;}
   for(auto k:map)if(k>=0)for(auto&r:modUnits805[k].outputs)if(r.enabled&&modIsTarget811(r)&&map[modTarget810(r)]>=0)modRouteKey931(r,map[modTarget810(r)]);
   if(linkOwner12(t,LINK_KB13)==t)for(auto&z:kbTracksR10[t].zone)if(z.type==KB_MOD_R10&&z.target<64&&map[z.target]>=0)z.target=map[z.target];
  }
  if(g==LINK_PATCH12){PatchContext808 temp[51]={};bool ok=true;for(uint8_t p=0;p<51&&ok;++p)ok=patch808Clone(patchDb808.context[from][p],temp[p]);if(!ok){for(auto&c:temp)patch808FreeValues(c.values);DISPLAY_SERIAL.println("MSG|PATCH MEMORY FULL");return false;}for(uint8_t p=0;p<51;++p){patch808FreeValues(patchDb808.context[t][p].values);patchDb808.context[t][p]=temp[p];}}
  if(g==LINK_DLY12){DelayValues907 values[4];bool present[4];DelayNode907*prepared[4]={};bool ok=true;for(uint8_t k=0;k<4;++k){present[k]=delayPresent925(t,k);values[k]=delayGet907(t*16+k*256);if(present[k]){prepared[k]=(DelayNode907*)extmem_malloc(sizeof(DelayNode907));if(!prepared[k])ok=false;}}if(!ok){for(auto*n:prepared)extmem_free(n);DISPLAY_SERIAL.println("MSG|DELAY MEMORY FULL");return false;}linkMask12[t]&=~(1u<<g);for(uint8_t k=0;k<4;++k){delayRemove907(t*16+k*256);if(prepared[k]){auto*n=prepared[k];*n={delayNodes907,(uint16_t)(t*16+k*256),values[k]};delayNodes907=n;delayIndex925[t][k]=n;delaySet907(n->key,n->value);}}if(delayCut12&(1u<<from))delayCut12|=1u<<t;else delayCut12&=~(1u<<t);}

  if(g==LINK_VIEW12)trackNoteViewMode[t]=trackNoteViewMode[from];
  if(g==LINK_POLY12)trackPlayMode[t]=trackPlayMode[from];
  if(g==LINK_MOUT12){memcpy(blocks21[t],blocks21[from],sizeof(blocks21[t]));trackOut902[t]=trackOut902[from];trackMidiChannel[t]=trackMidiChannel[from];}
  if(g==LINK_KB13)kbTracksR10[t]=kbTracksR10[from];
  linkMask12[t]&=~(1u<<g);
 }
 pidiRoutesDirty931=0xffff;pidiProject925();mod805Invalidate();storageDirty931=displayValueDirty=neoDisplayDirty=true;linkRender12();return true;
}
