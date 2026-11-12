#include "Graph36State.h"
// Livraison A01, affichage V1. Provenance : UrbanKaoz_Picran_V9_37.ino.
// CONSIGNE ROY : ne jamais publier ce firmware ou ses sources sur GitHub
// sans une demande explicite, tres precise de Roy visant cette publication.
// Sources preparees seulement : ni compilation ni televersement pour A01.
#include "DisplayWork931.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

/*
  UrbanKaoz_Picran_V8.08_ModeLeds

  CHAPITRES V27
  1A1 - Picran est le Pico Grove qui gere tous les ecrans.
  LED wiring: GP2 DELETE red, GP3 SONG green, GP4 selection orange,
  GP5 PATTERN yellow, GP10 GROOVE white, GP11 REMIX blue.
  MODELEDS,<selection>,<remix>,<pattern>,<groove>,<delete>,<song>.

  Grand LCD 20x4 sur I2C0 (GP8 SDA, GP9 SCL), adresse 0x27 : menus.
  Quatre LCD 16x2 sur I2C1 (GP6 SDA, GP7 SCL), dans l'ordre physique :
    Ecran 1 sans pont : 0x27   -> TRK CHA NOT VEL
    Ecran 2 pont A0   : 0x26   -> GAT TIM PBN BPM
    Ecran 3 pont A1   : 0x25   -> RST RSP RAC REX
    Ecran 4 pont A2   : 0x23   -> RCV RLV VST VTR

  UART vers Teensy Serial2 : Pico P2/GP1 RX, Pico P1/GP0 TX, 38400 bauds.
  Le pave 4x4 reste lu sur GP12..GP19 et envoie KEY,<touche> au Teensy.
*/

#define TEE_SERIAL Serial1

static const uint32_t TEE_BAUD = 38400;
static const uint8_t UART_TX_PIN = 0;
static const uint8_t UART_RX_PIN = 1;
static const uint8_t MAIN_LCD_SDA_PIN = 8;
static const uint8_t MAIN_LCD_SCL_PIN = 9;
static const uint8_t SMALL_LCD_SDA_PIN = 6;
static const uint8_t SMALL_LCD_SCL_PIN = 7;
static const uint8_t SELECT_LED_PIN = 4;
static const uint8_t REMIX_LED_PIN = 11;
static const uint8_t PATTERN_LED_PIN = 5;
static const uint8_t GROOVE_LED_PIN = 10;
static const uint8_t DELETE_LED_PIN = 2;
static const uint8_t SONG_LED_PIN = 3;
static const uint8_t MODE_LED_PINS[6] = {
  SELECT_LED_PIN, REMIX_LED_PIN, PATTERN_LED_PIN,
  GROOVE_LED_PIN, DELETE_LED_PIN, SONG_LED_PIN
};
bool modeLedBootPassed = false;
uint32_t songWarnStart932=0,songWarnDuration932=0;
bool songLedBase932=false;
static const uint8_t MAIN_LCD_ADDRESS = 0x27;
static const uint8_t LCD_COUNT = 4;
static const uint8_t LCD_COLS = 16;
static const uint8_t LCD_ADDRESS[LCD_COUNT] = {0x27, 0x26, 0x25, 0x23};
static const char* LCD_LABELS[LCD_COUNT] = {
  "TRK CHA NOT VEL",
  "GAT TIM MOV PBN",
  "RVL MOD RST RSP",
  "RAC SEX RCV VEX "
};
static const int8_t GRID_DISPLAY_FIELD[16] = {
  0, 1, 2, 3, 4, 5, -1, 6,
  13, 16, 8, 9, 10, 11, 12, 15
};

struct LcdScreen {
  TwoWire* bus;
  uint8_t address;
  bool present;
  char cache[2][17];
  bool cacheValid[2];
};

LiquidCrystal_I2C mainLcd(MAIN_LCD_ADDRESS, 20, 4);
bool mainLcdPresent = false;
char mainCache[4][21] = {{0}};
bool mainCacheValid[4] = {false, false, false, false};
LcdScreen screens[LCD_COUNT];
bool i2c1Found[128] = {false};
bool trackPreview811=false;
uint8_t trackNav815=255;
uint8_t trackVolume818=90;bool trackHeld818=false;
int32_t gridValue[16] = {
  1, 1, 36, 100,
  48, 0, 0, 120,
  24, 24, 0, 0,
  0, 0, 1, 1
};

char uartLine[320] = {0};
bool uartDiscardLine = false;
uint16_t uartLinePos = 0;
uint32_t rxCount = 0;
bool displayDirty = true;
bool synkoz933=false;
uint8_t renderMask931=31;bool statusRequested931=true,snapshotRequested931=true;
int8_t valueOverrideIndex = -1;
char valueOverrideText[5] = "";
bool loopShowsEnd = false;
uint8_t loopModes817=0;
uint16_t loopStartValue = 1;
uint16_t loopEndValue = 16;
uint8_t patternValue = 1;
uint8_t beatBars = 1;
uint32_t beatPosition = 1;
bool beatEditLength = false;
int8_t zoomLevel = 0;
bool roy1EditsZoom = false;
bool helpActive = false;
uint8_t helpKyIndex = 0;
char helpName[33] = "";
bool menuActive = false;
char menuLine[4][21] = {{0}};
uint8_t scrollRow821=255,scrollOffset821=0;char scrollText821[96]={};
uint32_t menuBlink821[4]={};
int8_t deletePreview55=-1;char deleteText55[5]={};
uint16_t lockBlink11[4]={};
uint16_t editBlink60[4]={};
bool blinkHidden821=false;
uint32_t blinkAt821=0;
void serviceBlink821(){
  if(deletePreview55<0&&!(lockBlink11[0]|lockBlink11[1]|lockBlink11[2]|lockBlink11[3]|editBlink60[0]|editBlink60[1]|editBlink60[2]|editBlink60[3])&&(!menuActive||!(menuBlink821[0]|menuBlink821[1]|menuBlink821[2]|menuBlink821[3])))return;
  uint32_t now=millis();if(now-blinkAt821<350)return;
  blinkAt821=now;blinkHidden821=!blinkHidden821;renderMask931|=31;displayDirty=true;
}
bool saveAlert08=false,sdReady08=true;char noteTexts08[4][12]={};
bool messageActive = false;
char messageText[21] = {0};
uint32_t messageUntilMs = 0;
uint32_t lastRenderMs = 0;
uint32_t lastStatusMs = 0;
uint32_t lastLcdRecoveryProbeMs = 0;
uint32_t lastMainLcdRecoveryProbeMs = 0;
bool loopBoundaryFlashOn = false;
uint32_t loopBoundaryFlashUntilMs = 0;
static const uint16_t LOOP_BOUNDARY_FLASH_MS = 120;
bool timeLabelMove = false;
bool grooveEditing = false, grooveAssigning = false;
uint8_t grooveSheet = 1;
bool grooveActive51=false;
int grooveValues[5] = {};
bool grooveCommon[5] = {true,true,true,true,true};
int grooveAssignment = 0;
bool grooveListVisible = false;
uint16_t grooveRepeatEditing = 0;
int16_t grooveListSheet[14] = {};
int8_t grooveListRepeats[14] = {};
int8_t grooveDeltaField = -1;
int grooveDeltaValue = 0;
uint32_t grooveDeltaUntil = 0;
static const uint16_t LCD_RECOVERY_PROBE_MS = 250;
static const uint16_t MAIN_LCD_RECOVERY_PROBE_MS = 500;
void renderDisplays(bool force);
bool debugDisplayActive = false;
bool debugScreenIdentity = false;
char debugMainLine[4][21] = {{0}};
char debugSmallLine[2][17] = {{0}};

static const uint8_t KEYPAD_ROW_PINS[4] = {15, 14, 13, 12};
static const uint8_t KEYPAD_COL_PINS[4] = {16, 17, 19, 18};
static const char KEYPAD_KEYS[4][4] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
static const uint16_t KEYPAD_DEBOUNCE_MS = 30;
char keypadCandidate = 0;
char keypadStable = 0;
uint32_t keypadCandidateSinceMs = 0;

void copyText(char* destination, size_t destinationSize, const char* source) {
  if (!destination || destinationSize == 0) return;
  if (!source) source = "";
  strncpy(destination, source, destinationSize - 1);
  destination[destinationSize - 1] = '\0';
}

// V9.37: publish the exact final LCD rows directly on Picran USB. This keeps
// display recording independent from the Teensy and from its firmware load.
static char inverseOriginal904[8]={};
static char smallInverse902[4]={};
static char presetInverse07=0;
bool underline927(uint8_t);uint8_t plain927(uint8_t);
bool quarter937(uint8_t c){return c>=0xA0&&c<=0xA9;}
bool sendGlyph37(const char*,uint8_t,uint8_t,uint8_t);
bool presentationActive43();const uint8_t* presentationGlyph43(uint8_t);
void sendUkdLine(const char* screen, uint8_t row, const char* text) {
  Serial.print(F("UKD1|L|"));
  Serial.print(screen);
  Serial.print('|');
  Serial.print(row);
  Serial.print('|');
  for (const char* p = text; p && *p; ++p) {
    char original=screen[0]>='A'&&screen[0]<='D'?(*p==1?smallInverse902[screen[0]-'A']:(screen[0]=='C'&&*p==2?presetInverse07:0)):(*p>0&&*p<8?inverseOriginal904[(uint8_t)*p]:0);
    if(sendGlyph37(screen,row,(uint8_t)(p-text),(uint8_t)*p)){}
    else if(quarter937((uint8_t)*p)){Serial.print("%U");Serial.print((uint8_t)('0'+(uint8_t)*p-0xA0),HEX);}
    else if(underline927((uint8_t)*p)){uint8_t ch=plain927((uint8_t)*p);if(ch>=0x12&&ch<=0x14){Serial.print(ch==0x12?"1/4":ch==0x13?"1/8":"1/16");}else Serial.write(ch);}
    else if(original){Serial.print(F("%I"));Serial.print((uint8_t)original,HEX);}
    else if (*p == '%') Serial.print(F("%25"));
    else if (*p == '|') Serial.print(F("%7C"));
    else Serial.print(*p);
  }
  Serial.println();
}

// V9.37: one dynamic CGRAM slot for the first character of the edited Delay value.
static int underlineCode927=-1;
static const uint8_t noteGlyph927[3][8]={
 {2,2,2,2,2,14,30,12},{6,5,5,4,4,12,28,8},{6,5,6,5,4,12,28,8}};
static const uint8_t digitGlyph927[11][8]={
 {14,17,19,21,25,17,14,0},{4,12,4,4,4,4,14,0},
 {14,17,1,2,4,8,31,0},{30,1,1,14,1,1,30,0},
 {2,6,10,18,31,2,2,0},{31,16,16,30,1,1,30,0},
 {14,16,16,30,17,17,14,0},{31,1,2,4,8,8,8,0},
 {14,17,17,14,17,17,14,0},{14,17,17,15,1,1,14,0},
 {6,8,8,30,8,8,8,0}};
