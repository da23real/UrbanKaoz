#pragma once
static bool captureSuppress15=false;
static uint8_t captureCc15[16][128],captureChannel15[16][128];
static bool captureKnown15[16][128];
static bool captureTaking15=false;
FLASHMEM void captureOffer15(uint8_t channel,uint16_t object,int value){
 if(!synkozTaking933){captureTaking15=false;return;}if(!captureTaking15){memset(captureKnown15,0,sizeof(captureKnown15));captureTaking15=true;}
 if(midiTrack940>=16||object>=128)return;auto t=midiTrack940;
 if(captureKnown15[t][object]&&captureChannel15[t][object]==channel&&captureCc15[t][object]==value)return;
 captureKnown15[t][object]=true;captureChannel15[t][object]=channel;captureCc15[t][object]=value;synkozNote934(0xB0|channel,object,value);
}
struct MidiPending810 {int16_t value,last;uint32_t first,changed,sent;bool pending,known,modulated;uint8_t track940;};
EXTMEM MidiPending810 midiPending810[128][129];
struct NrpnPending810 {NrpnPending810*next;uint16_t number,value;uint8_t channel,bits,output,track940;uint32_t first;};
NrpnPending810*nrpnPending810=nullptr;bool midiLastNrpn810=false;
struct MidiStats810 {uint32_t offered,replaced,duplicates,sent,uartBusy,overflow,maxWaitUs,maxValueAgeUs,maxLoopUs;uint16_t pending,peak;};
MidiStats810 midiCounters810={};
static uint32_t midiPendingBits55[516]={};
static int midiNext55(unsigned start,unsigned end){
 while(start<end){unsigned word=start/32;uint32_t bits=midiPendingBits55[word]&(~0u<<(start%32));
  if(bits){unsigned found=word*32+__builtin_ctz(bits);return found<end?(int)found:-1;}
  start=(word+1)*32;
 }return -1;
}
uint16_t midiCursor810=0;uint32_t midiCreditAt810=0;float midiCredit810=6;
FLASHMEM void midiReset810(){memset(midiPendingBits55,0,sizeof(midiPendingBits55));midiCursor810=0;modReset937();memset(midiPending810,0,sizeof(midiPending810));while(nrpnPending810){auto*p=nrpnPending810;nrpnPending810=p->next;extmem_free(p);}midiLastNrpn810=false;midiCounters810={};midiCreditAt810=micros();midiCredit810=6;}
FLASHMEM void midiForce810(uint8_t c,uint16_t o){if(c<16&&o<=128)for(unsigned port=0;port<8;++port)if(midiMask902(c)&(1u<<port))midiPending810[port*16+c][o].known=false;}
FLASHMEM void midiOfferPort902(uint8_t c,uint16_t o,int value,bool modulated,uint8_t port){
  Scope830 measured(P830_OFFER);
  if(c>=16||o>128)return;MidiScope902 route(1u<<port);if(remoteAddress54(o==128?1:0,port,c,o==128?0:o)){if(!modulated)remoteBase54(o==128?1:0,c,o==128?0:o,value,1u<<port);return;}if(o<128){int base=modulated?modBase19[port*16+c][o]:value;value=lockMidiOffer817(c,o,base)+(modulated?(modMidiMix806[port*16+c][o].count?(int)lroundf(modMidiMix806[port*16+c][o].value()):value-base):0);}auto&p=midiPending810[port*16+c][o];uint32_t now=micros();++midiCounters810.offered;
  value=clampInt(value,o==128?-8192:0,o==128?8191:127);captureOffer15(c,o,value);
  if(o==128&&p.known&&p.last!=midiChannelPitchBendSent[c])p.known=false;
  if((p.pending&&p.value==value)||(!p.pending&&p.known&&p.last==value)){if(p.pending&&!modulated)p.modulated=false;++midiCounters810.duplicates;return;}
  if(!p.pending){p.first=now;p.pending=true;++midiCounters810.pending;midiCounters810.peak=max(midiCounters810.peak,midiCounters810.pending);}else ++midiCounters810.replaced;
  midiPendingBits55[(port*16*129+c*129+o)/32]|=1u<<((port*16*129+c*129+o)%32);
  p.value=value;p.track940=midiTrack940;p.changed=now;p.modulated=modulated;
  if(p.known&&p.last==value){p.pending=false;--midiCounters810.pending;}
}
FLASHMEM void midiOffer810(uint8_t c,uint16_t o,int value,bool modulated){uint8_t mask=midiMask902(c);for(uint8_t port=0;port<8;++port)if(mask&(1u<<port))midiOfferPort902(c,o,value,modulated,port);}
FLASHMEM void midiCancel810(uint8_t c,uint16_t o){for(uint8_t port=0;port<8;++port){auto&p=midiPending810[port*16+c][o];if(p.pending&&p.modulated){p.pending=false;--midiCounters810.pending;}}}
FLASHMEM void midiClearStats810(){uint16_t pending=midiCounters810.pending;midiCounters810={};midiCounters810.pending=midiCounters810.peak=pending;}
FLASHMEM void midiNrpnPort902(uint8_t c,uint16_t number,uint8_t bits,uint16_t value,uint8_t port){
  if(c>=16||number>16383||(bits!=7&&bits!=14)||value>(bits==14?16383:127))return;
  MidiScope902 route(1u<<port);if(remoteBase54(bits==14?3:2,c,number,value,1u<<port))return;int delta54=nrpnOffset54(port,c,number,bits);int base54=nrpnIsMod54?nrpnBase54(port,c,number,bits,value):value;nrpnPrevious54(bits==14?LOCK_NRPN14817:LOCK_NRPN7817,c,number,port,base54);value=clampInt(lockValue817(bits==14?LOCK_NRPN14817:LOCK_NRPN7817,c,number,base54)+delta54,0,bits==14?16383:127);
  auto tail=&nrpnPending810;
  while(*tail){auto*p=*tail;if(p->channel==c&&p->number==number&&p->output==port){p->value=value;p->bits=bits;p->track940=midiTrack940;++midiCounters810.replaced;return;}tail=&p->next;}
  auto*p=(NrpnPending810*)extmem_malloc(sizeof(NrpnPending810));
  if(!p){++midiCounters810.overflow;return;}*p={nullptr,number,value,c,bits,port,midiTrack940,micros()};*tail=p;
}
FLASHMEM void midiNrpn810(uint8_t c,uint16_t number,uint8_t bits,uint16_t value){uint8_t mask=midiMask902(c);for(uint8_t port=0;port<8;++port)if(mask&(1u<<port))midiNrpnPort902(c,number,bits,value,port);}
FLASHMEM void midiRaw810(uint8_t status,uint8_t a,uint8_t b){pidiMidi827(status,a,b);}
FLASHMEM void midiService810(){
  Scope830 measured(P830_MIDI);
  uint32_t now=micros(),elapsed=now-midiCreditAt810;midiCreditAt810=now;
  midiCredit810=min(18.0f,midiCredit810+min(elapsed,100000u)*((float)ccBudget810*3/1000000));
  bool nrpnWaiting=nrpnPending810!=nullptr;
  bool reserveNrpn=nrpnWaiting&&!midiLastNrpn810;
  // Accumulate one complete NRPN burst before accepting further CC traffic.
  // Keep at least twelve UART bytes for notes/off. Never wait for UART space.
  unsigned remaining55=128*129;
  for(uint16_t visited=0;!reserveNrpn&&midiCounters810.pending&&remaining55&&visited<128*129;++visited){
    unsigned start55=midiCursor810,span55=min(remaining55,128u*129-start55);
    int found55=midiNext55(start55,start55+span55);
    if(found55<0){remaining55-=span55;midiCursor810=(start55+span55)%(128*129);continue;}
    uint16_t i=found55;remaining55-=i-start55+1;midiCursor810=(i+1)%(128*129);
    auto&p=midiPending810[i/129][i%129];if(!p.pending){midiPendingBits55[i/32]&=~(1u<<(i%32));continue;}
    midiCounters810.maxWaitUs=max(midiCounters810.maxWaitUs,now-p.first);
    if(midiCredit810<3)break;
    if(p.known&&now-p.sent<(uint32_t)ccGap810*1000)continue;
    if(MIDI_SERIAL.availableForWrite()<15){++midiCounters810.uartBusy;break;}
    uint8_t c=(i/129)%16;uint16_t o=i%129;MidiScope902 route(1u<<(i/(16*129)),p.track940);
    if(o==128){uint16_t v=p.value+8192;midiRaw810(0xE0|c,v&127,v>>7);midiChannelPitchBendSent[c]=p.value;}
    else {captureSuppress15=p.track940<16;midiRaw810(0xB0|c,o,p.value);captureSuppress15=false;}
    midiCounters810.maxValueAgeUs=max(midiCounters810.maxValueAgeUs,now-p.changed);
    midiPendingBits55[i/32]&=~(1u<<(i%32));p.last=p.value;p.known=true;p.pending=false;p.sent=now;--midiCounters810.pending;++midiCounters810.sent;midiCredit810-=3;midiLastNrpn810=false;if(nrpnWaiting)break;
  }
  if(nrpnPending810){auto&p=*nrpnPending810;MidiScope902 route(1u<<p.output,p.track940);
    uint8_t bytes=p.bits==14?18:15;if(midiCredit810<bytes||MIDI_SERIAL.availableForWrite()<bytes+12)return;
    midiRaw810(0xB0|p.channel,99,p.number>>7);midiRaw810(0xB0|p.channel,98,p.number&127);midiRaw810(0xB0|p.channel,6,p.bits==14?p.value>>7:p.value&127);
    if(p.bits==14)midiRaw810(0xB0|p.channel,38,p.value&127);
    midiRaw810(0xB0|p.channel,101,127);midiRaw810(0xB0|p.channel,100,127);
    midiCounters810.maxWaitUs=max(midiCounters810.maxWaitUs,now-p.first);midiCounters810.sent+=bytes/3;midiCredit810-=bytes;auto*sent=nrpnPending810;nrpnPending810=sent->next;extmem_free(sent);midiLastNrpn810=true;
  }
}
FLASHMEM void midiStats810(){
  Serial.printf("MIDISTAT,GAP_MS=%u,BUDGET_CC_S=%u,PENDING=%u,PEAK=%u,OFFERED=%lu,REPLACED=%lu,DUPLICATES=%lu,SENT=%lu,UART_BUSY=%lu,OVERFLOW=%lu,WAIT_MAX_US=%lu,VALUE_AGE_MAX_US=%lu,LOOP_MAX_US=%lu\n",ccGap810,ccBudget810,midiCounters810.pending,midiCounters810.peak,(unsigned long)midiCounters810.offered,(unsigned long)midiCounters810.replaced,(unsigned long)midiCounters810.duplicates,(unsigned long)midiCounters810.sent,(unsigned long)midiCounters810.uartBusy,(unsigned long)midiCounters810.overflow,(unsigned long)midiCounters810.maxWaitUs,(unsigned long)midiCounters810.maxValueAgeUs,(unsigned long)midiCounters810.maxLoopUs);

}
int midiDraftGap810=2,midiDraftBudget810=500;
FLASHMEM void midiConfig810(){midiDraftGap810=ccGap810;midiDraftBudget810=ccBudget810;patchUi808=6;patchDirty808=0;midiConfigRender810();}
FLASHMEM void midiConfigRender810(){char a[21],b[21];
char gap[6],budget[6];
snprintf(gap,6,"%d",midiDraftGap810);
snprintf(budget,6,"%d",midiDraftBudget810);
clearMenuBlink821();
if(patchDirty808&1)menuBlink821[1]|=((1UL<<strlen(gap))-1)<<4;
if(patchDirty808&2)menuBlink821[1]|=((1UL<<strlen(budget))-1)<<(13+strlen(gap));
snprintf(a,21,"Gap %s ms CC/s %s",gap,budget);
snprintf(b,21,"Wait %luus Q%u",(unsigned long)midiCounters810.maxWaitUs,midiCounters810.pending);
mod806LiveFrame("MIDI CC traffic",a,"Limits CC traffic; keeps latest value. J1 scroll. Gap: same CC interval; CC/s: total budget.","OA gap OB budget");
}
FLASHMEM bool midiConfigRoy810(uint8_t roy,int32_t delta,bool click){
  if(patchUi808!=6)return false;
  if(roy==0){if(delta){midiDraftGap810=clampInt(midiDraftGap810+delta,0,100);patchDirty808|=1;}if(click){ccGap810=midiDraftGap810;patchDirty808&=~1;}}
  if(roy==1){if(delta){midiDraftBudget810=clampInt(midiDraftBudget810+delta*25,50,900);patchDirty808|=2;}if(click){ccBudget810=midiDraftBudget810;patchDirty808&=~2;}}
  if(roy==2&&click){midiClearStats810();midiStats810();}
  midiConfigRender810();return true;
}

