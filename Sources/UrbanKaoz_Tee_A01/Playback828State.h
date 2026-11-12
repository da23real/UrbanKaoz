#pragma once
// Diagnostic bypasses: session-wide, reset ON at boot, never alter project data.
bool playbackLocks828=true, playbackEnv828=true, playbackLfo828=true;
uint8_t modKind931(uint16_t);
bool mixerMod938(uint16_t);
inline bool playbackMod828(uint16_t key){return (modKind931(key)?playbackLfo828:playbackEnv828)&&mixerMod938(key);}
void playbackSet828(uint8_t index,bool enabled);
void playbackRender828();
void playbackOpen828();
void playbackStats828();