bool underline927(uint8_t c){return (c>=0x80&&c<=0x8A)||(c>=0x90&&c<=0x92);}
uint8_t plain927(uint8_t c){return c>=0x80&&c<=0x89?'0'+c-0x80:c==0x8A?'f':c>=0x90&&c<=0x92?0x12+c-0x90:c;}
uint8_t noteSlot927(uint8_t c){return c==0x12?6:c==0x13?7:0;}

void mainPrintLine(uint8_t row, const char* text, bool force = false) {
  if (!mainLcdPresent || row > 3) return;
  char padded[21];
  memset(padded, ' ', 20);
  padded[20] = '\0';
  if (text) {
    for (uint8_t i = 0; i < 20 && text[i]; i++) padded[i] = text[i];
  }
  for(uint8_t i=0;i<20;++i){uint8_t ch=padded[i];if(quarter937(ch)){uint8_t g[8];memcpy(g,digitGlyph927[ch-0xA0],8);g[7]=31;mainLcd.createChar(1+row,g);inverseOriginal904[1+row]=0;}if(underline927(ch)&&underlineCode927!=ch){
    uint8_t glyph[8];memcpy(glyph,ch>=0x90?noteGlyph927[ch-0x90]:digitGlyph927[ch-0x80],8);glyph[7]=31;
    mainLcd.createChar(5,glyph);inverseOriginal904[5]=0;underlineCode927=ch;
    for(uint8_t row=0;row<4;++row)mainCacheValid[row]=false;
  }}
  if (!force && mainCacheValid[row] && strcmp(mainCache[row], padded) == 0) return;
  mainLcd.setCursor(0, row);
  for(uint8_t i=0;i<20;++i){uint8_t ch=padded[i];mainLcd.write(quarter937(ch)?1+row:underline927(ch)?5:ch>=0x12&&ch<=0x14?noteSlot927(ch):ch);}
  memcpy(mainCache[row], padded, sizeof(padded));
  mainCacheValid[row] = true;
  sendUkdLine("EP", row, padded);
}


void rhythmGlyphs923(){
 uint8_t quarter[8]={2,2,2,2,2,14,30,12};
 uint8_t eighth[8]={6,5,5,4,4,12,28,8};
 uint8_t sixteenth[8]={6,5,6,5,4,12,28,8};
 mainLcd.createChar(6,quarter);mainLcd.createChar(7,eighth);mainLcd.createChar(0,sixteenth);
}
void glyph902(char c,uint8_t* out){
 static const uint8_t letters[26][7]={
 {14,17,17,31,17,17,17},{30,17,17,30,17,17,30},{14,17,16,16,16,17,14},{30,17,17,17,17,17,30},
 {31,16,16,30,16,16,31},{31,16,16,30,16,16,16},{14,17,16,23,17,17,15},{17,17,17,31,17,17,17},
 {14,4,4,4,4,4,14},{7,2,2,2,18,18,12},{17,18,20,24,20,18,17},{16,16,16,16,16,16,31},
 {17,27,21,21,17,17,17},{17,25,21,19,17,17,17},{14,17,17,17,17,17,14},{30,17,17,30,16,16,16},
 {14,17,17,17,21,18,13},{30,17,17,30,20,18,17},{15,16,16,14,1,1,30},{31,4,4,4,4,4,4},
 {17,17,17,17,17,17,14},{17,17,17,17,17,10,4},{17,17,17,21,21,21,10},{17,17,10,4,10,17,17},
 {17,17,10,4,4,4,4},{31,1,2,4,8,16,31}};
 static const uint8_t digits58[10][7]={
 {14,17,19,21,25,17,14},{4,12,4,4,4,4,14},{14,17,1,2,4,8,31},
 {30,1,1,14,1,1,30},{2,6,10,18,31,2,2},{31,16,16,30,1,1,30},
 {14,16,16,30,17,17,14},{31,1,2,4,8,8,8},{14,17,17,14,17,17,14},{14,17,17,15,1,1,14}};
 if(c>='0'&&c<='9'){for(unsigned r=0;r<8;++r)out[r]=(r<7?digits58[c-'0'][r]:0)^31;return;}
 if(c>='a'&&c<='z')c-=32;
 for(unsigned r=0;r<8;++r)out[r]=(r<7&&c>='A'&&c<='Z'?letters[c-'A'][r]:0)^31;
}
int8_t pressIndex902=-1;uint32_t pressUntil902=0;
char pressLabel902[6]={};char pressRows902[4][21]={};
void press902(unsigned index,unsigned duration){
 if(index>7||!menuActive||!mainLcdPresent)return;
 underlineCode927=-1;pressIndex902=index;pressUntil902=millis()+duration;memcpy(pressRows902,menuLine,sizeof(pressRows902));
 memcpy(pressLabel902,menuLine[2+index/4]+5*(index%4),5);pressLabel902[5]=0;
 for(unsigned c=0;c<5;++c){uint8_t glyph[8];glyph902(pressLabel902[c],glyph);mainLcd.createChar(c+1,glyph);inverseOriginal904[c+1]=pressLabel902[c];}
 displayDirty=true;
}
// Only the first character of each selected field on the home EP is inverted.
// Slots 1..4 avoid a NUL in cached/USB strings; other menus remain unchanged.
void inverseFirst904(char* text,uint8_t slot){
 if(inverseOriginal904[slot]==text[0]){text[0]=slot;return;}
 inverseOriginal904[slot]=text[0];for(unsigned row=0;row<4;++row)mainCacheValid[row]=false;
 static const uint8_t digits[10][8]={
 {14,17,19,21,25,17,14,0},{4,12,4,4,4,4,14,0},
 {14,17,1,2,4,8,31,0},{30,1,1,14,1,1,30,0},
 {2,6,10,18,31,2,2,0},{31,16,16,30,1,1,30,0},
 {14,16,16,30,17,17,14,0},{31,1,2,4,8,8,8,0},
 {14,17,17,14,17,17,14,0},{14,17,17,15,1,1,14,0}};
 const uint8_t x[8]={17,17,10,4,10,17,17,0},slash[8]={1,2,2,4,8,8,16,0};
 const uint8_t* glyph=text[0]>='0'&&text[0]<='9'?digits[text[0]-'0']:text[0]=='X'?x:slash;
 uint8_t inverse[8];for(unsigned r=0;r<8;++r)inverse[r]=glyph[r]^31;
 mainLcd.createChar(slot,inverse);text[0]=slot;
}
#include "Ui910.h"
void renderMainScreen(bool force = false) {
  if(renderBusy910(force))return;
  if (!mainLcdPresent) return;
  if(songWarnDuration932){mainPrintLine(0,"connect song screen",force);for(uint8_t r=1;r<4;++r)mainPrintLine(r,"",force);return;}
  if (menuActive||pressIndex902>=0) {
    for (uint8_t row = 0; row < 4; row++) {
      char shown[21];memcpy(shown,menuLine[row],21);
      if(pressIndex902<0&&row==scrollRow821){strncpy(shown,scrollText821+scrollOffset821,20);shown[20]=0;}
      if(blinkHidden821)for(uint8_t c=0;c<20&&shown[c];++c)if(menuBlink821[row]&(1UL<<c))shown[c]=' ';
      if(pressIndex902>=0&&row==2+pressIndex902/4){unsigned col=5*(pressIndex902%4);
        if(!memcmp(shown+col,pressLabel902,5))for(unsigned c=0;c<5;++c)if(pressLabel902[c]!=' ')shown[col+c]=c+1;}
      if(pressIndex902>=0)for(uint8_t i=0;i<20;++i)shown[i]=plain927((uint8_t)shown[i]);
      mainPrintLine(row,shown,force);
    }
    return;
  }

  char line[21];
  char bpmBase[8], zoomBase[8], beatTopBase[8], beatBottomBase[8];
  char bpmText[6], zoomText[6], beatTop[6], beatBottom[6];
  snprintf(bpmBase, sizeof(bpmBase), "%ld", (long)gridValue[7]);
  uint8_t zoomFactor = (uint8_t)(1u << abs(zoomLevel));
  snprintf(zoomBase, sizeof(zoomBase), zoomLevel < 0 ? "/%02u" : "X%02u", zoomFactor);
  snprintf(beatTopBase, sizeof(beatTopBase), "%u/4", beatBars);
  if (zoomLevel < 0) {
    uint32_t lastBar = min((uint32_t)beatBars, beatPosition + zoomFactor - 1);
    snprintf(beatBottomBase, sizeof(beatBottomBase), "%lu-%lu",
             (unsigned long)beatPosition, (unsigned long)lastBar);
  } else if (zoomLevel > 0) {
    snprintf(beatBottomBase, sizeof(beatBottomBase), "%lu", (unsigned long)beatPosition);
  } else {
    snprintf(beatBottomBase, sizeof(beatBottomBase), "%lu", (unsigned long)beatPosition);
  }
  snprintf(bpmText,sizeof(bpmText),"%.5s",bpmBase);if(!roy1EditsZoom)inverseFirst904(bpmText,1);
  snprintf(zoomText,sizeof(zoomText),"%.5s",zoomBase);if(roy1EditsZoom)inverseFirst904(zoomText,2);
  snprintf(beatTop,sizeof(beatTop),"%.5s",beatTopBase);if(beatEditLength)inverseFirst904(beatTop,3);
  // Roy2 : le point n'apparait que sur la ligne 1. Son absence indique par
  // deduction que la ligne 2 (position de vue) est active.
  snprintf(beatBottom,sizeof(beatBottom),"%.5s",beatBottomBase);if(!beatEditLength)inverseFirst904(beatBottom,4);
  snprintf(line,sizeof(line),"%-5.5s%-5.5s%-4.4s%-4.4s %c",bpmText,beatTop,
    loopModes817&1?"S+16":"STA",loopModes817&2?"E+16":"END",synkoz933?'S':' ');
  if(!sdReady08){inverseFirst904(line+10,6);inverseFirst904(line+16,7);}
  mainPrintLine(0,line,force);
  char startText[6],endText[6];
  snprintf(startText,6,"%04u",loopStartValue);snprintf(endText,6,"%04u",loopEndValue);
  if(loopBoundaryFlashOn){if((loopStartValue-1)%16==0)startText[0]=0;if(loopEndValue%16==0)endText[0]=0;}
  snprintf(line,sizeof(line),"%-5.5s%-5.5s%-5.5s%-5.5s",zoomText,beatBottom,startText,endText);
  mainPrintLine(1, line, force);
  mainPrintLine(2, "PARA WRITE DLY  ENV ");
  mainPrintLine(3, messageActive ? messageText : "TRACK PAT   I/O LFO ");
}

