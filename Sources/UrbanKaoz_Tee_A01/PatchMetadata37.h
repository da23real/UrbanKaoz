#pragma once
// Bounded metadata transport, independent of musical MIDI packets and queues.
FLASHMEM void noteLabel37(char*out,size_t size,uint8_t track,uint8_t note){
 if(track<16){auto&c=patch808Context(track,activePattern[track]);for(uint8_t slot=0;slot<16;++slot){auto*d=patch808Find(patchDb808,patchId819(c,slot));for(auto*q=d?d->params:nullptr;q;q=q->next)if(q->kind==2&&(int)q->number+d->noteOffset44==note){snprintf(out,size,"%s",q->shortName[0]?q->shortName:q->name);return;}}}
 const char*en[]={"C","C#","D","D#","E","F","F#","G","G#","A","A#","B"};const char*fr[]={"do","do#","re","re#","mi","fa","fa#","sol","sol#","la","la#","si"};
 if(!noteScale37)snprintf(out,size,"%u",note);else snprintf(out,size,"%s%d",noteScale37==1?en[note%12]:fr[note%12],note/12-1);
}
static uint8_t metadataTrack37=16,metadataSlot37=0;static uint32_t metadataParam37=0,metadataCount37=0,metadataId37=0;static bool metadataBegin37=false;
FLASHMEM void patchMetadataStart37(){metadataTrack37=0;metadataSlot37=0;metadataParam37=0;metadataCount37=0;metadataBegin37=true;++metadataId37;}
FLASHMEM void patchMetadataService37(){
 if(metadataTrack37>=16||!synkozEnabled933||!Serial||Serial.availableForWrite()<220)return;
 if(metadataBegin37){Serial.printf("SKPATCH37,BEGIN,%lu\n",(unsigned long)metadataId37);metadataBegin37=false;return;}
 auto&c=patch808Context(metadataTrack37,activePattern[metadataTrack37]);auto*d=patch808Find(patchDb808,patchId819(c,metadataSlot37));auto*q=d?d->params:nullptr;for(uint32_t n=0;q&&n<metadataParam37;++n)q=q->next;
 if(q){uint16_t value=0;bool known=patchValue819(c,*q,value);Serial.printf("SKPATCH37,P,%lu,%u,%u,%u,%u,%u,%d,%s,%s,%s\n",(unsigned long)metadataId37,metadataTrack37,midiChannelForTrack(metadataTrack37),q->kind,q->number,q->bits,known?(int)value:-1,d->name,q->name,q->shortName);++metadataParam37;++metadataCount37;return;}
 metadataParam37=0;if(++metadataSlot37==16){metadataSlot37=0;if(++metadataTrack37==16)Serial.printf("SKPATCH37,END,%lu,%lu\n",(unsigned long)metadataId37,(unsigned long)metadataCount37);}
}
