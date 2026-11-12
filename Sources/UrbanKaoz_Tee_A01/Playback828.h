#pragma once
FLASHMEM void playbackSet828(uint8_t index,bool enabled){
  if(index>2)return;
  bool &setting=index==0?playbackLocks828:index==1?playbackEnv828:playbackLfo828;
  if(setting==enabled)return;
  // Release leases while the original P.lock lookup is still enabled.
  if(index==0&&!enabled)lockStop817();
  setting=enabled;
  memset(modLevel810,0,sizeof(modLevel810));
  memset(modDepths810,0,sizeof(modDepths810));
  memset(modSums810,0,sizeof(modSums810));
  memset(modCounts810,0,sizeof(modCounts810));
  memset(modParamSums811,0,sizeof(modParamSums811));
  memset(modParamCounts811,0,sizeof(modParamCounts811));
  // Discard obsolete automation still waiting for transmission, not direct MIDI.
  for(uint8_t c=0;c<16;++c)for(uint16_t o=0;o<129;++o)midiCancel810(c,o);
  mod805Invalidate();
  neoDisplayDirty=displayValueDirty=true;
}
FLASHMEM void playbackRender828(){
  char state[21],a[21],b[21];
  snprintf(state,sizeof(state),"P:%s E:%s L:%s",playbackLocks828?"ON":"OFF",playbackEnv828?"ON":"OFF",playbackLfo828?"ON":"OFF");
  const char* labels[8]={"P.LK","P.LK","ENV","ENV","LFO","LFO","BACK","BACK"};
  makeEightButtonLines(a,b,labels);
  patch808Frame("SEQ > CALCULS",state,a,b);
}
FLASHMEM void playbackOpen828(){patchUi808=13;settingsPageRetryPending=false;playbackRender828();}
FLASHMEM void playbackStats828(){
  Serial.printf("PLAYBACK828,PLOCK=%u,ENV=%u,LFO=%u,ACTIVE_MODS=%u,LEASES=%u,STORED_LOCKS=%u\n",playbackLocks828,playbackEnv828,playbackLfo828,modEnabledCount805,lockLeaseCount817,lockCount817);
}