bool i2cResponds(TwoWire &bus, uint8_t address) {
  bus.beginTransmission(address);
  return bus.endTransmission() == 0;
}

void invalidateMainScreenCache() {
  for (uint8_t row = 0; row < 4; row++) mainCacheValid[row] = false;
}

bool mainLcdBackpackLooksReset() {
  uint8_t received = Wire.requestFrom((int)MAIN_LCD_ADDRESS, 1);
  if (received != 1 || Wire.available() == 0) return false;
  return Wire.read() == 0xFF;
}

void releaseMainI2cBus() {
  Wire.end();
  pinMode(MAIN_LCD_SDA_PIN, INPUT_PULLUP);
  pinMode(MAIN_LCD_SCL_PIN, INPUT_PULLUP);
  delayMicroseconds(5);
  for (uint8_t pulse = 0; pulse < 9 && digitalRead(MAIN_LCD_SDA_PIN) == LOW; pulse++) {
    pinMode(MAIN_LCD_SCL_PIN, OUTPUT);
    digitalWrite(MAIN_LCD_SCL_PIN, LOW);
    delayMicroseconds(5);
    pinMode(MAIN_LCD_SCL_PIN, INPUT_PULLUP);
    delayMicroseconds(5);
  }
  pinMode(MAIN_LCD_SDA_PIN, OUTPUT);
  digitalWrite(MAIN_LCD_SDA_PIN, LOW);
  delayMicroseconds(5);
  pinMode(MAIN_LCD_SCL_PIN, INPUT_PULLUP);
  delayMicroseconds(5);
  pinMode(MAIN_LCD_SDA_PIN, INPUT_PULLUP);
  delayMicroseconds(5);
  Wire.setSDA(MAIN_LCD_SDA_PIN);
  Wire.setSCL(MAIN_LCD_SCL_PIN);
  Wire.begin();
  Wire.setClock(100000);
  Wire.setTimeout(10, true);
}

void recoverMainLcd(bool releaseBus) {
  if (releaseBus) releaseMainI2cBus();
  mainLcdPresent = i2cResponds(Wire, MAIN_LCD_ADDRESS);
  invalidateMainScreenCache();
  if (!mainLcdPresent) return;
  mainLcd.init();rhythmGlyphs923();memset(inverseOriginal904,0,sizeof(inverseOriginal904));
  mainLcd.backlight();
  renderDisplays(true);
}

void serviceMainLcdRecovery() {
  uint32_t now = millis();
  if ((uint32_t)(now - lastMainLcdRecoveryProbeMs) < MAIN_LCD_RECOVERY_PROBE_MS) return;
  lastMainLcdRecoveryProbeMs = now;
  bool responding = i2cResponds(Wire, MAIN_LCD_ADDRESS);
  if (!responding) {
    mainLcdPresent = false;
    invalidateMainScreenCache();
    return;
  }
  bool needsInit = !mainLcdPresent || mainLcdBackpackLooksReset();
  mainLcdPresent = true;
  if (needsInit) recoverMainLcd(false);
}

#include "ScreenMapPicran.h"
void expanderWrite(const LcdScreen &screen, uint8_t value) {
  screenMapWrite(screen,value);
}

void pulseEnable(const LcdScreen &screen, uint8_t value) {
  expanderWrite(screen, value | 0x04);
  delayMicroseconds(1);
  expanderWrite(screen, value & ~0x04);
  delayMicroseconds(50);
}

void writeNibble(const LcdScreen &screen, uint8_t nibble, bool dataMode) {
  uint8_t value = (uint8_t)((nibble & 0x0F) << 4);
  if (dataMode) value |= 0x01;
  expanderWrite(screen, value);
  pulseEnable(screen, value);
}

void lcdSend(const LcdScreen &screen, uint8_t value, bool dataMode) {
  writeNibble(screen, value >> 4, dataMode);
  writeNibble(screen, value, dataMode);
}

void lcdCommand(const LcdScreen &screen, uint8_t command) {
  lcdSend(screen, command, false);
  if (command == 0x01 || command == 0x02) delayMicroseconds(2000);
}

void lcdInit(LcdScreen &screen) {
  delay(50);
  writeNibble(screen, 0x03, false);
  delayMicroseconds(4500);
  writeNibble(screen, 0x03, false);
  delayMicroseconds(4500);
  writeNibble(screen, 0x03, false);
  delayMicroseconds(150);
  writeNibble(screen, 0x02, false);
  lcdCommand(screen, 0x28);
  lcdCommand(screen, 0x08);
  lcdCommand(screen, 0x01);
  lcdCommand(screen, 0x06);
  lcdCommand(screen, 0x0C);
  for(uint8_t n=0;n<3;++n){lcdCommand(screen,0x40|noteSlot927(0x12+n)*8);for(uint8_t row=0;row<8;++row)lcdSend(screen,noteGlyph927[n][row],true);}
  lcdCommand(screen,0x80);
}

void invalidateSmallScreenCache(uint8_t screenIndex) {
  if (screenIndex >= LCD_COUNT) return;
  screens[screenIndex].cacheValid[0] = false;
  screens[screenIndex].cacheValid[1] = false;
}

// Un PCF8574 fraichement remis sous tension relit 0xFF. Apres notre
// initialisation, E et RW sont maintenus bas : 0xFF signale donc une vraie
// perte d'alimentation du backpack, meme si elle a ete trop breve pour etre
// vue par le simple ping I2C.
bool lcdBackpackLooksReset(LcdScreen &screen) {
  uint8_t received = screen.bus->requestFrom((int)screen.address, 1);
  if (received != 1 || screen.bus->available() == 0) return false;
  return screen.bus->read() == 0xFF;
}

void serviceSmallLcdRecovery() {
 uint32_t now=millis();if((uint32_t)(now-lastLcdRecoveryProbeMs)<LCD_RECOVERY_PROBE_MS)return;lastLcdRecoveryProbeMs=now;
 bool redraw=false;
 for(unsigned p=0;p<4;++p){bool responding=i2cResponds(Wire1,LCD_ADDRESS[p]);LcdScreen hw={};hw.bus=&Wire1;hw.address=LCD_ADDRESS[p];hw.present=responding;
 bool init=responding&&(!screenPhysicalPresent[p]||(screenMapRoles[p]&&lcdBackpackLooksReset(hw)));
 if(responding!=screenPhysicalPresent[p])redraw=true;screenPhysicalPresent[p]=responding;
 if(init){lcdInit(hw);redraw=true;}if(responding&&!screenMapRoles[p]){lcdCommand(hw,0x08);Wire1.beginTransmission(hw.address);Wire1.write((uint8_t)0);Wire1.endTransmission();}}
 screenMapPresence();if(redraw){for(unsigned i=0;i<4;++i)invalidateSmallScreenCache(i);screenMapInvalidateGlyphs();renderDisplays(true);}
}

void screenMapHardware(){
 for(unsigned p=0;p<4;++p){screenPhysicalPresent[p]=i2cResponds(Wire1,LCD_ADDRESS[p]);if(!screenPhysicalPresent[p])continue;LcdScreen hw={};hw.bus=&Wire1;hw.address=LCD_ADDRESS[p];hw.present=true;lcdCommand(hw,screenMapRoles[p]?0x0C:0x08);for(uint8_t n=0;n<3;++n){lcdCommand(hw,0x40|noteSlot927(0x12+n)*8);for(uint8_t y=0;y<8;++y)lcdSend(hw,noteGlyph927[n][y],true);}lcdCommand(hw,0x80);if(!screenMapRoles[p]){lcdCommand(hw,0x08);Wire1.beginTransmission(hw.address);Wire1.write((uint8_t)0);Wire1.endTransmission();}}
 screenMapPresence();for(unsigned i=0;i<4;++i)invalidateSmallScreenCache(i);screenMapInvalidateGlyphs();displayDirty=true;
}
bool screenMapCommand(const char *line){
 if(!strncmp(line,"STEP26,",7)){unsigned on,value;char extra;if(sscanf(line+7,"%u,%u%c",&on,&value,&extra)==2&&on<2&&value>=1&&value<=4){if(stepLayoutVisible26!=on||stepLayoutValue26!=value){stepLayoutVisible26=on;stepLayoutValue26=value;displayDirty=true;}}return true;}

 if(!strncmp(line,"GTRACK35,",9)){unsigned t,l;char extra;if(sscanf(line+9,"%u,%u%c",&t,&l,&extra)==2&&t>=1&&t<=16&&l>=1&&l<=9){grooveTrack35=t;grooveList35=l;displayDirty=true;}return true;}
 if(!strncmp(line,"GACTIVE51,",10)){if(!strcmp(line+10,"0")||!strcmp(line+10,"1")){grooveActive51=line[10]=='1';displayDirty=true;}return true;}
 if(!strncmp(line,"GUS26,",6)){char*end;long n=strtol(line+6,&end,10);if(!*end&&n>=0&&n<=65535&&grooveUsers26!=n){grooveUsers26=n;displayDirty=true;}return true;}
 if(!strncmp(line,"SCRIDENT,",9)){char*end;long n=strtol(line+9,&end,10);if(!*end&&(n>=0&&n<4||n==255)){screenIdentify26=n;screenIdentifyService26();}return true;}
if(strncmp(line,"SCRMAP,",7))return false;uint8_t draft[4];if(!scrmap::parse(line+7,draft)){Serial.println("ERROR,SCRMAP,INVALID");return true;}if(memcmp(draft,screenMapRoles,4)){memcpy(screenMapRoles,draft,4);screenMapHardware();renderDisplays(true);}TEE_SERIAL.println("SCRMAP,OK");return true;}

void makePaddedLine(char output[17], const char* text) {
  memset(output, ' ', LCD_COLS);
  output[LCD_COLS] = '\0';
  if (!text) return;
  for (uint8_t i = 0; i < LCD_COLS && text[i]; i++) output[i] = text[i];
}

