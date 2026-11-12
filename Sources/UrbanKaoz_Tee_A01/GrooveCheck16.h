#pragma once
// Pure fixture, no project readback, backup, mutation or musical emission.
FLASHMEM void checkGroove16(){
 if(playing){Serial.println("CHECKGROOVE16,BUSY");return;}
 int8_t values[16]={};values[3]=-5;values[6]=2;values[11]=-4;values[14]=3;
 uint32_t checks=0,fail=0;uint32_t start=micros();int checksum=0;
 for(int sm=0;sm<=100;sm+=25)for(int w=0;w<=100;w+=25)for(int t=0;t<1536;t+=12){
  int q=groove16::q8(values,1,t,sm,w);++checks;fail+=q< -1280||q>768;
  ++checks;fail+=q!=groove16::q8(values,1,t+1536,sm,w);checksum+=q;
 }
 Serial.printf("CHECKGROOVE16,PASS=%lu,FAIL=%lu,US=%lu,SUM=%d\n",checks-fail,fail,(uint32_t)(micros()-start),checksum);
 for(int t=240;t<=384;t+=12)Serial.printf("G16SAMPLE,T=%d,TIM=%d\n",t,groove16::ticks(values,1,t,50,50));
}
