#pragma once
#define RMX_ALLOC extmem_malloc
#define RMX_FREE extmem_free
#include "RemixCore911.h"
rmx911::Model remixSource911,remixDraft911;
rmx911::History remixHistory911;
rmx911::Selection remixSelection911,remixCopySelection911;
uint8_t remixUndoRole911=1;
rmx911::Model* remixEditBefore911=nullptr;
uint16_t remixSourceMask911=0xFFFF,remixTargetMask911=0xFFFF;
uint16_t remixSourceBar911=0,remixHearBar911=0,remixLength911=96;
uint16_t remixTargetBar911=0,remixBars911=1;
int16_t remixLatency911=0,remixAnchor911=-1;
bool remixFine911=false,remixChain911=false,remixHold911=false,remixLoop911=false,remixReady911=false;
bool remixWriteView911=false,remixWriteCurrent911=false,remixLink911=false,remixConfirm911=false;
uint8_t remixWriteRow911=0,remixWriteMinimum911=0,remixPre911=0,remixCopies911=1,remixPost911=0;
bool remixBrowser911=false,remixLoading911=false;
bool remixLoadSource42=false;
uint8_t remixLoadBar42=0;
char remixLoadPath911[80]={};
uint8_t patternLinkColor55[16][16]={};
void linkRender55();void linkButton55(uint8_t);void linkRoy55(uint8_t,int,bool);
uint16_t patternLinks911[16][16]={};
uint16_t remixKeys911=0;
struct RemixAudition911 {bool active=false,held=false;uint8_t pad=0;uint32_t at=0,next=0;uint32_t ticks=0,length=96;uint16_t tracks=0xFFFF;int32_t start=0;uint8_t role=0;};
RemixAudition911 remixAudition911;
const rmx911::Model* remixEmittingModel911=nullptr;
const rmx911::Note* remixEmittingNote911=nullptr;
bool remixEnter911();void remixLeave911();
bool remixPad(uint8_t n,uint8_t p,uint8_t edge);bool remixRender();
bool remixScreen(uint8_t index);void remixOpenWrite();
bool remixKy911(uint8_t ky,int32_t delta);void remixKey911(uint8_t ky,bool pressed);
void remixDisplay911(bool force=false);void remixService911();
void remixInvalidateDisplay915();
bool remixTrack911(uint8_t track,uint32_t tick);
bool remixButton911(uint8_t bit);bool remixBack911();
bool remixFinishSelection911(bool accept);
bool remixQv911(uint8_t view);
bool remixDelay911(uint8_t track,uint8_t field,int&value);
bool remixNoteDelay911(uint8_t track,uint8_t note,uint8_t velocity,uint32_t gate);
bool remixLocks911(uint8_t track,uint8_t channel,uint8_t pitch,uint32_t gate);
void remixBrowserOpen911(bool save);void remixSave911();bool remixLoadChoice911();
int remixPatchValue937(const PatchParam808&,int);void remixPatchEdit937(const PatchParam808&,int,int);
bool patternLinkButton911(uint8_t bit);bool songLinkedPlace911(uint8_t track,uint8_t pattern,uint32_t position);
bool remixInstall911(const rmx911::Model&model,bool current,uint8_t row,uint8_t pre,uint8_t copies,uint8_t post,bool overwrite,bool dryRun=false,bool rangeOnly42=false);
void remixChecks911();void remixStatus911();
bool songExportNotes911(const rmx911::Model&model,uint8_t row,uint16_t bars);
void remixEditStatus912();void remixChecks912();

#include "RemixCore913.h"
#include "RemixCore916.h"
uint8_t remixHit916=rmx916::PadR,remixLen916=rmx916::Fixed,remixQuantum916=0,remixChannels916=0;
int8_t remixChainRole916=-1;
uint64_t remixChainBar917=UINT64_MAX;int remixPhaseOffset917=0;
rmx913::Phase remixPhase917(uint64_t tick);
bool remixChannelsPreview917=false;
uint16_t remixFixed916=96;
void remixPosition916(uint8_t role,uint8_t pad);
void checks916();
uint8_t remixWait913=0,remixQuan913=0,remixLayout913=0;
bool remixRecord913=false,remixDelete913=false,remixClearAsk913=false,remixClearYes913=false;
bool remixLayoutPreview913=false;uint32_t remixLayoutUntil913=0;
int16_t remixQuantShift913=0;uint8_t remixHeldMap913[2][16]={};
rmx911::Model* remixTakeBefore913=nullptr;
rmx911::List<uint32_t> remixRecorded913;uint64_t remixRecordCycle913=UINT64_MAX;
const uint16_t remixQuantTicks913[5]={0,48,96,192,384};
uint32_t remixEpoch913=0;
bool remixWaitActive913();
void remixRecordToggle913();bool remixFinishTake913();void remixRecordNote913(const rmx911::Model&,uint32_t,uint8_t,uint8_t=255);
void remixSessionService913();bool remixDeleteStep913(uint8_t);bool remixClearDraft913();
void remixLayoutEp913();void remixCancelPreview913();void remixChecks913();

bool remixVoiceSent913=false;

uint16_t remixPendingSource915=0;bool remixSeek915=false,remixDraftInitialized915=false;
uint32_t remixSourceBuilds915=0,remixTargetBuilds915=0;
void checks915();void status915();
bool remixStoreClipboard915();bool remixPasteClipboard915(uint8_t);void remixSeekPad915(uint8_t);