uint8_t cursorCode932[4]={};
void lcdPrintLine(uint8_t screenIndex, uint8_t row, const char* text, bool force = false) {
  if (screenIndex >= LCD_COUNT || row > 1) return;
  LcdScreen &screen = screens[screenIndex];
  if (!screen.present) return;
  char padded[17];
  makePaddedLine(padded, text);
  if(row==1&&!saveAlert08&&blinkHidden821)for(uint8_t i=0;i<16;++i)if((lockBlink11[screenIndex]|editBlink60[screenIndex])&(1u<<i))padded[i]=' ';
  if(row==1&&deletePreview55>=0&&screenIndex==deletePreview55/4){uint8_t col=(deletePreview55%4)*4;for(uint8_t i=0;i<4;++i)padded[col+i]=blinkHidden821?' ':deleteText55[i];}
  if (!force && screen.cacheValid[row] && strcmp(screen.cache[row], padded) == 0) return;


  for(uint8_t i=0;i<LCD_COLS;++i){uint8_t ch=padded[i];if(quarter937(ch)){uint8_t g[8];memcpy(g,digitGlyph927[ch-0xA0],8);g[7]=31;lcdCommand(screen,0x40|(2+i/4)*8);for(uint8_t k=0;k<8;++k)lcdSend(screen,g[k],true);}}
  for(uint8_t i=0;i<LCD_COLS;++i){uint8_t ch=(uint8_t)padded[i];if(underline927(ch)&&cursorCode932[screenIndex]!=ch){
    uint8_t glyph[8];memcpy(glyph,ch>=0x90?noteGlyph927[ch-0x90]:digitGlyph927[ch-0x80],8);glyph[7]=31;
    lcdCommand(screen,0x40|5*8);for(uint8_t r=0;r<8;++r)lcdSend(screen,glyph[r],true);cursorCode932[screenIndex]=ch;
  }}
  if (force || !screen.cacheValid[row]) {
    lcdCommand(screen, (uint8_t)(0x80 | (row == 0 ? 0x00 : 0x40)));
    for (uint8_t i = 0; i < LCD_COLS; i++) lcdSend(screen, quarter937((uint8_t)padded[i])?2+i/4:underline927((uint8_t)padded[i])?5:((uint8_t)padded[i]>=0x12&&(uint8_t)padded[i]<=0x14)?noteSlot927((uint8_t)padded[i]):(uint8_t)padded[i], true);
  } else {
    // Une rotation ne doit pas reecrire les 16 caracteres : cela faisait
    // visuellement bouger plusieurs valeurs alors qu'un seul champ changeait.
    for (uint8_t i = 0; i < LCD_COLS; i++) {
      if (screen.cache[row][i] == padded[i]) continue;
      lcdCommand(screen, (uint8_t)(0x80 | (row == 0 ? 0x00 : 0x40) | i));
      lcdSend(screen, quarter937((uint8_t)padded[i])?2+i/4:underline927((uint8_t)padded[i])?5:((uint8_t)padded[i]>=0x12&&(uint8_t)padded[i]<=0x14)?noteSlot927((uint8_t)padded[i]):(uint8_t)padded[i], true);
    }
  }
  memcpy(screen.cache[row], padded, sizeof(padded));
  screen.cacheValid[row] = true;
  char id[2] = {(char)('A' + screenIndex), '\0'};
  sendUkdLine(id, row, padded);
}

void sendUkdSnapshot() {
  for (uint8_t row = 0; row < 4; ++row)
    if (mainCacheValid[row]) sendUkdLine("EP", row, mainCache[row]);
  for (uint8_t screenIndex = 0; screenIndex < LCD_COUNT; ++screenIndex) {
    char id[2] = {(char)('A' + screenIndex), '\0'};
    for (uint8_t row = 0; row < 2; ++row)
      if (screens[screenIndex].cacheValid[row]) sendUkdLine(id, row, screens[screenIndex].cache[row]);
  }
}

void formatTwoDigitField(char output[5], int value) {
  value = constrain(value, 0, 99);
  snprintf(output, 4, " %02d", value);
}

void formatThreeDigitField(char output[5], int value) {
  value = constrain(value, 0, 999);
  snprintf(output, 4, "%03d", value);
}

void formatSignedField(char output[5], int value) {
  value = constrain(value, -999, 999);
  snprintf(output, 5, "%03d", value);
}

void formatCompactPositive(char output[5], int value) {
  if (value < 0) value = 0;
  if (value <= 999) {
    snprintf(output, 4, "%03d", value);
  } else {
    int thousands = constrain(value / 1000, 0, 9);
    int hundreds = constrain((value % 1000 + 50) / 100, 0, 9);
    snprintf(output, 4, "%dK%d", thousands, hundreds);
  }
}

void formatMidiNote(char output[5], int note) {
  static const char* NOTE_NAME[12] = {
    "C ", "C#", "D ", "D#", "E ", "F ",
    "F#", "G ", "G#", "A ", "A#", "B "
  };
  note = constrain(note, 0, 127);
  int octave = note / 12 - 1;
  snprintf(output, 4, "%s%d", NOTE_NAME[note % 12], octave);
}

void formatPitchBendPercent(char output[5], int rawValue) {
  rawValue = constrain(rawValue, -8192, 8191);
  int percent = rawValue >= 0 ? (rawValue * 100L + 4095) / 8191
                              : -(((-rawValue) * 99L + 4095) / 8192);
  formatSignedField(output, percent);
}

bool rollMod825=false;int rollVex1000=0,rollRelative1000=0,rollVel1000=0;
void formatGridField(uint8_t index, char output[5]) {
  if (grooveEditing || grooveAssigning) {
    if (index == 0) {
      int value = grooveEditing ? grooveSheet : grooveAssignment;
      if (value < 0) copyText(output, 5, "---");
      else if (value == 0) copyText(output, 5, "OFF");
      else if (value == 145) copyText(output, 5, "LIS");
      else snprintf(output, 5, "%c%u ", 'A' + (value - 1) / 9, (unsigned)((value - 1) % 9 + 1));
    } else if (grooveEditing && index >= 2 && index <= 6) {
      int field = index - 2, value = grooveValues[field];
      if (!grooveCommon[field] && field == grooveDeltaField && (int32_t)(grooveDeltaUntil - millis()) > 0)
        snprintf(output, 5, "%c%u ", grooveDeltaValue < 0 ? '-' : '+', (unsigned)abs(grooveDeltaValue));
      else if (!grooveCommon[field]) copyText(output, 5, "---");
      else if (!value) copyText(output, 5, "000");
      else snprintf(output, 5, "%c%u ", value < 0 ? '-' : '+', (unsigned)abs(value));
    } else copyText(output, 5, "---");
    return;
  }
  if(index==15){if(rollVex1000>=101)strcpy(output,rollVex1000==101?"/v/v":"/_/_");else snprintf(output,5,"%d",rollVex1000);return;}
  if(index==11&&gridValue[index]>=101){strcpy(output,gridValue[index]==101?"/v/v":"/_/_");return;}
  if(index==13&&rollRelative1000){snprintf(output,5,"%+d",rollVel1000);return;}
  if(index==16){copyText(output,5,rollMod825?"ON":"OFF");}
  else if(index==7){copyText(output,5,"   ");}
  else if(index==0&&trackNav815==3){snprintf(output,5,"%u",trackVolume818);}
  else if (index == 15) {
    copyText(output, 5, gridValue[index] ? "ON" : "OFF");
  } else if (index == 0 || index == 1 || index == 14) {
    formatTwoDigitField(output, gridValue[index]);
  } else if (index == 2) {
    formatMidiNote(output, gridValue[index]);
  } else if (index == 4) {
    snprintf(output,5,"%u",gridValue[index]);
  } else if (index == 5 || index == 10 || index == 12) {
    formatSignedField(output, gridValue[index]);
  } else if (index == 6) {
    formatPitchBendPercent(output, gridValue[index]);
  } else {
    formatThreeDigitField(output, gridValue[index]);
  }
}

uint16_t grooveSelection805 = 0;
bool customGrid805 = false;
bool modGrid806 = false;
char modRows806[4][2][17] = {};
uint16_t indMask50[4]={};char indGlyph50[4][8]={};

Graph36 graphs36[4];
void graphPixels36(Graph36&g){
 memset(g.pixels,0,sizeof(g.pixels));int previous=-1;
 for(int x=0;x<10;++x){float t=x/9.0f,y;
 if(!g.kind){switch(g.value){case 1:y=x<5?1:0;break;case 2:y=t;break;case 3:y=1-t;break;case 4:y=(sinf(t*6.2831853f)+1)/2;break;default:y=t<0.5f?t*2:2-t*2;break;}}
 else if(g.kind==4){y=g.value==0?t+.16f*sinf(t*12.5663706f):floorf(t*4)/4.0f;}
 else if(g.kind==2){float power=powf(32.0f,abs(g.value)/100.0f);y=g.value>=0?1-powf(t,power):powf(1-t,power);}
 else {float power=1+(g.maximum-1)*abs(g.value)/100.0f;float rise=g.value>0?1-powf(1-t,power):g.value<0?powf(t,power):t;y=g.kind==1?rise:1-rise;}
 int row=constrain((int)lroundf((1-y)*7),0,7);int a=previous<0?row:min(previous,row),b=previous<0?row:max(previous,row);for(int k=a;k<=b;++k)g.pixels[x/5][k]|=1<<(4-x%5);previous=row;
 }
}
bool graphCommand36(const char*line){int screen,col,kind,value,maximum=10;char tail;
 int fields=sscanf(line,"GRAPH36,%d,%d,%d,%d,%d%c",&screen,&col,&kind,&value,&maximum,&tail);if(fields!=5&&sscanf(line,"GRAPH36,%d,%d,%d,%d%c",&screen,&col,&kind,&value,&tail)!=4)return false;
 if(screen<0||screen>3||col<0||col>12||col%4||kind<0||kind>4||maximum<1||maximum>100||value<(kind?-100:0)||value>(kind?100:4))return true;
 auto&g=graphs36[screen];bool changed=!g.valid||g.col!=col||g.kind!=kind||g.value!=value||g.maximum!=maximum||strcmp(g.header,modRows806[screen][0]);
 if(changed){memset(indGlyph50[screen],0,8);g.dirty=true;g.valid=true;g.col=col;g.kind=kind;g.value=value;g.maximum=maximum;strcpy(g.header,modRows806[screen][0]);graphPixels36(g);screens[screen].cacheValid[1]=false;displayDirty=true;renderMask931|=1u<<screen;}return true;
}
void graphSlots36(uint8_t screen,uint8_t&first,uint8_t&second){first=3;second=4;if(screen==0&&graphs36[screen].kind)second=5;if(screen==3&&!strncmp(modRows806[3][0],"HOL",3)){first=1;second=5;}}
void graphUpload36(uint8_t screen){auto&g=graphs36[screen];if(!g.valid||!g.dirty||strcmp(g.header,modRows806[screen][0]))return;uint8_t a,b;graphSlots36(screen,a,b);auto&lcd=screens[screen];if(!lcd.present)return;g.dirty=false;for(int k=0;k<2;++k){lcdCommand(lcd,0x40|(k?b:a)*8);for(int y=0;y<8;++y)lcdSend(lcd,g.pixels[k][y],true);} }
void handleModLcd806(char *line) {
 memset(indGlyph50,0,sizeof(indGlyph50));
  // Reject partial or out-of-range frames without changing any display state.
  if(strlen(line)!=42||line[7]<'0'||line[7]>'3'||line[8]!='|'||line[25]!='|')return;
  uint8_t screen=line[7]-'0';
  memcpy(modRows806[screen][0],line+9,16);modRows806[screen][0][16]=0;
  memcpy(modRows806[screen][1],line+26,16);modRows806[screen][1][16]=0;
  modGrid806=true;displayDirty=true;
}
char customLabels805[16][4] = {};
int32_t customValues805[16] = {};
void handleCustom805(char *line) {
  memset(lockBlink11,0,sizeof(lockBlink11));
  strtok(line, ",");
  char labels[16][4] = {}; int32_t values[16] = {};
  char *label=strtok(nullptr,",");
  if(label&&strcmp(label,"OFF")==0){customGrid805=false;modGrid806=false;displayDirty=true;return;}
  for(uint8_t i=0;i<16;++i){if(i)label=strtok(nullptr,",");char *value=strtok(nullptr,",");if(!label||!value)return;
    strncpy(labels[i],label,3);values[i]=atol(value);}
  memcpy(customLabels805,labels,sizeof(labels));memcpy(customValues805,values,sizeof(values));
  modGrid806=false;customGrid805=true;displayDirty=true;renderDisplays(false);
}
#include "GrooveView16.h"
// Inline pixels preserve custom LCD glyphs without ambiguous CGRAM slot numbers.
bool sendGlyph37(const char* screen,uint8_t row,uint8_t col,uint8_t ch){
 uint8_t glyph[8];const uint8_t* source=nullptr;int index=screen[0]>='A'&&screen[0]<='D'?screen[0]-'A':-1;
 if(index>=0&&presentationActive43()){source=presentationGlyph43(ch);if(!source)return false;}else if(index>=0&&grooveGlyphs16[index]&&ch>=1&&ch<=6){
  uint8_t side=(ch-1)/3,strength=(ch-1)%3;
  for(uint8_t y=0;y<8;++y){uint8_t bits=oovGlyph16[strength][y];if(!side){uint8_t reverse=0;for(uint8_t x=0;x<5;++x)reverse|=((bits>>x)&1)<<(4-x);bits=reverse;}glyph[y]=bits;}source=glyph;
 }else if(index>=0&&row==1&&modGrid806&&graphs36[index].valid&&!strcmp(graphs36[index].header,modRows806[index][0])&&(col==graphs36[index].col||col==graphs36[index].col+1)){
  source=graphs36[index].pixels[col-graphs36[index].col];
 }  else if(index>=0&&row==0&&modGrid806&&(indMask50[index]&(1u<<col))){glyph902(modRows806[index][0][col],glyph);source=glyph;}else if(ch>=0x12&&ch<=0x14)source=noteGlyph927[ch-0x12];
 else if(underline927(ch)){memcpy(glyph,ch>=0x90?noteGlyph927[ch-0x90]:digitGlyph927[ch-0x80],8);glyph[7]=31;source=glyph;}
 else if(quarter937(ch)){memcpy(glyph,digitGlyph927[ch-0xA0],8);glyph[7]=31;source=glyph;}
 if(!source)return false;
 Serial.print("%G");const char*hex="0123456789ABCDEF";for(uint8_t y=0;y<8;++y){Serial.write(hex[(source[y]&31)>>4]);Serial.write(hex[source[y]&15]);}return true;
}

