#pragma once
// Synthetic database only. Never serialize the user's current project to SD.
EXTMEM PatchDb808 testSaved819,testStageSaved819;
FLASHMEM bool testLegacyWrite819(File&f){
 bool ok=true;auto put=[&](const void*p,size_t n){if(ok)ok=f.write((const uint8_t*)p,n)==n;};
 uint32_t count=1,next=2,id=1,zero=0;uint16_t cc=74,value=42;uint8_t kind=0,bits=7;int8_t channel=-1;
 put(&count,4);put(&next,4);put(&next,4);put(&id,4);char name[10]="Legacy";put(name,10);put(&count,4);
 put(&id,4);put(&cc,2);put(&kind,1);put(&bits,1);char param[7]="Cutoff";put(param,6);put("CTF",3);put(&kind,1);
 for(uint16_t i=0;i<272;++i){put(i?&zero:&id,4);put(&channel,1);put(i?&zero:&count,4);if(!i){put(&id,4);put(&value,2);}}
 uint16_t rat=8;for(uint16_t i=0;i<512;++i)put(&rat,2);int16_t advance=0;put(&advance,2);return ok;
}
FLASHMEM void checks819(){
 if(playing||!trackContext815()||selectionHasActiveNotes()||selectedStep>=0||lockLeaseCount817||trackChord815){Serial.println("CHECK819,BUSY");return;}
 for(const auto&n:activeNotes)if(n.active){Serial.println("CHECK819,BUSY_NOTES");return;}
 uint16_t pass=0,fail=0;auto check=[&](bool ok,const char*label){Serial.printf("CHECK819,%s,%s\n",ok?"OK":"FAIL",label);if(ok)++pass;else ++fail;};
 check(normalKyDelta819(3)==3&&normalKyDelta819(-3)==-3&&normalKyDelta819(100)==16,PSTR("normal_KY_direction"));
 bool rollDirection=true;for(uint8_t i=0;i<6;++i)rollDirection&=rollDelta819(i,3)==(i==0||i==2?-3:3);check(rollDirection,PSTR("only_roll_start_accel_inverted"));
 testSaved819=patchDb808;testStageSaved819=patchStage808;memset(&patchDb808,0,sizeof(patchDb808));memset(&patchStage808,0,sizeof(patchStage808));patch808Free(patchDb808);
 auto*a=patch808New();auto*b=patch808New();
 if(!a||!b){check(false,PSTR("allocate"));goto finish819;}
 {
 strcpy(a->name,"Analog Rytm Sample Long Name Test");strcpy(b->name,"Analog Rytm Filter Test");
 bool allocated=true;for(uint8_t i=0;i<8;++i){auto*q=patch808Add(a);if(!q){allocated=false;break;}q->number=24+i;}
 for(uint8_t i=0;i<8;++i){auto*q=patch808Add(b);if(!q){allocated=false;break;}q->number=70+i;}
 check(allocated,PSTR("sixteen_parameters"));if(!allocated)goto finish819;
 auto&c=patchDb808.context[0][0];check(patchAttach819(c,a->id,true)&&patchAttach819(c,b->id,true)&&patchCount819(c)==16,PSTR("compose_sample_filter"));
 uint32_t aid=a->id,bid=b->id;uint16_t value=0;
 auto*level=patchParam819(c,7);auto*cutoff=patchParam819(c,12);
 check(level&&level->number==31&&cutoff&&cutoff->number==74,PSTR("both_banks_accessible"));
 patchSetValue819(c,*level,90);patchSetValue819(c,*cutoff,64);
 check(patchValue819(c,*level,value)&&value==90,PSTR("sample_baseline"));
 check(&patch808Context(0,0)==&c,PSTR("pattern_inherits_stack"));
 auto&own=patchDb808.context[0][1];check(patch808Clone(c,own)&&patchHas819(own,bid),PSTR("clone_complete_stack"));
 patchAttach819(own,0,false);check(&patch808Context(0,0)==&own&&!patchCount819(own),PSTR("NONE_overrides_inheritance"));
 patchAttach819(c,aid,false);check(c.patch==bid&&patchValue819(c,*cutoff,value)&&value==64,PSTR("remove_first_preserves_filter"));
 patchAttach819(c,aid,true);check(patchCount819(c)==16&&patchHas819(c,aid)&&patchHas819(c,bid),PSTR("reattach_keeps_both"));
 auto*alias=patch808Add(a);if(!alias){check(false,PSTR("allocate_alias"));goto finish819;}alias->number=74;
 check(patchSetValue819(c,*cutoff,72)&&patchValue819(c,*alias,value)&&value==72,PSTR("overlapping_CC_same_value"));
 uint8_t oldTrack=selectedTrack,oldPattern=activePattern[0];selectedTrack=0;activePattern[0]=0;
 int base=0;check(patch808Base(patch808Channel(0,c),74,base)==false,PSTR("NONE_has_no_playback_baseline"));
 patch808Clone(c,own);check(patch808Base(patch808Channel(0,c),74,base)&&base==72,PSTR("stack_CC_baseline_for_modulations"));
 check(modPatchNext810(0,77,1)==24&&!strcmp(modPatchName810(0,31,false),"Param"),PSTR("modulation_crosses_patch_boundary"));
 selectedTrack=oldTrack;activePattern[0]=oldPattern;
 // Exercise the actual editor at every physical index without sending the queue.
 PatchContext808 snapshot=patchDb808.context[oldTrack][0];PatchContext808 testContext={0,-1,nullptr};patch808Clone(c,testContext);patchDb808.context[oldTrack][0]=testContext;
 bool bank=patchBank808;patchBank808=true;uint32_t oldPage=patchPage808;patchPage808=0;
 bool positive=true,negative=true;
 for(uint8_t k=0;k<16;++k){auto*q=patchParam819(patchDb808.context[oldTrack][0],k);auto&ctx=patchDb808.context[oldTrack][0];patchSetValue819(ctx,*q,64);auto pending=midiPending810[patch808Channel(oldTrack,ctx)][q->number];
  applyParamDeltaFromKy(k,1);positive&=patchValue819(ctx,*q,value)&&value==65;
  applyParamDeltaFromKy(k,-1);negative&=patchValue819(ctx,*q,value)&&value==64;
  midiPending810[patch808Channel(oldTrack,ctx)][q->number]=pending;
 }
 check(positive,PSTR("all_16_patch_KY_positive_increase"));check(negative,PSTR("all_16_patch_KY_negative_decrease"));
 patchBank808=bank;patchPage808=oldPage;patch808FreeValues(patchDb808.context[oldTrack][0].values);patchDb808.context[oldTrack][0]=snapshot;
 if(storageWriteAvailable){
  const char*path="/URBANKAOZ/TEST819.BIN";
  if(SD.exists(path)){check(false,PSTR("test_path_already_exists"));}
  else {uint32_t expected=patch808DiskSize(),hash=patch808Hash();File f=SD.open(path,FILE_WRITE_BEGIN);bool ok=f&&patch808Write(f);f.flush();check(ok&&f.size()==expected,PSTR("binary_size_matches"));f.close();
   f=SD.open(path,FILE_READ);ok=f&&patch808Read(f,true)&&!f.available();f.close();check(ok&&patch808Hash()==hash,PSTR("multi_patch_long_name_binary_roundtrip"));SD.remove(path);
  }
  // Portable long-name definition roundtrip, independent of current state.
  path="/URBANKAOZ/TEST819.UKH";
  if(SD.exists(path)){check(false,PSTR("item_test_path_already_exists"));}
  else {Item809 item;item.kind=ITEM_PATCH809;item.def=patch808Find(patchDb808,aid);check(itemAtomicWrite809(path,item),PSTR("UKH4_long_name_roundtrip"));SD.remove(path);}
  path="/URBANKAOZ/LEGACY819.BIN";
  if(SD.exists(path))check(false,PSTR("legacy_test_path_already_exists"));
  else {File f=SD.open(path,FILE_WRITE_BEGIN);bool ok=f&&testLegacyWrite819(f);f.close();f=SD.open(path,FILE_READ);ok=ok&&f&&patch808Read(f,false)&&!f.available();f.close();uint16_t v=0;
   check(ok&&patchStage808.context[0][0].patch==1&&!patchStage808.context[0][1].explicit819&&patch808Get(patchStage808.context[0][0],1,v)&&v==42&&!strcmp(patchStage808.defs->name,"Legacy"),PSTR("legacy_single_patch_file"));SD.remove(path);
  }
  path="/URBANKAOZ/PATCH/DELETE819TEST.UKH";
  if(SD.exists(path))check(false,PSTR("delete_test_path_already_exists"));
  else {Item809 item;item.kind=ITEM_PATCH809;item.def=patch808Find(patchDb808,aid);bool ok=itemAtomicWrite809(path,item);auto*e=patch808Allocate<PatchEntry819>();
   if(ok&&e){auto*savedEntries=patchEntries819;uint32_t savedCursor=patchCursor808,savedWanted=patchWanted819;patchEntries819=e;strcpy(e->name,"Delete test fixture");strcpy(e->path,path);patchCursor808=1;patchUi808=1;
    check(patchDelete819()&&patchUi808==12&&SD.exists(path),PSTR("DELETE_requires_confirmation"));
    patchDeleteConfirm819(false);check(patchUi808==1&&SD.exists(path),PSTR("DELETE_cancel_keeps_file"));
    patchDelete819();patchDeleteConfirm819(true);check(patchUi808==1&&!SD.exists(path),PSTR("DELETE_confirm_removes_file"));
    while(patchEntries819){auto*n=patchEntries819;patchEntries819=n->next;extmem_free(n);}patchEntries819=savedEntries;patchCursor808=savedCursor;patchWanted819=savedWanted;patchUi808=0;
   }else {check(false,PSTR("delete_fixture_setup"));if(e)extmem_free(e);}SD.remove(path);
  }
 }else Serial.println("CHECK819,SKIP,SD_FIXTURES");
 }
finish819:
 patch808Free(patchDb808);patch808Free(patchStage808);patchDb808=testSaved819;patchStage808=testStageSaved819;memset(&testSaved819,0,sizeof(testSaved819));memset(&testStageSaved819,0,sizeof(testStageSaved819));
 displayValueDirty=true;Serial.printf("CHECK819,%s,PASS=%u,FAIL=%u\n",fail?"FAIL":"PASS",pass,fail);
}
