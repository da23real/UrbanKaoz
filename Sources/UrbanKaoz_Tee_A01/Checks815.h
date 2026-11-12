#pragma once
FLASHMEM void checks815(){
 if(playing||!trackContext815()||kyBtnDown[0]){Serial.println("CHECK815,BUSY");return;}
 for(uint8_t n=0;n<4;++n)for(uint8_t p=0;p<16;++p)if(neoPadDown[n][p]||trackPads815[n][p]){Serial.println("CHECK815,PAD_HELD");return;}
 uint8_t savedTrack=selectedTrack,savedMode=trackNav815;uint16_t failures=0;
 trackNav815=1;applyParamDeltaFromKy(0,-1);
 if(trackNav815!=0||selectedTrack!=savedTrack)failures|=1;
 applyParamDeltaFromKy(0,1);applyParamDeltaFromKy(0,1);
 if(trackNav815!=2||selectedTrack!=savedTrack)failures|=1;
 // Use the real Neo handler; exercise all 64 pads and both release orders.
 trackNav815=2;
 for(uint8_t n=0;n<4;++n)for(uint8_t p=0;p<16;++p){
  uint8_t t=n*4+p%4;bool old=trackMuted[t];
  trackChord815=kyBtnDown[0]=true;
  onPadEdge(n,p,SEESAW_KEYPAD_EDGE_RISING);
  if(trackMuted[t]==old||selectedTrack!=savedTrack)failures|=2;
  onPadEdge(n,p,SEESAW_KEYPAD_EDGE_RISING); // duplicate must not toggle
  if(trackMuted[t]==old)failures|=4;
  trackChord815=kyBtnDown[0]=false;
  onPadEdge(n,p,SEESAW_KEYPAD_EDGE_FALLING);
  if(trackMuted[t]==old||trackPads815[n][p]||neoPadDown[n][p])failures|=8;
  trackChord815=kyBtnDown[0]=true;
  onPadEdge(n,p,SEESAW_KEYPAD_EDGE_RISING);
  onPadEdge(n,p,SEESAW_KEYPAD_EDGE_FALLING);
  if(trackMuted[t]!=old)failures|=16;
  trackMuted[t]=old;trackChord815=kyBtnDown[0]=false;
 }
 trackNav815=1;trackChord815=kyBtnDown[0]=true;
 onPadEdge(2,12,SEESAW_KEYPAD_EDGE_RISING); // column 9, fourth row
 if(selectedTrack!=8)failures|=32;
 onPadEdge(2,12,SEESAW_KEYPAD_EDGE_FALLING);
 trackNav815=0;onPadEdge(0,0,SEESAW_KEYPAD_EDGE_RISING);
 if(selectedTrack!=8)failures|=64;
 onPadEdge(0,0,SEESAW_KEYPAD_EDGE_FALLING);stopNoteAuditions();
 trackChord815=kyBtnDown[0]=false;trackNav815=savedMode;selectEditingTrack(savedTrack);
 neoDisplayDirty=displayValueDirty=true;
 Serial.printf("CHECK815,%s,MASK=%u,ALL_ROWS,MUTE_TOGGLE,RELEASE_ORDER,SELECT,PREVIEW_NO_SELECT\n",failures?"FAIL":"PASS",failures);
}
