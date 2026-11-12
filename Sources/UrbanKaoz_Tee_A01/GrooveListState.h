#pragma once
static const uint8_t GROOVE_LIST_SIZE = 14;
static const uint8_t GROOVE_USE_LIST = GROOVE_COUNT + 1;
struct GrooveListEntry { uint8_t sheet; uint8_t repeats; };
// 7168 bytes, no duplicate note/pattern bank.
DMAMEM GrooveListEntry grooveLists[TRACK_COUNT][PATTERN_COUNT][GROOVE_LIST_SIZE];
uint16_t grooveInitialized26=0;
uint32_t grooveNoticeAt26=0,grooveNoticeDuration26=0;
void grooveEditAssign26();
void grooveFirstNote26(uint8_t,uint16_t);
uint64_t grooveListOrigin[TRACK_COUNT] = {};
uint16_t grooveListRepeatEditing = 0;
int16_t grooveEventSheet = -1;
uint8_t grooveEventTrack = 255;
uint32_t grooveFeedbackUntil = 0;
int8_t grooveFeedbackField = -1;
int16_t grooveFeedbackDelta = 0;
bool grooveTestCapture = false;
uint16_t grooveTestNotes = 0;
uint8_t grooveTestPitch = 0;
uint32_t grooveTestGateUs = 0;
void processGrooveListTrack(uint8_t track, uint32_t local);

uint8_t listSheetAtBar(const GrooveListEntry *list, uint64_t bar) {
  uint16_t total = 0;
  for (uint8_t i = 0; i < GROOVE_LIST_SIZE && list[i].sheet; ++i)
    total += max((uint8_t)1, list[i].repeats);
  if (!total) return 0;
  uint16_t position = bar % total;
  for (uint8_t i = 0; i < GROOVE_LIST_SIZE && list[i].sheet; ++i) {
    uint8_t repeats = max((uint8_t)1, list[i].repeats);
    if (position < repeats) return list[i].sheet;
    position -= repeats;
  }
  return 0;
}

uint8_t listSheetAtTick(uint8_t track, uint8_t pattern, uint64_t tick) {
  uint64_t origin = grooveListOrigin[track];
  uint64_t bar = tick >= origin ? (tick - origin) / (STEPS_PER_BAR * TICKS_PER_STEP) : 0;
  return listSheetAtBar(grooveLists[track][pattern], bar);
}
