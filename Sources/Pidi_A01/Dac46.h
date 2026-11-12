#pragma once
#include <Wire.h>
namespace dac46 {
// Pico physical 1 (GP0/SDA), 2 (GP1/SCL), 3.3 V. Never drive either line high.
static uint8_t address=0;static uint16_t wanted[4]={},sent[4]={65535,65535,65535,65535};
static uint8_t lastError=0,lastRead=0;static uint32_t errors=0,writes=0,at=0;
static uint32_t readErrors=0,reads=0,recoveries=0,recoveryFailures=0,recoveryAt=0,scanAt=0;
static bool dirty=true,recoveryPending=false,failed=false;
static void configure(){Wire.setSDA(0);Wire.setSCL(1);Wire.begin();Wire.setClock(100000);Wire.setTimeout(5);}
static bool recover(){
 ++recoveries;recoveryAt=millis();Wire.end();
 pinMode(0,INPUT_PULLUP);pinMode(1,INPUT_PULLUP);delayMicroseconds(5);
 for(unsigned i=0;i<9&&!digitalRead(0)&&digitalRead(1);++i){
  digitalWrite(1,LOW);pinMode(1,OUTPUT);delayMicroseconds(5);pinMode(1,INPUT_PULLUP);delayMicroseconds(5);
 }
 if(digitalRead(1)){digitalWrite(1,LOW);pinMode(1,OUTPUT);digitalWrite(0,LOW);pinMode(0,OUTPUT);delayMicroseconds(5);pinMode(1,INPUT_PULLUP);delayMicroseconds(5);pinMode(0,INPUT_PULLUP);delayMicroseconds(5);}
 bool ok=digitalRead(0)&&digitalRead(1);if(!ok)++recoveryFailures;
 configure();dirty=true;recoveryPending=!ok;return ok;
}
static void scan(){scanAt=millis();address=0;for(uint8_t a=0x60;a<=0x67;++a){Wire.beginTransmission(a);if(!Wire.endTransmission()){address=a;break;}if(!digitalRead(0)||!digitalRead(1)){recoveryPending=true;break;}}}
static void set(const uint16_t*v){for(unsigned i=0;i<4;++i){if(wanted[i]!=v[i])dirty=true;wanted[i]=v[i];}}
static void begin(){configure();if(!digitalRead(0)||!digitalRead(1))recover();scan();}
static void service(){uint32_t now=millis();
 if(recoveryPending){if((uint32_t)(now-recoveryAt)<250)return;if(!recover())return;}
 if(!address){if((uint32_t)(now-scanAt)>=1000)scan();return;}
 if(!dirty||(uint32_t)(now-at)<(failed?50u:2u))return;at=now;Wire.beginTransmission(address);
 // Volatile multi-write: VDD reference (0..3.3 V), normal operation.
 // Never writes EEPROM; all four channels latch on their last data byte (UDAC=0).
 for(unsigned i=0;i<4;++i){Wire.write(uint8_t(0x40|(i<<1)));Wire.write(uint8_t(wanted[i]>>8));Wire.write(uint8_t(wanted[i]));}
 lastError=Wire.endTransmission();failed=lastError!=0;if(failed){++errors;recoveryPending=true;return;}memcpy(sent,wanted,sizeof(sent));++writes;dirty=false;}
static void readback(){unsigned n=address?Wire.requestFrom(address,(size_t)24):0;lastRead=n;Serial.printf("CVREAD46,N=%u,DATA=",n);while(Wire.available())Serial.printf("%02X",Wire.read());Serial.println();if(n==24)++reads;else{++readErrors;recoveryPending=true;}}
static void report(){Serial.printf("CV46,ADDR=%u,LAST=%u,SDA=%u,SCL=%u,ERRORS=%lu,WRITES=%lu,A=%u,B=%u,C=%u,D=%u\n",address,lastError,digitalRead(0),digitalRead(1),errors,writes,sent[0],sent[1],sent[2],sent[3]);Serial.printf("CV48,READS=%lu,READ_ERRORS=%lu,LAST_READ=%u,RECOVERIES=%lu,RECOVERY_FAILURES=%lu,PENDING=%u,WANTED=%u/%u/%u/%u\n",reads,readErrors,lastRead,recoveries,recoveryFailures,recoveryPending,wanted[0],wanted[1],wanted[2],wanted[3]);}
}
