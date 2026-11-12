#pragma once
#include "Item809Codec.h"
FLASHMEM const char* itemLabel809(){static const char* labels[]={"","PATCH","PRESET","ENV","LFO","DLY","TRACK","KB","ENVS","LFOS","DELAYS","GROOVE"};return labels[itemKind809];}
FLASHMEM const char* itemExtension809(){static const char* ext[]={"","UKH","UKV","ENV","LFO","UFX","UKT","UKB","ENS","LFS","DLS","UKG"};return ext[itemKind809];}
FLASHMEM bool itemFileMatches809(const char*name){if(remixBrowser911){const char*p=strrchr(name,'.');return p&&!strcasecmp(p+1,"PLK");}if(!itemKind809)return true;const char*p=strrchr(name,'.');return p&&!strcasecmp(p+1,itemExtension809());}
FLASHMEM PatchContext808& itemContext809(){return itemPatternMode809?patch808Context(itemTrack809,itemPattern809):patchDb808.context[linkOwner12(itemTrack809,LINK_PATCH12)][0];}
FLASHMEM void itemBrowser809(uint8_t kind){
  itemKind809=kind;itemChoosing809=false;
  if(itemSave809&&kind<=ITEM_PRESET809){auto d=patch808Find(patchDb808,itemPatch809);if(!d){DISPLAY_SERIAL.println("MSG|NO PATCH ASSIGNED");itemChoosing809=true;itemKind809=ITEM_PATCH809;return;}}
  fileBrowserMode=itemSave809?FILE_BROWSER_SAVE:FILE_BROWSER_LOAD;
  pendingSaveClipboard=false;patternLoadConfirmActive=false;saveMenuActive=loadTypeMenuActive=false;
  snprintf(fileBrowserPath,sizeof(fileBrowserPath),"/URBANKAOZ/%s",itemLabel809());
  copyField(fileBrowserRootPath,sizeof(fileBrowserRootPath),fileBrowserPath);
  if(itemSave809)SD.mkdir(fileBrowserPath);
  copyField(fileBrowserExtension,sizeof(fileBrowserExtension),itemExtension809());
  copyField(fileBrowserName,sizeof(fileBrowserName),itemLabel809());
  if(kind==ITEM_PATCH809||kind==ITEM_PRESET809){auto d=patch808Find(patchDb808,itemPatch809);if(d){copyField(fileBrowserName,sizeof(fileBrowserName),d->name);for(char*p=fileBrowserName;*p;++p)if(*p==' ')*p='_';}}
  if(itemSave809)namePropose12(lastFileName816[0]?lastFileName816:fileBrowserName);
  if(!itemSave809)storageNameCursor=0;fileBrowserEntryIndex=0;fileBrowserDirectoryPage=0;
  selectBrowserFile(0);renderFileBrowser();storageSync(true);
}
FLASHMEM void itemChoiceRender809(){mod806LiveFrame(itemSave809?"Save":"Load",itemChoice809?" Patch   >Preset":">Patch    Preset","","Patch     Preset");}
FLASHMEM bool itemOpen809(bool save){
  if(itemKind809||itemChoosing809)return true;
  if(grooveOpen32(save))return true;
  if(scopeOpen18(save))return true;
  if(!modUi805&&(!patchUi808||patchUi808>=5))return false;
  if(!sdAvailable||(save&&!storageWriteAvailable)){DISPLAY_SERIAL.println("MSG|SD UNAVAILABLE");return true;}
  if(patchUi808==4||patchDirty808||modDirty806){DISPLAY_SERIAL.println("MSG|FINISH EDIT FIRST");return true;}
  itemSave809=save;itemTrack809=modUi805?modOwnerTrack805:selectedTrack;itemPattern809=modUi805?modOwnerPattern805:activePattern[selectedTrack];itemPatternMode809=patternMode;
  itemReturnMod809=modUi805;itemReturnPatch809=patchUi808;itemMod809=modSelected806;
  if(modUi805==MOD_LIST806||modUi805==MOD_REMOVE806){mod806ListCount();itemMod809=modVisibleCount806?modVisible806[min(modCursor807,(uint16_t)(modVisibleCount806-1))]:UINT16_MAX;if(!save)itemMod809=UINT16_MAX;}
  uint8_t kind=modUi805?(modType805==2?ITEM_FX809:modType805?ITEM_LFO809:ITEM_ENV809):ITEM_PATCH809;
  if(save&&(kind==ITEM_ENV809||kind==ITEM_LFO809)&&itemMod809>=64){DISPLAY_SERIAL.println("MSG|NO MODULATION");return true;}
  itemPatch809=patchUi808==1?(patchSelected819(true)?patchSelected819(false)->id:0):patchUi808>=2&&patchEditing808?patchEditing808->id:itemContext809().patch;
  modUi805=patchUi808=0;deleteButtonActive=false;modRemove806=false;sendSelectionIndicatorsToPicran();
  itemKind809=kind;
  if(kind==ITEM_PATCH809){itemChoosing809=true;itemChoice809=0;itemChoiceRender809();}
  else itemBrowser809(kind);
  return true;
}
FLASHMEM bool itemRoy809(uint8_t roy,int32_t delta,bool click){
  if(!itemChoosing809)return false;if(roy==0){if(delta)itemChoice809=delta>0?1:0;if(click){itemBrowser809(itemChoice809?ITEM_PRESET809:ITEM_PATCH809);if(!itemChoosing809)return true;}itemChoiceRender809();}return true;
}
FLASHMEM bool itemScreen809(uint8_t index){if(!itemChoosing809)return false;if(index>=4){itemChoice809=index>=6;itemBrowser809(itemChoice809?ITEM_PRESET809:ITEM_PATCH809);if(itemChoosing809)itemChoiceRender809();}return true;}
FLASHMEM void itemClose809(){
  itemKind809=0;itemChoosing809=false;fileBrowserMode=FILE_BROWSER_NONE;patternLoadConfirmActive=false;saveMenuActive=loadTypeMenuActive=false;
  modUi805=itemReturnMod809;patchUi808=itemReturnPatch809;itemReturnMod809=itemReturnPatch809=0;
  if(itemReturnKb18){kbMenuR10=itemReturnKb18;itemReturnKb18=0;kbRenderMenuR10();}else if(modUi805){mod805Menu();mod805Display();}else if(patchUi808){if(patchUi808==1)patchList819();patch808Render();}else {showGameScreen();if(grooveReturn32){grooveReturn32=false;grooveEditMode=true;sendGroovePacket();}}storageSync(true);
}
FLASHMEM bool itemBack809(){
  if(saveCollision905){saveCollision905=false;renderFileBrowser();return true;}
  if(!itemKind809&&!itemChoosing809)return false;
  if(fileBrowserMode&&!browserAtTypeRoot()){browserParentDirectory();return true;}itemClose809();return true;
}
FLASHMEM bool itemCapture809(Item809&s){
  s.kind=itemKind809;
  if(s.kind<=ITEM_PRESET809){s.def=patch808Find(patchDb808,itemPatch809);if(!s.def)return false;if(s.kind==ITEM_PRESET809)s.values=itemContext809();}
  else if(s.kind==ITEM_FX809){s.delay907=delayGet907(delayKey924(itemTrack809,itemPattern809,delaySlot924));s.delay=delays805[itemTrack809][itemPattern809];s.delayExtra=delayExtra817[itemTrack809][itemPattern809];{auto&v=s.delay907;s.delay={(uint16_t)v.length,v.repeats,127,v.enabled};s.delayExtra={v.wait,v.output,v.rolls};}}
  else {if(itemMod809>=64)return false;s.curve=mod808EffectiveUnit(itemMod809);s.early50=min(0,holdValue50(itemMod809,s.curve));s.rat=modRat808[itemMod809];s.extra=modExtra810[itemMod809];s.cc941=ccEnv941[itemMod809];memcpy(s.name07,modNames07[itemMod809],8);}
  return true;
}
FLASHMEM bool itemAtomicWrite809(const char*path,const Item809&s){
  char temp[80];snprintf(temp,sizeof(temp),"%s.TMP",path);SD.remove(temp);
  File f=SD.open(temp,FILE_WRITE_BEGIN);if(!f)return false;bool ok=f.truncate(0)&&itemWrite809(f,s);f.flush();f.close();
  // Re-read using the production parser before exposing the file to Load.
  Item809 check;f=SD.open(temp,FILE_READ);ok=ok&&f&&itemRead809(f,check,s.kind);f.close();itemFree809(check);
  if(ok&&!SD.exists(path))ok=SD.sdfs.rename(temp,path);else ok=false;
  if(!ok)SD.remove(temp);return ok;
}
FLASHMEM void itemSaveFile809(){if(itemKind809==ITEM_GROOVE32){grooveSave32();return;}if(itemKind809>=ITEM_KB18){scopeSave18();return;}if(itemKind809==ITEM_TRACK12){trackSave12();return;}
  if(!fileBrowserName[0]||!storageNameValid(fileBrowserName)){DISPLAY_SERIAL.println("MSG|INVALID NAME");return;}
  Item809 s;if(!itemCapture809(s)){DISPLAY_SERIAL.println("MSG|NOTHING TO SAVE");return;}
  if(strlen(fileBrowserPath)+strlen(fileBrowserName)+7>=64){DISPLAY_SERIAL.println("MSG|PATH TOO LONG");return;}
  char path[64];bool ok=makeUniqueSavePath(itemLabel809(),itemLabel809(),itemExtension809(),path,sizeof(path))&&itemAtomicWrite809(path,s);
  ok=finishSave905(ok,path,sizeof(path));
  if(!ok){DISPLAY_SERIAL.println("MSG|SAVE ERROR");return;}
  if(itemKind809==ITEM_PATCH809&&s.def){strcpy(s.def->source819,path);patchCatalogScan10();}
  rememberFileName816(path);songSend("SAVED,%s",path);itemClose809();DISPLAY_SERIAL.println("MSG|SAVED");
}
struct ItemTarget809{ItemTarget809*next;PatchContext808*dst;PatchContext808 values;};
FLASHMEM bool itemInstallPreset809(const Item809&s){
  ItemTarget809*list=nullptr;bool ok=true,selected=false;
  auto prepare=[&](uint8_t t,int p){
    if(!ok)return;auto &ctx=p<0?patchDb808.context[linkOwner12(t,LINK_PATCH12)][0]:patch808Context(t,p);PatchDef808*d=nullptr;for(uint8_t slot=0;slot<16;++slot){auto*candidate=patch808Find(patchDb808,patchId819(ctx,slot));if(itemSame809(s.def,candidate)){d=candidate;break;}}
    if(!d){ok=false;return;}auto n=patch808Allocate<ItemTarget809>();if(!n){ok=false;return;}
    n->next=list;list=n;n->dst=&patchDb808.context[linkOwner12(t,LINK_PATCH12)][p+1];ok=patch808Clone(ctx,n->values);if(!ok)return;patchRemoveValues819(n->values,d);for(auto*v=s.values.values;v;v=v->next){auto*q=patch808ParamAt(d,v->parameter-1);if(!q||!patchSetValue819(n->values,*q,v->value)){ok=false;return;}}n->values.explicit819=true;
  };
  if(itemPatternMode809)for(uint8_t t=0;t<16;++t)for(uint8_t p=0;p<16;++p)if(patternSelected[t][p]){selected=true;prepare(t,p);}
  if(!selected)prepare(itemTrack809,itemPatternMode809?itemPattern809:-1);
  while(list){auto n=list;list=n->next;if(ok){patch808FreeValues(n->dst->values);*n->dst=n->values;}else patch808FreeValues(n->values.values);extmem_free(n);}
  return ok;
}
FLASHMEM bool itemInstallPatch809(Item809&s){
  auto d=patchDb808.defs;while(d&&!(s.def->source819[0]&&!strcasecmp(d->source819,s.def->source819))&&!itemSame809(d,s.def))d=d->next;
  if(!d){uint32_t count=patch808ParamCount(s.def);if(patchDb808.nextPatch==UINT32_MAX||count>=UINT32_MAX-patchDb808.nextParam)return false;
    d=s.def;s.def=nullptr;d->id=patchDb808.nextPatch++;for(auto q=d->params;q;q=q->next)q->id=patchDb808.nextParam++;
    auto tail=&patchDb808.defs;while(*tail)tail=&(*tail)->next;*tail=d;
  }
  if(s.def&&s.def->source819[0])strcpy(d->source819,s.def->source819);
  patchResolve10(patchDb808);patchSelectId819(d->id);
  itemReturnPatch809=1;itemReturnMod809=0;patchEditing808=nullptr;patchParam808=nullptr;return true;
}
FLASHMEM bool itemInstallMod809(const Item809&s){
  uint16_t key=itemMod809;bool created=key>=64;
  if(created){key=modFreeSlot931();if(key>=64)return false;modDetach45(key);mod807ClearHistory();}
  auto&m=mod806Unit(key);
  if(created){modKinds931[key]=s.kind==ITEM_LFO809;memset(&m,0,sizeof(m));memset(&modMeta806[key],0,sizeof(ModMeta806));m.inputs=1u<<itemTrack809;m.enabled=1;mod806Associate(modMeta806[key],itemTrack809,itemPattern809,true);mod807ClearHistory();}
  else {modSelected806=key;mod807Remember();}
  envEarly50[key]=s.early50;m.hold=s.curve.hold;m.attack=s.curve.attack;m.decay=s.curve.decay;m.release=s.curve.release;m.rate=s.curve.rate;m.sustain=s.curve.sustain;m.curve=s.curve.curve;m.shape=s.curve.shape;m.roll58=s.curve.roll58;m.delay58=s.curve.delay58;modRat808[key]=s.rat;modExtra810[key]=s.extra;ccEnv941[key]=s.cc941;ccSeen941[key]=false;memcpy(modNames07[key],s.name07,8);modPresetDirty07[key]=false;
  modSelected806=key;modDepthSlot806=-1;modSourceCursor807=modDestCursor807=0;modDirty806=0;itemReturnMod809=MOD_OBJECT806;mod805Invalidate();return true;
}
FLASHMEM void itemLoadFile809(){if(itemKind809==ITEM_GROOVE32){grooveLoad32();return;}if(itemKind809>=ITEM_KB18){scopeLoad18();return;}if(itemKind809==ITEM_TRACK12){trackLoad12();return;}
  if(!strcmp(fileBrowserEntry,"--")||!itemFileMatches809(fileBrowserEntry)){DISPLAY_SERIAL.println("MSG|NO MATCHING FILE");return;}
  char path[96];snprintf(path,sizeof(path),"%s/%s",fileBrowserPath,fileBrowserEntry);
  File f=SD.open(path,FILE_READ);Item809 s;bool ok=f&&itemRead809(f,s,itemKind809);f.close();
  const char* error="INVALID ITEM FILE";
  if(ok){
    if(itemKind809==ITEM_PATCH809){strcpy(s.def->source819,path);patchCatalogScan10();ok=itemInstallPatch809(s);}
    else if(itemKind809==ITEM_PRESET809){ok=itemInstallPreset809(s);error="PATCH MISMATCH / RAM";}
    else if(itemKind809==ITEM_FX809){auto v=s.delay907;v.length=s.delay.ticks;v.repeats=s.delay.repeats;v.enabled=s.delay.enabled;v.wait=s.delayExtra.wait;v.output=s.delayExtra.output;v.rolls=s.delayExtra.rolls;v.chain&=(1u<<delaySlot924)-1;ok=delaySet907(delayKey924(itemTrack809,0,delaySlot924),v);modSavedDelay806=s.delay;modDirty806=0;mod805Invalidate();}
    else ok=itemInstallMod809(s);
  }
  itemFree809(s);if(!ok){DISPLAY_SERIAL.print("MSG|");DISPLAY_SERIAL.println(error);songError(error);return;}
  rememberFileName816(path);itemClose809();displayValueDirty=true;DISPLAY_SERIAL.println("MSG|LOADED");
}
FLASHMEM void itemSeed809(){
  if(!storageWriteAvailable)return;
  const char*path="/URBANKAOZ/PATCH/ARminimal.UKH";if(SD.exists(path))return;
  SD.mkdir("/URBANKAOZ/PATCH");
  PatchParam808 q[3]={};const char*names[]={"Volume","Cutoff","Reso"};const char*shorts[]={"VOL","CTF","RES"};const uint8_t cc[]={7,74,75};
  for(uint8_t i=0;i<3;++i){q[i].id=i+1;q[i].next=i<2?&q[i+1]:nullptr;q[i].number=cc[i];q[i].bits=7;strcpy(q[i].name,names[i]);strcpy(q[i].shortName,shorts[i]);}
  PatchDef808 d={};strcpy(d.name,"ARminimal");d.params=q;Item809 s;s.kind=ITEM_PATCH809;s.def=&d;
  Serial.println(itemAtomicWrite809(path,s)?"PATCH809,AR_SD_READY":"PATCH809,AR_SD_ERROR");
}
