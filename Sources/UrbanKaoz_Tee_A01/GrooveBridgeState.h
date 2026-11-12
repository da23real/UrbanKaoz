#pragma once
// Fixed storage: only emitted occurrences crossing a pattern boundary.
struct GrooveBridgeEvent {
  Block21 route21;uint8_t block21;
  int16_t offset, bend;
  uint16_t gate; // low14: gate ticks; bit14: repetition; bit15: explicit RVL lock
  uint8_t track, note, velocity, kind; // kind 0=free, 1=incoming, 2=outgoing
  TrackRollSettings roll44;RollExtra1000 extra44;
  int32_t rollOffset44,sourceTick44;uint8_t pattern44,sourceNote44;
};
static const uint16_t GROOVE_BRIDGE_CAPACITY = 768;
EXTMEM GrooveBridgeEvent grooveBridgeEvents[GROOVE_BRIDGE_CAPACITY];
// One-based links: zero is empty, including before the first reset.
// Internal RAM index avoids scanning 768 large external-RAM events per track/tick.
static uint16_t bridgeHead49[TRACK_COUNT]={},bridgeTail49[TRACK_COUNT]={};
DMAMEM static uint16_t bridgeNext49[GROOVE_BRIDGE_CAPACITY]; // Set before each link becomes reachable.
static int16_t bridgeMin49[TRACK_COUNT]={},bridgeMax49[TRACK_COUNT]={};
static uint32_t bridgeEpoch49[TRACK_COUNT]={};
uint64_t grooveBridgeAt[TRACK_COUNT] = {};
uint64_t grooveBridgeFirstEnd[TRACK_COUNT] = {};
uint32_t grooveTrackPhase[TRACK_COUNT] = {};
bool grooveBridgeReady[TRACK_COUNT] = {};
bool grooveBridgeSwitched[TRACK_COUNT] = {};
void clearGrooveBridge(uint8_t track);
void resetGrooveBridges();
uint8_t grooveBridgeSource[TRACK_COUNT] = {};
