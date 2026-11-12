#pragma once
// Preference belongs to the receiving track, independently of its selected preset.
FLASHMEM void patchRecallLoad54(){uint32_t v=0;EEPROM.get(3930,v);if((v>>16)==0x54CC)patchRecallMask54=v&65535;}
FLASHMEM void patchRecallToggle54(){patchRecallMask54^=1u<<selectedTrack;uint32_t v=0x54CC0000u|patchRecallMask54;EEPROM.put(3930,v);}
FLASHMEM void patchAfterProgram54(uint8_t channel,uint8_t mask){
 for(uint8_t t=0;t<16;++t){if(!(patchRecallMask54&(1u<<t))||(midiTrack940<16&&midiTrack940!=t))continue;
  for(uint8_t b=0;b<3;++b){BlockScope21 block(b);auto br=blockAtNote21(t,b,patch808PlayingPattern(t),-1,255);if(!br.port||br.channel!=channel||!(mask&(1u<<(br.port-1))))continue;
   MidiScope902 scope(1u<<(br.port-1),t);const auto&c=patch808Context(t,patch808PlayingPattern(t));
   for(auto*q=patchNext819(c,nullptr);q;q=patchNext819(c,q)){uint16_t v;if(!patchValue819(c,*q,v))continue;
    if(!q->kind)pidiMidi827(0xB0|channel,q->number,min(127,(int)v));
    else if(q->kind==1){if(remoteBase54(q->bits==14?3:2,channel,q->number,v,1u<<(br.port-1)))continue;pidiMidi827(0xB0|channel,99,q->number>>7);pidiMidi827(0xB0|channel,98,q->number&127);pidiMidi827(0xB0|channel,6,q->bits==14?v>>7:v);if(q->bits==14)pidiMidi827(0xB0|channel,38,v&127);pidiMidi827(0xB0|channel,101,127);pidiMidi827(0xB0|channel,100,127);}
   }
  }
 }
}