void screenMapInvalidateGlyphs(){memset(indGlyph50,0,sizeof(indGlyph50));for(auto&g:graphs36)g.dirty=true;memset(smallInverse902,0,sizeof(smallInverse902));memset(cursorCode932,0,sizeof(cursorCode932));memset(grooveGlyphs16,0,sizeof(grooveGlyphs16));presetInverse07=0;}
char moveText40[12]="0";
void renderGridScreen(uint8_t screenIndex) {
  if(renderOov16(screenIndex))return;
  if(remixVisible910){lcdPrintLine(screenIndex,0,remixRows910[screenIndex][0]);lcdPrintLine(screenIndex,1,remixRows910[screenIndex][1]);return;}
  if(modGrid806){
    uint8_t used50=(1u<<0)|(1u<<6)|(1u<<7);auto &graph50=graphs36[screenIndex];if(graph50.valid&&!strcmp(graph50.header,modRows806[screenIndex][0])){uint8_t a,b;graphSlots36(screenIndex,a,b);used50|=(1u<<a)|(1u<<b);}if(screenIndex==2&&(uint8_t)modRows806[2][0][11]>=0xC0)used50|=1u<<2;
    for(unsigned row=0;row<2;++row){char shown[17];memcpy(shown,modRows806[screenIndex][row],17);
      if(row==0)for(uint8_t col=0;col<16;++col)if(indMask50[screenIndex]&(1u<<col)){uint8_t slot=1;while(slot<6&&(used50&(1u<<slot)))++slot;if(slot==6)continue;used50|=1u<<slot;char c=shown[col];if(indGlyph50[screenIndex][slot]!=c){uint8_t glyph[8];glyph902(c,glyph);auto&lcd=screens[screenIndex];if(lcd.present){lcdCommand(lcd,0x40|slot*8);for(auto v:glyph)lcdSend(lcd,v,true);}indGlyph50[screenIndex][slot]=c;lcd.cacheValid[0]=false;}shown[col]=slot;}

      // A marked preset uses a separate glyph from the live envelope phase.
      if(screenIndex==2&&row==0&&(uint8_t)shown[11]>=0xC0){
        char original=shown[11]&0x7F;
        if(presetInverse07!=original){uint8_t glyph[8];glyph902(original,glyph);auto&lcd=screens[screenIndex];if(lcd.present){lcdCommand(lcd,0x50);for(unsigned r=0;r<8;++r)lcdSend(lcd,glyph[r],true);}presetInverse07=original;lcd.cacheValid[0]=false;}
        shown[11]=2;
      }auto&g36=graphs36[screenIndex];if(row==1&&g36.valid&&!strcmp(g36.header,modRows806[screenIndex][0])){uint8_t a,b;graphSlots36(screenIndex,a,b);shown[g36.col]=a;shown[g36.col+1]=b;shown[g36.col+2]=shown[g36.col+3]=' ';}lcdPrintLine(screenIndex,row,shown);if(row==1)graphUpload36(screenIndex);
    }return;
  }
  if(customGrid805){char upper[17],lower[17];memset(upper,' ',16);memset(lower,' ',16);upper[16]=lower[16]=0;
    for(uint8_t i=0;i<4;++i){uint8_t n=screenIndex*4+i;memcpy(upper+i*4,customLabels805[n],3);char val[16];
      int32_t v=customValues805[n];if(v==-32768)strcpy(val,"---");else if(v>=1000)snprintf(val,sizeof(val),"%ldk%ld",(long)(v/1000),(long)((v%1000)/100));
      else if(v<=-100)snprintf(val,sizeof(val),"-%ldh",(long)(-v/100));else snprintf(val,sizeof(val),"%3ld",(long)v);
      memcpy(lower+i*4,val,3);}
    lcdPrintLine(screenIndex,0,upper);lcdPrintLine(screenIndex,1,lower);return;
  }
  if (grooveListVisible) {
    char upper[17] = "                ", lower[17] = "                ";
    bool stopped = false;
    for (uint8_t i = 0; i < 16; ++i) {
      char a[5] = "", b[5] = "";
      if (i < 14) {
        int sheet = grooveListSheet[i], repeats = grooveListRepeats[i];
        if (stopped || sheet < 0 || repeats < 0) { strcpy(a, "---"); strcpy(b, "---"); }
        else {
          snprintf(a, sizeof(a), "%c%02u", (grooveRepeatEditing & (1u << i)) ? 'X' : 'x', (unsigned)repeats);
          if (!sheet) strcpy(b, "OFF");
          else snprintf(b, sizeof(b), "%c%u ", 'A' + (sheet - 1) / 9, (unsigned)((sheet - 1) % 9 + 1));
        }
        if (!sheet) stopped = true;
      }
      if (i / 4 == screenIndex && i < 14) {
        if(grooveSelection805&(1u<<i))a[0]='>';
        memcpy(upper + (i % 4) * 4, a, 3);
        memcpy(lower + (i % 4) * 4, b, 3);
      }
    }
    if (screenIndex == 3) { memcpy(upper + 8, "ListAdd ", 8);char id[5];snprintf(id,sizeof(id),"%02u",grooveList35);memcpy(lower+8,id,2); }
    lcdPrintLine(screenIndex, 0, upper);
    lcdPrintLine(screenIndex, 1, lower);
    return;
  }
  if (helpActive && screenIndex == helpKyIndex / 4) {
    char line0[17], line1[17];
    copyText(line0, sizeof(line0), helpName);
    copyText(line1, sizeof(line1), strlen(helpName) > 16 ? helpName + 16 : "");
    lcdPrintLine(screenIndex, 0, line0);
    lcdPrintLine(screenIndex, 1, line1);
    return;
  }
  char field[4][12];
  char gateSign937=0;
  char valueLine[17];
  uint8_t first = screenIndex * 4;
  for (uint8_t i = 0; i < 4; i++) {
    int8_t dataIndex = GRID_DISPLAY_FIELD[first + i];
    if (dataIndex < 0) copyText(field[i], sizeof(field[i]), "   ");
    else if (!grooveEditing && !grooveAssigning && valueOverrideIndex == dataIndex && !(dataIndex==0 && trackNav815==3)) copyText(field[i], sizeof(field[i]), valueOverrideText);
    else formatGridField((uint8_t)dataIndex, field[i]);
    if(dataIndex>=2&&dataIndex<=5&&noteTexts08[dataIndex-2][0]&&!grooveEditing&&!grooveAssigning)copyText(field[i],sizeof(field[i]),noteTexts08[dataIndex-2]);
    if(dataIndex==4&&(field[i][0]=='+'||field[i][0]=='-')){gateSign937=field[i][0];memmove(field[i],field[i]+1,strlen(field[i]));}
    if(dataIndex==4||dataIndex==5||dataIndex==8||dataIndex==9){int v=valueOverrideIndex==dataIndex?atoi(valueOverrideText):gridValue[dataIndex];if(!grooveEditing&&v&&v%384==0){for(uint8_t k=0;k<3;++k)if(field[i][k]>='0'&&field[i][k]<='9'){field[i][k]=(char)(0xA0+field[i][k]-'0');break;}}}
  }
  if(screenIndex==1&&!grooveEditing){copyText(field[2],sizeof(field[2]),moveText40);if(strlen(moveText40)>4)snprintf(field[2],sizeof(field[2]),"%c...",moveText40[0]);}
  if(screenIndex==2&&stepLayoutVisible26)snprintf(field[0],sizeof(field[0]),"%u",stepLayoutValue26);
  snprintf(valueLine, sizeof(valueLine), "%4.4s%4.4s%4.4s%4.4s",
           field[0], field[1], field[2], field[3]);
  const char* labelLine = LCD_LABELS[screenIndex];
  if(screenIndex==2&&stepLayoutVisible26)labelLine="TRIX MOD RST RSP";
  if(screenIndex==0&&trackPreview811)labelLine="PRK CHA NOT VEL";
  if(screenIndex==0&&trackNav815==3)labelLine="VOL CHA NOT VEL";
  if (grooveEditing) labelLine = screenIndex == 0 ? "Oov --- NOT VEL" : screenIndex == 1 ? "GAT TIM MOV PBN" : "--- --- --- ---";
  else if (grooveAssigning) labelLine = screenIndex == 0 ? "Oov --- --- ---" : screenIndex==1?"--- --- MOV ---":"--- --- --- ---";
  char labels937[17];copyText(labels937,17,labelLine);if(screenIndex==1&&gateSign937){labels937[0]='G';labels937[1]='A';labels937[2]=gateSign937;}
  lcdPrintLine(screenIndex, 0, labels937);
  lcdPrintLine(screenIndex, 1, valueLine);
}

