#pragma once
// 0 = disabled, 1..16 = USB Host channel. Local to each track (not linked).
uint8_t midiInChannel17[16]={},midiInStage17[16]={},midiInMenu17=0;
void midiInOn17(uint8_t,uint8_t,uint8_t);
void midiInOff17(uint8_t,uint8_t,uint8_t);
void midiInRelease17(uint8_t track=255,uint8_t channel=255);
void midiInService17();
void midiInRender17();void midiInButton17(uint8_t);void midiInRoy17(uint8_t,int32_t);
bool midiInWrite17(File&);bool midiInRead17(File&,bool);
void midiInStatus17();
void midiInInit17();
