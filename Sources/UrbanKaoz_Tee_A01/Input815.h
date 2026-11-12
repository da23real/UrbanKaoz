#pragma once
#include "Mixer938State.h"
// The Pico emits complete signed detents. Musical direction belongs to the
// edited parameter, never to a physical KY number or the old global DIR flag.
static int normalKyDelta819(int raw){return raw < -16 ? -16 : raw > 16 ? 16 : raw;}
static int rollDelta819(uint8_t field,int raw){return raw;}
static uint8_t trackNav815=1; // pre, sel, mut; session-only, default sel
static bool trackChord815=false;
static bool trackPads815[4][16]={};
static const char*const trackNames815[3]={"pre","sel","mut"};
bool trackContext815();
uint16_t inputContext815();
enum InputSource815:uint8_t{I815_HC,I815_KY,I815_ROY,I815_J,I815_NEO};
enum InputStage815:uint8_t{I815_RAW,I815_STABLE,I815_ENTER,I815_EXIT};
struct InputEvent815{uint32_t at,us;uint16_t context;uint8_t source,id,pressed,stage;};
static InputEvent815 inputEvents815[128];
static uint16_t inputWrite815=0,inputUsed815=0;
static uint32_t inputOverwritten815=0,inputLastScan815=0,inputMaxScan815=0;
static void inputReset815(){inputWrite815=inputUsed815=0;inputOverwritten815=inputLastScan815=inputMaxScan815=0;}
static void inputLog815(uint8_t source,uint8_t id,bool pressed,uint8_t stage,uint32_t us=0){
 if(!diagActive814)return;
 inputEvents815[inputWrite815]={millis(),us,inputContext815(),source,id,(uint8_t)pressed,stage};
 inputWrite815=(inputWrite815+1)%128;if(inputUsed815<128)++inputUsed815;else ++inputOverwritten815;
}
void presentationTouch43();
struct InputScope815{
 bool enabled,pressed;uint8_t source,id;uint32_t start;
 InputScope815(uint8_t s,uint8_t i,bool p,bool capture=true):enabled(diagActive814&&capture),pressed(p),source(s),id(i){if(p)presentationTouch43();if(enabled){start=micros();inputLog815(s,i,p,I815_ENTER);}}
 ~InputScope815(){if(enabled)inputLog815(source,id,pressed,I815_EXIT,micros()-start);}
};

uint8_t trackVolume818[16]={90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90};
uint16_t trackSolo818=0;
bool trackSilent818(uint8_t track);
void trackAction818(uint8_t row,uint8_t track);
void trackVolumeTurn818(int32_t delta);
void trackReset818();
void patchSeed818();
void checks818();
void patchCheck818();