void renderDebugDisplays(bool force = false) {
  if (debugScreenIdentity) {
    mainPrintLine(0, "SCREEN TEST", force);
    mainPrintLine(1, "MAIN I2C0 0x27", force);
    mainPrintLine(2, "SMALL I2C1 x4", force);
    mainPrintLine(3, "0123456789ABCDEFGHIJ", force);
    for (uint8_t screenIndex = 0; screenIndex < LCD_COUNT; screenIndex++) {
      char identity[17];
      snprintf(identity,sizeof(identity),"E%u / SCR MAP",screenIndex+1);
      lcdPrintLine(screenIndex, 0, identity, force);
      lcdPrintLine(screenIndex, 1, "0123456789ABCDEF", force);
    }
    return;
  }

  for (uint8_t row = 0; row < 4; row++) mainPrintLine(row, debugMainLine[row], force);
  for (uint8_t screenIndex = 0; screenIndex < LCD_COUNT; screenIndex++) {
    lcdPrintLine(screenIndex, 0, debugSmallLine[0], force);
    lcdPrintLine(screenIndex, 1, debugSmallLine[1], force);
  }
}

#include "Presentation43.h"
void renderDisplays(bool force = false) {
  uint32_t now = millis();
  if(saveAlert08){static uint32_t blink08=UINT32_MAX;uint32_t phase08=now/400;if(force||phase08!=blink08){blink08=phase08;for(uint8_t i=0;i<4;++i){mainPrintLine(i,phase08%2?"SAVE ERROR":"",true);for(uint8_t row=0;row<2;++row)lcdPrintLine(i,row,phase08%2?"SAVE ERROR":"",true);}}return;}
  if (!force && !displayDirty) return;
  lastRenderMs = now;
  displayDirty = false;
  uint8_t mask=force?31:renderMask931;renderMask931=0;if(!mask)mask=31;

  if (debugDisplayActive&&!busyVisible910) {
    renderDebugDisplays(force);
    return;
  }

  for (uint8_t screenIndex = 0; screenIndex < LCD_COUNT; screenIndex++) {
    if(!presentation43.mode&&(mask&(1u<<screenIndex)))renderGridScreen(screenIndex);
  }
  if(mask&16)renderMainScreen(force);
}

void handleGrid(char* line) {
  memset(lockBlink11,0,sizeof(lockBlink11));
  memset(lockBlink11,0,sizeof(lockBlink11));memset(noteTexts08,0,sizeof(noteTexts08));customGrid805 = false; modGrid806 = false;
  valueOverrideIndex = -1;
  char* token = strtok(line, ",");
  for (uint8_t i = 0; i < 16; i++) {
    token = strtok(nullptr, ",");
    if (!token) return;
    gridValue[i] = atol(token);
  }
  token=strtok(nullptr,",");trackPreview811=token&&atoi(token)!=0;
  token=strtok(nullptr,",");trackNav815=token?(uint8_t)atoi(token):255;
  displayDirty = true;
  // Un cran KY correspond a un paquet GRID. Le rendre ici empeche plusieurs
  // paquets deja presents dans l'UART d'etre fusionnes en une seule image.
  // Le cache LCD limite l'ecriture a la ligne dont la valeur a change.
  serviceBlink821();
  renderDisplays();
}

int8_t valueFieldForName(const char* name) {
  if (!name) return -1;
  if (strncmp(name, "IN ", 3) == 0) name += 3;
  if (strcmp(name, "TRK") == 0) return 0;
  if (strcmp(name, "CHA") == 0) return 1;
  if (strcmp(name, "NOTE") == 0 || strcmp(name, "PITCH") == 0) return 2;
  if (strcmp(name, "VEL") == 0) return 3;
  if (strcmp(name, "GAT") == 0 || strcmp(name, "GATE") == 0) return 4;
  if (strcmp(name, "TIM") == 0 || strcmp(name, "TIME") == 0) return 5;
  if (strcmp(name, "PBN") == 0) return 6;
  if (strcmp(name, "BPM") == 0) return 7;
  return -1;
}

void handleValue(char* line) {
  char* token = strtok(line, ",");
  char* name = strtok(nullptr, ",");
  char* value = strtok(nullptr, ",");
  int8_t field = valueFieldForName(name);
  if (field < 0 || !value) return;
  valueOverrideIndex = field;
  copyText(valueOverrideText, sizeof(valueOverrideText), value);
  displayDirty = true;
  serviceBlink821();
  renderDisplays();
}

void handleField(char* line) {
  strtok(line, ",");
  char* indexText = strtok(nullptr, ",");
  char* type = strtok(nullptr, ",");
  char* value = strtok(nullptr, ",");
  if (!indexText || !type || !value) return;
  int index = atoi(indexText);
  if (index < 0 || index >= 16) return;
  // A newer FIELD supersedes the formatted text from the preceding full GRID.
  if(index>=2&&index<=5)noteTexts08[index-2][0]='\0';
  if(index==0&&type[0]=='V'){trackNav815=3;trackVolume818=constrain(atoi(value),0,127);valueOverrideIndex=-1;displayDirty=true;renderDisplays();return;}
  if (type[0] == 'N') {
    gridValue[index] = atol(value);if(index==15)rollVex1000=gridValue[index];
    if (valueOverrideIndex == index) valueOverrideIndex = -1;
  } else {
    valueOverrideIndex = (int8_t)index;
    copyText(valueOverrideText, sizeof(valueOverrideText), value);
  }
  displayDirty = true;
  serviceBlink821();
  renderDisplays();
}

void handleBpm(char* line) {
  char* token = strtok(line, ",");
  token = strtok(nullptr, ","); // selection
  token = strtok(nullptr, ","); // bpm
  if (token) gridValue[7] = atoi(token);
  token = strtok(nullptr, ","); // longueur 1..64 mesures
  if (token) beatBars = (uint8_t)constrain(atoi(token), 1, 64);
  token = strtok(nullptr, ","); // offset
  if (token) gridValue[14] = atoi(token);
  token = strtok(nullptr, ","); // 0=start, 1=end
  if (token) {loopModes817=atoi(token);loopShowsEnd=false;}
  token = strtok(nullptr, ","); // loop start
  if (token) loopStartValue = (uint16_t)constrain(atoi(token), 1, 1024);
  token = strtok(nullptr, ","); // loop end
  if (token) loopEndValue = (uint16_t)constrain(atoi(token), 1, 1024);
  token = strtok(nullptr, ","); // pattern
  if (token) patternValue = (uint8_t)atoi(token);
  token = strtok(nullptr, ","); // mesure ou segment de vue, jusqu'a 2048
  if (token) beatPosition = max((uint32_t)1, (uint32_t)strtoul(token, nullptr, 10));
  token = strtok(nullptr, ","); // 0=position, 1=longueur
  if (token) beatEditLength = atoi(token) != 0;
  token = strtok(nullptr, ","); // zoom -2..5
  if (token) zoomLevel = (int8_t)constrain(atoi(token), -2, 5);
  token = strtok(nullptr, ","); // 0=BPM, 1=zoom
  if (token) roy1EditsZoom = atoi(token) != 0;
  displayDirty = true;
  serviceBlink821();
  renderDisplays();
}

void handleMode(char* line) {
  strtok(line, ",");
  char* field = strtok(nullptr, ",");
  char* mode = strtok(nullptr, ",");
  if (!field || !mode || strcmp(field, "TIME") != 0) return;
  timeLabelMove = strcmp(mode, "MOV") == 0;
  displayDirty = true;
  serviceBlink821();
  renderDisplays();
}

void handleModeLeds(char* line) {
  strtok(line, ",");
  bool states[6] = {};
  uint8_t count = 0;
  for (char* value = strtok(nullptr, ","); value; value = strtok(nullptr, ",")) {
    if (count >= 6 || (strcmp(value, "0") && strcmp(value, "1"))) return;
    states[count++] = value[0] == '1';
  }
  // Accept legacy 2/4-field frames; absent outputs are explicitly off.
  if (count != 2 && count != 4 && count != 6) return;
  songLedBase932=states[5];
  for (uint8_t i = 0; i < 6; ++i)
    digitalWrite(MODE_LED_PINS[i], states[i] ? HIGH : LOW);
}

void handleBarPulse() {
  uint16_t shownLoop = loopShowsEnd ? loopEndValue : loopStartValue;
  bool atMeasureBoundary = loopShowsEnd ? (shownLoop % 16u) == 0
                                        : ((shownLoop - 1u) % 16u) == 0;
  if (!atMeasureBoundary || menuActive) return;
  loopBoundaryFlashOn = true;
  loopBoundaryFlashUntilMs = millis() + LOOP_BOUNDARY_FLASH_MS;
  displayDirty = true;
  serviceBlink821();
  renderDisplays();
}

void handleHelp(char* line) {
  char* token = strtok(line, ",");
  token = strtok(nullptr, ",");
  if (!token) return;
  if (strcmp(token, "OFF") == 0) {
    helpActive = false;
  } else {
    helpKyIndex = (uint8_t)constrain(atoi(token), 0, 15);
    token = strtok(nullptr, "");
    copyText(helpName, sizeof(helpName), token ? token : "");
    helpActive = true;
  }
  displayDirty = true;
}

void handleUi(char* line) {
  scrollRow821=255;
  memset(menuBlink821,0,sizeof(menuBlink821));blinkHidden821=false;blinkAt821=millis();
  char* payload = line + 3;
  if (strcmp(payload, "GAME") == 0) {
    menuActive = false;
    debugDisplayActive = false;
    debugScreenIdentity = false;
    invalidateMainScreenCache();
    displayDirty = true;
    return;
  }
  // Champs vides conserves; rejeter une trame incomplete sans toucher a l'ecran.
  char* fields[4] = {payload, nullptr, nullptr, nullptr};
  for (uint8_t row = 1; row < 4; row++) {
    char* separator = strchr(fields[row - 1], '|');
    if (!separator) return;
    *separator = '\0';
    fields[row] = separator + 1;
  }
  menuActive = true;
  debugDisplayActive = false;
  debugScreenIdentity = false;
  for (uint8_t row = 0; row < 4; row++) {
    copyText(menuLine[row], sizeof(menuLine[row]), fields[row]);
  }
  invalidateMainScreenCache();
  displayDirty = true;
}

