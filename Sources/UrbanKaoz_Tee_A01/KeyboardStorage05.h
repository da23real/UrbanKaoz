#pragma once
struct Project12 {int32_t center;uint16_t cut;uint8_t source[16],mask[16],outputs[16];};
static Project12 projectStage12;
FLASHMEM void kbPrefsSaveR10(){uint8_t v[7]={0xA1,0x10,(uint8_t)kbSelectHearR10,kbColorsR10[0],kbColorsR10[1],kbColorsR10[2],kbColorsR10[3]};EEPROM.put(3720,v);}
FLASHMEM void kbPrefsLoadR10(){uint8_t v[7];EEPROM.get(3720,v);if(v[0]!=0xA1||v[1]!=0x10||v[2]>1)return;for(uint8_t i=3;i<7;++i)if(v[i]>=NEO_PALETTE_COUNT)return;kbSelectHearR10=v[2];memcpy(kbColorsR10,v+3,4);}
FLASHMEM bool kbWriteR10(File&f){
 uint32_t magic=0xA0100018;if(f.write((uint8_t*)&magic,4)!=4)return false;
 for(auto&t:kbTracksR10){auto copy=t;for(auto&z:copy.zone)if(z.type==KB_MOD_R10)z.target=z.target<64&&mod806Unit(z.target).enabled?modDiskKey931(z.target):65535;
 if(f.write((uint8_t*)&copy,sizeof(copy))!=sizeof(copy))return false;}
 if(f.write((uint8_t*)trackNames07,sizeof(trackNames07))!=sizeof(trackNames07))return false;
 uint8_t count=0;for(uint16_t k=0;k<64;++k)if(mod806Unit(k).enabled)++count;if(f.write(&count,1)!=1)return false;
 for(uint16_t k=0;k<64;++k)if(mod806Unit(k).enabled){uint16_t disk=modDiskKey931(k);if(f.write((uint8_t*)&disk,2)!=2||f.write((uint8_t*)modNames07[k],8)!=8)return false;}
 Project12 p{};p.center=kbCenter12;p.cut=delayCut12;memcpy(p.source,linkSource12,16);memcpy(p.mask,linkMask12,16);memcpy(p.outputs,trackOut902,16);return f.write((uint8_t*)&p,sizeof(p))==sizeof(p);
}
FLASHMEM bool kbReadR10(File&f,bool present){
 for(auto&t:kbStageR10){t=KbTrackR10{};for(uint8_t z=1;z<4;++z){t.zone[z].type=KB_CC_R10;t.zone[z].center=t.zone[z].value=64;t.zone[z].target=z;}}
 memset(trackNamesStage07,0,sizeof(trackNamesStage07));memset(modNamesStage07,0,sizeof(modNamesStage07));
 projectStage12={};projectStage12.center=36;memcpy(projectStage12.outputs,trackOut902,16);
 if(!present)return true;uint32_t magic=0;if(f.read((uint8_t*)&magic,4)!=4||(magic!=0xA0100010&&magic!=0xA0100013&&magic!=0xA0100016&&magic!=0xA0100017&&magic!=0xA0100012&&magic!=0xA0100018)||f.read((uint8_t*)kbStageR10,sizeof(kbStageR10))!=sizeof(kbStageR10))return false;
 for(auto&t:kbStageR10){if(t.split>2||t.panels>1)return false;for(auto&z:t.zone){
 if(z.type>4||z.mute>1||z.valid>1||z.range< -65535||z.range>65535||z.center< -32767||z.center>65535||z.value< -32767||z.value>65535||(z.type==KB_MOD_R10&&z.parameter>=16)||(z.type==KB_DLY_R10&&(z.target>=4||z.parameter>=12))||(z.type==KB_CC_R10&&z.target>127))return false;
 if(magic==0xA0100010&&z.range){int pads=t.split==0?64:t.split==1?32:16;int divisor=z.type==KB_NOT_R10?pads/2:pads-1;int scale=z.type==KB_NOT_R10?1:2;z.range=max(1,(z.range*scale+divisor/2)/divisor);}
 if(z.type==KB_MOD_R10){if(z.target==65535){z.target=0;z.valid=0;}else {if(z.target>=512||fullMap931[z.target]<0)return false;z.target=fullMap931[z.target];if(z.parameter<8&&z.parameter>=(fullKinds931[z.target]?4:5))return false;}}
 }}
 if(magic==0xA0100017||magic==0xA0100012||magic==0xA0100018){
  if(f.read((uint8_t*)trackNamesStage07,sizeof(trackNamesStage07))!=sizeof(trackNamesStage07))return false;
  for(auto&name:trackNamesStage07)if(name[7])return false;
  uint8_t count;bool seen[64]={};if(f.read(&count,1)!=1||count>64)return false;
  for(uint8_t i=0;i<count;++i){uint16_t disk;if(f.read((uint8_t*)&disk,2)!=2||disk>=512||fullMap931[disk]<0)return false;uint16_t k=fullMap931[disk];if(seen[k]||f.read((uint8_t*)modNamesStage07[k],8)!=8||modNamesStage07[k][7])return false;seen[k]=true;}
 }
 projectStage12.center=kbStageR10[selectedTrack<16?selectedTrack:0].zone[0].center;
 if(magic==0xA0100012||magic==0xA0100018){auto&p=projectStage12;if(f.read((uint8_t*)&p,sizeof(p))!=sizeof(p)||p.center< -32767||p.center>65535)return false;for(uint8_t t=0;t<16;++t)if(p.source[t]>=16||(magic==0xA0100012&&p.mask[t]>127)||(p.mask[t]&&(p.source[t]==t||p.mask[p.source[t]])))return false;}
 return true;
}
FLASHMEM void kbInstallR10(){kbCenter12=projectStage12.center;delayCut12=projectStage12.cut;memcpy(linkSource12,projectStage12.source,16);memcpy(linkMask12,projectStage12.mask,16);memcpy(trackOut902,projectStage12.outputs,16);pidiRoutesDirty931=0xffff;memcpy(trackNames07,trackNamesStage07,sizeof(trackNames07));memcpy(modNames07,modNamesStage07,sizeof(modNames07));memset(modPresetDirty07,0,sizeof(modPresetDirty07));memcpy(kbTracksR10,kbStageR10,sizeof(kbTracksR10));for(auto&t:kbTracksR10)for(auto&z:t.zone)z.valid=0;}

