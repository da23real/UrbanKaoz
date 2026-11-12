#pragma once
uint16_t patchRecallMask54=65535;void patchRecallLoad54();void patchRecallToggle54();void patchAfterProgram54(uint8_t,uint8_t);
bool patchNoteOffsetEdit44(uint8_t,int32_t);
// Definitions contain no musical values. Contexts own channel and sparse values.
struct PatchParam808 { PatchParam808* next; uint32_t id; uint16_t number; uint8_t kind,bits; char name[7],shortName[4]; };
struct PatchDef808 { PatchDef808* next; uint32_t id; char name[49]; PatchParam808* params; char source819[96]; int8_t noteOffset44=0; };
PatchParam808* patchEditableAt44(PatchDef808*,uint32_t);
uint32_t patchEditableCount44(PatchDef808*);
struct PatchValue808 { PatchValue808* next; uint32_t parameter; uint16_t value; };
struct PatchContext808 { uint32_t patch; int8_t channel; PatchValue808* values; uint32_t extra[15]; bool explicit819; };
struct PatchDb808 { PatchDef808* defs; PatchContext808 context[16][51]; uint32_t nextPatch,nextParam; };
EXTMEM PatchDb808 patchDb808,patchStage808;
EXTMEM PatchContext808 patchClipboard808[4][16];
bool patchClipboardValid808[4][16]={};
EXTMEM uint16_t modRat808[64],fullRat808[64];
int16_t midiAdvance808=0,fullMidiAdvance808=0;
uint8_t patchUi808=0;bool patchBank808=false;uint32_t patchPage808=0;
uint8_t noteScale37=0;
void noteLabel37(char*,size_t,uint8_t,uint8_t);
void patchMetadataStart37();
void patchMetadataService37();
void patchStats10();
void patchCatalogScan10();
void patchResolve10(PatchDb808&);
PatchParam808* patchAddress10(const PatchContext808&,uint8_t,uint16_t,uint8_t);
const char* patchLabel10(const PatchContext808&,const PatchParam808&,bool);
void patch808Init();
void patch808Open();
bool patch808Back();
bool patch808Roy(uint8_t roy,int32_t delta,bool click);
bool patch808Screen(uint8_t index);
bool patch808Ky(uint8_t index,int32_t delta);
bool patch808Display();
void patch808Toggle(int direction);
void patch808Play();
void patch808Pattern(uint8_t track);
void patch808PatternAt(uint8_t track,uint8_t pattern);
void patch808Service();
bool patch808Base(uint8_t channel,uint8_t cc,int &value);
bool patch808Write(File &f);
bool patch808Read(File &f, bool install);
uint32_t patch808DiskSize();
uint32_t patch808Hash();
void patch808Copy(uint8_t ft,uint8_t fp,uint8_t tt,uint8_t tp);
void patch808Render();
ModUnit805 mod808EffectiveUnit(uint16_t key);
ModMeta806 mod808EffectiveMeta(uint16_t key);

void mod808FlushMidi(int64_t at,uint8_t budget);
void midi808Open();
void checks808();
void patch808ClearClipboard();
bool patch808CaptureClipboard(uint8_t slot,uint8_t index,uint8_t track,uint8_t pattern);
bool patch808PasteClipboard(uint8_t slot,uint8_t index,uint8_t track,uint8_t pattern);

// V8.19: sixteen composable definitions per context; NONE can override inheritance.
bool patchHas819(const PatchContext808&,uint32_t);
PatchParam808* patchNext819(const PatchContext808&,PatchParam808*);
PatchParam808* patchParam819(const PatchContext808&,uint32_t);
uint32_t patchCount819(const PatchContext808&);
void patchList819();
void patchListRender819(char (*)[21]);
PatchDef808* patchSelected819(bool load);
bool patchDelete819();
void patchDeleteConfirm819(bool);
void patchSelectId819(uint32_t);
void checks819();

uint32_t patchListCount819();

void patchListService819();
