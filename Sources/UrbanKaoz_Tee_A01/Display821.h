#pragma once
// Complete frames only. UART congestion must never wait inside a musical call.
static bool saveAlert08=false;
void alertAction08();
class Display821 : public Print {
 public:
  // Longest emitted UI frame is below 192 bytes (long scroll text <=95).
  static constexpr unsigned COUNT=24, LENGTH=192;
  struct Frame { char text[LENGTH]; uint32_t order,epoch; uint16_t length; bool used,pending; } frames[COUNT];
  char line[LENGTH], sending[LENGTH];
  uint16_t position=0,sendAt=0,sendLength=0;
  uint32_t sequence=0,epoch=0,queued=0,sent=0,replaced=0,duplicates=0,overflow=0;
  bool discard=false,resync=false;
  using Print::write;
  void begin(uint32_t baud);
  void invalidate();
  unsigned pendingCount()const{unsigned n=0;for(const auto&f:frames)n+=f.pending;return n;}
  int available(){return Serial2.available();}
  int read(){return Serial2.read();}
  static unsigned keyLength(const char*s){
    unsigned n=0;while(s[n]&&s[n]!=','&&s[n]!='|')++n;
    if(!strncmp(s,"GRAPH36,",8)||!strncmp(s,"FIELD,",6)||!strncmp(s,"MODLCD|",7)||!strncmp(s,"RMX910|",7)||!strncmp(s,"CFG,",4)){
      ++n;while(s[n]&&s[n]!=','&&s[n]!='|')++n;
    }
    return n;
  }
  static bool sameKey(const char*a,const char*b){if(!strncmp(a,"UI",2)&&!strncmp(b,"UI",2))return true;unsigned n=keyLength(a);return n==keyLength(b)&&!strncmp(a,b,n);}
  static bool persistent(const char*s){return !strncmp(s,"UI",2)||!strncmp(s,"MODLCD|",7)||!strncmp(s,"RMX910|",7)||!strncmp(s,"FIELD,",6)||!strncmp(s,"BPM,",4)||!strncmp(s,"MODELEDS,",9)||!strncmp(s,"VOL818,",7);}
  // These commands change which cached fields are visible. Keep their order;
  // discard dedup history at the transition, never pending transport bytes.
  static bool barrier(const char*s){return !strncmp(s,"UI",2)||!strncmp(s,"BANK",4)||!strncmp(s,"GRID,",5)||!strncmp(s,"CUSTOM,",7)||!strncmp(s,"DBG",3)||!strncmp(s,"HELLO",5);}
  static bool critical50(const char*s){return !strcmp(s,"HOME55")||!strncmp(s,"UI",2)||!strncmp(s,"BUSY910,",8)||!strncmp(s,"ALERT08|",8);}
  void enqueue();
  size_t write(uint8_t c) override;
  void service();
};
FLASHMEM void Display821::begin(uint32_t baud){memset(frames,0,sizeof(frames));position=sendAt=sendLength=0;Serial2.begin(baud);}
FLASHMEM void Display821::invalidate(){++epoch;for(auto&f:frames)if(!f.pending)f.used=false;}
FLASHMEM void Display821::enqueue(){
    line[position]=0;if(!position)return;
    if(!strncmp(line,"MSG|",4)&&(strstr(line,"SAVE ERROR")||strstr(line,"SAVE FAILED")||strstr(line,"SAVE UNAVAILABLE"))){saveAlert08=true;strcpy(line,"ALERT08|SAVE ERROR");position=strlen(line);}
    int slot=-1,freeSlot=-1;uint32_t oldest=UINT32_MAX;
    for(unsigned i=0;i<COUNT;++i){auto&f=frames[i];
      if(f.used&&sameKey(f.text,line)){slot=i;break;}
      if(!f.used){freeSlot=i;oldest=0;}else if(!f.pending&&f.order<oldest){freeSlot=i;oldest=f.order;}
    }
    bool boundary=barrier(line);
    if(slot>=0&&frames[slot].epoch==epoch&&!strcmp(frames[slot].text,line)&&persistent(line)){++duplicates;return;}
    if(boundary){
      // A newer mode must not reuse stale per-field deduplication history.
      ++epoch;
    }
    if(slot<0)slot=freeSlot;
    if(slot<0&&critical50(line)){
      // A full queue may shed a presentation update, never the mode/overlay
      // command needed to leave a confirmation or a completed Full operation.
      uint32_t first=UINT32_MAX;
      for(unsigned i=0;i<COUNT;++i)if(!critical50(frames[i].text)&&!barrier(frames[i].text)&&frames[i].order<first){first=frames[i].order;slot=i;}
      if(slot>=0)++overflow;
    }
    if(slot<0){++overflow;resync=false;return;}
    auto&f=frames[slot];if(f.pending)++replaced;
    memcpy(f.text,line,position+1);f.length=position;f.used=f.pending=true;f.order=++sequence;f.epoch=epoch;++queued;
  }
FLASHMEM size_t Display821::write(uint8_t c){
    if(c=='\r')return 1;
    if(c=='\n'){if(!discard)enqueue();position=0;discard=false;return 1;}
    if(discard)return 1;
    if(position+1>=LENGTH){discard=true;++overflow;resync=false;return 1;}
    line[position++]=c;return 1;
  }
FLASHMEM void Display821::service(){
    unsigned budget=64;
    while(budget){
      if(sendAt==sendLength){
        int slot=-1;uint32_t oldest=UINT32_MAX;
        for(unsigned i=0;i<COUNT;++i)if(frames[i].pending&&frames[i].order<oldest){slot=i;oldest=frames[i].order;}
        if(slot<0)return;auto&f=frames[slot];memcpy(sending,f.text,f.length);sending[f.length]='\n';sendLength=f.length+1;sendAt=0;f.pending=false;++sent;
      }
      int room=Serial2.availableForWrite();if(room<=0)return;
      unsigned n=min(budget,min((unsigned)room,(unsigned)(sendLength-sendAt)));
      Serial2.write((const uint8_t*)sending+sendAt,n);sendAt+=n;budget-=n;
    }
  }
DMAMEM Display821 display821;
void alertAction08(){if(saveAlert08){saveAlert08=false;display821.println("ALERT08|OFF");}}
// Bit masks describe presentation only; Picran owns the blink clock.
static uint32_t menuBlink821[4]={};
static void clearMenuBlink821(){memset(menuBlink821,0,sizeof(menuBlink821));}
extern char modPreviousLcd806[4][2][17];
