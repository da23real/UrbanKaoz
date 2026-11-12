#pragma once
struct ModPresetEntry07{ModPresetEntry07*next;char file[96];};
ModPresetEntry07*modPresets07=nullptr;uint16_t modPresetKey07=65535;int modPresetCursor07=-1;
FLASHMEM void modPresetOpen07(uint16_t key){
 while(modPresets07){auto*p=modPresets07;modPresets07=p->next;extmem_free(p);}modPresetKey07=key;modPresetCursor07=-1;
 if(!sdAvailable)return;const char*kind=modKind931(key)?"LFO":"ENV";char path[96];snprintf(path,sizeof(path),"/URBANKAOZ/%s",kind);File dir=SD.open(path);
 if(!dir||!dir.isDirectory()){dir.close();return;}
 while(true){File f=dir.openNextFile();if(!f)break;const char*ext=strrchr(f.name(),'.');
  if(!f.isDirectory()&&ext&&!strcasecmp(ext+1,kind)&&strlen(f.name())+strlen(path)+2<96){auto*p=patch808Allocate<ModPresetEntry07>();if(!p){f.close();break;}snprintf(p->file,96,"%s/%s",path,f.name());auto**link=&modPresets07;while(*link&&strcasecmp((*link)->file,p->file)<0)link=&(*link)->next;p->next=*link;*link=p;}
  f.close();serviceNoteOffs();
 }dir.close();
}
FLASHMEM void modPresetTurn07(uint16_t key,int delta){
 if(modPresetKey07!=key||!modPresets07)return;int count=0;for(auto*p=modPresets07;p;p=p->next)++count;
 int next=constrain(modPresetCursor07+(delta>0?1:-1),0,count-1);if(next==modPresetCursor07)return;auto*p=modPresets07;for(int i=0;i<next;++i)p=p->next;
 Item809 preset;File f=SD.open(p->file,FILE_READ);bool ok=f&&itemRead809(f,preset,modKind931(key)?ITEM_LFO809:ITEM_ENV809);f.close();
 if(ok){auto&m=mod806Unit(key);m.hold=preset.curve.hold;m.attack=preset.curve.attack;m.decay=preset.curve.decay;m.sustain=preset.curve.sustain;m.release=preset.curve.release;m.rate=preset.curve.rate;m.curve=preset.curve.curve;m.shape=preset.curve.shape;m.roll58=preset.curve.roll58;m.delay58=preset.curve.delay58;modExtra810[key]=preset.extra;modRat808[key]=preset.rat;ccEnv941[key]=preset.cc941;ccSeen941[key]=false;
  if(preset.name07[0])memcpy(modNames07[key],preset.name07,8);else {const char*base=strrchr(p->file,'/');snprintf(modNames07[key],8,"%.7s",base?base+1:p->file);char*dot=strchr(modNames07[key],'.');if(dot)*dot=0;}
  modPresetCursor07=next;modPresetDirty07[key]=false;storageDirty931=true;mod805Invalidate();displayValueDirty=true;
 }itemFree809(preset);
}
inline int modTimingField36(uint16_t key,uint8_t ky){if(modKind931(key))return ky>=12&&ky<=14?ky-12:-1;return ky==0?0:ky==2?1:ky==4?2:ky==6?3:ky==12?4:-1;}
inline bool modTimingKey36(uint16_t key,uint8_t ky){return !ioKey38(key)&&(modTimingField36(key,ky)>=0||(modKind931(key)&&ky==7));}
FLASHMEM void modTimingClick36(uint16_t key,uint8_t ky){int f=modTimingField36(key,ky);if(!ioKey38(key)&&f>=0&&!(modKind931(key)==0&&f==3))toggleTimeUnit38();else if(modKind931(key)&&ky==7){auto&m=mod806Unit(key);auto shown=m;shown.sustain=indDisplay50(LOCK_MOD817,key,6,m.sustain);uint8_t mode=lfoEnvMode36(shown);if(mode)shown.sustain=118+mode;else lfoEnvSet36(shown,shown.sustain>=119&&shown.sustain<=122?shown.sustain-118:1);if(indEdit50(LOCK_MOD817,key,6,m.sustain,0,shown.sustain,shown.sustain)){modDisplayDirty810=displayValueDirty=true;return;}if(mode){m.sustain=118+mode;}else lfoEnvSet36(m,m.sustain>=119&&m.sustain<=122?m.sustain-118:1);modPresetDirty07[key]=storageDirty931=true;mod805Invalidate();}modDisplayDirty810=displayValueDirty=true;}
FLASHMEM void quickModClick07(uint8_t ky){
 if((quickView903!=4&&quickView903!=5)||quickItem903>=64)return;uint16_t key=quickItem903;
 modTimingClick36(key,ky);
 if(ky==10){modPresetOpen07(key);modPresetTurn07(key,1);}
 if(ky==8&&ccEnv941[key].enabled)ccFine07[key]=!ccFine07[key];
 if(ky==8){for(uint8_t n=1;n<=8;++n){uint8_t i=(quickDepth903+n)%8;if(quickDepthVisible59(key,i)){quickDepth903=i;break;}}}
 displayValueDirty=true;
}
