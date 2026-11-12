#pragma once
struct CcEnvelope941 { uint8_t enabled,channel,number,invert; };
EXTMEM CcEnvelope941 ccEnv941[64],ccStage941[64];
uint8_t ccValue941[64]={};bool ccSeen941[64]={};
uint32_t ccBeginMs07[64]={},ccEndMs07[64]={};uint8_t ccTail07[64]={};
bool ccActive941(uint16_t key){return key<64&&!modKind931(key)&&ccEnv941[key].enabled&&ccSeen941[key];}
float ccLevel941(uint16_t key){return ccSeen941[key]?(ccEnv941[key].invert?127-ccValue941[key]:ccValue941[key]):0;}
void autoCcInput46(uint8_t,uint8_t,uint8_t);
void ccReceive941(uint8_t channel,uint8_t number,uint8_t value){autoCcInput46(channel,number,value);ioCc38(channel,number,value);bool changed=false;for(uint16_t key=0;key<64;++key){const auto&c=ccEnv941[key];if(modUnits805[key].enabled&&c.enabled&&c.channel==channel&&c.number==number){if(!ccSeen941[key]||ccValue941[key]!=value){uint8_t old=ccLevel941(key),next=c.invert?127-value:value;uint32_t now=millis();if(next&&!old)ccBeginMs07[key]=now;if(!next&&old){ccEndMs07[key]=now;ccTail07[key]=old;}ccValue941[key]=value;ccSeen941[key]=true;changed=true;}}}if(changed)++modEpoch805;}
bool ccWrite941(File&f){uint8_t count=0;for(uint16_t k=0;k<64;++k)if(modUnits805[k].enabled&&ccEnv941[k].enabled)++count;if(f.write(&count,1)!=1)return false;for(uint16_t k=0;k<64;++k)if(modUnits805[k].enabled&&ccEnv941[k].enabled){uint16_t disk=modDiskKey931(k);if(f.write((uint8_t*)&disk,2)!=2||f.write((uint8_t*)&ccEnv941[k],4)!=4)return false;}return true;}
bool ccRead941(File&f,bool present){memset(ccStage941,0,sizeof(ccStage941));if(!present)return true;uint8_t count=0;if(f.read(&count,1)!=1||count>64)return false;for(uint8_t i=0;i<count;++i){uint16_t disk;CcEnvelope941 c;if(f.read((uint8_t*)&disk,2)!=2||disk>=512||fullMap931[disk]<0||f.read((uint8_t*)&c,4)!=4||c.enabled!=1||c.channel>15||c.number>127||c.invert>1)return false;uint16_t key=fullMap931[disk];if(ccStage941[key].enabled)return false;ccStage941[key]=c;}return true;}
