#pragma once
// Synthetic RAM fixtures only, available after explicit B29PREP.
FLASHMEM void benchEffects30(unsigned effect){
 lockReset817();for(auto&m:modUnits805)m={};
 if(effect==1||effect==2){
  for(uint16_t i=0;i<2048;++i){auto&l=noteLocks817[lockCount817++];
   // 64 distinct positions and 32 note identities on track16; the first
   // voice matches that track and exercises actual lease creation.
   l={(int32_t)((i/32)*96),effect==1?64:0,15,(uint16_t)(effect==1?74:2),15,0,(uint8_t)(63+i%32),(uint8_t)(effect==1?LOCK_CC817:LOCK_BLOCK21)};
  }
  playbackLocks828=true;
 }
 if(effect==3){auto&m=modUnits805[0];m.enabled=1;m.rate=384;m.shape=1;modKinds931[0]=1;modExtra810[0].mode=2;m.outputs[0]={MOD_CC,0,74,1,20,1,0,64};}
 mod805Invalidate();
}