void handleUiBlink821(char* line) {
  if(strlen(line)<28||line[24]!='|')return;
  uint32_t masks[4]={};
  for(uint8_t i=0;i<20;++i){char c=line[4+i];uint8_t v;
    if(c>='0'&&c<='9')v=c-'0';else if(c>='A'&&c<='F')v=c-'A'+10;else return;
    masks[i/5]=(masks[i/5]<<4)|v;
  }
  unsigned separators=0;for(char*p=line+25;*p;++p)if(*p=='|')++separators;
  if(separators!=3)return;
  handleUi(line+22);memcpy(menuBlink821,masks,sizeof(masks));
}

void handleScrollText821(char* line){
  if(!menuActive)return;char *a=line+11,*b=strchr(a,'|');if(!b)return;
  char*c=strchr(b+1,'|');if(!c)return;*b=0;*c=0;
  unsigned row=atoi(a),offset=atoi(b+1);size_t length=strlen(c+1);
  if(row>=4||length>=sizeof(scrollText821)||offset>(length>20?length-20:0))return;
  strcpy(scrollText821,c+1);scrollRow821=row;scrollOffset821=offset;displayDirty=true;
}
void handleScrollOffset821(char* line){
  if(!menuActive||scrollRow821>=4)return;int offset=atoi(line+10);int maximum=max(0,(int)strlen(scrollText821)-20);
  scrollOffset821=constrain(offset,0,maximum);displayDirty=true;
}

void handleMessage(char* line) {
  char* separator = strchr(line, '|');
  if (!separator) return;
  copyText(messageText, sizeof(messageText), separator + 1);
  messageActive = true;
  messageUntilMs = millis() + 1800;
  displayDirty = true;
}

void handleDebugDisplay(char* line) {
  char* payload = line + 4;
  if (strcmp(payload, "OFF") == 0) {
    debugDisplayActive = false;
    debugScreenIdentity = false;
    displayDirty = true;
    return;
  }
  char* fields[6] = {payload, nullptr, nullptr, nullptr, nullptr, nullptr};
  for (uint8_t row = 1; row < 6; row++) {
    char* separator = strchr(fields[row - 1], '|');
    if (!separator) return;
    *separator = '\0';
    fields[row] = separator + 1;
  }
  debugDisplayActive = true;
  debugScreenIdentity = false;
  for (uint8_t row = 0; row < 4; row++) {
    copyText(debugMainLine[row], sizeof(debugMainLine[row]), fields[row]);
  }
  for (uint8_t row = 0; row < 2; row++) {
    copyText(debugSmallLine[row], sizeof(debugSmallLine[row]), fields[row + 4]);
  }
  invalidateMainScreenCache();
  displayDirty = true;
}

void handleDebugScreens() {
  debugDisplayActive = true;
  debugScreenIdentity = true;
  displayDirty = true;

  Serial.print(F("PICO_LCD_TEST,MAIN="));
  Serial.print(mainLcdPresent ? F("OK") : F("MISS"));
  TEE_SERIAL.print(F("PICO_LCD_TEST,MAIN="));
  TEE_SERIAL.print(mainLcdPresent ? F("OK") : F("MISS"));
  for (uint8_t i = 0; i < LCD_COUNT; i++) {
    Serial.print(F(",LCD")); Serial.print(i + 1); Serial.print('=');
    Serial.print(screens[i].present ? F("OK") : F("MISS"));
    TEE_SERIAL.print(F(",LCD")); TEE_SERIAL.print(i + 1); TEE_SERIAL.print('=');
    TEE_SERIAL.print(screens[i].present ? F("OK") : F("MISS"));
  }
  Serial.println();
  TEE_SERIAL.println();
}

void handleGroove(char* line) {
  char *token = strtok(line, ",");
  int values[9]; bool common[5] = {true,true,true,true,true};
  for (uint8_t i = 0; i < 9; i++) {
    token = strtok(nullptr, ","); if (!token) return;
    if (i >= 3 && i <= 7 && strcmp(token, "---") == 0) common[i-3] = false;
    values[i] = atoi(token);
  }
  grooveListVisible = values[0] != 0 && values[1] != 0;
  grooveEditing = values[0] != 0; grooveAssigning = values[1] != 0;
  grooveSheet = (uint8_t)constrain(values[2], 1, 144);
  for (uint8_t i = 0; i < 5; i++) { grooveValues[i] = constrain(values[i+3], i==2?0:-99, i==2?1000:99); grooveCommon[i] = common[i]; }
  grooveAssignment = constrain(values[8], -1, 145);
  valueOverrideIndex = -1;
  displayDirty = true;
  serviceBlink821();
  renderDisplays();
}

void handleGrooveList(char* line) {
  strtok(line, ",");
  char* token = strtok(nullptr, ",");
  if (!token) return;
  uint16_t mask = (uint16_t)atoi(token);
  int16_t sheets[14]; int8_t repeats[14];
  for (uint8_t i = 0; i < 14; ++i) {
    token = strtok(nullptr, ","); if (!token) return;
    int sheet = atoi(token); if (sheet < -1 || sheet > 144) return;
    sheets[i] = sheet;
    token = strtok(nullptr, ","); if (!token) return;
    int repeat = atoi(token); if (repeat < -1 || repeat > 99) return;
    repeats[i] = repeat;
  }
  memcpy(grooveListSheet, sheets, sizeof(sheets));
  memcpy(grooveListRepeats, repeats, sizeof(repeats));
  grooveRepeatEditing = mask;
  displayDirty = true;
}

void handleGrooveDelta(char* line) {
  int field, delta;
  if (sscanf(line, "GDELTA,%d,%d", &field, &delta) != 2 || field < 0 || field > 4) return;
  grooveDeltaField = field;
  grooveDeltaValue = constrain(delta, -198, 198);
  grooveDeltaUntil = millis() + 700;
  displayDirty = true;
}

void parseUartLine(char* line) {
  if(presentationCommand43(line))return;
  if(screenMapCommand(line))return;
  if(graphCommand36(line))return;
  rxCount++;
  renderMask931|=displayWork931(line);
  if(!strncmp(line,"DEF55,",6)){int id=atoi(line+6);char*text=strchr(line+6,',');deletePreview55=id>=0&&id<16&&text?id:-1;memset(deleteText55,' ',4);deleteText55[4]=0;if(deletePreview55>=0)memcpy(deleteText55,text+1,min((size_t)4,strlen(text+1)));renderMask931=31;displayDirty=true;return;}
  if(!strcmp(line,"HOME55")){busyVisible910=remixVisible910=false;messageActive=helpActive=false;deletePreview55=-1;char home55[]="UI|GAME";handleUi(home55);renderMask931=31;renderDisplays(true);TEE_SERIAL.println("HOME55,READY");return;}
  if(parseUi910(line))return;
  if(!strncmp(line,"ROLX1000,",9)){sscanf(line+9,"%d,%d,%d",&rollVex1000,&rollRelative1000,&rollVel1000);displayDirty=true;return;}
  if(!strncmp(line,"ROLLMOD825,",11)){rollMod825=atoi(line+11)!=0;displayDirty=true;}
  else if(strncmp(line,"GLSEL,",6)==0){grooveSelection805=atoi(line+6);displayDirty=true;}

  else if(strncmp(line,"PRESS,",6)==0){unsigned index,duration;if(sscanf(line+6,"%u,%u",&index,&duration)==2)press902(index,duration);}
  else if(strncmp(line,"IND50,",6)==0){unsigned screen,mask;char tail;if(sscanf(line,"IND50,%u,%u%c",&screen,&mask,&tail)==2&&screen<4&&mask<=65535){indMask50[screen]=mask;screens[screen].cacheValid[0]=false;displayDirty=true;renderMask931|=1u<<screen;}}
  else if (strncmp(line,"MODLCD|",7)==0) handleModLcd806(line);
  else if (strncmp(line,"CUSTOM,",7)==0) handleCustom805(line);
  else if (strncmp(line,"OOV16,",6)==0) handleOov16(line);
  else if(!strncmp(line,"MOV40,",6)){copyText(moveText40,sizeof(moveText40),line+6);displayDirty=true;}
  else if (strncmp(line, "GROOVE,", 7) == 0) handleGroove(line);
  else if (strncmp(line, "GLIST,", 6) == 0) handleGrooveList(line);
  else if (strncmp(line, "GDELTA,", 7) == 0) handleGrooveDelta(line);
  else if(!strncmp(line,"VOL818,",7)){char* v=strtok(line+7,",");if(v)trackVolume818=constrain(atoi(v),0,127);v=strtok(nullptr,",");trackHeld818=v&&atoi(v);displayDirty=true;return;}
  else if(!strncmp(line,"EDIT60,",7)){unsigned a,b,c,d;if(sscanf(line+7,"%u,%u,%u,%u",&a,&b,&c,&d)==4){editBlink60[0]=a;editBlink60[1]=b;editBlink60[2]=c;editBlink60[3]=d;displayDirty=true;renderMask931|=15;}}
  else if(!strncmp(line,"BLK11,",6)){unsigned s,m;if(sscanf(line+6,"%u,%u",&s,&m)==2&&s<4){lockBlink11[s]=m;displayDirty=true;renderMask931|=1u<<s;}}
  else if (strncmp(line, "GRID,", 5) == 0) handleGrid(line);
  else if (strncmp(line, "FIELD,", 6) == 0) handleField(line);
  else if (strncmp(line, "VAL,", 4) == 0) handleValue(line);
  else if (strncmp(line, "BPM,", 4) == 0) handleBpm(line);
  else if (strncmp(line, "MODE,", 5) == 0) handleMode(line);
  else if(strncmp(line,"SYNKOZ,",7)==0){synkoz933=atoi(line+7)!=0;renderMask931|=16;displayDirty=true;}
  else if(strncmp(line,"SONGWARN,",9)==0){songWarnStart932=millis();songWarnDuration932=strtoul(line+9,nullptr,10);displayDirty=true;}
  else if (strncmp(line, "MODELEDS,", 9) == 0) handleModeLeds(line);
  else if (strcmp(line, "BAR") == 0){presentationBeat43();handleBarPulse();}
  else if (strncmp(line, "HELP,", 5) == 0) handleHelp(line);
  else if(!strncmp(line,"SCROLLTEXT|",11))handleScrollText821(line);
  else if(!strncmp(line,"SCROLLOFF,",10))handleScrollOffset821(line);
  else if (strncmp(line,"UIB|",4)==0) handleUiBlink821(line);
  else if (strncmp(line, "UI|", 3) == 0) handleUi(line);
  else if(!strncmp(line,"ALERT08|",8)){saveAlert08=strcmp(line+8,"OFF")!=0;displayDirty=true;renderMask931=31;renderDisplays(true);}
  else if(!strncmp(line,"SD08,",5)){sdReady08=atoi(line+5)!=0;displayDirty=true;renderMask931|=16;}
  else if(!strncmp(line,"EDIT08|",7)){char*ctx=nullptr;char*p=strtok_r(line+7,"|",&ctx);for(uint8_t i=0;i<4;++i){copyText(noteTexts08[i],12,p?p:"");p=p?strtok_r(nullptr,"|",&ctx):nullptr;}displayDirty=true;renderMask931|=3;}
  else if (strncmp(line, "MSG|", 4) == 0) handleMessage(line);
  else if (strncmp(line, "DBG|", 4) == 0) handleDebugDisplay(line);
  else if (strcmp(line, "DBGSCREENS") == 0) handleDebugScreens();
  else if (strcmp(line, "RST|EP") == 0) recoverMainLcd(true);

  // Pas d'accuse par paquet : la liaison reste disponible pour touches et READY.
}

