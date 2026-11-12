#pragma once
#include <stdint.h>

// Positions in 1/16 steps, source interval [offset, offset + length).
// Editing the source interval NEVER changes the song position.
static const uint16_t SONG_CLIP_CAPACITY = 128;
static const uint32_t SONG_MAX_STEP = 1000000000UL;
struct SongClip {
  uint32_t position;
  uint16_t offset, length;
  uint8_t track, pattern;
  bool used;
};
inline bool songClipValid(const SongClip &c, uint32_t sourceLength) {
  return c.track < 16 && c.pattern < 16 && c.length > 0 &&
    (uint32_t)c.offset + c.length <= sourceLength &&
    c.position <= SONG_MAX_STEP && c.length <= SONG_MAX_STEP - c.position;
}
inline bool songClipsOverlap(const SongClip &a, const SongClip &b) {
  return a.used && b.used && a.track == b.track &&
    (uint64_t)a.position < (uint64_t)b.position + b.length &&
    (uint64_t)b.position < (uint64_t)a.position + a.length;
}
inline uint64_t songLoopPosition(uint64_t tick, bool enabled, uint32_t first,
                                 uint32_t last, uint32_t barTicks) {
  const uint64_t begin = (uint64_t)(first - 1) * barTicks;
  const uint64_t end = (uint64_t)last * barTicks; // inclusive numbered last bar
  return enabled && first >= 1 && last >= first && tick >= end
    ? begin + (tick - end) % (end - begin) : tick;
}
