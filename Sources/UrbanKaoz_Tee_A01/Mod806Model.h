#pragma once
// 64 shared ENV/LFO identities. Ownership is by track, independent of patterns.
// Legacy pattern words are retained only for the compatible streaming codec.
static const uint16_t MOD806_CAPACITY = 64;
static const uint8_t MOD806_ROUTES = 8;
static const uint8_t MOD_MIDI_BEND806 = 13;
struct ModMeta806 {
  uint16_t tracks;
  uint16_t midiInputs;
};
struct ModMean806 {
  float sum = 0;
  uint16_t count = 0;
  void add(float value) { sum += value; ++count; }
  float value() const { return count ? sum / count : 0; }
};
inline bool mod806Associated(const ModMeta806 &meta, uint8_t track, uint8_t pattern) {
  return track < 16 && (meta.tracks&(1u<<track))!=0;
}
inline void mod806Associate(ModMeta806 &meta, uint8_t track, uint8_t pattern, bool on) {
  if(track<16){if(on)meta.tracks|=1u<<track;else meta.tracks&=~(1u<<track);} // Track association; pattern argument is legacy.
}
// Exact collision key: MIDI channel/object/CC, or track/parameter.
inline bool mod806SameTarget(const ModRoute805 &a, const ModRoute805 &b) {
  return a.enabled && b.enabled && a.kind == b.kind && a.track == b.track && a.port==b.port && (!a.port||a.channel==b.channel) &&
    ((a.kind != MOD_CC && a.kind != MOD_VIRTUAL) || a.number == b.number);
}
inline bool mod806MidiTarget(const ModRoute805 &r) {
  return r.kind == MOD_CC || r.kind == MOD_MIDI_BEND806;
}
