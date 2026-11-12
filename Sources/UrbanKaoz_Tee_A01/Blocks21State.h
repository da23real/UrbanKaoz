#pragma once
struct Block21 {int16_t tim=0;uint8_t port=0,channel=0,groove=0;int8_t transpose=0;};
struct BlockListEntry21 {uint8_t sheet,repeats;};
constexpr uint8_t BLOCK_LISTS21=9;
Block21 blocks21[16][3],blockResolved21;
EXTMEM BlockListEntry21 trackLists44[16][BLOCK_LISTS21][14]{};
uint8_t trackListCounts44[16]={};
using ListBank44=BlockListEntry21[BLOCK_LISTS21][14];
ListBank44& listBank44(uint8_t t){return trackLists44[t];}
uint8_t& listCount44(uint8_t t){return trackListCounts44[t];}
uint8_t blockListEdit21=0,patchBlock21=0,quickPatchBlock21=0;
int8_t blockPlaying21=-1;bool blockResolvedValid21=false;
uint8_t blockIndex21(){return blockPlaying21>=0?(uint8_t)blockPlaying21:0;}
uint8_t patchBlockIndex21(){return blockPlaying21>=0?(uint8_t)blockPlaying21:patchBlock21;}
uint8_t patchContextIndex21(uint8_t p){return patchBlockIndex21()*17+p;}
struct BlockScope21 {int8_t old;bool valid;Block21 resolved;BlockScope21(uint8_t b):old(blockPlaying21),valid(blockResolvedValid21),resolved(blockResolved21){blockPlaying21=b;blockResolvedValid21=false;}~BlockScope21(){blockPlaying21=old;blockResolvedValid21=valid;blockResolved21=resolved;}};
void blockReset21();void blockDisplay21();bool blockKy21(uint8_t,int32_t);bool blockClick21(uint8_t,bool);
uint8_t blockStageCount40(uint8_t t);
void listMigrate44();
void blockProcessTrack21(uint8_t,uint32_t);void blockListDisplay21();bool blockListEditValue21(uint8_t,int32_t);
bool blockWrite21(File&);bool blockRead21(File&,bool);void blockInstall21();
Block21 blockAtNote21(uint8_t,uint8_t,uint8_t,int32_t,uint8_t);
uint8_t blockMask21(uint8_t);uint8_t blockChannel21(uint8_t);

void blockPatchChoose21();

bool blockImportTrack21(uint8_t,const Block21*,const BlockListEntry21 (*)[14],uint8_t,bool=true);
bool blockOptimize29=true;
bool blockCollectDue29=false;
int64_t blockNextDue29=0;
inline void blockObserveDue29(int64_t at,int64_t now){if(blockCollectDue29&&at>now&&at<blockNextDue29)blockNextDue29=at;}
bool blockTargeted30=true;

void lockEventInvalidate30();
int lockEventFirst30(uint8_t,uint8_t,int32_t,uint8_t);
int lockEventNext30(int);
bool lockEventPresent30(uint8_t,uint8_t,int32_t,uint8_t);
bool lockFreeTrack30(uint8_t);

uint16_t blockNoteFields30(uint8_t t);

int blockPitch34(uint8_t,uint8_t);void blockAudition34(uint8_t,uint8_t);
int blockNoteDisplay21(uint8_t);
struct NoteLock817;
bool blockMusicalField35(const NoteLock817&);
bool noteHasPlx35(uint8_t,uint16_t,uint8_t);
