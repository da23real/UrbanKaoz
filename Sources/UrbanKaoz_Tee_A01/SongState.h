#pragma once
#include <USBHost_t36.h>
#include "SongModel.h"
#include "SongWorkspace.h"
bool songMode = false;
bool songStateDirty931 = true;
bool selectionIndicatorsPending931 = true;
bool songDeleteArmed = false;
bool songExportArmed=false;
int16_t songExportClip=-1;
uint8_t songExportTrack=0;
uint64_t songExportBegin=0,songExportEnd=0;
uint32_t songExportRevision=0;
bool songExportTo(uint8_t track,uint8_t pattern);
bool songLoopEnabled = false;
uint32_t songStartBar = 1, songEndBar = 4;
uint64_t songTick = 0;
SongClip songClips[SONG_CLIP_CAPACITY] = {};
struct SongNote { int32_t tick, logical; uint16_t gate; uint8_t note, velocity; };
static const uint32_t SONG_NOTE_CAPACITY = 65536;
EXTMEM SongNote songNotes[SONG_NOTE_CAPACITY];
EXTMEM int32_t songLockTick817[SONG_NOTE_CAPACITY];
struct SongPatternCache { uint32_t first, count; bool valid; bool hasRoll; };
SongPatternCache songCache[16][16] = {};
uint32_t songNoteCount = 0;
USBHost songUsb;
MIDIDevice_BigBuffer songMidi(songUsb);
bool songUsbConnected = false;
int16_t songCatalogCursor = -1, songClipCursor = -1;
uint32_t songLastSendMs = 0, songLastStateMs = 0;
uint8_t songSelectedTrack = 0, songSelectedPattern = 0;
int8_t songHeldNeo = -1, songHeldPad = -1;
uint32_t songHeldSince = 0;
bool songHeldSent = false;
void songInit();
void songService();
void songClockTick();
void songSetMode(bool enabled);
bool songTransport(uint8_t index, bool pressed);
bool songPad(uint8_t neo, uint8_t pad, uint8_t edge);
void songSelect(uint8_t track, uint8_t pattern);
void songState();
void songSend(const char *format,...);
bool nameQueuedValid813(const char*line);
void songCommand(char *line);
void songRoy(uint8_t encoder, int delta);

DMAMEM char songOutbox[64][150];
uint8_t songOutRead=0,songOutWrite=0;
bool songOutOverflow=false;

FLASHMEM void songSnapshot();
