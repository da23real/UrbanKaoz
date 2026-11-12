#pragma once
#include "MidiAnalyzer28.h"
EXTMEM uint8_t pidiQueue827[16384];
// Keep the UART transmitting through the measured I2C input batches.
// 512 bytes cover 20.48 ms at 250 kbaud; this is not a larger musical queue.
DMAMEM uint8_t pidiUartTx29[512];
static uint16_t pidiHead827=0,pidiTail827=0,pidiCount827=0,pidiSeq827=0;
static bool pidiStarted827=false,pidiReset827=true,pidiHealthy827=true,pidiStateValid827=false;
static pidi827::State pidiState827{};
static uint32_t pidiFrames827=0,pidiDelays827=0,pidiOverflows827=0,pidiMaxService827=0;
FLASHMEM void pidiDrain827(){for(unsigned i=0;i<256&&pidiCount827&&Serial8.availableForWrite()>0;++i){Serial8.write(pidiQueue827[pidiHead827]);pidiHead827=(pidiHead827+1)%sizeof(pidiQueue827);--pidiCount827;}}
FLASHMEM bool pidiPacket827(uint8_t type,const void*p,unsigned n){
 Scope830 measured(P830_PIDI);
 if(pinTestActive33&&type!=pidi827::PINTEST33&&type!=pidi827::HEARTBEAT&&type!=pidi827::RESET)return false;
 if(!pidiStarted827||!pidiHealthy827)return false;
 uint8_t frame[2*(pidi827::PAYLOAD+5)+2];unsigned size=pidi827::encode(type,pidiSeq827,p,n,frame);
 if(!size)return false;if(size>sizeof(pidiQueue827)-pidiCount827){++pidiOverflows827;pidiHealthy827=false;return false;}
 ++pidiSeq827;++pidiFrames827;for(unsigned i=0;i<size;++i){pidiQueue827[pidiTail827]=frame[i];pidiTail827=(pidiTail827+1)%sizeof(pidiQueue827);}pidiCount827+=size;benchQueuePeak29=max(benchQueuePeak29,(uint32_t)pidiCount827);pidiDrain827();return true;
}
FLASHMEM void pidiStateSend827(){
 if(!pidiStateDirty931)return;pidiStateDirty931=false;songStateDirty931=true;
 static pidi827::State924 previous{};
 pidi827::State924 s{0,(uint8_t)(!controller940&&(playing||remixMode)),(uint8_t)(!controller940&&stableTransport904&&!cutDelay903)};
 for(uint8_t t=0;t<16;++t)if(!trackSilent818(t)||!(delayMute933&(1u<<t)))s.audible|=1u<<t;
 if((!pidiStateValid827||memcmp(&s,&previous,sizeof(s)))&&pidiPacket827(pidi827::STATE924,&s,sizeof(s))){previous=s;pidiStateValid827=true;}
}
FLASHMEM void pidiEnsure827(){
 if(pidiStarted827&&pidiReset827){pidiHealthy827=true;pidiReset827=false;pidiInvalidate924();remoteInvalidate54();fxDirty32=0xffff;fxClockValid32=false;pidiPacket827(pidi827::RESET,nullptr,0);pidiPacket827(pidi827::LABEL07,&delayLabelChannel07,1);pidiPacket827(pidi827::EXP14,&expDelayMaximum15,2);pidiStateValid827=false;pidiStateDirty931=true;pidiRoutesDirty931=0xffff;}
}
FLASHMEM void pidiInit827(){uint8_t data[2];EEPROM.get(3730,data);if(data[0]==0xD7&&data[1]<=16)delayLabelChannel07=data[1];pinMode(33,OUTPUT);digitalWrite(33,HIGH);Serial8.begin(pidi827::BAUD);Serial8.addMemoryForWrite(pidiUartTx29,sizeof(pidiUartTx29));pidiStarted827=true;pidiReset827=true;}
FLASHMEM void pidiReload827(){delayImport925();pidiReset827=true;}
FLASHMEM void pidiMidi827(uint8_t s,uint8_t a,uint8_t b){if(controller940)return;pidiEnsure827();uint8_t mask=midiMask902(s&15);if(!mask)return;fxSync32();uint8_t bytes[]={s,a,b,mask,midiTrack940};bool tracked32=midiTrack940<16;if(pidiPacket827(tracked32?pidi827::MIDI32:pidi827::MIDI,bytes,tracked32?5:4)){benchMidi29(s,b,mask);analyzerPush28(s,a,b,mask);if(!captureSuppress15)synkozNote934(s,a,b);if((s&0xf0)==0xC0)patchAfterProgram54(s&15,mask);}}
FLASHMEM void pidiCancel827(){if(suppressCancel903)return;pidiEnsure827();pidiPacket827(pidi827::CANCEL,nullptr,0);}
FLASHMEM void pidiService827(){uint32_t began=profile830?micros():0;pidiEnsure827();pidiDrain827();fxSync32();pidiSync925();pidiRoutes925();static uint32_t heartbeatAt=0;if(millis()-heartbeatAt>=500){heartbeatAt=millis();pidiPacket827(pidi827::HEARTBEAT,nullptr,0);}pidiStateSend827();analyzerService28();if(profile830)pidiMaxService827=max(pidiMaxService827,(uint32_t)(micros()-began));}
FLASHMEM void rollTriggerMod827(uint8_t t,uint32_t gateUs){
 if(!playing||!rollTrigger817)return;
 rollModAt827[t]=songEngineActive?songTick:transportTickAbsolute;rollModGate827[t]=(uint32_t)max(1.0f,gateUs/(tickDurationMs()*1000.0f));++modEpoch805;
}
FLASHMEM void pidiStats827(){Serial.printf("REMOTE54,NODES=%u,TEE_GROUPS=%lu,CAPACITY_GROUPS=%lu\n",remoteNodes54,remoteFallback54,remoteCapacity54);Serial.printf("PIDI827,HEALTHY=%u,QUEUE=%u,FRAMES=%lu,DELAYS=%lu,OVERFLOW=%lu,SERVICE_MAX_US=%lu\n",pidiHealthy827,pidiCount827,pidiFrames827,pidiDelays827,pidiOverflows827,pidiMaxService827);}

#include "PidiDelay924.h"

