#pragma once
// Direct, volatile test of physical CV channels; no musical settings are changed.
static uint8_t cvDebugMode47=0,cvDebugMask47=0,cvDebugVolts47=0;
static uint16_t cvDebugHz47=0; // hundredths of a Hz, 0..10 Hz; zero freezes phase
static float cvDebugPhase47=0;static uint32_t cvDebugAt47=0,cvDebugDisplayAt47=0;
FLASHMEM uint16_t cvDebugCode47(float volts){return constrain((int)lroundf(volts*4096.0f/3.3f),0,4095);}
FLASHMEM void cvDebugSample47(uint32_t now,uint16_t*out){
 uint32_t elapsed=now-cvDebugAt47;cvDebugAt47=now;if(cvDebugMode47==1){cvDebugPhase47=fmodf(cvDebugPhase47+(float)elapsed*cvDebugHz47/100000.0f,1.0f);}
 float volts=cvDebugMode47==2?cvDebugVolts47/10.0f:cvDebugMode47==1?(cvDebugVolts47/20.0f)*(1-cosf(6.28318530718f*cvDebugPhase47)):0;
 uint16_t value=cvDebugCode47(volts);for(uint8_t i=0;i<4;++i)out[i]=(cvDebugMask47&(1u<<i))?value:0;
}
FLASHMEM void cvDebugRender47(){
 uint16_t out[4];cvDebugSample47(millis(),out);char a[21],b[21],c[21],d[21];
 snprintf(a,21,"CV %-3s OA %u.%02uHz",cvDebugMode47==1?"SIN":cvDebugMode47==2?"FIX":"OFF",cvDebugHz47/100,cvDebugHz47%100);
 snprintf(b,21,cvDebugMode47==2?"OB FIX %u.%uV":"OB RANGE 0..%u.%uV",cvDebugVolts47/10,cvDebugVolts47%10);
 memset(c,' ',20);memset(d,' ',20);c[20]=d[20]=0;
 for(uint8_t i=0;i<4;++i){char value[6],label[6];unsigned tenths=lroundf(out[i]*33.0f/4096.0f);snprintf(value,6,"%u.%uV",tenths/10,tenths%10);snprintf(label,6,"%cCV%u",cvDebugMask47&(1u<<i)?'*':' ',i+1);memcpy(c+i*5,value,strlen(value));memcpy(d+i*5,label,strlen(label));}
 sendDebugDisplay(a,b,c,d,"EXPECTED VOLTS","J1 BACK");cvDebugDisplayAt47=millis();
}
FLASHMEM void cvDebugStart47(){if(playing){playing=internalClockRunning=false;pendingClockTicks=0;allNotesOff();}cvDebugMode47=cvDebugMask47=cvDebugVolts47=0;cvDebugHz47=0;cvDebugPhase47=0;cvDebugAt47=millis();cvDebugRender47();}
FLASHMEM void cvDebugStop47(){playing=internalClockRunning=false;pidiStateDirty931=true;noInterrupts();pendingClockTicks=0;interrupts();playHeld923=false;allNotesOff();modStop811();synkozStop933();cvDebugMode47=cvDebugMask47=0;uint16_t zero[4]={};pidiPacket827(pidi827::CV46,zero,sizeof(zero));memset(cvValues46,0,sizeof(cvValues46));}
FLASHMEM bool cvDebugValues47(uint16_t*out){if(debugTestMode!=DEBUG_TEST_CV47)return false;cvDebugSample47(millis(),out);if((uint32_t)(millis()-cvDebugDisplayAt47)>=100)cvDebugRender47();return true;}
FLASHMEM void cvDebugRoy47(uint8_t roy,int delta,bool click){
 if(roy>1)return;cvDebugSample47(millis(),cvValues46);
 if(click){cvDebugMode47=roy?2:1;if(!roy)cvDebugPhase47=0;}
 if(delta){if(!roy)cvDebugHz47=constrain((int)cvDebugHz47+delta,0,1000);else cvDebugVolts47=constrain((int)cvDebugVolts47+delta,0,33);}
 cvDebugRender47();
}
FLASHMEM void cvDebugButton47(uint8_t index){if(index>=4&&index<8){cvDebugMask47^=1u<<(index-4);cvDebugRender47();}}
FLASHMEM bool cvFineClick47(uint8_t ky,bool down){
 uint16_t key=(quickView903==4||quickView903==5)?quickItem903:modSelected806;int slot=(quickView903==4||quickView903==5)?quickDepth903:modDepthSlot806;
 if(ky!=8&&ky!=(ioKey38(key)?9:10))return false;
 if((!modUi805&&quickView903!=4&&quickView903!=5)||key>=64||slot<0||slot>7||!mod806Unit(key).outputs[slot].enabled||mod806Unit(key).outputs[slot].kind!=MOD_CV46)return false;
 bool click=!down&&kyBtnDown[ky]&&!kyRotatedWhileDown[ky];kyBtnDown[ky]=down;if(down){kyBtnDownAtMs[ky]=millis();kyRotatedWhileDown[ky]=false;}if(click){auto&step=ky==8?cvDepthStep48:cvBaseStep48;step=step==100?10:step==10?1:100;DISPLAY_SERIAL.printf("MSG|CV %s STEP %u\n",ky==8?"DEP":"REF",step);modDisplayDirty810=displayValueDirty=true;}return true;
}
