#pragma once
#include <malloc.h>
FLASHMEM void memory906(){
 struct mallinfo m=mallinfo();
 Serial.printf("MEM906,PATTERNS=%lu,PACKED_BYTES=%lu,ROLL_BYTES=%lu,RAM2_HEAP_USED=%d,RAM2_HEAP_FREE=%d,NUMERIC=%u,FAULT=%u\n",(unsigned long)patternCount906,(unsigned long)patternBytes906,(unsigned long)rollBytes906,m.uordblks,m.fordblks,numericEnabled906,memoryFault906);
}
FLASHMEM void checks906(){
 if(playing||menuNavigationOnly()||sdBusy905()||songMode||patternMode){Serial.println("CHECK906,BUSY_HOME_PAUSED");return;}
 unsigned pass=0,fail=0;auto check=[&](bool ok,const char*name){++(ok?pass:fail);Serial.printf("CHECK906,%s,%s\n",ok?"PASS":"FAIL",name);};
 auto&a=checkFixture805;auto&b=checkCache805[0];memset(&a,0,sizeof(a));a.bars=64;
 auto*p=packPattern906(a);check(p&&p->length<600,"empty_pattern_compact");if(p){check(unpackPattern906(p,b)&&!memcmp(&a,&b,sizeof(a)),"empty_roundtrip");extmem_free(p);}
 a.active[1023]=true;a.note[1023]=127;a.lockPitch[1023]=true;a.lockVel[500]=true;a.vel[500]=0;
 a.rollMask[500]=63;a.roll[500]={1,2,-3,4,-5,0};a.fine[127].active=true;a.fine[127].step=1023;a.fine[127].pxMask=1023;
 p=packPattern906(a);check(p&&unpackPattern906(p,b)&&!memcmp(&a,&b,sizeof(a)),"notes_dormant_zero_locks_roundtrip");if(p)extmem_free(p);
 allocationLimit906=0;p=packPattern906(a);check(!p,"allocation_failure_explicit");allocationLimit906=-1;
 // Exercise sparse active rolls, restoring the exact original masks/values in RAM.
 const uint8_t t=15;rollExport906(t,a.roll);memcpy(a.rollMask,stepRollLockMask[t],1024);
 memcpy(checkSaved805[1].fine,fineNote[t],sizeof(fineNote[t]));memset(fineNote[t],0,sizeof(fineNote[t]));
 uint32_t before=rollBytes906;for(uint16_t s=0;s<1024;++s)rollSet906(t,s,emptyRoll906,0);
 uint32_t base=rollBytes906;TrackRollSettings r={1,2,-3,4,-5,0};
 check(rollSet906(t,1023,r,32)&&rollBytes906==base+sizeof(RollPage906)&&stepRollLockMask[t][1023]==32,"explicit_zero_allocates_one_page");
 check(rollRead906(t,1023).speed==0,"inherited_roll_fields_not_stored");
 check(rollSet906(t,1022,r,63)&&rollBytes906==base+sizeof(RollPage906),"same_page_reused");
 auto oldDefault=trackRoll[t];trackRoll[t].speed=47;stepRollLockMask[t][1023]=32;
 check(configuredRollSettings(t,1023).speed==47&&configuredRollSettings(t,1023).level==0,"live_default_and_explicit_zero");trackRoll[t]=oldDefault;
 rollSet906(t,1022,r,0);rollSet906(t,1023,r,0);check(rollBytes906==base,"last_override_releases_page");
 check(rollImport906(t,a.roll,a.rollMask)&&rollBytes906==before,"roll_state_restored");memcpy(fineNote[t],checkSaved805[1].fine,sizeof(fineNote[t]));
 PatchContext808 context={};uint16_t value=55;
 check(!patch808Get(context,1,value),"patch_absence_has_no_value");
 check(patch808Set(context,1,0)&&patch808Get(context,1,value)&&value==0,"patch_explicit_zero_preserved");patch808FreeValues(context.values);
 bool oldNumeric=numericEnabled906,oldEntry=numericEntryActive;uint8_t oldLen=numericEntryLen;char digits[6];memcpy(digits,numericEntryDigits,6);
 numericEnabled906=false;numericEntryActive=false;numericEntryLen=0;handleNumericKey('5');check(!numericEntryActive&&!numericEntryLen,"numeric_off_ignores_value_key");
 numericEnabled906=oldNumeric;numericEntryActive=oldEntry;numericEntryLen=oldLen;memcpy(numericEntryDigits,digits,6);
 check(SETTINGS_CATEGORY_COUNT==12&&SETTINGS_CATEGORY_NAMES[11]&&SETTINGS_CATEGORY_DESCRIPTIONS[11],"numeric_menu_complete");
 neoDisplayDirty=displayValueDirty=true;memory906();Serial.printf("CHECK906,SUMMARY,PASS=%u,FAIL=%u\n",pass,fail);
}
