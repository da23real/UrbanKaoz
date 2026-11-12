#pragma once
// Diagnostic only. No transport, musical state or persistence side effects.
struct AnalyzerEvent28 { uint32_t us,seq;uint8_t status,a,b,mask; };
static AnalyzerEvent28 analyzerQueue28[512];
static uint16_t analyzerHead28=0,analyzerCount28=0;
static uint32_t analyzerUntil28=0,analyzerSeq28=0,analyzerDrops28=0;
static bool analyzerOn28=false;
static void analyzerPush28(uint8_t s,uint8_t a,uint8_t b,uint8_t mask){
 if(!analyzerOn28)return;
 uint32_t seq=++analyzerSeq28;
 if(analyzerCount28==512){++analyzerDrops28;return;}
 analyzerQueue28[(analyzerHead28+analyzerCount28)%512]={micros(),seq,s,a,b,mask};++analyzerCount28;
}
static bool analyzerCommand28(const char*cmd){
 if(!strcmp(cmd,"MA28,ON")){analyzerOn28=true;analyzerUntil28=millis()+5000;Serial.println("MA28,STATE,1");return true;}
 if(!strcmp(cmd,"MA28,PING")){if(analyzerOn28)analyzerUntil28=millis()+5000;return true;}
 if(!strcmp(cmd,"MA28,OFF")){analyzerOn28=false;analyzerCount28=0;Serial.printf("MA28,STATE,0,%lu\n",(unsigned long)analyzerDrops28);return true;}
 return false;
}
static void analyzerService28(){
 if(!analyzerOn28)return;
 if(!Serial||(int32_t)(millis()-analyzerUntil28)>=0){analyzerOn28=false;analyzerCount28=0;return;}
 for(uint8_t i=0;i<4&&analyzerCount28&&Serial.availableForWrite()>=96;++i){
  const auto&e=analyzerQueue28[analyzerHead28];
  Serial.printf("MA28,E,%lu,%lu,%u,%u,%u,%u,%lu\n",(unsigned long)e.seq,(unsigned long)e.us,e.mask,e.status,e.a,e.b,(unsigned long)analyzerDrops28);
  analyzerHead28=(analyzerHead28+1)%512;--analyzerCount28;
 }
}