void serviceUart() {
  while (TEE_SERIAL.available() > 0) {
    char c = (char)TEE_SERIAL.read();
    if (c == '\r') continue;
    if (c == '\n') {
      uartLine[uartLinePos] = '\0';
      if (!uartDiscardLine && uartLinePos > 0) parseUartLine(uartLine);
      uartDiscardLine = false;
      uartLinePos = 0;
    } else if (uartDiscardLine) {
      continue;
    } else if (uartLinePos < sizeof(uartLine) - 1) {
      uartLine[uartLinePos++] = c;
    } else {
      uartLinePos = 0;
      uartDiscardLine = true;
    }
  }
}

char readKeypad() {
  for (uint8_t row = 0; row < 4; row++) {
    pinMode(KEYPAD_ROW_PINS[row], OUTPUT);
    digitalWrite(KEYPAD_ROW_PINS[row], LOW);
    delayMicroseconds(4);
    for (uint8_t col = 0; col < 4; col++) {
      if (digitalRead(KEYPAD_COL_PINS[col]) == LOW) {
        pinMode(KEYPAD_ROW_PINS[row], INPUT_PULLUP);
        return KEYPAD_KEYS[row][col];
      }
    }
    pinMode(KEYPAD_ROW_PINS[row], INPUT_PULLUP);
  }
  return 0;
}

void serviceKeypad() {
  char reading = readKeypad();
  uint32_t now = millis();
  if (reading != keypadCandidate) {
    keypadCandidate = reading;
    keypadCandidateSinceMs = now;
    return;
  }
  if (reading == keypadStable || (uint32_t)(now - keypadCandidateSinceMs) < KEYPAD_DEBOUNCE_MS) return;
  keypadStable = reading;
  if (keypadStable == 0) return;
  TEE_SERIAL.print(F("KEY,"));
  TEE_SERIAL.println(keypadStable);
  Serial.print(F("KEY,"));
  Serial.println(keypadStable);
}

void setup() {
  Serial.begin(115200);

  for (uint8_t i = 0; i < 6; ++i) {
    pinMode(MODE_LED_PINS[i], OUTPUT);
    digitalWrite(MODE_LED_PINS[i], LOW);
  }

  // Demarrer USB et l'UART avant tout acces I2C : meme avec un bus bloque,
  // le Pico reste identifiable et reprogrammable sans demontage.
  TEE_SERIAL.setTX(UART_TX_PIN);
  TEE_SERIAL.setRX(UART_RX_PIN);
  TEE_SERIAL.setFIFOSize(2048); // Absorber les trames pendant les ecritures LCD I2C.
  TEE_SERIAL.setFIFOSize(1024);
  TEE_SERIAL.begin(TEE_BAUD);
  delay(50);
  Serial.println(F("BOOT,UrbanKaoz_Picran_V1_DisplayRecorder_USB"));
  for (uint8_t i = 0; i < 6; ++i) digitalWrite(MODE_LED_PINS[i], HIGH);
  delay(1000);
  modeLedBootPassed = true;
  for (uint8_t i = 0; i < 6; ++i)
    if (digitalRead(MODE_LED_PINS[i]) != HIGH) modeLedBootPassed = false;
  for (uint8_t i = 0; i < 6; ++i) digitalWrite(MODE_LED_PINS[i], LOW);
  for (uint8_t i = 0; i < 6; ++i)
    if (digitalRead(MODE_LED_PINS[i]) != LOW) modeLedBootPassed = false;

  Wire.setSDA(MAIN_LCD_SDA_PIN);
  Wire.setSCL(MAIN_LCD_SCL_PIN);
  Wire.begin();
  Wire.setClock(100000);
  Wire.setTimeout(10, true);

  Wire1.setSDA(SMALL_LCD_SDA_PIN);
  Wire1.setSCL(SMALL_LCD_SCL_PIN);
  Wire1.begin();
  Wire1.setClock(100000); // plus robuste avec hub et cables en Y
  Wire1.setTimeout(10, true); // aucun cable/ecran ne peut bloquer le demarrage USB

  for (uint8_t address = 0x08; address <= 0x77; address++) {
    i2c1Found[address] = i2cResponds(Wire1, address);
  }

  for (uint8_t col = 0; col < 4; col++) pinMode(KEYPAD_COL_PINS[col], INPUT_PULLUP);
  for (uint8_t row = 0; row < 4; row++) pinMode(KEYPAD_ROW_PINS[row], INPUT_PULLUP);

  mainLcdPresent = i2cResponds(Wire, MAIN_LCD_ADDRESS);
  Serial.print(F("MAIN_LCD,I2C0,0x27,"));
  Serial.println(mainLcdPresent ? F("OK") : F("MISSING"));
  if (mainLcdPresent) {
    mainLcd.init();rhythmGlyphs923();memset(inverseOriginal904,0,sizeof(inverseOriginal904));
    mainLcd.backlight();
  }

  for (uint8_t i = 0; i < LCD_COUNT; i++) {
    screens[i].bus = &Wire1;
    screens[i].address = LCD_ADDRESS[i];
    screens[i].present = i2cResponds(Wire1, LCD_ADDRESS[i]);
    screens[i].cacheValid[0] = false;
    screens[i].cacheValid[1] = false;
    Serial.print(F("SMALL_LCD"));
    Serial.print(i + 1);
    Serial.print(F(",I2C1,0x"));
    Serial.print(LCD_ADDRESS[i], HEX);
    Serial.println(screens[i].present ? F(",OK") : F(",MISSING"));
    if (screens[i].present) {LcdScreen hw=screens[i];lcdInit(hw);}
    screenPhysicalPresent[i]=screens[i].present;
  }

  screenMapPresence();screenMapInvalidateGlyphs();
  TEE_SERIAL.println(F("PICO_LCD_READY,V1"));
  Serial.println(F("UKD1|HELLO|UrbanKaoz|8.21"));

  renderDisplays(true);
  Serial.println(F("UrbanKaoz Picran V1: display recorder telemetry on USB"));
  presentationStart43(1);
}

void serviceUsb931(){
 static char command[32];static uint8_t used=0;
 while(Serial.available()){char c=Serial.read();if(c=='\r')continue;if(c=='\n'){
  command[used]=0;if(!strcmp(command,"STATUS"))statusRequested931=true;
  else if(!strcmp(command,"SNAPSHOT")||!strcmp(command,"UKD1|SNAPSHOT"))snapshotRequested931=true;
  used=0;
 }else if(used+1<sizeof(command))command[used++]=c;else used=0;}
}
void loop() {
  screenIdentifyService26();
  static uint32_t usersPage26=0;if(grooveEditing&&millis()/2000!=usersPage26){usersPage26=millis()/2000;displayDirty=true;}

  if(songWarnDuration932){uint32_t elapsed=millis()-songWarnStart932;if(elapsed>=songWarnDuration932){songWarnDuration932=0;digitalWrite(SONG_LED_PIN,songLedBase932?HIGH:LOW);displayDirty=true;}else digitalWrite(SONG_LED_PIN,(elapsed/max(1UL,songWarnDuration932/32))%2?LOW:HIGH);}

  serviceBusy910();presentationService43();
  if(pressIndex902>=0&&(int32_t)(millis()-pressUntil902)>=0){pressIndex902=-1;renderMask931|=16;displayDirty=true;}

  if (grooveDeltaField >= 0 && (int32_t)(millis() - grooveDeltaUntil) >= 0) {
    grooveDeltaField = -1;
    displayDirty = true;
  }
  serviceUart();
  serviceKeypad();
  // No background I2C recovery scans or unsolicited full snapshots.
  serviceUsb931();

  if (messageActive && (int32_t)(millis() - messageUntilMs) >= 0) {
    messageActive = false;
    displayDirty = true;
  }

  if (loopBoundaryFlashOn && (int32_t)(millis() - loopBoundaryFlashUntilMs) >= 0) {
    loopBoundaryFlashOn = false;
    if (!menuActive) displayDirty = true;
  }

  if (statusRequested931) {statusRequested931=false;
    lastStatusMs = millis();
    Serial.print(F("STATUS,FW=V1,REV=VA01.60,SELECT="));
    Serial.print(digitalRead(SELECT_LED_PIN));
    Serial.print(F(",REMIX="));
    Serial.print(digitalRead(REMIX_LED_PIN));
    Serial.print(F(",PATTERN=")); Serial.print(digitalRead(PATTERN_LED_PIN));
    Serial.print(F(",GROOVE=")); Serial.print(digitalRead(GROOVE_LED_PIN));
    Serial.print(F(",DELETE=")); Serial.print(digitalRead(DELETE_LED_PIN));
    Serial.print(F(",SONG=")); Serial.print(digitalRead(SONG_LED_PIN));
    Serial.print(F(",BOOTLEDS=")); Serial.print(modeLedBootPassed ? F("PASS") : F("FAIL"));
    Serial.print(F(",RX="));
    Serial.print(rxCount);
    Serial.print(F(",MAIN="));
    Serial.print(mainLcdPresent ? F("OK") : F("MISS"));
    for (uint8_t i = 0; i < LCD_COUNT; i++) {
      Serial.print(F(",LCD"));
      Serial.print(i + 1);
      Serial.print('=');
      Serial.print(screens[i].present ? F("OK") : F("MISS"));
    }
    Serial.print(F(",I2C1_FOUND="));
    bool firstAddress = true;
    for (uint8_t address = 0x08; address <= 0x77; address++) {
      if (!i2c1Found[address]) continue;
      if (!firstAddress) Serial.print('/');
      if (address < 0x10) Serial.print('0');
      Serial.print(address, HEX);
      firstAddress = false;
    }
    if (firstAddress) Serial.print(F("NONE"));
    Serial.println();
  }

  if (snapshotRequested931) {
    snapshotRequested931=false;
    sendUkdSnapshot();
  }

  serviceBlink821();
  renderDisplays();
}


