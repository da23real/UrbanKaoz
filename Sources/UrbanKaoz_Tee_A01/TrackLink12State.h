#pragma once
enum LinkGroup12 : uint8_t {LINK_PATCH12,LINK_ENV12,LINK_LFO12,LINK_DLY12,LINK_VIEW12,LINK_POLY12,LINK_MOUT12,LINK_KB13};
uint8_t linkSource12[16]={},linkMask12[16]={};
uint8_t linkOwner12(uint8_t t,uint8_t group){return t<16&&(linkMask12[t]&(1u<<group))?linkSource12[t]:t;}
bool linkMenu12=false;uint8_t linkDraft12=0;
KbTrackR10& kbSettings13(uint8_t t){uint8_t owner=linkOwner12(t,LINK_KB13);return kbTracksR10[owner<16?owner:0];}
int32_t kbCenter12=36;
int32_t& kbCenterFor12(uint8_t t,uint8_t z){return !kbSettings13(t).split?kbCenter12:kbSettings13(t).zone[z].center;}
uint16_t delayCut12=0;
char projectName12[17]={};bool nameProposed12=false;
void linkRender12();bool linkToggle12(uint8_t);void linkReset12();
bool modAssociated12(uint16_t,uint8_t);bool modSourceAudible12(uint8_t);
void trackCopy12(uint8_t);bool trackPaste12(uint8_t);bool trackStorageOpen12(bool);
void trackClear12();void trackSave12();void trackLoad12();
void namePropose12(const char*);void nameLetter12(char);