struct ModBase937 {ModBase937*next;uint16_t object;int16_t base;uint8_t channel,mask;};
ModBase937*modBases937=nullptr;
uint8_t modRemembered937[16][17]={};
FLASHMEM void modReset937(){while(modBases937){auto*n=modBases937;modBases937=n->next;extmem_free(n);}memset(modRemembered937,0,sizeof(modRemembered937));}
FLASHMEM void modRemember937(uint8_t c,uint16_t o,int base){
 if(c>=16||o>128)return;uint8_t mask=midiMask902(c);
 for(auto*n=modBases937;n;n=n->next)if(n->channel==c&&n->object==o&&n->mask==mask){n->base=base;return;}
 auto*n=(ModBase937*)extmem_malloc(sizeof(ModBase937));if(!n)return;*n={modBases937,o,(int16_t)base,c,mask};modBases937=n;
}
FLASHMEM void modRecall937(int track){
 for(auto*n=modBases937;n;n=n->next){uint8_t mask=n->mask;if(track>=0){auto&c=patch808Context(track,patch808PlayingPattern(track));if(patch808Channel(track,c)!=n->channel)continue;mask&=outMask902(track);}if(!mask)continue;
  int base=n->base;
  MidiScope902 route(mask);midiForce810(n->channel,n->object);midiOffer810(n->channel,n->object,base,false);
 }
}
FLASHMEM void modCancelPort19(uint8_t address,uint16_t o){auto&p=midiPending810[address][o];if(p.pending&&p.modulated){p.pending=false;--midiCounters810.pending;}}
