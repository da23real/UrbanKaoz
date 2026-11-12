#pragma once
// Pure HADSR model. Retrigger starts a new envelope; note-off starts Release
// from the actual current level, including during Hold/Attack/Decay.
inline float envelopeCurve805(float x, uint8_t curve) {
  x = x < 0 ? 0 : x > 1 ? 1 : x;
  return curve == 1 ? x*x : curve == 2 ? 1-(1-x)*(1-x) : x;
}
inline float envelopeHeld805(float ms, uint16_t hold, uint16_t attack,
                             uint16_t decay, uint8_t sustain, uint8_t curve) {
  if (ms < hold) return 0;
  ms -= hold;
  if (attack && ms < attack) return envelopeCurve805(ms/attack, curve);
  ms -= attack;
  float level = sustain / 127.0f;
  if (decay && ms < decay) return 1-(1-level)*envelopeCurve805(ms/decay, curve);
  return level;
}
inline float envelope805(float ageMs, float gateMs, uint16_t hold, uint16_t attack,
                         uint16_t decay, uint8_t sustain, uint16_t release, uint8_t curve) {
  if (ageMs < 0) return 0;
  if (ageMs <= gateMs) return envelopeHeld805(ageMs, hold, attack, decay, sustain, curve);
  if (!release || ageMs >= gateMs + release) return 0;
  return envelopeHeld805(gateMs, hold, attack, decay, sustain, curve) *
    (1-envelopeCurve805((ageMs-gateMs)/release, curve));
}
