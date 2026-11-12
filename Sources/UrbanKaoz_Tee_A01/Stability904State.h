#pragma once
static bool stableTrack904=false,stableQuick904=false,stableTransport904=false;
static uint8_t seqPage904=0,stabilityPage904=0;
void loadStability904();
void openStability904(uint8_t page);
void renderStability904();
void buttonStability904(uint8_t button);
void backStability904();
void statusStability904();
void checks904();
uint64_t readButtons904();
