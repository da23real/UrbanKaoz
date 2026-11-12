#pragma once
// Mean of depth-scaled active sources, then one common base per physical CV.
FLASHMEM void cvRaw50(int64_t at,uint16_t*out){
 float sum[4]={};uint16_t count[4]={},base[4]={};
 for(uint16_t n=0;n<modEnabledCount805;++n){uint16_t k=modEnabled805[n];if(remoteOwn54(k))continue;uint16_t owners50=indModOwners50(k);for(unsigned pass50=0;pass50<17;++pass50){if(owners50?(pass50==16||!(owners50&(1u<<pass50))):pass50!=16)continue;IndContext50 context50(pass50<16?pass50:-1);if(!playbackMod828(k))continue;
 bool active=(playing&&mod806Playing(k,at))||ioActive38(k)||ccActive941(k)||(modKind931(k)&&modMode810(k)==2)||manualModActive06(k);
 auto m=mod808EffectiveUnit(k);for(uint8_t j=0;j<8;++j){auto&r=m.outputs[j];if(!r.enabled||r.kind!=MOD_CV46||r.track>=4)continue;base[r.track]=r.reference;if(!active)continue;float d=modDepth810(k,j,at);if(d){sum[r.track]+=modRouteValues46[k][j]*d;++count[r.track];}}}
 }autoCv46(sum,count,base);for(unsigned i=0;i<4;++i)out[i]=clampInt(base[i]+(count[i]?lroundf(sum[i]/count[i]):0),0,4095);
}
struct CvHistory50 {int64_t tick;uint16_t values[4];};
static EXTMEM CvHistory50 cvHistory50[512];static uint16_t cvHistorySize50=0,cvHistoryHead50=0;static int64_t cvHistoryLast50=-1;
FLASHMEM void cvCollect46(int64_t at){
 static bool was=false;bool testing=debugTestMode==DEBUG_TEST_CV47;if(!testing&&!cvEnabled46&&!autoCvPresent46&&!was)return;was=testing||cvEnabled46||autoCvPresent46;
 uint16_t out[4];{QueryTime50 q(at,0);cvRaw50(at,out);}
 if(!playing||at<cvHistoryLast50){cvHistorySize50=cvHistoryHead50=0;}cvHistoryLast50=at;
 if(playing&&!testing){
  if(!cvHistorySize50||cvHistory50[(cvHistoryHead50+511)%512].tick!=at){auto&h=cvHistory50[cvHistoryHead50];h.tick=at;memcpy(h.values,out,sizeof(out));cvHistoryHead50=(cvHistoryHead50+1)%512;cvHistorySize50=min((uint16_t)512,(uint16_t)(cvHistorySize50+1));}
  for(unsigned ch=0;ch<4;++ch){int shift=cvShift50[ch];if(shift<0){uint16_t future[4];QueryTime50 q(at-shift,-shift);cvRaw50(at-shift,future);out[ch]=future[ch];}
   else if(shift>0){bool found=false;for(unsigned n=0;n<cvHistorySize50;++n){auto&h=cvHistory50[(cvHistoryHead50+511-n)%512];if(h.tick<=at-shift){out[ch]=h.values[ch];found=true;break;}}if(!found)out[ch]=0;}}
 }
 cvDebugValues47(out);memcpy(cvValues46,out,sizeof(out));static uint16_t previous[4]={65535,65535,65535,65535};static uint32_t when=0;
 if((uint32_t)(millis()-when)<(testing?32u:2u))return;
 if((memcmp(previous,out,sizeof(out))||(uint32_t)(millis()-when)>=500)&&pidiPacket827(pidi827::CV46,out,sizeof(out))){memcpy(previous,out,sizeof(out));when=millis();}
}
static uint8_t joyValues46[6]={64,0,0,64,0,0};
// Raw-minus-delta is the controller's startup center. Endpoints are ADC limits.
FLASHMEM uint8_t joyMap46(int raw,int delta,uint8_t mode){
 int center=clampInt(raw-delta,1,1022);int v=clampInt(raw,0,1023)-center;
 if(abs(v)<=8)v=0;
 int positive=v>0?min(127,(v*127+(1023-center)/2)/(1023-center)):0;
 int negative=v<0?min(127,(-v*127+center/2)/center):0;
 // X is inverted at the call site: HIG means right for X and up for Y.
 return mode==1?positive:mode==2?negative:v>=0?64+(positive*63+63)/127:64-(negative*64+63)/127;
}
FLASHMEM bool joyConflict46(uint16_t key){if(key>=64||modUnits805[key].hold<7)return false;uint8_t axis=(modUnits805[key].hold-7)/3;for(uint16_t k=0;k<64;++k)if(k!=key&&modUnits805[k].enabled&&ioKey38(k)&&modUnits805[k].hold>=7&&(modUnits805[k].hold-7)/3==axis)return true;return false;}
FLASHMEM void joyInput46(int x,int y,int dx,int dy){
 bool changed=false;for(uint8_t a=0;a<2;++a)for(uint8_t m=0;m<3;++m){uint8_t v=joyMap46(a?y:1023-x,a?dy:-dx,m),i=a*3+m;if(abs((int)v-joyValues46[i])>=2||v==0||v==64||v==127)joyValues46[i]=v;}
 for(uint16_t k=0;k<64;++k){const auto&m=modUnits805[k];if(!m.enabled||!ioKey38(k)||m.hold<7||m.hold>12)continue;uint8_t v=joyValues46[m.hold-7];if(ioSeen38[k]&&ioSample38[k]!=v)autoIoInput46(k,v);changed|=!ioSeen38[k]||ioSample38[k]!=v;ioSeen38[k]=true;ioSample38[k]=v;}if(changed)++modEpoch805;
}
