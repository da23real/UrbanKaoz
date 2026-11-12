#pragma once
static int32_t bridgeMov41=0,bridgeLength41=1536;
static uint64_t bridgeFirstCycleUntil51[16]={};
static int32_t bridgeTailUntil51[16]={};
static TrackRollSettings bridgeRoll44;static RollExtra1000 bridgeExtra44;
static int32_t bridgeOrigin44=0,bridgeSourceTick44=-1000;static uint8_t bridgePattern44=255,bridgeNote44=255;
static_assert(GATE_TICK_MAX<0x4000,"Bridge gate metadata needs two unused bits");
// Preparation runs outside the MIDI tick loop. Notes are captured when queued;
// changing a prepared piece is intentionally not a live Groove operation.
FLASHMEM void clearGrooveBridge(uint8_t track) {
  ++bridgeEpoch49[track];
  for (auto &e : grooveBridgeEvents) if (e.kind && e.track == track) e.kind = 0;
  bridgeHead49[track]=bridgeTail49[track]=0;
  grooveBridgeReady[track] = false;
  grooveBridgeSwitched[track] = false;
  grooveBridgeFirstEnd[track] = 0;
  bridgeTailUntil51[track]=-1;
}
FLASHMEM void resetGrooveBridges() {
  memset(bridgeFirstCycleUntil51,0,sizeof(bridgeFirstCycleUntil51));
  for(auto&v:bridgeTailUntil51)v=-1;
  for(auto&e:bridgeEpoch49)++e;
  for(auto&w:rollWindows44)w={};rollHoldUs44=0;
  memset(bridgeHead49,0,sizeof(bridgeHead49));memset(bridgeTail49,0,sizeof(bridgeTail49));
  memset(grooveBridgeEvents, 0, sizeof(grooveBridgeEvents));
  memset(grooveBridgeReady, 0, sizeof(grooveBridgeReady));
  memset(grooveBridgeSwitched, 0, sizeof(grooveBridgeSwitched));
  memset(grooveBridgeFirstEnd, 0, sizeof(grooveBridgeFirstEnd));
  memset(grooveTrackPhase, 0, sizeof(grooveTrackPhase));
  memset(grooveListOrigin, 0, sizeof(grooveListOrigin));
  for (uint8_t t = 0; t < TRACK_COUNT; ++t) pendingPattern[t] = -1;
}
FLASHMEM uint32_t bridgeMasterTickAt(uint64_t absolute) {
  uint32_t longest = (uint32_t)longestTrackStepCount() * TICKS_PER_STEP;
  uint32_t start = (uint32_t)(loopStartStep - 1) * TICKS_PER_STEP;
  uint32_t end = min((uint32_t)loopEndStep * TICKS_PER_STEP, longest);
  if (end <= start) { start = 0; end = longest; }
  if (absolute < end) return (uint32_t)absolute;
  return start + (uint32_t)((absolute - start) % max((uint32_t)1, end - start));
}
FLASHMEM uint64_t nextBridgeBoundary(uint8_t track, uint64_t earliest) {
  uint32_t length = trackSequenceTicks(track);
  uint32_t master = bridgeMasterTickAt(earliest);
  uint32_t local = (master + length - grooveTrackPhase[track] % length) % length;
  // Either the track wraps, or the master loop restarts. Both are stable
  // boundaries. Advance from the preparation deadline, never from a stale tick.
  uint32_t end = min((uint32_t)loopEndStep * TICKS_PER_STEP,
                    (uint32_t)longestTrackStepCount() * TICKS_PER_STEP);
  uint32_t untilMaster = end > master ? end - master : length;
  return earliest + min(length - local, untilMaster);
}
FLASHMEM bool addBridgeOccurrence(uint8_t track, uint8_t kind, int32_t tick,
    uint32_t length, uint8_t note, uint8_t velocity, uint16_t gate, int16_t bend,uint8_t rollFlags=0) {
  if ((kind == 1 && tick >= 0) || (kind == 2 && tick < (int32_t)length)) return true;
  int32_t offset = kind == 1 ? tick : tick - length;
  if (offset < INT16_MIN || offset > INT16_MAX) return false;
  for (auto &e : grooveBridgeEvents) if (!e.kind) {
    e.block21=blockIndex21();e.route21=blockResolvedValid21?blockResolved21:blocks21[track][e.block21];
    e.offset = (int16_t)offset; e.bend = bend; e.gate = gate|((uint16_t)rollFlags<<14);
    e.track = track; e.note = note; e.velocity = velocity; e.kind = kind;
    if(kind==2)bridgeTailUntil51[track]=max(bridgeTailUntil51[track],offset);
    e.roll44=bridgeRoll44;e.extra44=bridgeExtra44;e.rollOffset44=tick-bridgeOrigin44;
    uint16_t index=(uint16_t)(&e-grooveBridgeEvents),link=index+1;
    bridgeNext49[index]=0;
    if(bridgeTail49[track]){bridgeNext49[bridgeTail49[track]-1]=link;bridgeMin49[track]=min(bridgeMin49[track],e.offset);bridgeMax49[track]=max(bridgeMax49[track],e.offset);}
    else {bridgeHead49[track]=link;bridgeMin49[track]=bridgeMax49[track]=e.offset;}
    bridgeTail49[track]=link;
    e.sourceTick44=bridgeSourceTick44;e.pattern44=bridgePattern44;e.sourceNote44=bridgeNote44;
    return true;
  }
  return false;
}
FLASHMEM bool addBridgeNote(uint8_t track, uint8_t kind, int32_t target,
    uint32_t length, uint8_t note, uint8_t velocity, uint16_t gate,
    int16_t bend, const TrackRollSettings &roll,uint8_t pattern=255,int32_t sourceTick=-1000,uint8_t sourceNote=255,bool fixedRvl=false) {
  auto rx=rollExtra1000(track,pattern,sourceTick,sourceNote);
  bridgeRoll44=roll;bridgeExtra44=rx;bridgeOrigin44=target;bridgeSourceTick44=sourceTick;bridgePattern44=pattern;bridgeNote44=sourceNote;
  if (!addBridgeOccurrence(track, kind, target, length, note, velocity, gate, bend)) return false;
  if (!roll.start || (!roll.level&&!rx.v[1])) return true;
  const int32_t origin38=target;int32_t tick = target - roll.start;target-=rollStop09(roll,rx);
  int32_t spacing = clampInt(roll.speed, 1, TICKS_PER_STEP);
  int32_t acceleration = (int32_t)roll.accel * 256;
  for(uint16_t repeat = 0; repeat < MAX_PREROLL_NOTES && tick < target; ++repeat) {
    uint8_t vel=rollVelocity1000(roll,rx,velocity,repeat);
    if (vel && rollDirection60(track,pattern,sourceTick,sourceNote)!=1 && !addBridgeOccurrence(track, kind, tick, length, rollPitch1000(rx,note,repeat), vel, gate, bend,1|(rvlLock938(track,pattern,sourceTick,sourceNote,fixedRvl)?2:0))) return false;
    if (vel && rollDirection60(track,pattern,sourceTick,sourceNote)!=0 && !addBridgeOccurrence(track, kind, 2*origin38-tick, length, rollPitch1000(rx,note,repeat), vel, gate, bend,1|(rvlLock938(track,pattern,sourceTick,sourceNote,fixedRvl)?2:0))) return false;
    if (spacing >= target - tick) break;
    tick += spacing;
    rollAdvance60(spacing,acceleration,roll.expo,repeat);
  }
  return true;
}
template<class T> bool bridgeRead(File &file, uint32_t offset, T &value) {
  return file.seek(offset) && file.read((uint8_t*)&value, sizeof(value)) == sizeof(value);
}
FLASHMEM bool addStoredBridgeNote(uint8_t track, uint8_t pattern, FineNoteEvent event) {
  if (!event.active) return true;
  if(blockPlaying21<0){for(uint8_t b=0;b<3;++b){BlockScope21 scope(b);if(!addStoredBridgeNote(track,pattern,event))return false;}return true;}
  blockResolved21=blockAtNote21(track,blockIndex21(),pattern,event.step*96+event.logicalMicro,event.note);blockResolvedValid21=true;if(!blockResolved21.port)return true;
  uint8_t sheet=grooveSheet40(track,pattern,blockResolved21.groove,event.step/16);
  int8_t zero[5] = {};
  const int8_t *delta = !grooveBypass23 && sheet && sheet <= GROOVE_COUNT ? grooveOffset[sheet-1][event.step % GROOVE_STEPS] : zero;
  int micro = event.micro;
  int32_t head41=((int32_t)event.step*96+bridgeMov41)%bridgeLength41;if(head41<0)head41+=bridgeLength41;
  int32_t target = blockResolved21.tim + head41 + clampInt(event.logicalMicro + micro, -TICKS_PER_STEP, TICKS_PER_STEP) + grooveTime16(sheet,(int32_t)event.step*96+clampInt(event.logicalMicro+micro,-96,96));
  uint8_t note = (uint8_t)clampInt((event.note) + delta[0], 0, 127);
  uint8_t vel = (uint8_t)clampInt((event.vel) + delta[1], 0, 127);
  uint16_t gate = (uint16_t)clampInt((int64_t)event.gate * (grooveBypass23?100:grooveGate51(sheet,event.step)) / 100, GATE_TICK_MIN, GATE_TICK_MAX);
  for (uint8_t field = 0; field < 6; ++field) if (!(event.pxMask & (1u << (PX_ROLL_SHIFT + field))))
    setRollFieldValue(event.roll, field, rollFieldValue(trackRoll[track], field));
  return addBridgeNote(track, 1, target, 0, note, vel, gate, 0, event.roll,pattern,(int32_t)event.step*96+event.logicalMicro,event.note,(event.pxMask&(1u<<(PX_ROLL_SHIFT+5)))!=0);
}
FLASHMEM bool readIncomingBridge(uint8_t track, uint8_t pattern) {
  auto &image=patternWork906;
  if(patternDirty805[track][pattern]){if(!unpackPattern906(patterns906[track][pattern],image))return false;}
  else {
    char path[40];internalPatternPath(track,pattern,path);
    if(!patternBank[track][pattern].initialized&&(internalPatternSessionFresh||!SD.exists(path)))return true;
    File file=SD.open(path,FILE_READ);if(!file)return false;
    uint32_t magic=0;uint16_t bars=0;uint8_t hasNotes=1;
    bool ok=file.read((uint8_t*)&magic,4)==4&&file.read((uint8_t*)&bars,2)==2;
    if(ok&&magic!=0x554B4231UL)ok=file.read(&hasNotes,1)==1;
    if(ok)ok=readPatternPayload906(file,magic,bars,image);file.close();if(!ok)return false;
    if(!storePattern906(track,pattern,image))return false;
    patternDirty805[track][pattern]=true;auto &meta=patternBank[track][pattern];meta.initialized=true;meta.bars=bars;meta.hasNotes=hasNotes;
  }
  bridgeMov41=image.mov[0];bridgeLength41=image.bars*1536;
  for(uint16_t step=0;step<(bridgeMov41?image.bars*16:min((uint16_t)5,(uint16_t)(image.bars*16)));++step){
    if(!image.active[step])continue;
    FineNoteEvent e{};e.active=true;e.step=step;e.note=image.note[step];e.vel=image.vel[step];e.gate=image.gate[step];e.micro=image.micro[step];e.logicalMicro=image.logical[step];e.roll=image.roll[step];
    e.pxMask=(image.lockPitch[step]?PX_PITCH:0)|(image.lockVel[step]?PX_VEL:0)|(image.lockGate[step]?PX_GATE:0)|(image.lockTime[step]?PX_TIME:0)|((uint16_t)image.rollMask[step]<<PX_ROLL_SHIFT);
    uint8_t emitted[4],count=0;
for(uint8_t voice=0;
voice<=min((uint8_t)3,image.extraCount[step]);
++voice){if(voice&&trackPlayMode[linkOwner12(track,LINK_POLY12)]==TRACK_PLAY_MONO)break;
auto v=e;
if(voice){v.note=image.extraNote[step][voice-1];auto x=image.extra14[step][voice-1];v.vel=x.vel;v.gate=x.gate;v.micro=x.micro;
v.pxMask|=PX_PITCH;
}uint8_t pitch=v.note;
bool duplicate=false;
for(uint8_t j=0;
j<count;
++j)duplicate|=emitted[j]==pitch;
if(duplicate)continue;
emitted[count++]=pitch;
if(!addStoredBridgeNote(track,pattern,v))return false;
}
  }
  for(const auto&e:image.fine)if(e.active&&(bridgeMov41||e.step<5)&&!addStoredBridgeNote(track,pattern,e))return false;
  return true;
}
FLASHMEM bool prepareGrooveBridge(uint8_t track, uint8_t pattern) {
  Scope830 measured(P830_BRIDGE);
  // No dynamic allocation or partial queue: reject the change if storage fails
  // or the shared fixed reserve fills. Existing active notes remain untouched.
  if (grooveBridgeReady[track] && grooveBridgeSwitched[track] &&
      (int64_t)transportTickAbsolute-(int64_t)grooveBridgeAt[track] <= bridgeTailUntil51[track]) {
    pendingPattern[track] = pattern;
    return false;
  }
  clearGrooveBridge(track);
  grooveBridgeSource[track] = activePattern[track];
  pendingPattern[track] = -1;
  uint32_t waiting; noInterrupts(); waiting = pendingClockTicks; interrupts();
  grooveBridgeAt[track] = nextBridgeBoundary(track, transportTickAbsolute + waiting + 5 * TICKS_PER_STEP);
  pattern805CaptureLive(track,patternWork906);
  bool ok=storePattern906(track,activePattern[track],patternWork906);
  if(ok)ok = readIncomingBridge(track, pattern);
  uint32_t length = trackSequenceTicks(track);
  uint16_t steps = trackStepCount(track);
  for(uint8_t b=0;b<3;++b){BlockScope21 scope(b);
  for (uint16_t step = defaultMov41[track][0]?0:steps > 4 ? steps - 4 : 0; ok && step < steps; ++step) {
    if (patternGroove[track][activePattern[track]] == GROOVE_USE_LIST) {
      grooveEventTrack = track;
      int64_t occurrence = (int64_t)grooveBridgeAt[track] - length + (uint32_t)step * TICKS_PER_STEP;
      grooveEventSheet = listSheetAtTick(track, activePattern[track], occurrence > 0 ? occurrence : 0);
    }
    for (uint8_t voice = 0; voice < noteCountAtStep(track, step); ++voice) {
      if (previewSuppressesVoice(track, step, voice)) continue;
      if (voice && voice < baseVoiceCountAtStep(track, step) && trackPlayMode[linkOwner12(track,LINK_POLY12)] == TRACK_PLAY_MONO) continue;
      blockResolved21=blockAtNote21(track,b,activePattern[track],step*96+logicalMicroTick(track,step,voice),noteAtVoice(track,step,voice));blockResolvedValid21=true;if(!blockResolved21.port)continue;grooveEventTrack=track;grooveEventSheet=grooveSheet40(track,activePattern[track],blockResolved21.groove,step/16);
      int32_t head41=((int32_t)step*96+defaultMov41[track][0])%(int32_t)length;if(head41<0)head41+=length;
      ok = addBridgeNote(track, 2, blockResolved21.tim + head41 + audibleMicroTick(track, step, voice)-defaultMov41[track][0], length,
        audibleNoteAtVoice(track, step, voice), audibleVel(track, step, voice), audibleGateTicks(track, step, voice),
        0, audibleRollSettings(track, step, voice),activePattern[track],(int32_t)step*96+logicalMicroTick(track,step,voice),noteAtVoice(track,step,voice),rvlFixedLock938(track,step,voice));
      if (!ok) break;
    }
  }
  }
  grooveEventTrack = 255; grooveEventSheet = -1;
  if (!ok) { clearGrooveBridge(track); DISPLAY_SERIAL.println(F("MSG|PATTERN PREP FULL/SD")); return false; }
  noInterrupts(); waiting = pendingClockTicks; interrupts();
  if (transportTickAbsolute + waiting + 387 >= grooveBridgeAt[track]) {
    clearGrooveBridge(track); DISPLAY_SERIAL.println(F("MSG|PATTERN PREP TOO LATE")); return false;
  }
  grooveBridgeReady[track] = true;
  ++bridgeEpoch49[track];
  pendingPattern[track] = pattern;
  return true;
}
bool bridgeSuppressWrapped(uint8_t track, int32_t eventTick) {
  if (grooveBridgeReady[track] && !grooveBridgeSwitched[track] && eventTick < 0) {
    // Suppress only the final repetition of the outgoing head, not earlier loops.
    uint32_t length = trackSequenceTicks(track);
    int32_t wrapped = eventTick; while (wrapped < 0) wrapped += length;
    return transportTickAbsolute + (length - (uint32_t)wrapped) >= grooveBridgeAt[track];
  }
  return transportTickAbsolute < bridgeFirstCycleUntil51[track] &&
         eventTick >= (int32_t)trackSequenceTicks(track);
}
void playGrooveBridgeEvents(uint8_t track) {
  if (!grooveBridgeReady[track]) return;
  int64_t relative = (int64_t)transportTickAbsolute - (int64_t)grooveBridgeAt[track];
  // An empty bridge still carries its scheduled transition/first-cycle state.
  // Only the event lookup is skipped; expiry is always processed below.
  uint16_t link=(relative>=bridgeMin49[track]&&relative<=bridgeMax49[track])?bridgeHead49[track]:0;
  uint16_t previous=0;
  for (;link;) {
    auto&e=grooveBridgeEvents[link-1];
    uint16_t next=bridgeNext49[link-1];
    if (!e.kind || e.track != track || e.offset != relative ||
        (e.kind == 1 && grooveBridgeSwitched[track]) || (e.kind == 2 && !grooveBridgeSwitched[track])) {
      previous=link;link=next;continue;
    }
    if (!trackMuted[track]&&(!(e.gate&0x4000)||mixerOn938(track,0)||((e.gate&0x8000)&&playbackLocks828))) {
      uint32_t gateUs = (uint32_t)clampInt((int)((float)(e.gate&0x3fff) * tickDurationMs() * 1000.0f), 1000, 20000000);
      BlockScope21 block(e.block21);blockResolved21=e.route21;blockResolvedValid21=true;
      bool was=rollTrigger817;rollTrigger817=(e.gate&0x4000)!=0;
      if(rollTrigger817)rollEvent44(track,e.roll44,e.extra44,e.rollOffset44,e.sourceTick44,e.gate&0x3fff,rollDirection60(track,e.pattern44,e.sourceTick44,e.sourceNote44));
      triggerTrackNoteWithFixedGate(track, e.note, e.velocity, gateUs, e.bend,e.sourceTick44,e.sourceNote44,e.pattern44);rollTrigger817=was;rollHoldUs44=0;
    }
    if (e.kind == 2) {
      // Unlink before releasing this shared slot: another track may reuse it.
      if(previous)bridgeNext49[previous-1]=next;else bridgeHead49[track]=next;
      if(bridgeTail49[track]==link)bridgeTail49[track]=previous;
      e.kind=0;
    } else previous=link;
    link=next;
  }
  if (grooveBridgeSwitched[track] && relative >= bridgeTailUntil51[track]) clearGrooveBridge(track);
}
