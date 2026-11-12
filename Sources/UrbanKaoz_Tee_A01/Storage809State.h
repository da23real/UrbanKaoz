#pragma once
enum ItemKind809 : uint8_t { ITEM_NONE809, ITEM_PATCH809, ITEM_PRESET809, ITEM_ENV809, ITEM_LFO809, ITEM_FX809, ITEM_TRACK12, ITEM_KB18, ITEM_ENVS18, ITEM_LFOS18, ITEM_DELAYS18, ITEM_GROOVE32 };
bool grooveReturn32=false;
bool grooveOpen32(bool);void grooveSave32();void grooveLoad32();
uint8_t itemKind809=0,itemReturnMod809=0,itemReturnPatch809=0,itemTrack809=0,itemPattern809=0;
bool itemChoosing809=false,itemSave809=false,itemPatternMode809=false;
uint8_t itemChoice809=0;
uint16_t itemMod809=UINT16_MAX;
uint32_t itemPatch809=0;
bool itemOpen809(bool save);
bool itemRoy809(uint8_t roy,int32_t delta,bool click);
bool itemScreen809(uint8_t index);
bool itemBack809();
void itemClose809();
void itemSaveFile809();
void itemLoadFile809();
const char* itemLabel809();
bool itemFileMatches809(const char* name);
void itemSeed809();
void itemChecks809();
void nameSync809(bool force=false);
bool nameCommand809(char* line);
uint32_t nameSession809=0;

uint8_t itemReturnKb18=0,clipKinds18[4]={};
void scopeSave18();void scopeLoad18();
bool scopeOpen18(bool);const char* storageDescription18();
void clipMenuEnter18();
