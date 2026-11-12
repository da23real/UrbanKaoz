#pragma once
// External staging keeps edits to non-playing patterns away from live arrays.
// Only dirty images are authoritative; all other patterns retain the B4 disk format.
struct PatternImage805 {
  uint16_t bars;int32_t mov[1];
  bool active[1024]; uint8_t vel[1024], note[1024];
  uint16_t gate[1024]; int16_t micro[1024];
  bool lockVel[1024], lockPitch[1024], lockGate[1024], lockTime[1024];
  TrackRollSettings roll[1024]; uint8_t rollMask[1024], extraCount[1024];
  uint8_t extraNote[1024][3]; int8_t logical[1024]; FineNoteEvent fine[128]; ExtraNote14 extra14[1024][3];
};
#include "PatternStore906.h"
EXTMEM TrackRollSettings songRoll805[SONG_NOTE_CAPACITY];
bool patternDirty805[16][16] = {};
bool pattern805LoadDirty(uint8_t track, uint8_t pattern);
bool pattern805Flush(bool includeActive=true);
void pattern805Saved(uint8_t track, uint8_t pattern);
bool pattern805Edit(uint8_t parameter, int32_t delta, int8_t onlyTrack = -1);
void pattern805CaptureLive(uint8_t track, PatternImage805 &image);
bool pattern805ApplyLive(uint8_t track, const PatternImage805 &image);
void mod805SongRoll(uint8_t track, int64_t logical, int64_t target, uint32_t gate, uint8_t note, TrackRollSettings roll, uint64_t end, int32_t sourceTick=-1000,uint8_t originalNote=255,uint8_t pattern=255,uint8_t originalVelocity=127,bool fixedRvl=false);

