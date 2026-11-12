#pragma once
FLASHMEM bool quickDepthVisible59(uint16_t key,uint8_t slot){if(key>=64||slot>=8)return false;auto&m=mod806Unit(key);auto&r=m.outputs[slot];return r.enabled&&((m.inputs&(1u<<quickTrack903))||(!mod806MidiTarget(r)&&!modIsTarget811(r)&&r.kind!=MOD_VIRTUAL&&r.track==quickTrack903));}
// Reuses the value editors, preserving the complete EP editing context.
struct QuickContext903 {
 uint8_t dirty=modDirty806,draft=modDraftMode810,trigger=modSavedTrigger807;
 uint8_t ui=modUi805,type=modType805,t=modOwnerTrack805,p=modOwnerPattern805,patch=patchUi808;
 uint8_t block=patchBlock21;
 uint8_t delaySlot=delaySlot924;
 uint16_t selected=modSelected806;int8_t depth=modDepthSlot806;bool bank=patchBank808;uint32_t page=patchPage808;
 QuickContext903(){if(quickView903==6)patchBlock21=quickPatchBlock21;quickWorking903=true;modOwnerTrack805=quickTrack903;modOwnerPattern805=quickPattern903;patchUi808=0;if(quickView903==3&&quickItem903<2048){modOwnerTrack805=(quickItem903%256)/16;modOwnerPattern805=quickItem903%16;delaySlot924=quickItem903/256;}}
 ~QuickContext903(){patchBlock21=block;delaySlot924=delaySlot;modDirty806=dirty;modDraftMode810=draft;modSavedTrigger807=trigger;modUi805=ui;modType805=type;modOwnerTrack805=t;modOwnerPattern805=p;patchUi808=patch;modSelected806=selected;modDepthSlot806=depth;patchBank808=bank;patchPage808=page;quickWorking903=false;}
};
FLASHMEM void quickGateClick20(uint32_t held){QuickContext903 context;modUi805=0;patchBank808=false;toggleEditFastFromKy(4,held);}
FLASHMEM PatchContext808& quickPatchContext903(){return patchDb808.context[linkOwner12(quickTrack903,LINK_PATCH12)][patchContextIndex21(0)];}
FLASHMEM void quickGather903(){
 quickCount903=0;
 if(quickView903==6){auto&c=quickPatchContext903();for(uint8_t i=0;i<16;++i){uint32_t id=patchId819(c,i);if(id&&patch808Find(patchDb808,id))quickOrder903[quickCount903++]=id;}}
 else if(quickView903==4||quickView903==5){for(uint16_t k=0;k<64;++k){auto&m=mod806Unit(k);if(!m.enabled||modKind931(k)!=(quickView903==5))continue;
  bool visible=mod806Associated(modMeta806[k],quickTrack903,quickPattern903)||(m.inputs&(1u<<quickTrack903));
  for(auto&r:m.outputs)if(r.enabled&&!mod806MidiTarget(r)&&!modIsTarget811(r)&&r.kind!=MOD_VIRTUAL&&r.track==quickTrack903)visible=true;
  if(visible&&quickCount903<256)quickOrder903[quickCount903++]=k;
 }}else if(quickView903==3){quickCount903=delayList924(quickTrack903,quickPattern903,quickOrder903,256);}else{quickOrder903[0]=0;quickCount903=1;}
}
FLASHMEM void quickChoose903(uint8_t view){
 delayIncrementShown925=0;
 if(view==6){blockPatchChoose21();return;}
 if(remixMode&&view!=3&&view!=6&&remixQv911(view))return;
 if(!stableQuick904&&!remixMode)return;
 if(view==1&&quickView903==1&&quickTrack903==selectedTrack&&quickPattern903==(patternMode&&lastPatternValid&&lastPatternTrack==selectedTrack?lastPatternIndex:activePattern[selectedTrack])){
  quickView903=0;kbZoneViewR11=false;quickDepth903=0;quickPatchPage903=0;
  memset(modPreviousLcd806,0,sizeof(modPreviousLcd806));
  DISPLAY_SERIAL.println("CUSTOM,OFF");displayValueDirty=true;
  sendFourBlockPacket();sendTrackRollPacket();return;
 }
 kbZoneViewR11=false;
 bool next=quickView903==view&&quickTrack903==selectedTrack&&quickPattern903==(patternMode&&lastPatternValid&&lastPatternTrack==selectedTrack?lastPatternIndex:activePattern[selectedTrack]);
 if(next&&view==6&&16*(quickPatchPage903+1)<patchEditableCount44(patch808Find(patchDb808,quickItem903))){++quickPatchPage903;displayValueDirty=true;quickDisplay903();return;}
 if(view==3){bool repeat=quickView903==3&&quickTrack903==selectedTrack;uint32_t previous=quickItem903;quickView903=3;quickTrack903=selectedTrack;quickPattern903=0;quickItem903=selectedTrack*16+delayChosen925[selectedTrack]*256;
  if(repeat){quickGather903();if(quickCount903){unsigned pos=0;while(pos<quickCount903&&quickOrder903[pos]!=previous)++pos;quickItem903=quickOrder903[(pos+1)%quickCount903];if((quickItem903%256)/16==selectedTrack)delayChosen925[selectedTrack]=quickItem903/256;}}
  displayValueDirty=true;quickDisplay903();return;}
 quickDepth903=0;quickPatchPage903=0;quickView903=view;quickTrack903=selectedTrack;quickPattern903=(patternMode&&lastPatternValid&&lastPatternTrack==selectedTrack?lastPatternIndex:activePattern[selectedTrack]);quickGather903();
 unsigned pos=0;if(next){while(pos<quickCount903&&quickOrder903[pos]!=quickItem903)++pos;pos=quickCount903?(pos+1)%quickCount903:0;}
 quickItem903=quickCount903?quickOrder903[pos]:UINT32_MAX;if(view==3)delayChainView924=0;memset(modPreviousLcd806,0,sizeof(modPreviousLcd806));
 displayValueDirty=true;quickDisplay903();
}
FLASHMEM bool quickButton903(uint8_t bit){
 if(bit==52){toggleTrackView905();return true;}
 if(bit==54||bit==48){quickHeld55=bit;quickHeldAt55=millis();if(stableQuick904||remixMode)quickChoose903(bit==54?4:5);return true;}
 if(!stableQuick904&&!remixMode)return bit>=48;
 if(bit==55){quickChoose903(2);return true;}if(bit==49){quickChoose903(3);return true;}
 if(bit==54||bit==48){quickChoose903(bit==54?4:5);quickHeld55=bit;quickHeldAt55=millis();return true;}
 if(bit==51){quickChoose903(6);return true;}if(bit==50){quickChoose903(1);return true;}
 // Preserve legacy Save/Load, including their contextual object editors.
 return bit==15||bit==53;
}
FLASHMEM void quickFromEp903(){
 if(keyboardEnabled823||quickWorking903||!modUi805||modType805>=2)return;
 if(modUi805==MOD_FX806){quickView903=3;quickItem903=modOwnerTrack805*16+delayChosen925[modOwnerTrack805]*256;}
 else if(modUi805==MOD_LIST806){mod806ListCount();quickView903=modType805?5:4;quickItem903=modVisibleCount806?modVisible806[min(modCursor807,(uint16_t)(modVisibleCount806-1))]:UINT32_MAX;}
 else if(modUi805==MOD_DELAY806){quickView903=3;quickItem903=delayKey924(modOwnerTrack805,modOwnerPattern805,delaySlot924);}
 else if(modSelected806<64){quickView903=modKind931(modSelected806)?5:4;quickItem903=modSelected806;}
 else return;
 quickTrack903=modOwnerTrack805;quickPattern903=modOwnerPattern805;displayValueDirty=true;
}
FLASHMEM void quickPatchEp903(){} // EP patch browsing does not select a QV.
FLASHMEM void quickObject903(){
 modUi805=MOD_OBJECT806;modSelected806=quickItem903;modType805=quickView903==5;modDepthSlot806=-1;
 for(uint8_t n=0;n<8;++n){uint8_t i=(quickDepth903+n)%8;if(quickDepthVisible59(quickItem903,i)){modDepthSlot806=i;break;}}
}
FLASHMEM bool quickDisplay903(){
 if(!quickView903||quickWorking903)return false;
 QuickContext903 saved;
 if(quickView903==1){modUi805=0;patchBank808=false;blockDisplay21();return true;}
 if(quickView903==2){rollQuickDisplay1000();
return true;
auto r=displayedRollForTrack(quickTrack903);
const char*names[]={"Start","Speed","Accel","Sex","Curve","RVL"};
for(uint8_t screen=0;
screen<4;
++screen){char a[12]={},b[12]={};
uint8_t x=screen*2;
if(x<6)snprintf(a,sizeof(a),"%d",rollFieldValue(r,x));
if(x+1<6)snprintf(b,sizeof(b),"%d",rollFieldValue(r,x+1));
mod806Small(screen,x<6?names[x]:"",x<6?a:"",x+1<6?names[x+1]:"",b);
}return true;
}
 if(quickView903==3){if(quickItem903>=2048){for(uint8_t i=0;i<4;++i)mod806Small(i,i?"":"No Delay","","","");return true;}modUi805=MOD_DELAY806;modSelected806=UINT16_MAX;return modDisplay810();}
 if(quickView903==4||quickView903==5){if(quickItem903>=64||!mod806Unit(quickItem903).enabled){for(uint8_t i=0;i<4;++i)mod806Small(i,i?"":quickView903==4?"No ENV":"No LFO","","","");return true;}quickObject903();return modDisplay810();}
 if(quickView903==6){auto*d=patch808Find(patchDb808,quickItem903);
auto&c=quickPatchContext903();
for(uint8_t screen=0;
screen<4;
++screen){uint16_t blink11=0;char labels[17],values[17];
memset(labels,' ',16);
memset(values,' ',16);
labels[16]=values[16]=0;
for(uint8_t i=0;
i<4;
++i){auto*q=patchEditableAt44(d,quickPatchPage903*16+screen*4+i);
if(!q)continue;
{const char*label=patchLabel10(c,*q,true);memcpy(labels+i*4,label,min((size_t)3,strlen(label)));}
uint16_t v=0;
char b[8];
bool known=patchRawValue50(c,*q,v);if(remixMode)v=remixPatchValue937(*q,v);int shown=lockDisplay817(q->kind?(q->bits==14?LOCK_NRPN14817:LOCK_NRPN7817):LOCK_CC817,quickTrack903,q->number,v);if(shown!=v)blink11|=15u<<(i*4);if(known||lockWantsEdit817()||(remixMode&&remixSelection911.steps)){v=shown;if(v==65535)strcpy(b,"-");else if(v>9999)snprintf(b,sizeof(b),"%uk",v/1000);
else snprintf(b,sizeof(b),"%u",v);
}else strcpy(b,"--");
memcpy(values+i*4,b,min((size_t)4,strlen(b)));
}lockBlinkSend11(screen,blink11);DISPLAY_SERIAL.print("MODLCD|");
DISPLAY_SERIAL.print(screen);
DISPLAY_SERIAL.print('|');
DISPLAY_SERIAL.print(labels);
DISPLAY_SERIAL.print('|');
DISPLAY_SERIAL.println(values);
}return true;
}
 return true;
}
FLASHMEM bool quickKy903(uint8_t ky,int32_t delta){
 if(!quickView903)return false;if(!delta)return true;
 QuickContext903 saved;
 if(quickView903==1){modUi805=0;patchBank808=false;return blockKy21(ky,delta);}
 if(quickView903==2){ky=rollKey26(ky);if(ky==5||ky==9){applyNoteVelGateTimeDelta(ky==5?5:3,delta);return true;}if(rollQuickEdit1000(ky,delta))return true;if(ky%2==0&&ky<12)editTrackRollFromKy(ky/2,delta);displayValueDirty=true;return true;}
 if(quickView903==3){if(quickItem903>=2048)return true;modUi805=MOD_DELAY806;if(delayTargetsKy55(ky,delta))return true;modKy810(ky,delta);displayValueDirty=true;return true;}
 if(quickView903==4||quickView903==5){if(quickItem903>=64||!mod806Unit(quickItem903).enabled)return true;
  
  quickObject903();modKy810(ky,curveKey41(ky)?delta:(delta>0?1:-1)*(kyBtnDown[ky]?10:1));displayValueDirty=true;return true;
 }
 if(quickView903==6){auto*q=patchEditableAt44(patch808Find(patchDb808,quickItem903),quickPatchPage903*16+ky);if(!q)return true;
  auto&c=quickPatchContext903();
  uint16_t old=0;bool defined59=patchRawValue50(c,*q,old);if((lockWantsEdit817()||(remixMode&&remixSelection911.steps))&&indBase50(quickTrack903,activePattern[quickTrack903],q->kind?(q->bits==14?LOCK_NRPN14817:LOCK_NRPN7817):LOCK_CC817,quickTrack903,q->number,defined59?old:65535)==65535){lockBlinkSend11(ky/4,15u<<((ky%4)*4));DISPLAY_SERIAL.println("MSG|SET PATCH BASE FIRST");return true;}if(old==65535)old=0;if(remixMode){remixPatchEdit937(*q,old,delta);displayValueDirty=true;return true;}
  if(!lockEdit817(q->kind?(q->bits==14?LOCK_NRPN14817:LOCK_NRPN7817):LOCK_CC817,quickTrack903,q->number,old,delta,0,q->bits==14?16383:127)){
   uint16_t value=WRAP23(old,delta,0,q->bits==14?16383:127);bool ok51=true;for(auto*x=patchNext819(c,nullptr);x;x=patchNext819(c,x))if(patchSameAddress819(*x,*q)&&!patch808Set(c,x->id,value))ok51=false;if(ok51)patchChanged15(quickTrack903,c,*q,value);++modEpoch805;
  }displayValueDirty=true;return true;
 }return true;
}
struct TransportSettings903 {uint32_t magic;uint8_t notes,delay,check;};
FLASHMEM void loadTransport903(){TransportSettings903 s{};EEPROM.get(3040,s);if(s.magic==0x903C011&&s.notes<2&&s.delay<2&&s.check==(s.notes^s.delay^0x93)){cutNotes903=s.notes;cutDelay903=s.delay;}}
FLASHMEM void toggleTransport903(uint8_t index){pidiStateDirty931=true;if(index==6)cutDelay903=!cutDelay903;else cutNotes903=!cutNotes903;TransportSettings903 s{0x903C011,(uint8_t)cutNotes903,(uint8_t)cutDelay903,(uint8_t)(cutNotes903^cutDelay903^0x93)};EEPROM.put(3040,s);DISPLAY_SERIAL.printf("MSG|Cut %s %s\n",index==6?"Delay":"Notes",(index==6?cutDelay903:cutNotes903)?"ON":"OFF");}
FLASHMEM void transportCut903(){
 keepDelayPaused903=!cutDelay903;
 if(cutDelay903)pidiCancelDelays924();
 if(cutNotes903){recordClose820();keyboardLightsClear823();lockStop817();for(auto&n:activeNotes)if(n.active&&(cutDelay903||n.previewSlot!=254)){MidiScope902 route(n.outputMask902,n.track);noteOffNow(n.channel,n.note);n.active=false;}}
}


