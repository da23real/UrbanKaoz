#pragma once
struct RytParam818{uint8_t cc;char name[7],shortName[4];};
struct RytPatch818{char name[11];char title[49];uint8_t count;RytParam818 params[8];};
const RytPatch818 rytPatches818[] PROGMEM={
{"ARBDPLAS","Analog Rytm Bd Plastic",8,{{16,"Level","LEV"},{17,"Tune","TUN"},{18,"Decay","DEC"},{19,"SwDept","SWD"},{20,"SwTime","SWT"},{21,"Type","TYP"},{22,"Mod","MOD"},{23,"Tick","TIC"}}},
{"ARBDSHARP","Analog Rytm Bd Sharp",8,{{16,"Level","LEV"},{17,"Tune","TUN"},{18,"Decay","DEC"},{19,"SwDept","SWD"},{20,"SwTime","SWT"},{21,"Hold","HLD"},{22,"Tick","TIC"},{23,"Wave","WAV"}}},
{"ARBDHARD","Analog Rytm Bd Hard",8,{{16,"Level","LEV"},{17,"Tune","TUN"},{18,"Decay","DEC"},{19,"Hold","HLD"},{20,"SwTime","SWT"},{21,"SwDept","SWD"},{22,"Wave","WAV"},{23,"Tick","TIC"}}},
{"ARBDCL","Analog Rytm Bd Classic",8,{{16,"Level","LEV"},{17,"Tune","TUN"},{18,"Decay","DEC"},{19,"Hold","HLD"},{20,"SwTime","SWT"},{21,"SwDept","SWD"},{22,"Wave","WAV"},{23,"Tick","TIC"}}},
{"ARBDFM","Analog Rytm Bd Fm",8,{{16,"Level","LEV"},{17,"Tune","TUN"},{18,"Decay","DEC"},{19,"FMamt","FMA"},{20,"SwTime","SWT"},{21,"FMswep","FMS"},{22,"FMdec","FMD"},{23,"FMtune","FMT"}}},
{"ARBDSILKY","Analog Rytm Bd Silky",8,{{16,"Level","LEV"},{17,"Tune","TUN"},{18,"Decay","DEC"},{19,"SwDept","SWD"},{20,"SwTime","SWT"},{21,"Hold","HLD"},{22,"Click","CLK"},{23,"Dust","DST"}}},
{"ARBDACO","Analog Rytm Bd Acoustic",8,{{16,"Level","LEV"},{17,"Tune","TUN"},{18,"Decay","DEC"},{19,"SwDept","SWD"},{20,"SwTime","SWT"},{21,"Hold","HLD"},{22,"Impact","IMP"},{23,"Wave","WAV"}}},
{"ARSDNAT","Analog Rytm Sd Natural",8,{{16,"Level","LEV"},{17,"Tune","TUN"},{18,"BodyD","BDY"},{19,"NoiseD","NDC"},{20,"NoisLP","NLP"},{21,"NoisBl","NBL"},{22,"NoisRs","NRS"},{23,"NoisHP","NHP"}}},
{"ARSDHARD","Analog Rytm Sd Hard",8,{{16,"Level","LEV"},{17,"Tune","TUN"},{18,"Decay","DEC"},{19,"SwDept","SWD"},{20,"Tick","TIC"},{21,"NoiseD","NDC"},{22,"NoiseL","NLV"},{23,"SwTime","SWT"}}},
{"ARSDCL","Analog Rytm Sd Classic",8,{{16,"Level","LEV"},{17,"Tune","TUN"},{18,"Decay","DEC"},{19,"Detune","DET"},{20,"Snap","SNP"},{21,"NoiseD","NDC"},{22,"NoiseL","NLV"},{23,"OscBal","BAL"}}},
{"ARSDFM","Analog Rytm Sd Fm",8,{{16,"Level","LEV"},{17,"Tune","TUN"},{18,"Decay","DEC"},{19,"FMtune","FMT"},{20,"FMdec","FMD"},{21,"NoiseD","NDC"},{22,"NoiseL","NLV"},{23,"FMamt","FMA"}}},
{"ARSDACO","Analog Rytm Sd Acoustic",8,{{16,"Level","LEV"},{17,"Tune","TUN"},{18,"Decay","DEC"},{19,"NoiseD","NDC"},{20,"Hold","HLD"},{21,"NoiseL","NLV"},{22,"Impact","IMP"},{23,"SwDept","SWD"}}},
{"ARRSHARD","Analog Rytm Rs Hard",8,{{16,"Level","LEV"},{17,"Tune","TUN"},{18,"Decay","DEC"},{19,"SwDept","SWD"},{20,"Tick","TIC"},{21,"NoiseL","NLV"},{22,"Symm","SYM"},{23,"SwTime","SWT"}}},
{"ARRSCL","Analog Rytm Rs Classic",8,{{16,"Level","LEV"},{17,"Tune1","TN1"},{18,"Decay","DEC"},{19,"OscBal","BAL"},{20,"Tune2","TN2"},{21,"Symm","SYM"},{22,"NoiseL","NLV"},{23,"Tick","TIC"}}},
{"ARCPCL","Analog Rytm Cp Classic",8,{{16,"Level","LEV"},{17,"NoiseT","NTN"},{18,"NoiseD","NDC"},{19,"Claps","NUM"},{20,"ClapRt","RAT"},{21,"NoiseL","NLV"},{22,"Random","RND"},{23,"ClapDc","CDC"}}},
{"ARSYDUAL","Analog Rytm Sy Dual Vco",8,{{16,"Level","LEV"},{17,"Tune1","TN1"},{18,"Decay1","DC1"},{19,"Balanc","BAL"},{20,"Detun2","DT2"},{21,"OscCfg","CFG"},{22,"Decay2","DC2"},{23,"Bend","BND"}}},
{"ARSYCHIP","Analog Rytm Sy Chip",8,{{16,"Level","LEV"},{17,"Tune","TUN"},{18,"Decay","DEC"},{19,"Wave","WAV"},{20,"Speed","SPD"},{21,"Offst2","OF2"},{22,"Offst3","OF3"},{23,"Offst4","OF4"}}},
{"ARSYRAW","Analog Rytm Sy Raw",8,{{16,"Level","LEV"},{17,"Tune","TUN"},{18,"Decay","DEC"},{19,"NoiseL","NLV"},{20,"Detun2","DT2"},{21,"Wave1","WV1"},{22,"Wave2","WV2"},{23,"Balanc","BAL"}}},
{"ARBTCL","Analog Rytm Bt Classic",6,{{16,"Level","LEV"},{17,"Tune","TUN"},{18,"Decay","DEC"},{19,"SwDept","SWD"},{20,"NoiseL","NLV"},{21,"SnapTy","SNP"}}},
{"ARLTCL","Analog Rytm Lt Classic",8,{{16,"Level","LEV"},{17,"Tune","TUN"},{18,"Decay","DEC"},{19,"SwDept","SWD"},{20,"SwTime","SWT"},{21,"NoiseD","NDC"},{22,"NoiseL","NLV"},{23,"NoiseT","NTN"}}},
{"ARMTCL","Analog Rytm Mt Classic",8,{{16,"Level","LEV"},{17,"Tune","TUN"},{18,"Decay","DEC"},{19,"SwDept","SWD"},{20,"SwTime","SWT"},{21,"NoiseD","NDC"},{22,"NoiseL","NLV"},{23,"NoiseT","NTN"}}},
{"ARHTCL","Analog Rytm Ht Classic",8,{{16,"Level","LEV"},{17,"Tune","TUN"},{18,"Decay","DEC"},{19,"SwDept","SWD"},{20,"SwTime","SWT"},{21,"NoiseD","NDC"},{22,"NoiseL","NLV"},{23,"NoiseT","NTN"}}},
{"ARCHCL","Analog Rytm Ch Classic",4,{{16,"Level","LEV"},{17,"Tune","TUN"},{18,"Decay","DEC"},{19,"Color","COL"}}},
{"ARCHMET","Analog Rytm Ch Metallic",3,{{16,"Level","LEV"},{17,"Tune","TUN"},{18,"Decay","DEC"}}},
{"AROHCL","Analog Rytm Oh Classic",4,{{16,"Level","LEV"},{17,"Tune","TUN"},{18,"Decay","DEC"},{19,"Color","COL"}}},
{"AROHMET","Analog Rytm Oh Metallic",3,{{16,"Level","LEV"},{17,"Tune","TUN"},{18,"Decay","DEC"}}},
{"ARHHBASIC","Analog Rytm Hh Basic",6,{{16,"Level","LEV"},{17,"Tune","TUN"},{18,"Decay","DEC"},{19,"Tone","TON"},{20,"TranDc","TDC"},{21,"OscRst","RST"}}},
{"ARHHLAB","Analog Rytm Hh Lab",8,{{16,"Level","LEV"},{17,"Tune1","TN1"},{18,"Decay","DEC"},{19,"Tune2","TN2"},{20,"Tune3","TN3"},{21,"Tune4","TN4"},{22,"Tune5","TN5"},{23,"Tune6","TN6"}}},
{"ARCYMET","Analog Rytm Cy Metallic",5,{{16,"Level","LEV"},{17,"Tune","TUN"},{18,"Decay","DEC"},{19,"Tone","TON"},{20,"TranDc","TDC"}}},
{"ARCYCL","Analog Rytm Cy Classic",5,{{16,"Level","LEV"},{17,"Tune","TUN"},{18,"Decay","DEC"},{19,"Color","COL"},{20,"Tone","TON"}}},
{"ARCYRIDE","Analog Rytm Cy Ride",8,{{16,"Level","LEV"},{17,"Tune","TUN"},{18,"TailDc","TDC"},{19,"HitDec","HDC"},{20,"CymTyp","TYP"},{21,"Comp1","CP1"},{22,"Comp2","CP2"},{23,"Comp3","CP3"}}},
{"ARCBCL","Analog Rytm Cb Classic",4,{{16,"Level","LEV"},{17,"Tune","TUN"},{18,"Decay","DEC"},{19,"Detune","DET"}}},
{"ARCBMET","Analog Rytm Cb Metallic",4,{{16,"Level","LEV"},{17,"Tune","TUN"},{18,"Decay","DEC"},{19,"Detune","DET"}}},
{"ARUTNOISE","Analog Rytm Ut Noise",8,{{16,"Level","LEV"},{17,"LPfreq","LPF"},{18,"Decay","DEC"},{19,"SwDept","SWD"},{20,"SwTime","SWT"},{21,"LPreso","LPQ"},{22,"HPfreq","HPF"},{23,"Attack","ATK"}}},
{"ARUTIMPULS","Analog Rytm Ut Impulse",4,{{16,"Level","LEV"},{17,"Attack","ATK"},{18,"Decay","DEC"},{19,"Polar","POL"}}},
{"ARAMP","Analog Rytm Amp",8,{{78,"Attack","ATK"},{79,"Hold","HLD"},{80,"Decay","DEC"},{81,"Overdr","OVD"},{82,"Delay","DEL"},{83,"Reverb","REV"},{10,"Pan","PAN"},{7,"Volume","VOL"}}},
{"ARFILTER","Analog Rytm Filter",8,{{70,"Attack","ATK"},{71,"Decay","DEC"},{72,"Sustai","SUS"},{73,"Releas","REL"},{74,"Cutoff","CTF"},{75,"Reso","RES"},{76,"Mode","MOD"},{77,"EnvDpt","ENV"}}},
{"ARSAMPLE","Analog Rytm Sample",8,{{24,"Tune","TUN"},{25,"Fine","FIN"},{26,"BitRed","BIT"},{27,"Slot","SMP"},{28,"Start","STA"},{29,"End","END"},{30,"Loop","LOP"},{31,"Level","LEV"}}}
};
// Install/upgrade only factory definitions, once. A user's deletion survives reboot.
FLASHMEM void patchSeed818(){
 if(!storageWriteAvailable){Serial.println("PATCH819,SD_UNAVAILABLE");return;}
 const char*marker="/URBANKAOZ/PATCH/CATALOG819.OK";if(SD.exists(marker))return;
 SD.mkdir("/URBANKAOZ/PATCH");uint16_t added=0,upgraded=0,kept=0,failed=0;
 for(const auto&spec:rytPatches818){char path[64];snprintf(path,sizeof(path),"/URBANKAOZ/PATCH/%s.UKH",spec.name);
  PatchParam808 q[8]={};PatchDef808 def={};strcpy(def.name,spec.name);def.params=q;
  for(uint8_t i=0;i<spec.count;++i){q[i].id=i+1;q[i].next=i+1<spec.count?&q[i+1]:nullptr;q[i].number=spec.params[i].cc;q[i].bits=7;strcpy(q[i].name,spec.params[i].name);strcpy(q[i].shortName,spec.params[i].shortName);}
  char previous[80];snprintf(previous,sizeof(previous),"%s.OLD819",path);
  if(SD.exists(previous)&&!SD.exists(path))SD.sdfs.rename(previous,path);
  bool exists=SD.exists(path),replace=false;
  if(exists){File f=SD.open(path,FILE_READ);Item809 old;bool read=f&&itemRead809(f,old,ITEM_PATCH809);f.close();replace=read&&itemSame809(&def,old.def);itemFree809(old);if(!replace){++kept;continue;}}
  strcpy(def.name,spec.title);Item809 item;item.kind=ITEM_PATCH809;item.def=&def;
  if(replace){char temp[80];snprintf(temp,sizeof(temp),"%s.819",path);
   if(SD.exists(temp))SD.remove(temp);
   bool ok=!SD.exists(previous)&&itemAtomicWrite809(temp,item);
   if(ok)ok=SD.sdfs.rename(path,previous);
   if(ok){ok=SD.sdfs.rename(temp,path);if(!ok)SD.sdfs.rename(previous,path);else SD.remove(previous);}
   if(ok)++upgraded;else ++failed;
  }else if(itemAtomicWrite809(path,item))++added;else ++failed;
 }
 if(!failed){File f=SD.open(marker,FILE_WRITE_BEGIN);if(!f||f.println("819")!=5)++failed;f.close();}
 Serial.printf("PATCH819,ADDED=%u,UPGRADED=%u,KEPT=%u,FAILED=%u\n",added,upgraded,kept,failed);
}
FLASHMEM void patchCheck818(){
 uint16_t pass=0,fail=0;
 for(const auto&spec:rytPatches818){char path[64];snprintf(path,sizeof(path),"/URBANKAOZ/PATCH/%s.UKH",spec.name);File f=SD.open(path,FILE_READ);Item809 item;
  bool ok=f&&itemRead809(f,item,ITEM_PATCH809);f.close();if(ok){auto*q=item.def->params;ok=!strcmp(spec.title,item.def->name);
   for(uint8_t i=0;i<spec.count;++i){if(!q){ok=false;break;}ok=ok&&!q->kind&&q->bits==7&&q->number==spec.params[i].cc&&!strcmp(q->name,spec.params[i].name)&&!strcmp(q->shortName,spec.params[i].shortName);q=q->next;}if(q)ok=false;
  }itemFree809(item);if(ok)++pass;else{++fail;Serial.printf("PATCH819,MISMATCH,%s\n",spec.name);}
 }Serial.printf("PATCHCHECK819,%s,PASS=%u,FAIL=%u\n",fail?"FAIL":"PASS",pass,fail);
}
