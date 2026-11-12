#pragma once
// Physical B0..7 are the added register; legacy PC identifiers stay unchanged.
inline uint64_t normalize903(uint64_t physical){return (physical>>8)|((physical&255ULL)<<48);}
static bool quickWorking903=false,softTransport903=false,cutNotes903=true,cutDelay903=true;
static bool keepDelayPaused903=false,suppressCancel903=false;
void graphPreview59();
bool quickDepthVisible59(uint16_t,uint8_t);
static uint8_t quickView903=0,quickTrack903=0,quickPattern903=0,quickDepth903=0;
static uint32_t quickItem903=UINT32_MAX,quickPatchPage903=0;
// Small transient UI index stays in internal RAM; preserve the V9.02 PSRAM layout.
static uint32_t quickOrder903[256];
static uint16_t quickCount903=0;
struct TransportScope903 {bool old;TransportScope903(uint8_t index):old(softTransport903){softTransport903=stableTransport904&&(index==1||index==4||index==5);}~TransportScope903(){softTransport903=old;}};
bool quickButton903(uint8_t);bool quickKy903(uint8_t,int32_t);bool quickDisplay903();
void quickFromEp903();void quickPatchEp903();void transportCut903();void toggleTransport903(uint8_t);
void loadTransport903();void checks903();
void quickChoose903(uint8_t);
static uint8_t quickHeld55=255;static uint32_t quickHeldAt55=0;void quickHoldService55();
