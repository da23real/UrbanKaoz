#pragma once
static void pinTestClear33(){
 effects32::clear();delayUsb07::cancel();multi.cancel();engine.cancel();
 engine.playing=engine.continuePaused=false;engine.count=engine.head=engine.tail=0;
 for(auto&g:engine.gates)g.used=false;memset(engine.owners,0,sizeof(engine.owners));
 for(auto&o:outputs)o=PortTx{};
 memset(active903,0,sizeof(active903));memset(release903,0,sizeof(release903));releasesPending903=false;
}
static void pinTestEnd33(){
 if(!pinTestActive33)return;
 for(uint8_t i=0;i<8;++i)digitalWrite(i+2,HIGH);
 pinTestClear33();pinTestActive33=false;pinTestHigh33=1;
 for(auto*port:ports)port->begin(31250);
}
static void pinTestSet33(uint8_t port,uint8_t high){
 if(port==255){pinTestEnd33();return;}
 if(!pinTestActive33){
  pinTestClear33();for(auto*serial:ports)serial->end();
  for(uint8_t i=0;i<8;++i){digitalWrite(i+2,HIGH);pinMode(i+2,OUTPUT);}
  pinTestActive33=true;
 }
 // Return the previous selected output high before lowering the next one.
 for(uint8_t i=0;i<8;++i)if(i!=port)digitalWrite(i+2,HIGH);
 digitalWrite(port+2,high?HIGH:LOW);
 pinTestPort33=port;pinTestHigh33=high;pinTestSeen33=millis();
}
static void pinTestReport33(){uint8_t levels=0;for(uint8_t i=0;i<8;++i)if(digitalRead(i+2))levels|=1u<<i;Serial.printf("PINTEST33,ACTIVE=%u,OUT=%u,HIGH=%u,READ_MASK=%u\n",pinTestActive33,pinTestPort33+1,pinTestHigh33,levels);}
