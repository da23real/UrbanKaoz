#pragma once
// UI state only; no traversal of selection or history in the clock interrupt.
bool columnSelectionMode = false;
bool trackNavigationPreview = false;
uint8_t selectionAllCycle = 0, selectionAllContext = 255;
uint8_t lastPatternTrack = 0, lastPatternIndex = 0;
bool lastPatternValid = false;
bool rectangleArmed = false;
uint8_t rectangleNeo = 0, rectanglePad = 0, rectangleContext = 255;
uint32_t rectangleBegin = 0, rectangleEnd = 0;
uint8_t rectanglePitch = 0, rectangleTrack = 0;
struct ColumnScope805 { uint32_t begin, end; uint8_t track, context, stage; };
ColumnScope805 columnScopes805[32] = {};
uint16_t grooveListSelection805 = 0;
uint8_t grooveListFocus805 = 0;
int8_t grooveListAnchor805 = -1;
bool selection805GrooveClick(uint8_t ky);
bool selection805Pad(uint8_t neo, uint8_t pad, uint8_t edge);
void selection805All();
void selection805ToggleColumn();
void selection805ToggleShift();
void selection805Reset();
void selection805SongState();
