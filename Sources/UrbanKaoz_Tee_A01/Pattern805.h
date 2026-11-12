#pragma once
#define PATTERN805_FIELDS(X) \
 X(active, stepActive) X(vel, stepVel) X(note, stepMidiNote) X(gate, stepGateTicks) \
 X(micro, stepMicroTick) X(lockVel, stepLockVel) X(lockPitch, stepLockPitch) \
 X(lockGate, stepLockGate) X(lockTime, stepLockTime) \
 X(rollMask, stepRollLockMask) X(extraCount, stepExtraNoteCount) X(extraNote, stepExtraMidiNote) \
 X(logical, stepLogicalMicro) X(fine, fineNote) X(extra14, stepExtra14) X(mov, defaultMov41)
FLASHMEM void pattern805CaptureLive(uint8_t t,PatternImage805 &image) {
  image.bars=trackBars[t];
  rollExport906(t,image.roll);
  #define CAP805(member,live) memcpy(image.member,live[t],sizeof(image.member));
  PATTERN805_FIELDS(CAP805)
  #undef CAP805
}
FLASHMEM bool pattern805ApplyLive(uint8_t t,const PatternImage805 &image) {
  mixerValid930&=~(1u<<t);
  if(!rollImport906(t,image.roll,image.rollMask))return false;
  bool hasNotes57=false;for(auto v:image.active)hasNotes57|=v;for(auto&e:image.fine)hasNotes57|=e.active;
  trackBars[t]=hasNotes57?image.bars:1;
  #define APP805(member,live) memcpy(live[t],image.member,sizeof(image.member));
  PATTERN805_FIELDS(APP805)
  #undef APP805
  return true;
}
FLASHMEM bool pattern805Read(uint8_t t, uint8_t p, PatternImage805 &image) {
  char path[40]; internalPatternPath(t, p, path);
  File f = SD.open(path, FILE_READ); if (!f) return false;
  uint32_t magic = 0; uint8_t notes = 0;
  bool ok = f.read((uint8_t*)&magic, 4) == 4 && (magic == 0x554B4236UL || magic == 0x554B4235UL) &&
    f.read((uint8_t*)&image.bars, 2) == 2 && image.bars >= 1 && image.bars <= MAX_BARS &&
    f.read(&notes, 1) == 1;
  if(ok)ok=readPatternPayload906(f,magic,image.bars,image);
  f.close(); return ok;
}
FLASHMEM bool pattern805Write(uint8_t t, uint8_t p, const PatternImage805 &image) {
  if(!SD.exists("/URBANKAOZ/PAT")&&!SD.mkdir("/URBANKAOZ/PAT"))return false;
  char path[40], temp[48]; internalPatternPath(t, p, path); snprintf(temp, sizeof(temp), "%s.NEW", path);
  SD.remove(temp); File f = SD.open(temp, FILE_WRITE); if (!f) return false;
  uint32_t magic = 0x554B4236UL; uint8_t notes = 1;
  bool ok = f.write((uint8_t*)&magic, 4) == 4 && f.write((uint8_t*)&image.bars, 2) == 2 && f.write(&notes, 1) == 1;
  #define WRITE805(member, live) if (ok) {if(!strcmp(#member,"rollMask"))ok=f.write((const uint8_t*)image.roll,sizeof(image.roll))==sizeof(image.roll);if(ok)ok = f.write((const uint8_t*)image.member, sizeof(image.member)) == sizeof(image.member);}
  PATTERN805_FIELDS(WRITE805)
  #undef WRITE805
  f.flush(); f.close();
  if (!ok) return false;
  char backup[48]; snprintf(backup, sizeof(backup), "%s.BAK", path);
  SD.remove(backup);
  bool existed = SD.exists(path);
  if (existed && !SD.sdfs.rename(path, backup)) return false;
  if (!SD.sdfs.rename(temp, path)) { if (existed) SD.sdfs.rename(backup, path); return false; }
  SD.remove(backup); return true;
}
FLASHMEM bool pattern805LoadDirty(uint8_t t, uint8_t p) {
  if (!patternDirty805[t][p]) return false;
  auto &image = patternWork906;
  if(!unpackPattern906(patterns906[t][p],image))return false;
  if(!rollImport906(t,image.roll,image.rollMask))return false;
  #define LOAD805(member, live) memcpy(live[t], image.member, sizeof(image.member));
  PATTERN805_FIELDS(LOAD805)
  #undef LOAD805
  bool hasNotes57=false;for(auto v:image.active)hasNotes57|=v;for(auto&e:image.fine)hasNotes57|=e.active;
  trackBars[t]=hasNotes57?image.bars:1;trackLoopStartStep[t]=1;trackLoopEndStep[t]=trackBars[t]*16;
  memset(stepSelected[t], 0, sizeof(stepSelected[t]));
  memset(selectedBaseMask[t], 0, sizeof(selectedBaseMask[t]));
  memset(selectedFineNote[t], 0, sizeof(selectedFineNote[t]));
  for (uint8_t i = 0; i < FINE_EVENTS_PER_TRACK; ++i) fineLastTriggeredCycle[t][i] = UINT32_MAX;
  return true;
}
FLASHMEM void pattern805Saved(uint8_t t, uint8_t p) { patternDirty805[t][p] = false; releasePattern906(t,p); }
FLASHMEM bool pattern805Flush(bool includeActive) {
  for (uint8_t t = 0; t < 16; ++t) for (uint8_t p = 0; p < 16; ++p) {
    if (!patternDirty805[t][p]||(!includeActive&&p==activePattern[t])) continue;
    if (p == activePattern[t]) { if (!saveTrackPattern(t, p)) return false; }
    else if (!unpackPattern906(patterns906[t][p],patternWork906)||!pattern805Write(t,p,patternWork906)) return false;
    pattern805Saved(t,p); serviceNoteOffs();
  }
  return true;
}
FLASHMEM int pattern805Delta(uint8_t ky, int32_t delta) {
  // Same direction as normal NOT; the bank changes the target, not the gesture.
  return delta;
}
FLASHMEM void pattern805RollDelta(TrackRollSettings &r, uint8_t ky, int d) {
  uint8_t field=ky==8?5:ky-10;
  setRollFieldValue(r,field,clampRollFieldValue(field,rollFieldValue(r,field)+d));
}
FLASHMEM void pattern805EditImage(uint8_t t, PatternImage805 &a, uint8_t ky, int d) {
  for (uint16_t s = 0; s < a.bars * 16; ++s) if (a.active[s]) {
    for(uint8_t v=0;v<a.extraCount[s]&&v<3;++v){auto&x=a.extra14[s][v];if(ky==3)x.vel=clampInt((int)x.vel+d,1,127);if(ky==4)x.gate=clampInt((int)x.gate+d,GATE_TICK_MIN,GATE_TICK_MAX);if(ky==5)x.micro=clampInt((int)x.micro+d,-96,96);}
    if (ky == 2) {
      a.note[s] = clampInt((int)(a.note[s]) + d, 0, 127); a.lockPitch[s] = true;
      for (uint8_t v = 0; v < min((uint8_t)3, a.extraCount[s]); ++v) a.extraNote[s][v] = clampInt((int)a.extraNote[s][v] + d, 0, 127);
    } else if (ky == 3) { a.vel[s] = clampInt((int)(a.vel[s]) + d, 1, 127); a.lockVel[s] = true; }
    else if (ky == 4) { a.gate[s] = clampInt((int)(a.gate[s]) + d, GATE_TICK_MIN, GATE_TICK_MAX); a.lockGate[s] = true; }
    else if (ky == 5) { a.micro[s] = clampInt((int)(a.micro[s]) + d, -96, 96); a.lockTime[s] = true; }
    else {
      uint8_t field = ky == 8 ? 5 : ky - 10;
      TrackRollSettings effective = a.roll[s];
      if (!(a.rollMask[s] & (1u << field))) {
        // Set only the edited field, preserving unrelated locks.
        TrackRollSettings base = trackRoll[t];
        if (field == 0) effective.start = base.start;
        else if (field == 1) effective.speed = base.speed;
        else if (field == 2) effective.accel = base.accel;
        else if (field == 3) effective.expo = base.expo;
        else if (field == 4) effective.curve = base.curve;
        else effective.level = base.level;
      }
      pattern805RollDelta(effective, ky, d); a.roll[s] = effective; a.rollMask[s] |= 1u << field;
    }
  }
  for (auto &e : a.fine) if (e.active) {
    if (ky == 2) { e.note = clampInt((int)e.note + d, 0, 127); e.pxMask |= PX_PITCH; }
    else if (ky == 3) { e.vel = clampInt((int)(e.vel) + d, 1, 127); e.pxMask |= PX_VEL; }
    else if (ky == 4) { e.gate = clampInt((int)(e.gate) + d, GATE_TICK_MIN, GATE_TICK_MAX); e.pxMask |= PX_GATE; }
    else if (ky == 5) { e.micro = clampInt((int)(e.micro) + d, -96, 96); e.pxMask |= PX_TIME; }
    else {
      uint8_t field = ky == 8 ? 5 : ky - 10;
      uint16_t mask = 1u << (PX_ROLL_SHIFT + field);
      if (!(e.pxMask & mask)) {
        if (field == 0) e.roll.start = trackRoll[t].start;
        else if (field == 1) e.roll.speed = trackRoll[t].speed;
        else if (field == 2) e.roll.accel = trackRoll[t].accel;
        else if (field == 3) e.roll.expo = trackRoll[t].expo;
        else if (field == 4) e.roll.curve = trackRoll[t].curve;
        else e.roll.level = trackRoll[t].level;
      }
      pattern805RollDelta(e.roll, ky, d); e.pxMask |= mask;
    }
  }
}
FLASHMEM bool pattern805Edit(uint8_t ky, int32_t delta, int8_t onlyTrack) {
 if(indPatternEdit50(ky,delta,onlyTrack))return true;
  if(delta&&ky==1) {
    bool selected=false;for(uint8_t t=0;t<16;++t)if(onlyTrack<0||t==onlyTrack)for(uint8_t p=0;p<16;++p)selected|=patternSelected[t][p];
    if(selected){DISPLAY_SERIAL.println(F("MSG|TRACK PARAM: CLEAR SEL"));return true;}
  }
  if (!delta || !(ky==6 || (ky >= 2 && ky <= 5) || ky == 8 || (ky >= 10 && ky <= 14))) return false;
  if (!selectedPatternCount()) return false;
  if (onlyTrack >= 0) {
    bool selected=false;for(uint8_t p=0;p<16;++p)selected|=patternSelected[onlyTrack][p];
    if(!selected)return false;
  }
  if (external_psram_size < 16) { DISPLAY_SERIAL.println(F("MSG|16MB PSRAM REQUIRED")); return true; }
  // Prepare every result before committing: a read/allocation failure changes nothing.
  auto discard=[](){for(auto&r:staged906)for(auto*&v:r){if(v)extmem_free(v);v=nullptr;}};
  discard();
  int32_t oldMov41[16][16]={},changeMov41[16][16]={};
  int d=ky==6?delta:pattern805Delta(ky,delta);
  for(uint8_t t=0;t<16;++t)for(uint8_t p=0;p<16;++p){
    if((onlyTrack>=0&&t!=onlyTrack)||!patternSelected[t][p]||!songPatternPresent(t,p))continue;
    auto&image=patternWork906;
    bool ok=true;
    if(p==activePattern[t])pattern805CaptureLive(t,image);
    else if(patternDirty805[t][p])ok=unpackPattern906(patterns906[t][p],image);
    else ok=pattern805Read(t,p,image);
    if(!ok){discard();DISPLAY_SERIAL.println(F("MSG|PATTERN READ ERROR"));return true;}
    oldMov41[t][p]=image.mov[0];if(ky==6){image.mov[0]=ZCLAMP57(image.mov[0],delta,-98303,98303);changeMov41[t][p]=image.mov[0]-oldMov41[t][p];}else pattern805EditImage(t,image,ky,d);
    staged906[t][p]=packPattern906(image);
    if(!staged906[t][p]){discard();DISPLAY_SERIAL.println(F("MSG|PATTERN MEMORY FULL"));return true;}
  }
  bool reserved906[16][64]={};
  for(uint8_t t=0;t<16;++t){auto*v=staged906[t][activePattern[t]];if(!v)continue;
    if(!unpackPattern906(v,patternWork906)){discard();return true;}
    for(uint8_t page=0;page<64;++page){bool needed=false;for(uint8_t i=0;i<16;++i)needed|=patternWork906.rollMask[page*16+i]!=0;
      if(needed&&!rollPages906[t][page]){
        if(!rollReserve906(t,page)){
          for(uint8_t rt=0;rt<16;++rt)for(uint8_t rp=0;rp<64;++rp)if(reserved906[rt][rp]){extmem_free(rollPages906[rt][rp]);rollPages906[rt][rp]=nullptr;rollBytes906-=sizeof(RollPage906);}
          discard();DISPLAY_SERIAL.println(F("MSG|ROLL MEMORY FULL"));return true;
        }reserved906[t][page]=true;
      }
    }
  }
  for(uint8_t t=0;t<16;++t)for(uint8_t p=0;p<16;++p){
    auto*v=staged906[t][p];if(!v)continue;
    installPattern906(t,p,v);staged906[t][p]=nullptr;
    patternDirty805[t][p]=true;
    if(p==activePattern[t]){unpackPattern906(v,patternWork906);pattern805ApplyLive(t,patternWork906);}
    // Cached Song occurrences share the modified source. ZL is deliberately separate.
    auto &cache = songCache[t][p];

    if (cache.valid) for (uint32_t i = 0; i < cache.count; ++i) {
      auto &n = songNotes[cache.first + i];
      if(ky==6)n.tick+=changeMov41[t][p];
      else if (ky == 2) n.note = clampInt((int)n.note + d, 0, 127);
      else if (ky == 3) n.velocity = clampInt((int)n.velocity + d, 1, 127);
      else if (ky == 4) n.gate = clampInt((int)n.gate + d, GATE_TICK_MIN, GATE_TICK_MAX);
      else if (ky == 5) n.tick = n.logical + oldMov41[t][p]+clampInt((int)(n.tick-n.logical-oldMov41[t][p])+d,-96,96);
      else if(ky==8||ky>=10){pattern805RollDelta(songRoll805[cache.first+i],ky,d);cache.hasRoll|=songRoll805[cache.first+i].level!=0;}
    }
  }
  neoDisplayDirty = displayValueDirty = true; return true;
}
#undef PATTERN805_FIELDS

