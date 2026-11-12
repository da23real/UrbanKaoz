#pragma once
FLASHMEM void checks903(){
 if(playing||trackMenuActive||modUi805||patchUi808||menuStopLatched){Serial.println("CHECK903,BUSY_RETURN_HOME_STOPPED");return;}
 for(auto&n:activeNotes)if(n.active){Serial.println("CHECK903,BUSY_NOTES");return;}
 bool savedTrack=stableTrack904,savedQuick=stableQuick904,savedTransport=stableTransport904;stableTrack904=stableQuick904=stableTransport904=true;
 unsigned pass=0,fail=0;auto check=[&](bool ok,const char*n){ok?++pass:++fail;Serial.printf("CHECK903,%s,%s\n",ok?"PASS":"FAIL",n);};
 bool mapping=true;for(uint8_t p=0;p<56;++p)mapping&=normalize903(1ULL<<p)==(1ULL<<(p<8?p+48:p-8));check(mapping,"56_physical_to_logical_bits");
 uint8_t view=quickView903,track=quickTrack903,pattern=quickPattern903,depth=quickDepth903;uint32_t item=quickItem903,page=quickPatchPage903;
 bool clock=internalClockRunning,latched=menuStopLatched;uint64_t tick=transportTickAbsolute;
 playing=true;internalClockRunning=false;sendTrackMenu();check(playing&&!menuStopLatched&&transportTickAbsolute==tick,"TRACK_does_not_stop_or_seek");
 sendTrackMidi902();check(playing&&!menuStopLatched,"MOUT_navigation_keeps_playing");
 playing=false;internalClockRunning=clock;showGameScreen();menuStopLatched=latched;
 char ep[4][96];memcpy(ep,menuScreenRaw,sizeof(ep));
 const uint8_t bits[]={55,49,54,48,51,50},views[]={2,3,4,5,6,1};
 for(uint8_t i=0;i<6;++i){check(quickButton903(bits[i])&&quickView903==views[i],"quick_button_view");check(!memcmp(ep,menuScreenRaw,sizeof(ep))&&!modUi805&&!patchUi808,"quick_preserves_EP_context");}
 check(!quickButton903(12)&&!quickButton903(13),"save_load_reach_legacy_dispatch");
 check(!quickButton903(14)&&quickButton903(15)&&quickButton903(53),"freed_buttons_consumed");
 {TransportScope903 scope(1);check(softTransport903,"pause_soft");{TransportScope903 stop(2);check(!softTransport903,"stop_always_hard");}check(softTransport903,"nested_transport_restore");}
 check(!softTransport903,"transport_scope_restore");
 // Two enabled groups: cycling remains whole-group, without modifying stored units.
 uint16_t keys[2]={60,62};auto m0=mod806Unit(keys[0]),m1=mod806Unit(keys[1]);auto meta0=modMeta806[keys[0]],meta1=modMeta806[keys[1]];
 for(auto k:keys){auto&m=mod806Unit(k);m.enabled=true;m.inputs=1u<<selectedTrack;}
 quickView903=0;quickChoose903(4);quickItem903=keys[0];quickChoose903(4);check(quickItem903==keys[1],"ENV_cycles_to_next_whole_group");
 quickItem903=keys[0];mod806Unit(keys[0]).hold=10;quickKy903(0,1);
 check(mod806Unit(keys[0]).hold==11&&!modUi805&&!patchUi808&&!memcmp(ep,menuScreenRaw,sizeof(ep)),"quick_ENV_edits_value_preserves_EP");
 mod806Unit(keys[0])=m0;mod806Unit(keys[1])=m1;modMeta806[keys[0]]=meta0;modMeta806[keys[1]]=meta1;
 mod805Invalidate();
 // A 17-parameter patch stays on its own two pages before the next patch.
 PatchParam808 params[17]={};for(unsigned i=0;i<17;++i){params[i].id=0xFFFF0000+i;params[i].bits=7;params[i].next=i<16?&params[i+1]:nullptr;strcpy(params[i].shortName,"T");}
 PatchParam808 secondParam{};secondParam.id=0xFFFF0100;secondParam.bits=7;strcpy(secondParam.shortName,"U");
 PatchDef808 first{},second{};first.id=0xFFFF0000;second.id=0xFFFF0001;first.params=params;second.params=&secondParam;first.next=&second;second.next=patchDb808.defs;
 auto*oldDefs=patchDb808.defs;auto&ctx=quickPatchContext903();auto oldCtx=ctx;ctx={};ctx.patch=first.id;ctx.extra[0]=second.id;ctx.explicit819=true;patchDb808.defs=&first;
 quickView903=0;quickChoose903(6);check(quickItem903==first.id&&quickPatchPage903==0,"patch_first_group");
 quickChoose903(6);check(quickItem903==first.id&&quickPatchPage903==1,"patch_17_keeps_own_second_page");
 quickChoose903(6);check(quickItem903==second.id&&quickPatchPage903==0,"patch_next_group_after_continuation");
 quickChoose903(6);check(quickItem903==first.id&&quickPatchPage903==0,"patch_cycle_wrap");
 ctx=oldCtx;patchDb808.defs=oldDefs;
 quickView903=view;quickTrack903=track;quickPattern903=pattern;quickDepth903=depth;quickItem903=item;quickPatchPage903=page;
 memset(modPreviousLcd806,0,sizeof(modPreviousLcd806));displayValueDirty=true;sendDisplayValuePacket();
 stableTrack904=savedTrack;stableQuick904=savedQuick;stableTransport904=savedTransport;
 Serial.printf("CHECK903,SUMMARY,PASS=%u,FAIL=%u\n",pass,fail);
}
