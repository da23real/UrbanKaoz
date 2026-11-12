#pragma once
extern "C" uint8_t external_psram_size;
// Full workspace is external: no reduction of the existing sequencer arrays.
// 16 MB expansion supported; this workspace requires at least 8 MB detected.
static const uint32_t ZL_CAPACITY=32768;
struct ZLEvent { uint64_t tick; uint32_t id,gate; uint8_t track,kind,a,b; };
EXTMEM ZLEvent zlEvents[ZL_CAPACITY],zlStaging[ZL_CAPACITY];
uint32_t zlCount=0,zlNextId=1;
bool songEngineActive=false,songRecord=false,songWorkspaceReady=false;
uint8_t songView=0;
int32_t zlSendCursor=-1;
uint32_t songRevision=0,songSnapshotRevision=0;
void songExtraState();void songEmitZL(uint32_t index);void songLeaveSurface();
bool songExtraCommand(char*line);void songPlayZL();bool songSaveWorkspace();
bool songLoadWorkspace();void songWorkspaceInit();
