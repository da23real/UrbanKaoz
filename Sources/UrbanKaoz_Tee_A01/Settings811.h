#pragma once
// Independent of Full project files; loading a project cannot change boot policy.
struct Settings811 {uint32_t magic;uint16_t minutes;uint8_t startup,autosave;uint16_t checksum;};
static const int settingsAddress811=sizeof(PersistedSettings)+sizeof(PersistedPicoSettings)+sizeof(PersistedSeqSettings)+sizeof(PersistedUi2Settings)+sizeof(PersistedTrackOverrides);
FLASHMEM void expLoad15(){uint8_t v[10];EEPROM.get(3744,v);if(v[0]!=0xE1||v[1]!=0x15)return;uint8_t sum=0;for(uint8_t i=0;i<9;i++)sum^=v[i];if(sum!=v[9])return;for(uint8_t i=2;i<5;i++)if(v[i]<1||v[i]>100)return;for(uint8_t i=5;i<8;i++)if(v[i]>100)return;expMaximum14=v[2];expDelayMaximum15=v[3];expEnvMaximum15=v[4];memcpy(expDefault15,v+5,3);}
FLASHMEM void expSave15(){uint8_t v[10]={0xE1,0x15,(uint8_t)expMaximum14,(uint8_t)expDelayMaximum15,(uint8_t)expEnvMaximum15,expDefault15[0],expDefault15[1],expDefault15[2],0,0};for(uint8_t i=0;i<9;i++)v[9]^=v[i];EEPROM.put(3744,v);pidiPacket827(pidi827::EXP14,&expDelayMaximum15,2);}
FLASHMEM void settingsLoad811(){uint32_t scale37;EEPROM.get(3912,scale37);if((scale37&0xffffff00)==0x37010000&&(scale37&255)<=2)noteScale37=scale37&255;uint32_t e14=0;EEPROM.get(3740,e14);if((e14&0xffff0000u)==0xE0140000u&&(e14&65535)>=1&&(e14&65535)<=100)expMaximum14=e14&65535;expDelayMaximum15=expEnvMaximum15=expMaximum14;expLoad15();Settings811 s={};EEPROM.get(settingsAddress811,s);if(s.magic==0x8115345u&&s.minutes>=1&&s.minutes<=120&&s.startup<=2&&s.autosave<=1&&s.checksum==(uint16_t)(s.minutes^(s.startup<<8)^(s.autosave<<12)^0x811)){startupMode811=s.startup;autosaveEnabled811=false;autosaveMinutes811=s.minutes;}}
FLASHMEM void settingsSave811(){Settings811 s={};s.magic=0x8115345u;s.minutes=autosaveMinutes811;s.startup=startupMode811;s.autosave=autosaveEnabled811;s.checksum=s.minutes^(s.startup<<8)^(s.autosave<<12)^0x811;EEPROM.put(settingsAddress811,s);}
uint16_t configDraft811[2]={};
FLASHMEM void settingsRender811(){if(patchUi808==16){cvTimingRender50();return;}if(patchUi808==15){const char*names[]={"MIDI","English (C4)","Classical (do4)"};patch808Frame("SEQ > SCALE",names[noteScale37],"","");return;}
  if(patchUi808==14){const char*names[]={"Roll","DLY","ENV"};char b[21],c[21];uint16_t m=expFamily15==0?expMaximum14:expFamily15==1?expDelayMaximum15:expEnvMaximum15;snprintf(b,21,"%s   x%u   %u%%",names[expFamily15],m,expDefault15[expFamily15]);snprintf(c,21,"Type  Max  Default");patch808Frame("SEQ > EXP",b,c,"");return;}
  if(patchUi808==13){playbackRender828();return;}
  if(patchUi808==11){patch808Frame("SEQ > CALC","","CALCULS ON/OFF","RDEL      P.locks");return;}
  if(patchUi808==10){patch808Frame("SEQ > P.locks","End of note",configDraft811[0]?"Release end":"Gate end","OA change / click OK");return;}
  if(patchUi808==9){char b[21];snprintf(b,21,"Click: %u ms",configDraft811[0]);patch808Frame("KY23 > CLICK",b,"Press and release","");return;}
  char b[21];if(patchUi808==8){const char*names[]={"Empty","default","Last autosave"};snprintf(b,21,"%s",names[min(configDraft811[0],(uint16_t)2)]);patch808Frame("SEQ > STARTUP",b,"","OA select / click OK");}
  else {patch808Frame("MAIN > SAVE","Manual save only","","");}
}
FLASHMEM void settingsOpen811(bool startup){patchUi808=startup?8:7;settingsPageRetryPending=false;configDraft811[0]=startup?startupMode811:autosaveEnabled811;configDraft811[1]=autosaveMinutes811;settingsRender811();}
FLASHMEM bool settingsRoy811(uint8_t roy,int32_t delta,bool click){
  if(cvTimingRoy50(roy,delta,click))return true;
  if(patchUi808<7)return false;if(patchUi808==15){if(!roy&&delta){noteScale37=WRAP23(noteScale37,delta,0,2);uint32_t setting=0x37010000|noteScale37;EEPROM.put(3912,setting);}if(!roy&&click){patch808Back();return true;}settingsRender811();displayValueDirty=true;return true;}
  if(patchUi808==7)return true;
  if(patchUi808==14){if(delta){if(roy==0)expFamily15=WRAP23(expFamily15,delta,0,2);else if(roy==1){uint16_t&v=expFamily15==0?expMaximum14:expFamily15==1?expDelayMaximum15:expEnvMaximum15;v=WRAP23(v,delta,1,100);expSave15();}else if(roy==2){expDefault15[expFamily15]=WRAP23(expDefault15[expFamily15],delta,0,100);expSave15();}mod805Invalidate();remixScheduleDirty911=true;}settingsRender811();return true;}
  if(patchUi808==13){if(roy<3&&(delta||click)){bool old=roy==0?playbackLocks828:roy==1?playbackEnv828:playbackLfo828;playbackSet828(roy,click?!old:delta>0);}playbackRender828();return true;}
  if(patchUi808==11){if(click&&roy==0){patchUi808=10;configDraft811[0]=lockRelease817;settingsRender811();}return true;}
  if(patchUi808==10){if(roy==0&&delta)configDraft811[0]=delta>0;if(roy==0&&click){lockRelease817=configDraft811[0];lockSettingsSave817();patch808Back();return true;}settingsRender811();return true;}
  if(patchUi808==9){if(roy==0&&delta)configDraft811[0]=WRAP23(configDraft811[0],delta,0,120);
    if(roy==0&&click){ky23Debounce816=configDraft811[0];settingsSave816();patch808Back();return true;}
    settingsRender811();return true;}
  if(delta&&roy==0)configDraft811[0]=WRAP23(configDraft811[0],delta,0,patchUi808==8?2:1);
  if(delta&&roy==1&&patchUi808==7)configDraft811[1]=WRAP23(configDraft811[1],delta,1,120);
  if(click&&roy<2){if(patchUi808==8)startupMode811=configDraft811[0];else {autosaveEnabled811=configDraft811[0];autosaveMinutes811=configDraft811[1];autosaveLast807=millis();}settingsSave811();patch808Back();return true;}
  settingsRender811();return true;
}
FLASHMEM bool startupLatest811(char*path,size_t size){
  if(!sdAvailable)return false;uint32_t generations[10]={};
  for(uint8_t slot=0;slot<10;++slot){snprintf(path,size,"/URBANKAOZ/PRJ/autosave/auto%02u.UKP",slot+1);File f=SD.open(path,FILE_READ);if(!f)continue;FullFooter805 footer={};if(f.size()>=sizeof(footer)&&f.seek(f.size()-sizeof(footer))&&f.read((uint8_t*)&footer,sizeof(footer))==sizeof(footer)&&footer.magic==0x8054546u)generations[slot]=footer.generation;f.close();}
  for(uint8_t attempt=0;attempt<10;++attempt){int best=-1;for(uint8_t i=0;i<10;++i)if(generations[i]&&(best<0||generations[i]>generations[best]))best=i;if(best<0)break;snprintf(path,size,"/URBANKAOZ/PRJ/autosave/auto%02u.UKP",best+1);if(full805Validate(path))return true;generations[best]=0;}
  path[0]=0;return false;
}
FLASHMEM bool patchAssigned811(uint32_t id){
  bool any=false;if(patternMode)for(uint8_t t=0;t<16;++t)for(uint8_t p=0;p<16;++p)if(patternSelected[t][p]){any=true;if(!patchHas819(patch808Context(t,p),id))return false;}
  if(any)return true;return patchHas819(patternMode?patch808Context(selectedTrack,activePattern[selectedTrack]):patchDb808.context[linkOwner12(selectedTrack,LINK_PATCH12)][patchContextIndex21(0)],id);
}

FLASHMEM void lockSettingsOpen817(){patchUi808=11;settingsPageRetryPending=false;settingsRender811();}
FLASHMEM void lockSettingsSave817(){uint32_t setting=0x817A0000u|(lockRelease817?1:0);EEPROM.put(settingsAddress811+sizeof(Settings811)+8,setting);}
FLASHMEM void lockSettingsLoad817(){uint32_t setting=0;EEPROM.get(settingsAddress811+sizeof(Settings811)+8,setting);lockRelease817=(setting&0xfffffffeu)==0x817A0000u&&(setting&1);}
