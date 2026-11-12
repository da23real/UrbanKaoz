#pragma once
// The active roll working set contains only pages with explicit overrides.
// Lookup is O(1), never allocates, and falls back to track defaults in the caller.
struct RollPage906 {TrackRollSettings value[16];};
static RollPage906* rollPages906[16][64]={};
static uint32_t rollBytes906=0;
static bool memoryFault906=false;
static const TrackRollSettings emptyRoll906={};
static const size_t ROLL_ROW_BYTES906=1024*sizeof(TrackRollSettings);
EXTMEM TrackRollSettings rollIo906[1024];
static const TrackRollSettings& rollRead906(uint8_t t,uint16_t s){
 auto*p=rollPages906[t][s/16];return p?p->value[s%16]:emptyRoll906;
}
static bool rollReserve906(uint8_t t,uint16_t page){
 auto*&p=rollPages906[t][page];if(p)return true;
 p=(RollPage906*)extmem_malloc(sizeof(RollPage906));
 if(!p){memoryFault906=true;return false;}
 memset(p,0,sizeof(*p));rollBytes906+=sizeof(*p);return true;
}
static bool rollSet906(uint8_t t,uint16_t s,const TrackRollSettings&value,uint8_t mask){
 if(mask&&!rollReserve906(t,s/16))return false;
 auto*&p=rollPages906[t][s/16];
 if(p){TrackRollSettings local={};
  if(mask&1)local.start=value.start;if(mask&2)local.speed=value.speed;
  if(mask&4)local.accel=value.accel;if(mask&8)local.expo=value.expo;
  if(mask&16)local.curve=value.curve;if(mask&32)local.level=value.level;
  p->value[s%16]=local;
 }
 stepRollLockMask[t][s]=mask;
 if(p&&!mask){bool used=false;for(uint8_t i=0;i<16;++i)used|=stepRollLockMask[t][s/16*16+i]!=0;
  if(!used){extmem_free(p);p=nullptr;rollBytes906-=sizeof(RollPage906);}}
 return true;
}
static void rollExport906(uint8_t t,TrackRollSettings*dst){for(uint16_t s=0;s<1024;++s)dst[s]=rollRead906(t,s);}
static bool rollImport906(uint8_t t,const TrackRollSettings*src,const uint8_t*mask){
 // Reserve the required pages before altering any existing value/mask.
 bool created[64]={};
 for(uint8_t page=0;page<64;++page){bool needed=false;for(uint8_t i=0;i<16;++i)needed|=mask[page*16+i]!=0;
  if(needed&&!rollPages906[t][page]){if(!rollReserve906(t,page)){for(uint8_t j=0;j<64;++j)if(created[j]){extmem_free(rollPages906[t][j]);rollPages906[t][j]=nullptr;rollBytes906-=sizeof(RollPage906);}return false;}created[page]=true;}}
 memcpy(stepRollLockMask[t],mask,1024);
 for(uint16_t s=0;s<1024;++s)rollSet906(t,s,src[s],mask[s]);return true;
}
static size_t rollWriteFile906(File&f,uint8_t t){rollExport906(t,rollIo906);return f.write((uint8_t*)rollIo906,ROLL_ROW_BYTES906);}
static bool rollReadFile906(File&f,uint8_t t){
 uint8_t mask[1024];if(f.read((uint8_t*)rollIo906,ROLL_ROW_BYTES906)!=ROLL_ROW_BYTES906||f.read(mask,1024)!=1024)return false;
 return rollImport906(t,rollIo906,mask);
}