FLASHMEM void delayQuickKey925(uint8_t ky){if(ky==14){delayCut12^=1u<<linkOwner12(modOwnerTrack805,LINK_DLY12);modDisplayDirty810=displayValueDirty=true;return;}if(ky==2||ky==3)delayIncrement925(ky==2?1:0);}

FLASHMEM bool curveKey41(uint8_t ky){
 if(menuNavigationOnly()&&!modUi805)return false;
 if(quickView903==4||quickView903==5){if(quickItem903>=64||ioKey38(quickItem903))return false;return quickView903==5?ky==15:(ky==3||ky==5||ky==13);}
 if(quickView903==3||(!quickView903&&modUi805==MOD_DELAY806))return ky==5||ky==13;
 if(quickView903==2){uint8_t k=rollKey26(ky);return k==6||k==8;}
 if(!quickView903&&modUi805&&modSelected806<64&&!ioKey38(modSelected806))return modType805?ky==15:(ky==3||ky==5||ky==13);
 return !quickView903&&!patternMode&&!grooveEditMode&&!patchBank808&&(ky==13||ky==14);
}

FLASHMEM void quickHoldService55(){
 if(quickHeld55==255)return;
 if(!hc165ProbeStable[quickHeld55]||settingsOpen||controller940||sdBusy905()){quickHeld55=255;return;}
 if((uint32_t)(millis()-quickHeldAt55)<600)return;
 uint8_t view=quickHeld55==48?5:4;quickHeld55=255;
 if(quickView903!=view||quickTrack903!=selectedTrack){quickView903=view;quickTrack903=selectedTrack;quickPattern903=activePattern[selectedTrack];quickGather903();quickItem903=quickCount903?quickOrder903[0]:UINT32_MAX;}
 if(quickItem903>=64||!mod806Unit(quickItem903).enabled){modOwnerTrack805=selectedTrack;modOwnerPattern805=activePattern[selectedTrack];modType805=view==5;modUi805=MOD_LIST806;modCursor807=0;patchUi808=0;mod805Menu();return;}
 modOwnerTrack805=quickTrack903;modOwnerPattern805=quickPattern903;patchUi808=0;quickObject903();mod805Menu();mod805Display();
}
#include "GraphPreview59.h"
