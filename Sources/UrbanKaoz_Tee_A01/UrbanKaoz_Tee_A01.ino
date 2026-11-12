// Livraison A01, affichage V1. Provenance : UrbanKaoz_Tee_V9_42.ino.
// CONSIGNE ROY : ne jamais publier le sequenceur, ses sources ou ses firmwares.
// VA01.06: independent keyboard surface, manual modulation source, signed zones.
// Etat de compilation/installation consigne dans A01/Documentation.
#include <Wire.h>
#include <IntervalTimer.h>
#include <Adafruit_seesaw.h>
#include <seesaw_neopixel.h>
#include <Adafruit_NeoTrellis.h>
#include <SD.h>
#include <EEPROM.h>
#include "ClockProbe05.h"
#include "Bench29State.h"
#include "Evolution933State.h"
#include "Evolution932State.h"
#include "SongState.h"
#include "MidiIn17State.h"
#include "Selection805State.h"
#include "Playback828State.h"
#include "Blocks21State.h"
#include "Mod805State.h"
#include "Names07State.h"
#include "Exp14.h"
#include "Pidi827State.h"
#include "Patch808State.h"
#include "Storage809State.h"
#include "Content18State.h"
#include "Realtime810State.h"
#include "Settings811State.h"
#include "Full805State.h"
#include "Diag814.h"
#include "Input815.h"
#include "Buttons816.h"
#include "Pc831State.h"
#include "PLocks817State.h"
#include "Ind50State.h"
#include "Automation46State.h"
#include "KeyboardR10State.h"
#include "TrackLink12State.h"
void diagShowNeo814(uint8_t n);
#include "Remix911State.h"
uint8_t clipboardSaveKind(uint8_t slot);
bool savePatternClipboardBundle(uint8_t slot, char *path, size_t size);
bool isPatternClipboardBundle(const char *path);
bool loadPatternClipboardBundle(const char *path, int8_t slot);

/*
  UrbanKaoz_Tee_V7.9_LiveHardwareSettings

  CHAPITRES V7.3
  1A1 - Selection directe d'une nouvelle note; la selection precedente est validee puis remplacee.
  1A2 - Une selection active place NOT en edition; toute sortie normale revient au choix de hauteur.
  1A3 - VEL/GATE/NOT/TIM et PX sont valides aux sorties normales; Annuler seul restaure l'ancien etat.
  1A4 - Chaque piste memorise position d'edition, ligne et zoom.
  1B1 - TIM reste un PX de lecture non destructif; MOV deplace la composition; aucun mode global x10.
  1B2 - TIM n'affiche et ne rend selectionnable qu'une seule tete logique.
  1B3 - WRITE TIM deplace chaque evenement existant une seule fois et retire son ancienne position.
  1C1 - Chaque premier geste force le rafraichissement ROL/TIM, BPM/ZOOM et zoom Neo.
  1C2 - Les evenements WRITE ROL restent adressables par leur tick logique exact.
  1C3 - Plusieurs occurrences de meme hauteur dans un pad = orange; une occurrence isolee = jaune.
  1D1 - La portee maximale de RST est 192 ticks.
  1E1 - Depuis V7.11, SD MISSING/WRITE ERROR affiche SD rouge sur Neo sans bloquer.
  1F1 - Un pattern vide selectionne devient immediatement actif et boucle silencieusement.
  1G1 - START/END ne flashent qu'aux valeurs debut/fin de mesure, une fois par debut de mesure.
  2B1 - Supprimer un pad orange supprime toutes ses occurrences; zoomer permet la suppression individuelle.

  CHAPITRES V7.4
  8A1 - Les six boutons d'edition sont SHIFT/UNDO/REDO/SELECT/SELECT ALL/DELETE.
  8A2 - SHIFT et SELECT sont des bascules, utilisables sur les notes et les patterns.
  8A3 - SELECT ajoute ou retire chaque pad; SHIFT selectionne une plage sur une meme piste.
  8A4 - SELECT ALL prend toutes les notes de la piste ou tous ses patterns.
  8A5 - Un pattern vide maintenu devient la cible du bouton PASTE.
  8A6 - Les quatre NeoPixel Roy executent une sequence de test au demarrage.

  CHAPITRES V7.5
  9A1 - SELECT et SHIFT sont exclusifs et un changement de mode recommence la selection.
  9A2 - LED Roy OA = SELECT, LED Roy OB = SHIFT.
  9A3 - Les temoins externes sont pilotes par Picran, pas par les Pico KY.
  9A4 - START/END sont des bornes globales limitees par la piste la plus longue.

  CHAPITRES V7.6
  10A1 - Picran designe exclusivement le Pico Grove qui gere tous les ecrans.
  10A2 - Picran GP4 = temoin SELECT et Picran GP5 = temoin SHIFT.
  10B1 - Tout menu stoppe la sequence et bloque les commandes non liees a sa navigation.
  10B2 - Aucun menu n'affiche un numero de page. Une suite est signalee par > sur le dernier item.
  10B3 - Sur toute page suivante, le premier item commence par <. Cette regle vaut pour les futures branches.
  10B4 - Retour restaure le niveau et la page exacts qui ont ouvert la branche courante.
  10C1 - DEBUG contient BOUTONS, diagnostic brut de chacun des 48 bits HC165.
  10C2 - L'ancien contournement B31/B23 est supprime apres remplacement du HC165 fautif.

  CHAPITRES V7.8
  11A1 - Les broches HC165 26/27/28 sont initialisees apres tous les autres peripheriques.
  11A2 - HCRAW relit directement les 48 bits; HCINIT reprend explicitement les broches.

  CHAPITRES V7.11
  15A1 - SD absente/non inscriptible : indication Neo SD rouge non bloquante.
  15A2 - La premiere note creee par un pad efface cette indication.
  15A3 - Menus, selection, debug et transport restent accessibles sans SD.

  CHAPITRES V7.10
  13A1 - SELECT reste actif jusqu'a validation SELECT ou annulation J1.
  13A2 - Les objets notes et les cellules vides sont selectionnes sans mutation.
  13A3 - Les notes de hauteurs differentes ont une identite de selection propre.
  13A4 - STOP B29 et PATTERN B31 retrouvent leurs actions de transport.
  13A5 - J1 quitte les diagnostics sauf le test KY23 lui-meme.

  CHAPITRES V7.9
  12A1 - Le peripherique teste ne sert jamais a naviguer, valider ou quitter son propre test.
  12A2 - Reglage BUTTON en direct : OA tourne, OA valide, OB annule; les 48 HC165 restent testables.
  12A3 - KY23 affiche deux cibles et deux clics; aucun clic joystick ne quitte le test.
  12A4 - Les reglages Roy sont saisis par KY40 et appliques au test avant validation.

  Nouvelle config stable :
  - 4 NeoTrellis I2C, ordre physique Neo 1 -> Neo 4 : 0x2F, 0x2E, 0x32, 0x30
  - Sens interne corrige : la progression va de droite a gauche dans chaque Neo.
  - Affichage delegue au Pico ecran via Serial2, Teensy pins 7/8
  - Identifiant HELLO historique conserve pour compatibilite avec le firmware Pico afficheur.
  - 1 Roy / Quad Rotary I2C : 0x49 (mapping P1..P4 conserve)
  - Pico1 <-> Teensy : Serial1, Pico1 P1/P2 vers Teensy 0/1
  - Pico2 <-> Teensy : Serial6, Pico2 P1/P2 vers Teensy 25/24
  - 16 KY-040 recus par UART depuis les 2 Pico, avec une fonction directe fixe :
    TRK/CHA/NOT/VEL, GAT/TIM/PBN/BPM, RST/RSP/RAC/REX, RCV/RLV/VST/VTR.
  - Nano P23 recu sur Serial5 / Teensy RX5 pin 21. J1 navigue dans les menus
    et les hauteurs compactes; J2 est neutralise hors diagnostic.
  - Digipad 4x4 recu depuis le Pico ecran : chiffres = saisie numerique,
    # = valider, * = annuler, A/B/C/D reserves.

  Modele V4.80 timing 384 + note length fixed at Note On :
  - 16 pistes fixes : Track 1 = MIDI CH1 ... Track 16 = MIDI CH16.
  - Les quatre Neo forment une seule grille chromatique : temps horizontal,
    notes verticales, uniquement pour la piste selectionnee.
  - Selection piste/pas uniquement par appui sur les Neo.
  - Appui long Neo : la couleur de selection change pendant le maintien, des que le seuil est atteint.
  - Appui KY ne selectionne plus de piste.
  - Aucun toggle rapide x10; GATE conserve ses modes MUS/24t/1t et TIM alterne avec MOV.
  - Rotation KY1 = piste, KY2 = canal MIDI, KY3 = Pitch Bend MIDI,
    KY4 = velocity, KY5 = gate, KY6 = timing.
  - Rotation KY1..KY4 editent par defaut la valeur globale de la piste selectionnee.
  - Si le pad du pas selectionne est maintenu pendant la rotation : edition uniquement de ce pas, type P-lock.
  - Les changements de valeur par defaut se propagent aux notes qui n'ont pas de P-lock pour ce parametre.
  - KY7..KY13 : rotation et clic ignores pour l'instant. KY14..KY16 = test transport temporaire.
  - Preview de note configurable globalement et par piste.
  - Version stabilite : debug Serial reduit, refresh Neo limite, valeurs par defaut non propagees en boucle dans tous les pas.
  - Les notes deja declenchees gardent leur note-off planifie, donc une edition de gate/pitch/vel/time ne modifie pas la note en cours.
  - Moteur timing repasse en PPQN 384 : 1 step = 96 ticks, microtiming limite a +/-48 ticks.
  - Note On + Note Off planifies des le declenchement de la note : la longueur est figee au debut de la note.
  - Une note deja en cours garde son Note Off prevu, meme si on edite Gate/Pitch/Vel/Time pendant qu elle sonne.
  - Snapshot de mesure V4.60 retire : les edits concernent les prochains declenchements, sans reconstruire un cycle de 4 mesures.
  - Fond blanc de piste selectionnee supprime volontairement : moins de LEDs allumees, moins de refresh inutile.
  - Mapping Pico corrige : index UART strictement 0..7, plus de conversion 1..8.
  - Roy1 = piste, Roy2 = position/longueur, Roy3 = boucle, Roy4 = pattern.
  - Pico ecran = remplace les 2 OLED, donnees simples envoyees par Serial2.

  Notes strictes :
  - Ancien Zoo supprime volontairement.
  - Anciens Roy 0x4C / 0x4D supprimes volontairement.
  - Ancien KY local sur 30/31/32 supprime volontairement.
  - Ne pas reintroduire ces fonctions sans validation explicite.

  MIDI DIN :
  - Serial1 est reserve au Pico1, donc il ne peut plus servir au MIDI DIN.
  - Pin 30 n'est pas utilisee comme TX UART MIDI dans ce sketch.
  - Configuration validee : MIDI OUT sur Serial8, TX8 = Teensy pin physique 35.
*/

// ============================================================
// CONFIG UART / MIDI
// ============================================================
#define PICO1_SERIAL Serial1
#define PICO2_SERIAL Serial6

#define MIDI_SERIAL Serial8
static const uint8_t MIDI_TX_PIN_INFO = 35; // info affichage seulement : Serial8 TX = Teensy pin physique 35

#include "Display821.h"
#define DISPLAY_SERIAL display821
static const uint8_t DISPLAY_TX_PIN_INFO = 8; // Serial2 TX = Teensy pin physique 8 vers Pico ecran P2/RX
static const uint8_t DISPLAY_RX_PIN_INFO = 7; // Serial2 RX = Teensy pin physique 7 depuis Pico ecran P1/TX, optionnel

#define P23_SERIAL Serial5
static const uint8_t P23_RX_PIN_INFO = 21; // Serial5 RX = Teensy pin physique 21 depuis Nano P23 TX

static const uint32_t PICO_BAUD = 115200;
static const uint32_t DISPLAY_BAUD = 38400;
static const uint32_t P23_BAUD = 115200;
static const uint32_t MIDI_BAUD = 31250;
void quickGateClick20(uint32_t held);
static const char APP_VERSION[] = "VA01.60";
static const char APP_BUILD_ID[] = "V8_31_PC_SURFACE";

// Reglages envoyes aux Pico au demarrage.
static const uint8_t PICO_DEFAULT_D = 4; // one value per full mechanical detent
static const uint8_t PICO_DEFAULT_R = 2; // anti-retour rotation en ms
static const uint8_t PICO_DEFAULT_B = 15; // filtre bouton en ms
// A mechanical KY detent has four quadrature transitions. Lower thresholds
// emit several packets for one notch, independently of the edited function.
static const uint8_t PICO_DETENT_MIN = 4;
static const uint8_t PICO_DETENT_MAX = 15;

// ============================================================
// MODE STABILITE
// ============================================================
static const bool DEBUG_SERIAL = false;
static const uint16_t DISPLAY_BPM_MIN_REFRESH_MS = 300;
static const uint16_t NEO_MIN_REFRESH_MS = 20;
static const uint8_t PICO_MAX_CHARS_PER_LOOP = 48;
static const uint16_t PICO_RX_EXTRA_SIZE = 512;
uint8_t pico1RxExtra[PICO_RX_EXTRA_SIZE];
uint8_t pico2RxExtra[PICO_RX_EXTRA_SIZE];
uint32_t lastDisplayValueRefreshMs = 0;
uint32_t lastDisplayBpmRefreshMs = 0;
uint32_t lastNeoRefreshMs = 0;


// ============================================================
// NeoTrellis
// ============================================================
static const uint8_t NEO_COUNT = 4;
// Ordre physique releve avec un code couleur fixe par adresse :
// Neo 1=vert/0x2F, Neo 2=rouge/0x2E, Neo 3=blanc/0x32, Neo 4=bleu/0x30.
static const uint8_t NEO_ADDR[NEO_COUNT] = {0x2F, 0x2E, 0x32, 0x30};

Adafruit_NeoTrellis neo1;
Adafruit_NeoTrellis neo2;
Adafruit_NeoTrellis neo3;
Adafruit_NeoTrellis neo4;
Adafruit_NeoTrellis* neo[NEO_COUNT] = {&neo1, &neo2, &neo3, &neo4};
bool neoAvailable[NEO_COUNT] = {false, false, false, false};
DMAMEM uint32_t neoColors821[4][16];
uint16_t neoValid821[4]={};bool neoPending821[4]={};
uint32_t neoWrites821=0,neoAvoided821=0;
void neoPixel821(uint8_t n,uint8_t pad,uint32_t color);
static const uint8_t NEO_TEST_ACTIVE_COUNT = NEO_COUNT;

enum DebugTestMode : uint8_t {
  DEBUG_TEST_NONE = 0,
  DEBUG_TEST_KY40,
  DEBUG_TEST_NEO,
  DEBUG_TEST_KY23,
  DEBUG_TEST_ROY,
  DEBUG_TEST_BE,
  DEBUG_TEST_BUTTONS,
  DEBUG_TEST_TRANSPORT,
  DEBUG_TEST_CLIPBOARD,
  DEBUG_TEST_SCREENS,
  DEBUG_TEST_KEYPAD,
  DEBUG_TEST_CV47
};

DebugTestMode debugTestMode = DEBUG_TEST_NONE;
int32_t debugRoyTestValue = 0;
uint8_t debugRoyTestEncoder = 0;
int32_t debugRoyTestDelta = 0;
bool debugRoyTestClick = false;
uint8_t debugNeoLastPad = 255;
uint8_t debugNeoLastIndex = 255;
uint32_t debugP23LastReportMs = 0;
int debugP23LastY1Dx = 32767;
int debugP23LastY1Dy = 32767;
int debugP23LastY2Dx = 32767;
int debugP23LastY2Dy = 32767;
bool debugP23LastY1Switch = false;
bool debugP23LastY2Switch = false;
static const uint8_t DEBUG_TEST_COUNT = 11;
uint8_t debugMenuPage = 0;
static const uint8_t KY40_CLK_PIN[8] = {2, 5, 8, 11, 14, 17, 20, 26};
static const uint8_t KY40_DT_PIN[8]  = {3, 6, 9, 12, 15, 18, 21, 27};
static const uint8_t KY40_SW_PIN[8]  = {4, 7, 10, 13, 16, 19, 22, 28};

// ============================================================
// PICO ECRAN / LCD 4x20 DELEGUE
// ============================================================
// Notes strictes :
// - Les 2 anciens OLED 0x3C/0x3D sont remplaces par un Pico ecran.
// - Le Tee n'envoie que des donnees simples sur Serial2.
// - Ne pas remettre de refresh OLED direct dans le moteur sans validation explicite.
bool displayValueDirty = true;
bool displayBpmDirty = true;

// Nano P23 : ligne ASCII DATA,Y1,x,y,dx,dy,sw,Y2,x,y,dx,dy,sw.
DMAMEM uint8_t p23RxExtra811[2048];
char p23Line[128] = {0};
uint8_t p23LinePos = 0;
static const int16_t P23_AXIS_ON = 120;
static const int16_t P23_AXIS_OFF = 70;
static const uint16_t P23_REPEAT_FIRST_MS = 350;
static const uint16_t P23_REPEAT_MS = 140;

struct P23AxisRepeat {
  int8_t direction;
  uint32_t nextActionMs;
};

P23AxisRepeat p23Y1X = {0, 0};
P23AxisRepeat p23Y1Y = {0, 0};
P23AxisRepeat p23Y2X = {0, 0};
P23AxisRepeat p23Y2Y = {0, 0};
bool p23Y1SwitchWasPressed = false;
bool p23Y2SwitchWasPressed = false;
bool p23ZoomArmed = false;
uint32_t p23ZoomNeutralSinceMs = 0;
uint32_t p23DebugBytes = 0;
uint32_t p23DebugLines = 0;
uint32_t p23DebugValidLines = 0;
uint32_t p23DebugLastReportMs = 0;
int p23DebugY1Dx = 0;
int p23DebugY2Dx = 0;
int p23DebugY2Dy = 0;
bool p23DebugY2Switch = false;

// ============================================================
// ROY / QUAD ROTARY I2C
// ============================================================
static const uint8_t ROY_ADDR = 0x49;
static const uint8_t ROY_ENCODER_COUNT = 4;

// Adafruit Quad Rotary : index 0=P1, 1=P2, 2=P3, 3=P4
static const uint8_t ROY_SWITCH_PIN[ROY_ENCODER_COUNT] = {12, 14, 17, 9};

Adafruit_seesaw roy;
static const uint8_t ROY_NEO_PIN = 18;
seesaw_NeoPixel royPixels(4, ROY_NEO_PIN, NEO_GRB + NEO_KHZ800);
bool royOk = false;
bool royPixelsReady = false;
bool shiftButtonActive = false;
bool selectButtonActive = false;
bool selectPadPending[4][16] = {};
static const uint16_t SELECT_VALIDATE_HOLD_MS = 600;
bool deleteButtonActive = false;
bool recordDelete51=false;
uint8_t patternOrder51[16][16],fullOrder51[16][16];bool orderReady51=false;
#include "PatternOrder51.h"
void patternDown51();
void quickGather903();
bool recordArmed823();
void navigationPress49(uint8_t);
const char* functionName932(uint8_t);
bool deletePatternContext = false;
// L'ordre NeoPixel est oppose aux lettres imprimees : OA est le dernier
// pixel de la sequence de demarrage, puis OB est le precedent.
static const uint8_t ROY_PIXEL_SELECT_OA = 3;
static const uint8_t ROY_PIXEL_SHIFT_OB = 2;

int32_t royEncoderPos[ROY_ENCODER_COUNT];
bool roySwStableState[ROY_ENCODER_COUNT];
bool roySwLastRawState[ROY_ENCODER_COUNT];
uint32_t roySwLastRawChangeMs[ROY_ENCODER_COUNT];
uint32_t roySwLastToggleMs[ROY_ENCODER_COUNT];
uint32_t royLastPollMs = 0;

static const uint16_t ROY_POLL_INTERVAL_MS = 5;
uint16_t roySwitchDebounceMs = 15;
uint16_t royFunctionGuardMs = 250;
uint8_t royPulsesPerValue = 1;
int8_t royRotationDirection = -1;
int8_t royRotationAccumulator[ROY_ENCODER_COUNT] = {0, 0, 0, 0};


// ============================================================
// BOUTONS - 9 x 74HC165 : 56 anciennes entrees + 16 nouvelles en fin de lecture.
// ============================================================
// Cablage valide : SH/LD=Tee26, CLK=Tee27, DATA=Tee28.
// Cartes en pull-up : relache=1, appuye=0.
static const uint8_t HC165_PIN_LATCH = 26;
static const uint8_t HC165_PIN_CLOCK = 27;
static const uint8_t HC165_PIN_DATA  = 28;
// Keep legacy logical IDs and EEPROM mappings; new physical low 16 bits are separate.
static const uint8_t HC165_BIT_COUNT = 64;
static uint16_t navigationRaw49=0xffff;
void serviceNavigation49(uint16_t,uint32_t);

// 8A1 - Groupe edition physique, deux rangees de trois :
// SHIFT / UNDO / REDO puis SELECT / SELECT ALL / DELETE.
enum EditButtonAction : uint8_t {
  EDIT_BUTTON_SHIFT = 0,
  EDIT_BUTTON_UNDO,
  EDIT_BUTTON_REDO,
  EDIT_BUTTON_SELECT,
  EDIT_BUTTON_SELECT_ALL,
  EDIT_BUTTON_DELETE,
  EDIT_BUTTON_COUNT
};
static const uint8_t EDIT_BUTTON_BITS[EDIT_BUTTON_COUNT] = {1, 8, 2, 0, 6, 5};
// 14A1 - Groupe F2 reserve, ordre physique conserve pour attribution future.
static const uint8_t F2_BUTTON_BITS[6] = {10, 9, 3, 11, 8, 2};

// Mapping physique releve :
// A1=B27 REC, A2=B24 PLAY/PAUSE, A3=B29 STOP, A4=B31 DELAY CUT/KEEP
// B1=B26 RETOUR, B2=B25 MP, B3=B28 MS, B4=B30 NOTES CUT/KEEP
static const uint8_t BTN_REC_BIT    = 27;
static const uint8_t BTN_PLAY_BIT   = 24;
static const uint8_t BTN_STOP_BIT   = 29;
static const uint8_t BTN_RETURN_BIT = 26;
static const uint8_t BTN_MP_BIT     = 25;
static const uint8_t BTN_MS_BIT     = 28;
static const uint8_t BTN_DELAY_KEEP_BIT = 31;
static const uint8_t BTN_NOTES_KEEP_BIT  = 30;

static const uint8_t TRANSPORT_BUTTON_COUNT = 8;
static const uint8_t TRANSPORT_BUTTON_BITS[TRANSPORT_BUTTON_COUNT] = {
  BTN_REC_BIT, BTN_PLAY_BIT, BTN_STOP_BIT, BTN_RETURN_BIT,
  BTN_MP_BIT, BTN_MS_BIT, BTN_DELAY_KEEP_BIT, BTN_NOTES_KEEP_BIT
};

// Carte 4 x 4, une ligne par presse-papiers.
// Colonnes : COPIER / SELECTIONNER / COLLER / PREVIEW.
static const uint8_t CLIPBOARD_COUNT = 4;
static const uint8_t CLIPBOARD_ACTION_COUNT = 4;
// La quatrieme rangee B32..B35 est maintenant le groupe d'edition.
static const uint8_t CLIPBOARD_BUTTON_COUNT = 3 * CLIPBOARD_ACTION_COUNT;
static const uint8_t CLIPBOARD_BUTTON_BITS[CLIPBOARD_BUTTON_COUNT] = {
  44, 45, 46, 47,
  40, 41, 42, 43,
  36, 37, 38, 39
};

enum ClipboardAction : uint8_t {
  CLIP_ACTION_COPY = 0,
  CLIP_ACTION_SELECT = 1,
  CLIP_ACTION_PASTE = 2,
  CLIP_ACTION_PREVIEW = 3
};

bool transportButtonStable[TRANSPORT_BUTTON_COUNT];
bool transportButtonLastRaw[TRANSPORT_BUTTON_COUNT];
uint32_t transportButtonRawChangedMs[TRANSPORT_BUTTON_COUNT];
uint32_t transportMeasureRepeatAtMs[2] = {0, 0};
uint16_t transportRepeatWaitTicks = 384;
uint16_t transportRepeatRateTicks = 96;
bool clipboardButtonStable[CLIPBOARD_BUTTON_COUNT];
bool clipboardButtonLastRaw[CLIPBOARD_BUTTON_COUNT];
uint32_t clipboardButtonRawChangedMs[CLIPBOARD_BUTTON_COUNT];
bool hc165Initialised = false;
bool hc165ProbeInitialised = false;
bool hc165ProbeStable[HC165_BIT_COUNT];
bool hc165ProbeLastRaw[HC165_BIT_COUNT];
uint32_t hc165ProbeRawChangedMs[HC165_BIT_COUNT];
bool screenUiAwaitRelease = false;
// Ordre physique : haut gauche->droite, puis bas gauche->droite.
static const uint8_t SCREEN_BUTTON_BITS[8] = {23, 20, 22, 21, 16, 19, 17, 18};
uint32_t lastHc165PollMs = 0;
uint16_t hc165PollMs = 5;
uint16_t hc165PressDebounceMs = 15;
uint16_t hc165ReleaseDebounceMs = 15;

// Reglage temporaire par KY14/KY15/KY16.
bool transportTuningOverlayActive = false;
uint32_t transportTuningLastAdjustMs = 0;
static const uint16_t TRANSPORT_TUNING_OVERLAY_MS = 3000;

bool recordingArmed = false; // realtime overdub, V8.20

// ============================================================
// MIDI / SEQUENCE
// ============================================================
static const uint8_t MIDI_NOTE_ON  = 0x90;
static const uint8_t MIDI_NOTE_OFF = 0x80;
static const uint8_t MIDI_PITCH_BEND = 0xE0;

static const uint8_t TRACK_COUNT = 16;
static const uint16_t STEPS_PER_BAR = 16;
static const uint16_t MAX_BARS = 64;
static const uint16_t STEP_COUNT = STEPS_PER_BAR * MAX_BARS;
static const uint8_t PATTERN_COUNT = 16;
static const uint8_t MAX_NOTES_PER_STEP = 4;
static const uint8_t EXTRA_NOTES_PER_STEP = MAX_NOTES_PER_STEP - 1;

static const uint16_t PPQN = 384;
static const uint16_t TICKS_PER_STEP = PPQN / 4; // 96 ticks par double-croche
static const int16_t MICRO_TICK_MIN = -48;
static const int16_t MICRO_TICK_MAX = 48;
static const uint16_t GATE_TICK_MIN = 1;
static const uint16_t GATE_TICK_MAX = PPQN * 8; // 3072 ticks = 2 mesures en 4/4 avec PPQN 384
static const uint8_t GATE_MUSICAL_COUNT = 9;
static const uint16_t GATE_MUSICAL_VALUES[GATE_MUSICAL_COUNT] = {12, 24, 48, 96, 192, 384, 768, 1536, 3072};

static const float BPM_MIN = 30.0f;
static const float BPM_MAX = 300.0f;

DMAMEM bool stepActive[TRACK_COUNT][STEP_COUNT];
DMAMEM uint8_t stepVel[TRACK_COUNT][STEP_COUNT];
DMAMEM uint8_t stepMidiNote[TRACK_COUNT][STEP_COUNT];
// La premiere note reste dans stepMidiNote pour conserver les formats et
// fonctions historiques. MULTI et DRUM peuvent ajouter trois hauteurs au tick
// exact, sans deplacer l'evenement vers un pas voisin.
uint8_t stepExtraNoteCount[TRACK_COUNT][STEP_COUNT];
DMAMEM uint8_t stepExtraMidiNote[TRACK_COUNT][STEP_COUNT][EXTRA_NOTES_PER_STEP];
struct ExtraNote14 {uint16_t gate;int16_t micro;uint8_t vel;};
EXTMEM ExtraNote14 stepExtra14[TRACK_COUNT][STEP_COUNT][EXTRA_NOTES_PER_STEP];
DMAMEM uint16_t stepGateTicks[TRACK_COUNT][STEP_COUNT]; // duree en ticks, 1..3072
DMAMEM int16_t stepMicroTick[TRACK_COUNT][STEP_COUNT];
// Position de composition dans la cellule 1/16. TIM reste separe et ne
// deplace jamais la tete logique utilisee pour selection/suppression.
DMAMEM int8_t stepLogicalMicro[TRACK_COUNT][STEP_COUNT];

// Legacy edit markers; NOT/VEL/GAT/TIM always belong to the existing note.
// true = valeur propre au pas, modifiee pendant maintien du pad selectionne.
DMAMEM bool stepLockVel[TRACK_COUNT][STEP_COUNT];
DMAMEM bool stepLockPitch[TRACK_COUNT][STEP_COUNT];
DMAMEM bool stepLockGate[TRACK_COUNT][STEP_COUNT];
DMAMEM bool stepLockTime[TRACK_COUNT][STEP_COUNT];
DMAMEM bool stepSelected[TRACK_COUNT][STEP_COUNT];
// 13A3 - Object identity, independent of the displayed pitch and focus.
DMAMEM uint8_t selectedBaseMask[TRACK_COUNT][STEP_COUNT];
bool explicitNoteSelection = false;
struct EmptySelectionCell { uint16_t step; uint8_t track; uint8_t pitch; int32_t tick;uint32_t gate; };
static const uint16_t EMPTY_SELECTION_CAPACITY = 256;
EmptySelectionCell selectedEmptyCells[EMPTY_SELECTION_CAPACITY];
uint16_t selectedEmptyCount = 0;

uint8_t defaultVel[TRACK_COUNT];
uint8_t defaultMidiNote[TRACK_COUNT];
uint16_t defaultGateTicks[TRACK_COUNT];
int16_t defaultMicroTick[TRACK_COUNT];
uint8_t trackMidiChannel[TRACK_COUNT]; // MIDI 0..15, affiche CH1..16
int16_t trackPitchBend[TRACK_COUNT];   // valeur signee -8192..+8191

enum TrackPlayMode : uint8_t {
  TRACK_PLAY_DRUM = 0,
  TRACK_PLAY_MONO,
  TRACK_PLAY_MULTI
};

// -1 = herite de Para > Seq > Prev, 0 = OFF, 1 = ON.
bool globalNotePreview = true;
int8_t trackNotePreviewMode[TRACK_COUNT] = {
  -1, -1, -1, -1, -1, -1, -1, -1,
  -1, -1, -1, -1, -1, -1, -1, -1
};
TrackPlayMode trackPlayMode[TRACK_COUNT];

enum ClipboardContentType : uint8_t {
  CLIP_TYPE_NONE = 0,
  CLIP_TYPE_NOTES,
  CLIP_TYPE_SELECTION,
  CLIP_TYPE_SEQUENCE,
  CLIP_TYPE_EFFECT,
  CLIP_TYPE_REMIX, CLIP_TYPE_TRACK12
};

void releaseRemixClipboard915(uint8_t slot);
struct ClipboardSlot {
  rmx911::Model* remix;
  uint16_t remixSteps,remixTicks;
  bool valid;
  ClipboardContentType type;
  uint8_t sourceTrack;
  uint16_t startStep;
  uint16_t lengthSteps;
  bool active[STEP_COUNT];
  uint8_t vel[STEP_COUNT];
  uint8_t note[STEP_COUNT];
  uint16_t gateTicks[STEP_COUNT];
  int16_t microTick[STEP_COUNT];
  bool lockVel[STEP_COUNT];
  bool lockPitch[STEP_COUNT];
  bool lockGate[STEP_COUNT];
  bool lockTime[STEP_COUNT];
  uint8_t defaultVel;
  uint8_t defaultNote;
  uint16_t defaultGateTicks;
  int16_t defaultMicroTick;
};

// V8.10: clipboard payloads use the same PSRAM as their fine-note data.
EXTMEM ClipboardSlot clipboardSlots[CLIPBOARD_COUNT];
ClipboardContentType clipboardSelectedTypes[CLIPBOARD_COUNT] = {
  CLIP_TYPE_SEQUENCE, CLIP_TYPE_SEQUENCE, CLIP_TYPE_SEQUENCE, CLIP_TYPE_SEQUENCE
};
ClipboardContentType recentClipboardTypes[3] = {
  CLIP_TYPE_SELECTION, CLIP_TYPE_NOTES, CLIP_TYPE_SEQUENCE
};

enum ClipboardCopyScope : uint8_t {
  CLIP_COPY_ALL = 0,       // son complet fige en PX
  CLIP_COPY_NOTE_ONLY,     // evenements/hauteurs sans PX
  CLIP_COPY_NOTE_PX,       // evenements + PX explicites
  CLIP_COPY_PX_ONLY        // PX seuls, pour une destination existante
};
ClipboardCopyScope clipboardCopyScope[CLIPBOARD_COUNT] = {
  CLIP_COPY_ALL, CLIP_COPY_ALL, CLIP_COPY_ALL, CLIP_COPY_ALL
};
bool clipboardMenuActive = false;
uint8_t clipboardMenuSlot = 0;
uint8_t clipboardMenuRecentIndex = 0;
uint32_t clipboardMenuLastActionMs = 0;

enum ClipboardPasteMode : uint8_t {
  CLIP_PASTE_ERASE = 0,
  CLIP_PASTE_OVERDUB = 1
};

ClipboardPasteMode clipboardPasteMode[CLIPBOARD_COUNT] = {
  CLIP_PASTE_ERASE, CLIP_PASTE_ERASE, CLIP_PASTE_ERASE, CLIP_PASTE_ERASE
};
bool clipboardPasteDown[CLIPBOARD_COUNT] = {false, false, false, false};
bool clipboardPasteHoldMenu[CLIPBOARD_COUNT] = {false, false, false, false};
uint32_t clipboardPasteDownAtMs[CLIPBOARD_COUNT] = {0, 0, 0, 0};
bool clipboardPasteMenuActive = false;
uint8_t clipboardPasteMenuSlot = 0;
uint16_t clipboardPasteHoldMs = 500;

static const uint8_t PATTERN_CLIP_MAX = PATTERN_COUNT;
struct PatternClipboardManifest {
  bool valid;
  uint8_t count;
  uint8_t sourceTrack[PATTERN_CLIP_MAX];
  uint8_t sourcePattern[PATTERN_CLIP_MAX];
  bool sourceHasNotes[PATTERN_CLIP_MAX];
};
PatternClipboardManifest patternClipboard[CLIPBOARD_COUNT];
// Snapshot references at COPY; editing the referenced object remains shared.
uint32_t patternModRefs806[CLIPBOARD_COUNT][1][1] = {};
bool patternModRefsValid806[CLIPBOARD_COUNT] = {};

enum ClipboardPreviewMode : uint8_t {
  CLIP_PREVIEW_CUT = 0,
  CLIP_PREVIEW_ADD = 1,
  CLIP_PREVIEW_QUEUE = 2
};

ClipboardPreviewMode clipboardPreviewMode = CLIP_PREVIEW_CUT;

bool clipboardPreviewActive = false;
uint8_t clipboardPreviewSlot = 0;
uint8_t clipboardPreviewTargetTrack = 0;
uint32_t clipboardPreviewTick = 0;
uint32_t clipboardPreviewEndTick = 0;
bool clipboardPreviewFirstTick = true;
bool clipboardPreviewTriggered[STEP_COUNT];
static const uint8_t CLIPBOARD_PREVIEW_QUEUE_SIZE = 6;
uint8_t clipboardPreviewQueueSlot[CLIPBOARD_PREVIEW_QUEUE_SIZE];
uint8_t clipboardPreviewQueueTrack[CLIPBOARD_PREVIEW_QUEUE_SIZE];
uint8_t clipboardPreviewQueueHead = 0;
uint8_t clipboardPreviewQueueTail = 0;
uint8_t clipboardPreviewQueueCount = 0;
uint8_t clipboardPreviewQueueLimit = CLIPBOARD_PREVIEW_QUEUE_SIZE;

struct ClipboardLivePreview {
  bool active;
  uint8_t targetTrack;
  uint32_t tick;
  uint32_t endTick;
  bool firstTick;
  bool triggered[STEP_COUNT];
};
ClipboardLivePreview clipboardLivePreview[CLIPBOARD_COUNT];

enum SaveCandidateType : uint8_t {
  SAVE_CANDIDATE_NOTE = 0,
  SAVE_CANDIDATE_SELECTION,
  SAVE_CANDIDATE_SEQUENCE,
  SAVE_CANDIDATE_PATTERN,
  SAVE_CANDIDATE_SETTINGS,
  SAVE_CANDIDATE_FULL
};

bool sdAvailable = false;
bool sdMenu42=false,sdMenuSave42=false;
bool storageWriteAvailable = false;
bool sdNeoNoticeActive = false;
bool saveMenuActive = false;
bool saveMenuClipboardMode = false;
bool saveMenuPausedPlayback = false;
uint8_t saveMenuSelection = 0;
uint8_t saveMenuClipboardSlot = 0;
SaveCandidateType saveMenuCandidates[3] = {
  SAVE_CANDIDATE_NOTE, SAVE_CANDIDATE_SEQUENCE, SAVE_CANDIDATE_PATTERN
};

enum FileBrowserMode : uint8_t {
  FILE_BROWSER_NONE = 0,
  FILE_BROWSER_SAVE,
  FILE_BROWSER_LOAD
};

FileBrowserMode fileBrowserMode = FILE_BROWSER_NONE;
char fileBrowserPath[48] = "/URBANKAOZ";
char selectedSdFolder[48] = "";
char songFolder[48] = "/URBANKAOZ/SONG";
char fileBrowserRootPath[48] = "/URBANKAOZ";
char fileBrowserName[17] = "";
char lastFileName816[17] = "";
#include "Delivery905State.h"
void rememberFileName816(const char* path);
void settingsLoad816();
void settingsSave816();
void settingsOpenKy23816();
void checks816();
uint8_t storageNameCursor=0;
uint32_t storageSession=0;
bool storageDirty931=true,storageBlink931=false,nameDirty931=true;
void storageSync(bool force=false);
FLASHMEM void storageNameKey(char key);
bool nameDisplayR8();
void nameDeleteR8();
bool namePadR8(uint8_t neo,uint8_t pad,uint8_t edge); bool namePixels54(); bool recordErasePitch820(uint8_t,uint8_t);
void storageNameDelete();
void storageNameTurn(uint8_t encoder,int delta);
char fileBrowserExtension[5] = "SEQ";
char fileBrowserEntry[24] = "--";
bool fileBrowserEntryIsDirectory = false;
uint16_t fileBrowserEntryIndex = 0;
SaveCandidateType pendingSaveCandidate = SAVE_CANDIDATE_SEQUENCE;
bool pendingSaveClipboard = false;
uint8_t pendingSaveClipboardSlot = 0;
enum AfterSaveAction : uint8_t {
  AFTER_SAVE_NONE = 0,
  AFTER_SAVE_TRACK_MODE_CONVERSION
};
AfterSaveAction afterSaveAction = AFTER_SAVE_NONE;
uint8_t afterSaveTrack = 0;
TrackPlayMode afterSaveTrackMode = TRACK_PLAY_MULTI;
uint16_t afterSaveGateTicks = 48;
bool conversionReturnsHome = false;
void applyTrackPlayModeConversion();
// Like clipboardSlots, file-preview contents use the installed PSRAM.
// Keep interrupt/transport queues in their original memory regions.
EXTMEM ClipboardSlot loadPreviewBuffer;
bool clipboardPreviewUsesLoadBuffer = false;
bool patternLoadConfirmActive = false;
bool newProjectConfirm807 = false;
char pendingPatternPath[72] = "";
bool trackMenuActive = false;
bool trackResetConfirmActive = false;
bool trackAutoMenuActive = false;
bool trackMultiMenuActive = false;
bool trackPreviewMenuActive = false;
bool trackPlayMenuActive = false;
bool trackColorMenuActive = false;
bool trackViewMenuActive = false;
bool trackDrumWarningActive = false;
bool trackGateLengthPromptActive = false;
TrackPlayMode pendingTrackPlayMode = TRACK_PLAY_MULTI;
uint16_t trackGateLengthDraft = 48;
int8_t trackColorDraft = -1;
uint8_t trackColorRole = 1; // NEO_COLOR_NOTE, enum declare plus bas
int8_t trackMenuEditingRoy = -1;

enum StorageContentType : uint8_t {
  STORAGE_FULL = 0,
  STORAGE_SONG,
  STORAGE_PATTERN,
  STORAGE_SEQUENCE,
  STORAGE_NOTE,
  STORAGE_FX,
  STORAGE_LFO,
  STORAGE_RESERVED
};

bool loadTypeMenuActive = false;
bool writeMenuActive = false;uint8_t writeMask41=0;uint16_t writeTracks41=0;
bool writeRollEnabled = false;
bool writeTimeEnabled = false;
StorageContentType activeStorageType = STORAGE_PATTERN;
uint8_t fileBrowserDirectoryPage = 0;
static const uint8_t BROWSER_BUTTON_DIRECTORY_MAX = 5;
char browserButtonDirectory[BROWSER_BUTTON_DIRECTORY_MAX][24] = {{0}};
uint8_t browserButtonDirectoryCount = 0;
bool browserHasMoreDirectoryPages = false;

void makeEightButtonLines(char top[21], char bottom[21], const char* labels[8],
                          bool hasPrevious = false, bool hasNext = false);

struct TrackRollSettings {
  uint16_t start;
  uint16_t speed;
  int16_t accel;
  uint8_t expo;
  int16_t curve;
  uint8_t level;
};

#include "Roll1000State.h"
TrackRollSettings trackRoll[TRACK_COUNT];
DMAMEM uint8_t stepRollLockMask[TRACK_COUNT][STEP_COUNT];
#include "RollStore906.h"

// 1A1 - Le tableau historique reste le premier evenement de chaque cellule
// 1/16. Les evenements supplementaires possedent leur propre position exacte,
// leurs valeurs et leur masque PX. Il n'existe donc plus de collision logique
// entre les pads X2/X4/X8 qui partagent la meme cellule historique.
static const uint8_t FINE_EVENTS_PER_TRACK = 128;
static const uint8_t PX_VEL = 1u << 0;
static const uint8_t PX_PITCH = 1u << 1;
static const uint8_t PX_GATE = 1u << 2;
static const uint8_t PX_TIME = 1u << 3;
static const uint8_t PX_ROLL_SHIFT = 4;

struct FineNoteEvent {
  bool active;
  uint16_t step;
  int8_t logicalMicro;
  int16_t micro;
  uint8_t note;
  uint8_t vel;
  uint16_t gate;
  uint16_t pxMask;
  TrackRollSettings roll;
};

int32_t defaultMov41[16][1]={};
#include "Pattern805State.h"
FineNoteEvent fineNote[TRACK_COUNT][FINE_EVENTS_PER_TRACK];
uint8_t selectedFineNote[TRACK_COUNT][FINE_EVENTS_PER_TRACK]; // 0=none, 1=selected, 2=exiled
DMAMEM FineNoteEvent clipboardFine[CLIPBOARD_COUNT][FINE_EVENTS_PER_TRACK];
uint8_t clipboardFineCount[CLIPBOARD_COUNT] = {0, 0, 0, 0};
DMAMEM FineNoteEvent loadPreviewFine[FINE_EVENTS_PER_TRACK];
uint8_t loadPreviewFineCount = 0;
uint32_t fineLastTriggeredCycle[TRACK_COUNT][FINE_EVENTS_PER_TRACK];

int16_t selectedFineIndex = -1;
uint32_t lastGridTick = 0;
bool lastGridTickValid = false;
int16_t lastNoteStep = -1;
int16_t lastNoteFineIndex = -1;
uint8_t lastNoteTrack = 0;
bool lastNoteValid = false;

uint32_t playbackStepCounter = 0;
struct DeferredRollAudio {
  bool active;
  uint8_t track;
  int16_t step; // -1 = valeur Roll generale de piste
  uint32_t dueStep;
  TrackRollSettings previous;
};
static const uint8_t DEFERRED_ROLL_COUNT = 32;
DeferredRollAudio deferredRoll[DEFERRED_ROLL_COUNT];

// Chaque piste garde son pattern actif et ses 15 autres patterns en memoire.
// Le changement de mode d'affichage ne touche jamais aux mutes.
struct StoredTrackPattern {
  bool initialized;
  uint16_t bars;
  bool hasNotes;
};
StoredTrackPattern patternBank[TRACK_COUNT][PATTERN_COUNT];
static const uint8_t GROOVE_COUNT = 144;
static const uint8_t GROOVE_STEPS = 16;
// Signed offsets: NOT semitones, VEL units, GAT/TIM ticks. Zero is neutral.
#include "GrooveState16.h"
uint8_t patternGroove[TRACK_COUNT][PATTERN_COUNT] = {}; // 0=off, 1..64=sheet
#include "GrooveListState.h"
bool grooveEditMode = false;
uint16_t grooveAssigned805[16] = {};
uint8_t editedGroove = 1;
uint16_t grooveStepMask = 0;
int16_t pendingPatternGroove = -1;

bool internalPatternSessionFresh = true;
uint8_t activePattern[TRACK_COUNT];
bool trackMuted[TRACK_COUNT];
bool patternMode = false;
bool patternSelected[TRACK_COUNT][PATTERN_COUNT];
uint16_t deleteRenamedPatterns[TRACK_COUNT] = {};
bool patternSelectionAnchorValid = false;
uint8_t patternSelectionAnchorTrack = 0;
uint8_t patternSelectionAnchorPattern = 0;
bool patternPasteTargetHeld = false;
bool patternPasteTargetConsumed = false;
uint8_t patternPasteTargetTrack = 0;
uint8_t patternPasteTargetPattern = 0;
uint8_t patternRowOffset = 0;
uint16_t sequenceBars = 1;
uint16_t trackBars[TRACK_COUNT];
uint16_t trackLoopStartStep[TRACK_COUNT];
uint16_t trackLoopEndStep[TRACK_COUNT];
uint32_t lastTrackLocalTick[TRACK_COUNT];
bool beatEditActive = false;
bool beatEditLength = false;
uint16_t beatDraftBars = 1;
uint16_t beatOriginalBars = 1;
bool barFillPromptActive = false;
uint16_t pendingOldBars = 1;
uint16_t pendingNewBars = 1;
bool barDeletePromptActive = false;
uint16_t barDeleteOriginalBars = 1;
uint16_t barDeleteTargetBars = 1;
uint16_t barDeleteCount = 0;
uint16_t barDeleteStartBar = 0; // index reel 0..N-1 de la premiere mesure supprimee
bool patternModeEditActive = false;
bool patternModeDraftNext = true;
bool patternChangeNext = true;
int8_t pendingPattern[TRACK_COUNT] = {
  -1, -1, -1, -1, -1, -1, -1, -1,
  -1, -1, -1, -1, -1, -1, -1, -1
};
#include "GrooveBridgeState.h"
uint32_t patternBeatBlinkUntilMs = 0;
uint16_t neoSelectionHoldMs = 700;
uint8_t rollApplyDelaySteps = 0;
bool trackParamsOpen = false;
uint8_t trackRollLastEditedField = 255;
static const uint16_t MAX_PREROLL_NOTES = 1536;

struct PatternLoadBuffer {
  float bpm;
  uint16_t lengthSteps;
  uint8_t defaultNote[TRACK_COUNT];
  uint8_t defaultVelocity[TRACK_COUNT];
  uint16_t defaultGate[TRACK_COUNT];
  int16_t defaultMicro[TRACK_COUNT];
  uint8_t midiChannel[TRACK_COUNT];
  int16_t pitchBend[TRACK_COUNT];
  TrackRollSettings roll[TRACK_COUNT];
  int8_t autoSelect[TRACK_COUNT];
};

PatternLoadBuffer patternLoadBuffer;

float bpm = 115.0f;
bool playing = false;
int16_t midiChannelPitchBendSent[16];

// Test temporaire du mode Remix temps reel.
// La sequence actuelle ne contient qu'une mesure, donc seul Neo 1 est actif en Remix.
bool remixMode = false;


uint16_t currentStep = 0;
static const uint32_t TICKS_PER_MEASURE = (uint32_t)TICKS_PER_STEP * STEP_COUNT;
volatile uint32_t pendingClockTicks = 0;
#include "Transport830.h"
volatile uint32_t clockOverruns821=0;
uint32_t transportSlices821=0,transportMax821=0,transportBacklog821=0;
bool transportBusy821=false;
volatile bool internalClockRunning = false;
IntervalTimer internalClockTimer;
uint32_t measureTick = 0;
uint64_t transportTickAbsolute = 0;
bool evaluateCurrentTickOnNextClock = true;
uint16_t cycleCounter = 0;
DMAMEM uint32_t lastTriggeredCycle[TRACK_COUNT][STEP_COUNT];

// V4.80 : pas de snapshot de mesure.
// Les valeurs sont lues au declenchement, puis Note On + Note Off sont figes pour cette note.

// Selection / affichage.
// Une seule piste est visible. Les quatre lignes physiques sont quatre notes
// chromatiques consecutives, et les quatre Neo donnent 16 positions de temps.
uint8_t viewStartTrack = 0;
uint8_t selectedTrack = 0;
int16_t selectedStep = -1;
uint8_t selectedVoice = 0;
uint8_t chromaticBottomNote = 36;
enum NoteViewMode : uint8_t {
  NOTE_VIEW_COMPACT = 0,
  NOTE_VIEW_CHROMATIC
};
NoteViewMode globalNoteViewMode = NOTE_VIEW_COMPACT;
// -1 = global, 0 = compact, 1 = chromatic.
int8_t trackNoteViewMode[TRACK_COUNT] = {
  -1, -1, -1, -1, -1, -1, -1, -1,
  -1, -1, -1, -1, -1, -1, -1, -1
};
uint8_t compactRowOffset = 0;bool compactRecenter08=true;
uint8_t activePitchRow = 255;
uint8_t activePitchTrack = 255;

uint32_t neoPressStartMs[NEO_COUNT][16];
bool neoPadDown[NEO_COUNT][16];
bool neoPadEdited[NEO_COUNT][16];
bool neoPadCancelSelectionOnly[NEO_COUNT][16];
bool neoPadCreatedOnPress[NEO_COUNT][16];
static const uint16_t NEO_SHORT_PRESS_MAX_MS = 350;

bool selectionMode = false;
bool allTrackNotesSelected = false;
bool selectionAnchorValid = false;
uint8_t selectionAnchorTrack = 0;
uint16_t selectionAnchorStep = 0;
bool selectionRangeMade = false;
bool selectionPitchValid = false;
uint8_t selectionPitch = 60;

uint32_t viewTickOffset = 0;
// -2=4 mesures, -1=2 mesures, 0=1 mesure, puis jusqu'a 1/32 mesure.
int8_t zoomLevel = 0;
// Vue propre a chaque piste : changer de piste annule la selection, mais
// retrouver la piste restitue sa position, son zoom et sa ligne musicale.
uint32_t trackViewTickOffset[TRACK_COUNT];
int8_t trackZoomLevel[TRACK_COUNT];
uint8_t trackActivePitchRow[TRACK_COUNT];
// Roy1 partage BPM et zoom; false=BPM, true=zoom.
bool roy1EditsZoom = true;
bool headFollow = false;
uint8_t stepLayout25=0,headTrail25=0;
#include "StepLayout25.h"
bool stepContext25();

enum ViewChangeSource : uint8_t {
  VIEW_SRC_BOOT = 0, VIEW_SRC_J2_ZOOM, VIEW_SRC_J2_PAGE, VIEW_SRC_J2_RESET,
  VIEW_SRC_ROY1_ZOOM, VIEW_SRC_HEAD, VIEW_SRC_ROY2, VIEW_SRC_TRACK, VIEW_SRC_PATTERN,
  VIEW_SRC_LENGTH, VIEW_SRC_UNKNOWN
};
struct ViewLogEntry {
  uint32_t atMs;
  uint32_t offset;
  uint32_t playTick;
  int8_t zoom;
  uint8_t track;
  uint8_t source;
  uint8_t playingState;
};
static const uint8_t VIEW_LOG_COUNT = 32;
ViewLogEntry viewLog[VIEW_LOG_COUNT];
uint8_t viewLogWrite = 0;
uint8_t viewLogUsed = 0;
uint32_t viewAuditOffset = 0;
int8_t viewAuditZoom = 0;
ViewChangeSource pendingViewSource = VIEW_SRC_BOOT;
bool transportMeasureWasPlaying[2] = {false, false};

uint16_t lastDisplayedStep = 0xFFFF;
uint8_t lastDisplayedVisualPos = 255;
bool neoDisplayDirty = true;
uint32_t lastButtonReadMs = 0;

// Intensites LED.
// Fond blanc de piste selectionnee supprime en V4.80.
static const uint8_t NEO_GLOBAL_BRIGHTNESS = 45;
enum NeoColorRole : uint8_t {
  NEO_COLOR_EMPTY = 0,
  NEO_COLOR_NOTE,
  NEO_COLOR_PLAYHEAD,
  NEO_COLOR_NOTE_PLAYHEAD,
  NEO_COLOR_SELECTED_NOTE,
  NEO_COLOR_SELECTED_EMPTY,
  NEO_COLOR_REMIX,
  NEO_COLOR_REMIX_PLAYHEAD,
  NEO_COLOR_ROLE_COUNT
};

struct NeoPaletteColor {
  const char* name;
  uint8_t r;
  uint8_t g;
  uint8_t b;
};

static const NeoPaletteColor NEO_PALETTE[] = {
  {"OFF",       0,  0,  0},
  {"RED",      70,  0,  0},
  {"ORANGE",   85, 35,  0},
  {"YELLOW",   65, 55,  0},
  {"GREEN",     0, 30,  0},
  {"CYAN",      0, 65, 65},
  {"BLUE",      0,  0, 45},
  {"DARK VIOLET", 22, 0, 38},
  {"VIOLET",   55,  0, 90},
  {"PINK",     90,  0, 60},
  {"WHITE",    45, 45, 45}
};
static const uint8_t NEO_PALETTE_COUNT = sizeof(NEO_PALETTE) / sizeof(NEO_PALETTE[0]);
static const char* NEO_COLOR_ROLE_NAME[NEO_COLOR_ROLE_COUNT] = {
  "EMPTY", "NOTE", "PLAYHEAD", "NOTE + PLAYHEAD",
  "SELECTED NOTE", "SELECTED EMPTY", "ROLL ZONE", "REMIX PLAYHEAD"
};
static const char* NEO_COLOR_ROLE_SHORT[NEO_COLOR_ROLE_COUNT] = {
  "EMPTY", "NOTE", "PLAY", "N+PL", "SNOT", "SEMP", "ROLL", "RPLY"
};
uint8_t neoColorChoice[NEO_COLOR_ROLE_COUNT] = {0, 3, 4, 5, 9, 7, 6, 4};
#include "Config902.h"
#include "PinTest33State.h"
#include "FxState32.h"
#include "Stability904State.h"
#include "Quick903State.h"
#include "Ui910State.h"
// -1 = couleur generale du role, sinon index de NEO_PALETTE.
// Couleurs exclusivement globales; aucun tableau de couleurs par piste en RAM.

// -1 = herite du parametre general, 0 = OFF, 1 = ON.
int8_t trackAutoSelectMode[TRACK_COUNT] = {
  -1, -1, -1, -1, -1, -1, -1, -1,
  -1, -1, -1, -1, -1, -1, -1, -1
};
int8_t trackMultiEditMode[TRACK_COUNT] = {
  -1, -1, -1, -1, -1, -1, -1, -1,
  -1, -1, -1, -1, -1, -1, -1, -1
};

// 0A1 - Undo/Redo minimal : seules les deux dernieres suppressions individuelles sont
// memorisee. L'evenement conserve sa position, ses valeurs et son masque PX.
// Aucun snapshot de piste, aucun Undo de Paste, WRITE ou conversion.
struct DeletedNoteUndoRecord {
  bool valid;
  bool restored;
  uint8_t track;
  uint8_t pattern;
  uint8_t originalVoice;
  int16_t restoredFineIndex;
  FineNoteEvent event;
};
bool deleteSelection23();bool deleteHistory23(bool);void deleteFree23(uint8_t);void deleteShift23();
static const uint8_t NOTE_UNDO_LEVELS = 2;
DeletedNoteUndoRecord deletedNoteHistory[NOTE_UNDO_LEVELS] = {};
uint8_t deletedNoteHistoryCount = 0;
uint8_t deletedNoteHistoryCursor = 0;
void rememberDeletedNote(uint8_t track, uint16_t step, uint8_t voice);
void discardNoteRedo();
FLASHMEM void performUndo(); bool moveHistory939(bool redo); int moveDisplay939();
FLASHMEM void performRedo();

// ============================================================
// EDIT PARAMETRE AFFICHE OLED
// ============================================================
enum EditMode : uint8_t {
  EDIT_VEL = 0,
  EDIT_PITCH = 1,
  EDIT_GATE = 2,
  EDIT_TIME = 3
};

static const uint8_t EDIT_MODE_COUNT = 4;
EditMode editMode = EDIT_VEL;

// K3 alterne entre le choix de ligne (SEL) et l'edition de hauteur (NOT).
bool pitchEncoderSelectMode = true;
uint32_t activeLineWhiteUntilMs = 0;
static const uint16_t ACTIVE_LINE_WHITE_MS = 90;

struct PendingSelectionField {
  bool touched;
  bool sameValue;
  int32_t delta;
  int target;
};

struct PendingSelectionTransaction {
  bool active;
  bool range;
  uint8_t track;
  uint8_t sourcePitch;
  int16_t step;
  uint8_t voice;
  PendingSelectionField field[EDIT_MODE_COUNT];
  PendingSelectionField roll[6];
};

PendingSelectionTransaction pendingSelectionEdit = {};

// TIM est un PX de lecture. Son clic alterne avec MOV, qui deplace la
// position logique des notes selectionnees sans creer de copie.
bool timeEncoderMoveMode = false;

enum GateEditMode : uint8_t {
  GATE_EDIT_MUSICAL = 0,
  GATE_EDIT_24T = 1,
  GATE_EDIT_1T = 2
};
GateEditMode gateEditMode = GATE_EDIT_1T;
#include "Ui60State.h"
bool fineTimeUnit38(){return gateEditMode==GATE_EDIT_1T;}
void toggleTimeUnit38(){gateEditMode=fineTimeUnit38()?GATE_EDIT_24T:GATE_EDIT_1T;delayTimeMode817[0]=delayTimeMode817[1]=fineTimeUnit38()?0:1;displayValueDirty=true;}
struct GateDraftSegment { int32_t delta; GateEditMode unit; };
GateDraftSegment gateDraftSegments[24];
uint8_t gateDraftCount = 0;

enum NumericTarget : uint8_t {
  NUMERIC_TARGET_PARAM = 0,
  NUMERIC_TARGET_BPM = 1,
  NUMERIC_TARGET_TRACK = 2,
  NUMERIC_TARGET_CHANNEL = 3,
  NUMERIC_TARGET_PITCH_BEND = 4
};

enum UiEditTarget : uint8_t {
  UI_EDIT_TRACK = 0,
  UI_EDIT_CHANNEL = 1,
  UI_EDIT_PITCH_BEND = 2,
  UI_EDIT_VEL = 3,
  UI_EDIT_GATE = 4,
  UI_EDIT_TIME = 5,
  UI_EDIT_NOTE = 6
};

UiEditTarget uiEditTarget = UI_EDIT_VEL;
bool pitchBendFast = false;
uint8_t pitchBendStep44=0;
bool pitchBendStepShown44=false;
int pitchBendIncrement44(){static const int steps[]={1,10,100};return steps[pitchBendStep44];}

// Etat d'affichage uniquement : le delta d'une edition multiple disparait
// des que la selection est quittee et n'est jamais sauvegarde.
bool selectionEditDeltaVisible = false;
EditMode selectionEditDeltaMode = EDIT_VEL;
int32_t selectionEditDelta = 0;
bool multiEditSameValue = false;
bool loopEditStart = true;
bool loopBarMode817[2]={true,true};
uint16_t loopStartStep = 1;
uint16_t loopEndStep = STEPS_PER_BAR;
uint8_t displayedPattern = 1;

enum NumericScope : uint8_t {
  NUMERIC_SCOPE_DEFAULT = 0,
  NUMERIC_SCOPE_PLOCK = 1,
  NUMERIC_SCOPE_SELECTION = 2
};

NumericTarget lastNumericTarget = NUMERIC_TARGET_PARAM;
bool numericEntryActive = false;
bool numericEnabled906=true;
void checks906();void memory906();
char numericEntryDigits[8] = "";
uint8_t numericEntryLen = 0;
NumericTarget numericEntryTarget = NUMERIC_TARGET_PARAM;
NumericScope numericEntryScope = NUMERIC_SCOPE_DEFAULT;
EditMode numericEntryMode = EDIT_VEL;
uint8_t numericEntryTrack = 0;
int16_t numericEntryStep = -1;

// ============================================================
// NOTE-OFF SCHEDULER
// ============================================================
struct ActiveNote {
  uint8_t block21=0;
  uint8_t outputMask902;
  bool active;
  bool preview;
  bool audition;
  bool remixLive913;
  uint8_t track;
  uint8_t channel;
  uint8_t note;
  uint8_t previewSlot;
  uint32_t offAtUs;
};

static const uint16_t MAX_ACTIVE_NOTES = 256;
uint32_t activeNoteRejects15=0;uint16_t activeNotePeak15=0;
ActiveNote activeNotes[MAX_ACTIVE_NOTES];

// ============================================================
// UART LINE BUFFERS PICO
// ============================================================
char pico1Line[80];
char pico2Line[80];
uint8_t pico1LinePos = 0;
uint8_t pico2LinePos = 0;
// V8.19 display recorder: UKD1 rows from Picran can contain 20 characters
// plus escaping, so keep this separate receive line large enough.
char displayRxLine[96];
uint8_t displayRxLinePos = 0;

// Debounce/securite boutons KY cote Teensy.
// Les clics de mode se font seulement sur un cycle press/release credible,
// pas sur un B,idx,1 isole.
bool kyBtnDown[TRACK_COUNT];
uint32_t kyBtnDownAtMs[TRACK_COUNT];
bool kyRotatedWhileDown[TRACK_COUNT];
bool kyHelpShown[TRACK_COUNT];
bool gateHoldValueShown = false;
bool gateIncrementShown15=false;
bool returnHeld41=false,returnWasPlaying41=false;
bool curveFine41=true;
bool curveKey41(uint8_t);
bool moveFine40=false,moveUnitShown40=false;
uint8_t moveUnit42=1; // bar, quarter, eighth, sixteenth, 64th, 128th
const uint16_t moveTicks42[4]={1536,384,96,1};
void moveEdit40(int32_t);void moveText40(char*);void moveFinish41(bool);bool moveTransaction41=false,moveInternal41=false;
void grooveListPad40(uint8_t,uint8_t);void grooveListFinish40();
void grooveListClipboard40(uint8_t,bool);void grooveListFocus40(uint8_t,uint8_t);
bool grooveListEdit40(uint8_t,int32_t);void grooveListDisplay40();
uint8_t grooveSheet40(uint8_t,uint8_t,uint8_t,uint64_t);
uint64_t songDisplayTick40();void transportCancel40();
void pendingApply40();void transportBrowse40(uint8_t,bool);void transportRepeat40();

uint32_t kyBtnLastToggleMs[TRACK_COUNT];
uint32_t kyRotationLastAcceptedMs[TRACK_COUNT];
int8_t kyRotationAccumulator[TRACK_COUNT];
static const char* KY_FULL_NAME[TRACK_COUNT] = {
  "Track", "MIDI channel", "Note pitch", "Velocity",
  "Gate duration", "Micro timing", "Pitch bend", "BPM",
  "Roll level", "Disabled", "Roll start", "Roll speed",
  "Roll acceleration", "Roll exponent", "Roll curve", "Mute track"
};


// Prototypes UI utilises avant declaration complete.
bool isTrackVisible(uint8_t track);
void padToSlotStep(uint8_t neoIndex, uint8_t pad, uint8_t &slot, uint16_t &step);
bool visibleNoteAtPad(uint8_t track, uint8_t rowNote, uint8_t neoIndex, uint8_t pad,
                      uint16_t &sourceStep, uint8_t *sourceVoice = nullptr,
                      bool headOnly = false);
int16_t fineIndexForVoice(uint8_t track, uint16_t step, uint8_t voice);
uint8_t voiceForFineIndex(uint8_t track, uint16_t step, uint8_t fineIndex);
void tickToStepAndMicro(uint8_t track, uint32_t tick, uint16_t &step, int16_t &micro);
void setCommittedStepValue(uint8_t track, uint16_t step, uint8_t voice,
                           EditMode mode, int value);
void clearTrackStep(uint8_t track, uint16_t step);
NoteViewMode noteViewForTrack(uint8_t track);
void jumpPlaybackToMeasureStep(uint16_t measureIndex, uint16_t step);
void setTransportPositionToMeasureStart();
void updateInternalClockPeriod();
void processClockTick();
bool recordContext820();
bool recordPad820(uint8_t n,uint8_t pad,uint8_t edge);
bool recordOctave820(uint8_t bit);
bool recordRender820();
void recordToggle820();
void recordClose820();
void recordErase820();
void recordAdvance820();
void recordCheck820();
bool keyboardContext823();
bool recordCapture823();
bool keyboardTrackPulse823(uint8_t track);
void keyboardToggle823(); void recordExitA01(); void leavePadModesA01(bool preserveSelection);
extern bool keyboardEnabled823;
void keyboardMidi823(uint8_t channel,uint8_t note,bool on);
void keyboardDrum823(uint8_t channel,uint8_t note,uint32_t gateUs);
void keyboardLightsClear823();
void keyboardService823();
void recordSuspend823();
void keyboardCheck823();
void sendWriteMenu();
void openWriteMenu();
void applyWriteToSequence();
FLASHMEM void handleClipboardButtonEdge(uint8_t buttonIndex, bool pressed);
void serviceClipboardUi();
bool anyLiveClipboardPreview();
bool cancelStepSelection();
bool capturePattern15(uint8_t slot);
bool installPattern15(uint8_t slot,uint8_t index,uint8_t t,uint8_t p);
bool finishDeleteMode(bool commit);
bool deleteLocksMode15=false;
void deleteEncoderLocks15(uint8_t id);
void checksEmpty15();
bool pendingDeleteNote(uint8_t track, uint16_t step, uint8_t voice);
bool cancelStepSelectionWithoutCommit();
bool selectionHasActiveNotes();
void commitPendingSelectionEdit();
void discardPendingSelectionEdit();
int pendingEditedValue(uint8_t track, uint16_t step, uint8_t voice, EditMode mode, int baseValue);
TrackRollSettings pendingDisplayedRoll(uint8_t track, uint16_t step, uint8_t voice,
                                       TrackRollSettings base);
int32_t audibleMicroTick(uint8_t track, uint16_t step, uint8_t voice);
TrackRollSettings audibleRollSettings(uint8_t track, uint16_t step, uint8_t voice);
int rollFieldValue(const TrackRollSettings &roll, uint8_t field);
void setRollFieldValue(TrackRollSettings &roll, uint8_t field, int value);

FLASHMEM void switchTrackPattern(uint8_t track, uint8_t pattern, bool selectAfter = true);
FLASHMEM void requestTrackPattern(uint8_t track, uint8_t pattern);
void showStorageAlertIfNeeded();
void internalPatternPath(uint8_t track, uint8_t pattern, char path[40]);
bool saveTrackPattern(uint8_t track, uint8_t pattern);
bool loadTrackPattern(uint8_t track, uint8_t pattern);
bool captureNoteRecord(uint8_t track, uint16_t step, uint8_t voice, FineNoteEvent &event);
uint16_t moveSelectedNotesByTicks(uint8_t track, int32_t deltaTicks);

// ============================================================
// HELPERS
// ============================================================
FLASHMEM void neoPixel821(uint8_t n,uint8_t pad,uint32_t color){
  if(n>=4||pad>=16)return;
  uint16_t bit=1u<<pad;
  if((neoValid821[n]&bit)&&neoColors821[n][pad]==color){++neoAvoided821;return;}
  if(neoAvailable[n])neo[n]->pixels.setPixelColor(pad,color);neoColors821[n][pad]=color;pcNeoDirty931|=1u<<n;neoValid821[n]|=bit;neoPending821[n]=true;++neoWrites821;
}
int clampInt(int v, int lo, int hi) {
  if (v < lo) return lo;
  if (v > hi) return hi;
  return v;
}

float stepDurationMs() {
  return 60000.0f / bpm / 4.0f;
}

uint16_t trackStepCount(uint8_t track) {
  if (track >= TRACK_COUNT) return STEPS_PER_BAR;
  return (uint16_t)clampInt(trackBars[track], 1, MAX_BARS) * STEPS_PER_BAR;
}

uint16_t activeStepCount() {
  return trackStepCount(selectedTrack);
}

uint16_t longestTrackStepCount() {
  uint16_t longest = STEPS_PER_BAR;
  for (uint8_t track = 0; track < TRACK_COUNT; track++) {
    longest = max(longest, trackStepCount(track));
  }
  return longest;
}

void syncLegacyTrackLoopBounds() {
  loopStartStep=clampInt(loopStartStep,1,longestTrackStepCount());
  loopEndStep=clampInt(loopEndStep,loopStartStep,longestTrackStepCount());
  for (uint8_t track = 0; track < TRACK_COUNT; track++) {
    trackLoopStartStep[track] = loopStartStep;
    trackLoopEndStep[track] = loopEndStep;
  }
}

uint32_t globalLoopedTick() {
  uint32_t longestTicks = (uint32_t)longestTrackStepCount() * TICKS_PER_STEP;
  uint32_t start = (uint32_t)(loopStartStep - 1) * TICKS_PER_STEP;
  uint32_t end = min((uint32_t)loopEndStep * TICKS_PER_STEP, longestTicks);
  if (end <= start) {
    start = 0;
    end = longestTicks;
  }
  if (transportTickAbsolute < end) return (uint32_t)transportTickAbsolute;
  return start + (uint32_t)((transportTickAbsolute - start) % (end - start));
}

float measureDurationMs() {
  return stepDurationMs() * activeStepCount();
}

uint32_t activeSequenceTicks() {
  return (uint32_t)activeStepCount() * TICKS_PER_STEP;
}

uint32_t trackSequenceTicks(uint8_t track) {
  return (uint32_t)trackStepCount(track) * TICKS_PER_STEP;
}

uint32_t localTickForTrack(uint8_t track) {
  uint32_t length = trackSequenceTicks(track);
  if (length == 0) return 0;
  return (globalLoopedTick() + length - grooveTrackPhase[track] % length) % length;
}

float tickDurationMs() {
  return 60000.0f / bpm / (float)PPQN;
}

float stepDurationUs() {
  return stepDurationMs() * 1000.0f;
}

uint8_t midiChannelForTrack(uint8_t track) {
  if (track >= TRACK_COUNT) return 0;
  return blockChannel21(track);
}

uint8_t humanTrack(uint8_t track) {
  return track + 1;
}

uint32_t colorFor(uint8_t neoIndex, uint8_t r, uint8_t g, uint8_t b) {
  return neo[neoIndex]->pixels.Color(r, g, b);
}

uint32_t colorForRole(uint8_t neoIndex, NeoColorRole role) {
  if(role==NEO_COLOR_REMIX_PLAYHEAD)role=NEO_COLOR_PLAYHEAD;
  uint8_t choice = neoColorChoice[role==NEO_COLOR_SELECTED_EMPTY?NEO_COLOR_EMPTY:(uint8_t)role];
  if (choice >= NEO_PALETTE_COUNT) choice = 0;
  const NeoPaletteColor &color = NEO_PALETTE[choice];
  return colorScale11(colorFor(neoIndex, color.r, color.g, color.b),colorRoleBrightness11(role));
}

uint32_t colorForTrackRole(uint8_t neoIndex, uint8_t track, NeoColorRole role) {
  (void)track;
  if(role==NEO_COLOR_REMIX_PLAYHEAD)role=NEO_COLOR_PLAYHEAD;
  int choice = neoColorChoice[role==NEO_COLOR_SELECTED_EMPTY?NEO_COLOR_EMPTY:(uint8_t)role];
  if (choice < 0 || choice >= NEO_PALETTE_COUNT) choice = 0;
  const NeoPaletteColor &color = NEO_PALETTE[choice];
  return colorScale11(colorFor(neoIndex, color.r, color.g, color.b),colorRoleBrightness11(role));
}

bool selectedPadIsHeld() {
  if (selectedStep < 0) return false;
  if (!isTrackVisible(selectedTrack)) return false;

  uint8_t slot = selectedTrack - viewStartTrack;

  for (uint8_t n = 0; n < NEO_COUNT; n++) {
    if (!neoAvailable[n]) continue;
    for (uint8_t pad = 0; pad < 16; pad++) {
      if (!neoPadDown[n][pad]) continue;

      uint8_t pSlot;
      uint16_t pStep;
      padToSlotStep(n, pad, pSlot, pStep);
      uint8_t voice = 0;
      uint16_t headStep = pStep;
      uint8_t note = visualNoteAtVoice(selectedTrack, (uint16_t)selectedStep, selectedVoice);
      if (pSlot == slot && visibleNoteAtPad(selectedTrack, note, n, pad, headStep, &voice, true) &&
          headStep == (uint16_t)selectedStep && voice == selectedVoice) return true;
    }
  }

  return false;
}

void markSelectedHeldPadEdited() {
  if (selectedStep < 0 || !isTrackVisible(selectedTrack)) return;
  uint8_t slot = selectedTrack - viewStartTrack;

  for (uint8_t n = 0; n < NEO_COUNT; n++) {
    if (!neoAvailable[n]) continue;
    for (uint8_t pad = 0; pad < 16; pad++) {
      if (!neoPadDown[n][pad]) continue;
      uint8_t pSlot;
      uint16_t pStep;
      padToSlotStep(n, pad, pSlot, pStep);
      uint8_t voice = 0;
      uint16_t headStep = pStep;
      uint8_t note = visualNoteAtVoice(selectedTrack, (uint16_t)selectedStep, selectedVoice);
      if (pSlot == slot && visibleNoteAtPad(selectedTrack, note, n, pad, headStep, &voice, true) &&
          headStep == (uint16_t)selectedStep && voice == selectedVoice) neoPadEdited[n][pad] = true;
    }
  }
}

const char* uiEditName() {
  if (uiEditTarget == UI_EDIT_TRACK) return "TRK";
  if (uiEditTarget == UI_EDIT_CHANNEL) return "CHA";
  if (uiEditTarget == UI_EDIT_PITCH_BEND) return "PBN";
  if (uiEditTarget == UI_EDIT_VEL) return "VEL";
  if (uiEditTarget == UI_EDIT_GATE) return "GAT";
  if (uiEditTarget == UI_EDIT_TIME) return timeEncoderMoveMode ? "MOV" : "TIM";
  return "NOTE";
}

int uiEditValue() {
  if (uiEditTarget == UI_EDIT_TRACK) return humanTrack(selectedTrack);
  if (uiEditTarget == UI_EDIT_CHANNEL) return midiChannelForTrack(selectedTrack) + 1;
  if (uiEditTarget == UI_EDIT_PITCH_BEND) return indNote50(selectedTrack,4,true);
  return getCurrentEditedValue();
}

const char* uiEditSpeedText() {
  if (uiEditTarget == UI_EDIT_PITCH_BEND) return "100";
  if (uiEditTarget == UI_EDIT_TRACK || uiEditTarget == UI_EDIT_CHANNEL) return "x1";
  if (uiEditTarget == UI_EDIT_GATE) {
    if (gateEditMode == GATE_EDIT_MUSICAL) return "MUS";
    if (gateEditMode == GATE_EDIT_24T) return "\x14";
    return "1t";
  }
  if (uiEditTarget == UI_EDIT_TIME){if(!timeEncoderMoveMode)return "1t";static const char*units09[]={"1/4","1/8","1/16","1/32","1/64","1/128","1/256","1/512"};return units09[clampInt(zoomLevel+2,0,7)];}
  return "x1";
}

uint8_t effectiveVel(uint8_t track, uint16_t step, uint8_t voice = 0) {
  int16_t fine = fineIndexForVoice(track, step, voice);
  if (fine >= 0) {
    FineNoteEvent &event = fineNote[track][fine];
    return event.vel;
  }
  if(voice&&voice<=stepExtraNoteCount[track][step])return stepExtra14[track][step][voice-1].vel;
  return stepVel[track][step];
}

uint8_t effectiveMidiNote(uint8_t track, uint16_t step) {
  return stepMidiNote[track][step];
}

uint8_t baseVoiceCountAtStep(uint8_t track, uint16_t step) {
  if (track >= TRACK_COUNT || step >= STEP_COUNT || !stepActive[track][step]) return 0;
  return (uint8_t)(1 + min(stepExtraNoteCount[track][step], EXTRA_NOTES_PER_STEP));
}

int16_t fineIndexForVoice(uint8_t track, uint16_t step, uint8_t voice) {
  uint8_t baseCount = baseVoiceCountAtStep(track, step);
  if (voice < baseCount) return -1;
  uint8_t wanted = voice - baseCount;
  for (uint8_t index = 0; index < FINE_EVENTS_PER_TRACK; index++) {
    if (!fineNote[track][index].active || fineNote[track][index].step != step) continue;
    if (wanted == 0) return index;
    wanted--;
  }
  return -1;
}

uint8_t voiceForFineIndex(uint8_t track, uint16_t step, uint8_t fineIndex) {
  uint8_t voice = baseVoiceCountAtStep(track, step);
  for (uint8_t index = 0; index < FINE_EVENTS_PER_TRACK; index++) {
    if (!fineNote[track][index].active || fineNote[track][index].step != step) continue;
    if (index == fineIndex) return voice;
    voice++;
  }
  return 255;
}

uint8_t noteCountAtStep(uint8_t track, uint16_t step) {
  if (track >= TRACK_COUNT || step >= STEP_COUNT) return 0;
  uint16_t count = baseVoiceCountAtStep(track, step);
  for (uint8_t index = 0; index < FINE_EVENTS_PER_TRACK; index++) {
    if (fineNote[track][index].active && fineNote[track][index].step == step) count++;
  }
  return (uint8_t)min((uint16_t)255, count);
}

uint8_t noteAtVoice(uint8_t track, uint16_t step, uint8_t voice) {
  uint8_t baseCount = baseVoiceCountAtStep(track, step);
  if (voice == 0 && baseCount > 0) return effectiveMidiNote(track, step);
  if (voice < baseCount && voice <= stepExtraNoteCount[track][step]) {
    return stepExtraMidiNote[track][step][voice - 1];
  }
  int16_t fine = fineIndexForVoice(track, step, voice);
  if (fine >= 0) {
    FineNoteEvent &event = fineNote[track][fine];
    return event.note;
  }
  return defaultMidiNote[track];
}

int8_t findVoiceAtStep(uint8_t track, uint16_t step, uint8_t note) {
  uint8_t count = noteCountAtStep(track, step);
  for (uint8_t voice = 0; voice < count; voice++) {
    if (noteAtVoice(track, step, voice) == note) return (int8_t)voice;
  }
  return -1;
}

int8_t findVoiceAtExactTick(uint8_t track, uint16_t step, uint8_t note, int16_t micro) {
  uint8_t baseCount = baseVoiceCountAtStep(track, step);
  if (baseCount > 0 && stepLogicalMicro[track][step] == micro) {
    for (uint8_t voice = 0; voice < baseCount; voice++) {
      if (noteAtVoice(track, step, voice) == note) return (int8_t)voice;
    }
  }
  uint8_t voice = baseCount;
  for (uint8_t index = 0; index < FINE_EVENTS_PER_TRACK; index++) {
    FineNoteEvent &event = fineNote[track][index];
    if (!event.active || event.step != step) continue;
    if (event.logicalMicro == micro && event.note == note) return (int8_t)voice;
    voice++;
  }
  return -1;
}

int16_t addFineNote(uint8_t track, uint16_t step, int16_t micro, uint8_t note,
                    int16_t logicalMicro) {
  for (uint8_t index = 0; index < FINE_EVENTS_PER_TRACK; index++) {
    FineNoteEvent &event = fineNote[track][index];
    if (event.active) continue;
    event.active = true;
    fineLastTriggeredCycle[track][index]=UINT32_MAX;
    event.step = step;
    event.logicalMicro = (int8_t)clampInt(logicalMicro, MICRO_TICK_MIN, TICKS_PER_STEP-1);
    event.micro = micro;
    event.note = note;
    event.vel = indNote50(track,1);
    event.gate = indNote50(track,2);
    event.pxMask = PX_PITCH|PX_VEL|PX_GATE|PX_TIME;
    event.roll = trackRoll[track];
    return index;
  }
  DISPLAY_SERIAL.println(F("MSG|EVENT POOL FULL"));
  return -1;
}

bool addNoteAtStep(uint8_t track, uint16_t step, uint8_t note) {

  if (!stepActive[track][step] || findVoiceAtStep(track, step, note) >= 0) return false;
  uint8_t count = stepExtraNoteCount[track][step];
  if (count >= EXTRA_NOTES_PER_STEP) return false;
  stepExtraMidiNote[track][step][count] = note;
  stepExtra14[track][step][count]={(uint16_t)indNote50(track,2),(int16_t)indNote50(track,3),(uint8_t)indNote50(track,1)};
  stepExtraNoteCount[track][step] = count + 1;
  return true;
}

void removeVoiceAtStep(uint8_t track, uint16_t step, uint8_t voice) {
  lockRemoveNote817(track,step,voice);
  int16_t fine = fineIndexForVoice(track, step, voice);
  if (fine >= 0) {
    fineNote[track][fine].active = false;
    if (selectedFineIndex == fine) selectedFineIndex = -1;
    return;
  }
  uint8_t extraCount = stepExtraNoteCount[track][step];
  if (voice == 0) {
    if (extraCount == 0) {
      int16_t promote = -1;
      for (uint8_t index = 0; index < FINE_EVENTS_PER_TRACK; index++) {
        if (fineNote[track][index].active && fineNote[track][index].step == step) {
          promote = index;
          break;
        }
      }
      if (promote < 0) {
        clearTrackStep(track, step);
        return;
      }
      FineNoteEvent event = fineNote[track][promote];
      fineNote[track][promote].active = false;
      stepActive[track][step] = true;
      stepMidiNote[track][step] = event.note;
      stepVel[track][step] = event.vel;
      stepGateTicks[track][step] = event.gate;
      stepLogicalMicro[track][step] = event.logicalMicro;
      stepMicroTick[track][step] = event.micro;
      stepLockPitch[track][step] = (event.pxMask & PX_PITCH) != 0;
      stepLockVel[track][step] = (event.pxMask & PX_VEL) != 0;
      stepLockGate[track][step] = (event.pxMask & PX_GATE) != 0;
      stepLockTime[track][step] = (event.pxMask & PX_TIME) != 0;
      rollSet906(track,step,event.roll,(uint8_t)((event.pxMask >> PX_ROLL_SHIFT) & 0x3F));
      return;
    }
    stepMidiNote[track][step] = stepExtraMidiNote[track][step][0];
    auto x14=stepExtra14[track][step][0];stepVel[track][step]=x14.vel;stepGateTicks[track][step]=x14.gate;stepMicroTick[track][step]=x14.micro;
    stepLockPitch[track][step] = true;
    for (uint8_t i = 1; i < extraCount; i++) {
      stepExtraMidiNote[track][step][i - 1] = stepExtraMidiNote[track][step][i];stepExtra14[track][step][i-1]=stepExtra14[track][step][i];
    }
    stepExtraNoteCount[track][step] = extraCount - 1;
    return;
  }
  uint8_t index = voice - 1;
  if (index >= extraCount) return;
  for (uint8_t i = index + 1; i < extraCount; i++) {
    stepExtraMidiNote[track][step][i - 1] = stepExtraMidiNote[track][step][i];stepExtra14[track][step][i-1]=stepExtra14[track][step][i];
  }
  stepExtraNoteCount[track][step] = extraCount - 1;
}

bool monoMeasureRowsView(uint8_t track) {
  return track < TRACK_COUNT && trackPlayMode[linkOwner12(track,LINK_POLY12)] == TRACK_PLAY_MONO &&
         noteViewForTrack(track) == NOTE_VIEW_CHROMATIC;
}

uint16_t effectiveGateTicks(uint8_t track, uint16_t step, uint8_t voice = 0) {
  int16_t fine = fineIndexForVoice(track, step, voice);
  if (fine >= 0) {
    FineNoteEvent &event = fineNote[track][fine];
    return event.gate;
  }
  if(voice&&voice<=stepExtraNoteCount[track][step])return stepExtra14[track][step][voice-1].gate;
  return stepGateTicks[track][step];
}

int16_t effectiveMicroTick(uint8_t track, uint16_t step, uint8_t voice = 0) {
  int16_t fine = fineIndexForVoice(track, step, voice);
  if (fine >= 0) {
    FineNoteEvent &event = fineNote[track][fine];
    return event.micro;
  }
  if(voice&&voice<=stepExtraNoteCount[track][step])return stepExtra14[track][step][voice-1].micro;
  return stepMicroTick[track][step];
}

int16_t logicalMicroTick(uint8_t track, uint16_t step, uint8_t voice = 0) {
  int16_t fine = fineIndexForVoice(track, step, voice);
  if (fine >= 0) return fineNote[track][fine].logicalMicro;
  return stepLogicalMicro[track][step];
}

TrackRollSettings configuredRollSettings(uint8_t track, uint16_t step, uint8_t voice = 0) {
  int16_t fine = fineIndexForVoice(track, step, voice);
  TrackRollSettings result = fine >= 0 ? fineNote[track][fine].roll : rollRead906(track,step);
  uint8_t mask = fine >= 0 ? (uint8_t)((fineNote[track][fine].pxMask >> PX_ROLL_SHIFT) & 0x3F)
                           : stepRollLockMask[track][step];
  for (uint8_t field = 0; field < 6; field++) {
    if ((mask & (1u << field)) == 0) setRollFieldValue(result, field, indBase50(track,indRuntimePattern50(track),IND_ROLL50,track,field,rollFieldValue(trackRoll[track], field)));
  }
  return result;
}

TrackRollSettings effectiveRollSettings(uint8_t track, uint16_t step, uint8_t voice = 0) {
  TrackRollSettings result = configuredRollSettings(track, step, voice);
  for (uint8_t i = 0; i < DEFERRED_ROLL_COUNT; i++) {
    if (!deferredRoll[i].active || deferredRoll[i].track != track) continue;
    if (deferredRoll[i].step == (int16_t)step) return deferredRoll[i].previous;
    // Une modification de valeur Roll par defaut (step < 0) ne concerne
    // aucune note existante, meme pendant le delai audio configure.
  }
  return result;
}

void deferRollAudio(uint8_t track, int16_t step, const TrackRollSettings &previous) {
  if (rollApplyDelaySteps == 0) return;
  int8_t freeSlot = -1;
  for (uint8_t i = 0; i < DEFERRED_ROLL_COUNT; i++) {
    if (deferredRoll[i].active && deferredRoll[i].track == track && deferredRoll[i].step == step) {
      deferredRoll[i].dueStep = playbackStepCounter + rollApplyDelaySteps;
      return; // conserver la toute premiere valeur audible pendant la rafale d'edition
    }
    if (!deferredRoll[i].active && freeSlot < 0) freeSlot = i;
  }
  if (freeSlot < 0) freeSlot = 0;
  deferredRoll[freeSlot].active = true;
  deferredRoll[freeSlot].track = track;
  deferredRoll[freeSlot].step = step;
  deferredRoll[freeSlot].dueStep = playbackStepCounter + rollApplyDelaySteps;
  deferredRoll[freeSlot].previous = previous;
}

void serviceDeferredRollAudio() {
  for (uint8_t i = 0; i < DEFERRED_ROLL_COUNT; i++) {
    if (deferredRoll[i].active && playbackStepCounter >= deferredRoll[i].dueStep) {
      deferredRoll[i].active = false;
    }
  }
}

void applyConfiguredRollToAudio() {
  for (uint8_t i = 0; i < DEFERRED_ROLL_COUNT; i++) deferredRoll[i].active = false;
}

void scheduleConfiguredRollApply() {
  applyConfiguredRollToAudio();
}

int getCurrentEditedValue() {
  uint8_t t = selectedTrack;

  // A range has no single selectedStep. Read its first selected voice,
  // including the live draft, rather than the defaults for new notes.
  if (hasSelectedSteps()) {
    for(uint8_t pass=0;pass<16;++pass){uint8_t tr=(t+pass)%16;
      for(uint16_t s=0;s<trackStepCount(tr);++s)if(stepSelected[tr][s])
        for(uint8_t v=0;v<noteCountAtStep(tr,s);++v)if(selectedNoteObject(tr,s,v)){
          int base=editMode==EDIT_VEL?effectiveVel(tr,s,v):editMode==EDIT_PITCH?noteAtVoice(tr,s,v):editMode==EDIT_GATE?effectiveGateTicks(tr,s,v):effectiveMicroTick(tr,s,v);
          return pendingEditedValue(tr,s,v,editMode,base);
        }
    }
  }
  bool pLockEdit = selectedStep >= 0 && selectedVoice<noteCountAtStep(t,(uint16_t)selectedStep);

  if (!pLockEdit) {
    if (editMode == EDIT_VEL) return indNote50(t,1,true);
    if (editMode == EDIT_PITCH) {
      if (activePitchTrack == t && activePitchRow <= 127 &&
          (noteViewForTrack(t) == NOTE_VIEW_COMPACT || monoMeasureRowsView(t))) return activePitchRow;
      return indNote50(t,0,true);
    }
    if (editMode == EDIT_GATE) return indNote50(t,2,true);
    return indNote50(t,3,true);
  }

  uint16_t s = (uint16_t)selectedStep;
  int baseValue = editMode == EDIT_VEL ? effectiveVel(t, s, selectedVoice)
                : editMode == EDIT_PITCH ? noteAtVoice(t, s, selectedVoice)
                : editMode == EDIT_GATE ? effectiveGateTicks(t, s, selectedVoice)
                                        : effectiveMicroTick(t, s, selectedVoice);
  return pendingEditedValue(t, s, selectedVoice, editMode, baseValue);
}

void clearSelectionEditDisplay() {
  selectionEditDeltaVisible = false;
  selectionEditDelta = 0;
}

bool selectedNoteObject(uint8_t track, uint16_t step, uint8_t voice) {
  if (!stepSelected[track][step]) return false;
  if (allTrackNotesSelected) return true;
  if (explicitNoteSelection) {
    int16_t fine = fineIndexForVoice(track, step, voice);
    return fine >= 0 ? selectedFineNote[track][fine] == 1
                     : voice < MAX_NOTES_PER_STEP && (selectedBaseMask[track][step] & (1u << voice));
  }
  if (!selectionPitchValid) return false;
  uint8_t source = pendingSelectionEdit.active && pendingSelectionEdit.range
                     ? pendingSelectionEdit.sourcePitch : selectionPitch;
  return noteAtVoice(track, step, voice) == source;
}

bool pendingDeleteNote(uint8_t track, uint16_t step, uint8_t voice) {
  return deleteButtonActive && !deleteNumeric55 && !deleteLocksMode15 && (deletePatternContext
    ? patternSelected[track][activePattern[track]] : selectedNoteObject(track, step, voice));
}

bool exiledNoteObject(uint8_t track, uint16_t step, uint8_t voice) {
  if (!selectButtonActive) return false;
  int16_t fine = fineIndexForVoice(track, step, voice);
  return fine >= 0 ? selectedFineNote[track][fine] == 2
    : voice < MAX_NOTES_PER_STEP && (selectedBaseMask[track][step] & (1u << (voice + 4)));
}

void exileNoteObject(uint8_t track, uint16_t step, uint8_t voice) {
  if (!noteChangedInCurrentSelection(track, step, voice)) {
    setNoteObjectSelected(track, step, voice, false);
    return;
  }
  int16_t fine = fineIndexForVoice(track, step, voice);
  if (fine >= 0) selectedFineNote[track][fine] = 2;
  else selectedBaseMask[track][step] |= (1u << (voice + 4));
  setNoteObjectSelected(track, step, voice, false);
}

bool selectedEmptyCell(uint8_t track, uint16_t step, uint8_t pitch) {
  for (uint16_t i = 0; i < selectedEmptyCount; i++) {
    if (selectedEmptyCells[i].track == track && selectedEmptyCells[i].step == step &&
        selectedEmptyCells[i].pitch == pitch) return true;
  }
  return false;
}

void refreshObjectSelectionStep(uint8_t track, uint16_t step) {
  bool selected = (selectedBaseMask[track][step] & 0x0F) != 0;
  for (uint8_t i = 0; i < FINE_EVENTS_PER_TRACK && !selected; i++)
    selected = selectedFineNote[track][i] == 1 && fineNote[track][i].active && fineNote[track][i].step == step;
  for (uint16_t i = 0; i < selectedEmptyCount && !selected; i++)
    selected = selectedEmptyCells[i].track == track && selectedEmptyCells[i].step == step;
  stepSelected[track][step] = selected;
}

void setNoteObjectSelected(uint8_t track, uint16_t step, uint8_t voice, bool selected) {
  int16_t fine = fineIndexForVoice(track, step, voice);
  if (fine >= 0) {
    if (selected) selectedFineNote[track][fine] = 1;
    else if (selectedFineNote[track][fine] == 1) selectedFineNote[track][fine] = 0;
  }
  else if (voice < MAX_NOTES_PER_STEP) {
    if (selected) { selectedBaseMask[track][step] |= (1u << voice); selectedBaseMask[track][step] &= ~(1u << (voice + 4)); }
    else selectedBaseMask[track][step] &= ~(1u << voice);
  }
  refreshObjectSelectionStep(track, step);
}

void toggleEmptyCellSelection(uint8_t track, uint16_t step, uint8_t pitch) {
  explicitNoteSelection = true;
  for (uint16_t i = 0; i < selectedEmptyCount; i++) {
    if (selectedEmptyCells[i].track == track && selectedEmptyCells[i].step == step &&
        selectedEmptyCells[i].pitch == pitch) {
      selectedEmptyCells[i] = selectedEmptyCells[--selectedEmptyCount];
      refreshObjectSelectionStep(track, step);
      selectedStep = -1;
      neoDisplayDirty = displayValueDirty = true;
      return;
    }
  }
  if (selectedEmptyCount >= EMPTY_SELECTION_CAPACITY) {
    DISPLAY_SERIAL.println(F("MSG|EMPTY SELECTION FULL"));
    return;
  }
  selectedEmptyCells[selectedEmptyCount++] = {step, track, pitch,(int32_t)step*96,defaultGateTicks[track]};
  stepSelected[track][step] = true;
  selectedStep = -1; // Empty coordinates must never become a note edit focus.
  neoDisplayDirty = displayValueDirty = true;
}

int8_t selectedRangeVoice(uint8_t track, uint16_t step) {
  if (!stepSelected[track][step]) return -1;
  for (uint8_t voice = 0; voice < noteCountAtStep(track, step); voice++)
    if (selectedNoteObject(track, step, voice)) return (int8_t)voice;
  return -1;
}

int selectedStepValue(uint8_t track, uint16_t step, uint8_t voice, EditMode mode) {
  int baseValue = mode == EDIT_VEL ? effectiveVel(track, step, voice)
                : mode == EDIT_PITCH ? noteAtVoice(track, step, voice)
                : mode == EDIT_GATE ? effectiveGateTicks(track, step, voice)
                                    : effectiveMicroTick(track, step, voice);
  return pendingEditedValue(track, step, voice, mode, baseValue);
}

bool multipleSelectedNotes() {
  uint8_t count = 0;
  for (uint8_t t = 0; t < TRACK_COUNT; ++t)
    for (uint16_t s = 0; s < trackStepCount(t); ++s) {
      if (!stepSelected[t][s]) continue;
      for (uint8_t v = 0; v < noteCountAtStep(t, s); ++v)
        if (selectedNoteObject(t, s, v) && ++count == 2) return true;
    }
  return false;
}

bool selectedStepsHaveCommonValue(EditMode mode, int &value) {
  bool found = false;
  for (uint8_t track = 0; track < TRACK_COUNT; track++) {
    for (uint16_t step = 0; step < STEP_COUNT; step++) {
      if (!stepSelected[track][step]) continue;
      for (uint8_t voice = 0; voice < noteCountAtStep(track, step); voice++) {
      if (!selectedNoteObject(track, step, voice)) continue;
      int current = selectedStepValue(track, step, (uint8_t)voice, mode);
      if (!found) {
        value = current;
        found = true;
      } else if (current != value) {
        return false;
      }
      }
    }
  }
  return found;
}

bool rollZoneAtPad(uint8_t track, uint8_t rowNote, uint8_t neoIndex, uint8_t pad) {
  if (track >= TRACK_COUNT) return false;
  uint32_t pixelStart;
  uint32_t pixelEnd;
  padTimelineBounds(neoIndex, pad, pixelStart, pixelEnd);
  uint16_t count = trackStepCount(track);
  if (count == 0) return false;
  uint16_t firstCandidate = pixelStart > 1728
                              ? (uint16_t)((pixelStart - 1728) / TICKS_PER_STEP) : 0;
  uint16_t lastCandidate = (uint16_t)min((uint32_t)(count - 1),
                                         (pixelEnd + 1728) / TICKS_PER_STEP);
  for (uint16_t candidate = firstCandidate; candidate <= lastCandidate; candidate++) {
    if (!stepActive[track][candidate]) continue;
    for (uint8_t voice = 0; voice < noteCountAtStep(track, candidate); voice++) {
      if (pendingDeleteNote(track, candidate, voice)) continue;
      if (!monoMeasureRowsView(track) && visualNoteAtVoice(track, candidate, voice) != rowNote) continue;
      TrackRollSettings roll = headFollowForTrack(track)?audibleRollSettings(track,candidate,voice):pendingDisplayedRoll(track,candidate,voice,configuredRollSettings(track,candidate,voice));
      if (roll.start == 0 || (roll.level == 0 && !rollExtra1000(track,activePattern[track],(int32_t)candidate*96+logicalMicroTick(track,candidate,voice),noteAtVoice(track,candidate,voice)).v[1])) continue;
      int32_t target = (int32_t)candidate * TICKS_PER_STEP +
                       (headFollowForTrack(track)?audibleMicroTick(track,candidate,voice):selectedStepValue(track,candidate,voice,EDIT_TIME));
      const int32_t origin38=target;int32_t zoneStart = target - roll.start;
      target-=rollStop09(roll,rollExtra1000(track,activePattern[track],(int32_t)candidate*96+logicalMicroTick(track,candidate,voice),noteAtVoice(track,candidate,voice)));
      if (rollDirection60(track,activePattern[track],lockTick817(track,candidate,voice),noteAtVoice(track,candidate,voice),!headFollowForTrack(track))!=1 && target > (int32_t)pixelStart && zoneStart < (int32_t)pixelEnd) return true;
      int32_t postEnd=2*origin38-zoneStart;
      if (rollDirection60(track,activePattern[track],lockTick817(track,candidate,voice),noteAtVoice(track,candidate,voice),!headFollowForTrack(track))!=0 && postEnd > (int32_t)pixelStart && 2*origin38-target < (int32_t)pixelEnd) return true;
    }
  }
  return false;
}

int8_t pendingVoiceForStep(uint8_t track, uint16_t step) {
  if (!pendingSelectionEdit.active || pendingSelectionEdit.track != track) return -1;
  if (pendingSelectionEdit.range) return selectedRangeVoice(track, step);
  if (pendingSelectionEdit.step != (int16_t)step || pendingSelectionEdit.voice>=noteCountAtStep(track,step)) return -1;
  return (int8_t)pendingSelectionEdit.voice;
}

uint8_t effectiveGrooveAssignment(uint8_t track, uint8_t pattern) {
  return pendingPatternGroove >= 0 && selectButtonActive && patternMode && patternSelected[track][pattern]
    ? (uint8_t)pendingPatternGroove : patternGroove[track][pattern];
}

uint8_t playbackGroove(uint8_t track, uint8_t pattern) {
  if(grooveBypass23||grooveAllOff59||(grooveTrackOff59&(1u<<track)))return 0;
 auto b=blockAtNote21(track,blockIndex21(),255,-1,255);
 return grooveSheet40(track,pattern,b.groove,transportTickAbsolute>=grooveListOrigin[track]?(transportTickAbsolute-grooveListOrigin[track])/1536:0);
}

int grooveDelta(uint8_t track, uint16_t step, uint8_t field) {
  if(grooveBypass23||grooveAllOff59||(grooveTrackOff59&(1u<<track)))return field==2?100:0;
  uint8_t sheet = grooveEventTrack == track && grooveEventSheet >= 0
    ? (uint8_t)grooveEventSheet : playbackGroove(track, activePattern[track]);
  if (!sheet || sheet > GROOVE_COUNT || field >= 4) return field==2?100:0;
  if(field==2)return grooveGate51(sheet,step);
  return grooveOffset[sheet - 1][step % GROOVE_STEPS][field];
}

uint8_t audibleVel(uint8_t track, uint16_t step, uint8_t voice = 0) {
  int base = effectiveVel(track, step, voice);
  return (uint8_t)clampInt(pendingEditedValue(track, step, voice, EDIT_VEL, base) + grooveDelta(track, step, 1) + mod805Offset(track, MOD_VEL, modLogicalTick805), 0, 127);
}

uint8_t audibleNoteAtVoice(uint8_t track, uint16_t step, uint8_t voice) {
  int base = noteAtVoice(track, step, voice);
  return (uint8_t)clampInt(pendingEditedValue(track, step, voice, EDIT_PITCH, base) + grooveDelta(track, step, 0) + mod805Offset(track, MOD_NOT, modLogicalTick805), 0, 127);
}

uint16_t audibleGateTicks(uint8_t track, uint16_t step, uint8_t voice = 0) {
  int base = effectiveGateTicks(track, step, voice);
  return (uint16_t)clampInt(((int64_t)pendingEditedValue(track, step, voice, EDIT_GATE, base) * grooveDelta(track, step, 2) / 100) + mod805Offset(track, MOD_GAT, modLogicalTick805), GATE_TICK_MIN, GATE_TICK_MAX);
}

int32_t audibleMicroTick(uint8_t track, uint16_t step, uint8_t voice = 0) {
  int baseOffset = effectiveMicroTick(track, step, voice);
  int audibleOffset = pendingEditedValue(track, step, voice, EDIT_TIME, baseOffset);
  return defaultMov41[track][0]+(int32_t)(clampInt((int)logicalMicroTick(track, step, voice) + audibleOffset,
                           -TICKS_PER_STEP, TICKS_PER_STEP) + grooveTime16(grooveEventTrack==track&&grooveEventSheet>=0?(uint8_t)grooveEventSheet:playbackGroove(track,activePattern[track]), (int32_t)step*96+clampInt((int)logicalMicroTick(track,step,voice)+audibleOffset,-96,96)) + clampInt(mod805Offset(track, MOD_TIM, modLogicalTick805), -96, 96));
}

TrackRollSettings audibleRollSettings(uint8_t track, uint16_t step, uint8_t voice = 0) {
  TrackRollSettings roll = effectiveRollSettings(track, step, voice);
  if(!rollNoteAllowed938(track,step,voice)){roll.start=0;return roll;}
  roll = pendingDisplayedRoll(track, step, voice, roll);
  roll.start = clampInt((int)roll.start + mod805Offset(track, MOD_RST, modLogicalTick805), 0, 1536);
  roll.speed = clampInt((int)roll.speed + mod805Offset(track, MOD_RSP, modLogicalTick805), 1, 96);
  roll.accel = clampInt((int)roll.accel + mod805Offset(track, MOD_RAC, modLogicalTick805), -128, 127);
  if(roll.expo<=100)roll.expo = clampInt((int)roll.expo + mod805Offset(track, MOD_REX, modLogicalTick805), 0, 100);
  roll.curve = clampInt((int)roll.curve + mod805Offset(track, MOD_RCV, modLogicalTick805), -100, 100);
  roll.level = clampInt((int)roll.level + mod805Offset(track, MOD_RLV, modLogicalTick805), 0, 127);
  return roll;
}

FLASHMEM void sendMidi3(uint8_t status, uint8_t data1, uint8_t data2) {
  pidiMidi827(status,data1,data2);
}

FLASHMEM void sendPitchBendNow(uint8_t channel, int16_t signedValue) {
  channel &= 0x0F;
  // Destination-aware bend is sent before each note; no cross-port suppression.
  int32_t value14 = (int32_t)signedValue + 8192;
  value14 = clampInt((int)value14, 0, 16383);
  sendMidi3(MIDI_PITCH_BEND | (channel & 0x0F),
            (uint8_t)(value14 & 0x7F),
            (uint8_t)((value14 >> 7) & 0x7F));
  midiChannelPitchBendSent[channel] = signedValue;
}

void lockBeforeNote14(uint8_t,uint8_t);
bool noteOnNow(uint8_t channel, uint8_t note, uint8_t vel) {
 if(controller940)return false;
  if(!vel)return false;
  if(rejectLateNote830())return false;
  if(playing)mod808FlushMidi(max((int64_t)0,(int64_t)(songEngineActive?songTick:transportTickAbsolute)-midiAdvance808),4);
  if(rejectLateNote830())return false; // preparation itself may have exceeded the deadline
  // MIDI contract: effective PLX CC/NRPN (including drafts) MUST precede Note On.
  // Nothing that can restore an older parameter may run between these calls.
  lockBeforeNote14(channel,note);
  sendMidi3(MIDI_NOTE_ON | (channel & 0x0F), note, vel);
  keyboardMidi823(channel,note,vel!=0);
  if(remixEmittingModel911==&remixSource911)remixVoiceSent913=true;
  return true;
}

void noteOffNow(uint8_t channel, uint8_t note) {
  sendMidi3(MIDI_NOTE_OFF | (channel & 0x0F), note, 0);
  keyboardMidi823(channel,note,false);
  lockOff817(channel,note);
}

bool scheduleNoteOffAtUs(uint8_t channel, uint8_t note, uint32_t offAtUs, bool preview,
                         bool audition = false, uint8_t track = 255, uint8_t previewSlot = 255, bool remixLive = false) {
  if(track==255&&midiTrack940<16)track=midiTrack940;
  uint16_t used15=0;for(const auto&n:activeNotes)used15+=n.active;activeNotePeak15=max(activeNotePeak15,used15);
  // Si la meme note est deja active sur le meme canal, on l'eteint d'abord.
  // Cela evite les notes longues/coincees quand un pas se rejoue avant son ancien Note Off.
  for (uint16_t i = 0; i < MAX_ACTIVE_NOTES; i++) {
    if (activeNotes[i].active && activeNotes[i].channel == channel && activeNotes[i].note == note && activeNotes[i].outputMask902==midiMask902(channel)) {
      {MidiScope902 prior(activeNotes[i].outputMask902,activeNotes[i].track);noteOffNow(channel, note);}
      activeNotes[i].active = true;
      activeNotes[i].preview = preview;
      activeNotes[i].audition = audition;activeNotes[i].remixLive913=remixLive;
      activeNotes[i].track = track;activeNotes[i].block21=blockIndex21();
      activeNotes[i].channel = channel;activeNotes[i].outputMask902=midiMask902(channel);
      activeNotes[i].note = note;
      activeNotes[i].previewSlot = previewSlot;
      activeNotes[i].offAtUs = offAtUs;
      return true;
    }
  }

  for (uint16_t i = 0; i < MAX_ACTIVE_NOTES; i++) {
    if (!activeNotes[i].active) {
      activeNotes[i].active = true;
      activeNotes[i].preview = preview;
      activeNotes[i].audition = audition;activeNotes[i].remixLive913=remixLive;
      activeNotes[i].track = track;activeNotes[i].block21=blockIndex21();
      activeNotes[i].channel = channel;activeNotes[i].outputMask902=midiMask902(channel);
      activeNotes[i].note = note;
      activeNotes[i].previewSlot = previewSlot;
      activeNotes[i].offAtUs = offAtUs;
      return true;
    }
  }

  // Si la file est pleine, on evite de laisser une note sans Note Off planifie.
  // Refuser seulement la nouvelle note, sans couper une autre adresse.
  ++activeNoteRejects15; // Reject without sending an unrelated Note Off.
  return false;
}

void triggerNoteWithFixedGate(uint8_t channel, uint8_t note, uint8_t vel, uint32_t gateUs) {
  if(rejectLateNote830())return;
  uint32_t nowUs = micros();
  uint32_t offAtUs = nowUs + gateUs;
  // Programmer d'abord coupe proprement une ancienne occurrence de la meme
  // note. Le Note On doit venir ensuite, sinon le Note Off de retrigger coupe
  // instantanement la nouvelle note (Roll inaudible).
  if (scheduleNoteOffAtUs(channel, note, offAtUs, false)) noteOnNow(channel, note, vel);
}

void triggerPreviewNoteWithFixedGate(uint8_t channel, uint8_t note, uint8_t vel, uint32_t gateUs,
                                     uint8_t previewSlot = 255,uint8_t previewTrack=255) {
  if(rejectLateNote830())return;
  uint32_t offAtUs = micros() + gateUs;
  if (scheduleNoteOffAtUs(channel, note, offAtUs, true, false, 255, previewSlot)&&noteOnNow(channel,note,vel))manualDelay1000(previewTrack<16?previewTrack:selectedTrack,note,vel,gateUs);
}

void stopNoteAuditions() {
  for (uint16_t i = 0; i < MAX_ACTIVE_NOTES; i++) {
    if (!activeNotes[i].active || !activeNotes[i].audition) continue;
    {MidiScope902 route(activeNotes[i].outputMask902,activeNotes[i].track);noteOffNow(activeNotes[i].channel, activeNotes[i].note);}
    activeNotes[i].active = false;
  }
}

FLASHMEM void triggerNoteAudition(uint8_t track, uint8_t note) {
  if(blockPlaying21<0){stopNoteAuditions();for(uint8_t b=0;b<3;++b){BlockScope21 scope(b);triggerNoteAudition(track,note);}return;}
  int step=-1;uint8_t voice=0;
  if(selectedTrack==track&&selectedStep>=0&&selectedVoice<noteCountAtStep(track,selectedStep)){step=selectedStep;voice=selectedVoice;}
  if(hasSelectedSteps()){step=-1;for(uint16_t st=0;st<trackStepCount(track)&&step<0;++st)for(uint8_t v=0;v<noteCountAtStep(track,st);++v)if(selectedNoteObject(track,st,v)){step=st;voice=v;break;}}
  const bool explicitBlock=blockResolvedValid21;const auto previousBlock=blockResolved21;
  uint8_t source=step>=0?noteAtVoice(track,step,voice):255;
  int32_t identity=step>=0?lockTick817(track,step,voice):-1000;
  if(!explicitBlock){blockResolved21=blockAtNote21(track,blockIndex21(),step>=0?activePattern[track]:255,identity,source);blockResolvedValid21=true;
    if(step>=0){auto&b=blockResolved21;uint8_t base=blockIndex21()*4;
      b.port=lockDisplay817(LOCK_BLOCK21,track,base,b.port);b.channel=lockDisplay817(LOCK_BLOCK21,track,base+1,b.channel);
      b.transpose=lockDisplay817(LOCK_BLOCK21,track,base+3,b.transpose);b.groove=lockDisplay817(LOCK_BLOCK21,track,12,b.groove);}}
  if(blockIndex21()==0){uint32_t epoch=modEpoch805;ioNote38(track,step>=0?pendingEditedValue(track,step,voice,EDIT_PITCH,noteAtVoice(track,step,voice)):note,step>=0?pendingEditedValue(track,step,voice,EDIT_VEL,effectiveVel(track,step,voice)):defaultVel[track],step>=0?pendingEditedValue(track,step,voice,EDIT_GATE,effectiveGateTicks(track,step,voice)):defaultGateTicks[track],false);if(epoch!=modEpoch805)mod808FlushMidi(step>=0?(int64_t)step*96:transportTickAbsolute,16);}
  uint8_t previousTrack=grooveEventTrack;int previousSheet=grooveEventSheet;int64_t previousTick=modLogicalTick805;
  if(step>=0){grooveEventTrack=track;grooveEventSheet=blockResolved21.groove?listSheetAtBar(reinterpret_cast<const GrooveListEntry*>(listBank44(track)[blockResolved21.groove-1]),step/16):0;modLogicalTick805=(int64_t)step*96;note=audibleNoteAtVoice(track,step,voice);}
  uint8_t velocity=step>=0?audibleVel(track,step,voice):defaultVel[track];
  uint16_t gate=step>=0?audibleGateTicks(track,step,voice):defaultGateTicks[track];
  note=blockPitch34(track,note);
  MidiScope902 route(outMask902(track),track);uint8_t channel=midiChannelForTrack(track);
  uint32_t gateUs=clampInt((int)(gate*tickDurationMs()*1000.0f),1000,20000000);
  if(blockResolved21.port&&velocity&&scheduleNoteOffAtUs(channel,note,micros()+gateUs,true,true,track)){
    if(step>=0)lockBegin817(track,activePattern[track],identity,source,channel,note,gateUs);
    if(noteOnNow(channel,note,velocity))manualDelay1000(track,note,velocity,gateUs);
  }
  grooveEventTrack=previousTrack;grooveEventSheet=previousSheet;modLogicalTick805=previousTick;
  blockResolved21=previousBlock;blockResolvedValid21=explicitBlock;
}

void stopActiveNotesForTrack(uint8_t track) {
  for (uint16_t i = 0; i < MAX_ACTIVE_NOTES; i++) {
    if (!activeNotes[i].active || activeNotes[i].track != track) continue;
    {MidiScope902 route(activeNotes[i].outputMask902,activeNotes[i].track);noteOffNow(activeNotes[i].channel, activeNotes[i].note);}
    activeNotes[i].active = false;
  }
}

void triggerTrackNoteWithFixedGate(uint8_t track, uint8_t note, uint8_t vel, uint32_t gateUs, int16_t bendDelta = 0, int32_t lockTick = -1000, uint8_t sourceNote = 255, uint8_t sourcePattern = 255) {
  if(!vel)return;
  if(blockPlaying21<0){for(uint8_t b=0;b<3;++b){BlockScope21 scope(b);if(outMask902(track))triggerTrackNoteWithFixedGate(track,note,vel,gateUs,bendDelta,lockTick,sourceNote,sourcePattern);}return;}
  note=blockPitch34(track,note);

  MidiScope902 route(outMask902(track),track);
  if(rejectLateNote830())return; // before capture, locks, MIDI and Pidi delays
  if (grooveTestCapture) {
    ++grooveTestNotes; grooveTestPitch = note; grooveTestGateUs = gateUs;
    return;
  }
  uint8_t lockPattern=(sourcePattern<16||sourcePattern==254)?sourcePattern:activePattern[track];
  uint8_t channel = midiChannelForTrack(track);
  // PBN est le filtre de sortie de la piste : sa nouvelle valeur ne touche
  // pas une note deja en cours et est appliquee juste avant la prochaine.
  int bendOut=clampInt(indNote50(track,4)+bendDelta,-8192,8191);
  mod806ChannelBend(channel,songEngineActive?songTick:transportTickAbsolute,bendOut);
  sendPitchBendNow(channel,bendOut);
  if (trackSilent818(track)) return;
  if (trackPlayMode[linkOwner12(track,LINK_POLY12)] == TRACK_PLAY_DRUM) {
    lockBegin817(track,lockPattern,lockTick,sourceNote,channel,note,gateUs);
    rollTriggerMod827(track,gateUs);
    if(noteOnNow(channel, note, vel)){
      if(!playing&&remixEmittingModel911)manualModStart06(track,255,gateUs);
      if(blockIndex21()==0)mod805Note(track,note,vel,gateUs);
      keyboardDrum823(channel,note,gateUs);
    }
    return;
  }
  if(trackPlayMode[linkOwner12(track,LINK_POLY12)]==TRACK_PLAY_MONO){for(auto&n:activeNotes)if(n.active&&n.track==track&&n.block21==blockIndex21()){MidiScope902 old(n.outputMask902,track);noteOffNow(n.channel,n.note);n.active=false;}}
  uint32_t offAtUs = micros() + gateUs;
  if (scheduleNoteOffAtUs(channel, note, offAtUs, false, false, track,255,remixEmittingModel911==&remixSource911)) {
    lockBegin817(track,lockPattern,lockTick,sourceNote,channel,note,gateUs);
    rollTriggerMod827(track,gateUs);
    if(noteOnNow(channel, note, vel)){if(!playing&&remixEmittingModel911)manualModStart06(track,255,gateUs);if(blockIndex21()==0)mod805Note(track,note,vel,gateUs);}
  }
}

void stopClipboardPreviewNotes(uint8_t previewSlot = 255) {
  for (uint16_t i = 0; i < MAX_ACTIVE_NOTES; i++) {
    if (!activeNotes[i].active || !activeNotes[i].preview ||
        (previewSlot != 255 && activeNotes[i].previewSlot != previewSlot)) continue;
    {MidiScope902 route(activeNotes[i].outputMask902,activeNotes[i].track);noteOffNow(activeNotes[i].channel, activeNotes[i].note);}
    activeNotes[i].active = false;
  }
}

void allNotesOff() {
  if(softTransport903){transportCut903();return;}
  keepDelayPaused903=false;
  pidiCancel827();
  recordClose820();
  keyboardLightsClear823();
  lockStop817();
  // IMPORTANT : ne couper que les notes rÃƒÆ’Ã†â€™Ãƒâ€ Ã¢â‚¬â„¢ÃƒÆ’Ã¢â‚¬Â ÃƒÂ¢Ã¢â€šÂ¬Ã¢â€žÂ¢ÃƒÆ’Ã†â€™ÃƒÂ¢Ã¢â€šÂ¬Ã‚Â ÃƒÆ’Ã‚Â¢ÃƒÂ¢Ã¢â‚¬Å¡Ã‚Â¬ÃƒÂ¢Ã¢â‚¬Å¾Ã‚Â¢ÃƒÆ’Ã†â€™Ãƒâ€ Ã¢â‚¬â„¢ÃƒÆ’Ã‚Â¢ÃƒÂ¢Ã¢â‚¬Å¡Ã‚Â¬Ãƒâ€¦Ã‚Â¡ÃƒÆ’Ã†â€™ÃƒÂ¢Ã¢â€šÂ¬Ã…Â¡ÃƒÆ’Ã¢â‚¬Å¡Ãƒâ€šÃ‚Â©ellement actives.
  // L'ancienne version envoyait 16 x 128 = 2048 Note Off ÃƒÆ’Ã†â€™Ãƒâ€ Ã¢â‚¬â„¢ÃƒÆ’Ã¢â‚¬Â ÃƒÂ¢Ã¢â€šÂ¬Ã¢â€žÂ¢ÃƒÆ’Ã†â€™ÃƒÂ¢Ã¢â€šÂ¬Ã‚Â ÃƒÆ’Ã‚Â¢ÃƒÂ¢Ã¢â‚¬Å¡Ã‚Â¬ÃƒÂ¢Ã¢â‚¬Å¾Ã‚Â¢ÃƒÆ’Ã†â€™Ãƒâ€ Ã¢â‚¬â„¢ÃƒÆ’Ã‚Â¢ÃƒÂ¢Ã¢â‚¬Å¡Ã‚Â¬Ãƒâ€¦Ã‚Â¡ÃƒÆ’Ã†â€™ÃƒÂ¢Ã¢â€šÂ¬Ã…Â¡ÃƒÆ’Ã¢â‚¬Å¡Ãƒâ€šÃ‚Â  chaque saut,
  // ce qui saturait la sortie MIDI DIN 31250 bauds pendant environ 2 secondes.
  uint8_t stoppedCount = 0;

  for (uint16_t i = 0; i < MAX_ACTIVE_NOTES; i++) {
    if (!activeNotes[i].active) continue;

    {MidiScope902 route(activeNotes[i].outputMask902,activeNotes[i].track);noteOffNow(activeNotes[i].channel, activeNotes[i].note);}
    activeNotes[i].active = false;
    stoppedCount++;
  }

  // Diagnostic lÃƒÆ’Ã†â€™Ãƒâ€ Ã¢â‚¬â„¢ÃƒÆ’Ã¢â‚¬Â ÃƒÂ¢Ã¢â€šÂ¬Ã¢â€žÂ¢ÃƒÆ’Ã†â€™ÃƒÂ¢Ã¢â€šÂ¬Ã‚Â ÃƒÆ’Ã‚Â¢ÃƒÂ¢Ã¢â‚¬Å¡Ã‚Â¬ÃƒÂ¢Ã¢â‚¬Å¾Ã‚Â¢ÃƒÆ’Ã†â€™Ãƒâ€ Ã¢â‚¬â„¢ÃƒÆ’Ã‚Â¢ÃƒÂ¢Ã¢â‚¬Å¡Ã‚Â¬Ãƒâ€¦Ã‚Â¡ÃƒÆ’Ã†â€™ÃƒÂ¢Ã¢â€šÂ¬Ã…Â¡ÃƒÆ’Ã¢â‚¬Å¡Ãƒâ€šÃ‚Â©ger : permet de confirmer qu'un saut ne coupe que quelques notes.
  if (stoppedCount > 0) {
    Serial.print(F("MIDI: notes actives coupees = "));
    Serial.println(stoppedCount);
  }
}

void serviceNoteOffs() {
  uint32_t now = micros();
  for (uint16_t i = 0; i < MAX_ACTIVE_NOTES; i++) {
    if (activeNotes[i].active && (int32_t)(now - activeNotes[i].offAtUs) >= 0) {
      {MidiScope902 route(activeNotes[i].outputMask902,activeNotes[i].track);noteOffNow(activeNotes[i].channel, activeNotes[i].note);}
      activeNotes[i].active = false;
    }
  }
  lockService817();
}

// ============================================================
// QUATRE PRESSE-PAPIERS : COPIE / TYPE / COLLAGE / PREVIEW
// ============================================================
const char* clipboardTypeCode(ClipboardContentType type) {
  if (type == CLIP_TYPE_NOTES) return "NOTES";
  if (type == CLIP_TYPE_SELECTION) return "SEL";
  if (type == CLIP_TYPE_SEQUENCE) return "SEQ";
  if (type == CLIP_TYPE_EFFECT) return "FX";
  if (type == CLIP_TYPE_REMIX) return "REMIX";
  if(type==CLIP_TYPE_TRACK12)return "TRACK";
  return "--";
}

FLASHMEM void sendClipboardMessage(uint8_t slot, ClipboardContentType type, const char* action) {
  DISPLAY_SERIAL.print(F("MSG|C"));
  DISPLAY_SERIAL.print(slot + 1);
  DISPLAY_SERIAL.print(' ');
  DISPLAY_SERIAL.print(clipboardTypeCode(type));
  DISPLAY_SERIAL.print(' ');
  DISPLAY_SERIAL.println(action);
}

uint8_t recentIndexForClipboardType(ClipboardContentType type) {
  for (uint8_t i = 0; i < 3; i++) {
    if (recentClipboardTypes[i] == type) return i;
  }
  return 0;
}

FLASHMEM void sendClipboardMenu() {
  char lines[4][21];
  uint8_t selectedNotes = 0;
  for (uint16_t step = 0; step < STEP_COUNT && selectedNotes < 2; step++) {
    if (selectedRangeVoice(selectedTrack, step) >= 0) selectedNotes++;
  }
  bool sequence = selectedNotes > 1;
  static const char* NOTE_LABEL[4] = {"NOTE ALL", "NOTE ONLY", "NOTE+PX", "PX ONLY"};
  static const char* SEQ_LABEL[4] = {"SEQ ALL", "SEQ NOTES", "SEQ+PX", "PX ONLY"};
  for (uint8_t row = 0; row < 4; row++) {
    snprintf(lines[row], sizeof(lines[row]), "%c C%c %-14.14s",
             row == (uint8_t)clipboardCopyScope[clipboardMenuSlot] ? '*' : ' ',
             '1' + clipboardMenuSlot, sequence ? SEQ_LABEL[row] : NOTE_LABEL[row]);
  }

  sendMenuScreen(lines[0], lines[1], lines[2], lines[3]);
}

void closeClipboardMenu() {
  if (!clipboardMenuActive) return;
  clipboardMenuActive = false;
  showGameScreen();
  displayValueDirty = true;
  displayBpmDirty = true;
}

FLASHMEM void openOrAdvanceClipboardMenu(uint8_t slot) {
  uint32_t now = millis();
  if (!clipboardMenuActive || clipboardMenuSlot != slot) {
    clipboardMenuActive = true;
    clipboardMenuSlot = slot;
    clipboardMenuRecentIndex = (uint8_t)clipboardCopyScope[slot];
  } else {
    clipboardMenuRecentIndex = (clipboardMenuRecentIndex + 1) % 4;
    clipboardCopyScope[slot] = (ClipboardCopyScope)clipboardMenuRecentIndex;
  }
  clipboardMenuLastActionMs = now;
  sendClipboardMenu();
}

FLASHMEM void captureTrackIntoClipboard(uint8_t slot) {
  if (slot < CLIPBOARD_COUNT) patternClipboard[slot].valid=false;
  if (slot >= CLIPBOARD_COUNT || selectedTrack >= TRACK_COUNT) return;
  ClipboardSlot &clip = clipboardSlots[slot];
  uint8_t track = selectedTrack;
  releaseRemixClipboard915(slot);
  memset(&clip, 0, sizeof(clip));
  memset(clipboardFine[slot], 0, sizeof(clipboardFine[slot]));
  clipboardFineCount[slot] = 0;
  uint16_t firstStep = STEP_COUNT;
  uint16_t lastStep = 0;
  bool range = selectionHasActiveNotes();
  if (range) {
    for (uint16_t step = 0; step < STEP_COUNT; step++) {
      if (!stepSelected[track][step] || !noteCountAtStep(track, step)) continue;
      int8_t selectedVoiceAtStep = selectedRangeVoice(track, step);
      (void)selectedVoiceAtStep;
      uint8_t voiceCount = noteCountAtStep(track, step);
      for (uint8_t voice = 0; voice < voiceCount; voice++) {
        if (!selectedNoteObject(track, step, voice)) continue;
        if (clipboardFineCount[slot] >= FINE_EVENTS_PER_TRACK) break;
        FineNoteEvent &event = clipboardFine[slot][clipboardFineCount[slot]++];
        event.active = true;
        event.step = step;
        event.logicalMicro = (int8_t)logicalMicroTick(track, step, voice);
        event.micro = effectiveMicroTick(track, step, voice);
        event.note = noteAtVoice(track, step, voice);
        event.vel = effectiveVel(track, step, voice);
        event.gate = effectiveGateTicks(track, step, voice);
        int16_t fine = fineIndexForVoice(track, step, voice);
        event.pxMask = fine >= 0 ? fineNote[track][fine].pxMask
                                : (uint16_t)((stepLockVel[track][step] ? PX_VEL : 0) |
                                  (stepLockPitch[track][step] ? PX_PITCH : 0) |
                                  (stepLockGate[track][step] ? PX_GATE : 0) |
                                  (stepLockTime[track][step] ? PX_TIME : 0) |
                                  ((uint16_t)stepRollLockMask[track][step] << PX_ROLL_SHIFT));
        event.roll = configuredRollSettings(track, step, voice);
        firstStep = min(firstStep, step);
        lastStep = max(lastStep, step);
      }
    }
  } else {
    int16_t sourceStep = selectedStep;
    uint8_t sourceVoice = selectedVoice;
    if (sourceStep < 0 && lastNoteValid && lastNoteTrack == track) {
      sourceStep = lastNoteStep;
      sourceVoice = lastNoteFineIndex >= 0
                      ? voiceForFineIndex(track, (uint16_t)sourceStep, (uint8_t)lastNoteFineIndex) : 0;
    }
    if (sourceStep >= 0 && stepActive[track][(uint16_t)sourceStep]) {
      FineNoteEvent &event = clipboardFine[slot][0];
      event.active = true;
      event.step = (uint16_t)sourceStep;
      event.logicalMicro = (int8_t)logicalMicroTick(track, (uint16_t)sourceStep, sourceVoice);
      event.micro = effectiveMicroTick(track, (uint16_t)sourceStep, sourceVoice);
      event.note = noteAtVoice(track, (uint16_t)sourceStep, sourceVoice);
      event.vel = effectiveVel(track, (uint16_t)sourceStep, sourceVoice);
      event.gate = effectiveGateTicks(track, (uint16_t)sourceStep, sourceVoice);
      int16_t fine = fineIndexForVoice(track, (uint16_t)sourceStep, sourceVoice);
      event.pxMask = fine >= 0 ? fineNote[track][fine].pxMask
                              : (uint16_t)((stepLockVel[track][sourceStep] ? PX_VEL : 0) |
                                (stepLockPitch[track][sourceStep] ? PX_PITCH : 0) |
                                (stepLockGate[track][sourceStep] ? PX_GATE : 0) |
                                (stepLockTime[track][sourceStep] ? PX_TIME : 0) |
                                ((uint16_t)stepRollLockMask[track][sourceStep] << PX_ROLL_SHIFT));
      event.roll = configuredRollSettings(track, (uint16_t)sourceStep, sourceVoice);
      clipboardFineCount[slot] = 1;
      firstStep = lastStep = (uint16_t)sourceStep;
    }
  }
  if (clipboardFineCount[slot] == 0) {
    sendClipboardMessage(slot, CLIP_TYPE_NONE, "NOTHING TO COPY");
    return;
  }

  ClipboardCopyScope scope = clipboardCopyScope[slot];
  for (uint8_t index = 0; index < clipboardFineCount[slot]; index++) {
    FineNoteEvent &event = clipboardFine[slot][index];
    event.step -= firstStep;
    if (scope == CLIP_COPY_ALL) event.pxMask = 0x03FF;
    else if (scope == CLIP_COPY_NOTE_ONLY) event.pxMask = PX_PITCH | PX_VEL | PX_GATE;
    else if (scope == CLIP_COPY_NOTE_PX) event.pxMask |= PX_PITCH | PX_VEL | PX_GATE;
  }
  clip.valid = true;
  clip.type = scope == CLIP_COPY_PX_ONLY ? CLIP_TYPE_EFFECT
            : clipboardFineCount[slot] > 1 ? CLIP_TYPE_SEQUENCE : CLIP_TYPE_NOTES;
  clip.sourceTrack = track;
  clip.startStep = 0;
  clip.lengthSteps = lastStep - firstStep + 1;
  clip.defaultVel = defaultVel[track];
  clip.defaultNote = defaultMidiNote[track];
  clip.defaultGateTicks = defaultGateTicks[track];
  clip.defaultMicroTick = defaultMicroTick[track];
  for (uint8_t index = 0; index < clipboardFineCount[slot]; index++) {
    FineNoteEvent &event = clipboardFine[slot][index];
    uint16_t step = event.step;
    if (step >= STEP_COUNT || clip.active[step]) continue;
    clip.active[step] = true;
    clip.vel[step] = event.vel;
    clip.note[step] = event.note;
    clip.gateTicks[step] = event.gate;
    clip.microTick[step] = event.micro;
    clip.lockVel[step] = (event.pxMask & PX_VEL) != 0;
    clip.lockPitch[step] = true;
    clip.lockGate[step] = (event.pxMask & PX_GATE) != 0;
    clip.lockTime[step] = (event.pxMask & PX_TIME) != 0;
  }
  contentCaptureClip18(slot,track,firstStep);
  sendClipboardMessage(slot, clip.type, "COPIED");
}

const char* clipboardPasteModeCode(ClipboardPasteMode mode) {
  return mode == CLIP_PASTE_OVERDUB ? "OVERDUB" : "ERASE";
}

FLASHMEM void sendPasteImpossible(ClipboardContentType type, const char* destination) {
  char content[21];
  char target[21];
  snprintf(content, sizeof(content), "CLIP %-15.15s", clipboardTypeCode(type));
  snprintf(target, sizeof(target), "DEST %-15.15s", destination);
  sendMenuScreen("PASTE IMPOSSIBLE", content, target, "");
  displayValueDirty = true;
  displayBpmDirty = true;
}

int16_t firstSelectedStepOnTrack(uint8_t track) {
  for (uint16_t step = 0; step < STEP_COUNT; step++) {
    if (stepSelected[track][step]) return (int16_t)step;
  }
  return -1;
}

void applyPxRecordToVoice(uint8_t track, uint16_t step, uint8_t voice,
                          const FineNoteEvent &record) {
  if (record.pxMask & PX_VEL) setCommittedStepValue(track, step, voice, EDIT_VEL, record.vel);
  if (record.pxMask & PX_PITCH) setCommittedStepValue(track, step, voice, EDIT_PITCH, record.note);
  if (record.pxMask & PX_GATE) setCommittedStepValue(track, step, voice, EDIT_GATE, record.gate);
  if (record.pxMask & PX_TIME) setCommittedStepValue(track, step, voice, EDIT_TIME, record.micro);
  int16_t fine = fineIndexForVoice(track, step, voice);
  for (uint8_t field = 0; field < 6; field++) {
    if ((record.pxMask & (1u << (PX_ROLL_SHIFT + field))) == 0) continue;
    if (fine >= 0) {
      setRollFieldValue(fineNote[track][fine].roll, field, rollFieldValue(record.roll, field));
      fineNote[track][fine].pxMask |= (uint16_t)(1u << (PX_ROLL_SHIFT + field));
    } else {
      {auto value906=rollRead906(track,step);setRollFieldValue(value906, field, rollFieldValue(record.roll, field));rollSet906(track,step,value906,stepRollLockMask[track][step]|(1u << field));}
    }
  }
}

void placeClipboardEvent(uint8_t track, uint32_t absoluteTick, const FineNoteEvent &record) {
  uint16_t step;
  int16_t logicalMicro;
  tickToStepAndMicro(track, absoluteTick, step, logicalMicro);
  if (step >= STEP_COUNT) return;
  FineNoteEvent placed = record;
  placed.logicalMicro = (int8_t)logicalMicro;
  placed.micro = record.micro;
  if (!stepActive[track][step]) {
    clearTrackStep(track, step);
    stepActive[track][step] = true;
    stepMidiNote[track][step] = placed.note;
    stepLockPitch[track][step] = true;
    stepLogicalMicro[track][step] = placed.logicalMicro;
    stepMicroTick[track][step] = indNote50(track,3);
    stepLockTime[track][step] = false;
    applyPxRecordToVoice(track, step, 0, placed);
    return;
  }
  if (trackPlayMode[linkOwner12(track,LINK_POLY12)] == TRACK_PLAY_MONO && logicalMicroTick(track, step, 0) == logicalMicro) {
    stepMidiNote[track][step] = placed.note;
    stepLockPitch[track][step] = true;
    applyPxRecordToVoice(track, step, 0, placed);
    return;
  }
  int16_t fine = addFineNote(track, step, placed.micro, placed.note, placed.logicalMicro);
  if (fine < 0) return;
  fineNote[track][fine] = placed;
  fineNote[track][fine].active = true;
  fineNote[track][fine].step = step;
  fineNote[track][fine].pxMask |= PX_PITCH;
}

FLASHMEM void sendClipboardPasteMenu(uint8_t slot) {
  char line0[21];
  char line1[21];
  char line2[21];
  snprintf(line0, sizeof(line0), "PASTE C%c STEP %02u", '1' + slot,
           selectedStep >= 0 ? selectedStep + 1 : currentStep + 1);
  snprintf(line1, sizeof(line1), "%c ERASE", clipboardPasteMode[slot] == CLIP_PASTE_ERASE ? '*' : ' ');
  snprintf(line2, sizeof(line2), "%c OVERDUB", clipboardPasteMode[slot] == CLIP_PASTE_OVERDUB ? '*' : ' ');
  sendMenuScreen(line0, line1, line2,
                 "ERASE replaces the range; OVERDUB keeps destination gaps");
}

void pasteClipboardToSelectedTrack(uint8_t slot) {if(slot<CLIPBOARD_COUNT&&patternClipboard[slot].valid){uint8_t t=selectedTrack,p=activePattern[t];if(patternMode&&lastPatternValid){t=lastPatternTrack;p=lastPatternIndex;}pastePatternClipboard(slot,t,p);return;}if(trackPaste12(slot))return;
  if(contentEffect18(slot)||contentPaste18(slot))return;
  if(slot<CLIPBOARD_COUNT&&clipboardSlots[slot].type==CLIP_TYPE_REMIX){DISPLAY_SERIAL.println("MSG|CLIP4 REMIX: PASTE ON TARGET");return;}
  if (slot >= CLIPBOARD_COUNT || selectedTrack >= TRACK_COUNT) return;
  discardNoteRedo();
  ClipboardSlot &clip = clipboardSlots[slot];
  if (!clip.valid) {
    sendClipboardMessage(slot, CLIP_TYPE_NONE, "EMPTY");
    return;
  }
  if (clipboardFineCount[slot] > 0) {
    if (clip.type == CLIP_TYPE_EFFECT) {
      if (!selectionHasActiveNotes() && selectedStep < 0) {
        sendPasteImpossible(clip.type, "NOTES");
        return;
      }
      FineNoteEvent &record = clipboardFine[slot][0];
      if (selectionHasActiveNotes()) {
        for (uint16_t step = 0; step < STEP_COUNT; step++) {
          int8_t voice = selectedRangeVoice(selectedTrack, step);
          if (voice >= 0) applyPxRecordToVoice(selectedTrack, step, (uint8_t)voice, record);
        }
      } else {
        applyPxRecordToVoice(selectedTrack, (uint16_t)selectedStep, selectedVoice, record);
      }
    } else {
      int16_t firstSelected = firstSelectedStepOnTrack(selectedTrack);
      uint32_t anchorTick = firstSelected >= 0 ? (uint32_t)firstSelected * TICKS_PER_STEP
                           : lastGridTickValid ? lastGridTick
                           : selectedStep >= 0 ? (uint32_t)selectedStep * TICKS_PER_STEP
                                               : (uint32_t)currentStep * TICKS_PER_STEP;
      for (uint8_t index = 0; index < clipboardFineCount[slot]; index++) {
        FineNoteEvent &record = clipboardFine[slot][index];
        int32_t destinationTick = (int32_t)anchorTick + (int32_t)record.step * TICKS_PER_STEP +
                                  record.logicalMicro;
        if (destinationTick < 0) continue;
        placeClipboardEvent(selectedTrack, (uint32_t)destinationTick, record);
      }
    }
    invalidateStepTriggers();
    neoDisplayDirty = true;
    displayValueDirty = true;
    sendClipboardMessage(slot, clip.type, "PASTED");
    return;
  }
  if (clip.type == CLIP_TYPE_EFFECT) {
    sendPasteImpossible(clip.type, "NOTES");
    return;
  }
  uint8_t track = selectedTrack;
  uint16_t anchor = selectedStep >= 0 ? (uint16_t)selectedStep : currentStep;
  ClipboardPasteMode mode = clipboardPasteMode[slot];
  if (clip.type == CLIP_TYPE_SEQUENCE) {
    defaultVel[track] = clip.defaultVel;
    defaultMidiNote[track] = clip.defaultNote;
    defaultGateTicks[track] = clip.defaultGateTicks;
    defaultMicroTick[track] = clip.defaultMicroTick;
    for (uint16_t offset = 0; offset < clip.lengthSteps; offset++) {
      uint16_t source = clip.startStep + offset;
      uint16_t destination = anchor + offset;
      if (source >= STEP_COUNT || destination >= STEP_COUNT) break;
      if (mode == CLIP_PASTE_OVERDUB && !clip.active[source]) continue;
      stepActive[track][destination] = clip.active[source];
      stepVel[track][destination] = clip.vel[source];
      stepMidiNote[track][destination] = clip.note[source];
      stepGateTicks[track][destination] = clip.gateTicks[source];
      stepLogicalMicro[track][destination] = 0;
      stepMicroTick[track][destination] = clip.microTick[source];
      stepLockVel[track][destination] = clip.lockVel[source];
      stepLockPitch[track][destination] = clip.lockPitch[source];
      stepLockGate[track][destination] = clip.lockGate[source];
      stepLockTime[track][destination] = clip.lockTime[source];
    }
  } else if (clip.type == CLIP_TYPE_NOTES || clip.type == CLIP_TYPE_SELECTION) {
    defaultMidiNote[track] = clip.defaultNote;
    for (uint16_t offset = 0; offset < clip.lengthSteps; offset++) {
      uint16_t source = clip.startStep + offset;
      uint16_t destination = anchor + offset;
      if (source >= STEP_COUNT || destination >= STEP_COUNT) break;
      if (mode == CLIP_PASTE_OVERDUB && !clip.active[source]) continue;
      stepActive[track][destination] = clip.active[source];
      stepVel[track][destination] = clip.vel[source];
      stepMidiNote[track][destination] = clip.note[source];
      stepGateTicks[track][destination] = clip.gateTicks[source];
      stepLogicalMicro[track][destination] = 0;
      stepMicroTick[track][destination] = clip.microTick[source];
      stepLockVel[track][destination] = clip.lockVel[source];
      stepLockPitch[track][destination] = clip.lockPitch[source];
      stepLockGate[track][destination] = clip.lockGate[source];
      stepLockTime[track][destination] = clip.lockTime[source];
    }
  }

  invalidateStepTriggers();
  neoDisplayDirty = true;
  lastNeoRefreshMs = millis() - NEO_MIN_REFRESH_MS;
  displayValueDirty = true;
  sendClipboardMessage(slot, clip.type, clipboardPasteModeCode(mode));
}

const char* clipboardPreviewModeCode(ClipboardPreviewMode mode) {
  if (mode == CLIP_PREVIEW_CUT) return "CUT";
  if (mode == CLIP_PREVIEW_ADD) return "ADD";
  return "QUEUE";
}

void clearClipboardPreviewQueue() {
  clipboardPreviewQueueHead = 0;
  clipboardPreviewQueueTail = 0;
  clipboardPreviewQueueCount = 0;
}

FLASHMEM void beginClipboardPreview(uint8_t slot, uint8_t targetTrack) {
  if(slot<CLIPBOARD_COUNT&&clipboardSlots[slot].type==CLIP_TYPE_REMIX){DISPLAY_SERIAL.println("MSG|CLIP4 REMIX: PASTE ON TARGET");return;}
  ClipboardSlot &clip = slot == 255 ? loadPreviewBuffer : clipboardSlots[slot];
  clipboardPreviewUsesLoadBuffer = slot == 255;
  clipboardPreviewActive = true;
  clipboardPreviewSlot = slot;
  clipboardPreviewTargetTrack = targetTrack;
  clipboardPreviewTick = 0;
  clipboardPreviewEndTick = max((uint32_t)TICKS_PER_STEP,
                                (uint32_t)clip.lengthSteps * TICKS_PER_STEP);
  if (clip.type == CLIP_TYPE_NOTES || clip.type == CLIP_TYPE_SELECTION) {
    uint8_t activeCount = 0;
    uint8_t noteStep = clip.startStep;
    for (uint16_t step = clip.startStep;
         step < STEP_COUNT && step < clip.startStep + clip.lengthSteps; step++) {
      if (!clip.active[step]) continue;
      activeCount++;
      noteStep = step;
    }
    if (activeCount == 1) {
      int16_t micro = clip.lockTime[noteStep] ? clip.microTick[noteStep] : clip.defaultMicroTick;
      uint16_t gate = clip.gateTicks[noteStep];
      int32_t endTick = max((int32_t)1, (int32_t)micro + gate);
      clipboardPreviewEndTick = (uint16_t)min((int32_t)TICKS_PER_MEASURE, endTick);
    }
  }
  clipboardPreviewFirstTick = true;
  for (uint16_t step = 0; step < STEP_COUNT; step++) clipboardPreviewTriggered[step] = false;
  if (!playing) internalClockRunning = true;
}

void stopClipboardPreview(bool clearQueue, bool stopNotes) {contentPreviewClear18();
  clipboardPreviewActive = false;
  if (clearQueue) {
    clearClipboardPreviewQueue();
    for (uint8_t slot = 0; slot < CLIPBOARD_COUNT; slot++) clipboardLivePreview[slot].active = false;
  }
  if (stopNotes) stopClipboardPreviewNotes();
  if (!playing && !anyLiveClipboardPreview()) internalClockRunning = false;
}

bool anyLiveClipboardPreview() {
  for (uint8_t slot = 0; slot < CLIPBOARD_COUNT; slot++) {
    if (clipboardLivePreview[slot].active) return true;
  }
  return false;
}

bool previewSuppressesVoice(uint8_t track, uint16_t step, uint8_t voice) {
  if (pendingDeleteNote(track, step, voice)) return true;
  if (!anyLiveClipboardPreview() || track != selectedTrack || !selectionHasActiveNotes()) return false;
  int8_t selected = selectedRangeVoice(track, step);
  return selected >= 0 && voice == (uint8_t)selected;
}

FLASHMEM void beginLiveClipboardPreview(uint8_t slot, uint8_t targetTrack) {
  ClipboardSlot &clip = clipboardSlots[slot];
  ClipboardLivePreview &live = clipboardLivePreview[slot];
  stopClipboardPreviewNotes(slot);
  live.active = true;
  live.targetTrack = targetTrack;
  live.tick = 0;
  live.endTick = max((uint32_t)TICKS_PER_STEP, (uint32_t)clip.lengthSteps * TICKS_PER_STEP);
  live.firstTick = true;
  memset(live.triggered, 0, sizeof(live.triggered));
  if (!playing) internalClockRunning = true;
}

void processLiveClipboardPreviews() {
  for (uint8_t slot = 0; slot < CLIPBOARD_COUNT; slot++) {
    ClipboardLivePreview &live = clipboardLivePreview[slot];
    if (!live.active) continue;
    if (live.firstTick) live.firstTick = false;
    else if (++live.tick >= live.endTick) {
      live.active = false;
      continue;
    }
    ClipboardSlot &clip = clipboardSlots[slot];
    uint8_t channel = midiChannelForTrack(live.targetTrack);
    if(contentPreviewTick18(slot,live.targetTrack,live.tick,live.endTick))continue;
    if (clipboardFineCount[slot] > 0) {
      for (uint8_t index = 0; index < clipboardFineCount[slot]; index++) {
        if (live.triggered[index]) continue;
        FineNoteEvent &event = clipboardFine[slot][index];
        int16_t timeOffset = event.micro;
        int32_t eventTick = (int32_t)event.step * TICKS_PER_STEP +
                            event.logicalMicro + timeOffset;
        while (eventTick < 0) eventTick += live.endTick;
        while (eventTick >= live.endTick) eventTick -= live.endTick;
        if ((uint16_t)eventTick != live.tick) continue;
        live.triggered[index] = true;
        uint8_t vel = event.vel;
        uint16_t gate = event.gate;
        uint32_t gateUs = (uint32_t)((float)gate * tickDurationMs() * 1000.0f);
        triggerPreviewNoteWithFixedGate(channel, event.note, vel,
                                        (uint32_t)clampInt((int)gateUs, 1000, 20000000), slot,live.targetTrack);
      }
      continue;
    }
    for (uint16_t step = clip.startStep;
         step < STEP_COUNT && step < clip.startStep + clip.lengthSteps; step++) {
      if (!clip.active[step] || live.triggered[step]) continue;
      int16_t micro = clip.lockTime[step] ? clip.microTick[step] : clip.defaultMicroTick;
      int32_t eventTick = (int32_t)(step - clip.startStep) * TICKS_PER_STEP + micro;
      while (eventTick < 0) eventTick += live.endTick;
      while (eventTick >= live.endTick) eventTick -= live.endTick;
      if ((uint16_t)eventTick != live.tick) continue;
      live.triggered[step] = true;
      uint8_t note = clip.lockPitch[step] ? clip.note[step] : clip.defaultNote;
      uint8_t vel = clip.vel[step];
      uint16_t gate = clip.gateTicks[step];
      uint32_t gateUs = (uint32_t)((float)gate * tickDurationMs() * 1000.0f);
      triggerPreviewNoteWithFixedGate(channel, note, vel,
                                      (uint32_t)clampInt((int)gateUs, 1000, 20000000), slot,live.targetTrack);
    }
  }
  if (!playing && !clipboardPreviewActive && !anyLiveClipboardPreview()) internalClockRunning = false;
}

void startClipboardPreview(uint8_t slot) {
 if(slot<CLIPBOARD_COUNT&&patternClipboard[slot].valid){startPatternPreview36(slot);return;}
  if (slot >= CLIPBOARD_COUNT) return;
  ClipboardSlot &clip = clipboardSlots[slot];
  if (!clip.valid) {
    sendClipboardMessage(slot, CLIP_TYPE_NONE, "EMPTY");
    return;
  }
  if (clip.type == CLIP_TYPE_EFFECT) {
    if (!selectionHasActiveNotes() || clipboardFineCount[slot] == 0) {
      sendPasteImpossible(clip.type, "SELECTION");
      return;
    }
    stopClipboardPreviewNotes(slot);
    FineNoteEvent &record = clipboardFine[slot][0];
    for (uint16_t step = 0; step < STEP_COUNT; step++) {
      int8_t voice = selectedRangeVoice(selectedTrack, step);
      if (voice < 0) continue;
      uint8_t note = (record.pxMask & PX_PITCH) ? record.note
                                                : audibleNoteAtVoice(selectedTrack, step, (uint8_t)voice);
      uint8_t vel = (record.pxMask & PX_VEL) ? record.vel
                                             : audibleVel(selectedTrack, step, (uint8_t)voice);
      uint16_t gate = (record.pxMask & PX_GATE) ? record.gate
                                                : audibleGateTicks(selectedTrack, step, (uint8_t)voice);
      uint32_t gateUs = (uint32_t)((float)gate * tickDurationMs() * 1000.0f);
      triggerPreviewNoteWithFixedGate(midiChannelForTrack(selectedTrack), note, vel,
                                      (uint32_t)clampInt((int)gateUs, 1000, 20000000), slot);
    }
    sendClipboardMessage(slot, clip.type, "NOW");
    return;
  }

  // 5A1 : aucune pression n'est mise en attente. Une nouvelle pression du
  // meme slot remplace son preview; des slots differents restent simultanes.
  beginLiveClipboardPreview(slot, selectedTrack);
  sendClipboardMessage(slot, clip.type, "NOW");
}

void processClipboardPreviewTick() {
  processLiveClipboardPreviews();
  if (!clipboardPreviewActive) return;
  if (clipboardPreviewFirstTick) {
    clipboardPreviewFirstTick = false;
  } else {
    clipboardPreviewTick++;
    if (clipboardPreviewTick >= clipboardPreviewEndTick) {
      clipboardPreviewActive = false;
      contentPreviewClear18();
      if (clipboardPreviewQueueCount > 0) {
        uint8_t nextSlot = clipboardPreviewQueueSlot[clipboardPreviewQueueHead];
        uint8_t nextTrack = clipboardPreviewQueueTrack[clipboardPreviewQueueHead];
        clipboardPreviewQueueHead = (clipboardPreviewQueueHead + 1) % CLIPBOARD_PREVIEW_QUEUE_SIZE;
        clipboardPreviewQueueCount--;
        beginClipboardPreview(nextSlot, nextTrack);
      } else if (!playing) {
        internalClockRunning = false;
      }
      return;
    }
  }

  ClipboardSlot &clip = clipboardPreviewUsesLoadBuffer
                          ? loadPreviewBuffer : clipboardSlots[clipboardPreviewSlot];
  uint8_t channel = midiChannelForTrack(clipboardPreviewTargetTrack);
  if(contentPreviewTick18(clipboardPreviewUsesLoadBuffer?255:clipboardPreviewSlot,clipboardPreviewTargetTrack,clipboardPreviewTick,clipboardPreviewEndTick))return;
  if (clipboardPreviewUsesLoadBuffer && loadPreviewFineCount > 0) {
    for (uint8_t index = 0; index < loadPreviewFineCount; index++) {
      if (clipboardPreviewTriggered[index]) continue;
      FineNoteEvent &event = loadPreviewFine[index];
      int16_t timeOffset = (event.pxMask & PX_TIME) ? event.micro : clip.defaultMicroTick;
      int32_t eventTick = (int32_t)event.step * TICKS_PER_STEP +
                          event.logicalMicro + timeOffset;
      while (eventTick < 0) eventTick += clipboardPreviewEndTick;
      while (eventTick >= clipboardPreviewEndTick) eventTick -= clipboardPreviewEndTick;
      if ((uint16_t)eventTick != clipboardPreviewTick) continue;
      clipboardPreviewTriggered[index] = true;
      uint8_t note = (event.pxMask & PX_PITCH) ? event.note : clip.defaultNote;
      uint8_t vel = (event.pxMask & PX_VEL) ? event.vel : clip.defaultVel;
      uint16_t gate = (event.pxMask & PX_GATE) ? event.gate : clip.defaultGateTicks;
      uint32_t gateUs = (uint32_t)((float)gate * tickDurationMs() * 1000.0f);
      triggerPreviewNoteWithFixedGate(channel, note, vel,
                                      (uint32_t)clampInt((int)gateUs, 1000, 20000000));
    }
    return;
  }
  for (uint16_t step = clip.startStep;
       step < STEP_COUNT && step < clip.startStep + clip.lengthSteps;
       step++) {
    if (!clip.active[step] || clipboardPreviewTriggered[step]) continue;
    int16_t micro = clip.lockTime[step] ? clip.microTick[step] : clip.defaultMicroTick;
    int32_t eventTick = (int32_t)(step - clip.startStep) * TICKS_PER_STEP + micro;
    while (eventTick < 0) eventTick += clipboardPreviewEndTick;
    while (eventTick >= clipboardPreviewEndTick) eventTick -= clipboardPreviewEndTick;
    if ((uint16_t)eventTick != clipboardPreviewTick) continue;

    clipboardPreviewTriggered[step] = true;
    uint8_t note = clip.lockPitch[step] ? clip.note[step] : clip.defaultNote;
    uint8_t vel = clip.vel[step];
    uint16_t gate = clip.gateTicks[step];
    uint32_t gateUs = (uint32_t)((float)gate * tickDurationMs() * 1000.0f);
    gateUs = (uint32_t)clampInt((int)gateUs, 1000, 20000000);
    triggerPreviewNoteWithFixedGate(channel, note, vel, gateUs);
  }
}

FLASHMEM void handleClipboardButtonEdge(uint8_t buttonIndex, bool pressed) {
 if(controller940)return;
  InputScope815 input(I815_HC,CLIPBOARD_BUTTON_BITS[buttonIndex],pressed);
  if (grooveEditMode && patternMode){grooveListClipboard40(buttonIndex,pressed);return;}
  if (grooveEditMode) {
    if(!pressed||buttonIndex>=CLIPBOARD_BUTTON_COUNT||patternMode)return;
    static int8_t sheets23[CLIPBOARD_COUNT][GROOVE_STEPS][5];static uint8_t shapes23[CLIPBOARD_COUNT][2];static bool valid23[CLIPBOARD_COUNT]={};
    uint8_t slot=buttonIndex/CLIPBOARD_ACTION_COUNT,action=buttonIndex%CLIPBOARD_ACTION_COUNT;
    if(action==CLIP_ACTION_COPY){memcpy(sheets23[slot],grooveOffset[editedGroove-1],sizeof(sheets23[slot]));memcpy(shapes23[slot],grooveShape16[editedGroove-1],2);valid23[slot]=true;DISPLAY_SERIAL.println("MSG|GROOVE COPIED");}
    if(action==CLIP_ACTION_PASTE&&valid23[slot]){memcpy(grooveOffset[editedGroove-1],sheets23[slot],sizeof(sheets23[slot]));memcpy(grooveShape16[editedGroove-1],shapes23[slot],2);mod805Invalidate();invalidateStepTriggers();sendGroovePacket();neoDisplayDirty=true;DISPLAY_SERIAL.println("MSG|GROOVE PASTED");}
    return;
  }
  if (deleteButtonActive) return;
  if (buttonIndex >= CLIPBOARD_BUTTON_COUNT) return;
  uint8_t slot = buttonIndex / CLIPBOARD_ACTION_COUNT;
  ClipboardAction action = (ClipboardAction)(buttonIndex % CLIPBOARD_ACTION_COUNT);

  if (debugTestMode == DEBUG_TEST_CLIPBOARD) {
    static const char* ACTION_NAME[4] = {"COPY", "SELECT", "PASTE", "PREVIEW"};
    char line0[21], line1[21], line2[21], small0[17], small1[17];
    snprintf(line0, sizeof(line0), "CLIP TEST C%u %s", slot + 1, ACTION_NAME[action]);
    snprintf(line1, sizeof(line1), "HC165 BIT%u", CLIPBOARD_BUTTON_BITS[buttonIndex]);
    snprintf(line2, sizeof(line2), "INDEX%u %s", buttonIndex, pressed ? "ON" : "OFF");
    snprintf(small0, sizeof(small0), "C%u %s", slot + 1, ACTION_NAME[action]);
    snprintf(small1, sizeof(small1), "B%u %s", CLIPBOARD_BUTTON_BITS[buttonIndex], pressed ? "ON" : "OFF");
    sendDebugDisplay(line0, line1, line2, "", small0, small1);
    Serial.print(F("DBG,CLIPBOARD,SLOT=")); Serial.print(slot + 1);
    Serial.print(F(",ACTION=")); Serial.print(ACTION_NAME[action]);
    Serial.print(F(",INDEX=")); Serial.print(buttonIndex);
    Serial.print(F(",HC165_BIT=")); Serial.print(CLIPBOARD_BUTTON_BITS[buttonIndex]);
    Serial.print(F(",PRESSED=")); Serial.println(pressed ? 1 : 0);
    return;
  }
  if (debugTestMode != DEBUG_TEST_NONE) return;
  if(trackMenuActive&&!fileBrowserMode&&!saveMenuActive&&!loadTypeMenuActive&&!hasSelectedSteps()&&selectedStep<0){if(pressed&&action==CLIP_ACTION_COPY)trackCopy12(slot);else if(pressed&&action==CLIP_ACTION_PASTE)trackPaste12(slot);return;}
  if (menuNavigationOnly() && !clipboardMenuActive && !clipboardPasteMenuActive &&
      fileBrowserMode != FILE_BROWSER_LOAD) return;

  if (fileBrowserMode == FILE_BROWSER_LOAD) {
    if (!pressed) return;
    if (action == CLIP_ACTION_COPY) {
      loadSelectedFileIntoClipboard(slot);
      renderFileBrowser();
    } else if (action == CLIP_ACTION_PREVIEW) {
      previewSelectedLoadFile(selectedTrack);
      renderFileBrowser();
    }
    return;
  }

  if (patternMode && action == CLIP_ACTION_COPY) {
    if (pressed) capturePatternSelectionIntoClipboard(slot);
    return;
  }

  if (patternMode && action == CLIP_ACTION_PASTE && patternPasteTargetHeld) {
    if (pressed) {
      patternPasteTargetConsumed = pastePatternClipboard(slot, patternPasteTargetTrack,
                                                         patternPasteTargetPattern);
    }
    return;
  }

  if (action == CLIP_ACTION_PASTE) {
    if (pressed) {
      clipboardPasteDown[slot] = true;
      clipboardPasteHoldMenu[slot] = false;
      clipboardPasteDownAtMs[slot] = millis();
      closeClipboardMenu();
    } else if (clipboardPasteDown[slot]) {
      clipboardPasteDown[slot] = false;
      if (clipboardPasteMenuActive && clipboardPasteMenuSlot == slot) {
        clipboardPasteMenuActive = false;
        DISPLAY_SERIAL.println(F("UI|GAME"));
      }
      pasteClipboardToSelectedTrack(slot);
    }
    return;
  }

  if (!pressed) return;
  if (action == CLIP_ACTION_SELECT) {
    if (clipboardPasteMenuActive && clipboardPasteMenuSlot == slot) {
      clipboardPasteMode[slot] = clipboardPasteMode[slot] == CLIP_PASTE_ERASE
                                   ? CLIP_PASTE_OVERDUB : CLIP_PASTE_ERASE;
      sendClipboardPasteMenu(slot);
      return;
    }
    openOrAdvanceClipboardMenu(slot);
    return;
  }

  closeClipboardMenu();
  if (action == CLIP_ACTION_COPY) {
    captureTrackIntoClipboard(slot);
  } else if (action == CLIP_ACTION_PREVIEW) {
    startClipboardPreview(slot);
  }
}

void serviceClipboardUi() {
  if(controller940)return;
  uint32_t now = millis();
  for (uint8_t slot = 0; slot < CLIPBOARD_COUNT; slot++) {
    if (!clipboardPasteDown[slot] || clipboardPasteHoldMenu[slot]) continue;
    if ((uint32_t)(now - clipboardPasteDownAtMs[slot]) < clipboardPasteHoldMs) continue;
    clipboardPasteHoldMenu[slot] = true;
    clipboardPasteMenuActive = true;
    clipboardPasteMenuSlot = slot;
    sendClipboardPasteMenu(slot);
  }
  // 1B1 : ce menu reste ouvert jusqu'a une annulation ou une validation.
  // Une expiration rendait le choix impossible pendant une lecture attentive.
}

// ============================================================
// SAUVEGARDE SD : .NPX / .SEQ / .PAT
// ============================================================
bool makeUniqueSavePath(const char* folder, const char* prefix, const char* extension,
                        char* path, size_t pathSize) {
  if (!storageWriteAvailable) return false;
  if(saveWork905[0]){if(strlen(saveWork905)>=pathSize)return false;strcpy(path,saveWork905);return true;}
  if (fileBrowserMode == FILE_BROWSER_SAVE && fileBrowserName[0] != '\0') {
    SD.mkdir(fileBrowserPath);
    snprintf(path, pathSize, "%s/%s.%s", fileBrowserPath, fileBrowserName, extension);
    if (!SD.exists(path)) return true;
    return false;
  }
  SD.mkdir("/URBANKAOZ");
  char directory[32];
  snprintf(directory, sizeof(directory), "/URBANKAOZ/%s", folder);
  SD.mkdir(directory);
  for (uint16_t number = 1; number < 10000; number++) {
    snprintf(path, pathSize, "%s/%s%04u.%s", directory, prefix, number, extension);
    if (!SD.exists(path)) return true;
  }
  return false;
}

void writeSavedStep(File &file, uint16_t index, bool active, uint8_t note, uint8_t velocity,
                    uint16_t gate, int16_t micro, bool lockPitch, bool lockVel,
                    bool lockGate, bool lockTime) {
  file.print(index);
  file.print(','); file.print(active ? 1 : 0);
  file.print(','); file.print(note);
  file.print(','); file.print(velocity);
  file.print(','); file.print(gate);
  file.print(','); file.print(micro);
  file.print(','); file.print(lockPitch ? 1 : 0);
  file.print(','); file.print(lockVel ? 1 : 0);
  file.print(','); file.print(lockGate ? 1 : 0);
  file.print(','); file.println(lockTime ? 1 : 0);
}

void writeSavedFine(File &file, uint16_t index, const FineNoteEvent &event) {
  file.print(F("F,")); file.print(index);
  file.print(','); file.print(event.logicalMicro);
  file.print(','); file.print(event.micro);
  file.print(','); file.print(event.note);
  file.print(','); file.print(event.vel);
  file.print(','); file.print(event.gate);
  file.print(','); file.print(event.pxMask);
  file.print(','); file.print(event.roll.start);
  file.print(','); file.print(event.roll.speed);
  file.print(','); file.print(event.roll.accel);
  file.print(','); file.print(event.roll.expo);
  file.print(','); file.print(event.roll.curve);
  file.print(','); file.println(event.roll.level);
}

void writeTrackVoices(File &file, uint8_t track, uint16_t first, uint16_t last,
                      bool selectedOnly) {
  if (track >= TRACK_COUNT || first > last) return;
  for (uint16_t step = first; step <= last && step < STEP_COUNT; step++) {
    if (!stepActive[track][step]) continue;
    if (selectedOnly) {
      int8_t voice = selectedRangeVoice(track, step);
      if (voice < 0) continue;
      FineNoteEvent event = {};
      if (captureNoteRecord(track, step, (uint8_t)voice, event)) {
        writeSavedFine(file, step - first, event);
      }
      continue;
    }
    for (uint8_t voice = 0; voice < noteCountAtStep(track, step); voice++) {
      FineNoteEvent event = {};
      if (captureNoteRecord(track, step, voice, event)) writeSavedFine(file, step - first, event);
    }
  }
}

FLASHMEM void sendSaveResult(const char* path, bool ok) {
  if (!ok) {
    DISPLAY_SERIAL.println(sdAvailable ? F("MSG|SAVE ERROR") : F("MSG|SD MISSING"));
    return;
  }
  rememberFileName816(path);
  const char* name = strrchr(path, '/');
  DISPLAY_SERIAL.print(F("MSG|"));
  DISPLAY_SERIAL.print(name ? name + 1 : path);
  DISPLAY_SERIAL.println(F(" SAVED"));
}

FLASHMEM bool saveTrackNoteNpl(uint8_t track,uint16_t step,char*path,size_t size){(void)step;return contentSave18(1,track,false,path,size);}

FLASHMEM bool saveClipboardNoteNpl(uint8_t slot, uint16_t step, char* path, size_t pathSize) {
  if (slot >= CLIPBOARD_COUNT || step >= STEP_COUNT) return false;
  ClipboardSlot &clip = clipboardSlots[slot];
  if (!clip.valid || !clip.active[step]) return false;
  if (!makeUniqueSavePath("NPX", "CLIP", "NPX", path, pathSize)) return false;
  File file = SD.open(path, FILE_WRITE);
  if (!file) return false;
  file.println(F("URBANKAOZ_NPX_V2"));
  file.print(F("clipboard=")); file.println(slot + 1);
  file.print(F("source_track=")); file.println(clip.sourceTrack + 1);
  file.println(F("index,active,note,velocity,gate,micro,lockPitch,lockVel,lockGate,lockTime"));
  writeSavedStep(file, 0, true, clip.note[step], clip.vel[step], clip.gateTicks[step],
                 clip.microTick[step], clip.lockPitch[step], clip.lockVel[step],
                 clip.lockGate[step], clip.lockTime[step]);
  if (clipboardFineCount[slot] > 0) writeSavedFine(file, 0, clipboardFine[slot][0]);
  file.close();
  return true;
}

FLASHMEM bool saveTrackSequence(uint8_t track,char*path,size_t size){return contentSave18(2,track,false,path,size);}

FLASHMEM bool saveSelectedSequence(uint8_t track,char*path,size_t size){return contentSave18(2,track,true,path,size);}

FLASHMEM bool saveClipboardSequence(uint8_t slot, char* path, size_t pathSize) {
  if (slot >= CLIPBOARD_COUNT || !clipboardSlots[slot].valid) return false;
  ClipboardSlot &clip = clipboardSlots[slot];
  if (!makeUniqueSavePath("SEQ", "CLIP", "SEQ", path, pathSize)) return false;
  File file = SD.open(path, FILE_WRITE);
  if (!file) return false;
  file.println(F("URBANKAOZ_SEQ_V3"));
  file.print(F("clipboard=")); file.println(slot + 1);
  file.print(F("content_type=")); file.println(clipboardTypeCode(clip.type));
  file.print(F("length_steps=")); file.println(clip.lengthSteps);
  file.print(F("source_track=")); file.println(clip.sourceTrack + 1);
  file.print(F("default_note=")); file.println(clip.defaultNote);
  file.print(F("default_velocity=")); file.println(clip.defaultVel);
  file.print(F("default_gate=")); file.println(clip.defaultGateTicks);
  file.print(F("default_micro=")); file.println(clip.defaultMicroTick);
  file.println(F("index,active,note,velocity,gate,micro,lockPitch,lockVel,lockGate,lockTime"));
  for (uint16_t offset = 0; offset < clip.lengthSteps; offset++) {
    uint16_t step = clip.startStep + offset;
    if (step >= STEP_COUNT) break;
    writeSavedStep(file, offset, clip.active[step], clip.note[step], clip.vel[step],
                   clip.gateTicks[step], clip.microTick[step], clip.lockPitch[step],
                   clip.lockVel[step], clip.lockGate[step], clip.lockTime[step]);
  }
  for (uint8_t index = 0; index < clipboardFineCount[slot]; index++) {
    writeSavedFine(file, clipboardFine[slot][index].step, clipboardFine[slot][index]);
  }
  file.close();
  return true;
}

FLASHMEM bool savePattern(char*path,size_t size){return contentSave18(3,selectedTrack,false,path,size);}

static const uint32_t FULL_PROJECT_MAGIC = 0x554B5031UL;
static const uint16_t FULL_PROJECT_VERSION = 35;
struct FullProjectHeader {
  uint32_t magic;
  uint16_t version;
  float bpm;
  uint8_t selectedTrack;
  uint8_t activePattern[TRACK_COUNT];
  uint8_t defaultNote[TRACK_COUNT];
  uint8_t defaultVelocity[TRACK_COUNT];
  uint16_t defaultGate[TRACK_COUNT];
  int16_t defaultMicro[TRACK_COUNT];
  uint8_t midiChannel[TRACK_COUNT];
  int16_t pitchBend[TRACK_COUNT];
  TrackRollSettings roll[TRACK_COUNT];
  int8_t autoSelect[TRACK_COUNT];
  int8_t multiEdit[TRACK_COUNT];
  int8_t notePreview[TRACK_COUNT];
  uint8_t polyphony[TRACK_COUNT];
  int8_t noteView[TRACK_COUNT];
  int8_t colours[TRACK_COUNT][NEO_COLOR_ROLE_COUNT];
  uint16_t loopStart[TRACK_COUNT];
  uint16_t loopEnd[TRACK_COUNT];
  uint8_t patternChangeNext;
  uint8_t globalViewMode;
  uint8_t headFollow;
};
void fillFullProjectHeader(FullProjectHeader &header);
bool full805ValidateFile08(File);bool loadFullFile08(File,bool);bool recoveryStage08(File);void recoveryInstall08();bool recoveryCommand08(char*);void recoveryService08();

void fillFullProjectHeader(FullProjectHeader &header) {
  memset(&header, 0, sizeof(header));
  static_assert(sizeof(FullProjectHeader)==644,"converter header layout");
  header.magic = FULL_PROJECT_MAGIC;
  header.version = FULL_PROJECT_VERSION;
  header.bpm = bpm;
  header.selectedTrack = selectedTrack;
  memcpy(header.activePattern, activePattern, sizeof(activePattern));
  memcpy(header.defaultNote, defaultMidiNote, sizeof(defaultMidiNote));
  memcpy(header.defaultVelocity, defaultVel, sizeof(defaultVel));
  memcpy(header.defaultGate, defaultGateTicks, sizeof(defaultGateTicks));
  memcpy(header.defaultMicro, defaultMicroTick, sizeof(defaultMicroTick));
  memcpy(header.midiChannel, trackMidiChannel, sizeof(trackMidiChannel));
  memcpy(header.pitchBend, trackPitchBend, sizeof(trackPitchBend));
  memcpy(header.roll, trackRoll, sizeof(trackRoll));
  memcpy(header.autoSelect, trackAutoSelectMode, sizeof(trackAutoSelectMode));
  memcpy(header.multiEdit, trackMultiEditMode, sizeof(trackMultiEditMode));
  memcpy(header.notePreview, trackNotePreviewMode, sizeof(trackNotePreviewMode));
  for (uint8_t track = 0; track < TRACK_COUNT; track++) {
    header.polyphony[track] = (uint8_t)trackPlayMode[linkOwner12(track,LINK_POLY12)];
  }
  memcpy(header.noteView, trackNoteViewMode, sizeof(trackNoteViewMode));
  memset(header.colours, -1, sizeof(header.colours)); // Reserve format SD historique.
  for (uint8_t track = 0; track < TRACK_COUNT; track++) {
    header.loopStart[track] = loopStartStep;
    header.loopEnd[track] = loopEndStep;
  }
  header.patternChangeNext = patternChangeNext ? 1 : 0;
  header.globalViewMode = (uint8_t)globalNoteViewMode;
  header.headFollow = headFollow ? 1 : 0;
}

bool copyFileIntoProject(File &project, const char* sourcePath) {
  File source = SD.open(sourcePath, FILE_READ);
  uint32_t size = source ? source.size() : 0;
  // Internal patterns are bounded records; a corrupt directory size must not
  // turn a project save into an unbounded copy.
  if(source&&(source.isDirectory()||size>100000)){source.close();return false;}
  if (project.write((const uint8_t*)&size, sizeof(size)) != sizeof(size)) {
    if (source) source.close();
    return false;
  }
  uint8_t buffer[4096];
  while (source && size > 0) {
    size_t wanted = min((uint32_t)sizeof(buffer), size);
    int readCount = source.read(buffer, wanted);
    if (readCount <= 0 || project.write(buffer, readCount) != (size_t)readCount) {
      source.close();
      return false;
    }
    size -= (uint32_t)readCount;
    serviceNoteOffs();
  }
  if (source) source.close();
  return true;
}

struct SaveProbe07{uint32_t started,elapsed;uint16_t step;uint8_t phase;bool active,ok;}saveProbe07={};
FLASHMEM void savePhase07(uint8_t phase,uint16_t step=0){saveProbe07.phase=phase;saveProbe07.step=step;saveProbe07.elapsed=millis()-saveProbe07.started;if(Serial&&Serial.availableForWrite()>80)Serial.printf("SAVE07,PHASE=%u,STEP=%u,MS=%lu\n",phase,step,(unsigned long)saveProbe07.elapsed);}
bool recoveryWritePattern08(File&,const PatternImage805&);
FLASHMEM bool saveFullProject(char* path, size_t pathSize) {
  saveProbe07={millis(),0,0,0,true,false};savePhase07(1);
  if(!saveCheck45(true)){savePhase07(9);saveProbe07.active=false;return false;}
  // Stream the explicit Full save directly from live/packed patterns; no intermediate SD rewrites.
  bool resumePlaying = playing;
  (pidiStateDirty931=true, playing = false);
  internalClockRunning = false;
  pendingClockTicks = 0;
  allNotesOff();
  bool ok = true;
  savePhase07(2);

  SD.mkdir("/URBANKAOZ/PRJ");
  if (ok && full805ForcedPath[0]) { snprintf(path, pathSize, "%s", full805ForcedPath); }
  else if (ok && saveWork905[0]){copyField(path,pathSize,saveWork905);}
  else if (ok && fileBrowserMode!=FILE_BROWSER_SAVE && fileBrowserName[0] != '\0' && strcasecmp(fileBrowserName, "default") == 0) {
    snprintf(path, pathSize, "/URBANKAOZ/PRJ/default.UKP");
    SD.remove(path);
  } else if (ok && !makeUniqueSavePath("PRJ", "PROJECT", "UKP", path, pathSize)) {
    ok = false;
  }
  File project;
  savePhase07(3);if (ok) project = SD.open(path, FILE_WRITE_BEGIN);
  if(project&&!project.truncate(0))ok=false;
  if (ok && !project) ok = false;
  FullProjectHeader header;
  fillFullProjectHeader(header);
  if (ok) ok = project.write((const uint8_t*)&header, sizeof(header)) == sizeof(header);
  for (uint8_t track = 0; track < TRACK_COUNT && ok; track++) {
    savePhase07(4,track);for (uint8_t pattern = 0; pattern < PATTERN_COUNT && ok; pattern++) {
      char internalPath[40];
      internalPatternPath(track, pattern, internalPath);
      if(pattern==activePattern[track]){pattern805CaptureLive(track,patternWork906);ok=recoveryWritePattern08(project,patternWork906);}
      else if(patterns906[track][pattern]){ok=unpackPattern906(patterns906[track][pattern],patternWork906)&&recoveryWritePattern08(project,patternWork906);}
      else if (internalPatternSessionFresh && !patternBank[track][pattern].initialized) {
        uint32_t emptySize = 0;
        ok = project.write((const uint8_t*)&emptySize, sizeof(emptySize)) == sizeof(emptySize);
      } else {
        ok = copyFileIntoProject(project, internalPath);
      }
    }
  }
  if (ok) ok = project.write((const uint8_t*)grooveOffset, sizeof(grooveOffset)) == sizeof(grooveOffset)
    && project.write((const uint8_t*)grooveShape16,sizeof(grooveShape16))==sizeof(grooveShape16)
    && project.write((const uint8_t*)patternGroove, sizeof(patternGroove)) == sizeof(patternGroove)
    && project.write((const uint8_t*)grooveLists, sizeof(grooveLists)) == sizeof(grooveLists);
  savePhase07(5);if (ok) ok = full805Append(project);
  savePhase07(6);if (project) project.close();
  savePhase07(7);if(ok)ok=full805Validate(path); // Never report success for a file our loader rejects.
  if(ok&&!autosaveBusy805)autosave807Baseline();
  (pidiStateDirty931=true, playing = resumePlaying);
  internalClockRunning = playing;
  saveProbe07.ok=ok;saveProbe07.active=false;savePhase07(ok?8:9);return ok;
}

static bool fullPreferRam44=false;
FLASHMEM bool loadFullProject(const char*path){
 if(!beginFullBusy910(true)){DISPLAY_SERIAL.println("MSG|LOAD: PICRAN NOT READY");return false;}
 File f=SD.open(path,FILE_READ);fullPreferRam44=true;bool ok=loadFullFile08(f,true);fullPreferRam44=false;
 if(ok){rememberFileName816(path);copyField(projectName12,sizeof(projectName12),lastFileName816);}
 endFullBusy910();return ok;
}

bool restoreProjectFileRecord(File &project, const char* targetPath) {
  uint32_t size = 0;
  if (project.read((uint8_t*)&size, sizeof(size)) != sizeof(size) || size > 100000UL) return false;
  SD.remove(targetPath);
  if (size == 0) return true;
  File target = SD.open(targetPath, FILE_WRITE);
  if (!target) return false;
  uint8_t buffer[4096];
  while (size > 0) {
    size_t wanted = min((uint32_t)sizeof(buffer), size);
    int readCount = project.read(buffer, wanted);
    if (readCount <= 0 || target.write(buffer, readCount) != (size_t)readCount) {
      target.close();
      return false;
    }
    size -= (uint32_t)readCount;
  }
  target.close();
  return true;
}

FLASHMEM bool loadFullFile08(File project,bool ram) {
  if(!full805ValidateFile08(project))return false;
  if(ram&&!recoveryStage08(project)){if(!fullPreferRam44)return false;ram=false;Serial.println("LOAD44,RAM_CAPACITY,SD_FALLBACK");}
  project.seek(0);
  memset(linkMask12,0,sizeof(linkMask12));
  mod807ClearHistory();
  resetPatterns906();memset(patternDirty805, 0, sizeof(patternDirty805));
  if (!project) return false;
  FullProjectHeader header;
  bool ok = project.read((uint8_t*)&header, sizeof(header)) == sizeof(header) &&
            header.magic == FULL_PROJECT_MAGIC && (header.version == FULL_PROJECT_VERSION || header.version == 33 || header.version == 34) &&
            header.selectedTrack < TRACK_COUNT && header.globalViewMode <= NOTE_VIEW_CHROMATIC &&
            header.headFollow <= 1;
  for (uint8_t track = 0; track < TRACK_COUNT && ok; track++) {
    if (header.activePattern[track] >= PATTERN_COUNT ||
        header.polyphony[track] > TRACK_PLAY_MULTI ||
        header.noteView[track] < -1 || header.noteView[track] > NOTE_VIEW_CHROMATIC) ok = false;
  }
  for (uint8_t track = 0; track < TRACK_COUNT && ok; track++) {
    for (uint8_t pattern = 0; pattern < PATTERN_COUNT && ok; pattern++) {
      char internalPath[40];
      internalPatternPath(track, pattern, internalPath);
      if(ram){uint32_t n=0;ok=project.read((uint8_t*)&n,4)==4&&project.seek(project.position()+n);}
      else ok = restoreProjectFileRecord(project, internalPath);
      patternBank[track][pattern].initialized = false;
    }
  }
  if (ok && header.version >= 2) {
    ok = project.read((uint8_t*)grooveOffset, sizeof(grooveOffset)) == sizeof(grooveOffset)
      && project.read((uint8_t*)grooveShape16,sizeof(grooveShape16))==sizeof(grooveShape16)
      && project.read((uint8_t*)patternGroove, sizeof(patternGroove)) == sizeof(patternGroove);
    if(ok&&header.version<28)grooveLegacy51();else if(ok&&header.version<32)grooveZeroMigration57();
    for (uint8_t t = 0; t < TRACK_COUNT; t++) for (uint8_t p = 0; p < PATTERN_COUNT; p++)
      if (patternGroove[t][p] > (header.version >= 21 ? GROOVE_COUNT+2 : header.version >= 3 ? GROOVE_USE_LIST : GROOVE_COUNT)) ok = false;
  } else if (ok) { memset(grooveOffset, 0, sizeof(grooveOffset)); grooveDefaults16();memset(patternGroove, 0, sizeof(patternGroove)); }
  memset(grooveLists, 0, sizeof(grooveLists));
  if (ok && header.version >= 3) {
    ok = project.read((uint8_t*)grooveLists, sizeof(grooveLists)) == sizeof(grooveLists);
    for (uint8_t t = 0; t < TRACK_COUNT && ok; ++t)
      for (uint8_t p = 0; p < PATTERN_COUNT && ok; ++p)
        for (uint8_t i = 0; i < GROOVE_LIST_SIZE; ++i)
          if (grooveLists[t][p][i].sheet > GROOVE_COUNT || grooveLists[t][p][i].repeats > 99) ok = false;
  }
  // This exact open file was fully validated before any installation.
  if (ok && header.version >= 4) ok = full805Read(project,true);
  else if (ok) { mod805Init();patch808Init(); zlCount = 0; zlNextId = 1; memset(songClips, 0, sizeof(songClips)); }
  project.close();
  if (!ok) return false;
  internalPatternSessionFresh = ram;
  if(ram)recoveryInstall08();

  (pidiStateDirty931=true, playing = false);
  internalClockRunning = false;
  pendingClockTicks = 0;
  allNotesOff();
  resetGrooveBridges();
  bpm = constrain(header.bpm, BPM_MIN, BPM_MAX);
  if(fullMigrateNeeded810){modMigrate810();fullMigrateNeeded810=false;}
  midiReset810();
  memcpy(defaultMidiNote, header.defaultNote, sizeof(defaultMidiNote));
  memcpy(defaultVel, header.defaultVelocity, sizeof(defaultVel));
  memcpy(defaultGateTicks, header.defaultGate, sizeof(defaultGateTicks));
  memcpy(defaultMicroTick, header.defaultMicro, sizeof(defaultMicroTick));
  memcpy(trackMidiChannel, header.midiChannel, sizeof(trackMidiChannel));pidiRoutesDirty931=0xffff;
  memcpy(trackPitchBend, header.pitchBend, sizeof(trackPitchBend));
  memcpy(trackRoll, header.roll, sizeof(trackRoll));
  memcpy(trackAutoSelectMode, header.autoSelect, sizeof(trackAutoSelectMode));
  memcpy(trackMultiEditMode, header.multiEdit, sizeof(trackMultiEditMode));
  memcpy(trackNotePreviewMode, header.notePreview, sizeof(trackNotePreviewMode));
  memcpy(trackNoteViewMode, header.noteView, sizeof(trackNoteViewMode));
  // Couleurs historiques par piste ignorees; format SD conserve.
  for (uint8_t track = 0; track < TRACK_COUNT; track++) {
    (pidiRoutesDirty931|=1u<<(track), trackPlayMode[linkOwner12(track,LINK_POLY12)] = (TrackPlayMode)header.polyphony[track]);
    activePattern[track] = header.activePattern[track];
    if (!loadTrackPattern(track, activePattern[track])) return false;
    trackLoopStartStep[track] = (uint16_t)clampInt(header.loopStart[track], 1, trackStepCount(track));
    trackLoopEndStep[track] = (uint16_t)clampInt(header.loopEnd[track],
                                                trackLoopStartStep[track], trackStepCount(track));
  }
  selectedTrack = header.selectedTrack;
  uint16_t longestSteps = longestTrackStepCount();
  loopStartStep = (uint16_t)clampInt(header.loopStart[selectedTrack], 1, longestSteps);
  loopEndStep = (uint16_t)clampInt(header.loopEnd[selectedTrack], loopStartStep, longestSteps);
  syncLegacyTrackLoopBounds();
  patternChangeNext = header.patternChangeNext != 0;
  globalNoteViewMode = (NoteViewMode)header.globalViewMode;
  headFollow = header.headFollow != 0;
  compactRowOffset = 0;
  ensureSelectedTrackVisible();
  beatDraftBars = sequenceBars;
  displayedPattern = activePattern[selectedTrack] + 1;
  zoomLevel = 0;
  viewTickOffset = 0;
  selectedStep = -1;
  updateInternalClockPeriod();
  setTransportPositionToMeasureStart();
  invalidateStepTriggers();
  neoDisplayDirty = true;
  lastNeoRefreshMs = millis() - NEO_MIN_REFRESH_MS;
  displayValueDirty = true;
  displayBpmDirty = true;
  autosave807Baseline();
  pidiReload827();patch808Play();
  return true;
}

const char* baseNameOnly(const char* path) {
  const char* slash = strrchr(path, '/');
  return slash ? slash + 1 : path;
}

void copyField(char* destination, size_t destinationSize, const char* source) {
  if (!destination || destinationSize == 0) return;
  if (!source) source = "";
  strncpy(destination, source, destinationSize - 1);
  destination[destinationSize - 1] = '\0';
}

uint16_t countBrowserEntries(bool directoriesOnly) {
  File directory = SD.open(fileBrowserPath);
  if (!directory || !directory.isDirectory()) return 0;
  uint16_t count = 0;
  while (true) {
    File entry = directory.openNextFile();
    if (!entry) break;
    if (!directoriesOnly || entry.isDirectory()) count++;
    entry.close();
  }
  directory.close();
  return count;
}

bool selectBrowserEntry(uint16_t wanted, bool directoriesOnly) {
  File directory = SD.open(fileBrowserPath);
  if (!directory || !directory.isDirectory()) return false;
  uint16_t found = 0;
  bool ok = false;
  while (true) {
    File entry = directory.openNextFile();
    if (!entry) break;
    if (!directoriesOnly || entry.isDirectory()) {
      if (found == wanted) {
        copyField(fileBrowserEntry, sizeof(fileBrowserEntry), baseNameOnly(entry.name()));
        fileBrowserEntryIsDirectory = entry.isDirectory();
        ok = true;
        entry.close();
        break;
      }
      found++;
    }
    entry.close();
  }
  directory.close();
  if (!ok) {
    copyField(fileBrowserEntry, sizeof(fileBrowserEntry), "--");
    fileBrowserEntryIsDirectory = false;
  }
  return ok;
}

uint16_t countBrowserFiles() {
  File directory = SD.open(fileBrowserPath);
  if (!directory || !directory.isDirectory()) return 0;
  uint16_t count = 0;
  while (true) {
    File entry = directory.openNextFile();
    if (!entry) break;
    if (!entry.isDirectory() && itemFileMatches809(baseNameOnly(entry.name()))) count++;
    entry.close();
  }
  directory.close();
  return count;
}

bool selectBrowserFile(uint16_t wanted) {
  // One directory pass, in native SD order; no timestamp reads or sorting.
  copyField(fileBrowserEntry, sizeof(fileBrowserEntry), "--");
  fileBrowserEntryIsDirectory = false;
  File directory = SD.open(fileBrowserPath);
  if (!directory || !directory.isDirectory()) { directory.close(); return false; }
  uint16_t index = 0;
  bool found = false;
  while (true) {
    File entry = directory.openNextFile();
    if (!entry) break;
    if (!entry.isDirectory() && itemFileMatches809(baseNameOnly(entry.name()))) {
      if (index == wanted) {
        copyField(fileBrowserEntry, sizeof(fileBrowserEntry), baseNameOnly(entry.name()));
        found = true;
      }
      ++index;
    }
    entry.close();
    if (found) break;
  }
  directory.close();
  return found;
}

void cycleBrowserFile(int direction) {
  uint16_t count = countBrowserFiles();
  if (count == 0) {
    fileBrowserEntryIndex = 0;
    selectBrowserFile(0);
    return;
  }
  int next = (int)fileBrowserEntryIndex + direction;
  while (next < 0) next += count;
  while (next >= count) next -= count;
  fileBrowserEntryIndex = (uint16_t)next;
  selectBrowserFile(fileBrowserEntryIndex);
}

bool browserAtTypeRoot() {
  return strcmp(fileBrowserPath, fileBrowserRootPath) == 0;
}

void refreshBrowserButtonDirectories() {
  browserButtonDirectoryCount = 0;
  browserHasMoreDirectoryPages = false;
  for (uint8_t i = 0; i < BROWSER_BUTTON_DIRECTORY_MAX; i++) {
    browserButtonDirectory[i][0] = '\0';
  }

  File directory = SD.open(fileBrowserPath);
  if (!directory || !directory.isDirectory()) return;
  uint16_t firstWanted = fileBrowserDirectoryPage;
  uint8_t capacity = 1;
  uint16_t found = 0;
  while (true) {
    File entry = directory.openNextFile();
    if (!entry) break;
    if (entry.isDirectory()) {
      if (found >= firstWanted && browserButtonDirectoryCount < capacity) {
        copyField(browserButtonDirectory[browserButtonDirectoryCount],
                  sizeof(browserButtonDirectory[0]), baseNameOnly(entry.name()));
        browserButtonDirectoryCount++;
      } else if (found >= firstWanted + capacity) {
        browserHasMoreDirectoryPages = true;
      }
      found++;
    }
    entry.close();
  }
  directory.close();
  // One directory per page; J1 left/right navigates pages.
}

void browserEnterButtonDirectory(uint8_t directoryButtonIndex) {
  if (directoryButtonIndex >= browserButtonDirectoryCount) return;
  const char* name = browserButtonDirectory[directoryButtonIndex];
  if (strlen(fileBrowserPath) + strlen(name) + 2 >= sizeof(fileBrowserPath)) return;
  strcat(fileBrowserPath, "/");
  strcat(fileBrowserPath, name);
  fileBrowserEntryIndex = 0;
  fileBrowserDirectoryPage = 0;
  if (fileBrowserMode == FILE_BROWSER_LOAD) selectBrowserFile(0);
  refreshBrowserButtonDirectories();
  renderFileBrowser();
}

void cycleBrowserEntry(int direction, bool directoriesOnly) {
  uint16_t count = countBrowserEntries(directoriesOnly);
  if (count == 0) {
    selectBrowserEntry(0, directoriesOnly);
    return;
  }
  int next = (int)fileBrowserEntryIndex + direction;
  while (next < 0) next += count;
  while (next >= count) next -= count;
  fileBrowserEntryIndex = (uint16_t)next;
  selectBrowserEntry(fileBrowserEntryIndex, directoriesOnly);
}

void renderFileBrowser() {if(!storageBlink931)storageDirty931=true;
  if(saveCollision905){renderSaveCollision905();return;}
  char lines[4][21];
  for (uint8_t row = 0; row < 4; row++) {
    memset(lines[row], ' ', 20);
    lines[row][20] = '\0';
  }
  refreshBrowserButtonDirectories();
  const char* labels[8] = {nullptr, nullptr, nullptr, nullptr,
                           nullptr, nullptr, nullptr, nullptr};
  uint8_t offset = 0;
  labels[offset] = fileBrowserMode == FILE_BROWSER_SAVE ? "SAVE" : "LOAD";
  if (!browserAtTypeRoot()) labels[offset + 1] = "BACK";
  labels[offset + 2] = fileBrowserMode == FILE_BROWSER_SAVE ? "NEWD" : "PREV";
  for (uint8_t i = 0; i < browserButtonDirectoryCount; i++) {
    labels[offset + 3 + i] = browserButtonDirectory[i];
  }
  makeEightButtonLines(lines[2], lines[3], labels,
                       false, false);
  if (fileBrowserMode == FILE_BROWSER_SAVE) {
    char shown[11];uint8_t start=storageNameCursor>9?storageNameCursor-9:0;
    memset(shown,' ',10);shown[10]=0;
    for(uint8_t i=0;i<10&&start+i<strlen(fileBrowserName);++i)shown[i]=fileBrowserName[start+i];
    if((millis()/400)%2==0)shown[storageNameCursor-start]='_';
    snprintf(lines[0], sizeof(lines[0]), "N:%s.%-4.4s", shown, fileBrowserExtension);
    snprintf(lines[1], sizeof(lines[1]), "EX:%-17.17s", fileBrowserEntry);
  } else {
    const char* displayEntry = fileBrowserEntry;
    if (fileBrowserEntryIsDirectory) {
      if (strcmp(fileBrowserEntry, "NPX") == 0 || strcmp(fileBrowserEntry, "NPL") == 0) displayEntry = "NOTES + PX";
      else if (strcmp(fileBrowserEntry, "SEQ") == 0) displayEntry = "SEQUENCES";
      else if (strcmp(fileBrowserEntry, "PAT") == 0) displayEntry = "PATTERNS";
      else if (strcmp(fileBrowserEntry, "XEQ") == 0) displayEntry = "MULTI-SEQUENCES";
      else if (strcmp(fileBrowserEntry, "SNG") == 0) displayEntry = "SONGS";
    }
    snprintf(lines[0], sizeof(lines[0]), "%-20.20s", displayEntry);
  }
  if (fileBrowserMode != FILE_BROWSER_SAVE) {
    size_t pathLength = strlen(fileBrowserPath);
    const char* visiblePath = pathLength > 16 ? fileBrowserPath + pathLength - 16 : fileBrowserPath;
    snprintf(lines[1], sizeof(lines[1]), "DIR:%-16.16s", visiblePath);
  }
  sendMenuScreen(lines[0], lines[1], lines[2], storageDescription18());nameDisplayR8();
}

void browserParentDirectory() {
  if (browserAtTypeRoot()) return;
  char* slash = strrchr(fileBrowserPath, '/');
  if (slash && slash != fileBrowserPath) *slash = '\0';
  if (strlen(fileBrowserPath) < strlen(fileBrowserRootPath)) {
    copyField(fileBrowserPath, sizeof(fileBrowserPath), fileBrowserRootPath);
  }
  fileBrowserEntryIndex = 0;
  fileBrowserDirectoryPage = 0;
  selectBrowserFile(0);
  renderFileBrowser();
}

void browserOpenDirectory() {
  if (!fileBrowserEntryIsDirectory || strcmp(fileBrowserEntry, "--") == 0) return;
  if (strlen(fileBrowserPath) + strlen(fileBrowserEntry) + 2 >= sizeof(fileBrowserPath)) return;
  strcat(fileBrowserPath, "/");
  strcat(fileBrowserPath, fileBrowserEntry);
  fileBrowserEntryIndex = 0;
  selectBrowserFile(0);
  renderFileBrowser();
}

void browserCreateDirectory() {
  if (fileBrowserMode != FILE_BROWSER_SAVE) return;
  char path[64];
  for (uint8_t number = 1; number < 100; number++) {
    snprintf(path, sizeof(path), "%s/FOLDER%02u", fileBrowserPath, number);
    if (SD.exists(path)) continue;
    SD.mkdir(path);
    break;
  }
  fileBrowserDirectoryPage = 0;
  renderFileBrowser();
}

void closeFileBrowser() {storageDirty931=true;
  remixBrowser911=remixLoading911=remixWriteView911=remixConfirm911=false;
  saveCollision905=false;saveOverwrite905=false;
  if(itemKind809||itemChoosing809){itemClose809();return;}
  fileBrowserMode = FILE_BROWSER_NONE;
  patternLoadConfirmActive = false;
  saveMenuActive = false;
  showGameScreen();
}

bool clipboardHasLineSelection(uint8_t track) {
  for (uint16_t step = 0; step < STEP_COUNT; step++) {
    if (stepSelected[track][step] && noteCountAtStep(track, step)) return true;
  }
  return false;
}

FLASHMEM void openSaveBrowser(SaveCandidateType candidate, bool fromClipboard, uint8_t slot) {
  pendingSaveCandidate = candidate;
  pendingSaveClipboard = fromClipboard;
  pendingSaveClipboardSlot = slot;
  fileBrowserMode = FILE_BROWSER_SAVE;
  storageNameCursor=0;
  saveMenuActive = false;

  if (fromClipboard) {
    const uint8_t kind = clipKinds18[slot];
    if (!kind) { fileBrowserMode=FILE_BROWSER_NONE; saveMenuActive=true; saveMenuClipboardMode=true; sendSaveMenu(); return; }
    pendingSaveCandidate = kind == 3 ? SAVE_CANDIDATE_PATTERN : kind == 1 ? SAVE_CANDIDATE_NOTE : SAVE_CANDIDATE_SEQUENCE;
    copyField(fileBrowserExtension, sizeof(fileBrowserExtension), kind == 4 ? "UKT" : kind == 5 ? "PLK" : kind == 3 ? "PAT" : kind == 1 ? "NPX" : "SEQ");
    snprintf(fileBrowserName, sizeof(fileBrowserName), "CLIP%u", slot + 1);
  } else if (candidate == SAVE_CANDIDATE_FULL) {
    copyField(fileBrowserExtension, sizeof(fileBrowserExtension), "UKP");
    copyField(fileBrowserName, sizeof(fileBrowserName), "PROJECT");
  } else if (candidate == SAVE_CANDIDATE_NOTE) {
    copyField(fileBrowserExtension, sizeof(fileBrowserExtension), "NPX");
    snprintf(fileBrowserName, sizeof(fileBrowserName), "NOTE_T%u", selectedTrack + 1);
  } else if (candidate == SAVE_CANDIDATE_PATTERN) {
    copyField(fileBrowserExtension, sizeof(fileBrowserExtension), "PAT");
    copyField(fileBrowserName, sizeof(fileBrowserName), "PATTERN");
  } else if (candidate == SAVE_CANDIDATE_SETTINGS) {
    copyField(fileBrowserExtension, sizeof(fileBrowserExtension), "CFG");
    copyField(fileBrowserName, sizeof(fileBrowserName), "PARAMS");
  } else {
    copyField(fileBrowserExtension, sizeof(fileBrowserExtension), "SEQ");
    snprintf(fileBrowserName, sizeof(fileBrowserName), candidate == SAVE_CANDIDATE_SELECTION ? "SELECT_T%u" : "SEQ_T%u", selectedTrack + 1);
  }

  if (candidate == SAVE_CANDIDATE_FULL) {
    snprintf(fileBrowserPath, sizeof(fileBrowserPath), "/URBANKAOZ/PRJ");
  } else {
    snprintf(fileBrowserPath, sizeof(fileBrowserPath), "/URBANKAOZ/%s", fileBrowserExtension);
  }
  namePropose12(lastFileName816[0]?lastFileName816:candidate==SAVE_CANDIDATE_FULL?projectName12:nullptr);
  // Generic content browsers always start in the folder for their content type.
  SD.mkdir(fileBrowserPath);
  copyField(fileBrowserRootPath, sizeof(fileBrowserRootPath), fileBrowserPath);
  fileBrowserEntryIndex = 0;
  fileBrowserDirectoryPage = 0;
  selectBrowserFile(0);
  renderFileBrowser();
}

bool saveAllSettingsFile(char* path, size_t pathSize);

void executeBrowserSave() {
  if(!prepareSave905())return;
  if(remixBrowser911){remixSave911();return;}
  if(itemKind809){itemSaveFile809();return;}
  if(fileBrowserMode==FILE_BROWSER_SAVE&&!fileBrowserName[0]){DISPLAY_SERIAL.println("MSG|FILE NAME REQUIRED");return;}
  char path[64] = "";
  bool ok = false;
  if (pendingSaveClipboard) {
    ok=contentSaveClip18(pendingSaveClipboardSlot,path,sizeof(path));
  } else if (pendingSaveCandidate == SAVE_CANDIDATE_FULL) {
    // Picran acknowledges only after the first busy frame is on its LCD.
    if(!beginFullBusy910()){saveWork905[0]=0;saveOverwrite905=false;DISPLAY_SERIAL.println("MSG|DISPLAY NOT READY");return;}
    ok = saveFullProject(path, sizeof(path));
  } else if (pendingSaveCandidate == SAVE_CANDIDATE_NOTE) {
    if (selectedStep >= 0) ok = saveTrackNoteNpl(selectedTrack, selectedStep, path, sizeof(path));
  } else if (pendingSaveCandidate == SAVE_CANDIDATE_SEQUENCE || pendingSaveCandidate == SAVE_CANDIDATE_SELECTION) {
    if (pendingSaveCandidate == SAVE_CANDIDATE_SELECTION) {
      ok = saveSelectedSequence(selectedTrack, path, sizeof(path));
    } else ok = saveTrackSequence(selectedTrack, path, sizeof(path));
  } else if (pendingSaveCandidate == SAVE_CANDIDATE_SETTINGS) {
    ok = saveAllSettingsFile(path, sizeof(path));
  } else ok = savePattern(path, sizeof(path));
  ok=finishSave905(ok,path,sizeof(path));
  endFullBusy910();
  bool convertAfterSave = ok && afterSaveAction == AFTER_SAVE_TRACK_MODE_CONVERSION;
  uint8_t conversionTrack = afterSaveTrack;
  TrackPlayMode conversionMode = afterSaveTrackMode;
  uint16_t conversionGate = afterSaveGateTicks;
  afterSaveAction = AFTER_SAVE_NONE;
  closeFileBrowser();
  if (convertAfterSave && conversionTrack < TRACK_COUNT) {
    selectedTrack = conversionTrack;
    pendingTrackPlayMode = conversionMode;
    trackGateLengthDraft = conversionGate;
    conversionReturnsHome = true;
    applyTrackPlayModeConversion();
  }
  sendSaveResult(path, ok);
}

bool selectedBrowserFilePath(char* path, size_t pathSize) {
  if (fileBrowserEntryIsDirectory || strcmp(fileBrowserEntry, "--") == 0) return false;
  snprintf(path, pathSize, "%s/%s", fileBrowserPath, fileBrowserEntry);
  return true;
}

void resetLoadedClipboard(ClipboardSlot &clip) {
  memset(&clip, 0, sizeof(ClipboardSlot));
  clip.valid = false;
  clip.type = CLIP_TYPE_SEQUENCE;
  clip.startStep = 0;
  clip.lengthSteps = STEP_COUNT;
  clip.defaultVel = 100;
  clip.defaultNote = 60;
  clip.defaultGateTicks = TICKS_PER_STEP / 2;
}

FLASHMEM bool loadFileIntoClipboard(const char* path, ClipboardSlot &clip,
                           FineNoteEvent *loadedFine = nullptr,
                           uint8_t *loadedFineCount = nullptr, File*source15=nullptr) {
  File file = source15?*source15:SD.open(path, FILE_READ);
  if (!file) return false;
  resetLoadedClipboard(clip);
  if (loadedFineCount) *loadedFineCount = 0;
  if (loadedFine) memset(loadedFine, 0, sizeof(FineNoteEvent) * FINE_EVENTS_PER_TRACK);
  String header = file.readStringUntil('\n');
  header.trim();
  if (header == "URBANKAOZ_NPX_V1" || header == "URBANKAOZ_NPX_V2" ||
      header == "URBANKAOZ_NPL_V1") {
    clip.type = CLIP_TYPE_NOTES;
    clip.lengthSteps = 1;
  } else if (header != "URBANKAOZ_SEQ_V3") {
    file.close();
    return false;
  }

  while (file.available()) {
    String line = file.readStringUntil('\n');
    line.trim();
    if (line.startsWith("length_steps=")) {
      clip.lengthSteps = (uint16_t)clampInt(line.substring(13).toInt(), 1, STEP_COUNT);
    } else if (line.startsWith("source_track=")) {
      clip.sourceTrack = (uint8_t)clampInt(line.substring(13).toInt() - 1, 0, TRACK_COUNT - 1);
    } else if (line.startsWith("content_type=")) {
      String type = line.substring(13);
      if (type == "NOTES") clip.type = CLIP_TYPE_NOTES;
      else if (type == "SEL") clip.type = CLIP_TYPE_SELECTION;
      else clip.type = CLIP_TYPE_SEQUENCE;
    } else if (line.startsWith("default_note=")) {
      clip.defaultNote = (uint8_t)clampInt(line.substring(13).toInt(), 0, 127);
    } else if (line.startsWith("default_velocity=")) {
      clip.defaultVel = (uint8_t)clampInt(line.substring(17).toInt(), 1, 127);
    } else if (line.startsWith("default_gate=")) {
      clip.defaultGateTicks = (uint16_t)clampInt(line.substring(13).toInt(), GATE_TICK_MIN, GATE_TICK_MAX);
    } else if (line.startsWith("default_micro=")) {
      clip.defaultMicroTick = (int16_t)clampInt(line.substring(14).toInt(), MICRO_TICK_MIN, MICRO_TICK_MAX);
    } else if (line.startsWith("F,") && loadedFine && loadedFineCount &&
               *loadedFineCount < FINE_EVENTS_PER_TRACK) {
      int index, logical, micro, note, velocity, gate, pxMask;
      int start, speed, accel, expo, curve, level;
      if (sscanf(line.c_str(), "F,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
                 &index, &logical, &micro, &note, &velocity, &gate, &pxMask,
                 &start, &speed, &accel, &expo, &curve, &level) == 13 &&
          index >= 0 && index < STEP_COUNT) {
        FineNoteEvent &event = loadedFine[(*loadedFineCount)++];
        event.active = true;
        event.step = (uint16_t)index;
        event.logicalMicro = (int8_t)clampInt(logical, MICRO_TICK_MIN, TICKS_PER_STEP-1);
        event.micro = (int16_t)clampInt(micro, MICRO_TICK_MIN, MICRO_TICK_MAX);
        event.note = (uint8_t)clampInt(note, 0, 127);
        event.vel = (uint8_t)clampInt(velocity, 1, 127);
        event.gate = (uint16_t)clampInt(gate, GATE_TICK_MIN, GATE_TICK_MAX);
        event.pxMask = (uint16_t)pxMask;
        event.roll.start = (uint16_t)clampInt(start, 0, TICKS_PER_STEP);
        event.roll.speed = (uint16_t)clampInt(speed, 1, TICKS_PER_STEP);
        event.roll.accel = (int16_t)clampInt(accel, -127, 127);
        event.roll.expo = (uint8_t)clampInt(expo, 0, 102);
        event.roll.curve = (int16_t)clampInt(curve, -127, 127);
        event.roll.level = (uint8_t)clampInt(level, 0, 127);
        clip.active[index] = true;
        clip.note[index] = event.note;
        clip.vel[index] = event.vel;
        clip.gateTicks[index] = event.gate;
        clip.microTick[index] = event.micro;
      }
    } else if (line.length() > 0 && isDigit(line[0])) {
      int index, active, note, velocity, gate, micro, lockPitch, lockVel, lockGate, lockTime;
      if (sscanf(line.c_str(), "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
                 &index, &active, &note, &velocity, &gate, &micro,
                 &lockPitch, &lockVel, &lockGate, &lockTime) == 10 &&
          index >= 0 && index < STEP_COUNT) {
        clip.active[index] = active != 0;
        clip.note[index] = (uint8_t)clampInt(note, 0, 127);
        clip.vel[index] = (uint8_t)clampInt(velocity, 1, 127);
        clip.gateTicks[index] = (uint16_t)clampInt(gate, GATE_TICK_MIN, GATE_TICK_MAX);
        clip.microTick[index] = (int16_t)clampInt(micro, MICRO_TICK_MIN, MICRO_TICK_MAX);
        clip.lockPitch[index] = lockPitch != 0;
        clip.lockVel[index] = lockVel != 0;
        clip.lockGate[index] = lockGate != 0;
        clip.lockTime[index] = lockTime != 0;
      }
    }
  }
  file.close();
  if (clip.type == CLIP_TYPE_NOTES && clip.lengthSteps == 1) {
    clip.defaultNote = clip.note[0];
    clip.defaultVel = clip.vel[0];
    clip.defaultGateTicks = clip.gateTicks[0];
    clip.defaultMicroTick = clip.microTick[0];
  }
  clip.valid = true;
  return true;
}

FLASHMEM void initialisePatternLoadBuffer() {
  patternLoadBuffer.bpm = bpm;
  patternLoadBuffer.lengthSteps = STEPS_PER_BAR;
  for (uint8_t track = 0; track < TRACK_COUNT; track++) {
    memset(fineNote[track], 0, sizeof(fineNote[track]));
    patternLoadBuffer.defaultNote[track] = defaultMidiNote[track];
    patternLoadBuffer.defaultVelocity[track] = defaultVel[track];
    patternLoadBuffer.defaultGate[track] = defaultGateTicks[track];
    patternLoadBuffer.defaultMicro[track] = defaultMicroTick[track];
    patternLoadBuffer.midiChannel[track] = midiChannelForTrack(track);
    patternLoadBuffer.pitchBend[track] = trackPitchBend[track];
    patternLoadBuffer.roll[track] = trackRoll[track];
    patternLoadBuffer.autoSelect[track] = trackAutoSelectMode[track];
    for (uint16_t step = 0; step < STEP_COUNT; step++) {
      stepActive[track][step] = false;
      stepMidiNote[track][step] = indNote50(track,0);
      stepExtraNoteCount[track][step] = 0;
      stepVel[track][step] = indNote50(track,1);
      stepGateTicks[track][step] = indNote50(track,2);
      stepLogicalMicro[track][step] = 0;
      stepMicroTick[track][step] = indNote50(track,3);
      stepLockPitch[track][step] = false;
      stepLockVel[track][step] = false;
      stepLockGate[track][step] = false;
      stepLockTime[track][step] = false;
      rollSet906(track,step,emptyRoll906,0);
      stepSelected[track][step] = false;
    }
  }
}

FLASHMEM bool readPatternFile(const char* path,File*source15=nullptr) {
  File file = source15?*source15:SD.open(path, FILE_READ);
  if (!file) return false;
  String header = file.readStringUntil('\n');
  header.trim();
  bool version1 = header == "URBANKAOZ_PAT_V1";
  bool version2 = header == "URBANKAOZ_PAT_V2";
  bool version3 = header == "URBANKAOZ_PAT_V3";
  bool version4 = header == "URBANKAOZ_PAT_V4";
  bool version5 = header == "URBANKAOZ_PAT_V6";
  if(!version5){file.close();DISPLAY_SERIAL.println("MSG|USE PC CONVERTER 14");return false;}
  initialisePatternLoadBuffer();
  if (!version1 && !version2 && !version3 && !version4 && !version5) {
    file.close();
    return false;
  }

  uint16_t loadedSteps = 0;
  while (file.available()) {
    String line = file.readStringUntil('\n');
    line.trim();
    if (line.startsWith("length_steps=")) {
      patternLoadBuffer.lengthSteps = (uint16_t)clampInt(line.substring(13).toInt(), 1, STEP_COUNT);
      continue;
    }
    if (line.startsWith("bpm=")) {
      patternLoadBuffer.bpm = constrain(line.substring(4).toFloat(), BPM_MIN, BPM_MAX);
      continue;
    }

    int track, step, active, note, velocity, gate, micro;
    int lockPitch, lockVel, lockGate, lockTime;
    if ((version2 || version3 || version4 || version5) && line.startsWith("D,")) {
      int channel, bend;
      if (sscanf(line.c_str(), "D,%d,%d,%d,%d,%d,%d,%d",
                 &track, &note, &velocity, &gate, &micro, &channel, &bend) == 7) {
        track--;
        if (track >= 0 && track < TRACK_COUNT) {
          patternLoadBuffer.defaultNote[track] = (uint8_t)clampInt(note, 0, 127);
          patternLoadBuffer.defaultVelocity[track] = (uint8_t)clampInt(velocity, 1, 127);
          patternLoadBuffer.defaultGate[track] = (uint16_t)clampInt(gate, GATE_TICK_MIN, GATE_TICK_MAX);
          patternLoadBuffer.defaultMicro[track] = (int16_t)clampInt(micro, MICRO_TICK_MIN, MICRO_TICK_MAX);
          patternLoadBuffer.midiChannel[track] = (uint8_t)clampInt(channel - 1, 0, 15);
          patternLoadBuffer.pitchBend[track] = (int16_t)clampInt(bend, -8192, 8191);
        }
      }
      continue;
    }

    if ((version3 || version4 || version5) && line.startsWith("R,")) {
      int start, speed, accel, expo, curve, level;
      if (sscanf(line.c_str(), "R,%d,%d,%d,%d,%d,%d,%d",
                 &track, &start, &speed, &accel, &expo, &curve, &level) == 7) {
        track--;
        if (track >= 0 && track < TRACK_COUNT) {
          patternLoadBuffer.roll[track].start = (uint16_t)clampInt(start, 0, TICKS_PER_STEP);
          patternLoadBuffer.roll[track].speed = (uint16_t)clampInt(speed, 1, TICKS_PER_STEP);
          patternLoadBuffer.roll[track].accel = (int16_t)clampInt(accel, -127, 127);
          patternLoadBuffer.roll[track].expo = (uint8_t)clampInt(expo, 0, 102);
          patternLoadBuffer.roll[track].curve = (int16_t)clampInt(curve, -127, 127);
          patternLoadBuffer.roll[track].level = (uint8_t)clampInt(level, 0, 127);
        }
      }
      continue;
    }

    if ((version3 || version4 || version5) && line.startsWith("A,")) {
      int autoMode;
      if (sscanf(line.c_str(), "A,%d,%d", &track, &autoMode) == 2) {
        track--;
        if (track >= 0 && track < TRACK_COUNT) {
          patternLoadBuffer.autoSelect[track] = (int8_t)clampInt(autoMode, -1, 1);
        }
      }
      continue;
    }

    if ((version4 || version5) && line.startsWith("Q,")) {
      int mask, start, speed, accel, expo, curve, level;
      if (sscanf(line.c_str(), "Q,%d,%d,%d,%d,%d,%d,%d,%d,%d",
                 &track, &step, &mask, &start, &speed, &accel, &expo, &curve, &level) == 9) {
        track--;
        if (track >= 0 && track < TRACK_COUNT && step >= 0 && step < STEP_COUNT) {
          TrackRollSettings parsed906={};
          parsed906.start = (uint16_t)clampInt(start, 0, TICKS_PER_STEP);
          parsed906.speed = (uint16_t)clampInt(speed, 1, TICKS_PER_STEP);
          parsed906.accel = (int16_t)clampInt(accel, -127, 127);
          parsed906.expo = (uint8_t)clampInt(expo, 0, 102);
          parsed906.curve = (int16_t)clampInt(curve, -127, 127);
          parsed906.level = (uint8_t)clampInt(level, 0, 127);
          rollSet906(track,step,parsed906,(uint8_t)(mask & 0x3F));
        }
      }
      continue;
    }

    if(version5&&line.startsWith("E,")){int v,n,vel,g,tm;if(sscanf(line.c_str(),"E,%d,%d,%d,%d,%d,%d,%d",&track,&step,&v,&n,&vel,&g,&tm)==7&&track>=1&&track<=16&&step>=0&&step<1024&&v>=0&&v<3&&n>=0&&n<=127&&vel>=1&&vel<=127&&g>=1&&g<=65535&&tm>=-96&&tm<=96){--track;stepExtraMidiNote[track][step][v]=n;stepExtra14[track][step][v]={(uint16_t)g,(int16_t)tm,(uint8_t)vel};stepExtraNoteCount[track][step]=max((int)stepExtraNoteCount[track][step],v+1);}continue;}
    if (version5 && line.startsWith("L,")) {
      int logical;
      if (sscanf(line.c_str(), "L,%d,%d,%d", &track, &step, &logical) == 3) {
        track--;
        if (track >= 0 && track < TRACK_COUNT && step >= 0 && step < STEP_COUNT) {
          stepLogicalMicro[track][step] = (int8_t)clampInt(logical, MICRO_TICK_MIN, TICKS_PER_STEP-1);
        }
      }
      continue;
    }

    if (version5 && line.startsWith("F,")) {
      int logical, pxMask, start, speed, accel, expo, curve, level;
      if (sscanf(line.c_str(), "F,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
                 &track, &step, &logical, &micro, &note, &velocity, &gate, &pxMask,
                 &start, &speed, &accel, &expo, &curve, &level) == 14) {
        track--;
        if (track >= 0 && track < TRACK_COUNT && step >= 0 && step < STEP_COUNT) {
          for (uint8_t fine = 0; fine < FINE_EVENTS_PER_TRACK; fine++) {
            if (fineNote[track][fine].active) continue;
            FineNoteEvent &event = fineNote[track][fine];
            event.active = true;
            event.step = (uint16_t)step;
            event.logicalMicro = (int8_t)clampInt(logical, MICRO_TICK_MIN, TICKS_PER_STEP-1);
            event.micro = (int16_t)clampInt(micro, MICRO_TICK_MIN, MICRO_TICK_MAX);
            event.note = (uint8_t)clampInt(note, 0, 127);
            event.vel = (uint8_t)clampInt(velocity, 1, 127);
            event.gate = (uint16_t)clampInt(gate, GATE_TICK_MIN, GATE_TICK_MAX);
            event.pxMask = (uint16_t)pxMask;
            event.roll.start = (uint16_t)clampInt(start, 0, TICKS_PER_STEP);
            event.roll.speed = (uint16_t)clampInt(speed, 1, TICKS_PER_STEP);
            event.roll.accel = (int16_t)clampInt(accel, -127, 127);
            event.roll.expo = (uint8_t)clampInt(expo, 0, 102);
            event.roll.curve = (int16_t)clampInt(curve, -127, 127);
            event.roll.level = (uint8_t)clampInt(level, 0, 127);
            loadedSteps++;
            break;
          }
        }
      }
      continue;
    }

    int parsed = 0;
    if ((version2 || version3 || version4 || version5) && line.startsWith("S,")) {
      parsed = sscanf(line.c_str(), "S,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
                      &track, &step, &active, &note, &velocity, &gate, &micro,
                      &lockPitch, &lockVel, &lockGate, &lockTime);
    } else if (version1 && line.length() > 0 && isDigit(line[0])) {
      parsed = sscanf(line.c_str(), "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
                      &track, &step, &active, &note, &velocity, &gate, &micro,
                      &lockPitch, &lockVel, &lockGate, &lockTime);
    }
    if (parsed != 11) continue;
    track--;
    if (track < 0 || track >= TRACK_COUNT || step < 0 || step >= STEP_COUNT) continue;
    stepActive[track][step] = active != 0;
    stepMidiNote[track][step] = (uint8_t)clampInt(note, 0, 127);
    stepVel[track][step] = (uint8_t)clampInt(velocity, 1, 127);
    stepGateTicks[track][step] = (uint16_t)clampInt(gate, GATE_TICK_MIN, GATE_TICK_MAX);
    stepLogicalMicro[track][step] = 0;
    stepMicroTick[track][step] = (int16_t)clampInt(micro, MICRO_TICK_MIN, MICRO_TICK_MAX);
    stepLockPitch[track][step] = lockPitch != 0;
    stepLockVel[track][step] = lockVel != 0;
    stepLockGate[track][step] = lockGate != 0;
    stepLockTime[track][step] = lockTime != 0;
    loadedSteps++;
  }
  file.close();
  return loadedSteps > 0;
}

bool applyPatternFile(const char* path,File*source15=nullptr) {
  if(!source15)return contentLoadPattern18(path);
  stopClipboardPreview(true, true);
  (pidiStateDirty931=true, playing = false);
  internalClockRunning = false;
  noInterrupts(); pendingClockTicks = 0; interrupts();
  allNotesOff();
  if (!readPatternFile(path,source15)) return false;

  for (uint8_t track = 0; track < TRACK_COUNT; track++) {
    defaultMidiNote[track] = patternLoadBuffer.defaultNote[track];
    defaultVel[track] = patternLoadBuffer.defaultVelocity[track];
    defaultGateTicks[track] = patternLoadBuffer.defaultGate[track];
    defaultMicroTick[track] = patternLoadBuffer.defaultMicro[track];
    (pidiRoutesDirty931|=1u<<(track), trackMidiChannel[track] = patternLoadBuffer.midiChannel[track]);
    trackPitchBend[track] = patternLoadBuffer.pitchBend[track];
    trackRoll[track] = patternLoadBuffer.roll[track];
    trackAutoSelectMode[track] = patternLoadBuffer.autoSelect[track];
  }
  bpm = patternLoadBuffer.bpm;
  sequenceBars = (uint16_t)clampInt((patternLoadBuffer.lengthSteps + STEPS_PER_BAR - 1) / STEPS_PER_BAR, 1, MAX_BARS);
  for (uint8_t track = 0; track < TRACK_COUNT; track++) {
    trackBars[track] = sequenceBars;
    trackLoopStartStep[track] = 1;
    trackLoopEndStep[track] = trackStepCount(track);
  }
  beatDraftBars = sequenceBars;
  beatEditActive = false;
  loopStartStep = 1;
  loopEndStep = longestTrackStepCount();
  syncLegacyTrackLoopBounds();
  viewTickOffset = 0;
  zoomLevel = 0;
  applyConfiguredRollToAudio();
  updateInternalClockPeriod();
  cycleCounter = 0;
  setTransportPositionToMeasureStart();
  selectedStep = -1;
  invalidateStepTriggers();
  neoDisplayDirty = true;
  lastNeoRefreshMs = millis() - NEO_MIN_REFRESH_MS;
  displayValueDirty = true;
  displayBpmDirty = true;
  return true;
}

FLASHMEM void openPatternLoadConfirmation() {
  if(remixLoadChoice911())return;
  if (!selectedBrowserFilePath(pendingPatternPath, sizeof(pendingPatternPath))) return;
  String name = fileBrowserEntry;
  name.toUpperCase();
  const char* expected = activeStorageType == STORAGE_FULL ? ".UKP" : ".PAT";
  if (!name.endsWith(expected)) {
    DISPLAY_SERIAL.print(F("MSG|SELECT "));
    DISPLAY_SERIAL.println(expected);
    renderFileBrowser();
    return;
  }
  patternLoadConfirmActive = true;
  const char* labels[8] = {nullptr, "LOAD", nullptr, nullptr,
                           nullptr, nullptr, nullptr, nullptr};
  char top[21], bottom[21];
  makeEightButtonLines(top, bottom, labels);
  sendMenuScreen(activeStorageType == STORAGE_FULL ? "LOAD FULL PROJECT?" : "LOAD THIS PATTERN?",
                 "Current data will be replaced", top, bottom);
}

void answerPatternLoadConfirmation(bool accept) {
  if (!patternLoadConfirmActive) return;
  patternLoadConfirmActive = false;
  if(newProjectConfirm807){newProjectConfirm807=false;if(accept)newProject807();else showGameScreen();return;}
  if (!accept) {
    renderFileBrowser();
    return;
  }
  bool fullLoad51=activeStorageType==STORAGE_FULL;
  bool ok = activeStorageType == STORAGE_FULL
              ? loadFullProject(pendingPatternPath)
              : applyPatternFile(pendingPatternPath);
  if(ok)rememberFileName816(pendingPatternPath);
  if(ok){itemKind809=0;itemChoosing809=false;grooveEditMode=patternMode=false;grooveStepMask=0;showGameScreen();DISPLAY_SERIAL.println("HOME55");}
  else closeFileBrowser();
  if(!ok)DISPLAY_SERIAL.println(F("MSG|LOAD ERROR"));
}

FLASHMEM bool loadSelectedFileIntoClipboard(uint8_t slot){char path[96];if(!selectedBrowserFilePath(path,sizeof(path)))return false;bool ok=contentLoadClip18(path,slot);if(ok)rememberFileName816(path);return ok;}

bool previewSelectedLoadFile(uint8_t targetTrack){(void)targetTrack;char path[96];return selectedBrowserFilePath(path,sizeof(path))&&contentPreview18(path);}

FLASHMEM void sdDisplay42(){sendMenuScreen("SD",sdAvailable?"Montee":"Retiree",sdAvailable?"Retirer":"Inserer","");}
FLASHMEM void sdOpen42(bool save){sdMenuSave42=save;saveMenuActive=loadTypeMenuActive=false;sdMenu42=true;sdDisplay42();}
FLASHMEM void sdAction42(){
 if(playing||anyLiveClipboardPreview()||clipboardPreviewActive||sdBusy905()){DISPLAY_SERIAL.println("MSG|STOP / SD BUSY");return;}
 if(sdAvailable){storageWriteAvailable=sdAvailable=false;SD.sdfs.end();DISPLAY_SERIAL.println("MSG|SD RETIRABLE");}
 else {sdAvailable=SD.begin(BUILTIN_SDCARD);storageWriteAvailable=sdAvailable;if(sdAvailable){patchCatalogScan10();patchResolve10(patchDb808);patchMetadataStart37();}DISPLAY_SERIAL.println(sdAvailable?"MSG|SD MONTEE":"MSG|SD ABSENTE");}
 sdNeoNoticeActive=!sdAvailable;sdDisplay42();
}
FLASHMEM void openLoadBrowser() {
  remixBrowser911=remixLoading911=remixWriteView911=remixConfirm911=false; itemKind809=0;itemChoosing809=false;fileBrowserExtension[0]=0;
  closeClipboardMenu();
  saveMenuActive = false;
  fileBrowserMode = FILE_BROWSER_NONE;
  loadTypeMenuActive = true;
  const char* labels[8] = {"FULL", "NOTE", "PATT", "SEQ",
                           "SONG", "NewP", nullptr, "SD"};
  char top[21], bottom[21];
  makeEightButtonLines(top, bottom, labels);

  sendMenuScreen("LOAD", "Choose the kind of content to load", top, bottom);
}

FLASHMEM void sendLoadTypeMenu() {
  const char* labels[8] = {"FULL", "NOTE", "PATT", "SEQ",
                           "SONG", "NewP", nullptr, "SD"};
  char top[21], bottom[21];
  makeEightButtonLines(top, bottom, labels);

  sendMenuScreen("LOAD", "Choose the kind of content to load", top, bottom);
}

bool storageTypeImplemented(StorageContentType type) {
  return type == STORAGE_FULL || type == STORAGE_PATTERN ||
         type == STORAGE_SEQUENCE || type == STORAGE_NOTE;
}

FLASHMEM void openStorageLoadBrowser(StorageContentType type) {
  remixBrowser911=remixLoading911=remixWriteView911=remixConfirm911=false; itemKind809=0;itemChoosing809=false;fileBrowserExtension[0]=0;
  if(!sdAvailable){DISPLAY_SERIAL.println("MSG|SD MISSING");return;}
  if (!storageTypeImplemented(type)) {
    DISPLAY_SERIAL.println(F("MSG|INDISPONIBLE"));
    sendLoadTypeMenu();
    return;
  }
  activeStorageType = type;
  const char* folder = type == STORAGE_FULL ? "PRJ" :
                       (type == STORAGE_PATTERN ? "PAT" :
                       (type == STORAGE_SEQUENCE ? "SEQ" : "NPX"));
  snprintf(fileBrowserPath, sizeof(fileBrowserPath), "/URBANKAOZ/%s", folder);
  // Generic content browsers always start in the folder for their content type.
  copyField(fileBrowserRootPath, sizeof(fileBrowserRootPath), fileBrowserPath);
  loadTypeMenuActive = false;
  writeMenuActive = false;
  fileBrowserMode = FILE_BROWSER_LOAD;
  fileBrowserEntryIndex = 0;
  fileBrowserDirectoryPage = 0;
  selectBrowserFile(0);
  renderFileBrowser();
}

void closeSaveMenu() {
  if (!saveMenuActive) return;
  saveMenuActive = false;
  saveMenuClipboardMode = false;
  DISPLAY_SERIAL.println(F("UI|GAME"));
}

FLASHMEM void sendSaveMenu() {
  char lines[4][21];
  for (uint8_t row = 0; row < 4; row++) {
    memset(lines[row], ' ', 20);
    lines[row][20] = '\0';
  }
  if (saveMenuClipboardMode) {
    const char* types[]={"Empty","Note","Seq","Patt","Track","Remix","PLX"};
    for(uint8_t i=0;i<4;++i){char label[11];snprintf(label,sizeof(label),"%cC%u %-6.6s",saveMenuSelection==i?'>':' ',i+1,types[min(clipKinds18[i],(uint8_t)6)]);memcpy(lines[i/2]+10*(i%2),label,10);}
    lines[2][0]=lines[3][0]=0;
  } else {
    const char* labels[8] = {"FULL", "CLIP", "PATT", "SEQ",
                             "NOTE", "SONG", "PARA", "SD"};
    makeEightButtonLines(lines[2], lines[3], labels);
    snprintf(lines[0], sizeof(lines[0]), "SAVE ITEMS T%u", selectedTrack + 1);
    const char* choices[8]={"FULL","CLIPBOARD","PATTERN","SEQUENCE","NOTE","SONG","PARA","SD"};
    snprintf(lines[1], sizeof(lines[1]), "> %s - J1", choices[min(saveMenuSelection,(uint8_t)7)]);
  }
  sendMenuScreen(lines[0], lines[1],
                 lines[2], lines[3]);
}

FLASHMEM void openSaveMenu() {
  remixBrowser911=remixLoading911=remixWriteView911=remixConfirm911=false; itemKind809=0;itemChoosing809=false;fileBrowserExtension[0]=0;
  closeClipboardMenu();
  saveMenuPausedPlayback = playing;
  stopClipboardPreview(true, true);
  if (playing) {
    (pidiStateDirty931=true, playing = false);
    internalClockRunning = anyLiveClipboardPreview() || clipboardPreviewActive;
    allNotesOff();
  }
  saveMenuActive = true;
  saveMenuClipboardMode = false;
  if (clipboardHasLineSelection(selectedTrack)) {
    saveMenuCandidates[0] = SAVE_CANDIDATE_SELECTION;
  } else {
    saveMenuCandidates[0] = SAVE_CANDIDATE_NOTE;
  }
  saveMenuCandidates[1] = SAVE_CANDIDATE_SEQUENCE;
  saveMenuCandidates[2] = SAVE_CANDIDATE_PATTERN;
  saveMenuSelection = 0;
  sendSaveMenu();
}

FLASHMEM void openClipboardSaveMenu(uint8_t slot) {
 // Save is entered only through the main Save menu.
 (void)slot;
}

void moveSaveMenu(int direction) {
  int maximum = saveMenuClipboardMode ? 3 : 7;
  saveMenuSelection = (uint8_t)clampInt((int)saveMenuSelection + direction, 0, maximum);
  sendSaveMenu();
}

void confirmSaveMenu() {
  uint8_t index=saveMenuSelection;

  handleScreenButtonPress(SCREEN_BUTTON_BITS[index]);
}

int8_t heldClipboardPreviewSlot() {
  for (uint8_t slot = 0; slot < CLIPBOARD_COUNT; slot++) {
    uint8_t index = slot * CLIPBOARD_ACTION_COUNT + CLIP_ACTION_PREVIEW;
    if (index < CLIPBOARD_BUTTON_COUNT && clipboardButtonStable[index]) return slot;
  }
  return -1;
}

// ============================================================
// PICO ECRAN / LCD 4x20 DELEGUE
// ============================================================
const char* numericEntryLabel() {
  if (numericEntryTarget == NUMERIC_TARGET_BPM) return "IN BPM";
  if (numericEntryTarget == NUMERIC_TARGET_TRACK) return "IN TRK";
  if (numericEntryTarget == NUMERIC_TARGET_CHANNEL) return "IN CHA";
  if (numericEntryTarget == NUMERIC_TARGET_PITCH_BEND) return "IN PBN";
  if (numericEntryMode == EDIT_PITCH) return "IN PITCH";
  if (numericEntryMode == EDIT_VEL) return "IN VEL";
  if (numericEntryMode == EDIT_GATE) return "IN GATE";
  return "IN TIME";
}

FLASHMEM void sendNoteSummaryPacket() {
  if(quickView903==1&&!quickWorking903){blockDisplay21();return;}
  // NOTE was not consumed by Picran; the visible values are carried by GRID/FIELD.
}

TrackRollSettings displayedRollForTrack(uint8_t track) {
  int16_t step = selectedStep;
  uint8_t voice = selectedVoice;
  if (hasSelectedSteps()) {
    step = -1;
    voice = 0;
    for (uint16_t candidate = 0; candidate < STEP_COUNT; candidate++) {
      int8_t selectedVoiceAtStep = selectedRangeVoice(track, candidate);
      if (selectedVoiceAtStep >= 0) {
        step = (int16_t)candidate;
        voice = (uint8_t)selectedVoiceAtStep;
        break;
      }
    }
  }
  if (step < 0 || !stepActive[track][(uint16_t)step]) return trackRoll[track];
  TrackRollSettings roll = configuredRollSettings(track, (uint16_t)step, voice);
  return pendingDisplayedRoll(track, (uint16_t)step, voice, roll);
}

FLASHMEM void sendTrackRollPacket() {
  if(quickView903==1&&!quickWorking903){blockDisplay21();return;}
  auto rx=rollExtra1000(selectedTrack);
  DISPLAY_SERIAL.printf("ROLX1000,%d,%d,%d\n",lockDisplay817(LOCK_ROLL1000,selectedTrack,0,rx.v[0]),lockDisplay817(LOCK_ROLL1000,selectedTrack,1,rx.v[1]),lockDisplay817(LOCK_ROLL1000,selectedTrack,2,rx.v[2]));
  // Legacy ROLL packet was not consumed. GRID/FIELD already supplies its fields.
}

FLASHMEM void noteTexts08(uint8_t note,uint8_t vel,uint16_t gate,int timing){
 char text[4][12];int value[4]={note,vel,gate,timing};EditMode modes[4]={EDIT_PITCH,EDIT_VEL,EDIT_GATE,EDIT_TIME};
 bool selected=selectionHasActiveNotes()||currentEditIsPLock(),multi=selected&&multipleSelectedNotes();
 bool names=((transportTickAbsolute/(96u*16u))&1)!=0;
 for(uint8_t i=0;i<4;++i){auto&f=pendingSelectionEdit.field[(uint8_t)modes[i]];
  int common15=0;bool commonSelection15=selectedStepsHaveCommonValue(modes[i],common15);
  if(selected&&!commonSelection15&&multi){
   int32_t difference=pendingSelectionEdit.active&&f.touched?f.delta:0;
   if(pendingSelectionEdit.active&&f.touched&&(f.sameValue||i==2)){bool found=false;for(uint8_t t=0;t<16&&!found;++t)for(uint16_t st=0;st<trackStepCount(t)&&!found;++st)for(uint8_t v=0;v<noteCountAtStep(t,st);++v)if(pendingSelectionContains(t,st,v)){int original=i==0?noteAtVoice(t,st,v):i==1?effectiveVel(t,st,v):i==2?effectiveGateTicks(t,st,v):effectiveMicroTick(t,st,v);difference=(f.sameValue?f.target:applyGateDraft(original))-original;found=true;break;}}
   snprintf(text[i],12,"%+ld",(long)difference);
  }
  else snprintf(text[i],12,"%d",value[i]);
 }
 if(!multi)noteLabel37(text[0],12,selectedTrack,note);
 if(selectedEmptyCount&&!selectionHasActiveNotes())snprintf(text[2],12,"%d",freeGateValue11());
 if(gateIncrementShown15)snprintf(text[2],12,"%s",gateEditMode==GATE_EDIT_1T?"fin":"\x14");
 DISPLAY_SERIAL.printf("EDIT08|%s|%s|%s|%s\n",text[0],text[1],text[2],text[3]);
}
FLASHMEM void sendFourBlockPacket() {
  if(quickView903==1&&!quickWorking903){blockDisplay21();return;}
  uint8_t track = selectedTrack;
  bool singleNote = selectedStep >= 0 && selectedVoice<noteCountAtStep(track,(uint16_t)selectedStep) && !hasSelectedSteps();
  uint16_t step = singleNote ? (uint16_t)selectedStep : 0;
  uint8_t note = singleNote ? effectiveMidiNote(track, step) : defaultMidiNote[track];
  uint8_t velocity = singleNote ? effectiveVel(track, step, selectedVoice) : defaultVel[track];
  uint16_t gate = singleNote ? effectiveGateTicks(track, step, selectedVoice) : defaultGateTicks[track];
  int16_t timing = singleNote ? effectiveMicroTick(track, step, selectedVoice) : defaultMicroTick[track];
  if (singleNote) {
    note = selectedStepValue(track, step, selectedVoice, EDIT_PITCH);
    velocity = selectedStepValue(track, step, selectedVoice, EDIT_VEL);
    gate = selectedStepValue(track, step, selectedVoice, EDIT_GATE);
    timing = selectedStepValue(track, step, selectedVoice, EDIT_TIME);
  } else if (selectionHasActiveNotes()) {
    int value;
    if (selectedStepsHaveCommonValue(EDIT_PITCH, value)) note = value;
    if (selectedStepsHaveCommonValue(EDIT_VEL, value)) velocity = value;
    if (selectedStepsHaveCommonValue(EDIT_GATE, value)) gate = value;
    if (selectedStepsHaveCommonValue(EDIT_TIME, value)) timing = value;
  }
  TrackRollSettings roll = displayedRollForTrack(track);

  if(timeEncoderMoveMode)timing=moveDisplay939();
  for(uint8_t i=0;i<4;++i)lockBlinkSend11(i,0);
  DISPLAY_SERIAL.print(F("GRID,"));
  DISPLAY_SERIAL.print(track + 1);
  DISPLAY_SERIAL.print(','); DISPLAY_SERIAL.print(midiChannelForTrack(track) + 1);
  DISPLAY_SERIAL.print(','); DISPLAY_SERIAL.print(note);
  DISPLAY_SERIAL.print(','); DISPLAY_SERIAL.print(velocity);
  DISPLAY_SERIAL.print(','); DISPLAY_SERIAL.print(gate);
  DISPLAY_SERIAL.print(','); DISPLAY_SERIAL.print(timing);
  DISPLAY_SERIAL.print(','); DISPLAY_SERIAL.print(trackPitchBend[track]);
  DISPLAY_SERIAL.print(','); DISPLAY_SERIAL.print((int)bpm);
  DISPLAY_SERIAL.print(','); DISPLAY_SERIAL.print(roll.start);
  DISPLAY_SERIAL.print(','); DISPLAY_SERIAL.print(roll.speed);
  DISPLAY_SERIAL.print(','); DISPLAY_SERIAL.print(roll.accel);
  DISPLAY_SERIAL.print(','); DISPLAY_SERIAL.print(roll.expo);
  DISPLAY_SERIAL.print(','); DISPLAY_SERIAL.print(roll.curve);
  DISPLAY_SERIAL.print(','); DISPLAY_SERIAL.print(roll.level);
  DISPLAY_SERIAL.print(','); DISPLAY_SERIAL.print((viewTickOffset / TICKS_PER_STEP) + 1);
  DISPLAY_SERIAL.print(','); DISPLAY_SERIAL.print(trackMuted[track] ? 1 : 0);
  DISPLAY_SERIAL.print(','); DISPLAY_SERIAL.print(0);
  DISPLAY_SERIAL.print(','); DISPLAY_SERIAL.println(trackContext815() ? 3 : 255);
  DISPLAY_SERIAL.print("ROLLMOD825,");DISPLAY_SERIAL.println(rollMod827[selectedTrack]?1:0);
  if(patternMode&&!grooveEditMode)programDisplay939();
  if(!patternMode&&!grooveEditMode){noteTexts08(note,velocity,gate,timing);char text40[12];moveText40(text40);DISPLAY_SERIAL.printf("MOV40,%s\n",text40);if(pitchBendStepShown44)DISPLAY_SERIAL.printf("FIELD,6,T,%s\n",pitchBendStep44==0?"fin":pitchBendStep44==1?"+10":"+100");}
  DISPLAY_SERIAL.print("VOL818,");DISPLAY_SERIAL.print(trackVolume818[selectedTrack]);DISPLAY_SERIAL.print(',');DISPLAY_SERIAL.println(trackChord815);
}

FLASHMEM void sendActiveValuePacket() {
  if(quickView903==1&&!quickWorking903){blockDisplay21();return;}
  if (numericEntryActive) {
    DISPLAY_SERIAL.print(F("VAL,"));
    DISPLAY_SERIAL.print(numericEntryLabel());
    DISPLAY_SERIAL.print(',');
    DISPLAY_SERIAL.print(numericEntryDigits);
    DISPLAY_SERIAL.print(',');
    if (numericEntryTarget == NUMERIC_TARGET_BPM) DISPLAY_SERIAL.print('-');
    else DISPLAY_SERIAL.print(humanTrack(numericEntryTrack));
    DISPLAY_SERIAL.print(',');
    if (numericEntryTarget == NUMERIC_TARGET_PARAM &&
        numericEntryScope == NUMERIC_SCOPE_PLOCK && numericEntryStep >= 0) {
      DISPLAY_SERIAL.print(numericEntryStep + 1);
    } else {
      DISPLAY_SERIAL.print(0);
    }
    DISPLAY_SERIAL.print(F(",KEY,"));
    DISPLAY_SERIAL.println(midiChannelForTrack(selectedTrack) + 1);
    return;
  }

  DISPLAY_SERIAL.print(F("VAL,"));
  DISPLAY_SERIAL.print(uiEditName());
  DISPLAY_SERIAL.print(',');
  if (hasSelectedSteps() && !selectionHasActiveNotes() &&
      uiEditTarget >= UI_EDIT_VEL && uiEditTarget <= UI_EDIT_NOTE) {
    DISPLAY_SERIAL.print(F("???"));
  } else if (hasSelectedSteps() && selectionHasActiveNotes() &&
      uiEditTarget >= UI_EDIT_VEL && uiEditTarget <= UI_EDIT_NOTE) {
    int commonValue = 0;
    if (multipleSelectedNotes() && !multiEditSameForTrack(selectedTrack) && selectionEditDeltaVisible &&
        selectionEditDeltaMode == editMode) {
      if (selectionEditDelta >= 0) DISPLAY_SERIAL.print('+');
      DISPLAY_SERIAL.print(selectionEditDelta);
    } else if (selectedStepsHaveCommonValue(editMode, commonValue)) {
      DISPLAY_SERIAL.print(commonValue);
    } else {
      DISPLAY_SERIAL.print(F("---"));
    }
  } else {
    DISPLAY_SERIAL.print(uiEditValue());
  }
  DISPLAY_SERIAL.print(',');
  DISPLAY_SERIAL.print(humanTrack(selectedTrack));
  DISPLAY_SERIAL.print(',');
  if (selectedStep >= 0) DISPLAY_SERIAL.print(selectedStep + 1);
  else DISPLAY_SERIAL.print(0);
  DISPLAY_SERIAL.print(',');
  DISPLAY_SERIAL.print(uiEditSpeedText());
  DISPLAY_SERIAL.print(',');
  DISPLAY_SERIAL.println(midiChannelForTrack(selectedTrack) + 1);
}

#include "Wrap23.h"
#include "GrooveList40.h"
#include "GrooveListUi.h"


FLASHMEM void sendGroovePacket() {
  static bool wasVisible931=false;
  bool visible931=grooveEditMode||(patternMode&&selectButtonActive);
  if(!visible931){if(wasVisible931){DISPLAY_SERIAL.println("GROOVE,0,0,1,0,0,0,0,0,-1");sendGrooveVisual16();wasVisible931=false;}return;}
  wasVisible931=true;
  bool assigning = patternMode && (grooveEditMode || selectButtonActive);
  DISPLAY_SERIAL.print(F("GROOVE,")); DISPLAY_SERIAL.print(grooveEditMode ? 1 : 0);
  DISPLAY_SERIAL.print(','); DISPLAY_SERIAL.print(assigning ? 1 : 0);
  DISPLAY_SERIAL.print(','); DISPLAY_SERIAL.print(editedGroove);
  for (uint8_t field = 0; field < 5; field++) {
    int value = 0; bool first = true, common = true;
    for (uint8_t step = 0; step < GROOVE_STEPS; step++) {
      if (!(grooveStepMask & (1u << step))) continue;
      int current = field==2?grooveGate51(editedGroove,step):field==4?0:grooveOffset[editedGroove - 1][step][field];
      if (first) { value = current; first = false; } else if (value != current) common = false;
    }
    DISPLAY_SERIAL.print(','); if (common) DISPLAY_SERIAL.print(value); else DISPLAY_SERIAL.print(F("---"));
  }
  int assignment = -1; bool first = true, common = true;
  for (uint8_t t = 0; t < TRACK_COUNT; t++) for (uint8_t pat = 0; pat < PATTERN_COUNT; pat++) {
    if (!patternSelected[t][pat]) continue;
    int current = pendingPatternGroove >= 0 && selectButtonActive ? pendingPatternGroove : patternGroove[t][pat];
    if (first) { assignment = current; first = false; } else if (assignment != current) common = false;
  }
  DISPLAY_SERIAL.print(','); DISPLAY_SERIAL.println(common ? assignment : -1);
  sendGrooveListPacket();
  sendGrooveVisual16();
}

FLASHMEM void toggleGrooveEditor() {
  if (selectButtonActive || deleteButtonActive || pendingSelectionEdit.active) {
    DISPLAY_SERIAL.println(F("MSG|FINISH SELECTION FIRST")); return;
  }
  if (remixMode) (pidiStateDirty931=true, remixMode = false);
  grooveEditMode = !grooveEditMode;
  grooveView16=false;
  memset(grooveAssigned805, 0, sizeof(grooveAssigned805));
  grooveStepMask = 0;
  sendGroovePacket();
  sendSelectionIndicatorsToPicran();
  neoDisplayDirty = displayValueDirty = displayBpmDirty = true;
}

FLASHMEM bool editGrooveFromKy(uint8_t kyIndex, int32_t delta) {
  if (patternMode) return editGrooveList(kyIndex, delta);
  if(kyIndex==4){grooveTrack35(clampInt((int)selectedTrack+delta,0,15));return true;}
  if(kyIndex==15)return true;
  if(kyIndex==12||kyIndex==13){grooveBegin60();grooveEditAssign26();auto &v=grooveShape16[editedGroove-1][kyIndex-12];v=WRAP23(v,delta,0,100);mod805Invalidate();invalidateStepTriggers();sendGrooveVisual16();return true;}
  if(kyIndex>=8&&kyIndex<=11)kyIndex-=6;else if(kyIndex!=0)return true;
  if (kyIndex == 0) {
    grooveFinish60(true);
    grooveFeedbackField = -1;
    editedGroove = (uint8_t)WRAP23(editedGroove,delta,1,GROOVE_COUNT);
    grooveEditTrack23[selectedTrack]=editedGroove;
  } else if (kyIndex >= 2 && kyIndex <= 5) {
    if (!grooveStepMask) { DISPLAY_SERIAL.println(F("MSG|SELECT GROOVE STEPS")); return false; }
    grooveBegin60();
    bool changed805 = false;
    for (uint8_t step = 0; step < GROOVE_STEPS; step++) if (grooveStepMask & (1u << step)) {
      if(kyIndex==4){int old=grooveGate51(editedGroove,step);int next=clampInt(old+delta*(grooveGateTens51?10:1),0,1000);changed805|=old!=next;grooveGateSet51(editedGroove,step,next);continue;}
      int8_t &value = grooveOffset[editedGroove - 1][step][kyIndex - 2];
      int next805 = clampInt((int)value+delta,-99,99); changed805 |= value != next805;
      value = (int8_t)next805;
    }
    if (changed805 && !(grooveAssigned805[selectedTrack] & (1u << activePattern[selectedTrack]))) {
      grooveEditAssign26();
      grooveAssigned805[selectedTrack] |= 1u << activePattern[selectedTrack];
    }
    if (grooveFeedbackField != kyIndex - 2) grooveFeedbackDelta = 0;
    grooveFeedbackField = kyIndex - 2;
    grooveFeedbackDelta = (int16_t)clampInt((int32_t)grooveFeedbackDelta + delta, -198, 198);
    grooveFeedbackUntil = millis() + 700;
    DISPLAY_SERIAL.print(F("GDELTA,")); DISPLAY_SERIAL.print(grooveFeedbackField);
    DISPLAY_SERIAL.print(','); DISPLAY_SERIAL.println(grooveFeedbackDelta);
  } else return false;
  mod805Invalidate();invalidateStepTriggers();
  neoDisplayDirty = displayValueDirty = true;
  sendGroovePacket();
  return true;
}

FLASHMEM void sendDisplayValuePacket() {
  if(!grooveEditMode&&!quickView903){char mv41[12];moveText40(mv41);DISPLAY_SERIAL.printf("MOV40,%s\n",mv41);}
  DISPLAY_SERIAL.printf("STEP26,%u,%u\n",stepContext25(),stepLayout25+1);
  if(nameDisplayR8())return;
  if(kbDisplayR10())return;
  if(!quickWorking903&&quickDisplay903())return;
  if (mod805Display() || patch808Display()) return;
  sendGroovePacket();
  if(grooveEditMode||(patternMode&&selectButtonActive)){sendActiveValuePacket();return;}
  sendFourBlockPacket();
  sendNoteSummaryPacket();
  sendTrackRollPacket();
  sendActiveValuePacket();
}

int8_t gridFieldForKy(uint8_t kyIndex) {
  static const int8_t FIELD[16] = {0, 1, 2, 3, 4, 5, -1, 6, 13, -1, 8, 9, 10, 11, 12, 15};
  return kyIndex < 16 ? FIELD[kyIndex] : -1;
}

FLASHMEM void sendKyFieldPacket(uint8_t kyIndex) {if(kyIndex==6&&!grooveEditMode&&!quickView903){char mv41[12];moveText40(mv41);DISPLAY_SERIAL.printf("MOV40,%s\n",mv41);return;}if(nameDisplayR8())return;if(kbDisplayR10())return;if(patternMode&&!grooveEditMode&&kyIndex==0){programDisplay939();return;}
  if(kyIndex==15||kyIndex==8)sendTrackRollPacket();
  if(!quickWorking903&&quickDisplay903())return;
  if (mod805Display() || patch808Display()) return;
  if (grooveEditMode || (patternMode && selectButtonActive && selectedPatternCount())) { sendGroovePacket(); return; }
  if(selectionHasActiveNotes()){sendFourBlockPacket();sendTrackRollPacket();} // Refresh the selection snapshot only on an edit event.
  if(kyIndex==6){char mv[12];moveText40(mv);DISPLAY_SERIAL.printf("MOV40,%s\n",mv);return;}
  int8_t fieldIndex = gridFieldForKy(kyIndex);
  if (fieldIndex < 0) return;
  if(kyIndex==0&&trackContext815()){
    DISPLAY_SERIAL.print("FIELD,0,V,");DISPLAY_SERIAL.println(trackVolume818[selectedTrack]);return;
  }
  DISPLAY_SERIAL.print(F("FIELD,"));
  DISPLAY_SERIAL.print(fieldIndex);
  DISPLAY_SERIAL.print(',');
  if(kyIndex==7&&pitchBendStepShown44){DISPLAY_SERIAL.printf("T,%s\n",pitchBendStep44==0?"fin":pitchBendStep44==1?"+10":"+100");return;}

  bool selectionValueField = (kyIndex >= 2 && kyIndex <= 6) || kyIndex == 8 ||
                             (kyIndex >= 10 && kyIndex <= 14);
  if (selectionValueField && hasSelectedSteps() && !selectionHasActiveNotes()) {
    DISPLAY_SERIAL.println(F("T,???"));
    return;
  }
  if (kyIndex >= 2 && kyIndex <= 6 && selectionHasActiveNotes()) {
    int commonValue = 0;
    if (multipleSelectedNotes() && !multiEditSameForTrack(selectedTrack) && selectionEditDeltaVisible &&
        selectionEditDeltaMode == editMode) {
      DISPLAY_SERIAL.print(F("T,"));
      if (selectionEditDelta >= 0) DISPLAY_SERIAL.print('+');
      DISPLAY_SERIAL.println(selectionEditDelta);
      return;
    }
    if (!selectedStepsHaveCommonValue(editMode, commonValue)) {
      DISPLAY_SERIAL.println(F("T,---"));
      return;
    }
    DISPLAY_SERIAL.print(F("N,"));
    DISPLAY_SERIAL.println(commonValue);
    return;
  }

  int value = 0;
  if (kyIndex == 0) value = selectedTrack + 1;
  else if (kyIndex == 1) value = midiChannelForTrack(selectedTrack) + 1;
  else if (kyIndex == 2) value = pitchEncoderSelectMode ? defaultMidiNote[selectedTrack]
                                                       : getCurrentEditedValue();
  else if (kyIndex >= 3 && kyIndex <= 5) value = getCurrentEditedValue();
  else if (kyIndex == 6) value = trackPitchBend[selectedTrack];
  else if (kyIndex == 7) value = trackPitchBend[selectedTrack];
  else if (kyIndex == 15) value=lockDisplay817(LOCK_ROLL1000,selectedTrack,0,rollExtra1000(selectedTrack).v[0]);
  else {
    TrackRollSettings roll = displayedRollForTrack(selectedTrack);
    uint8_t rollField = kyIndex == 8 ? 5 : kyIndex - 10;
    value = rollFieldValue(roll, rollField);
  }
  DISPLAY_SERIAL.print(F("N,"));
  DISPLAY_SERIAL.println(value);
}

FLASHMEM void sendDisplayBpmPacket() {
  songStateDirty931=true;
  DISPLAY_SERIAL.print(F("BPM,"));
  DISPLAY_SERIAL.print(selectionMode ? 1 : 0);
  DISPLAY_SERIAL.print(',');
  DISPLAY_SERIAL.print((int)bpm);
  DISPLAY_SERIAL.print(',');
  DISPLAY_SERIAL.print(beatEditActive ? beatDraftBars : sequenceBars);
  DISPLAY_SERIAL.print(',');
  DISPLAY_SERIAL.print((viewTickOffset / TICKS_PER_STEP) + 1);
  DISPLAY_SERIAL.print(',');
  DISPLAY_SERIAL.print((loopBarMode817[0]?1:0)|(loopBarMode817[1]?2:0));
  DISPLAY_SERIAL.print(',');
  DISPLAY_SERIAL.print(loopStartStep);
  DISPLAY_SERIAL.print(',');
  DISPLAY_SERIAL.print(loopEndStep);
  DISPLAY_SERIAL.print(',');
  DISPLAY_SERIAL.print(displayedPattern);
  uint32_t barTicks = (uint32_t)STEPS_PER_BAR * TICKS_PER_STEP;
  uint32_t viewPosition = min((uint32_t)sequenceBars, (viewTickOffset / barTicks) + 1);
  DISPLAY_SERIAL.print(',');
  DISPLAY_SERIAL.print(viewPosition);
  DISPLAY_SERIAL.print(',');
  DISPLAY_SERIAL.print(beatEditLength ? 1 : 0);
  DISPLAY_SERIAL.print(',');
  DISPLAY_SERIAL.print(zoomLevel);
  DISPLAY_SERIAL.print(',');
  DISPLAY_SERIAL.print(roy1EditsZoom ? 1 : 0);
  DISPLAY_SERIAL.println();
}

static const uint8_t SETTINGS_PAGE_COUNT = 8;
static const uint8_t SETTINGS_CATEGORY_COUNT = 12;

// ============================================================
// REGLES PERMANENTES DE TOUS LES MENUS
// ============================================================
// - Aucun ecran ne doit afficher "PAGE x/y" : cette information masque
//   l'explication utile et ne participe jamais a la navigation.
// - Si une page suivante existe, le dernier item visible porte > a sa fin.
// - Sur toute page apres la premiere, le premier item visible commence par <.
// - Ces deux chevrons font partie du nom du bouton et valent pour toutes les
//   branches de menu existantes ou ajoutees plus tard.
// - Entrer dans un menu execute un vrai STOP, puis seules les commandes de
//   navigation de ce menu restent actives jusqu'au retour a l'ecran principal.
// - Retour restaure exactement le parent et sa page, jamais une page imposee.
bool settingsOpen = false;
uint8_t settingsPage = 0;
int16_t settingsDraftValue = 0;
uint8_t settingsItemSelection = 0;
uint8_t settingsCategorySelection = 0;
uint8_t settingsCategoryTop = 0;
uint8_t settingsCategoryPage = 0;
uint8_t settingsMarqueeOffset = 0;
uint32_t settingsMarqueeLastMs = 0;
bool settingsPageRetryPending = false;
bool settingsPageRetrySending = false;
uint32_t settingsPageLastSendMs = 0;

enum SettingsMenuLevel : uint8_t {
  SETTINGS_CATEGORIES = 0,
  SETTINGS_ITEMS,
  SETTINGS_EDIT,
  SETTINGS_PICO_ITEMS,
  SETTINGS_PICO_EDIT,
  SETTINGS_COLOR_ITEMS,
  SETTINGS_COLOR_EDIT,
  SETTINGS_MIXER_EDIT915,
  SETTINGS_ROY_ITEMS,
  SETTINGS_ROY_EDIT,
  SETTINGS_SEQ_EXTRA_EDIT,
  SETTINGS_TRANSPORT_ITEMS,
  SETTINGS_TRANSPORT_EDIT,
  SETTINGS_MAIN_ITEMS,
  SETTINGS_STABILITY904
};
SettingsMenuLevel settingsMenuLevel = SETTINGS_CATEGORIES;
uint8_t mixerOriginal915[4]={};
uint8_t settingsColorSelection = 0;
uint8_t settingsColorDraft = 0;
uint8_t settingsColorOriginal = 0;
uint8_t settingsColorPage = 0;
int8_t settingsEditingRoy = -1;
int16_t settingsRoyOriginal = 0;
uint8_t settingsSeqExtraSelection = 0; // 0=Preview, 1=Head, 2=View
uint8_t settingsSeqExtraDraft = 0;
uint8_t settingsTransportSelection = 0; // 0=attente, 1=vitesse, 2=BPM par defaut
uint16_t settingsTransportDraft = 0;
uint8_t settingsAccessMode = 2; // Compatibilite EEPROM; les menus imposent toujours STOP.

static const char* SETTINGS_ITEM_NAMES[SETTINGS_PAGE_COUNT][4] = {
  {"Button scan", "Button press", "Button release", "Screen delay"},
  {"Function hold", "Debounce", "Pulses/detent", "Rotation dir"},
  {"Dead zone", "Center X", "Center Y", "Sensitivity"},
  {"Min delay", "Neo select hold", "Safe display", "Active screen"},
  {"MIDI USB", "MIDI DIN", "MIDI THRU", "MIDI clock"},
  {"Auto select last note", "Colors", "Multi edit", "Roll delay steps"},
  {"KY40 test", "Neo test", "KY23 test", "Roy test"},
  {"Preview mode", "Paste hold", "Default paste", "Preview queue"}
};

static const char* SETTINGS_MENU_CATEGORY_SHORT[SETTINGS_CATEGORY_COUNT] = {
  "MAIN", "ROY", "KY40", "KY23", "SCR", "MIDI", "SEQ", "BTN", "COPY", "DBG", "QUICK ACCESS"
};

static const char* SETTINGS_ITEM_SHORT[SETTINGS_PAGE_COUNT][4] = {
  {"Assign", "PRES", "RELS", "DLY"},
  {"HELP", "DBNC", "PULS", "DIR"},
  {"DEAD", "CTRX", "CTRY", "SENS"},
  {"DLY", "NEOH", "SAFE", "ACT"},
  {"USB", "DIN", "THRU", "CLK"},
  {"AUTO", "COLR", "MULT", "RDEL"},
  {"KY40", "NEO", "KY23", "ROY"},
  {"PREV", "HOLD", "PAST", "QUE"}
};

static const char* SETTINGS_PAGE_DESCRIPTIONS[SETTINGS_PAGE_COUNT] = {
  "Transport button scanning and screen response timing",
  "KY40 encoder movement and direction settings",
  "Joystick centre dead zone and sensitivity calibration",
  "Display refresh selection hold and active screen options",
  "MIDI input output thru and clock routing",
  "Sequencer editing display and preview behaviour",
  "Run hardware diagnostics without musical actions",
  "Clipboard preview paste and queue behaviour"
};

static const char* SETTINGS_ITEM_DESCRIPTIONS[SETTINGS_PAGE_COUNT][4] = {
  {"Time between button scans", "Stable time before a press", "Stable time before release", "Delay before screen update"},
  {"Hold time before help", "Ignore encoder reversals", "Pulses for one value", "Physical rotation direction"},
  {"Neutral area around centre", "Horizontal centre value", "Vertical centre value", "Movement sensitivity"},
  {"Minimum display refresh delay", "Hold time to select a note", "Use conservative display timing", "Choose the active screen"},
  {"Enable USB MIDI", "Enable DIN MIDI", "Forward incoming MIDI", "Use or send MIDI clock"},
  {"Select newly created notes", "Choose Neo colour roles", "How multiple notes are edited", "Delay before roll changes audio"},
  {"Test all KY40 encoders", "Test Neo pads and LEDs", "Show live joystick axes", "Test all Roy encoders"},
  {"How clipboard preview plays", "Hold time before paste menu", "Erase or overdub by default", "Maximum queued previews"}
};

static const char* ROY_SETTING_DESCRIPTIONS[4] = {
  "Minimum time between Roy actions", "Stable time before Roy click",
  "Encoder pulses for one value", "Use the same rotation sense everywhere"
};

static const uint8_t PICO_SETTINGS_ITEM_COUNT = 4;
static const char* PICO_SETTINGS_NAMES[PICO_SETTINGS_ITEM_COUNT] = {
  "Pulses/value", "Reverse guard", "Button debounce", "RX recovery"
};
static const char* PICO_SETTINGS_SHORT[PICO_SETTINGS_ITEM_COUNT] = {
  "PULS", "REVR", "BTND", "RCVR"
};
static const char* PICO_SETTING_DESCRIPTIONS[PICO_SETTINGS_ITEM_COUNT] = {
  "Pico pulses required for one value", "Reject brief reverse movements",
  "Stable time before KY40 click", "Recover automatically after lost data"
};

static const char* DEBUG_TEST_SHORT[DEBUG_TEST_COUNT] = {
  "KY40", "NEO", "KY23", "ROY", "BE", "BTNS", "TRNS", "CLIP", "SCRN", "KEYP", "CV"
};

static const uint8_t SETTINGS_CATEGORY_PAGE[SETTINGS_CATEGORY_COUNT] = {254, 255, 1, 2, 3, 4, 5, 0, 7, 6, 253, 252};
static const char* SETTINGS_CATEGORY_NAMES[SETTINGS_CATEGORY_COUNT] = {
  "MAIN", "ROY", "KY40", "KY23", "SCREEN", "MIDI", "SEQ", "BUTTON", "COPY", "DEBUG", "QUICK ACCESS", "NUMERIC VALUE"
};
static const char* SETTINGS_CATEGORY_DESCRIPTIONS[SETTINGS_CATEGORY_COUNT] = {
  "General interface behaviour",
  "Roy encoder settings",
  "KY-040 encoder settings",
  "KY-023 control settings",
  "Display and timing",
  "MIDI connections and clock",
  "Sequencer behaviour and colors",
  "Playback and transport buttons",
  "Copy paste and preview modes",
  "Hardware tests and diagnostics",
  "Enable quick value access buttons",
  "Enable numeric editing of the current value"
};

static const char* ROY_SETTING_NAMES[4] = {
  "Function guard", "Button debounce", "Pulses/value", "Rotation dir"
};
static const char* ROY_SETTING_SHORT[4] = {"GUAR", "DBNC", "PULS", "DIR"};

int16_t settingsValue[SETTINGS_PAGE_COUNT][4] = {
  {5, 15, 15, 80},
  {3000, 0, 2, 1},
  {30, 512, 512, 100},
  {80, 700, 0, 0},
  {1, 1, 0, 1},
  {0, 0, 0, 0},
  {384, 96, 115, 0},
  {0, 500, 0, 6}
};

static const uint32_t SETTINGS_EEPROM_MAGIC = 0x554B5331UL;
static const uint16_t SETTINGS_EEPROM_VERSION = 4;
static const uint32_t PICO_SETTINGS_EEPROM_MAGIC = 0x50494331UL;
static const uint32_t SEQ_SETTINGS_EEPROM_MAGIC = 0x53455131UL;
int16_t picoButtonDebounceMs = PICO_DEFAULT_B;
int16_t picoRecoveryMode = 1;

struct PersistedSettings {
  uint32_t magic;
  uint16_t version;
  int16_t values[SETTINGS_PAGE_COUNT][4];
  uint16_t checksum;
};

struct PersistedPicoSettings {
  uint32_t magic;
  int16_t buttonDebounceMs;
  int16_t recoveryMode;
  uint16_t checksum;
};

struct PersistedSeqSettings {
  uint32_t magic;
  int8_t trackAutoSelect[TRACK_COUNT];
  int8_t trackMultiEdit[TRACK_COUNT];
  uint8_t colors[NEO_COLOR_ROLE_COUNT];
  uint16_t checksum;
};

static const uint32_t UI2_SETTINGS_EEPROM_MAGIC = 0x554B4332UL;
static const uint16_t UI2_SETTINGS_EEPROM_VERSION = 1;
struct PersistedUi2Settings {
  uint32_t magic;
  uint16_t version;
  uint8_t globalPreview;
  uint8_t headFollow;
  int8_t trackPreview[TRACK_COUNT];
  uint8_t trackPlay[TRACK_COUNT];
  uint16_t royGuard;
  uint16_t royDebounce;
  uint8_t royPulses;
  int8_t royDirection;
  uint16_t checksum;
};
uint16_t ui2SettingsChecksum(const PersistedUi2Settings &data);

static const uint32_t TRACK_OVERRIDE_EEPROM_MAGIC = 0x54524B36UL;
struct PersistedTrackOverrides {
  uint32_t magic;
  int8_t colors[TRACK_COUNT][NEO_COLOR_ROLE_COUNT];
  int8_t viewMode[TRACK_COUNT];
  uint8_t globalViewMode;
  uint8_t accessMode;
  uint16_t checksum;
};
uint16_t trackOverrideChecksum(const PersistedTrackOverrides &data);

uint16_t trackOverrideChecksum(const PersistedTrackOverrides &data) {
  uint16_t checksum = 0x4A31;
  for (uint8_t track = 0; track < TRACK_COUNT; track++) {
    for (uint8_t role = 0; role < NEO_COLOR_ROLE_COUNT; role++) {
      checksum = (uint16_t)((checksum << 3) | (checksum >> 13));
      checksum ^= (uint8_t)(data.colors[track][role] + 1);
    }
    checksum = (uint16_t)((checksum << 3) | (checksum >> 13));
    checksum ^= (uint8_t)(data.viewMode[track] + 1);
  }
  checksum = (uint16_t)((checksum << 3) | (checksum >> 13));
  checksum ^= data.globalViewMode;
  checksum = (uint16_t)((checksum << 3) | (checksum >> 13));
  checksum ^= data.accessMode;
  return checksum;
}

uint16_t settingsChecksum(const PersistedSettings &data);

char menuScreenRaw[4][96] = {{0}};
uint8_t menuScrollLine = 255;
uint8_t menuScrollOffset = 0;
bool menuStopLatched = false;
uint32_t menuScrollAt905=0;

void setTransportPositionToMeasureStart();

void stopSequencerForMenu() {
  if(stableTrack904&&trackMenuActive)return;
  if(songEngineActive) return;
  if (patMenu46 || barFillPromptActive || itemKind809 || itemChoosing809 || modUi805 || patchUi808 || menuStopLatched) return;
  if (playing) autosave805Request();
  menuStopLatched = true;
  stopClipboardPreview(true, true);
  (pidiStateDirty931=true, playing = false);
  internalClockRunning = false;
  noInterrupts();
  pendingClockTicks = 0;
  interrupts();
  cycleCounter = 0;
  transportMeasureRepeatAtMs[0] = transportMeasureRepeatAtMs[1] = 0;
  transportMeasureWasPlaying[0] = transportMeasureWasPlaying[1] = false;
  memset(neoPadDown, 0, sizeof(neoPadDown));
  memset(kyBtnDown, 0, sizeof(kyBtnDown));
  patternPasteTargetHeld = false;
  setTransportPositionToMeasureStart();
  Serial.println(F("MENU: SEQUENCE STOPPED, NAVIGATION ONLY"));
}

bool menuNavigationOnly() {
  if(quickWorking903&&quickView903==1)return debugTestMode!=DEBUG_TEST_NONE;
  return sdMenu42 || itemKind809 || itemChoosing809 || (!neoInput932&&(patchUi808 || modUi805 || menuStopLatched)) || debugTestMode != DEBUG_TEST_NONE;
}

void renderMenuScreenRaw() {
  DISPLAY_SERIAL.print(F("UIB|"));
  for(uint8_t i=0;i<4;++i){char mask[6];snprintf(mask,sizeof(mask),"%05lX",(unsigned long)(menuBlink821[i]&0xFFFFF));DISPLAY_SERIAL.print(mask);}
  DISPLAY_SERIAL.print('|');
  for (uint8_t row = 0; row < 4; row++) {
    if (row > 0) DISPLAY_SERIAL.print('|');
    const char* text = menuScreenRaw[row];
    if (row == menuScrollLine && strlen(text) > 20) {
      char window[21];
      size_t length = strlen(text);
      size_t maxOffset = length - 20;
      size_t offset = min((size_t)menuScrollOffset, maxOffset);
      memcpy(window, text + offset, 20);
      window[20] = '\0';
      DISPLAY_SERIAL.print(window);
    } else {
      char window[21];
      strncpy(window, text, 20);
      window[20] = '\0';
      DISPLAY_SERIAL.print(window);
    }
  }
  DISPLAY_SERIAL.println();
  if(menuScrollLine<4){DISPLAY_SERIAL.print("SCROLLTEXT|");DISPLAY_SERIAL.print(menuScrollLine);DISPLAY_SERIAL.print('|');DISPLAY_SERIAL.print(menuScrollOffset);DISPLAY_SERIAL.print('|');DISPLAY_SERIAL.println(menuScreenRaw[menuScrollLine]);}
}

FLASHMEM void sendMenuScreen(const char* line0, const char* line1, const char* line2, const char* line3) {
  clearMenuBlink821();
  settingsPageRetryPending = false;
  stopSequencerForMenu();
  const char* incoming[4] = {line0 ? line0 : "", line1 ? line1 : "",
                             line2 ? line2 : "", line3 ? line3 : ""};
  bool changed = false;
  for (uint8_t row = 0; row < 4; row++) {
    if (strncmp(menuScreenRaw[row], incoming[row], sizeof(menuScreenRaw[row])) != 0) changed = true;
  }
  for (uint8_t row = 0; row < 4; row++) {
    strncpy(menuScreenRaw[row], incoming[row], sizeof(menuScreenRaw[row]) - 1);
    menuScreenRaw[row][sizeof(menuScreenRaw[row]) - 1] = '\0';
  }
  if (changed) {menuScrollOffset = 0;menuScrollAt905=millis()+1200;}
  menuScrollLine = 255;
  const uint8_t priority[4] = {3, 1, 0, 2};
  for (uint8_t index = 0; index < 4; index++) {
    uint8_t row = priority[index];
    if (strlen(menuScreenRaw[row]) > 20) {
      menuScrollLine = row;
      break;
    }
  }
  renderMenuScreenRaw();
}

bool scrollMenuText(int32_t delta) {
  if (delta == 0 || menuScrollLine >= 4) return false;
  size_t length = strlen(menuScreenRaw[menuScrollLine]);
  if (length <= 20) return false;
  int next = (int)menuScrollOffset + (delta > 0 ? 1 : -1);
  menuScrollOffset = (uint8_t)clampInt(next, 0, (int)length - 20);
  DISPLAY_SERIAL.print("SCROLLOFF,");DISPLAY_SERIAL.println(menuScrollOffset);
  return true;
}

void sendDebugDisplay(const char* line0, const char* line1, const char* line2,
                      const char* line3, const char* small0, const char* small1) {
  stopSequencerForMenu();
  DISPLAY_SERIAL.print(F("DBG|"));
  DISPLAY_SERIAL.print(line0 ? line0 : "");
  DISPLAY_SERIAL.print('|');
  DISPLAY_SERIAL.print(line1 ? line1 : "");
  DISPLAY_SERIAL.print('|');
  DISPLAY_SERIAL.print(line2 ? line2 : "");
  DISPLAY_SERIAL.print('|');
  DISPLAY_SERIAL.print(line3 ? line3 : "");
  DISPLAY_SERIAL.print('|');
  DISPLAY_SERIAL.print(small0 ? small0 : "");
  DISPLAY_SERIAL.print('|');
  DISPLAY_SERIAL.println(small1 ? small1 : "");
}

void stopDebugTest() {
  if (debugTestMode == DEBUG_TEST_NONE) return;
  if(debugTestMode==DEBUG_TEST_CV47)cvDebugStop47();
  debugTestMode = DEBUG_TEST_NONE;
  debugNeoLastIndex = 255;
  debugNeoLastPad = 255;
  // Le rendu du parent quitte DEBUG dans une seule trame UI complete.
  neoDisplayDirty = true;
  Serial.println(F("DEBUG TEST STOP"));
}

FLASHMEM void showGameScreen() {neoDisplayDirty=true;patMenu46=0;storageDirty931=true;kbMenuR10=0;delayTargetsOpen55=false;
  // Stop the old menu producer, otherwise its marquee reopens the menu after HOME.
  menuScrollLine=255;menuScrollOffset=0;menuScrollAt905=0;clearMenuBlink821();
  modUi805 = patchUi808 = 0;
  stopDebugTest();
  barFillPromptActive = false;
  barDeletePromptActive = false;
  patternModeEditActive = false;
  settingsOpen = false;
  writeMenuActive = false;
  settingsPageRetryPending = false;
  saveMenuActive = false;
  saveMenuClipboardMode = false;
  fileBrowserMode = FILE_BROWSER_NONE;
  loadTypeMenuActive = false;
  patternLoadConfirmActive = false;
  afterSaveAction = AFTER_SAVE_NONE;
  midiInMenu17=0;trackMenuActive = false;linkMenu12=false;trackMidiMenu902=false;trackPage933=trackHomePage938=0;
  trackResetConfirmActive = false;
  trackAutoMenuActive = false;
  trackMultiMenuActive = false;
  trackPreviewMenuActive = false;
  trackPlayMenuActive = false;
  trackColorMenuActive = false;
  trackViewMenuActive = false;
  trackDrumWarningActive = false;
  trackGateLengthPromptActive = false;
  trackMenuEditingRoy = -1;
  pendingPatternPath[0] = '\0';
  clipboardMenuActive = false;
  clipboardPasteMenuActive = false;
  numericEntryActive = false;
  numericEntryLen = 0;
  numericEntryDigits[0] = '\0';
  menuStopLatched = false;
  DISPLAY_SERIAL.println(F("UI|GAME"));
  displayValueDirty = true;
  displayBpmDirty = true;
}

void sendTrackMidi902(){
 trackMidiMenu902=true;trackMenuActive=true;
 char values[8][5];const char* labels[8];for(uint8_t i=0;i<8;++i){snprintf(values[i],5,"%u%s",i+1,(trackMidiDraft902&(1u<<i))?" ON":"");labels[i]=values[i];}char top[21],bottom[21],title[21],value[64];
 makeEightButtonLines(top,bottom,labels);snprintf(title,21,"TRACK %02u MIDI OUT",selectedTrack+1);
 strcpy(value,"MIDI OUT");
 sendMenuScreen(title,value,top,bottom);
}
void commitTrackMidi902(){
 allNotesOff();pidiCancel827();(pidiRoutesDirty931|=1u<<(selectedTrack), trackOut902[linkOwner12(selectedTrack,LINK_MOUT12)]=trackMidiDraft902);pidiRoutesDirty931=0xffff;midiReset810();save902();sendTrackMidi902();
}
FLASHMEM void sendTrackMenu() {midiInMenu17=0;linkMenu12=false;
  trackPage933=0;
  trackMidiMenu902=false;
  trackMenuActive = true;
  trackResetConfirmActive = false;
  trackAutoMenuActive = false;
  trackMultiMenuActive = false;
  trackPreviewMenuActive = false;
  trackPlayMenuActive = false;
  trackColorMenuActive = false;
  trackViewMenuActive = false;
  trackDrumWarningActive = false;
  trackGateLengthPromptActive = false;
  trackMenuEditingRoy = -1;
  const char* labels[18] = {"MIDI", "MULT", "CC", "VIEW",
                           "POLY", "NPEW", "RST", "PATCH", "MIXER", "MODFX", "DELAY", "CAM<", "CAM>", "KB", "Name", "Link", "Chord", "Arp"};
  char top[21], bottom[21], title[21];
  makePagedMenu(top, bottom, labels,18,trackHomePage938);
  snprintf(title, sizeof(title), "TRACK %02u PARAMETERS", selectedTrack + 1);
  sendMenuScreen(title, "OB: select track", top, bottom);
}

int8_t wideThreeChoice(uint8_t buttonIndex) {
  if (buttonIndex <= 1) return 0; // le premier label occupe deux zones
  if (buttonIndex == 2) return 1;
  if (buttonIndex == 3) return 2;
  return -1;
}

void makeWideThreeOptionLine(char line[21], const char* first, const char* second,
                             const char* third, uint8_t selected) {
  memset(line, ' ', 20);
  line[20] = '\0';
  const char* labels[3] = {first, second, third};
  const uint8_t starts[3] = {0, 10, 15};
  const uint8_t widths[3] = {10, 5, 5};
  for (uint8_t option = 0; option < 3; option++) {
    line[starts[option]] = selected == option ? '*' : ' ';
    size_t length = strlen(labels[option]);
    if (length > (size_t)(widths[option] - 1)) length = widths[option] - 1;
    memcpy(line + starts[option] + 1, labels[option], length);
  }
}

bool headFollowForTrack(uint8_t track) {
  (void)track;
  return headFollow;
}

NoteViewMode noteViewForTrack(uint8_t track) {
  if (track >= TRACK_COUNT || trackNoteViewMode[linkOwner12(track,LINK_VIEW12)] < 0) return globalNoteViewMode;
  return trackNoteViewMode[linkOwner12(track,LINK_VIEW12)] == NOTE_VIEW_CHROMATIC ? NOTE_VIEW_CHROMATIC : NOTE_VIEW_COMPACT;
}

FLASHMEM void sendTrackColorMenu() {
  trackMenuActive = true;
  trackColorMenuActive = true;
  trackViewMenuActive = false;
  if (trackColorRole > NEO_COLOR_REMIX) trackColorRole = NEO_COLOR_NOTE;
  if (trackColorDraft < -1 || trackColorDraft >= (int8_t)NEO_PALETTE_COUNT) {
    trackColorDraft = -1;
  }
  char title[21], typeLine[21], colorLine[21];
  snprintf(title, sizeof(title), "T%02u COLOUR OVERRIDE", selectedTrack + 1);
  snprintf(typeLine, sizeof(typeLine), "TYPE: %s", NEO_COLOR_ROLE_NAME[trackColorRole]);
  snprintf(colorLine, sizeof(colorLine), "COLOUR: %s",
           trackColorDraft < 0 ? "DEFAULT" : NEO_PALETTE[trackColorDraft].name);
  sendMenuScreen(title, typeLine, colorLine, "R1 TYPE / R2 COLOUR");
}

FLASHMEM void sendTrackViewMenu() {
  trackMenuActive = true;
  trackColorMenuActive = false;
  trackViewMenuActive = true;
  char top[21], title[21], detail[21];
  snprintf(title, sizeof(title), "T%02u NOTE VIEW", selectedTrack + 1);
  int8_t mode = trackNoteViewMode[linkOwner12(selectedTrack,LINK_VIEW12)];
  makeWideThreeOptionLine(top, "DEFAULT", "COND", "CHROM", mode < 0 ? 0 : (mode ? 2 : 1));
  snprintf(detail, sizeof(detail), "%s", mode < 0 ? "Uses global VIEW" : "Track override");
  sendMenuScreen(title, "Choose how pitch rows are arranged", top, detail);
}

FLASHMEM void sendTrackMultiMenu() {
  trackMenuActive = true;
  trackResetConfirmActive = false;
  trackAutoMenuActive = false;
  trackMultiMenuActive = true;
  char top[21], title[21], detail[21];
  snprintf(title, sizeof(title), "T%02u MULTI EDIT", selectedTrack + 1);
  int8_t mode = trackMultiEditMode[selectedTrack];
  makeWideThreeOptionLine(top, "DEFAULT", "KEEP", "SAME", mode < 0 ? 0 : (mode ? 2 : 1));
  snprintf(detail, sizeof(detail), "%s", mode < 0 ? "Uses global MULT" : "Track override");
  sendMenuScreen(title, "Edit selected notes", top, detail);
}

bool notePreviewForTrack(uint8_t track) {
  if (track >= TRACK_COUNT) return false;
  int8_t mode = trackNotePreviewMode[track];
  return mode < 0 ? globalNotePreview : mode != 0;
}

FLASHMEM void sendTrackPreviewMenu() {
  trackMenuActive = true;
  trackResetConfirmActive = false;
  trackAutoMenuActive = false;
  trackMultiMenuActive = false;
  trackPreviewMenuActive = true;
  trackPlayMenuActive = false;
  char top[21], title[21], detail[21];
  snprintf(title, sizeof(title), "T%02u NOTE PREVIEW", selectedTrack + 1);
  int8_t mode = trackNotePreviewMode[selectedTrack];
  makeWideThreeOptionLine(top, "DEFAULT", "ON", "OFF", mode < 0 ? 0 : (mode ? 1 : 2));
  snprintf(detail, sizeof(detail), "%s", mode < 0 ? "Uses global PREV" : "Track override");
  sendMenuScreen(title, "Play note on pitch edit", top, detail);
}

const char* trackPlayModeName(TrackPlayMode mode) {
  if (mode == TRACK_PLAY_DRUM) return "DRUM";
  if (mode == TRACK_PLAY_MONO) return "MONO";
  return "POLY";
}

FLASHMEM void sendTrackPlayMenu() {
  trackMenuActive = true;
  trackResetConfirmActive = false;
  trackAutoMenuActive = false;
  trackMultiMenuActive = false;
  trackPreviewMenuActive = false;
  trackPlayMenuActive = true;
  trackDrumWarningActive = false;
  trackGateLengthPromptActive = false;
  char top[21], title[21];
  makeWideThreeOptionLine(top, "DRUM", "MONO", "POLY", (uint8_t)trackPlayMode[linkOwner12(selectedTrack,LINK_POLY12)]);
  snprintf(title, sizeof(title), "T%02u POLYPHONY", selectedTrack + 1);
  sendMenuScreen(title, "How notes overlap", top, "DRUM HAS NO NOTE OFF");
}

FLASHMEM void sendDrumGateWarning() {
  trackDrumWarningActive = true;
  trackGateLengthPromptActive = false;
  const char* labels[8] = {"NO", "YES", "SAVE", "CONV",
                           nullptr, nullptr, nullptr, nullptr};
  char top[21], bottom[21];
  makeEightButtonLines(top, bottom, labels);
  sendMenuScreen("TURN TO DRUM MODE", "WILL LOSE GATES", top, "BE3/4 SAVE&CONVERT");
}

FLASHMEM void sendGateLengthPrompt() {
  trackDrumWarningActive = false;
  trackGateLengthPromptActive = true;
  const char* labels[8] = {"NO", "YES", "SAVE", "CONV",
                           nullptr, nullptr, nullptr, nullptr};
  char top[21], bottom[21], value[21];
  makeEightButtonLines(top, bottom, labels);
  snprintf(value, sizeof(value), "%u TICKS", trackGateLengthDraft);
  sendMenuScreen("SET GATE LENGTH", value, top, "BE3/4 SAVE&CONVERT");
}

FLASHMEM void beginSaveBeforeTrackModeConversion() {
  if (!storageWriteAvailable) {
    DISPLAY_SERIAL.println(sdAvailable ? F("MSG|SD WRITE ERROR") : F("MSG|SD MISSING"));
    return;
  }
  afterSaveAction = AFTER_SAVE_TRACK_MODE_CONVERSION;
  afterSaveTrack = selectedTrack;
  afterSaveTrackMode = pendingTrackPlayMode;
  afterSaveGateTicks = trackGateLengthDraft;
  saveMenuPausedPlayback = playing;
  if (playing) {
    (pidiStateDirty931=true, playing = false);
    internalClockRunning = anyLiveClipboardPreview() || clipboardPreviewActive;
    allNotesOff();
  }
  trackDrumWarningActive = false;
  trackGateLengthPromptActive = false;
  openSaveBrowser(SAVE_CANDIDATE_SEQUENCE, false, 0);
}

void applyTrackPlayModeConversion() {
  uint8_t track = selectedTrack;
  discardNoteRedo();
  stopActiveNotesForTrack(track);
  uint16_t gate = pendingTrackPlayMode == TRACK_PLAY_DRUM ? 1 : trackGateLengthDraft;
  for (uint16_t step = 0; step < trackStepCount(track); step++) {
    if (!stepActive[track][step]) continue;
    stepGateTicks[track][step] = gate;
    stepLockGate[track][step] = true;
  }
  if (pendingTrackPlayMode != TRACK_PLAY_DRUM) defaultGateTicks[track] = gate;
  (pidiRoutesDirty931|=1u<<(track), trackPlayMode[linkOwner12(track,LINK_POLY12)] = pendingTrackPlayMode);
  trackDrumWarningActive = false;
  trackGateLengthPromptActive = false;
  saveSettingsToEEPROM();
  if (conversionReturnsHome) {
    conversionReturnsHome = false;
    showGameScreen();
  } else {
    sendTrackMenu();
  }
  neoDisplayDirty = true;
  displayValueDirty = true;
}

bool multiEditSameForTrack(uint8_t track) {
  if (track >= TRACK_COUNT) return multiEditSameValue;
  int8_t mode = trackMultiEditMode[track];
  return mode < 0 ? multiEditSameValue : mode != 0;
}

FLASHMEM void sendTrackAutoMenu() {
  trackMenuActive = true;
  trackResetConfirmActive = false;
  trackAutoMenuActive = true;
  trackMultiMenuActive = false;
  sendMenuScreen("AUTO SELECT OFF", "Creation: no select", "Use SELECT mode", "J1 BACK");
}

bool autoSelectLastNoteForTrack(uint8_t track) {
  // Creation and selection are separate actions, including saved AUTO settings.
  (void)track;
  return false;
}

static uint8_t trackInitMask57=0;
void trackInitRender57();bool trackInitApply57();
FLASHMEM void openTrackResetConfirmation(){trackMenuActive=true;trackResetConfirmActive=true;trackInitMask57=0;trackInitRender57();}
void resetSelectedTrackToDefaults(){trackInitApply57();}

int settingMin(uint8_t page, uint8_t field) {
  if (page == 0) return field == 0 ? 1 : (field == 3 ? 20 : 0);
  if (page == 1) {
    if (field == 0) return 1000;
    if (field == 2) return PICO_DETENT_MIN;
    return field == 3 ? -1 : 0;
  }
  if (page == 2) return field == 3 ? 1 : 0;
  if (page == 3 && field == 1) return 200;
  if (page == 7) return field == 1 ? 250 : (field == 3 ? 1 : 0);
  return 0;
}

int settingMax(uint8_t page, uint8_t field) {
  if (page == 0) return field == 0 ? 20 : (field == 3 ? 1000 : 100);
  if (page == 1) {
    if (field == 0) return 5000;
    if (field == 1) return 80;
    if (field == 2) return PICO_DETENT_MAX;
    return 1;
  }
  if (page == 2) return field == 0 ? 300 : (field < 3 ? 1023 : 200);
  if (page == 3) return field == 1 ? 2000 : (field == 0 ? 1000 : (field == 3 ? 2 : 2));
  if (page == 5) return field == 3 ? 64 : ((field == 0 || field == 2) ? 1 : 0);
  if (page == 7) {
    if (field == 0) return 2;
    if (field == 1) return 1500;
    if (field == 2) return 1;
    return CLIPBOARD_PREVIEW_QUEUE_SIZE;
  }
  return 1;
}

uint16_t settingsChecksum(const PersistedSettings &data) {
  uint16_t checksum = (uint16_t)(data.version ^ 0xA55A);
  for (uint8_t page = 0; page < SETTINGS_PAGE_COUNT; page++) {
    for (uint8_t field = 0; field < 4; field++) {
      uint16_t value = (uint16_t)data.values[page][field];
      checksum = (uint16_t)((checksum << 5) | (checksum >> 11));
      checksum ^= value;
      checksum = (uint16_t)(checksum + 0x31D7);
    }
  }
  return checksum;
}

void applySettingsToRuntime() {
  hc165PollMs = (uint16_t)settingsValue[0][0];
  hc165PressDebounceMs = (uint16_t)settingsValue[0][1];
  hc165ReleaseDebounceMs = (uint16_t)settingsValue[0][2];
  clipboardPreviewMode = (ClipboardPreviewMode)settingsValue[7][0];
  clipboardPasteHoldMs = (uint16_t)settingsValue[7][1];
  ClipboardPasteMode pasteMode = settingsValue[7][2] == 0 ? CLIP_PASTE_ERASE : CLIP_PASTE_OVERDUB;
  for (uint8_t slot = 0; slot < CLIPBOARD_COUNT; slot++) clipboardPasteMode[slot] = pasteMode;
  clipboardPreviewQueueLimit = (uint8_t)settingsValue[7][3];
  multiEditSameValue = settingsValue[5][2] != 0;
  neoSelectionHoldMs = (uint16_t)settingsValue[3][1];
  rollApplyDelaySteps = (uint8_t)settingsValue[5][3];
  transportRepeatWaitTicks = (uint16_t)settingsValue[6][0];
  transportRepeatRateTicks = (uint16_t)settingsValue[6][1];
}

uint16_t seqSettingsChecksum(const int8_t* trackModes, const int8_t* multiModes, const uint8_t* colors) {
  uint16_t checksum = 0x5E21;
  for (uint8_t track = 0; track < TRACK_COUNT; track++) {
    checksum = (uint16_t)((checksum << 3) | (checksum >> 13));
    checksum ^= (uint8_t)(trackModes[track] + 1);
    checksum = (uint16_t)((checksum << 3) | (checksum >> 13));
    checksum ^= (uint8_t)(multiModes[track] + 1);
  }
  for (uint8_t role = 0; role < NEO_COLOR_ROLE_COUNT; role++) {
    checksum = (uint16_t)((checksum << 3) | (checksum >> 13));
    checksum ^= colors[role];
  }
  return checksum;
}

uint16_t ui2SettingsChecksum(const PersistedUi2Settings &data) {
  uint16_t checksum = (uint16_t)(data.version ^ 0xC247);
  checksum ^= data.globalPreview;
  checksum = (uint16_t)((checksum << 3) | (checksum >> 13));
  checksum ^= data.headFollow;
  for (uint8_t track = 0; track < TRACK_COUNT; track++) {
    checksum = (uint16_t)((checksum << 3) | (checksum >> 13));
    checksum ^= (uint8_t)(data.trackPreview[track] + 1);
    checksum = (uint16_t)((checksum << 3) | (checksum >> 13));
    checksum ^= data.trackPlay[track];
  }
  checksum ^= data.royGuard;
  checksum = (uint16_t)((checksum << 5) | (checksum >> 11));
  checksum ^= data.royDebounce;
  checksum ^= (uint16_t)data.royPulses << 8;
  checksum ^= (uint8_t)data.royDirection;
  return checksum;
}

FLASHMEM bool loadSettingsFromEEPROM() {
  bool settingsMigrated = false;
  PersistedSettings stored;
  EEPROM.get(0, stored);
  if (stored.magic != SETTINGS_EEPROM_MAGIC || stored.version != SETTINGS_EEPROM_VERSION) return false;
  if (stored.checksum != settingsChecksum(stored)) return false;

  // Migrate only the multiplying threshold, preserving every other setting.
  if(stored.values[1][2]>=1&&stored.values[1][2]<4){stored.values[1][2]=4;settingsMigrated=true;}

  for (uint8_t page = 0; page < SETTINGS_PAGE_COUNT; page++) {
    for (uint8_t field = 0; field < 4; field++) {
      // La page 5 etait APPLY dans les versions precedentes. Ses anciennes
      // valeurs sont migrees ci-dessous au lieu d'invalider tous les autres
      // reglages deja calibres (notamment les KY40).
      if (page == 5 || page == 6) continue;
      if (stored.values[page][field] < settingMin(page, field) ||
          stored.values[page][field] > settingMax(page, field)) return false;
    }
  }

  memcpy(settingsValue, stored.values, sizeof(settingsValue));
  if (settingsValue[5][0] < 0 || settingsValue[5][0] > 1) settingsValue[5][0] = 0;
  settingsValue[5][1] = 0;
  if (settingsValue[5][2] < 0 || settingsValue[5][2] > 1) settingsValue[5][2] = 0;
  if (settingsValue[5][3] < 0 || settingsValue[5][3] > 64) settingsValue[5][3] = 0;
  if (settingsValue[6][0] < 1 || settingsValue[6][0] > 4096) settingsValue[6][0] = 384;
  if (settingsValue[6][1] < 1 || settingsValue[6][1] > 1536) settingsValue[6][1] = 96;
  if (settingsValue[6][2] < (int)BPM_MIN || settingsValue[6][2] > (int)BPM_MAX) {
    settingsValue[6][2] = 115;
    settingsMigrated = true;
  }
  applySettingsToRuntime();

  PersistedPicoSettings picoStored;
  EEPROM.get(sizeof(PersistedSettings), picoStored);
  uint16_t expectedPicoChecksum = (uint16_t)(picoStored.buttonDebounceMs ^
                                             (picoStored.recoveryMode << 8) ^ 0x6B59);
  if (picoStored.magic == PICO_SETTINGS_EEPROM_MAGIC &&
      picoStored.checksum == expectedPicoChecksum &&
      picoStored.buttonDebounceMs >= 0 && picoStored.buttonDebounceMs <= 120 &&
      picoStored.recoveryMode >= 0 && picoStored.recoveryMode <= 1) {
    picoButtonDebounceMs = picoStored.buttonDebounceMs;
    picoRecoveryMode = picoStored.recoveryMode;
  } else {
    picoButtonDebounceMs = PICO_DEFAULT_B;
    picoRecoveryMode = 1;
  }

  PersistedSeqSettings seqStored;
  size_t seqAddress = sizeof(PersistedSettings) + sizeof(PersistedPicoSettings);
  EEPROM.get(seqAddress, seqStored);
  bool seqValid = seqStored.magic == SEQ_SETTINGS_EEPROM_MAGIC &&
                  seqStored.checksum == seqSettingsChecksum(seqStored.trackAutoSelect, seqStored.trackMultiEdit,
                                                           seqStored.colors);
  if (seqValid) {
    for (uint8_t track = 0; track < TRACK_COUNT; track++) {
      if (seqStored.trackAutoSelect[track] < -1 || seqStored.trackAutoSelect[track] > 1) {
        seqValid = false;
        break;
      }
      if (seqStored.trackMultiEdit[track] < -1 || seqStored.trackMultiEdit[track] > 1) {
        seqValid = false;
        break;
      }
    }
    for (uint8_t role = 0; role < NEO_COLOR_ROLE_COUNT && seqValid; role++) {
      if (seqStored.colors[role] >= NEO_PALETTE_COUNT) seqValid = false;
    }
  }
  if (seqValid) {
    memcpy(trackAutoSelectMode, seqStored.trackAutoSelect, sizeof(trackAutoSelectMode));
    memcpy(trackMultiEditMode, seqStored.trackMultiEdit, sizeof(trackMultiEditMode));
    memcpy(neoColorChoice, seqStored.colors, sizeof(neoColorChoice));
  }

  PersistedUi2Settings ui2Stored;
  size_t ui2Address = seqAddress + sizeof(PersistedSeqSettings);
  EEPROM.get(ui2Address, ui2Stored);
  bool ui2Valid = ui2Stored.magic == UI2_SETTINGS_EEPROM_MAGIC &&
                  ui2Stored.version == UI2_SETTINGS_EEPROM_VERSION &&
                  ui2Stored.checksum == ui2SettingsChecksum(ui2Stored) &&
                  ui2Stored.globalPreview <= 1 && ui2Stored.headFollow <= 1 &&
                  ui2Stored.royGuard >= 50 && ui2Stored.royGuard <= 2000 &&
                  ui2Stored.royDebounce <= 200 &&
                  ui2Stored.royPulses >= 1 && ui2Stored.royPulses <= 8 &&
                  (ui2Stored.royDirection == -1 || ui2Stored.royDirection == 1);
  for (uint8_t track = 0; track < TRACK_COUNT && ui2Valid; track++) {
    if (ui2Stored.trackPreview[track] < -1 || ui2Stored.trackPreview[track] > 1 ||
        ui2Stored.trackPlay[track] > TRACK_PLAY_MULTI) ui2Valid = false;
  }
  if (ui2Valid) {
    globalNotePreview = ui2Stored.globalPreview != 0;
    headFollow = ui2Stored.headFollow != 0;
    memcpy(trackNotePreviewMode, ui2Stored.trackPreview, sizeof(trackNotePreviewMode));
    for (uint8_t track = 0; track < TRACK_COUNT; track++) {
      (pidiRoutesDirty931|=1u<<(track), trackPlayMode[linkOwner12(track,LINK_POLY12)] = (TrackPlayMode)ui2Stored.trackPlay[track]);
    }
    royFunctionGuardMs = ui2Stored.royGuard;
    roySwitchDebounceMs = ui2Stored.royDebounce;
    royPulsesPerValue = ui2Stored.royPulses;
    royRotationDirection = ui2Stored.royDirection;
  }

  PersistedTrackOverrides overrides;
  size_t overrideAddress = ui2Address + sizeof(PersistedUi2Settings);
  EEPROM.get(overrideAddress, overrides);
  bool overridesValid = overrides.magic == TRACK_OVERRIDE_EEPROM_MAGIC &&
                        overrides.checksum == trackOverrideChecksum(overrides) &&
                        overrides.accessMode <= 2 &&
                        overrides.globalViewMode <= NOTE_VIEW_CHROMATIC;
  for (uint8_t track = 0; track < TRACK_COUNT && overridesValid; track++) {
    if (overrides.viewMode[track] < -1 || overrides.viewMode[track] > NOTE_VIEW_CHROMATIC) {
      overridesValid = false;
    }
    for (uint8_t role = 0; role < NEO_COLOR_ROLE_COUNT && overridesValid; role++) {
      if (overrides.colors[track][role] < -1 ||
          overrides.colors[track][role] >= (int8_t)NEO_PALETTE_COUNT) overridesValid = false;
    }
  }
  if (overridesValid) {
    // Anciennes couleurs EEPROM ignorees.
    memcpy(trackNoteViewMode, overrides.viewMode, sizeof(trackNoteViewMode));
    globalNoteViewMode = (NoteViewMode)overrides.globalViewMode;
    settingsAccessMode = overrides.accessMode;
  }
  if (settingsMigrated) saveSettingsToEEPROM();
  return true;
}

FLASHMEM void saveSettingsToEEPROM() {
  PersistedSettings stored;
  stored.magic = SETTINGS_EEPROM_MAGIC;
  stored.version = SETTINGS_EEPROM_VERSION;
  memcpy(stored.values, settingsValue, sizeof(settingsValue));
  stored.checksum = settingsChecksum(stored);

  const uint8_t* bytes = reinterpret_cast<const uint8_t*>(&stored);
  for (size_t address = 0; address < sizeof(stored); address++) EEPROM.update(address, bytes[address]);

  PersistedPicoSettings picoStored;
  picoStored.magic = PICO_SETTINGS_EEPROM_MAGIC;
  picoStored.buttonDebounceMs = (int16_t)clampInt(picoButtonDebounceMs, 0, 120);
  picoStored.recoveryMode = (int16_t)clampInt(picoRecoveryMode, 0, 1);
  picoStored.checksum = (uint16_t)(picoStored.buttonDebounceMs ^
                                   (picoStored.recoveryMode << 8) ^ 0x6B59);
  const uint8_t* picoBytes = reinterpret_cast<const uint8_t*>(&picoStored);
  for (size_t offset = 0; offset < sizeof(picoStored); offset++) {
    EEPROM.update(sizeof(PersistedSettings) + offset, picoBytes[offset]);
  }

  PersistedSeqSettings seqStored;
  seqStored.magic = SEQ_SETTINGS_EEPROM_MAGIC;
  memcpy(seqStored.trackAutoSelect, trackAutoSelectMode, sizeof(trackAutoSelectMode));
  memcpy(seqStored.trackMultiEdit, trackMultiEditMode, sizeof(trackMultiEditMode));
  memcpy(seqStored.colors, neoColorChoice, sizeof(neoColorChoice));
  seqStored.checksum = seqSettingsChecksum(seqStored.trackAutoSelect, seqStored.trackMultiEdit,
                                           seqStored.colors);
  const uint8_t* seqBytes = reinterpret_cast<const uint8_t*>(&seqStored);
  size_t seqAddress = sizeof(PersistedSettings) + sizeof(PersistedPicoSettings);
  for (size_t offset = 0; offset < sizeof(seqStored); offset++) {
    EEPROM.update(seqAddress + offset, seqBytes[offset]);
  }

  PersistedUi2Settings ui2Stored;
  ui2Stored.magic = UI2_SETTINGS_EEPROM_MAGIC;
  ui2Stored.version = UI2_SETTINGS_EEPROM_VERSION;
  ui2Stored.globalPreview = globalNotePreview ? 1 : 0;
  ui2Stored.headFollow = headFollow ? 1 : 0;
  memcpy(ui2Stored.trackPreview, trackNotePreviewMode, sizeof(trackNotePreviewMode));
  for (uint8_t track = 0; track < TRACK_COUNT; track++) {
    ui2Stored.trackPlay[track] = (uint8_t)trackPlayMode[linkOwner12(track,LINK_POLY12)];
  }
  ui2Stored.royGuard = royFunctionGuardMs;
  ui2Stored.royDebounce = roySwitchDebounceMs;
  ui2Stored.royPulses = royPulsesPerValue;
  ui2Stored.royDirection = royRotationDirection;
  ui2Stored.checksum = ui2SettingsChecksum(ui2Stored);
  const uint8_t* ui2Bytes = reinterpret_cast<const uint8_t*>(&ui2Stored);
  size_t ui2Address = seqAddress + sizeof(PersistedSeqSettings);
  for (size_t offset = 0; offset < sizeof(ui2Stored); offset++) {
    EEPROM.update(ui2Address + offset, ui2Bytes[offset]);
  }

  PersistedTrackOverrides overrides;
  overrides.magic = TRACK_OVERRIDE_EEPROM_MAGIC;
  memset(overrides.colors, -1, sizeof(overrides.colors)); // Reserve compatibilite EEPROM.
  memcpy(overrides.viewMode, trackNoteViewMode, sizeof(trackNoteViewMode));
  overrides.globalViewMode = (uint8_t)globalNoteViewMode;
  overrides.accessMode = settingsAccessMode;
  overrides.checksum = trackOverrideChecksum(overrides);
  const uint8_t* overrideBytes = reinterpret_cast<const uint8_t*>(&overrides);
  size_t overrideAddress = ui2Address + sizeof(PersistedUi2Settings);
  for (size_t offset = 0; offset < sizeof(overrides); offset++) {
    EEPROM.update(overrideAddress + offset, overrideBytes[offset]);
  }
}

FLASHMEM bool saveAllSettingsFile(char* path, size_t pathSize) {
  saveSettingsToEEPROM();
  if (!makeUniqueSavePath("CFG", "PARAMS", "CFG", path, pathSize)) return false;
  File file = SD.open(path, FILE_WRITE);
  if (!file) return false;
  file.println(F("URBANKAOZ_SETTINGS_V1"));
  file.println(F("section,page,field,value"));
  for (uint8_t page = 0; page < SETTINGS_PAGE_COUNT; page++) {
    for (uint8_t field = 0; field < 4; field++) {
      file.print(F("GENERAL,")); file.print(page); file.print(',');
      file.print(field); file.print(','); file.println(settingsValue[page][field]);
    }
  }
  file.print(F("PICO,button_debounce,0,")); file.println(picoButtonDebounceMs);
  file.print(F("PICO,recovery,0,")); file.println(picoRecoveryMode);
  for (uint8_t track = 0; track < TRACK_COUNT; track++) {
    file.print(F("TRACK_AUTO,")); file.print(track + 1); file.print(F(",0,"));
    file.println(trackAutoSelectMode[track]);
    file.print(F("TRACK_MULTI,")); file.print(track + 1); file.print(F(",0,"));
    file.println(trackMultiEditMode[track]);
  }
  for (uint8_t role = 0; role < NEO_COLOR_ROLE_COUNT; role++) {
    file.print(F("COLOR,")); file.print(role); file.print(F(",0,"));
    file.println(neoColorChoice[role]);
  }
  file.close();
  return true;
}

int settingIncrement(uint8_t page, uint8_t field) {
  if (page == 0 && field == 3) return 10;
  if (page == 1 && field == 0) return 250;
  if (page == 1 && field == 1) return 1;
  if (page == 2 && (field == 0 || field == 3)) return 5;
  if (page == 3 && field == 0) return 10;
  if (page == 3 && field == 1) return 100;
  if (page == 7 && field == 1) return 50;
  return 1;
}

void formatSettingValue(uint8_t page, uint8_t field, int value, char* out, size_t outSize) {
  if (page == 1 && field == 3) {
    snprintf(out, outSize, "%s", value < 0 ? "INV" : "NORM");
  } else if (page == 5 && field == 0) {
    snprintf(out, outSize, "%s", value ? "ON" : "OFF");
  } else if (page == 5 && field == 1) {
    snprintf(out, outSize, "OPEN");
  } else if (page == 5 && field == 2) {
    snprintf(out, outSize, "%s", value == 0 ? "KEEP" : "SAME");
  } else if (page == 3 && field == 2) {
    snprintf(out, outSize, "%s", value == 0 ? "OFF" : (value == 1 ? "500ms" : "1s"));
  } else if (page == 6) {
    snprintf(out, outSize, "OPEN");
  } else if (page == 7 && field == 0) {
    snprintf(out, outSize, "%s", value == 0 ? "CUT" : (value == 1 ? "ADD" : "QUEUE"));
  } else if (page == 7 && field == 2) {
    snprintf(out, outSize, "%s", value == 0 ? "ERASE" : "OVER");
  } else {
    snprintf(out, outSize, "%d", value);
  }
}

int16_t effectiveKy40Setting(uint8_t field) {
  if (field >= 4) return 0;
  if (settingsOpen && settingsPage == 1 && settingsMenuLevel == SETTINGS_EDIT &&
      settingsItemSelection == field) {
    return settingsDraftValue;
  }
  return settingsValue[1][field];
}

FLASHMEM void sendKy40LiveFieldToPicos(uint8_t field, int16_t value) {
  if (field == 1) {
    int16_t debounceMs = (int16_t)clampInt(value, 0, 80);
    PICO1_SERIAL.print(F("R,")); PICO1_SERIAL.println(debounceMs);
    PICO2_SERIAL.print(F("R,")); PICO2_SERIAL.println(debounceMs);
  } else if (field == 2) {
    int16_t pulses = (int16_t)clampInt(value, PICO_DETENT_MIN, PICO_DETENT_MAX);
    PICO1_SERIAL.print(F("D,")); PICO1_SERIAL.println(pulses);
    PICO2_SERIAL.print(F("D,")); PICO2_SERIAL.println(pulses);
  }
}

int16_t picoSettingValue(uint8_t item) {
  if (item == 0) return settingsValue[1][2]; // D / PULS
  if (item == 1) return settingsValue[1][1]; // R / anti-retour
  if (item == 2) return picoButtonDebounceMs; // B / bouton
  if (item == 3) return picoRecoveryMode; // recuperation par valeur absolue
  return 0;
}

int picoSettingMin(uint8_t item) {
  return item == 0 ? PICO_DETENT_MIN : 0;
}

int picoSettingMax(uint8_t item) {
  if (item == 0) return PICO_DETENT_MAX;
  if (item == 1) return 80;
  if (item == 3) return 1;
  return 120;
}

FLASHMEM void sendPicoSettingLive(uint8_t item, int16_t value) {
  if (item >= 3) return;
  char command = item == 0 ? 'D' : (item == 1 ? 'R' : 'B');
  int16_t limited = (int16_t)clampInt(value, picoSettingMin(item), picoSettingMax(item));
  PICO1_SERIAL.print(command); PICO1_SERIAL.print(','); PICO1_SERIAL.println(limited);
  PICO2_SERIAL.print(command); PICO2_SERIAL.print(','); PICO2_SERIAL.println(limited);
}

void fillCategoryDescription(char* line, const char* description, uint8_t offset) {
  const uint8_t start = 7;
  const uint8_t width = 13;
  size_t length = strlen(description);
  if (length <= width) {
    memcpy(line + start, description, length);
    return;
  }

  size_t cycle = length + 3;
  for (uint8_t i = 0; i < width; i++) {
    size_t pos = (offset + i) % cycle;
    line[start + i] = pos < length ? description[pos] : ' ';
  }
}

void makeCategoryLine(char* line, uint8_t category, bool selected) {
  memset(line, ' ', 20);
  line[20] = '\0';
  line[0] = selected ? '*' : ' ';
  size_t nameLength = strlen(SETTINGS_CATEGORY_NAMES[category]);
  if (nameLength > 6) nameLength = 6;
  memcpy(line + 1, SETTINGS_CATEGORY_NAMES[category], nameLength);
  fillCategoryDescription(line, SETTINGS_CATEGORY_DESCRIPTIONS[category], selected ? settingsMarqueeOffset : 0);
}

void makeSettingItemLine(char* line, uint8_t item) {
  memset(line, ' ', 20);
  line[20] = '\0';
  bool selected = item == settingsItemSelection;
  line[0] = selected ? (settingsMenuLevel == SETTINGS_EDIT ? '#' : '*') : ' ';

  const char* name = SETTINGS_ITEM_NAMES[settingsPage][item];
  size_t nameLength = strlen(name);
  if (nameLength > 14) nameLength = 14;
  memcpy(line + 1, name, nameLength);

  int value = (selected && settingsMenuLevel == SETTINGS_EDIT)
                ? settingsDraftValue
                : settingsValue[settingsPage][item];
  char valueText[8];
  formatSettingValue(settingsPage, item, value, valueText, sizeof(valueText));
  size_t valueLength = strlen(valueText);
  if (valueLength > 5) valueLength = 5;
  memcpy(line + 20 - valueLength, valueText, valueLength);
}

void makeFourFieldLine(char line[21], const char* a, const char* b,
                       const char* c, const char* d) {
  const char* labels[4] = {a, b, c, d};
  memset(line, ' ', 20);
  line[20] = '\0';
  for (uint8_t field = 0; field < 4; field++) {
    if (!labels[field]) continue;
    size_t length = strlen(labels[field]);
    if (length > 4) length = 4;
    memcpy(line + field * 5, labels[field], length);
  }
}

// Les huit boutons ecran sont toujours representes aux memes positions :
// BE1..BE4 sur la premiere ligne, BE5..BE8 sur la derniere.
// Les chevrons n'apparaissent que lorsqu'une autre page de huit existe.
void makeEightButtonLines(char top[21], char bottom[21], const char* labels[8],
                          bool hasPrevious, bool hasNext) {
  makeFourFieldLine(top, labels[0], labels[1], labels[2], labels[3]);
  makeFourFieldLine(bottom, labels[4], labels[5], labels[6], labels[7]);
  if (hasPrevious) {
    memset(top, ' ', 5);
    top[0] = '<';
  }
  if (hasNext) {
    memset(bottom + 15, ' ', 5);
    bottom[19] = '>';
    if (labels[6]) {
      memset(bottom + 10, ' ', 9);
      memcpy(bottom + 10, labels[6], min((size_t)9, strlen(labels[6])));
    }
  }
}

// Eight real items per page. Chevrons only indicate J1 navigation;
// they occupy spare separator cells and never replace a button's item.
uint8_t menuPageStart(uint8_t page) { return page * 8; }
uint8_t menuPageForItem(uint8_t item) { return item / 8; }
int8_t menuPageItem(uint8_t page, uint8_t button, uint8_t count) {
  uint16_t item = menuPageStart(page) + button;
  return button < 8 && item < count ? item : -1;
}
void makePagedMenu(char top[21], char bottom[21], const char* const* items,
                   uint8_t count, uint8_t page) {
  const char* labels[8] = {};
  for (uint8_t button = 0; button < 8; button++) {
    int8_t item = menuPageItem(page, button, count);
    if (item >= 0) labels[button] = items[item];
  }
  makeEightButtonLines(top, bottom, labels, false, false);
  if(page)top[4]='<';
  if(menuPageStart(page)+8<count)bottom[19]='>';
}

const char* categoryNameForSettingsPage(uint8_t page) {
  for (uint8_t category = 0; category < SETTINGS_CATEGORY_COUNT; category++) {
    if (SETTINGS_CATEGORY_PAGE[category] == page) return SETTINGS_CATEGORY_NAMES[category];
  }
  return "MENU";
}

// 13A1 - Cette grande fonction d'interface n'est pas critique en temps reel.
// La laisser en Flash evite de franchir un bloc d'alignement ITCM de 32 Ko
// dans la RAM1, sans consommer davantage de RAM2.
#include "ScreenMapUi.h"
#include "PresentationUi43.h"
FLASHMEM FLASHMEM void sendSettingsPage() {
  if(pinTestActive33){pinTestRender33();return;}
  if(sleepMenu43){sleepRender43();return;}
  if(screenMapMenu){screenMapRender();return;}
 if(oovMenu16){oovRender16();return;}
 if(assignOpen932){assignRender932();return;}
 if(settingsMenuLevel==SETTINGS_COLOR_EDIT){colorsRender932();return;}
  char lines[4][21];
  const char* longLine1 = nullptr;
  const char* longLine3 = nullptr;
  for (uint8_t row = 0; row < 4; row++) {
    memset(lines[row], ' ', 20);
    lines[row][20] = '\0';
  }

  if(settingsMenuLevel==SETTINGS_STABILITY904){renderStability904();return;}
  if (settingsMenuLevel == SETTINGS_CATEGORIES) {
    makePagedMenu(lines[2], lines[3], SETTINGS_MENU_CATEGORY_SHORT,
                  SETTINGS_CATEGORY_COUNT, settingsCategoryPage);
    snprintf(lines[0], sizeof(lines[0]), "%-13s%7s", "SETTINGS", APP_VERSION);
    snprintf(lines[1], sizeof(lines[1]), "SEQUENCER STOPPED");
  } else if (settingsMenuLevel == SETTINGS_MAIN_ITEMS) {
    snprintf(lines[0], sizeof(lines[0]), "MAIN");
    const char* labels[8]={"AUTO",nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr};
    makeEightButtonLines(lines[2],lines[3],labels);
    snprintf(lines[1],sizeof(lines[1]),"Manual save only");
  } else if (settingsMenuLevel == SETTINGS_ROY_EDIT) {
    const char* labels[8] = {"OK", nullptr, nullptr, nullptr,
                             "BACK", nullptr, nullptr, nullptr};
    makeEightButtonLines(lines[2], lines[3], labels);
    snprintf(lines[0], sizeof(lines[0]), "ROY %.10s V%d", ROY_SETTING_SHORT[settingsItemSelection],
             settingsDraftValue);
    if (settingsItemSelection == 3) {
      snprintf(lines[1], sizeof(lines[1]), "R%u T:%ld D:%ld C:%d", debugRoyTestEncoder + 1,
               (long)debugRoyTestValue, (long)debugRoyTestDelta, debugRoyTestClick ? 1 : 0);
    } else {
      snprintf(lines[1], sizeof(lines[1]), "R%u T:%ld D:%ld C:%d", debugRoyTestEncoder + 1,
               (long)debugRoyTestValue, (long)debugRoyTestDelta, debugRoyTestClick ? 1 : 0);
    }
    longLine3 = "KY40 changes value; turn and click every Roy; BE1 OK, BE5 BACK";
  } else if (settingsMenuLevel == SETTINGS_ROY_ITEMS) {
    const char* labels[8] = {ROY_SETTING_SHORT[0], ROY_SETTING_SHORT[1],
                             ROY_SETTING_SHORT[2], ROY_SETTING_SHORT[3],
                             nullptr, nullptr, nullptr, nullptr};
    makeEightButtonLines(lines[2], lines[3], labels);
    snprintf(lines[0], sizeof(lines[0]), "ROY");
    longLine1 = "Adjust Roy encoder response and physical direction";
  } else if (settingsMenuLevel == SETTINGS_SEQ_EXTRA_EDIT) {
    const char* labels[8] = {
      settingsSeqExtraSelection == 0 ? "OFF" : (settingsSeqExtraSelection == 1 ? "FREE" : "COND"),
      settingsSeqExtraSelection == 0 ? "ON" : (settingsSeqExtraSelection == 1 ? "FOLL" : "CHROM"),
      nullptr, nullptr, nullptr, nullptr, nullptr, nullptr
    };
    makeEightButtonLines(lines[2], lines[3], labels);
    if (settingsSeqExtraSelection == 0) {
      snprintf(lines[0], sizeof(lines[0]), "NOTE PREVIEW DURING");
      snprintf(lines[1], sizeof(lines[1]), "SELECTION: %s", settingsSeqExtraDraft ? "ON" : "OFF");
      longLine3 = "Audition notes while changing pitch with no note selected";
    } else if (settingsSeqExtraSelection == 1) {
      snprintf(lines[0], sizeof(lines[0]), "PLAYHEAD VIEW");
      snprintf(lines[1], sizeof(lines[1]), "%s", settingsSeqExtraDraft ? "FOLLOW" : "FREE");
      longLine3 = "Choose whether the view follows the playhead";
    } else {
      snprintf(lines[0], sizeof(lines[0]), "DEFAULT NOTE VIEW");
      snprintf(lines[1], sizeof(lines[1]), "%s", settingsSeqExtraDraft ? "CHROMATIC" : "CONDENSED");
      longLine3 = "Compact shows only used pitches plus insertion rows";
    }
  } else if (settingsMenuLevel == SETTINGS_TRANSPORT_ITEMS) {
    const char* labels[8] = {"WAIT", "RATE", "BPM", "",
                             "BOOT", "SPEC", nullptr, nullptr};
    makeEightButtonLines(lines[2], lines[3], labels);
    snprintf(lines[0], sizeof(lines[0]), "SEQ > TRANSPORT");
    snprintf(lines[1], sizeof(lines[1]), "FW / RW HOLD");
    longLine3 = "Set hold wait and repeat speed in sequencer ticks";
  } else if (settingsMenuLevel == SETTINGS_TRANSPORT_EDIT) {
    const char* labels[8] = {nullptr, nullptr, nullptr, nullptr,
                             nullptr, nullptr, nullptr, nullptr};
    labels[settingsTransportSelection] = "OK";
    makeEightButtonLines(lines[2], lines[3], labels);
    if (settingsTransportSelection == 2) {
      snprintf(lines[0], sizeof(lines[0]), "DEFAULT BPM");
      snprintf(lines[1], sizeof(lines[1]), "%u BPM", settingsTransportDraft);
      longLine3 = "Tempo loaded from EEPROM at every startup";
    } else {
      snprintf(lines[0], sizeof(lines[0]), "FW/RW %s",
               settingsTransportSelection == 0 ? "WAIT" : "SPEED");
      snprintf(lines[1], sizeof(lines[1]), "%u TICKS", settingsTransportDraft);
      longLine3 = settingsTransportSelection == 0
                    ? "Ticks held before measure scrolling starts"
                    : "Ticks between repeated measure moves";
    }
  } else if (settingsMenuLevel == SETTINGS_MIXER_EDIT915) {
    for(uint8_t i=0;i<4;++i)snprintf(lines[i],21,"O%c %-6s %.9s",'A'+i,modeNames902[i],NEO_PALETTE[modeColors902[i]].name);
    sendMenuScreen(lines[0],lines[1],lines[2],lines[3]);settingsPageLastSendMs=millis();return;
  } else if (settingsMenuLevel == SETTINGS_COLOR_EDIT) {
    const char* labels[8] = {nullptr, nullptr, nullptr, nullptr,
                             nullptr, nullptr, nullptr, nullptr};
    labels[settingsColorSelection<7?settingsColorSelection:settingsColorSelection-7] = "OK";
    makeEightButtonLines(lines[2], lines[3], labels);
    snprintf(lines[0], sizeof(lines[0]), "%.20s", colourName902(settingsColorSelection));
    snprintf(lines[1], sizeof(lines[1]), "COLOR: %.13s", NEO_PALETTE[settingsColorDraft].name);
    longLine3 = "Roy1 changes the colour and its click confirms";
  } else if (settingsMenuLevel == SETTINGS_COLOR_ITEMS) {
    const char* labels[8];
    for(uint8_t i=0;i<8;++i)labels[i]=nullptr;
    const char* groups932[]={"Note","Patt","Mixer","TrMix","DlyMx","Head","ModFx","Zones"};
    for(uint8_t i=0;i<8;++i)labels[i]=groups932[i];
    makeEightButtonLines(lines[2], lines[3], labels);
    snprintf(lines[0], sizeof(lines[0]), "Neo colours");

  } else if (settingsMenuLevel == SETTINGS_PICO_EDIT) {
    const char* labels[8] = {nullptr, nullptr, nullptr, nullptr,
                             nullptr, nullptr, nullptr, nullptr};
    labels[settingsItemSelection] = "OK";
    makeEightButtonLines(lines[2], lines[3], labels);
    snprintf(lines[0], sizeof(lines[0]), "PICO %.14s",
             PICO_SETTINGS_NAMES[settingsItemSelection]);
    snprintf(lines[1], sizeof(lines[1]), "LIVE: %d", settingsDraftValue);
    longLine3 = PICO_SETTING_DESCRIPTIONS[settingsItemSelection];
  } else if (settingsMenuLevel == SETTINGS_PICO_ITEMS) {
    const char* labels[8] = {
      PICO_SETTINGS_SHORT[0], PICO_SETTINGS_SHORT[1], PICO_SETTINGS_SHORT[2], PICO_SETTINGS_SHORT[3],
      nullptr, nullptr, nullptr, nullptr
    };
    makeEightButtonLines(lines[2], lines[3], labels);
    snprintf(lines[0], sizeof(lines[0]), "KY40 > PICO");
    longLine1 = "Communication settings shared by both KY40 Pico modules";
  } else if (settingsMenuLevel == SETTINGS_EDIT) {
    const char* labels[8] = {nullptr, nullptr, nullptr, nullptr,
                             nullptr, nullptr, nullptr, nullptr};
    if (settingsPage != 0 || settingsItemSelection == 3) labels[settingsItemSelection] = "OK";
    makeEightButtonLines(lines[2], lines[3], labels);
    if (settingsPage == 5 && settingsItemSelection == 0) {
      snprintf(lines[0], sizeof(lines[0]), "AUTO SELECT LAST");
    } else {
      snprintf(lines[0], sizeof(lines[0]), "%.6s %.13s", categoryNameForSettingsPage(settingsPage),
               SETTINGS_ITEM_NAMES[settingsPage][settingsItemSelection]);
    }
    char valueText[8];
    formatSettingValue(settingsPage, settingsItemSelection, settingsDraftValue,
                       valueText, sizeof(valueText));
    if (settingsPage == 5 && settingsItemSelection == 0) {
      snprintf(lines[1], sizeof(lines[1]), "NOTE: %s", valueText);
    } else {
      snprintf(lines[1], sizeof(lines[1]), settingsPage == 1 ? "LIVE: %s" : "VALUE: %s", valueText);
    }
    longLine3 = (settingsPage == 0 && settingsItemSelection < 3)
                  ? "All HC165 buttons are test inputs; OA turn/click OK, OB click cancels"
                  : SETTINGS_ITEM_DESCRIPTIONS[settingsPage][settingsItemSelection];
  } else {
    if (settingsPage == 6) {
      makePagedMenu(lines[2], lines[3], DEBUG_TEST_SHORT, DEBUG_TEST_COUNT, debugMenuPage);
      snprintf(lines[0], sizeof(lines[0]), "DEBUG");
      snprintf(lines[1], sizeof(lines[1]), "CHOOSE HARDWARE TEST");
    } else {
      const char* labels[8] = {
        SETTINGS_ITEM_SHORT[settingsPage][0], SETTINGS_ITEM_SHORT[settingsPage][1],
        SETTINGS_ITEM_SHORT[settingsPage][2], SETTINGS_ITEM_SHORT[settingsPage][3],
        nullptr, nullptr, nullptr, nullptr
      };
      if(settingsPage==3)labels[4]="OOV";
      if(settingsPage==4){labels[4]="CC time";labels[5]="CC flow";labels[6]="Pidi";labels[7]="PIN";}
      if(settingsPage==3){labels[5]="MAP";labels[6]="SLP";labels[7]="RST";}
      if (settingsPage == 1) labels[7] = "PICO";
      if (settingsPage == 2) labels[4] = "Click";
      if (settingsPage == 5) {
        labels[0] = "AUTO"; labels[1] = "COLR"; labels[2] = "MULT"; labels[3] = "PREV";
        labels[4] = "HEAD"; labels[5] = "VIEW"; labels[6] = "STABLE"; labels[7] = nullptr;
      }
      if(settingsPage==5){const char* items[]={"AUTO","COLR","MULT","PREV","HEAD","VIEW","STABLE","TRANSPORT","CALC","KB","EXP","SCALE","CV"};makePagedMenu(lines[2],lines[3],items,13,seqPage904);}
      else makeEightButtonLines(lines[2], lines[3], labels);
      snprintf(lines[0], sizeof(lines[0]), "%s", categoryNameForSettingsPage(settingsPage));
      longLine1 = SETTINGS_PAGE_DESCRIPTIONS[settingsPage];
    }
  }

  char description905[96];
  size_t valueLength905=strlen(lines[1]);while(valueLength905&&lines[1][valueLength905-1]==' ')lines[1][--valueLength905]=0;
  if(longLine3){snprintf(description905,sizeof(description905),"%s%s%s",lines[1],lines[1][0]&&lines[1][0]!=' '?" - ":"",longLine3);longLine1=description905;}
  sendMenuScreen(lines[0], longLine1 ? longLine1 : lines[1], lines[2], lines[3]);
  settingsPageLastSendMs = millis();
  if (!settingsPageRetrySending) settingsPageRetryPending = true;
}

FLASHMEM void showParaScreen() {
  stopDebugTest();
  settingsAccessMode = 2;
  settingsOpen = true;
  neoDisplayDirty = true;
  settingsMenuLevel = SETTINGS_CATEGORIES;
  settingsCategorySelection = 0;
  settingsCategoryTop = 0;
  settingsCategoryPage = 0;
  debugMenuPage = 0;
  settingsMarqueeOffset = 0;
  settingsMarqueeLastMs = millis();
  settingsValue[0][0] = hc165PollMs;
  settingsValue[0][1] = hc165PressDebounceMs;
  settingsValue[0][2] = hc165ReleaseDebounceMs;
  sendSettingsPage();
}

void startDebugTest(DebugTestMode mode) {
  stopDebugTest();
  settingsPageRetryPending = false;
  debugTestMode = mode;
  settingsOpen = true;
  settingsMenuLevel = SETTINGS_ITEMS;
  settingsPage = 6;

  if(mode==DEBUG_TEST_CV47){cvDebugStart47();return;}
  if (mode == DEBUG_TEST_KY40) {
    resetPicoRxStats();
    sendDebugDisplay("KY40 TEST", "TURN OR CLICK", "K1..K16 DO NOT EDIT",
                     "BE1 BACK", "KY40 TEST", "WAITING");
    Serial.println(F("DEBUG TEST START,KY40"));
  } else if (mode == DEBUG_TEST_NEO) {
    sendDebugDisplay("NEO TEST", "PRESS A PAD", "ITS LED SHOWS INPUT",
                     "BE1 BACK", "NEO TEST", "WAITING PAD");
    Serial.println(F("DEBUG TEST START,NEO"));
    neoDisplayDirty = true;
  } else if (mode == DEBUG_TEST_KY23) {
    sendDebugDisplay("KY23 TARGETS", "J1 X+0 Y+0 CLICK:0", "J2 X+0 Y+0 CLICK:0",
                     "BE1 BACK", "J1 MOVE+CLICK", "J2 MOVE+CLICK");
    debugP23LastReportMs = 0;
    Serial.println(F("DEBUG TEST START,KY23"));
  } else if (mode == DEBUG_TEST_ROY) {
    sendDebugDisplay("ROY TEST", "TURN OR CLICK R1-R4", "VALUE AND CLICK LIVE",
                     "BE1 BACK", "ROY I2C 0x49", "WAITING");
    Serial.println(F("DEBUG TEST START,ROY"));
  } else if (mode == DEBUG_TEST_BE) {
    sendDebugDisplay("BE TEST", "PRESS BE1..BE8", "SHOW HC165 INPUT BIT",
                     "OA CLICK BACK", "BE TEST", "WAITING");
    Serial.println(F("DEBUG TEST START,BE"));
  } else if (mode == DEBUG_TEST_BUTTONS) {
    sendDebugDisplay("BUTTONS TEST", "PRESS ANY BUTTON", "",
                     "", "BUTTONS TEST", "WAITING");
    Serial.println(F("DEBUG TEST START,BUTTONS"));
  } else if (mode == DEBUG_TEST_TRANSPORT) {
    sendDebugDisplay("TRANSPORT TEST", "PRESS ANY TRANSPORT", "NO MUSICAL ACTION",
                     "OA CLICK BACK", "TRANSPORT TEST", "WAITING");
    Serial.println(F("DEBUG TEST START,TRANSPORT"));
  } else if (mode == DEBUG_TEST_CLIPBOARD) {
    sendDebugDisplay("CLIPBOARD TEST", "PRESS ANY CLIP KEY", "NO MUSICAL ACTION",
                     "OA CLICK BACK", "CLIPBOARD TEST", "WAITING");
    Serial.println(F("DEBUG TEST START,CLIPBOARD"));
  } else if (mode == DEBUG_TEST_SCREENS) {
    DISPLAY_SERIAL.println(F("DBGSCREENS"));
    Serial.println(F("DEBUG TEST START,SCREENS"));
  } else if (mode == DEBUG_TEST_KEYPAD) {
    sendDebugDisplay("KEYPAD TEST", "PRESS 0..9 OR A..D", "NO VALUE IS ENTERED",
                     "BE1 BACK", "KEYPAD TEST", "WAITING");
    Serial.println(F("DEBUG TEST START,KEYPAD"));
  }
}

FLASHMEM void openSettingsCategoryByBe(uint8_t beIndex) {
  int8_t mapped = menuPageItem(settingsCategoryPage, beIndex, SETTINGS_CATEGORY_COUNT);
  if (mapped < 0) return;
  uint8_t category = (uint8_t)mapped;
  if (category >= SETTINGS_CATEGORY_COUNT) return;
  settingsCategorySelection = category;
  if(category==10){openStability904(3);return;}
  if(category==11){openStability904(5);return;}
  seqPage904=0;
  if (category == 0) {
    settingsMenuLevel = SETTINGS_MAIN_ITEMS;
    sendSettingsPage();
    return;
  }
  if (category == 1) {
    settingsMenuLevel = SETTINGS_ROY_ITEMS;
    settingsItemSelection = 0;
    sendSettingsPage();
    return;
  }
  settingsPage = SETTINGS_CATEGORY_PAGE[category];
  if (settingsPage == 6) debugMenuPage = 0;
  settingsItemSelection = 0;
  settingsMenuLevel = SETTINGS_ITEMS;
  sendSettingsPage();
}

FLASHMEM void openSettingsItemByBe(uint8_t beIndex) {
 if(settingsPage==0&&beIndex==0){assignStart932();return;}
  if (settingsPage == 6) {
    int8_t testIndex = menuPageItem(debugMenuPage, beIndex, DEBUG_TEST_COUNT);
    if (testIndex >= 0) startDebugTest((DebugTestMode)(DEBUG_TEST_KY40 + testIndex));
    return;
  }
  if(settingsPage==4&&beIndex==7){pinTestStart33();return;}
  if(settingsPage==4&&beIndex==6){delayLabelMenu07=1;delayLabelRender07();return;}
  if(settingsPage==4&&beIndex==4){midi808Open();return;}
  if(settingsPage==4&&beIndex==5){midiConfig810();return;}
  if (beIndex >= 4) return;
  if (settingsPage == 5 && beIndex == 1) {
    settingsColorPage = 0;
    settingsMenuLevel = SETTINGS_COLOR_ITEMS;
    sendSettingsPage();
    return;
  }
  if (settingsPage == 5 && beIndex >= 3) return;
  settingsItemSelection = beIndex;
  settingsDraftValue = settingsValue[settingsPage][settingsItemSelection];
  settingsMenuLevel = SETTINGS_EDIT;
  sendSettingsPage();
}

FLASHMEM void openColorSettingByBe(uint8_t beIndex) {
  if(beIndex==8){memcpy(mixerOriginal915,modeColors902,4);settingsMenuLevel=SETTINGS_MIXER_EDIT915;settingsEditingRoy=-1;sendSettingsPage();return;}
  if (beIndex >= 8) return;
  uint8_t role = beIndex;
  if (role >= 12) return;
  settingsColorSelection = role;
  settingsColorDraft = colourChoice902(role);
  settingsColorOriginal = settingsColorDraft;
  settingsMenuLevel = SETTINGS_COLOR_EDIT;
  sendSettingsPage();
}

void editSelectedColor(int32_t rawDelta) {
  if (settingsMenuLevel != SETTINGS_COLOR_EDIT || rawDelta == 0) return;
  int direction = rawDelta > 0 ? 1 : -1;
  settingsColorDraft = (uint8_t)clampInt((int)settingsColorDraft + direction,
                                         0, NEO_PALETTE_COUNT - 1);
  colourChoice902(settingsColorSelection) = settingsColorDraft;
  neoDisplayDirty = true;
  sendSettingsPage();
}

FLASHMEM void commitSelectedColor() {
  if (settingsMenuLevel != SETTINGS_COLOR_EDIT) return;
  colourChoice902(settingsColorSelection) = settingsColorDraft;
  saveSettingsToEEPROM();save902();
  settingsMenuLevel = SETTINGS_COLOR_ITEMS;
  neoDisplayDirty = true;
  sendSettingsPage();
}

FLASHMEM void openPicoSettingsMenu() {
  settingsItemSelection = 0;
  settingsMenuLevel = SETTINGS_PICO_ITEMS;
  sendSettingsPage();
}

FLASHMEM void openPicoSettingByBe(uint8_t beIndex) {
  if (beIndex >= PICO_SETTINGS_ITEM_COUNT) return;
  settingsItemSelection = beIndex;
  settingsDraftValue = picoSettingValue(beIndex);
  settingsMenuLevel = SETTINGS_PICO_EDIT;
  sendSettingsPage();
}

void editSelectedPicoSetting(int32_t rawDelta) {
  if (settingsMenuLevel != SETTINGS_PICO_EDIT || rawDelta == 0) return;
  int direction = rawDelta > 0 ? 1 : -1;
  int amount = abs((int)rawDelta);
  settingsDraftValue = (int16_t)clampInt((int)settingsDraftValue + direction * amount,
                                        picoSettingMin(settingsItemSelection),
                                        picoSettingMax(settingsItemSelection));
  sendPicoSettingLive(settingsItemSelection, settingsDraftValue);
  sendSettingsPage();
}

FLASHMEM void commitSelectedPicoSetting() {
  if (settingsMenuLevel != SETTINGS_PICO_EDIT) return;
  if (settingsItemSelection == 0) settingsValue[1][2] = settingsDraftValue;
  else if (settingsItemSelection == 1) settingsValue[1][1] = settingsDraftValue;
  else if (settingsItemSelection == 2) picoButtonDebounceMs = settingsDraftValue;
  else if (settingsItemSelection == 3) picoRecoveryMode = settingsDraftValue;
  sendPicoDefaults();
  saveSettingsToEEPROM();
  settingsMenuLevel = SETTINGS_PICO_ITEMS;
  sendSettingsPage();
}

int16_t roySettingValue(uint8_t item) {
  if (item == 0) return royFunctionGuardMs;
  if (item == 1) return roySwitchDebounceMs;
  if (item == 2) return royPulsesPerValue;
  return royRotationDirection;
}

int roySettingMin(uint8_t item) {
  if (item == 0) return 50;
  if (item == 1) return 0;
  if (item == 2) return 1;
  return -1;
}

int roySettingMax(uint8_t item) {
  if (item == 0) return 2000;
  if (item == 1) return 200;
  if (item == 2) return 8;
  return 1;
}

int roySettingIncrement(uint8_t item) {
  if (item == 0) return 25;
  if (item == 1) return 5;
  return 1;
}

FLASHMEM void openRoySettingByBe(uint8_t beIndex) {
  if (beIndex >= 4) return;
  settingsItemSelection = beIndex;
  settingsDraftValue = roySettingValue(beIndex);
  settingsRoyOriginal = settingsDraftValue;
  settingsEditingRoy = -1;
  debugRoyTestValue = 0;
  debugRoyTestEncoder = 0;
  debugRoyTestDelta = 0;
  debugRoyTestClick = false;
  settingsMenuLevel = SETTINGS_ROY_EDIT;
  sendSettingsPage();
}

void applyRoyDraftToRuntime() {
  if (settingsItemSelection == 0) royFunctionGuardMs = (uint16_t)settingsDraftValue;
  else if (settingsItemSelection == 1) roySwitchDebounceMs = (uint16_t)settingsDraftValue;
  else if (settingsItemSelection == 2) royPulsesPerValue = (uint8_t)settingsDraftValue;
  else royRotationDirection = settingsDraftValue < 0 ? -1 : 1;
}

void editRoySettingFromKy40(int32_t delta) {
  if (settingsMenuLevel != SETTINGS_ROY_EDIT || delta == 0) return;
  int direction = delta > 0 ? 1 : -1;
  int amount = abs((int)delta) * roySettingIncrement(settingsItemSelection);
  if (settingsItemSelection == 3) settingsDraftValue = direction > 0 ? 1 : -1;
  else settingsDraftValue = (int16_t)clampInt((int)settingsDraftValue + direction * amount,
                                               roySettingMin(settingsItemSelection),
                                               roySettingMax(settingsItemSelection));
  applyRoyDraftToRuntime();
  sendSettingsPage();
}

void editSelectedRoySetting(int32_t delta, uint8_t logicalRoy) {
  if (settingsMenuLevel != SETTINGS_ROY_EDIT || delta == 0) return;
  settingsEditingRoy = logicalRoy;
  int amount = abs((int)delta) * roySettingIncrement(settingsItemSelection);
  int direction = delta > 0 ? 1 : -1;
  if (settingsItemSelection == 3) {
    settingsDraftValue = settingsDraftValue < 0 ? 1 : -1;
  } else {
    settingsDraftValue = (int16_t)clampInt((int)settingsDraftValue + direction * amount,
                                           roySettingMin(settingsItemSelection),
                                           roySettingMax(settingsItemSelection));
  }
  sendSettingsPage();
}

FLASHMEM void commitSelectedRoySetting() {
  if (settingsMenuLevel != SETTINGS_ROY_EDIT) return;
  applyRoyDraftToRuntime();
  saveSettingsToEEPROM();
  settingsEditingRoy = -1;
  settingsMenuLevel = SETTINGS_ROY_ITEMS;
  sendSettingsPage();
}

FLASHMEM void openSeqExtraSetting(uint8_t selection) {
  settingsSeqExtraSelection = min(selection, (uint8_t)2);
  if (settingsSeqExtraSelection == 0) settingsSeqExtraDraft = globalNotePreview ? 1 : 0;
  else if (settingsSeqExtraSelection == 1) settingsSeqExtraDraft = headFollow ? 1 : 0;
  else settingsSeqExtraDraft = globalNoteViewMode == NOTE_VIEW_CHROMATIC ? 1 : 0;
  settingsEditingRoy = -1;
  settingsMenuLevel = SETTINGS_SEQ_EXTRA_EDIT;
  sendSettingsPage();
}

void editSelectedSeqExtraSetting(int32_t delta, uint8_t logicalRoy) {
  if (settingsMenuLevel != SETTINGS_SEQ_EXTRA_EDIT || delta == 0 || (settingsSeqExtraSelection==1&&logicalRoy!=0)) return;
  settingsEditingRoy = logicalRoy;
  settingsSeqExtraDraft = delta > 0 ? 1 : 0;
  sendSettingsPage();
}

FLASHMEM void commitSelectedSeqExtraSetting() {
  if (settingsMenuLevel != SETTINGS_SEQ_EXTRA_EDIT) return;
  if (settingsSeqExtraSelection == 0) globalNotePreview = settingsSeqExtraDraft != 0;
  else if (settingsSeqExtraSelection == 1) headFollow = settingsSeqExtraDraft != 0;
  else {
    globalNoteViewMode = settingsSeqExtraDraft ? NOTE_VIEW_CHROMATIC : NOTE_VIEW_COMPACT;
    compactRowOffset = 0;compactRecenter08=true;
    neoDisplayDirty = true;
  }
  saveSettingsToEEPROM();
  settingsEditingRoy = -1;
  settingsMenuLevel = SETTINGS_ITEMS;
  settingsPage = 5;
  sendSettingsPage();
}

FLASHMEM void openTransportSettings() {
  settingsTransportSelection = 0;
  settingsEditingRoy = -1;
  settingsMenuLevel = SETTINGS_TRANSPORT_ITEMS;
  sendSettingsPage();
}

FLASHMEM void openTransportSetting(uint8_t selection) {
  if (selection > 2) return;
  settingsTransportSelection = selection;
  settingsTransportDraft = (uint16_t)settingsValue[6][selection];
  settingsEditingRoy = -1;
  settingsMenuLevel = SETTINGS_TRANSPORT_EDIT;
  sendSettingsPage();
}

void editTransportSetting(int32_t delta, uint8_t logicalRoy) {
  if (settingsMenuLevel != SETTINGS_TRANSPORT_EDIT || delta == 0) return;
  settingsEditingRoy = logicalRoy;
  int increment = settingsTransportSelection == 0 ? 24
                : settingsTransportSelection == 1 ? 12 : 1;
  int minimum = settingsTransportSelection == 2 ? (int)BPM_MIN : 1;
  int maximum = settingsTransportSelection == 0 ? 4096
                : settingsTransportSelection == 1 ? 1536 : (int)BPM_MAX;
  int next = (int)settingsTransportDraft + (delta > 0 ? increment : -increment) * abs((int)delta);
  settingsTransportDraft = (uint16_t)clampInt(next, minimum, maximum);
  sendSettingsPage();
}

FLASHMEM void commitTransportSetting() {
  if (settingsMenuLevel != SETTINGS_TRANSPORT_EDIT) return;
  settingsValue[6][settingsTransportSelection] = settingsTransportDraft;
  transportRepeatWaitTicks = (uint16_t)settingsValue[6][0];
  transportRepeatRateTicks = (uint16_t)settingsValue[6][1];
  if (settingsTransportSelection == 2) {
    bpm = (float)settingsValue[6][2];
    updateInternalClockPeriod();
    displayBpmDirty = true;
  }
  saveSettingsToEEPROM();
  settingsEditingRoy = -1;
  settingsMenuLevel = SETTINGS_TRANSPORT_ITEMS;
  sendSettingsPage();
}

void moveSettingsSelection(int direction) {
  if (settingsMenuLevel == SETTINGS_CATEGORIES) {
    int next = clampInt((int)settingsCategorySelection + direction, 0, SETTINGS_CATEGORY_COUNT - 1);
    if (next == settingsCategorySelection) return;
    settingsCategorySelection = (uint8_t)next;
    settingsCategoryPage = menuPageForItem(settingsCategorySelection);
    if (settingsCategorySelection < settingsCategoryTop) settingsCategoryTop = settingsCategorySelection;
    if (settingsCategorySelection >= settingsCategoryTop + 4) settingsCategoryTop = settingsCategorySelection - 3;
    settingsMarqueeOffset = 0;
    settingsMarqueeLastMs = millis();
  } else {
    // Haut/bas abandonne un brouillon eventuel et change de ligne.
    if (settingsMenuLevel == SETTINGS_EDIT) settingsMenuLevel = SETTINGS_ITEMS;
    settingsItemSelection = (uint8_t)clampInt((int)settingsItemSelection + direction, 0,
                                             settingsPage == 6 ? DEBUG_TEST_COUNT - 1 : 3);
    if (settingsPage == 6) debugMenuPage = menuPageForItem(settingsItemSelection);
  }
  sendSettingsPage();
}

void editSelectedSetting(int32_t rawDelta) {
  if (rawDelta == 0) return;
  if (settingsMenuLevel == SETTINGS_ITEMS) {
    // Aucun appui EDITER necessaire : le premier mouvement du Roy ouvre
    // directement le brouillon de la ligne selectionnee.
    settingsDraftValue = settingsValue[settingsPage][settingsItemSelection];
    settingsMenuLevel = SETTINGS_EDIT;
  }
  if (settingsMenuLevel != SETTINGS_EDIT) return;
  int direction = rawDelta > 0 ? 1 : -1;
  int amount = abs((int)rawDelta) * settingIncrement(settingsPage, settingsItemSelection);
  settingsDraftValue = (int16_t)clampInt((int)settingsDraftValue + direction * amount,
                                        settingMin(settingsPage, settingsItemSelection),
                                        settingMax(settingsPage, settingsItemSelection));
  if (settingsPage == 0) {
    if (settingsItemSelection == 0) hc165PollMs = (uint16_t)settingsDraftValue;
    else if (settingsItemSelection == 1) hc165PressDebounceMs = (uint16_t)settingsDraftValue;
    else if (settingsItemSelection == 2) hc165ReleaseDebounceMs = (uint16_t)settingsDraftValue;
  } else if (settingsPage == 1) {
    // DBNC et PULS vivent dans les Pico. MIN et DIR sont lus directement
    // depuis le brouillon par le Teensy pendant toute l'edition.
    sendKy40LiveFieldToPicos(settingsItemSelection, settingsDraftValue);
  }
  sendSettingsPage();
}

FLASHMEM void commitSelectedSetting() {
  if (settingsMenuLevel != SETTINGS_EDIT) return;
  uint8_t item = settingsItemSelection;
  settingsValue[settingsPage][item] = settingsDraftValue;

  if (settingsPage == 0) {
    if (item == 0) hc165PollMs = (uint16_t)settingsDraftValue;
    else if (item == 1) hc165PressDebounceMs = (uint16_t)settingsDraftValue;
    else if (item == 2) hc165ReleaseDebounceMs = (uint16_t)settingsDraftValue;
    else if (item == 3) {
      DISPLAY_SERIAL.print(F("CFG,DLY,"));
      DISPLAY_SERIAL.println(settingsDraftValue);
    }
  } else if (settingsPage == 5 && item == 2) {
    multiEditSameValue = settingsDraftValue != 0;
  } else if (settingsPage == 5 && item == 3) {
    rollApplyDelaySteps = (uint8_t)settingsDraftValue;
  } else if (settingsPage == 3 && item == 1) {
    neoSelectionHoldMs = (uint16_t)settingsDraftValue;
  } else if (settingsPage == 7) {
    if (item == 0) clipboardPreviewMode = (ClipboardPreviewMode)settingsDraftValue;
    else if (item == 1) clipboardPasteHoldMs = (uint16_t)settingsDraftValue;
    else if (item == 2) {
      ClipboardPasteMode mode = settingsDraftValue == 0 ? CLIP_PASTE_ERASE : CLIP_PASTE_OVERDUB;
      for (uint8_t slot = 0; slot < CLIPBOARD_COUNT; slot++) clipboardPasteMode[slot] = mode;
    } else if (item == 3) {
      clipboardPreviewQueueLimit = (uint8_t)settingsDraftValue;
    }
  } else if (settingsPage == 1) {
    // Un seul jeu de reglages KY40 pilote les 16 encodeurs des deux Pico.
    sendPicoDefaults();
  }

  saveSettingsToEEPROM();

  settingsMenuLevel = SETTINGS_ITEMS;
  sendSettingsPage();
}

FLASHMEM void openOrCommitSettingsSelection() {
  if (settingsMenuLevel == SETTINGS_CATEGORIES) {
    if (settingsCategorySelection == 0) {
      settingsMenuLevel = SETTINGS_MAIN_ITEMS;
      sendSettingsPage();
      return;
    }
    if (settingsCategorySelection == 1) {
      settingsMenuLevel = SETTINGS_ROY_ITEMS;
      settingsItemSelection = 0;
      sendSettingsPage();
      return;
    }
    settingsPage = SETTINGS_CATEGORY_PAGE[settingsCategorySelection];
    settingsItemSelection = 0;
    settingsMenuLevel = SETTINGS_ITEMS;
  } else if (settingsMenuLevel == SETTINGS_ITEMS) {
    if (settingsPage == 6) {
      startDebugTest((DebugTestMode)(DEBUG_TEST_KY40 + settingsItemSelection));
      return;
    }
    settingsDraftValue = settingsValue[settingsPage][settingsItemSelection];
    settingsMenuLevel = SETTINGS_EDIT;
  } else {
    commitSelectedSetting();
    return;
  }
  sendSettingsPage();
}

void backSettingsMenu() {
  if(pinTestActive33){pinTestStop33();return;}
  if(sleepMenu43){sleepMenu43=false;sendSettingsPage();return;}
  if(screenMapMenu){screenMapClose(false);return;}
  if(oovMenu16){oovClose16(false);return;}
  if(settingsMenuLevel==SETTINGS_STABILITY904){backStability904();return;}
  if (settingsMenuLevel == SETTINGS_MAIN_ITEMS) {
    settingsMenuLevel = SETTINGS_CATEGORIES;
    settingsCategoryPage = menuPageForItem(settingsCategorySelection);
    sendSettingsPage();
  } else if (settingsMenuLevel == SETTINGS_ROY_EDIT) {
    settingsDraftValue = settingsRoyOriginal;
    applyRoyDraftToRuntime();
    settingsEditingRoy = -1;
    settingsMenuLevel = SETTINGS_ROY_ITEMS;
    sendSettingsPage();
  } else if (settingsMenuLevel == SETTINGS_ROY_ITEMS) {
    settingsMenuLevel = SETTINGS_CATEGORIES;
    settingsCategoryPage = menuPageForItem(settingsCategorySelection);
    sendSettingsPage();
  } else if (settingsMenuLevel == SETTINGS_SEQ_EXTRA_EDIT) {
    settingsEditingRoy = -1;
    settingsMenuLevel = SETTINGS_ITEMS;
    settingsPage = 5;
    sendSettingsPage();
  } else if (settingsMenuLevel == SETTINGS_TRANSPORT_EDIT) {
    settingsEditingRoy = -1;
    settingsMenuLevel = SETTINGS_TRANSPORT_ITEMS;
    sendSettingsPage();
  } else if (settingsMenuLevel == SETTINGS_TRANSPORT_ITEMS) {
    settingsMenuLevel = SETTINGS_ITEMS;
    settingsPage = 5;
    sendSettingsPage();
  } else if(settingsMenuLevel==SETTINGS_MIXER_EDIT915){
    memcpy(modeColors902,mixerOriginal915,4);settingsMenuLevel=SETTINGS_COLOR_ITEMS;neoDisplayDirty=true;sendSettingsPage();
  } else if (settingsMenuLevel == SETTINGS_COLOR_EDIT) {
    colorsCancel932();
  } else if (settingsMenuLevel == SETTINGS_COLOR_ITEMS) {
    if(settingsColorPage){settingsColorPage=0;sendSettingsPage();return;}
    settingsMenuLevel = SETTINGS_ITEMS;
    settingsPage = 5;
    settingsItemSelection = 1;
    sendSettingsPage();
  } else if (settingsMenuLevel == SETTINGS_PICO_EDIT) {
    sendPicoSettingLive(settingsItemSelection, picoSettingValue(settingsItemSelection));
    settingsMenuLevel = SETTINGS_PICO_ITEMS;
    sendSettingsPage();
  } else if (settingsMenuLevel == SETTINGS_PICO_ITEMS) {
    settingsMenuLevel = SETTINGS_ITEMS;
    settingsPage = 1;
    sendSettingsPage();
  } else if (settingsMenuLevel == SETTINGS_EDIT) {
    if (settingsPage == 0) {
      hc165PollMs = (uint16_t)settingsValue[0][0];
      hc165PressDebounceMs = (uint16_t)settingsValue[0][1];
      hc165ReleaseDebounceMs = (uint16_t)settingsValue[0][2];
    } else if (settingsPage == 1) {
      // Annuler restaure aussi immediatement la configuration des deux Pico.
      sendKy40LiveFieldToPicos(settingsItemSelection,
                              settingsValue[settingsPage][settingsItemSelection]);
    }
    settingsMenuLevel = SETTINGS_ITEMS; // annulation du brouillon
    sendSettingsPage();
  } else if (settingsMenuLevel == SETTINGS_ITEMS) {
    settingsMenuLevel = SETTINGS_CATEGORIES;
    settingsCategoryPage = menuPageForItem(settingsCategorySelection);
    settingsMarqueeOffset = 0;
    settingsMarqueeLastMs = millis();
    sendSettingsPage();
  } else {
    showGameScreen();
  }
}

void serviceMenuScroll905(){
 if(menuScrollLine>=4||(int32_t)(millis()-menuScrollAt905)<0)return;
 size_t end=strlen(menuScreenRaw[menuScrollLine])-20;
 if(menuScrollOffset>=end){menuScrollOffset=0;menuScrollAt905=millis()+1200;}
 else {++menuScrollOffset;menuScrollAt905=millis()+(menuScrollOffset==end?1500:250);}
 renderMenuScreenRaw();
}
void serviceSettingsMarquee() {
  if (!settingsOpen || settingsMenuLevel != SETTINGS_CATEGORIES) return;
  uint32_t now = millis();
  if ((uint32_t)(now - settingsMarqueeLastMs) < 500) return;
  settingsMarqueeLastMs = now;
  settingsMarqueeOffset++;
  sendSettingsPage();
}

void serviceSettingsPageRetry() {settingsPageRetryPending=false;return;
  if (!settingsOpen || debugTestMode != DEBUG_TEST_NONE) {
    settingsPageRetryPending = false;
    return;
  }
  if (!settingsPageRetryPending) return;
  if ((uint32_t)(millis() - settingsPageLastSendMs) < 60) return;
  settingsPageRetryPending = false;
  settingsPageRetrySending = true;
  sendSettingsPage();
  settingsPageRetrySending = false;
  settingsPageRetryPending = false;
}

int8_t screenButtonIndexForBit(uint8_t bit) {
  for (uint8_t i = 0; i < 8; i++) {
    if (SCREEN_BUTTON_BITS[i] == bit) return (int8_t)i;
  }
  return -1;
}

void handleFileBrowserScreenButton(uint8_t index) {
  if(saveCollision905){saveCollisionButton905(index);return;}
  if (patternLoadConfirmActive) {
    if(newProjectConfirm807){if(index==2||index==3)answerPatternLoadConfirmation(true);return;}
    if (index == 1) answerPatternLoadConfirmation(true);

    return;
  }
  if(index>=4)return;
  if (fileBrowserMode == FILE_BROWSER_SAVE) {
    if (index == 0) executeBrowserSave();
    else if (index == 1 && !browserAtTypeRoot()) browserParentDirectory();
    else if (index == 2) browserCreateDirectory();
    else if (index >= 3) browserEnterButtonDirectory(index - 3);
  } else if (fileBrowserMode == FILE_BROWSER_LOAD) {
    if (index == 0) {
      if(itemKind809){itemLoadFile809();}
      else if (activeStorageType == STORAGE_FULL || activeStorageType == STORAGE_PATTERN) {
        openPatternLoadConfirmation();
      }
      else DISPLAY_SERIAL.println(F("MSG|CHOOSE C1 C2 C3 C4"));
    } else if (index == 1 && !browserAtTypeRoot()) {
      browserParentDirectory();
    } else if (index == 2) {
      bool ok = !itemKind809 && activeStorageType != STORAGE_PATTERN && previewSelectedLoadFile(selectedTrack);
      if (!ok) DISPLAY_SERIAL.println(F("MSG|INDISPONIBLE"));
    } else if (index >= 3) {
      browserEnterButtonDirectory(index - 3);
    }
  }
  if (fileBrowserMode != FILE_BROWSER_NONE && !patternLoadConfirmActive) renderFileBrowser();
}

void reportDebugBe(uint8_t index) {
  char line0[21], line1[21], small0[17], small1[17];
  snprintf(line0, sizeof(line0), "BE TEST BE%u", index + 1);
  snprintf(line1, sizeof(line1), "HC165 BIT%u", SCREEN_BUTTON_BITS[index]);
  snprintf(small0, sizeof(small0), "BE%u APPUI", index + 1);
  snprintf(small1, sizeof(small1), "HC165 B%u", SCREEN_BUTTON_BITS[index]);
  sendDebugDisplay(line0, line1, "ETAT PRESSE", "", small0, small1);
  Serial.print(F("DBG,BE,INDEX=")); Serial.print(index + 1);
  Serial.print(F(",HC165_BIT=")); Serial.println(SCREEN_BUTTON_BITS[index]);
}

FLASHMEM const char* buttonAssignment917(uint8_t bit){
 static const char* names[56]={
 "SELECT","SHIFT RECTANGLE","REDO","GROOVE","REMIX","DELETE","SELECT ALL","COLUMN",
 "UNDO","SONG","TRACK / PATTERN","KEYBOARD","SAVE / OBJECT SAVE","LOAD / OBJECT LOAD","DELAY MIXER","LEGACY PATCH +",
 "BE5 (CONTEXT)","BE7 (CONTEXT)","BE8 (CONTEXT)","BE6 (CONTEXT)","BE2 (CONTEXT)","BE4 (CONTEXT)","BE3 (CONTEXT)","BE1 (CONTEXT)",
 "PLAY / PAUSE","PREVIOUS BAR","RETURN TO START","RECORD","NEXT BAR","STOP","NOTES CUT/KEEP","DELAY CUT/KEEP",
 "UNASSIGNED","UNASSIGNED","UNASSIGNED","UNASSIGNED","CLIP3 COPY","CLIP3 SELECT","CLIP3 PASTE","CLIP3 HEAR",
 "CLIP2 COPY","CLIP2 SELECT","CLIP2 PASTE","CLIP2 HEAR","CLIP1 COPY","CLIP1 SELECT","CLIP1 PASTE","CLIP1 HEAR",
 "QV LFO","QV DELAY","QV NOTES","QV PATCH","TRACK MIXER","PATTERN LINK","QV ENV","QV ROLL"};
 if(bit>=56)return bit<64?functionName932(bit):"UNASSIGNED";
 if(remixMode&&(bit==1||bit==6||bit==7))return "INACTIVE IN REMIX";
 if(remixMode&&remixHit916==rmx916::Chain&&(bit==0||bit==5))return "INACTIVE IN CHN";
 if(bit==15)return stableQuick904||remixMode?"INACTIVE (QUICK)":names[bit];
 if(bit==53)return "LINK (PATTERN+SEL)";
 if(bit>=48&&bit!=52&&!stableQuick904&&!remixMode)return "INACTIVE (QUICK OFF)";
 if((bit==30||bit==31)&&!stableTransport904)return "INACTIVE (SPEC OFF)";
 return names[bit];
}
FLASHMEM void buttonScan917(uint8_t bit){
 char line[21];snprintf(line,sizeof(line),"BIT B%u",bit);
 sendMenuScreen(line,buttonAssignment917(bit),"","");settingsPageRetryPending=false;
}
FLASHMEM void reportDebugButton(uint8_t bit, bool pressed) {
  if(pressed)buttonScan917(bit);
  Serial.print(F("DBG,BUTTONS,HC165_BIT=")); Serial.print(bit);
  Serial.print(F(",PRESSED=")); Serial.println(pressed ? 1 : 0);
}

bool buttonSettingsLiveTest() {
  return settingsOpen && settingsMenuLevel == SETTINGS_EDIT &&
         settingsPage == 0 && settingsItemSelection < 3;
}

void reportLiveButtonSetting(uint8_t bit, bool pressed) {
  if(pressed)buttonScan917(bit);
}

FLASHMEM void handleScreenButtonPress(uint8_t bit) { if(pinTestActive33)return; alertAction08(); EpRefreshR10 refresh;
 if(assignOpen932){for(uint8_t i=0;i<8;++i)if(SCREEN_BUTTON_BITS[i]==bit){if(i>=4&&assignConflict932>=0){assignReplace60=i>=6;assignRender932();}return;}return;}
  if (deleteButtonActive) return;
  int8_t index = screenButtonIndexForBit(bit);
  if (index < 0) return;
  if(debugTestMode==DEBUG_TEST_CV47){cvDebugButton47(index);return;}
  if(patMenu46&&!patchUi808){patButton46(index);return;}
  recordExitA01();
  if(delayLabelMenu07){if(delayLabelMenu07==1&&index==4){delayLabelMenu07=2;delayLabelRender07();}return;}
  if(kbScreenR10(index))return;
  DISPLAY_SERIAL.print("PRESS,");DISPLAY_SERIAL.print(index);DISPLAY_SERIAL.print(',');DISPLAY_SERIAL.println((unsigned long)max(1.0f,tickDurationMs()*24));
  if (itemScreen809((uint8_t)index) || patch808Screen((uint8_t)index)) return;
  if (mod805Screen((uint8_t)index)) return;
  if (screenUiAwaitRelease) return;
  if(remixScreen(index))return;
  if (!menuNavigationOnly()) discardNoteRedo();

  if (barDeletePromptActive || patternModeEditActive) return;

  if (barFillPromptActive) {
    if (index == 0 || index == 1) {

      bool fill = index == 1;
      uint16_t oldSteps = pendingOldBars * STEPS_PER_BAR;
      uint16_t newSteps = pendingNewBars * STEPS_PER_BAR;
      uint8_t track = selectedTrack;
      uint16_t fineNeeded09=0,fineFree09=0,locksNeeded09=0,oldLocks09=lockCount817;
      for(auto &event:fineNote[track]){if(!event.active)++fineFree09;else if(fill&&event.step<oldSteps)for(uint16_t at=event.step+oldSteps;at<newSteps;at+=oldSteps)++fineNeeded09;}
      if(fill)for(uint16_t i=0;i<oldLocks09;++i){auto &l=noteLocks817[i];if(l.track==track&&l.pattern==activePattern[track]&&l.tick>=0&&l.tick<(int32_t)oldSteps*96)for(int32_t at=l.tick+(int32_t)oldSteps*96;at<(int32_t)newSteps*96;at+=(int32_t)oldSteps*96)++locksNeeded09;}
      if(fineNeeded09>fineFree09||lockCount817+locksNeeded09>LOCK_CAPACITY817){DISPLAY_SERIAL.println("MSG|EXTENSION CAPACITY");return;}
      for (uint16_t step = oldSteps; step < newSteps; step++) {
        uint16_t source = fill ? (uint16_t)(step % oldSteps) : step;
        if (fill) {
          copyTrackStep(track, step, source);
          stepSelected[track][step] = false;
        } else {
          clearTrackStep(track, step);
        }
      }
      if(fill){
        for(uint16_t i=0;i<FINE_EVENTS_PER_TRACK;++i){auto source=fineNote[track][i];if(!source.active||source.step>=oldSteps)continue;
          for(uint16_t at=source.step+oldSteps;at<newSteps;at+=oldSteps){int16_t slot=addFineNote(track,at,source.micro,source.note,source.logicalMicro);if(slot>=0){fineNote[track][slot]=source;fineNote[track][slot].step=at;fineLastTriggeredCycle[track][slot]=UINT32_MAX;}}
        }
        for(uint16_t i=0;i<oldLocks09;++i){auto source=noteLocks817[i];if(source.track!=track||source.pattern!=activePattern[track]||source.tick<0||source.tick>=(int32_t)oldSteps*96)continue;
          for(int32_t at=source.tick+(int32_t)oldSteps*96;at<(int32_t)newSteps*96;at+=(int32_t)oldSteps*96){auto copy=source;copy.tick=at;noteLocks817[lockCount817++]=copy;}
        }
      }
      mod805Invalidate();
      trackBars[track] = pendingNewBars;
      sequenceBars = pendingNewBars;
      loopEndStep = max(loopEndStep, longestTrackStepCount());
      syncLegacyTrackLoopBounds();
      beatDraftBars = pendingNewBars;
      beatEditLength = false;
      barFillPromptActive = false;
      pendingViewSource = VIEW_SRC_LENGTH;
      showGameScreen();
      neoDisplayDirty = true;
      displayBpmDirty = true;
    }
    return;
  }


  if(debugTestMode==DEBUG_TEST_CV47){cvDebugButton47(index);return;}
  // Le peripherique teste ne pilote jamais sa propre sortie. Pour KY40,
  // Neo, KY23, Roy et Keypad, BE1 est une sortie alternative explicite.
  if (debugTestMode == DEBUG_TEST_KY40 || debugTestMode == DEBUG_TEST_NEO ||
      debugTestMode == DEBUG_TEST_KY23 || debugTestMode == DEBUG_TEST_ROY ||
      debugTestMode == DEBUG_TEST_KEYPAD) {
    if (index == 0) {
      stopDebugTest();
      sendSettingsPage();
    }
    return;
  }

  if (debugTestMode == DEBUG_TEST_BE) {
    reportDebugBe((uint8_t)index);
    return;
  }

  if (writeMenuActive) {
    if(index>=4&&index<7){DISPLAY_SERIAL.println("MSG|WRITE ENV LFO PLX LATER");return;}
    if(index<7){writeMask41^=1u<<index;writeTimeEnabled=writeMask41&1;writeRollEnabled=writeMask41&4;sendWriteMenu();}return;
  }

  if (fileBrowserMode != FILE_BROWSER_NONE) {
    handleFileBrowserScreenButton((uint8_t)index);
    return;
  }

  if(sdMenu42){if(index==0)sdAction42();return;}
  if (loadTypeMenuActive) {
    if(index==7){sdOpen42(false);return;}
    if(index==5){openNewProject807();return;}
    if(index==6)return;
    if(index==4){songLoadWorkspace();return;}
    if(index==1){openStorageLoadBrowser(STORAGE_NOTE);return;}
    if (index < 7) openStorageLoadBrowser((StorageContentType)index);
    return;
  }

  if (saveMenuActive) {
    if (saveMenuClipboardMode) {
      if (index < CLIPBOARD_COUNT) {
        if (!clipKinds18[index]) { sendSaveMenu(); sendClipboardMessage(index, CLIP_TYPE_NONE, "EMPTY"); }
        else openSaveBrowser(SAVE_CANDIDATE_SEQUENCE, true, index);
      }
    } else if (index == STORAGE_FULL) {
      openSaveBrowser(SAVE_CANDIDATE_FULL, false, 0);
    } else if (index == 1) {
      clipMenuEnter18();
      sendSaveMenu();
    } else if(index==5){songSaveWorkspace();return;
    } else if (index == STORAGE_PATTERN) {
      openSaveBrowser(SAVE_CANDIDATE_PATTERN, false, 0);
    } else if (index == STORAGE_SEQUENCE) {
      openSaveBrowser(SAVE_CANDIDATE_SEQUENCE, false, 0);
    } else if (index == STORAGE_NOTE) {
      openSaveBrowser(saveMenuCandidates[0], false, 0);
    } else if(index==7){sdOpen42(true);return;
    } else if (index == 6) {
      openSaveBrowser(SAVE_CANDIDATE_SETTINGS, false, 0);
    } else {
      DISPLAY_SERIAL.println(F("MSG|INDISPONIBLE"));
      sendSaveMenu();
    }
    return;
  }

  if (trackDrumWarningActive) {
    if (index == 1) applyTrackPlayModeConversion();
    else if (index == 2 || index == 3) beginSaveBeforeTrackModeConversion();
    else if (index == 0 || index >= 4) sendTrackPlayMenu();
    return;
  }

  if (trackGateLengthPromptActive) {
    if (index == 1) applyTrackPlayModeConversion();
    else if (index == 2 || index == 3) beginSaveBeforeTrackModeConversion();
    else if (index == 0 || index >= 4) sendTrackPlayMenu();
    return;
  }

  if(trackResetConfirmActive){if(index<8){trackInitMask57^=1u<<index;trackInitRender57();}return;}

  if (trackAutoMenuActive) {
    return;
  }

  if (trackMultiMenuActive) {
    int8_t choice = wideThreeChoice((uint8_t)index);
    if (choice >= 0) {
      trackMultiEditMode[selectedTrack] = choice == 0 ? -1 : (choice == 1 ? 0 : 1);
      saveSettingsToEEPROM();
      sendTrackMenu();
    }
    return;
  }

  if (trackPreviewMenuActive) {
    int8_t choice = wideThreeChoice((uint8_t)index);
    if (choice >= 0) {
      trackNotePreviewMode[selectedTrack] = choice == 0 ? -1 : (choice == 1 ? 1 : 0);
      saveSettingsToEEPROM();
      sendTrackMenu();
    }
    return;
  }

  if (trackViewMenuActive) {
    int8_t choice = wideThreeChoice((uint8_t)index);
    if (choice >= 0) {
      trackNoteViewMode[linkOwner12(selectedTrack,LINK_VIEW12)] = choice == 0 ? -1 : (choice == 1 ? NOTE_VIEW_COMPACT : NOTE_VIEW_CHROMATIC);
      compactRowOffset = 0;compactRecenter08=true;
      saveSettingsToEEPROM();
      sendTrackMenu();
      neoDisplayDirty = true;
    }
    return;
  }

  if (trackColorMenuActive) {
    // La couleur s'edite au Roy et se valide avec le Roy qui l'a modifiee.
    return;
  }

  if (trackPlayMenuActive) {
    int8_t choice = wideThreeChoice((uint8_t)index);
    if (choice >= 0) {
      pendingTrackPlayMode = (TrackPlayMode)choice;
      if (pendingTrackPlayMode == trackPlayMode[linkOwner12(selectedTrack,LINK_POLY12)]) {
        sendTrackMenu();
      } else if (pendingTrackPlayMode == TRACK_PLAY_DRUM) {
        sendDrumGateWarning();
      } else if (trackPlayMode[linkOwner12(selectedTrack,LINK_POLY12)] == TRACK_PLAY_DRUM) {
        trackGateLengthDraft = defaultGateTicks[selectedTrack];
        sendGateLengthPrompt();
      } else {
        stopActiveNotesForTrack(selectedTrack);
        (pidiRoutesDirty931|=1u<<(selectedTrack), trackPlayMode[linkOwner12(selectedTrack,LINK_POLY12)] = pendingTrackPlayMode);
        saveSettingsToEEPROM();
        sendTrackMenu();
        neoDisplayDirty = true;
      }
    }
    return;
  }

  if(linkMenu12){if(index<8)linkToggle12(index);return;}
  if(midiInMenu17){midiInButton17(index);return;}
  if(trackMidiMenu902){trackMidiDraft902^=1u<<index;commitTrackMidi902();return;}
  if(trackPage933){trackButton933(index);return;}
  if (trackMenuActive) {
    int item=menuPageItem(trackHomePage938,index,18);if(item<0)return;index=item;
    if(index==16||index==17){fxOpen32(index==16?5:6);return;}
    if(index==15){linkMenu12=true;linkDraft12=linkMask12[selectedTrack]?linkSource12[selectedTrack]:(selectedTrack?0:1);linkRender12();return;}
    if(index==14){objectName07(3);return;}
    if(index==13){kbOpenR10(true);return;}
    if(index==11||index==12){Serial.println(index==11?"SKCAM,-1":"SKCAM,1");return;}
    if(index==7){trackMenuActive=false;patch808Open();return;}
    if(index==9){leavePadModesA01(false);patternMode=grooveEditMode=false;delayMixer930=false;modMixer939=true;trackChord815=true;neoDisplayDirty=true;sendSelectionIndicatorsToPicran();return;}
    if(index==8){modMixer939=false;trackPage933=3;mixerRender938();return;}
    if(index==2){trackOpen933(1);return;}if(index==10){trackOpen933(2);return;}
    if(index==0){midiInMenu17=1;midiInRender17();}
    else if (index == 1) sendTrackMultiMenu();
    else if (index == 3) sendTrackViewMenu();
    else if (index == 4) sendTrackPlayMenu();
    else if (index == 5) sendTrackPreviewMenu();
    else if (index == 6) openTrackResetConfirmation();
    return;
  }

  if (!settingsOpen) {
    if(index==7){mod805Open(1);return;}
    if(index==2){mod805Open(3);return;}
    if(index==3){mod805Open(0);return;}
    if(index==5){patMenu46=1;patRender46();return;}
    if(index==6){mod805Open(4);return;}
    if(grooveEditMode&&index!=0&&index!=4){DISPLAY_SERIAL.println(F("MSG|J1 EXIT GROOVE"));return;}
    if(index==0){screenUiAwaitRelease=true;showParaScreen();}
    else if(index==4){screenUiAwaitRelease=true;sendTrackMenu();}
    else if(index==1){if(remixMode){remixOpenWrite();return;}screenUiAwaitRelease=true;openWriteMenu();}
    return;
  }

  // En test, les BE ne declenchent aucune fonction musicale. Le clic Y1
  // reste le retour HOME commun a toute l'interface.
  if (debugTestMode != DEBUG_TEST_NONE) return;

  if(oovMenu16){return;}
  if(settingsMenuLevel==SETTINGS_STABILITY904){buttonStability904(index);return;}

  // Menu direct : BE1..BE8 ouvrent les huit categories affichees.
  if (settingsMenuLevel == SETTINGS_CATEGORIES) {
    openSettingsCategoryByBe((uint8_t)index);
    return;
  }

  if (settingsMenuLevel == SETTINGS_MAIN_ITEMS) {
    if(index==0)settingsOpen811(false);
    return;
  }

  // Sous-menu : chaque BE ouvre l'item place physiquement en face de lui.
  if(sleepMenu43){if(index==0){sleepSave43();sleepMenu43=false;sendSettingsPage();}else if(index==4){sleepMenu43=false;sendSettingsPage();}else if(index==7){sleepSave43();sleepMenu43=false;sleepShow43();}return;}
  if(screenMapMenu)return;
  if (settingsMenuLevel == SETTINGS_ITEMS) {
    if(settingsPage==3&&index==6){sleepDraft43=sleepMinutes43;sleepMenu43=true;sleepRender43();return;}
    if(settingsPage==3&&index==7){if(playing){DISPLAY_SERIAL.println("MSG|STOP BEFORE LCD RST");return;}DISPLAY_SERIAL.println("LCDRESET43");return;}
    if(settingsPage==3&&index==5){screenMapOpen();return;}
    if(settingsPage==5){
      int item=menuPageItem(seqPage904,index,13);
      if(item==12){patchUi808=16;settingsPageRetryPending=false;cvTimingRender50();return;}
      if(item==6){openStability904(1);return;}
      if(item==7){openTransportSettings();return;}
      if(item==8){lockSettingsOpen817();return;}
      if(item==9){kbOpenR10(false);return;}
      if(item==11){patchUi808=15;settingsPageRetryPending=false;settingsRender811();return;}if(item==10){patchUi808=14;settingsPageRetryPending=false;settingsRender811();return;}
      if(item<0)return;
    }
    if(settingsPage==3&&index==4){oovMenu16=true;oovOld16[0]=oovLow16;oovOld16[1]=oovHigh16;oovRoy16=-1;oovRender16();return;}
    if (settingsPage == 6) openSettingsItemByBe((uint8_t)index);
    else if(settingsPage==4&&index==7){pinTestStart33();}
    else if(settingsPage==4&&index==6){delayLabelMenu07=1;delayLabelRender07();}
    else if(settingsPage==4&&index==4)midi808Open();
    else if(settingsPage==4&&index==5)midiConfig810();
    else if(settingsPage==2&&index==4)settingsOpenKy23816();
    else if (settingsPage == 5 && index == 3) openSeqExtraSetting(0);
    else if (settingsPage == 5 && index == 4) openSeqExtraSetting(1);
    else if (settingsPage == 5 && index == 5) openSeqExtraSetting(2);
    else if (settingsPage == 5 && index == 6) {lockSettingsOpen817();}
    else if (settingsPage == 5 && index == 7) openTransportSettings();
    else if (index < 4) openSettingsItemByBe((uint8_t)index);
    else if (settingsPage == 1 && index == 7) openPicoSettingsMenu();
    else backSettingsMenu();
    return;
  }

  if (settingsMenuLevel == SETTINGS_ROY_ITEMS) {
    if (index < 4) openRoySettingByBe((uint8_t)index);
    return;
  }

  if (settingsMenuLevel == SETTINGS_ROY_EDIT) {
    if (index == 0) commitSelectedRoySetting();
    else if (index == 4) backSettingsMenu();
    return;
  }

  if (settingsMenuLevel == SETTINGS_SEQ_EXTRA_EDIT) {
    if (index == 0 || index == 1) {
      settingsSeqExtraDraft = index == 1 ? 1 : 0;
      commitSelectedSeqExtraSetting();
    }
    return;
  }

  if (settingsMenuLevel == SETTINGS_TRANSPORT_ITEMS) {
    if(index==5){openStability904(4);return;}
    if(index==4){settingsOpen811(true);return;}
    if (index <= 2) openTransportSetting((uint8_t)index);
    return;
  }

  if (settingsMenuLevel == SETTINGS_TRANSPORT_EDIT) {
    if ((uint8_t)index == settingsTransportSelection) commitTransportSetting();
    return;
  }

  if (settingsMenuLevel == SETTINGS_PICO_ITEMS) {
    if (index < PICO_SETTINGS_ITEM_COUNT) openPicoSettingByBe((uint8_t)index);
    return;
  }

  if (settingsMenuLevel == SETTINGS_PICO_EDIT) {
    if ((uint8_t)index == settingsItemSelection) commitSelectedPicoSetting();
    return;
  }

  if (settingsMenuLevel == SETTINGS_COLOR_ITEMS) {if(index<8)colorsOpen932(index);return;}
  if (settingsMenuLevel == SETTINGS_COLOR_EDIT) {if(index>=4)colorsBrightness11(index<6?-5:5);return;}

  // Edition : le meme BE valide. Le clic J1 annule et revient.
  if (settingsMenuLevel == SETTINGS_EDIT) {
    if ((uint8_t)index == settingsItemSelection) commitSelectedSetting();
  }
}

// ============================================================
// NANO P23 / Y1-Y2 SUR TEENSY RX5 PIN 21
// ============================================================
int p23ScaledAxisValue(int value) {
  int sensitivity = clampInt(settingsValue[2][3], 1, 200);
  return (value * sensitivity) / 100;
}

int p23AxisOnThreshold() {
  // DEAD=30 retrouve le seuil historique 120. Une grande zone morte peut
  // maintenant neutraliser reellement les mouvements accidentels.
  return max(20, clampInt(settingsValue[2][0], 0, 300) * 4);
}

int p23AxisOffThreshold() {
  // DEAD=30 retrouve aussi l'hysteresis historique proche de 70.
  return max(10, (clampInt(settingsValue[2][0], 0, 300) * 7) / 3);
}

int8_t p23AxisDirection(int value, int8_t previous) {
  value = p23ScaledAxisValue(value);
  int onThreshold = p23AxisOnThreshold();
  int offThreshold = p23AxisOffThreshold();
  if (value >= onThreshold) return 1;
  if (value <= -onThreshold) return -1;
  if (previous != 0 && abs(value) > offThreshold) return previous;
  return 0;
}

bool p23AxisActionDue(P23AxisRepeat &axis, int value, int8_t &direction) {
  uint32_t now = millis();
  int8_t nextDirection = p23AxisDirection(value, axis.direction);
  if (nextDirection != axis.direction) {
    axis.direction = nextDirection;
    axis.nextActionMs = now + P23_REPEAT_FIRST_MS;
    direction = nextDirection;
    return nextDirection != 0;
  }
  if (nextDirection != 0 && (int32_t)(now - axis.nextActionMs) >= 0) {
    axis.nextActionMs = now + P23_REPEAT_MS;
    direction = nextDirection;
    return true;
  }
  direction = 0;
  return false;
}

uint32_t p23ZoomAnchorTick() {
  uint16_t minimum = STEP_COUNT;
  uint16_t maximum = 0;
  for (uint8_t track = 0; track < TRACK_COUNT; track++) {
    for (uint16_t step = 0; step < STEP_COUNT; step++) {
      if (!stepSelected[track][step]) continue;
      if (step < minimum) minimum = step;
      if (step > maximum) maximum = step;
    }
  }
  if (minimum < STEP_COUNT) {
    return ((uint32_t)minimum + maximum) * TICKS_PER_STEP / 2;
  }
  if (selectedStep >= 0) return (uint32_t)selectedStep * TICKS_PER_STEP;

  // HEAD=FREE : sans selection, le zoom reste centre sur la vue. Il ne doit
  // jamais utiliser currentStep, qui se deplace uniquement pendant Play.
  return viewTickOffset + windowTickSpan() / 2;
}

const char* viewSourceName(uint8_t source) {
  static const char* names[] = {
    "BOOT", "J2_ZOOM", "J2_PAGE", "J2_RESET", "ROY1_ZOOM", "HEAD",
    "ROY2", "TRACK", "PATTERN", "LENGTH", "UNKNOWN"
  };
  return source < (sizeof(names) / sizeof(names[0])) ? names[source] : "UNKNOWN";
}

void serviceViewAudit() {
  if(!diagActive814)return;
  if (viewTickOffset == viewAuditOffset && zoomLevel == viewAuditZoom) return;
  ViewLogEntry &entry = viewLog[viewLogWrite];
  entry.atMs = millis();
  entry.offset = viewTickOffset;
  entry.playTick = measureTick;
  entry.zoom = zoomLevel;
  entry.track = selectedTrack;
  entry.source = pendingViewSource == VIEW_SRC_BOOT ? VIEW_SRC_UNKNOWN : pendingViewSource;
  entry.playingState = playing ? 1 : 0;
  viewLogWrite = (uint8_t)((viewLogWrite + 1) % VIEW_LOG_COUNT);
  if (viewLogUsed < VIEW_LOG_COUNT) viewLogUsed++;
  viewAuditOffset = viewTickOffset;
  viewAuditZoom = zoomLevel;
  Serial.print(F("VIEWLOG,MS=")); Serial.print(entry.atMs);
  Serial.print(F(",SRC=")); Serial.print(viewSourceName(entry.source));
  Serial.print(F(",PLAY=")); Serial.print(entry.playingState);
  Serial.print(F(",T=")); Serial.print(entry.track + 1);
  Serial.print(F(",ZOOM=")); Serial.print(entry.zoom);
  Serial.print(F(",OFFSET=")); Serial.print(entry.offset);
  Serial.print(F(",HEAD=")); Serial.println(entry.playTick);
  pendingViewSource = VIEW_SRC_UNKNOWN;
}

FLASHMEM void printViewLog() {
  Serial.print(F("VIEWLOG BEGIN,COUNT=")); Serial.println(viewLogUsed);
  uint8_t first = (uint8_t)((viewLogWrite + VIEW_LOG_COUNT - viewLogUsed) % VIEW_LOG_COUNT);
  for (uint8_t i = 0; i < viewLogUsed; i++) {
    const ViewLogEntry &entry = viewLog[(first + i) % VIEW_LOG_COUNT];
    Serial.print(F("VIEWLOG,MS=")); Serial.print(entry.atMs);
    Serial.print(F(",SRC=")); Serial.print(viewSourceName(entry.source));
    Serial.print(F(",PLAY=")); Serial.print(entry.playingState);
    Serial.print(F(",T=")); Serial.print(entry.track + 1);
    Serial.print(F(",ZOOM=")); Serial.print(entry.zoom);
    Serial.print(F(",OFFSET=")); Serial.print(entry.offset);
    Serial.print(F(",HEAD=")); Serial.println(entry.playTick);
  }
  Serial.println(F("VIEWLOG END"));
}

void zoomAroundSelection(int direction, uint8_t source) {
  if (grooveEditMode) return;
  if (direction == 0) return;
  uint32_t anchorTick = p23ZoomAnchorTick();
  int8_t oldZoom = zoomLevel;
  if (direction > 0 && zoomLevel < 5) zoomLevel++;
  else if (direction < 0 && zoomLevel > -2) zoomLevel--;
  if (zoomLevel == oldZoom) return;
  pendingViewSource = (ViewChangeSource)source;

  uint32_t spanTicks = windowTickSpan();
  // Les vues d'une mesure ou davantage commencent toujours sur une vraie
  // frontiere de vue. Cela garde le premier temps sur le premier pad.
  int32_t offset = zoomLevel <= 0
                     ? (int32_t)((anchorTick / spanTicks) * spanTicks)
                     : (int32_t)anchorTick - (int32_t)(spanTicks / 2);
  uint32_t maxOffset = activeSequenceTicks() > spanTicks ? activeSequenceTicks() - spanTicks : 0;
  viewTickOffset = (uint32_t)clampInt((int)offset, 0, (int)maxOffset);
  neoDisplayDirty = true;
  lastNeoRefreshMs = millis() - NEO_MIN_REFRESH_MS;
  displayValueDirty = true;
  displayBpmDirty = true;
}

void p23MoveViewPage(int direction) {
  if (direction == 0) return;
  int32_t next = (int32_t)viewTickOffset + direction * (int32_t)max(1UL,windowTickSpan()/2);
  uint32_t maxOffset = activeSequenceTicks() > windowTickSpan() ? activeSequenceTicks() - windowTickSpan() : 0;
  viewTickOffset = (uint32_t)clampInt((int)next, 0, (int)maxOffset);
  pendingViewSource = VIEW_SRC_J2_PAGE;
  neoDisplayDirty = true;
  lastNeoRefreshMs = millis() - NEO_MIN_REFRESH_MS;
  displayBpmDirty = true;
}

void requestEpRecovery() {
  DISPLAY_SERIAL.println(F("RST|EP"));
  showGameScreen();
}

// References de pages, exclusivement dans le code. Aucun historique de textes.
// A=accueil, AA=parametres, AC=track, ACB=mult, ACF=note preview.
enum MenuPageRef : uint8_t {
  PAGE_A, PAGE_AA, PAGE_AA_CHILD, PAGE_AA_DEBUG_TEST,
  PAGE_AC, PAGE_ACB, PAGE_ACD, PAGE_ACE, PAGE_ACF, PAGE_ACG,
  PAGE_ACE_CONFIRM, PAGE_SAVE, PAGE_SAVE_CLIP, PAGE_LOAD,
  PAGE_FILE, PAGE_FILE_CONFIRM, PAGE_WRITE, PAGE_CLIP, PAGE_PASTE,
  PAGE_BAR, PAGE_PATTERN, PAGE_STORAGE
};
MenuPageRef currentMenuPageRef();
MenuPageRef parentMenuPageRef(MenuPageRef page);
MenuPageRef currentMenuPageRef() {
  if (debugTestMode != DEBUG_TEST_NONE) return PAGE_AA_DEBUG_TEST;
  if (patternLoadConfirmActive) return PAGE_FILE_CONFIRM;
  if (fileBrowserMode != FILE_BROWSER_NONE) return PAGE_FILE;
  if (saveMenuActive) return saveMenuClipboardMode ? PAGE_SAVE_CLIP : PAGE_SAVE;
  if (loadTypeMenuActive) return PAGE_LOAD;
  if (writeMenuActive) return PAGE_WRITE;
  if (settingsOpen) return settingsMenuLevel == SETTINGS_CATEGORIES ? PAGE_AA : PAGE_AA_CHILD;
  if (trackDrumWarningActive || trackGateLengthPromptActive) return PAGE_ACE_CONFIRM;
  if (trackMultiMenuActive) return PAGE_ACB;
  if (trackViewMenuActive) return PAGE_ACD;
  if (trackPlayMenuActive) return PAGE_ACE;
  if (trackPreviewMenuActive) return PAGE_ACF;
  if (trackResetConfirmActive) return PAGE_ACG;
  if (trackMenuActive) return PAGE_AC;
  if (clipboardPasteMenuActive) return PAGE_PASTE;
  if (clipboardMenuActive) return PAGE_CLIP;
  if (barFillPromptActive || barDeletePromptActive) return PAGE_BAR;
  if (patternModeEditActive) return PAGE_PATTERN;
  return PAGE_A;
}
MenuPageRef parentMenuPageRef(MenuPageRef page) {
  switch (page) {
    case PAGE_ACB: case PAGE_ACD: case PAGE_ACE: case PAGE_ACF: case PAGE_ACG: return PAGE_AC;
    case PAGE_ACE_CONFIRM: return PAGE_ACE;
    case PAGE_SAVE_CLIP: return PAGE_SAVE;
    case PAGE_FILE_CONFIRM: return PAGE_FILE;
    case PAGE_AA_DEBUG_TEST: return PAGE_AA_CHILD;
    case PAGE_STORAGE: return PAGE_STORAGE;
    default: return PAGE_A;
  }
}

void p23BackOrRecover() {
 if(fileBrowserMode==FILE_BROWSER_LOAD||loadTypeMenuActive){
  if(patternLoadConfirmActive){answerPatternLoadConfirmation(false);return;}
  if(itemBack809())return;
  if(fileBrowserMode==FILE_BROWSER_LOAD){if(!browserAtTypeRoot()){browserParentDirectory();return;}fileBrowserMode=FILE_BROWSER_NONE;loadTypeMenuActive=true;sendLoadTypeMenu();return;}
  showGameScreen();return;
 }
 if(patMenu46&&!patchUi808){patBack46();return;}  if(sdMenu42){sdMenu42=false;if(sdMenuSave42)openSaveMenu();else openLoadBrowser();return;} if(pinTestActive33){pinTestStop33();return;} if(modUi805&&!itemKind809&&!itemChoosing809&&!patchUi808){mod805Back();return;}
  if(sleepMenu43){sleepMenu43=false;sendSettingsPage();return;}
  if(screenMapMenu){screenMapClose(false);return;}
  if(kbBackR10())return;
  // J1 returns through EP without changing the independent note selection.
 if(assignOpen932){assignCancel932();return;}
  if(saveCollision905){saveCollision905=false;renderFileBrowser();return;}
  if(linkMenu12){sendTrackMenu();return;}
  if(midiInMenu17){if(midiInMenu17==2){midiInMenu17=1;midiInRender17();}else sendTrackMenu();return;}
  if(trackPage933){sendTrackMenu();return;}
  if(trackMidiMenu902){trackMidiMenu902=false;midiInMenu17=1;midiInRender17();return;}
  if(itemBack809())return;
  if(patch808Back())return;
  if (mod805Back()) return;
  if(songExportArmed){songExportArmed=false;songSend("EXPORT,CANCELLED");return;}
  if(settingsOpen&&debugTestMode==DEBUG_TEST_NONE){backSettingsMenu();return;}
  MenuPageRef page = currentMenuPageRef();
  MenuPageRef parent = parentMenuPageRef(page);
  // Storage has a real parent; do not swallow J1 here.
  if (parent == PAGE_AC) { sendTrackMenu(); return; }
  if (parent == PAGE_ACE) { sendTrackPlayMenu(); return; }
  if (page == PAGE_SAVE_CLIP) {
    saveMenuClipboardMode = false;
    saveMenuSelection = 1;
    sendSaveMenu();
    return;
  }
  if (beatEditActive && page == PAGE_A) {
    beatEditActive = false;
    beatEditLength = false;
    beatDraftBars = sequenceBars;
    displayBpmDirty = true;
    return;
  }
  if (barDeletePromptActive) {
    barDeletePromptActive = false;
    beatDraftBars = sequenceBars;
    beatEditLength = false;
    showGameScreen();
    return;
  }
  if (patternModeEditActive) {
    patternModeEditActive = false;
    showGameScreen();
    return;
  }
  if (barFillPromptActive) {
    barFillPromptActive = false;
    showGameScreen();
    return;
  }
  if (debugTestMode != DEBUG_TEST_NONE) {
    stopDebugTest();
    settingsOpen = true;
    settingsMenuLevel = SETTINGS_ITEMS;
    settingsPage = 6;
    sendSettingsPage();
    return;
  }
  if (patternLoadConfirmActive) {
    answerPatternLoadConfirmation(false);
    return;
  }
  if (fileBrowserMode != FILE_BROWSER_NONE) {
    if (!browserAtTypeRoot()) {
      browserParentDirectory();
      return;
    }
    FileBrowserMode previousMode = fileBrowserMode;
    fileBrowserMode = FILE_BROWSER_NONE;
    patternLoadConfirmActive = false;
    if (previousMode == FILE_BROWSER_SAVE) {
      afterSaveAction = AFTER_SAVE_NONE;
      saveMenuActive = true;
      if(saveMenuClipboardMode)clipMenuEnter18();
      sendSaveMenu();
    } else {
      loadTypeMenuActive = true;
      sendLoadTypeMenu();
    }
    return;
  }
  if (loadTypeMenuActive || saveMenuActive) {
    showGameScreen();
    return;
  }
  if (writeMenuActive) {
    showGameScreen();
    return;
  }
  if (trackDrumWarningActive || trackGateLengthPromptActive) {
    sendTrackPlayMenu();
    return;
  }
  if (trackColorMenuActive || trackViewMenuActive || trackResetConfirmActive ||
      trackAutoMenuActive || trackMultiMenuActive || trackPreviewMenuActive || trackPlayMenuActive) {
    sendTrackMenu();
    return;
  }
  if (trackMenuActive) {
    showGameScreen();
    return;
  }
  if(oovMenu16){oovClose16(false);return;}
  if (settingsOpen) {
    backSettingsMenu();
    return;
  }
  if (clipboardPasteMenuActive) {
    clipboardPasteMenuActive = false;
    showGameScreen();
    return;
  }
  if (clipboardMenuActive) {
    closeClipboardMenu();
    return;
  }
  if (numericEntryActive) {
    finishNumericEntry(false);
    return;
  }
  if(remixBack911())return;
  if (songMode && songDeleteArmed) { songDeleteArmed=false; songState(); return; }
  if (grooveEditMode) { grooveEditMode = false; grooveStepMask = 0; selectButtonActive = shiftButtonActive = columnSelectionMode = selectionMode = false; selection805Reset(); clearPatternSelection(); sendGroovePacket(); sendSelectionIndicatorsToPicran(); neoDisplayDirty = displayValueDirty = displayBpmDirty = true; return; }
  if (deleteButtonActive) { finishDeleteMode(false); return; }
  // Selection cancellation belongs to J2; J1 only closes the EP context.
  if(menuStopLatched||currentMenuPageRef()!=PAGE_A)showGameScreen(); // Recover an orphaned storage latch.
  // A (accueil) n'a pas de parent.
}

void p23NavigateMenu(int xDirection, int yDirection, bool click) {
  if(patMenu46&&!patchUi808){if(click)patBack46();else patRoy46(0,xDirection?xDirection:yDirection,false);return;}
  // Move confirmation/cancellation belongs to the dedicated selection buttons.
  if(settingsOpen&&settingsMenuLevel==SETTINGS_COLOR_EDIT&&xDirection&&!click){saveSettingsToEEPROM();save902();savePrefs932();colorsOpen932((colorGroup932+(xDirection>0?1:7))%8);return;}
  if(click){p23BackOrRecover();return;}
  if(kbMenuR10){if(kbMenuTrackR10&&xDirection){kbMenuR10=xDirection>0?2:3;kbRenderMenuR10();}return;}
  if(saveCollision905){if(click){saveCollision905=false;renderFileBrowser();}return;}
  if(itemChoosing809){if(click)itemBack809();else if(xDirection||yDirection)itemRoy809(0,xDirection?xDirection:yDirection,false);return;}
  if(itemKind809&&click){itemBack809();return;}
  if(delayLabelMenu07){if(click){if(--delayLabelMenu07)delayLabelRender07();else sendSettingsPage();}else if(xDirection||yDirection)scrollMenuText(xDirection?xDirection:yDirection);return;}
  if(patchUi808){if(click){if(!patchParent07())patch808Back();}else if(patchUi808==6&&(xDirection||yDirection))scrollMenuText(xDirection?xDirection:yDirection);return;}
  if(modUi805){if(click)mod805Back();else mod806Navigate(xDirection);return;}
  // Le clic Y1 remonte d'un etage. Depuis HOME il recupere EP.
  if (click) {
    p23BackOrRecover();
    return;
  }

  if (patternLoadConfirmActive) {
    return;
  }

  if (fileBrowserMode != FILE_BROWSER_NONE) {
    if (xDirection > 0 && browserHasMoreDirectoryPages) {
      fileBrowserDirectoryPage++;
    } else if (xDirection < 0 && fileBrowserDirectoryPage > 0) {
      fileBrowserDirectoryPage--;
    }
    if (yDirection != 0 && fileBrowserMode == FILE_BROWSER_LOAD) {
      cycleBrowserFile(yDirection > 0 ? 1 : -1);
    }
    if (fileBrowserMode != FILE_BROWSER_NONE) renderFileBrowser();
    return;
  }

  if(trackMenuActive&&!midiInMenu17&&!linkMenu12&&!trackPage933&&!trackMidiMenu902&&!trackResetConfirmActive&&!trackAutoMenuActive&&!trackMultiMenuActive&&!trackPreviewMenuActive&&!trackPlayMenuActive&&!trackColorMenuActive&&!trackViewMenuActive&&!trackDrumWarningActive&&!trackGateLengthPromptActive&&!kbMenuR10&&xDirection){trackHomePage938=clampInt((int)trackHomePage938+(xDirection>0?1:-1),0,2);sendTrackMenu();return;}
  if (loadTypeMenuActive) return;

  if (saveMenuActive) {
    return;
  }

  if (settingsOpen) {
    if(settingsMenuLevel==SETTINGS_STABILITY904)return;
    if(settingsMenuLevel==SETTINGS_ITEMS&&settingsPage==5&&xDirection){seqPage904=xDirection>0?1:0;sendSettingsPage();return;}
    if (settingsMenuLevel == SETTINGS_CATEGORIES && xDirection != 0) {
      uint8_t nextPage = xDirection > 0 ? 1 : 0;
      if (nextPage != settingsCategoryPage) {
        settingsCategoryPage = nextPage;
        settingsCategorySelection = menuPageStart(settingsCategoryPage);
        sendSettingsPage();
      }
      return;
    }
    if ((settingsMenuLevel == SETTINGS_EDIT || settingsMenuLevel == SETTINGS_PICO_EDIT) &&
        yDirection != 0) {
      if (settingsMenuLevel == SETTINGS_PICO_EDIT) editSelectedPicoSetting(yDirection > 0 ? -1 : 1);
      else editSelectedSetting(yDirection > 0 ? -1 : 1);
      return;
    }
    if (settingsMenuLevel == SETTINGS_ITEMS && settingsPage == 6) {
      if (xDirection > 0 && debugMenuPage == 0) {
        debugMenuPage = 1;
        sendSettingsPage();
      } else if (xDirection < 0 && debugMenuPage > 0) {
        debugMenuPage--;
        sendSettingsPage();
      }
    }
    return;
  }

  if (clipboardMenuActive) {
    if (yDirection != 0) {
      int next = (int)clipboardMenuRecentIndex + (yDirection > 0 ? 1 : -1);
      while (next < 0) next += 4;
      clipboardMenuRecentIndex = (uint8_t)(next % 4);
      clipboardCopyScope[clipboardMenuSlot] = (ClipboardCopyScope)clipboardMenuRecentIndex;
      clipboardMenuLastActionMs = millis();
      sendClipboardMenu();
    }
    return;
  }

  if (clipboardPasteMenuActive) {
    if (yDirection != 0) {
      uint8_t slot = clipboardPasteMenuSlot;
      clipboardPasteMode[slot] = clipboardPasteMode[slot] == CLIP_PASTE_ERASE
                                   ? CLIP_PASTE_OVERDUB : CLIP_PASTE_ERASE;
      sendClipboardPasteMenu(slot);
    }
  }
}

void reportDebugKy23(int y1Dx, int y1Dy, bool y1Switch,
                     int y2Dx, int y2Dy, bool y2Switch) {
  uint32_t now = millis();
  bool changed = abs(y1Dx - debugP23LastY1Dx) >= 5 ||
                 abs(y1Dy - debugP23LastY1Dy) >= 5 ||
                 abs(y2Dx - debugP23LastY2Dx) >= 5 ||
                 abs(y2Dy - debugP23LastY2Dy) >= 5 ||
                 y1Switch != debugP23LastY1Switch || y2Switch != debugP23LastY2Switch;
  if (!changed && (uint32_t)(now - debugP23LastReportMs) < 250) return;

  debugP23LastReportMs = now;
  debugP23LastY1Dx = y1Dx;
  debugP23LastY1Dy = y1Dy;
  debugP23LastY2Dx = y2Dx;
  debugP23LastY2Dy = y2Dy;
  debugP23LastY1Switch = y1Switch;
  debugP23LastY2Switch = y2Switch;

  char line1[21], line2[21];
  snprintf(line1, sizeof(line1), "J1 X%+d Y%+d CLICK:%d", y1Dx, y1Dy, y1Switch ? 1 : 0);
  snprintf(line2, sizeof(line2), "J2 X%+d Y%+d CLICK:%d", y2Dx, y2Dy, y2Switch ? 1 : 0);
  sendDebugDisplay("KY23 TARGETS", line1, line2, "BE1 BACK",
                   y1Switch ? "J1 CLICK [X]" : "J1 CLICK [ ]",
                   y2Switch ? "J2 CLICK [X]" : "J2 CLICK [ ]");

  Serial.print(F("DBG,KY23,Y1,DX=")); Serial.print(y1Dx);
  Serial.print(F(",DY=")); Serial.print(y1Dy);
  Serial.print(F(",SW=")); Serial.print(y1Switch ? 1 : 0);
  Serial.print(F(",Y2,DX=")); Serial.print(y2Dx);
  Serial.print(F(",DY=")); Serial.print(y2Dy);
  Serial.print(F(",SW=")); Serial.println(y2Switch ? 1 : 0);
}

FLASHMEM void handleP23Line(char* line) {
  if(sdBusy905())return;
  if (!line || strncmp(line, "DATA,", 5) != 0) return;
  char* fields[13] = {nullptr};
  uint8_t count = 0;
  char* token = strtok(line, ",");
  while (token && count < 13) {
    fields[count++] = token;
    token = strtok(nullptr, ",");
  }
  if (count < 13 || strcmp(fields[1], "Y1") != 0 || strcmp(fields[7], "Y2") != 0) return;

  int y1Dx = atoi(fields[4]);
  int y1Dy = atoi(fields[5]);
  bool y1SwitchPressed = atoi(fields[6]) != 0;
  int y2Dx = atoi(fields[10]);
  int y2Dy = atoi(fields[11]);
  bool y2SwitchPressed = atoi(fields[12]) != 0;
  if(!pc831.injecting){
    if((pc831.joyMask&1)||pc831.joy[0]||pc831.joy[1]||pc831.joy[2]){y1Dx=pc831.joy[0];y1Dy=pc831.joy[1];y1SwitchPressed=pc831.joy[2];}
    if((pc831.joyMask&2)||pc831.joy[3]||pc831.joy[4]||pc831.joy[5]){y2Dx=pc831.joy[3];y2Dy=pc831.joy[4];y2SwitchPressed=pc831.joy[5];}
  }
  if(abs(y1Dx)>80||abs(y1Dy)>80||abs(y2Dx)>80||abs(y2Dy)>80){presentationTouch43();inputActivity811=millis();}
  joyInput46(atoi(fields[8]),atoi(fields[9]),y2Dx,y2Dy);
  p23DebugValidLines++;
  p23DebugY1Dx = y1Dx;
  p23DebugY2Dx = y2Dx;
  p23DebugY2Dy = y2Dy;
  p23DebugY2Switch = y2SwitchPressed;

  uint32_t buttonNow816=millis();
  if(ky23Buttons816[0].sample(y1SwitchPressed,buttonNow816))inputLog815(I815_J,0,y1SwitchPressed,I815_RAW);
  if(ky23Buttons816[1].sample(y2SwitchPressed,buttonNow816))inputLog815(I815_J,1,y2SwitchPressed,I815_RAW);
  // Clicks are serviced separately after the configured stable interval.
  bool click=false,y2Click=false;
  if(controller940){int8_t move942=0;if(p23AxisActionDue(p23Y1X,y1Dx,move942))controllerEmit940("JOY",0,-move942);if(p23AxisActionDue(p23Y2X,y2Dx,move942))controllerEmit940("JOY",1,-move942);return;}

  if (debugTestMode == DEBUG_TEST_KY23) {
    reportDebugKy23(y1Dx, y1Dy, y1SwitchPressed, y2Dx, y2Dy, y2SwitchPressed);
    return;
  }
  if (debugTestMode != DEBUG_TEST_NONE) {
    if (click) p23BackOrRecover();
    return;
  }

  int8_t direction = 0;

  int xAction = 0;
  int yAction = 0;
  bool menuActive = patMenu46 || patchUi808 || modUi805 || settingsOpen || saveMenuActive || loadTypeMenuActive || writeMenuActive || fileBrowserMode != FILE_BROWSER_NONE ||
                    patternLoadConfirmActive || clipboardMenuActive || clipboardPasteMenuActive ||
                    trackMenuActive || trackResetConfirmActive || trackAutoMenuActive ||
                    trackMultiMenuActive || trackPreviewMenuActive || trackPlayMenuActive ||
                    trackColorMenuActive || trackViewMenuActive ||
                    barDeletePromptActive || patternModeEditActive;
  // J2 horizontal fait defiler la ligne d'explication longue des menus.
  (void)y2Click;
  // Le cablage analogique donne un delta positif a gauche/haut.
  // J1 navigates EP only; dedicated arrows navigate Neo.
  if (p23AxisActionDue(p23Y1X, y1Dx, direction)) {
    if (menuActive) xAction = -direction;
  }
  if (p23AxisActionDue(p23Y1Y, y1Dy, direction)) {
    if (menuActive) yAction = -direction;

  }
  // J2 axes are reserved for I/O; explanatory text scrolls automatically.
  if(xAction||yAction||click){alertAction08();delayIncrementShown925=0;inputActivity811=millis();}
  if (menuActive && (xAction != 0 || yAction != 0 || click)) {
    p23NavigateMenu(xAction, yAction, click);
  } else if (click) {
    p23BackOrRecover();
  }
}

void serviceP23() {
  while (P23_SERIAL.available() > 0) {
    char c = (char)P23_SERIAL.read();
    p23DebugBytes++;
    if (c == '\r') continue;
    if (c == '\n') {
      p23Line[p23LinePos] = '\0';
      if (p23LinePos > 0) {
        p23DebugLines++;
        handleP23Line(p23Line);
      }
      p23LinePos = 0;
    } else if (p23LinePos < sizeof(p23Line) - 1) {
      p23Line[p23LinePos++] = c;
    } else {
      p23LinePos = 0;
    }
  }
  for(uint8_t i=0;i<2;++i){
    if(!ky23Buttons816[i].poll(millis(),ky23Debounce816))continue;
    bool pressed=ky23Buttons816[i].stable;
    inputLog815(I815_J,i,pressed,I815_STABLE);InputScope815 input(I815_J,i,pressed);
    if(i)p23Y2SwitchWasPressed=pressed;else p23Y1SwitchWasPressed=pressed;
    if(controller940){if(!i&&pressed)controllerEmit940("UNDO",0,1);continue;}
    if(!i&&pressed){
      inputActivity811=millis();
      if(debugTestMode==DEBUG_TEST_KY23)continue;
      if(debugTestMode!=DEBUG_TEST_NONE)p23BackOrRecover();else p23NavigateMenu(0,0,true);
    }
  }
}

void serviceP23Debug() {
  if (debugTestMode != DEBUG_TEST_KY23) return;
  uint32_t now = millis();
  if ((uint32_t)(now - p23DebugLastReportMs) < 1000) return;
  p23DebugLastReportMs = now;
  Serial.print(F("P23DBG,B="));
  Serial.print(p23DebugBytes);
  Serial.print(F(",L="));
  Serial.print(p23DebugLines);
  Serial.print(F(",V="));
  Serial.print(p23DebugValidLines);
  Serial.print(F(",Y1DX="));
  Serial.print(p23DebugY1Dx);
  Serial.print(F(",Y2DX="));
  Serial.print(p23DebugY2Dx);
  Serial.print(F(",Y2DY="));
  Serial.print(p23DebugY2Dy);
  Serial.print(F(",SW="));
  Serial.println(p23DebugY2Switch ? 1 : 0);
}

bool transportTuningOverlayVisible() {
  if (!transportTuningOverlayActive) return false;
  if ((uint32_t)(millis() - transportTuningLastAdjustMs) < TRANSPORT_TUNING_OVERLAY_MS) return true;

  transportTuningOverlayActive = false;
  displayValueDirty = true;
  displayBpmDirty = true;
  return false;
}

FLASHMEM void sendTransportTuningOverlay(const char* label, uint16_t value, uint8_t kyNumber) {
  transportTuningOverlayActive = true;
  transportTuningLastAdjustMs = millis();

  DISPLAY_SERIAL.print(F("VAL,"));
  DISPLAY_SERIAL.print(label);
  DISPLAY_SERIAL.print(',');
  DISPLAY_SERIAL.print(value);
  DISPLAY_SERIAL.print(F("ms,-,0,KY"));
  DISPLAY_SERIAL.println(kyNumber);

  Serial.print(F("TEST TRANSPORT: "));
  Serial.print(label);
  Serial.print(F(" = "));
  Serial.print(value);
  Serial.println(F(" ms"));
}

FLASHMEM void refreshOledValue(bool force = false) {
  if (!force && transportTuningOverlayVisible()) return;
  if (!force && !displayValueDirty) return;
  uint32_t now = millis();
  // Parametres > SCREEN > DLY pilote maintenant reellement la cadence des
  // valeurs EP/ABCD. 0 = chaque evenement, 20..50 ms = reactif, 180 ms =
  // ancien comportement qui pouvait regrouper visuellement plusieurs crans.
  uint16_t minRefreshMs = (uint16_t)clampInt(settingsValue[3][0], 0, 1000);
  if (settingsValue[3][2] == 1 && minRefreshMs < 500) minRefreshMs = 500;
  else if (settingsValue[3][2] == 2) minRefreshMs = 1000;
  if (!force && (uint32_t)(now - lastDisplayValueRefreshMs) < minRefreshMs) return;
  lastDisplayValueRefreshMs = now;
  displayValueDirty = false;
  sendDisplayValuePacket();
}

FLASHMEM void refreshOledBpm(bool force = false) {
  static uint32_t lastView38=UINT32_MAX;static int lastZoom38=99;static uint8_t lastTrack38=255;
  if(lastView38!=viewTickOffset||lastZoom38!=zoomLevel||lastTrack38!=selectedTrack){lastView38=viewTickOffset;lastZoom38=zoomLevel;lastTrack38=selectedTrack;displayBpmDirty=true;}
  if (!force && transportTuningOverlayVisible()) return;
  if (!force && !displayBpmDirty) return;
  uint32_t now = millis();
  if (!force && (uint32_t)(now - lastDisplayBpmRefreshMs) < DISPLAY_BPM_MIN_REFRESH_MS) return;
  lastDisplayBpmRefreshMs = now;
  displayBpmDirty = false;
  sendDisplayBpmPacket();
}

// ============================================================
// NEO MAPPING / UI
// ============================================================
uint8_t visibleTrackForSlot(uint8_t slot) {
  return slot <= 3 ? selectedTrack : 255;
}

bool isTrackVisible(uint8_t track) {
  return track == selectedTrack && track < TRACK_COUNT;
}

void ensureSelectedTrackVisible() {
  if (selectedTrack >= TRACK_COUNT) selectedTrack = 0;
  viewStartTrack = selectedTrack;
  sequenceBars = trackBars[selectedTrack];
  measureTick = localTickForTrack(selectedTrack);
  currentStep = (uint16_t)(measureTick / TICKS_PER_STEP);
}

uint32_t windowTickSpan() {
  uint32_t barTicks = (uint32_t)STEPS_PER_BAR * TICKS_PER_STEP;
  if (zoomLevel < 0) return barTicks << (-zoomLevel);
  return barTicks >> zoomLevel;
}

uint16_t visibleStepSpan() {
  uint32_t ticks = windowTickSpan();
  return (uint16_t)((ticks + TICKS_PER_STEP - 1) / TICKS_PER_STEP);
}

void clampViewStepOffset() {
  uint32_t spanTicks = windowTickSpan();
  uint32_t maxOffset = activeSequenceTicks() > spanTicks ? activeSequenceTicks() - spanTicks : 0;
  if (viewTickOffset > maxOffset) viewTickOffset = maxOffset;
}

uint16_t visualPosToStep(uint8_t visualPos) {
  clampViewStepOffset();
  uint32_t pixelTicks = windowTickSpan() / 16;
  if (pixelTicks < 1) pixelTicks = 1;
  uint32_t tick = viewTickOffset + (uint32_t)visualPos * pixelTicks;
  uint16_t step = (uint16_t)(tick / TICKS_PER_STEP);
  return step < activeStepCount() ? step : activeStepCount() - 1;
}

uint8_t padVisualPosition(uint8_t neoIndex, uint8_t pad) {
  uint8_t localCol = pad % 4;
  return neoIndex * 4 + localCol;
}

void padTimelineBounds(uint8_t neoIndex, uint8_t pad, uint32_t &pixelStart, uint32_t &pixelEnd) {
  uint32_t span = windowTickSpan();
  uint32_t pixelTicks = max((uint32_t)1, span / 16);
  uint32_t rowOffset = monoMeasureRowsView(selectedTrack) ? (uint32_t)(pad / 4) * span : 0;
  pixelStart = viewTickOffset + rowOffset + (uint32_t)padVisualPosition(neoIndex, pad) * pixelTicks;
  pixelEnd = pixelStart + pixelTicks;
}

void tickToStepAndMicro(uint8_t track, uint32_t tick, uint16_t &step, int16_t &micro) {
  uint16_t count = trackStepCount(track);
  if (count == 0) {
    step = STEP_COUNT;
    micro = 0;
    return;
  }
  if (tick >= (uint32_t)count * TICKS_PER_STEP) {
    step = STEP_COUNT;
    micro = 0;
    return;
  }
  step = (uint16_t)(tick / TICKS_PER_STEP);
  micro = (int16_t)(tick % TICKS_PER_STEP);
}

uint8_t compactPitchRows[4] = {255, 36, 255, 255};

uint8_t collectCompactPitchEntries(uint8_t track, uint8_t entries[130]) {
  bool used[128] = {};
  if (track < TRACK_COUNT) {
    for (uint16_t step = 0; step < trackStepCount(track); step++) {
      uint8_t count = noteCountAtStep(track, step);
      for (uint8_t voice = 0; voice < count; voice++) used[visualNoteAtVoice(track, step, voice)] = true;
    }
  }
  uint8_t pitchCount = 0;
  for (int pitch = 127; pitch >= 0; pitch--) {
    if (used[pitch]) entries[1 + pitchCount++] = (uint8_t)pitch;
  }
  if (pitchCount == 0) return 0;
  entries[0] = used[defaultMidiNote[track]]&&entries[1]<127?entries[1]+1:255;
  entries[pitchCount + 1] = used[defaultMidiNote[track]]&&entries[pitchCount]>0?entries[pitchCount]-1:255;
  return pitchCount + 2;
}

void rebuildCompactPitchRows() {
  for (uint8_t row = 0; row < 4; row++) compactPitchRows[row] = 255;
  uint8_t entries[130];
  uint8_t entryCount = collectCompactPitchEntries(selectedTrack, entries);
  if (entryCount == 0) {
    compactRowOffset = 0;compactRecenter08=true;
    compactPitchRows[1] = defaultMidiNote[selectedTrack]; // ligne de depart 2
    return;
  }

  uint8_t maxOffset = entryCount > 4 ? entryCount - 4 : 0;
  compactRecenter08=false; // Keep the chosen rows when changing measure.
  if (selectedStep >= 0 && selectedStep < (int16_t)trackStepCount(selectedTrack) &&
      noteCountAtStep(selectedTrack,selectedStep)>selectedVoice) {
    uint8_t selectedPitch = visualNoteAtVoice(selectedTrack, (uint16_t)selectedStep, selectedVoice);
    for (uint8_t index = 0; index < entryCount; index++) {
      if (entries[index] != selectedPitch) continue;
      if (index < compactRowOffset) compactRowOffset = index;
      else if (index >= compactRowOffset + 4) compactRowOffset = index - 3;
      break;
    }
  }
  compactRowOffset = min(compactRowOffset, maxOffset);
  for (uint8_t row = 0; row < 4; row++) {
    uint8_t index = compactRowOffset + row;
    if (index < entryCount) compactPitchRows[row] = entries[index];
  }
}

void moveCompactPitchRows(int direction) {
  if (noteViewForTrack(selectedTrack) != NOTE_VIEW_COMPACT || direction == 0) return;
  uint8_t entries[130];
  uint8_t entryCount = collectCompactPitchEntries(selectedTrack, entries);
  uint8_t maxOffset = entryCount > 4 ? entryCount - 4 : 0;
  compactRowOffset = (uint8_t)clampInt((int)compactRowOffset + (direction > 0 ? 1 : -1),
                                       0, maxOffset);
  rebuildCompactPitchRows();
  neoDisplayDirty = true;
}

uint8_t padChromaticNote(uint8_t pad) {
  if (monoMeasureRowsView(selectedTrack)) return defaultMidiNote[selectedTrack];
  uint8_t localRow = pad / 4; // haut = note la plus haute
  if (noteViewForTrack(selectedTrack) == NOTE_VIEW_COMPACT) return compactPitchRows[localRow];
  return (uint8_t)clampInt((int)chromaticBottomNote + 3 - localRow, 0, 127);
}

bool visibleNoteAtPad(uint8_t track, uint8_t rowNote, uint8_t neoIndex, uint8_t pad,
                      uint16_t &sourceStep, uint8_t *sourceVoice, bool headOnly) {
  if (track >= TRACK_COUNT) return false;
  uint32_t pixelStart;
  uint32_t pixelEnd;
  padTimelineBounds(neoIndex, pad, pixelStart, pixelEnd);
  uint16_t count = trackStepCount(track);
  if (count == 0) return false;
  uint16_t lastCandidate = (uint16_t)min((uint32_t)(count - 1), pixelEnd / TICKS_PER_STEP);
  uint16_t firstCandidate = pixelStart > 384
                              ? (uint16_t)((pixelStart - 384) / TICKS_PER_STEP) : 0;
  bool found = false;
  uint32_t centre = pixelStart + (pixelEnd - pixelStart) / 2;
  uint32_t bestDistance = 0xFFFFFFFFUL;
  for (uint16_t candidate = firstCandidate; candidate <= lastCandidate; candidate++) {
    uint8_t voiceCount = noteCountAtStep(track, candidate);
    for (uint8_t voice = 0; voice < voiceCount; voice++) {
      if (pendingDeleteNote(track, candidate, voice)) continue;
      if (!monoMeasureRowsView(track) && visualNoteAtVoice(track, candidate, voice) != rowNote) continue;
      int32_t logicalHead = (int32_t)((uint32_t)candidate * TICKS_PER_STEP) +
                            logicalMicroTick(track, candidate, voice);
      bool headIntersects = logicalHead >= (int32_t)pixelStart && logicalHead < (int32_t)pixelEnd;
      // 1B2/1C2 : la grille de notes ne represente que les tetes logiques.
      // TIM reste audible mais ne peut jamais fabriquer un deuxieme pad-note.
      if (headIntersects) {
        uint32_t distance = logicalHead >= (int32_t)centre
                              ? (uint32_t)(logicalHead - (int32_t)centre)
                              : (uint32_t)((int32_t)centre - logicalHead);
        if (found && distance >= bestDistance) continue;
        sourceStep = candidate;
        if (sourceVoice) *sourceVoice = voice;
        found = true;
        bestDistance = distance;
      }
    }
  }
  return found;
}

uint16_t noteHeadCountAtPad(uint8_t track, uint8_t rowNote, uint8_t neoIndex, uint8_t pad) {
  if (track >= TRACK_COUNT) return 0;
  uint32_t pixelStart, pixelEnd;
  padTimelineBounds(neoIndex, pad, pixelStart, pixelEnd);
  uint16_t heads = 0;
  for (uint16_t step = 0; step < trackStepCount(track); step++) {
    for (uint8_t voice = 0; voice < noteCountAtStep(track, step); voice++) {
      if (!monoMeasureRowsView(track) && visualNoteAtVoice(track, step, voice) != rowNote) continue;
      int32_t logicalHead = (int32_t)step * TICKS_PER_STEP + logicalMicroTick(track, step, voice);
      if (logicalHead >= (int32_t)pixelStart && logicalHead < (int32_t)pixelEnd) heads++;
    }
  }
  return heads;
}

uint16_t deleteNoteHeadsAtPad(uint8_t track, uint8_t rowNote, uint8_t neoIndex, uint8_t pad) {
  if (track >= TRACK_COUNT) return 0;
  uint32_t pixelStart, pixelEnd;
  padTimelineBounds(neoIndex, pad, pixelStart, pixelEnd);
  uint16_t removed = 0;
  for (int step = (int)trackStepCount(track) - 1; step >= 0; step--) {
    for (int voice = (int)noteCountAtStep(track, (uint16_t)step) - 1; voice >= 0; voice--) {
      if (!monoMeasureRowsView(track) &&
          visualNoteAtVoice(track, (uint16_t)step, (uint8_t)voice) != rowNote) continue;
      int32_t logicalHead = step * (int32_t)TICKS_PER_STEP +
                            logicalMicroTick(track, (uint16_t)step, (uint8_t)voice);
      if (logicalHead < (int32_t)pixelStart || logicalHead >= (int32_t)pixelEnd) continue;
      rememberDeletedNote(track, (uint16_t)step, (uint8_t)voice);
      removeVoiceAtStep(track, (uint16_t)step, (uint8_t)voice);
      removed++;
    }
  }
  return removed;
}

void ensurePitchVisible(uint8_t note) {
  if (noteViewForTrack(selectedTrack) == NOTE_VIEW_COMPACT) {
    uint8_t entries[130];uint8_t count=collectCompactPitchEntries(selectedTrack,entries);
    for(uint8_t i=0;i<count;++i)if(entries[i]==note){compactRowOffset=clampInt((int)i-1,0,max(0,(int)count-4));break;}
    compactRecenter08=false;rebuildCompactPitchRows();
    return;
  }
  if (note < chromaticBottomNote) chromaticBottomNote = note;
  else if (note > chromaticBottomNote + 3) chromaticBottomNote = note - 3;
}

void padToSlotStep(uint8_t neoIndex, uint8_t pad, uint8_t &slot, uint16_t &step) {
  slot = 0;
  uint32_t pixelStart;
  uint32_t pixelEnd;
  padTimelineBounds(neoIndex, pad, pixelStart, pixelEnd);
  int16_t micro;
  tickToStepAndMicro(selectedTrack, pixelStart, step, micro);
}

void clearStepSelection() {
  moveFinish41(true);
  memset(selectedBaseMask, 0, sizeof(selectedBaseMask));
  memset(selectedFineNote, 0, sizeof(selectedFineNote));
  selectedEmptyCount = 0;
  explicitNoteSelection = false;
  for (uint8_t t = 0; t < TRACK_COUNT; t++) {
    for (uint16_t s = 0; s < STEP_COUNT; s++) {
      stepSelected[t][s] = false;
    }
  }
  allTrackNotesSelected = false;
  selectionPitchValid = false;
}

void clearPatternSelection() {
  selectionIndicatorsPending931=true;
  pendingPatternGroove = -1;
  memset(patternSelected, 0, sizeof(patternSelected));
  patternSelectionAnchorValid = false;
  neoDisplayDirty = true;
}

uint16_t selectedPatternCount() {
  uint16_t count = 0;
  for (uint8_t track = 0; track < TRACK_COUNT; track++) {
    for (uint8_t pattern = 0; pattern < PATTERN_COUNT; pattern++) {
      if (patternSelected[track][pattern]) count++;
    }
  }
  return count;
}

uint8_t selectedPatternCountOnTrack(uint8_t track) {
  if (track >= TRACK_COUNT) return 0;
  uint8_t count = 0;
  for (uint8_t pattern = 0; pattern < PATTERN_COUNT; pattern++) {
    if (patternSelected[track][pattern]) count++;
  }
  return count;
}

uint8_t selectedPatternTrackCount() {
  uint8_t count = 0;
  for (uint8_t track = 0; track < TRACK_COUNT; track++) {
    if (selectedPatternCountOnTrack(track) > 0) count++;
  }
  return count;
}

bool canAddPatternSelection(uint8_t track) {
  uint16_t total = selectedPatternCount();
  if (total == 0) return true;
  uint8_t tracks = selectedPatternTrackCount();
  uint8_t onTrack = selectedPatternCountOnTrack(track);
  if (tracks == 1) {
    uint8_t onlyTrack = 0;
    while (onlyTrack < TRACK_COUNT && selectedPatternCountOnTrack(onlyTrack) == 0) onlyTrack++;
    if (onlyTrack == track) return true;       // plusieurs patterns sur une seule piste
    return total == 1 && onTrack == 0;         // passage vers un pattern par piste
  }
  return onTrack == 0;                         // deja multi-pistes : un seul par piste
}

void togglePatternSelection(uint8_t track, uint8_t pattern) {
  selectionIndicatorsPending931=true;
  if (track >= TRACK_COUNT || pattern >= PATTERN_COUNT) return;
  if (patternSelected[track][pattern]) {
    patternSelected[track][pattern] = false;
  } else {
    patternSelected[track][pattern] = true;
  }
  selectedTrack = track;
  displayedPattern = pattern + 1;
  neoDisplayDirty = true;
}

void selectPatternRange(uint8_t track, uint8_t first, uint8_t last) {
  if (track >= TRACK_COUNT || first >= PATTERN_COUNT || last >= PATTERN_COUNT) return;
  clearPatternSelection();
  uint8_t lo = min(patternRow51(track,first), patternRow51(track,last));
  uint8_t hi = max(patternRow51(track,first), patternRow51(track,last));
  for (uint8_t row = lo; row <= hi; row++) patternSelected[track][patternAtRow51(track,row)] = true;
  selectedTrack = track;
  displayedPattern = last + 1;
  neoDisplayDirty = true;
}

void selectAllCurrentTrackNotes() {
  clearStepSelection();
  uint8_t firstPitch = 255;
  for (uint16_t step = 0; step < trackStepCount(selectedTrack); step++) {
    if (!stepActive[selectedTrack][step]) continue;
    if (firstPitch == 255) firstPitch = noteAtVoice(selectedTrack, step, 0);
    explicitNoteSelection = true;
    for (uint8_t voice = 0; voice < noteCountAtStep(selectedTrack, step); voice++)
      setNoteObjectSelected(selectedTrack, step, voice, true);
  }
  if (firstPitch != 255) {
    allTrackNotesSelected = false; // explicit masks include every selected voice
    selectionPitch = firstPitch;
    selectionPitchValid = true;
    selectionMode = true;
    pitchEncoderSelectMode = false;
    DISPLAY_SERIAL.println(F("FIELD,2,T,NOT"));
    DISPLAY_SERIAL.println(F("MSG|ALL TRACK NOTES SELECTED"));
  }
  neoDisplayDirty = true;
}

void selectAllCurrentTrackPatterns() {
  clearPatternSelection();
  for (uint8_t pattern = 0; pattern < PATTERN_COUNT; pattern++) {
    patternSelected[selectedTrack][pattern] = true;
  }
  DISPLAY_SERIAL.println(F("MSG|ALL TRACK PATTERNS SELECTED"));
  neoDisplayDirty = true;
}

bool hasSelectedSteps() {
  for (uint8_t t = 0; t < TRACK_COUNT; t++) {
    for (uint16_t s = 0; s < STEP_COUNT; s++) {
      if (stepSelected[t][s]) return true;
    }
  }
  return false;
}

bool selectionHasActiveNotes() {
  for (uint8_t track = 0; track < TRACK_COUNT; track++) {
    for (uint16_t step = 0; step < STEP_COUNT; step++) {
      if (selectedRangeVoice(track, step) >= 0) return true;
    }
  }
  return false;
}

bool leaveStepSelection(bool commitChanges) {
  moveFinish41(commitChanges);
  lockHoldCancel26();
  if (deleteButtonActive) return false;
  if (!hasSelectedSteps() && selectedStep < 0 && !selectionMode && !selectionAnchorValid &&
      !pendingSelectionEdit.active && !selectButtonActive && !shiftButtonActive &&
      selectedPatternCount() == 0) return false;
  if (commitChanges) {
    grooveListFinish40();
    if(!freeDraftFinish25(true))return false;
    if (pendingPatternGroove >= 0) for (uint8_t t = 0; t < TRACK_COUNT; t++) for (uint8_t p = 0; p < PATTERN_COUNT; p++)
      if (patternSelected[t][p]) patternGroove[t][p] = (uint8_t)pendingPatternGroove;
    commitPendingSelectionEdit();
    if (pendingSelectionEdit.active) return false;
  } else discardPendingSelectionEdit();
  freeDraftFinish25(false);
  clearStepSelection();
  clearPatternSelection();
  grooveStepMask=0;
  selectButtonActive = shiftButtonActive = columnSelectionMode = false;
  rectangleArmed = false; selection805Reset(); selection805SongState();
  memset(selectPadPending, 0, sizeof(selectPadPending));
  selectionMode = false;
  updateRoySelectionIndicators();
  sendSelectionIndicatorsToPicran();
  selectionAnchorValid = false;
  selectionRangeMade = false;
  selectionPitchValid = false;
  selectedStep = -1;
  pitchEncoderSelectMode = true;
  DISPLAY_SERIAL.println(F("FIELD,2,T,SEL"));
  clearSelectionEditDisplay();
  neoDisplayDirty = true;
  lastNeoRefreshMs = millis() - NEO_MIN_REFRESH_MS;
  displayValueDirty = true;
  displayBpmDirty = true;
  return true;
}

FLASHMEM bool cancelStepSelection() {
  return leaveStepSelection(true);
}

FLASHMEM bool cancelStepSelectionWithoutCommit() {
  if(grooveEditMode){grooveFinish60(false);grooveStepMask=grooveListSelection805=0;selection805Changed();return true;}
  return leaveStepSelection(false);
}

void moveStepSelection(int32_t rawDelta) {
  if (rawDelta == 0 || !hasSelectedSteps()) return;

  uint16_t minStep = STEP_COUNT;
  uint16_t maxStep = 0;
  for (uint8_t track = 0; track < TRACK_COUNT; track++) {
    for (uint16_t step = 0; step < STEP_COUNT; step++) {
      if (!stepSelected[track][step]) continue;
      if (step < minStep) minStep = step;
      if (step > maxStep) maxStep = step;
    }
  }
  if (minStep >= STEP_COUNT) return;

  int requested = rawDelta > 0 ? (int)rawDelta : -(int)(-rawDelta);
  int shift = requested;
  if ((int)minStep + shift < 0) shift = -(int)minStep;
  if ((int)maxStep + shift >= STEP_COUNT) shift = STEP_COUNT - 1 - maxStep;
  if (shift == 0) return;

  bool moved[TRACK_COUNT][STEP_COUNT] = {};
  for (uint8_t track = 0; track < TRACK_COUNT; track++) {
    for (uint16_t step = 0; step < STEP_COUNT; step++) {
      if (stepSelected[track][step]) moved[track][(int)step + shift] = true;
    }
  }
  memcpy(stepSelected, moved, sizeof(stepSelected));

  if (selectedStep >= 0) selectedStep = (int16_t)clampInt((int)selectedStep + shift, 0, STEP_COUNT - 1);
  neoDisplayDirty = true;
  displayValueDirty = true;
}

FLASHMEM void deleteStepSelection() {
  if (!selectionHasActiveNotes()) return;
  allNotesOff();
  discardPendingSelectionEdit();
  for (uint8_t track = 0; track < TRACK_COUNT; track++) {
    for (uint16_t step = 0; step < STEP_COUNT; step++) {
      if (!stepSelected[track][step]) continue;
      for (int16_t voice = (int16_t)noteCountAtStep(track, step) - 1; voice >= 0; voice--) {
        if (!selectedNoteObject(track, step, (uint8_t)voice)) continue;
        rememberDeletedNote(track, step, (uint8_t)voice);
        removeVoiceAtStep(track, step, (uint8_t)voice);
      }
    }
  }
  leaveStepSelection(false);
  DISPLAY_SERIAL.println(F("MSG|SELECTION DELETED"));
}

void toggleSelectionMode() {
  if (selectionMode) commitPendingSelectionEdit();
  selectionMode = !selectionMode;
  selectionAnchorValid = false;
  selectionRangeMade = false;
  if (!selectionMode) selectionPitchValid = false;
  if (!selectionMode) {
    clearStepSelection();
    pitchEncoderSelectMode = true;
    DISPLAY_SERIAL.println(F("FIELD,2,T,SEL"));
  }
  clearSelectionEditDisplay();
  neoDisplayDirty = true;
  displayValueDirty = true;
  displayBpmDirty = true;
}

void selectStepRange(uint8_t track, uint16_t a, uint16_t b) {
  if (track >= TRACK_COUNT) return;
  if (a >= STEP_COUNT || b >= STEP_COUNT) return;
  if (pendingSelectionEdit.active || (lockDraftAny26()&&!selectButtonActive)) commitPendingSelectionEdit();
  uint16_t lo = a < b ? a : b;
  uint16_t hi = a < b ? b : a;
  for (uint16_t s = lo; s <= hi; s++) stepSelected[track][s] = true;
  selectedTrack = track;
  selectedStep = b;
  if (selectionHasActiveNotes()) {
    pitchEncoderSelectMode = false;
    DISPLAY_SERIAL.println(F("FIELD,2,T,NOT"));
  }
  clearSelectionEditDisplay();
  ensureSelectedTrackVisible();
  neoDisplayDirty = true;
  lastNeoRefreshMs = millis() - NEO_MIN_REFRESH_MS;
  displayValueDirty = true;
}

void selectStepOnly(uint8_t track, uint16_t step, uint8_t voice = 0) {
  if (track >= TRACK_COUNT || step >= STEP_COUNT) return;
  if (pendingSelectionEdit.active &&
      (pendingSelectionEdit.track != track || pendingSelectionEdit.step != (int16_t)step ||
       pendingSelectionEdit.voice != voice)) commitPendingSelectionEdit();
  selectedTrack = track;
  selectedStep = step;
  selectedVoice = min(voice, (uint8_t)(noteCountAtStep(track, step) ? noteCountAtStep(track, step) - 1 : 0));
  selectedFineIndex = fineIndexForVoice(track, step, selectedVoice);
  if (stepActive[track][step]) {
    pitchEncoderSelectMode = false;
    DISPLAY_SERIAL.println(F("FIELD,2,T,NOT"));
  }
  selectionPitchValid = false;
  clearSelectionEditDisplay();
  ensureSelectedTrackVisible();
  if (stepActive[track][step]) ensurePitchVisible(visualNoteAtVoice(track, step, selectedVoice));

  if (DEBUG_SERIAL) {
    Serial.print(F("STEP SELECT T"));
    Serial.print(humanTrack(track));
    Serial.print(F(" STEP "));
    Serial.println(step + 1);
  }

  neoDisplayDirty = true;
  lastNeoRefreshMs = millis() - NEO_MIN_REFRESH_MS;
  displayValueDirty = true;
}

void toggleNotePadSelection(uint8_t track, uint16_t step, uint8_t voice) {
  if (track >= TRACK_COUNT || step >= STEP_COUNT || !stepActive[track][step]) return;
  if (!explicitNoteSelection) { clearStepSelection(); explicitNoteSelection = true; }
  bool selected = !selectedNoteObject(track, step, voice);
  if (!selected && selectButtonActive) exileNoteObject(track, step, voice);
  else setNoteObjectSelected(track, step, voice, selected);
  selectionMode = true;
  selectionPitch = noteAtVoice(track, step, voice);
  selectionPitchValid = true;
  selectedTrack = track;
  selectedStep = selected ? (int16_t)step : -1;
  selectedVoice = voice;
  pitchEncoderSelectMode = !selectionHasActiveNotes();
  DISPLAY_SERIAL.println(F("FIELD,2,T,NOT"));
  neoDisplayDirty = displayValueDirty = true;
}

void shiftSelectNotePad(uint8_t track, uint16_t step, uint8_t voice) {
  if (track >= TRACK_COUNT || step >= STEP_COUNT) return;
  uint8_t pitch = noteAtVoice(track, step, voice);
  if (!selectionAnchorValid || selectionAnchorTrack != track) {
    selectionAnchorValid = true;
    selectionAnchorTrack = track;
    selectionAnchorStep = step;
  }
  explicitNoteSelection = true;
  uint16_t lo = min(selectionAnchorStep, step), hi = max(selectionAnchorStep, step);
  for (uint16_t cell = lo; cell <= hi; cell++) {
    for (uint8_t v = 0; v < noteCountAtStep(track, cell); v++) {
      if (noteAtVoice(track, cell, v) == pitch) setNoteObjectSelected(track, cell, v, true);
    }
    if (findVoiceAtStep(track, cell, pitch) < 0 && !selectedEmptyCell(track, cell, pitch))
      toggleEmptyCellSelection(track, cell, pitch);
  }
  selectionMode = true;
  selectionPitch = pitch;
  selectionPitchValid = true;
  selectedTrack = track;
  selectedStep = step;
  selectedVoice = voice;
  pitchEncoderSelectMode = !selectionHasActiveNotes();
  neoDisplayDirty = displayValueDirty = true;
}

void shortPressStep(uint8_t neoIndex, uint8_t pad, uint8_t slot, uint16_t step,
                    uint8_t note, int16_t padMicro,
                    bool hitExistingHead, uint8_t hitVoice) {
  if (slot > 3 || step >= STEP_COUNT) return;
  uint8_t t = visibleTrackForSlot(slot);
  if (t >= TRACK_COUNT) return;
  discardNoteRedo();

  int16_t timeOffset = defaultMicroTick[t];
  int8_t existingVoice = hitExistingHead ? (int8_t)hitVoice
                                         : findVoiceAtExactTick(t, step, note, padMicro);
  if (!hitExistingHead && existingVoice >= 0) return; // Empty row must not duplicate or delete an existing NOT.
  lastGridTick = (uint32_t)step * TICKS_PER_STEP + padMicro;
  lastGridTickValid = true;

  if (hitExistingHead && selectedTrack == t && selectedStep == (int16_t)step &&
      selectedVoice == hitVoice) {
    commitPendingSelectionEdit();
    selectedStep = -1;
    selectedFineIndex = -1;
  } else if (hitExistingHead) {
    commitPendingSelectionEdit();
    deleteNoteHeadsAtPad(t, note, neoIndex, pad);
    lastNoteValid = false;
    if (selectedTrack == t && selectedStep == (int16_t)step) selectedStep = -1;
  } else if (stepActive[t][step] && trackPlayMode[linkOwner12(t,LINK_POLY12)] == TRACK_PLAY_MONO &&
             logicalMicroTick(t, step, 0) == padMicro) {
    // 1B1 : en MONO, seule une note posee sur le meme tick remplace la hauteur.
    commitPendingSelectionEdit();
    lockPitch817(t,step,0,note);stepMidiNote[t][step] = note;
    stepLockPitch[t][step] = true;kbStepR10(t,step,0);
    stepExtraNoteCount[t][step] = 0;
    lastNoteTrack = t;
    lastNoteStep = step;
    lastNoteFineIndex = -1;
    lastNoteValid = true;
    selectedStep = -1;
    selectedVoice = 0;
  } else if (stepActive[t][step] && trackPlayMode[linkOwner12(t,LINK_POLY12)] != TRACK_PLAY_MONO &&
             logicalMicroTick(t, step, 0) == padMicro && existingVoice < 0) {
    if (addNoteAtStep(t, step, note)) {
      dismissSdNoticeAfterNoteCreation();
      selectedTrack = t;
      selectedVoice = stepExtraNoteCount[t][step];kbStepR10(t,step,selectedVoice);
      lastNoteTrack = t;
      lastNoteStep = step;
      lastNoteFineIndex = -1;
      lastNoteValid = true;
      if (autoSelectLastNoteForTrack(t)) selectStepOnly(t, step, selectedVoice);
      else selectedStep = -1;
    }
  } else if (stepActive[t][step]) {
    // Autre tick de la meme cellule : evenement independant, y compris MONO.
    int16_t fine = addFineNote(t, step, timeOffset, note, padMicro);
    if (fine >= 0) {
      dismissSdNoticeAfterNoteCreation();
      uint8_t voice = voiceForFineIndex(t, step, (uint8_t)fine);kbStepR10(t,step,voice);
      lastNoteTrack = t;
      lastNoteStep = step;
      lastNoteFineIndex = fine;
      lastNoteValid = true;
      selectedFineIndex = fine;
      if (autoSelectLastNoteForTrack(t)) selectStepOnly(t, step, voice);
      else selectedStep = -1;
    }
  } else {
    clearTrackStep(t, step);
    grooveFirstNote26(t,step);
    stepActive[t][step] = true;
    dismissSdNoticeAfterNoteCreation();
    stepVel[t][step] = indNote50(t,1);
    stepMidiNote[t][step] = note;
    stepExtraNoteCount[t][step] = 0;
    stepLockPitch[t][step] = true;
    stepGateTicks[t][step] = indNote50(t,2);
    stepLogicalMicro[t][step] = (int8_t)padMicro;
    stepMicroTick[t][step] = indNote50(t,3);
    stepLockVel[t][step]=stepLockGate[t][step]=stepLockTime[t][step]=true;kbStepR10(t,step,0);
    lastNoteTrack = t;
    lastNoteStep = step;
    lastNoteFineIndex = -1;
    lastNoteValid = true;
    selectedTrack = t;
    ensureSelectedTrackVisible();
    if (autoSelectLastNoteForTrack(t)) {
      selectStepOnly(t, step, 0);
    } else {
      // Comportement par defaut : poser une note laisse l'edition sur MAIN.
      selectedStep = -1;
    }

    if (DEBUG_SERIAL) {
      Serial.print(F("STEP ON  T"));
      Serial.print(humanTrack(t));
      Serial.print(F(" STEP "));
      Serial.print(step + 1);
      Serial.print(F(" VEL "));
      Serial.print(effectiveVel(t, step));
      Serial.print(F(" NOTE "));
      Serial.print(effectiveMidiNote(t, step));
      Serial.print(F(" GATE "));
      Serial.print(effectiveGateTicks(t, step));
      Serial.print(F(" TIME "));
      Serial.println(effectiveMicroTick(t, step));
    }
  }

  neoDisplayDirty = true;
  displayValueDirty = true;
}

void handleSelectionPadEdge(uint8_t track, uint16_t step, uint8_t edge) {
  if (track >= TRACK_COUNT || step >= STEP_COUNT) return;

  if (edge == SEESAW_KEYPAD_EDGE_RISING) {
    if (!selectionAnchorValid) {
      selectionAnchorValid = true;
      selectionAnchorTrack = track;
      selectionAnchorStep = step;
      selectionRangeMade = false;
      selectionPitch = activePitchRow <= 127 ? activePitchRow : defaultMidiNote[track];
      selectionPitchValid = true;
      stepSelected[track][step] = true;
      selectedTrack = track;
      selectedStep = step;
      ensureSelectedTrackVisible();
      neoDisplayDirty = true;
      displayValueDirty = true;
      return;
    }

    if (selectionAnchorTrack == track && selectionAnchorStep != step) {
      selectStepRange(track, selectionAnchorStep, step);
      selectionRangeMade = true;
    }
    return;
  }

  if (edge == SEESAW_KEYPAD_EDGE_FALLING) {
    if (selectionAnchorValid && selectionAnchorTrack == track && selectionAnchorStep == step) {
      selectionAnchorValid = false;
      selectionRangeMade = false;
    }
  }
}

bool tryDirectLineSelection(uint8_t neoIndex, uint8_t pad, uint8_t slot, uint16_t step,
                            uint8_t track) {
  for (uint8_t otherNeo = 0; otherNeo < NEO_COUNT; otherNeo++) {
    for (uint8_t otherPad = 0; otherPad < 16; otherPad++) {
      if (otherNeo == neoIndex && otherPad == pad) continue;
      if (!neoPadDown[otherNeo][otherPad]) continue;
      uint8_t otherSlot;
      uint16_t otherStep;
      padToSlotStep(otherNeo, otherPad, otherSlot, otherStep);
      if (otherSlot != slot || otherStep == step) continue;

      for (uint8_t clearTrack = 0; clearTrack < TRACK_COUNT; clearTrack++) {
        for (uint16_t clearStep = 0; clearStep < STEP_COUNT; clearStep++) {
          stepSelected[clearTrack][clearStep] = false;
        }
      }
      selectStepRange(track, otherStep, step);
      selectionPitch = padChromaticNote(pad);
      selectionPitchValid = true;
      neoPadEdited[neoIndex][pad] = true;
      neoPadEdited[otherNeo][otherPad] = true;
      selectionMode = false;
      selectionAnchorValid = false;
      return true;
    }
  }
  return false;
}

void reportDebugNeo(uint8_t neoIndex, uint8_t pad, bool pressed) {
  uint8_t row = pad / 4;
  uint8_t column = pad % 4;
  char line0[21], line1[21], line2[21], small0[17], small1[17];
  snprintf(line0, sizeof(line0), "NEO TEST N%u PAD%02u", neoIndex + 1, pad + 1);
  snprintf(line1, sizeof(line1), "ADDR 0x%02X", NEO_ADDR[neoIndex]);
  snprintf(line2, sizeof(line2), "ROW%u COL%u RAW%u", row, column, pad);
  snprintf(small0, sizeof(small0), "N%u P%02u 0x%02X", neoIndex + 1, pad + 1, NEO_ADDR[neoIndex]);
  snprintf(small1, sizeof(small1), "R%u C%u %s", row, column, pressed ? "ON" : "OFF");
  sendDebugDisplay(line0, line1, line2, "BE1 BACK",
                   small0, small1);

  Serial.print(F("DBG,NEO,N=")); Serial.print(neoIndex + 1);
  Serial.print(F(",ADDR=0x")); Serial.print(NEO_ADDR[neoIndex], HEX);
  Serial.print(F(",PAD=")); Serial.print(pad + 1);
  Serial.print(F(",RAW=")); Serial.print(pad);
  Serial.print(F(",ROW=")); Serial.print(row);
  Serial.print(F(",COL=")); Serial.print(column);
  Serial.print(F(",PRESSED=")); Serial.println(pressed ? 1 : 0);
}

bool noteHeadInZone(uint8_t track, uint16_t step, uint8_t voice,
                    uint8_t rowNote, uint32_t start, uint32_t end) {
  if (!monoMeasureRowsView(track) && visualNoteAtVoice(track, step, voice) != rowNote) return false;
  int32_t head = (int32_t)step * TICKS_PER_STEP + logicalMicroTick(track, step, voice);
  return head >= (int32_t)start && head < (int32_t)end;
}

void selectOrDeletePadZone(uint8_t neoIndex, uint8_t pad, bool deleting) {
  uint8_t slot; uint16_t padStep;
  padToSlotStep(neoIndex, pad, slot, padStep);
  uint8_t track = visibleTrackForSlot(slot), pitch = padChromaticNote(pad);
  if (track >= TRACK_COUNT || pitch > 127 || padStep >= trackStepCount(track)) return;
  uint32_t start, end;
  padTimelineBounds(neoIndex, pad, start, end);
  bool found = false, anySelected = false;
  for (uint16_t step = 0; step < trackStepCount(track); step++) {
    for (uint8_t voice = 0; voice < noteCountAtStep(track, step); voice++) {
      if (!noteHeadInZone(track, step, voice, pitch, start, end)) continue;
      found = true;
      anySelected |= selectedNoteObject(track, step, voice);
    }
  }
  if (!found) {
    if(deleting&&(!freeAtPad11(track,pitch,start,end)||freeTail11(track,pitch,start,end)))return;
    toggleEmptyCellSelection(track,padStep,pitch);for(uint16_t k=0;k<selectedEmptyCount;++k)if(selectedEmptyCells[k].track==track&&selectedEmptyCells[k].step==padStep&&selectedEmptyCells[k].pitch==pitch)selectedEmptyCells[k].tick=freeTick11(track,pitch,start,end);
    return;
  }
  explicitNoteSelection = true;
  for (uint16_t step = 0; step < trackStepCount(track); step++) {
    for (uint8_t voice = 0; voice < noteCountAtStep(track, step); voice++) {
      if (noteHeadInZone(track, step, voice, pitch, start, end)) {
        if (!deleting && anySelected) exileNoteObject(track, step, voice);
        else setNoteObjectSelected(track, step, voice, true);
      }
    }
  }
  selectedTrack = track;
  selectedStep = -1;
  selectionPitch = pitch;
  // Selection is always silent; pitch browsing keeps its own audition path.
  selectionPitchValid = !deleting;
  pitchEncoderSelectMode = !selectionHasActiveNotes();
  neoDisplayDirty = displayValueDirty = true;
}

// Commit a single object without consuming the shared draft of the group.
FLASHMEM void commitSelectedObject(uint8_t track, uint16_t step, uint8_t voice) {
  if (pendingSelectionEdit.active) {
    for (uint8_t m = 0; m < EDIT_MODE_COUNT; m++) {
      if (!pendingSelectionEdit.field[m].touched) continue;
      EditMode mode = (EditMode)m;
      int base = mode == EDIT_VEL ? effectiveVel(track, step, voice)
        : mode == EDIT_PITCH ? noteAtVoice(track, step, voice)
        : mode == EDIT_GATE ? effectiveGateTicks(track, step, voice) : effectiveMicroTick(track, step, voice);
      setCommittedStepValue(track, step, voice, mode, pendingEditedValue(track, step, voice, mode, base));
    }
    TrackRollSettings roll = pendingDisplayedRoll(track, step, voice, configuredRollSettings(track, step, voice));
    int16_t fine = fineIndexForVoice(track, step, voice);
    for (uint8_t f = 0; f < 6; f++) {
      if (!pendingSelectionEdit.roll[f].touched) continue;
      if (fine >= 0) {
        setRollFieldValue(fineNote[track][fine].roll, f, rollFieldValue(roll, f));
        fineNote[track][fine].pxMask |= (uint16_t)(1u << (PX_ROLL_SHIFT + f));
      } else {
        {auto value906=rollRead906(track,step);setRollFieldValue(value906, f, rollFieldValue(roll, f));rollSet906(track,step,value906,stepRollLockMask[track][step]|(1u << f));}
      }
    }
  }
  setNoteObjectSelected(track, step, voice, false);
}

FLASHMEM void validateSelectedPad(uint8_t neoIndex, uint8_t pad) {
  uint8_t slot; uint16_t padStep;
  padToSlotStep(neoIndex, pad, slot, padStep);
  uint8_t track = visibleTrackForSlot(slot), pitch = padChromaticNote(pad);
  if (track >= TRACK_COUNT || pitch > 127) return;
  if (pendingSelectionEdit.active && !prepareSelectedChordVoices()) return;
  uint32_t start, end; padTimelineBounds(neoIndex, pad, start, end);
  for (uint16_t step = 0; step < trackStepCount(track); step++) {
    uint8_t count = noteCountAtStep(track, step);
    bool targets[FINE_EVENTS_PER_TRACK + MAX_NOTES_PER_STEP] = {};
    for (uint8_t v = 0; v < count; v++) targets[v] = selectedNoteObject(track, step, v)
      && noteHeadInZone(track, step, v, pitch, start, end);
    for (uint8_t v = 0; v < count; v++) if (targets[v]) commitSelectedObject(track, step, v);
  }
  selectedStep = -1;
  scheduleConfiguredRollApply();
  neoDisplayDirty = displayValueDirty = true;
  DISPLAY_SERIAL.println(F("MSG|NOTE VALIDATED"));
}

bool trackSelectContext817(){return debugTestMode==DEBUG_TEST_NONE&&!itemKind809&&!itemChoosing809&&!patternMode&&!songMode&&(modUi805||patchBank808||patchUi808==1);}
bool trackContext815(){
  return debugTestMode==DEBUG_TEST_NONE&&!menuNavigationOnly()&&!patternMode&&!songMode&&!remixMode&&!grooveEditMode&&!modUi805&&!patchUi808&&!patchBank808&&!numericEntryActive;
}
uint16_t inputContext815(){
  return (playing?1:0)|(selectButtonActive?2:0)|(deleteButtonActive?4:0)|(screenUiAwaitRelease?8:0)|(menuNavigationOnly()?16:0)|(patternMode?32:0)|(modUi805?64:0)|(patchUi808?128:0)|(kyBtnDown[0]?256:0)|((uint16_t)selectedTrack<<12);
}

bool stepContext25(){return !patternMode&&!grooveEditMode&&!remixMode&&!songMode&&!keyboardEnabled823&&!quickView903&&!trackMenuActive&&!modUi805&&!patchUi808&&!menuNavigationOnly()&&!controller940&&debugTestMode==DEBUG_TEST_NONE;}
FLASHMEM void onPadEdge(uint8_t neoIndex, uint8_t pad, uint8_t edge) {
 if(neoIndex<4&&pad<16){uint64_t bit=1ULL<<(neoIndex*16+pad);if(edge==SEESAW_KEYPAD_EDGE_RISING)ukPads60|=bit;else if(edge==SEESAW_KEYPAD_EDGE_FALLING)ukPads60&=~bit;}
 if(neoIndex<4&&pad<16&&edge==SEESAW_KEYPAD_EDGE_RISING)presentationTouch43();
 if(edge==SEESAW_KEYPAD_EDGE_RISING)alertAction08();
  if(modRemove806)return;
 if(controller940){if(neoIndex<4&&pad<16&&edge==SEESAW_KEYPAD_EDGE_RISING)controllerEmit940("NEO",neoIndex*4+pad%4,pad/4);return;}
  NeoInputScope932 neoScope;
  selectionIndicatorsPending931=true;
  if(sdBusy905())return;
  if(neoIndex>=4||pad>=16)return;
  if(!pc831.injecting && ((pc831.neoMask&(1ULL<<(neoIndex*16+pad))) || (pc831.neoDown&(1ULL<<(neoIndex*16+pad)))))return;
  InputScope815 input(I815_NEO,neoIndex*16+pad,edge==SEESAW_KEYPAD_EDGE_RISING);

  // Consume the paired release even if KY1 or the UI context changed first.
  if(writeMenuActive){if(edge==SEESAW_KEYPAD_EDGE_RISING){writeTracks41^=1u<<(neoIndex*4+pad%4);neoDisplayDirty=true;}return;}
  if(namePadR8(neoIndex,pad,edge))return;
  if(trackPads815[neoIndex][pad]){
    if(edge==SEESAW_KEYPAD_EDGE_FALLING){trackPads815[neoIndex][pad]=false;neoDisplayDirty=true;neoPadDown[neoIndex][pad]=false;neoPadEdited[neoIndex][pad]=false;}
    return;
  }
  if(recordPad820(neoIndex,pad,edge))return;
  if(delayMixerPad930(neoIndex,pad,edge))return;
  if(trackChord815&&(trackContext815()||trackSelectContext817()||keyboardContext823()||(trackMenuActive&&!fileBrowserMode&&!itemKind809&&!numericEntryActive))&&edge==SEESAW_KEYPAD_EDGE_RISING){
    trackPads815[neoIndex][pad]=true;
    uint8_t target=neoIndex*4+pad%4;
    trackAction818(pad/4,target);
    return;
  }
  mod805Invalidate();
  if(stepContext25())pad=stepLogical25(pad);
  if(grooveEditMode&&patternMode&&!deleteButtonActive){
    uint8_t t=neoIndex*4+pad%4,p=patternAtRow51(t,patternRowOffset+pad/4);
    neoPadDown[neoIndex][pad]=edge==SEESAW_KEYPAD_EDGE_RISING;
    if(edge==SEESAW_KEYPAD_EDGE_RISING&&t<16&&p<16)grooveListPad40(t,p);
    neoPadEdited[neoIndex][pad]=false;return;
  }
  if (selection805Pad(neoIndex, pad, edge)) return;
  if (songPad(neoIndex,pad,edge)) return;
  if (remixPad(neoIndex,pad,edge)) return;
  if (debugTestMode == DEBUG_TEST_NEO) {
    bool pressed = edge == SEESAW_KEYPAD_EDGE_RISING;
    neoPadDown[neoIndex][pad] = pressed;
    debugNeoLastIndex = neoIndex;
    debugNeoLastPad = pad;
    reportDebugNeo(neoIndex, pad, pressed);
    neoDisplayDirty = true;
    return;
  }
  if (debugTestMode != DEBUG_TEST_NONE) return;
  if (menuNavigationOnly()) return;
  if (grooveEditMode && !patternMode) {
    neoPadDown[neoIndex][pad] = edge == SEESAW_KEYPAD_EDGE_RISING;
    if (edge == SEESAW_KEYPAD_EDGE_RISING) {
      neoPadEdited[neoIndex][pad] = true;
      if (pad < 4) { grooveStepMask ^= (uint16_t)(1u << (neoIndex * 4 + pad)); grooveFeedbackField = -1; }
      sendGroovePacket(); neoDisplayDirty = displayValueDirty = true;
    } else neoPadEdited[neoIndex][pad] = false;
    return;
  }
  if (edge == SEESAW_KEYPAD_EDGE_FALLING && selectPadPending[neoIndex][pad]) {
    selectPadPending[neoIndex][pad] = false;
    if (selectButtonActive) selectOrDeletePadZone(neoIndex, pad, false);
  }
  if (edge == SEESAW_KEYPAD_EDGE_FALLING && neoPadEdited[neoIndex][pad]) {
    neoPadDown[neoIndex][pad] = false;
    neoPadEdited[neoIndex][pad] = false;
    return;
  }
  if (deleteButtonActive) {
    neoPadDown[neoIndex][pad] = edge == SEESAW_KEYPAD_EDGE_RISING;
    if (edge == SEESAW_KEYPAD_EDGE_RISING) {
      neoPadEdited[neoIndex][pad] = true;
      if (deletePatternContext) {
        uint8_t track = neoIndex * 4 + pad % 4;
        uint8_t pattern = patternAtRow51(track,patternRowOffset + pad / 4);
        if (track < TRACK_COUNT && pattern < PATTERN_COUNT) patternSelected[track][pattern] = true;
      } else {
        selectOrDeletePadZone(neoIndex, pad, true);
      }
      allNotesOff();
      neoDisplayDirty = displayValueDirty = true;
    }
    return;
  }

  if(indPad50(neoIndex,pad,edge))return;
  // MODE PATTERN : 16 pistes de gauche a droite, 4 lignes de patterns.
  if (patternMode && !remixMode) {
    uint8_t localCol = pad % 4;
    uint8_t localRow = pad / 4;
    uint8_t track = neoIndex * 4 + localCol;
    uint8_t pattern = patternAtRow51(track,patternRowOffset + localRow);
    if (track >= TRACK_COUNT || pattern >= PATTERN_COUNT) return;
    if (edge == SEESAW_KEYPAD_EDGE_RISING) {
      if(shiftButtonActive||selectButtonActive||grooveEditMode){
      lastPatternTrack = track; lastPatternIndex = pattern; lastPatternValid = true;
      selectedTrack=track;indEditTrack50=track;indEditPattern50=pattern;
      quickTrack903=track;quickPattern903=pattern;displayedPattern=pattern+1;
      if(trackMenuActive&&trackPage933==4)programMenu939();if(patMenu46==2)patRender46();
      if(quickView903){if(quickView903==3)quickItem903=track*16+delayChosen925[track]*256+pattern;quickGather903();bool found=false;for(unsigned i=0;i<quickCount903;++i)found|=quickOrder903[i]==quickItem903;if(!found){quickItem903=quickCount903?quickOrder903[0]:UINT32_MAX;quickPatchPage903=0;}}
      displayValueDirty=modDisplayDirty810=true;
      }
      neoPadDown[neoIndex][pad] = true;
      neoPadEdited[neoIndex][pad] = shiftButtonActive || selectButtonActive;
      if (shiftButtonActive) {
        if (!patternSelectionAnchorValid || patternSelectionAnchorTrack != track) {
          clearPatternSelection();
          patternSelectionAnchorValid = true;
          patternSelectionAnchorTrack = track;
          patternSelectionAnchorPattern = pattern;
          patternSelected[track][pattern] = true;
        } else {
          selectPatternRange(track, patternSelectionAnchorPattern, pattern);
          patternSelectionAnchorValid = true;
          patternSelectionAnchorTrack = track;
          patternSelectionAnchorPattern = pattern;
        }
        selectedTrack = track;
        displayedPattern = pattern + 1;
        neoDisplayDirty = true;
        return;
      }
      if (selectButtonActive || grooveEditMode) {
        togglePatternSelection(track, pattern);
        if (grooveEditMode) { neoPadEdited[neoIndex][pad] = true; sendGroovePacket(); }
        return;
      }
      patternPasteTargetHeld = true;
      patternPasteTargetConsumed = false;
      patternPasteTargetTrack = track;
      patternPasteTargetPattern = pattern;
      return;
    }
    if (edge == SEESAW_KEYPAD_EDGE_FALLING) {
      neoPadDown[neoIndex][pad] = false;
      if (neoPadEdited[neoIndex][pad]) {
        neoPadEdited[neoIndex][pad] = false;
        return;
      }
      if (patternPasteTargetHeld && patternPasteTargetTrack == track &&
          patternPasteTargetPattern == pattern) {
        if (!patternPasteTargetConsumed) {
          requestTrackPattern(track, pattern);
        }
        patternPasteTargetHeld = false;
        patternPasteTargetConsumed = false;
      }
    }
    return;
  }



  uint8_t slot;
  uint16_t step;
  padToSlotStep(neoIndex, pad, slot, step);
  uint8_t t = visibleTrackForSlot(slot);
  uint8_t note = padChromaticNote(pad);
  if (t >= TRACK_COUNT || step >= trackStepCount(t)) return;
  bool emptyCompactRow20 = noteViewForTrack(t) == NOTE_VIEW_COMPACT && note > 127;
  if (emptyCompactRow20) note = defaultMidiNote[t];
  if (note > 127) return;
  uint32_t pixelStart;
  uint32_t pixelEnd;
  padTimelineBounds(neoIndex, pad, pixelStart, pixelEnd);
  int16_t padMicro = 0;
  uint16_t padStep = step;
  tickToStepAndMicro(t, pixelStart, padStep, padMicro);
  uint16_t headStep = step;
  uint8_t headVoice = 0;
  bool hitExistingHead = !emptyCompactRow20 && visibleNoteAtPad(t, note, neoIndex, pad, headStep, &headVoice, true);
  if (hitExistingHead) {
    step = headStep;
    note = noteAtVoice(t, headStep, headVoice);
  }

  if (edge == SEESAW_KEYPAD_EDGE_RISING) {
    neoPressStartMs[neoIndex][pad] = millis();
    neoPadDown[neoIndex][pad] = true;
    neoPadEdited[neoIndex][pad] = false;
    neoPadCancelSelectionOnly[neoIndex][pad] = false;
    neoPadCreatedOnPress[neoIndex][pad] = false;
    activePitchRow = note;
    activePitchTrack = t;
    bool sameSelectedHead = hitExistingHead && selectedTrack == t &&
                            selectedStep == (int16_t)headStep && selectedVoice == headVoice;
    if (selectButtonActive || shiftButtonActive) {
      neoPadEdited[neoIndex][pad] = true;
      if (hitExistingHead) {
        if (selectButtonActive) selectPadPending[neoIndex][pad] = true;
        else shiftSelectNotePad(t, headStep, headVoice);
      } else if (shiftButtonActive) {
        if (!selectionAnchorValid || selectionAnchorTrack != t) {
          selectionAnchorValid = true;
          selectionAnchorTrack = t;
          selectionAnchorStep = step;
        }
        uint16_t lo = min(selectionAnchorStep, step), hi = max(selectionAnchorStep, step);
        explicitNoteSelection = true;
        for (uint16_t cell = lo; cell <= hi; cell++) {
          int8_t voice = findVoiceAtStep(t, cell, note);
          if (voice >= 0) setNoteObjectSelected(t, cell, (uint8_t)voice, true);
          else if (!selectedEmptyCell(t, cell, note)) toggleEmptyCellSelection(t, cell, note);
        }
        selectedStep = -1;
      } else {toggleEmptyCellSelection(t, step, note);for(uint16_t i=0;i<selectedEmptyCount;++i)if(selectedEmptyCells[i].track==t&&selectedEmptyCells[i].step==step&&selectedEmptyCells[i].pitch==note)selectedEmptyCells[i].tick=freeTick11(t,note,pixelStart,pixelEnd);}
      return;
    }
    if (!selectionMode && !sameSelectedHead &&
        (hasSelectedSteps() || selectedStep >= 0 || pendingSelectionEdit.active)) {
      cancelStepSelection();
      // 1A1 : le meme appui continue et peut selectionner la nouvelle note.
    }
    // La selection par maintien est remplacee par les bascules SHIFT/SELECT.
    // Hors mode selection, une note vide n'est creee qu'au relachement. Cela
    // laisse le premier pad vide disponible pour former une plage avec un
    // second pad maintenu, sans creer de note parasite.
    return;
  }

  if (edge == SEESAW_KEYPAD_EDGE_FALLING) {
    bool wasEdited = neoPadEdited[neoIndex][pad];
    bool cancelOnly = neoPadCancelSelectionOnly[neoIndex][pad];
    neoPadDown[neoIndex][pad] = false;
    neoPadEdited[neoIndex][pad] = false;
    neoPadCancelSelectionOnly[neoIndex][pad] = false;
    neoPadCreatedOnPress[neoIndex][pad] = false;
    if (cancelOnly) return;
    if (!wasEdited) {
      shortPressStep(neoIndex, pad, slot, step, note, padMicro, hitExistingHead, headVoice);
    }
  }
}

void serviceNeoSelectionHolds() {
 if(controller940)return;
  if (songMode) return;
  if (!selectButtonActive || patternMode || menuNavigationOnly()) return;
  for (uint8_t n = 0; n < NEO_COUNT; n++) for (uint8_t p = 0; p < 16; p++) {
    if (!selectPadPending[n][p] || !neoPadDown[n][p]) continue;
    if ((uint32_t)(millis() - neoPressStartMs[n][p]) < SELECT_VALIDATE_HOLD_MS) continue;
    selectPadPending[n][p] = false;
    validateSelectedPad(n, p);
  }
}

TrellisCallback callbackNeo1(keyEvent evt) {
  onPadEdge(0, evt.bit.NUM, evt.bit.EDGE);
  return 0;
}

TrellisCallback callbackNeo2(keyEvent evt) {
  onPadEdge(1, evt.bit.NUM, evt.bit.EDGE);
  return 0;
}

TrellisCallback callbackNeo3(keyEvent evt) {
  onPadEdge(2, evt.bit.NUM, evt.bit.EDGE);
  return 0;
}

TrellisCallback callbackNeo4(keyEvent evt) {
  onPadEdge(3, evt.bit.NUM, evt.bit.EDGE);
  return 0;
}

void registerNeoCallbacks(uint8_t neoIndex) {
  for (uint8_t pad = 0; pad < 16; pad++) {
    neo[neoIndex]->activateKey(pad, SEESAW_KEYPAD_EDGE_RISING, true);
    neo[neoIndex]->activateKey(pad, SEESAW_KEYPAD_EDGE_FALLING, true);

    if (neoIndex == 0) neo[neoIndex]->registerCallback(pad, callbackNeo1);
    if (neoIndex == 1) neo[neoIndex]->registerCallback(pad, callbackNeo2);
    if (neoIndex == 2) neo[neoIndex]->registerCallback(pad, callbackNeo3);
    if (neoIndex == 3) neo[neoIndex]->registerCallback(pad, callbackNeo4);
  }
}

void serviceNeoButtons() {
  uint32_t now = millis();
  if ((uint32_t)(now - lastButtonReadMs) < 5) return;
  lastButtonReadMs = now;

  for (uint8_t n = 0; n < NEO_COUNT; n++) {
    if (!neoAvailable[n]) continue;
    neo[n]->read();
    if(optim31&16)serviceRealtimeBetweenNeoPanels();
  }
}

void serviceRealtimeBetweenNeoPanels() {
  DiagScope814 diag(D814_CATCHUP,pendingClockTicks);
  // Une transmission NeoTrellis est bloquante sur I2C. Rattraper horloge et
  // Note Off entre deux panneaux limite la gigue audible pendant une edition.
  updateTransport();
  serviceNoteOffs();
}

void diagShowNeo814(uint8_t n) {
  if(diagActive814&&diagNeo814!=0)return;
  DiagScope814 diag(D814_TX);
  if(neoPending821[n]){if(neoAvailable[n])neo[n]->pixels.show();neoPending821[n]=false;}
}

#include "GateView09.h"
#include "Playhead24.h"
FLASHMEM void updateNeoPixels() {if(namePixels54())return;if(controller940){for(uint8_t n=0;n<4;++n){if(!(neoAvailable[n]||pc831.active))continue;for(uint8_t p=0;p<16;++p){uint8_t t=n*4+p%4,row=p/4,v=controllerLeds940[t];uint32_t c=0;if(v&1){if(row==0)c=(v&2)?colorFor(n,0,75,85):colorFor(n,0,8,10);if(row==1)c=(v&8)?colorFor(n,80,40,0):colorFor(n,0,10,0);if(row==2)c=(v&4)?colorFor(n,80,65,0):colorFor(n,12,10,0);if(row==3)c=colorFor(n,12,0,18);}neoPixel821(n,p,c);}diagShowNeo814(n);}return;} if(synkozTaking933&&(synkozPreroll933||snkozTest935))return;
  DiagScope814 diag(D814_NEO,pendingClockTicks);
  static uint32_t previousDelayEdit930=UINT32_MAX;
  uint32_t delayEdit930=(modUi805==MOD_DELAY806?(uint32_t)(modOwnerTrack805*16+delaySlot924*256):0xFFFFu)|((quickView903==3?quickItem903:0xFFFFu)<<16);
  if(delayEdit930!=previousDelayEdit930){previousDelayEdit930=delayEdit930;if(delayMixer930)neoDisplayDirty=true;}

  if (!delayMixer930) {
  static uint8_t lastHead24=255;uint8_t head24=(playheadTick24()/TICKS_PER_STEP)%64;
  if(head24!=lastHead24){lastHead24=head24;neoDisplayDirty=true;}
  if (currentStep != lastDisplayedStep) {
    lastDisplayedStep = currentStep;
    neoDisplayDirty = true;
  }
  uint8_t visualPos = 255;
  uint32_t span = windowTickSpan();
  uint32_t pixelTicks = max((uint32_t)1, span / 16);
  visualPos = (uint8_t)min((uint32_t)15, ((measureTick+span-viewTickOffset%span)%span) / pixelTicks);
  if (visualPos != lastDisplayedVisualPos) {
    lastDisplayedVisualPos = visualPos;
    neoDisplayDirty = true;
  }
  }

  if (!neoDisplayDirty) return;
  uint32_t now = millis();
  if ((uint32_t)(now - lastNeoRefreshMs) < NEO_MIN_REFRESH_MS) return;
  lastNeoRefreshMs = now;
  neoDisplayDirty = false;

  // Frozen visuals retain the normal between-panel musical service points.
  // Still obey the normal dirty/refresh gates; do not create a fast catch-up loop.
  if(diagActive814&&diagNeo814==2){
    for(uint8_t n=0;n<NEO_COUNT;++n)if(neoAvailable[n])serviceRealtimeBetweenNeoPanels();
    return;
  }

  if(writeMenuActive){for(uint8_t n=0;n<4;++n){for(uint8_t p=0;p<16;++p){uint8_t t=n*4+p%4;neoPixel821(n,p,colorForTrackRole(n,t,writeTracks41&(1u<<t)?NEO_COLOR_SELECTED_NOTE:NEO_COLOR_EMPTY));}diagShowNeo814(n);}return;}
  if(colorsPreview11())return;
  if(delayMixerRender930())return;
  if(recordRender820())return;
  if((trackChord815&&(trackContext815()||trackSelectContext817()||keyboardContext823()||(settingsOpen&&debugTestMode==DEBUG_TEST_NONE)))||(settingsOpen&&settingsMenuLevel==SETTINGS_MIXER_EDIT915)){
    for(uint8_t n=0;n<NEO_COUNT;++n){if(!(neoAvailable[n]||pc831.active))continue;
      for(uint8_t p=0;p<16;++p){uint8_t t=n*4+p%4,row=p/4;uint32_t col;
        bool active=mixerActive938(t,row,trackPads815[n][p]||keyboardTrackPulse823(t));
        const auto&rgb=NEO_PALETTE[modMixer939?modColors939[row]:modeColors902[row]];uint8_t div=active?1:5;
        col=mixerFunction938(t,row)==MIX_MUTE938&&!(settingsOpen&&settingsMenuLevel==SETTINGS_MIXER_EDIT915)?mixerMuteColor930(n,t):colorFor(n,rgb.r/div,rgb.g/div,rgb.b/div);
        col=colorScale11(col,colorBrightness11[modMixer939?6:2][row]);neoPixel821(n,p,col);
      }diagShowNeo814(n);serviceRealtimeBetweenNeoPanels();
    }return;
  }
  if (debugTestMode == DEBUG_TEST_NEO) {
    for (uint8_t n = 0; n < NEO_COUNT; n++) {
      if (!(neoAvailable[n]||pc831.active)) continue;
      for (uint8_t pad = 0; pad < 16; pad++) {
        uint32_t col = neoPadDown[n][pad]
                         ? colorFor(n, 40, 40, 40)
                         : colorFor(n, 0, 0, 0);
        if(shiftButtonActive&&rectangleArmed&&rectangleNeo==n&&rectanglePad==pad)col=colorFor(n,80,80,80);
        neoPixel821(n,pad, col);
      }
      diagShowNeo814(n);
      serviceRealtimeBetweenNeoPanels();
    }
    return;
  }

  if (grooveEditMode && !patternMode && debugTestMode == DEBUG_TEST_NONE) {
    for (uint8_t n = 0; n < NEO_COUNT; n++) {
      if (!(neoAvailable[n]||pc831.active)) continue;
      for (uint8_t pad = 0; pad < 16; pad++) {
        uint32_t color = 0;
        if (pad < 4) {
          uint8_t step = n * 4 + pad;
          bool modified = false;
          for (uint8_t f = 0; f < 4; f++) modified |= f == 2 ? grooveGate51(editedGroove, step) != 100 : grooveOffset[editedGroove - 1][step][f] != 0;
          color = (grooveStepMask & (1u << step)) ? colorFor(n, 0, 80, 85)
            : modified ? colorFor(n, 75, 50, 0) : colorFor(n, 8, 8, 8);
          if (playing && !trackSilent818(selectedTrack) &&
              playbackGroove(selectedTrack, activePattern[selectedTrack]) == editedGroove &&
              (localTickForTrack(selectedTrack) / TICKS_PER_STEP) % GROOVE_STEPS == step)
            color = colorFor(n, 40, 95, 40);
        }
        neoPixel821(n,pad, color);
      }
      diagShowNeo814(n); serviceRealtimeBetweenNeoPanels();
    }
    return;
  }
  if (sdNeoNoticeActive && debugTestMode == DEBUG_TEST_NONE &&
      !menuNavigationOnly() && !patternMode && !remixMode) {
    for (uint8_t n = 0; n < NEO_COUNT; n++) {
      if (!(neoAvailable[n]||pc831.active)) continue;
      for (uint8_t pad = 0; pad < 16; pad++) {
        neoPixel821(n,pad, sdNoticePixel(n, pad)
                                           ? colorFor(n, 90, 0, 0) : 0);
      }
      diagShowNeo814(n);
      serviceRealtimeBetweenNeoPanels();
    }
    return;
  }

  if(remixRender())return;


  if (patternMode) {
    bool blinkOff = (millis()/240)%2;
    for (uint8_t n = 0; n < NEO_COUNT; n++) {
      if (!(neoAvailable[n]||pc831.active)) continue;
      for (uint8_t pad = 0; pad < 16; pad++) {
        uint8_t track = n * 4 + (pad % 4);
        uint8_t pattern = patternAtRow51(track,patternRowOffset + (pad / 4));
        bool populated = track < TRACK_COUNT && pattern < PATTERN_COUNT && patternHasNotes(track, pattern);
        bool selected=track<TRACK_COUNT&&pattern<PATTERN_COUNT&&patternSelected[track][pattern];
        bool current=populated&&track<TRACK_COUNT&&activePattern[track]==pattern;bool waiting=track<TRACK_COUNT&&pendingPattern[track]==pattern;
        uint8_t role11=waiting?2:selected?3:current?2:populated?1:0;uint32_t col=waiting&&blinkOff?0:colorScale11(palette932(n,patternColors932[role11]),colorBrightness11[1][role11]);
        if(track<16&&pattern<16){if(patternLinks911[track][pattern])col=colorScale11(palette932(n,patternLinkColor55[track][pattern]),colorBrightness11[1][role11]);if(ind50.flags[track*16+pattern]&0x80)col=colorScale11(col,75);if(waiting&&blinkOff)col=0;if(current&&playing&&!trackSilent818(track)&&localTickForTrack(track)<max(1,(int)(65.0f/tickDurationMs())))col=0;}
        neoPixel821(n,pad, col);
      }
      diagShowNeo814(n);
      serviceRealtimeBetweenNeoPanels();
    }
    return;
  }

  if (noteViewForTrack(selectedTrack) == NOTE_VIEW_COMPACT) rebuildCompactPitchRows();
  uint16_t gateBody09[4]={};buildGateBody09(gateBody09);
  for (uint8_t n = 0; n < NEO_COUNT; n++) {
    if (!(neoAvailable[n]||pc831.active)) continue;
    for (uint8_t pad = 0; pad < 16; pad++) {
	      uint8_t slot;
	      uint16_t step;
	      padToSlotStep(n, pad, slot, step);

      uint32_t col = colorForTrackRole(n, selectedTrack, NEO_COLOR_EMPTY);
      uint32_t pixelStart;
      uint32_t pixelEnd;
      padTimelineBounds(n, pad, pixelStart, pixelEnd);
      uint8_t head=playheadBrightness24(selectedTrack,n,pad);
      bool isCurrent=head!=0;
      if(isCurrent){stepPixel25(n,pad,colorScale11(colorForTrackRole(n,selectedTrack,NEO_COLOR_PLAYHEAD),head));continue;}
      uint8_t t = selectedTrack;
      uint8_t rowNote = padChromaticNote(pad);
      if (rowNote > 127 || step >= trackStepCount(t)) {
        if(shiftButtonActive&&rectangleArmed&&rectangleNeo==n&&rectanglePad==pad)col=colorFor(n,80,80,80);
        stepPixel25(n,pad, col);
        continue;
      }
      if (t < TRACK_COUNT) {
        // Un pixel represente les tetes logiques qu'il contient.
        uint16_t visibleNoteStep = step;
        uint8_t visibleVoice = 0;
        bool monoRows = monoMeasureRowsView(t);
        bool noteVisible = visibleNoteAtPad(t, rowNote, n, pad, visibleNoteStep, &visibleVoice);
        bool rollZoneVisible = !noteVisible && rollZoneAtPad(t, rowNote, n, pad);
        bool isSelectedStep = selectedTrack == t && selectedStep == (int16_t)visibleNoteStep &&
                              selectedVoice == visibleVoice && noteVisible;
        bool selectedPitchRow = selectionPitchValid && rowNote == selectionPitch;
        bool isMultiSelected = noteVisible &&
                               ((allTrackNotesSelected && t == selectedTrack) ||
                                selectedNoteObject(t, visibleNoteStep, visibleVoice));
        bool isSelectedEmpty = !noteVisible && (explicitNoteSelection
          ? selectedEmptyCell(t, step, rowNote) : selectedPitchRow && stepSelected[t][step]);
        if (noteVisible && exiledNoteObject(t, visibleNoteStep, visibleVoice)) {
          col = colorFor(n, 65, 0, 95);
        } else if (isMultiSelected) {
          col = colorForTrackRole(n, t, NEO_COLOR_SELECTED_NOTE);
        } else if (isSelectedStep) {
          col = colorForTrackRole(n, t, NEO_COLOR_SELECTED_NOTE);
        } else if (isSelectedEmpty) {
          col = freeAtPad11(t,rowNote,pixelStart,pixelEnd)?plxColor11(n,false,true):colorForTrackRole(n, t, NEO_COLOR_SELECTED_EMPTY);
        } else if(freeAtPad11(t,rowNote,pixelStart,pixelEnd)){col=plxColor11(n,false,false);

        } else if (noteVisible) {
          col = noteHeadCountAtPad(t, rowNote, n, pad) > 1
                  ? colorFor(n, 85, 35, 0)
                  : colorForTrackRole(n, t, NEO_COLOR_NOTE);
          if(blockTargeted30){if(noteHasPlx35(t,visibleNoteStep,visibleVoice))col=plxColor11(n,true,false);}else if(lockCount817)for(uint16_t k=0;k<lockCount817;++k){auto&l=noteLocks817[k];if(!blockMusicalField35(l)&&l.track==t&&l.pattern==activePattern[t]&&l.note==noteAtVoice(t,visibleNoteStep,visibleVoice)&&l.tick==lockTick817(t,visibleNoteStep,visibleVoice)){col=plxColor11(n,true,false);break;}}
        } else if(gateBody09[pad/4]&(1u<<(n*4+pad%4))){
          uint16_t bit41=1u<<(n*4+pad%4);bool selected41=selectedGateBody11[pad/4]&bit41;
          uint32_t base41=selected41?colorForTrackRole(n,t,NEO_COLOR_SELECTED_NOTE):(plxGateBody41[pad/4]&bit41)?plxColor11(n,true,false):colorForTrackRole(n,t,NEO_COLOR_NOTE);col=(base41&0xFEFEFEu)>>1;
        } else if (rollZoneVisible) {
          col = colorForTrackRole(n, t, NEO_COLOR_REMIX);
        }
        if (!monoRows && (int32_t)(activeLineWhiteUntilMs - millis()) > 0 &&
            rowNote == activePitchRow) {
          col = colorFor(n, 95, 95, 95);
        }
      }

      if(shiftButtonActive&&rectangleArmed&&rectangleNeo==n&&rectanglePad==pad)col=colorFor(n,80,80,80);
        stepPixel25(n,pad, col);
    }
    diagShowNeo814(n);
    serviceRealtimeBetweenNeoPanels();
  }
}

// ============================================================
// EDIT KY / ROY
// ============================================================
void setEditModeFromKyTrack(uint8_t kyTrack) {
  if (kyTrack == 2) editMode = EDIT_PITCH;
  else if (kyTrack == 3) editMode = EDIT_VEL;
  else if (kyTrack == 4) editMode = EDIT_GATE;
  else if (kyTrack == 5) editMode = EDIT_TIME;
}

FLASHMEM void selectEditingTrack(uint8_t track) {
  if (track >= TRACK_COUNT || track == selectedTrack) return; moveFinish41(true);recordExitA01();
  uint8_t previousTrack = selectedTrack;
  if(grooveEditMode){grooveEditTrack23[selectedTrack]=editedGroove;editedGroove=grooveSheet35(track);uint8_t id=blocks21[track][0].groove;blockListEdit21=id?id-1:0;}
  trackViewTickOffset[previousTrack] = viewTickOffset;
  trackZoomLevel[previousTrack] = zoomLevel;
  trackActivePitchRow[previousTrack] =
    activePitchTrack == previousTrack && activePitchRow <= 127
      ? activePitchRow : defaultMidiNote[previousTrack];
  if (!selectButtonActive && !shiftButtonActive && !deleteButtonActive) {
    DiagScope814 diag(D814_CLEAR);
    discardNoteRedo();
    cancelStepSelection();
  }
  selectedTrack = track;
  selectedStep = -1;
  displayedPattern = activePattern[selectedTrack] + 1;
  trackRollLastEditedField = 255;
  DiagScope814 diagView(D814_VIEW);
  ensureSelectedTrackVisible();
  zoomLevel = trackZoomLevel[selectedTrack];
  viewTickOffset = trackViewTickOffset[selectedTrack];
  activePitchRow = trackActivePitchRow[selectedTrack];
  activePitchTrack = selectedTrack;
  ensurePitchVisible(activePitchRow);
  clampViewStepOffset();
  lastNeoRefreshMs = millis() - NEO_MIN_REFRESH_MS;
  displayBpmDirty = true;
  neoDisplayDirty = true;
  displayValueDirty = true;
  pendingViewSource = VIEW_SRC_TRACK;
  if(quickView903)quickChoose903(quickView903);
}

FLASHMEM void selectTrackWithPreview814(uint8_t next) {
  if(next>=TRACK_COUNT)return;
  if (next == selectedTrack) return;
  DiagScope814 diagTrack(D814_TRACK);
  diagTrack.track=next;
  selectEditingTrack((uint8_t)next);
  if (trackNavigationPreview) {DiagScope814 diag(D814_AUDITION);triggerNoteAudition(selectedTrack, defaultMidiNote[selectedTrack]);}
  DiagScope814 diagMenu(D814_MENU);
  if (trackColorMenuActive) {
    trackColorDraft = -1;
    sendTrackColorMenu();
  } else if (trackViewMenuActive) {
    sendTrackViewMenu();
  } else if (trackMenuActive) {
    sendTrackMenu();
  }
}

FLASHMEM void selectTrackFromKy(int32_t rawDelta) {
  int next=(int)selectedTrack+(rawDelta>0?1:rawDelta<0?-1:0);
  selectTrackWithPreview814((uint8_t)clampInt(next,0,TRACK_COUNT-1));
}

void internalPatternPath(uint8_t track, uint8_t pattern, char path[40]) {
  snprintf(path, 40, "/URBANKAOZ/PAT/.T%02u_P%02u.BIN", track + 1, pattern + 1);
}

FLASHMEM bool saveTrackPattern(uint8_t track, uint8_t pattern) {
  if (track >= TRACK_COUNT || pattern >= PATTERN_COUNT || !storageWriteAvailable) return false;
  if(!SD.exists("/URBANKAOZ/PAT")&&!SD.mkdir("/URBANKAOZ/PAT"))return false;
  lockPrune817(track,pattern);
  StoredTrackPattern &dst = patternBank[track][pattern];
  dst.initialized = true;
  dst.bars = trackBars[track];
  dst.hasNotes = false;
  for (uint16_t step = 0; step < trackStepCount(track); step++) if (stepActive[track][step]) { dst.hasNotes = true; break; }
  if (!dst.hasNotes) for (const auto &e : fineNote[track]) if (e.active) { dst.hasNotes = true; break; }
  char path[40];
  internalPatternPath(track, pattern, path);
  SD.remove(path);
  // A failed removal must never turn a replacement into an append.
  File file = SD.open(path, FILE_WRITE_BEGIN);
  if (!file || !file.truncate(0)) return false;
  const uint32_t magic = 0x554B4236UL;
  file.write((const uint8_t*)&magic, sizeof(magic));
  file.write((const uint8_t*)&dst.bars, sizeof(dst.bars));
  uint8_t hasNotes = dst.hasNotes ? 1 : 0;
  file.write(&hasNotes, sizeof(hasNotes));
  file.write((const uint8_t*)stepActive[track], sizeof(stepActive[track]));
  file.write((const uint8_t*)stepVel[track], sizeof(stepVel[track]));
  file.write((const uint8_t*)stepMidiNote[track], sizeof(stepMidiNote[track]));
  file.write((const uint8_t*)stepGateTicks[track], sizeof(stepGateTicks[track]));
  file.write((const uint8_t*)stepMicroTick[track], sizeof(stepMicroTick[track]));
  file.write((const uint8_t*)stepLockVel[track], sizeof(stepLockVel[track]));
  file.write((const uint8_t*)stepLockPitch[track], sizeof(stepLockPitch[track]));
  file.write((const uint8_t*)stepLockGate[track], sizeof(stepLockGate[track]));
  file.write((const uint8_t*)stepLockTime[track], sizeof(stepLockTime[track]));
  rollWriteFile906(file,track);
  file.write((const uint8_t*)stepRollLockMask[track], sizeof(stepRollLockMask[track]));
  file.write((const uint8_t*)stepExtraNoteCount[track], sizeof(stepExtraNoteCount[track]));
  file.write((const uint8_t*)stepExtraMidiNote[track], sizeof(stepExtraMidiNote[track]));
  file.write((const uint8_t*)stepLogicalMicro[track], sizeof(stepLogicalMicro[track]));
  file.write((const uint8_t*)fineNote[track], sizeof(fineNote[track]));
  file.write((const uint8_t*)stepExtra14[track], sizeof(stepExtra14[track]));
  file.write((const uint8_t*)defaultMov41[track],sizeof(defaultMov41[track]));
  bool written = !file.getWriteError();
  file.close();
  if (written) pattern805Saved(track, pattern);
  return written;
}

FLASHMEM bool loadTrackPattern(uint8_t track, uint8_t pattern) {
  mixerValid930&=~(1u<<track);
  if (track >= TRACK_COUNT || pattern >= PATTERN_COUNT) return false;
  mod805Invalidate();
  if (patternDirty805[track][pattern]) return pattern805LoadDirty(track, pattern);
  StoredTrackPattern &src = patternBank[track][pattern];
  char path[40];
  internalPatternPath(track, pattern, path);
  if (src.initialized || (!internalPatternSessionFresh && SD.exists(path))) {
    File file = SD.open(path, FILE_READ);
    if (!file) return false;
    uint32_t magic = 0;
    uint16_t bars = 1;
    if (file.read((uint8_t*)&magic, sizeof(magic)) != sizeof(magic) ||
        (magic != 0x554B4231UL && magic != 0x554B4232UL &&
         magic != 0x554B4233UL && magic != 0x554B4234UL && magic != 0x554B4235UL && magic != 0x554B4236UL) ||
        file.read((uint8_t*)&bars, sizeof(bars)) != sizeof(bars)) {
      file.close();
      return false;
    }
    uint8_t hasNotes=src.hasNotes;
    if(magic!=0x554B4231UL&&file.read(&hasNotes,1)!=1){file.close();return false;}
    bool ok=readPatternPayload906(file,magic,bars,patternWork906);
    file.close();
    if(!ok||!pattern805ApplyLive(track,patternWork906))return false;
    src.initialized=true;src.bars=trackBars[track];src.hasNotes=hasNotes!=0;
    trackLoopStartStep[track]=1;trackLoopEndStep[track]=trackStepCount(track);
  } else {
    memset(fineNote[track], 0, sizeof(fineNote[track]));
    defaultMov41[track][0]=0;trackBars[track] = 1;
    trackLoopStartStep[track] = 1;
    trackLoopEndStep[track] = STEPS_PER_BAR;
    for (uint16_t step = 0; step < STEP_COUNT; step++) {
      stepActive[track][step] = false;
      stepVel[track][step] = indNote50(track,1);
      stepMidiNote[track][step] = indNote50(track,0);
      stepExtraNoteCount[track][step] = 0;
      stepExtraNoteCount[track][step] = 0;
      stepGateTicks[track][step] = indNote50(track,2);
      stepLogicalMicro[track][step] = 0;
      stepMicroTick[track][step] = indNote50(track,3);
      stepLockVel[track][step] = false;
      stepLockPitch[track][step] = false;
      stepLockGate[track][step] = false;
      stepLockTime[track][step] = false;
      rollSet906(track,step,emptyRoll906,0);
    }
  }
  for (uint16_t step = 0; step < STEP_COUNT; step++) {
    stepSelected[track][step] = false;
  }
  for (uint8_t fine = 0; fine < FINE_EVENTS_PER_TRACK; fine++) {
    fineLastTriggeredCycle[track][fine] = UINT32_MAX;
  }
  return true;
}

bool patternHasNotes(uint8_t track, uint8_t pattern) {
  static uint16_t absent[16]={};static bool previousFresh=false,previousSd=false;static uint32_t generation=0;
  if(generation!=patternGeneration906||previousFresh!=internalPatternSessionFresh||previousSd!=sdAvailable){generation=patternGeneration906;memset(absent,0,sizeof(absent));previousFresh=internalPatternSessionFresh;previousSd=sdAvailable;}
  if (deleteButtonActive && !deleteNumeric55 && deletePatternContext && track < TRACK_COUNT && pattern < PATTERN_COUNT && patternSelected[track][pattern]) return false;
  if (track >= TRACK_COUNT || pattern >= PATTERN_COUNT) return false;
  if (mod805OwnerPresent(track, pattern)) return true;
  if (pattern == activePattern[track]) {
    for (uint16_t step = 0; step < trackStepCount(track); step++) if (stepActive[track][step]) return true;
    return false;
  }
  StoredTrackPattern &stored = patternBank[track][pattern];
  if (!stored.initialized && sdAvailable && !internalPatternSessionFresh && !(absent[track]&(1u<<pattern))) {
    char path[40];
    internalPatternPath(track, pattern, path);
    File file = SD.open(path, FILE_READ);
    if(!file)absent[track]|=1u<<pattern;
    if (file) {
      uint32_t magic = 0;
      uint16_t bars = 1;
      uint8_t hasNotes = 1; // ancien format : considere occupe par prudence
      if (file.read((uint8_t*)&magic, sizeof(magic)) == sizeof(magic) &&
          file.read((uint8_t*)&bars, sizeof(bars)) == sizeof(bars) &&
          (magic == 0x554B4231UL || magic == 0x554B4232UL ||
           magic == 0x554B4233UL || magic == 0x554B4235UL || magic == 0x554B4236UL)) {
        if (magic != 0x554B4231UL) file.read(&hasNotes, sizeof(hasNotes));
        stored.initialized = true;
        stored.bars = (uint16_t)clampInt(bars, 1, MAX_BARS);
        stored.hasNotes = hasNotes != 0;
      }
      file.close();
    }
  }
  return stored.initialized && stored.hasNotes;
}

void patternClipboardPath(uint8_t slot, uint8_t index, char path[40]) {
  snprintf(path, 40, "/URBANKAOZ/PAT/.C%u_%02u.BIN", slot + 1, index);
}

bool copySdBinary(const char* sourcePath, const char* destinationPath) {
  if (!pattern805Flush()) return false;
  File source = SD.open(sourcePath, FILE_READ);
  if (!source) return false;
  SD.remove(destinationPath);
  File destination = SD.open(destinationPath, FILE_WRITE);
  if (!destination) {
    source.close();
    return false;
  }
  uint8_t buffer[128];
  bool ok = true;
  while (source.available()) {
    int count = source.read(buffer, sizeof(buffer));
    if (count <= 0) { ok = false; break; }
    if (destination.write(buffer, (size_t)count) != (size_t)count) { ok = false; break; }
  }
  source.close();
  destination.close();
  if (!ok) SD.remove(destinationPath);
  return ok;
}

bool capturePatternSelectionIntoClipboard(uint8_t slot) {
  return capturePattern15(slot);
}

void clearTrackPatternData(uint8_t track) {
 defaultMov41[track][0]=0;
  if (track >= TRACK_COUNT) return;
  for (uint16_t step = 0; step < STEP_COUNT; step++) clearTrackStep(track, step);
  memset(fineNote[track], 0, sizeof(fineNote[track]));
  trackBars[track] = 1;
  trackLoopStartStep[track] = 1;
  trackLoopEndStep[track] = STEPS_PER_BAR;
}

bool installPatternClipboardEntry(uint8_t slot, uint8_t index,
                                  uint8_t destinationTrack, uint8_t destinationPattern) {
  return installPattern15(slot,index,destinationTrack,destinationPattern);
}

bool pastePatternClipboard(uint8_t slot, uint8_t targetTrack, uint8_t targetPattern) {
  if (slot >= CLIPBOARD_COUNT || !patternClipboard[slot].valid || !patternClipboard[slot].count) {
    sendClipboardMessage(slot, CLIP_TYPE_NONE, "NO PATTERN CLIP");
    return false;
  }
  PatternClipboardManifest &manifest = patternClipboard[slot];
  bool sameSourceTrack = true;
  for (uint8_t i = 1; i < manifest.count; i++) {
    if (manifest.sourceTrack[i] != manifest.sourceTrack[0]) sameSourceTrack = false;
  }
  uint8_t pasted = 0;
  for (uint8_t i = 0; i < manifest.count; i++) {
    uint8_t destinationTrack = targetTrack;
    uint8_t destinationPattern = targetPattern;
    if (manifest.count > 1 && sameSourceTrack) {
      destinationPattern = targetPattern + i;
      if (destinationPattern >= PATTERN_COUNT) break;
    } else if (manifest.count > 1) {
      destinationTrack = manifest.sourceTrack[i];
    }
    if (!installPatternClipboardEntry(slot, i, destinationTrack, destinationPattern)) break;
    pasted++;
  }
  invalidateStepTriggers();
  neoDisplayDirty = true;
  displayValueDirty = true;
  if (pasted == manifest.count) {
    sendClipboardMessage(slot, CLIP_TYPE_SEQUENCE, "PATTERNS PASTED");
    return true;
  }
  DISPLAY_SERIAL.println(F("MSG|PATTERN PASTE PARTIAL"));
  return pasted > 0;
}

#include "GrooveBridge.h"

#include "ClipboardStorage.h"

FLASHMEM void switchTrackPattern(uint8_t track, uint8_t pattern, bool selectAfter) {
  uint32_t phase59=micros();auto mark59=[&](int i){uint32_t now=micros();if(benchPrepared29)transitionMax59[i]=max(transitionMax59[i],now-phase59);phase59=now;};
  recordClose820();
  if (deleteButtonActive || (selectButtonActive && hasSelectedSteps())) return;
  if (track >= TRACK_COUNT || pattern >= PATTERN_COUNT) return;
  if (pattern != activePattern[track]) discardNoteRedo();
  if (pattern != activePattern[track]) {
    mark59(0);pattern805CaptureLive(track,patternWork906);mark59(1);
    if (!storedMatches59(track,activePattern[track],patternWork906)&&!storePattern906(track,activePattern[track],patternWork906)) {
      DISPLAY_SERIAL.println(F("MSG|PATTERN MEMORY FULL"));
      return;
    }
    mark59(2);patternDirty805[track][activePattern[track]]=true;
    patternBank[track][activePattern[track]].initialized=true;
    patternBank[track][activePattern[track]].bars=trackBars[track];
    patternBank[track][activePattern[track]].hasNotes=false;
    for(uint16_t s=0;s<trackStepCount(track);++s)if(stepActive[track][s]){patternBank[track][activePattern[track]].hasNotes=true;break;}
    for(const auto&e:fineNote[track])if(e.active)patternBank[track][activePattern[track]].hasNotes=true;
    mark59(3);if (!loadTrackPattern(track, pattern)) {
      DISPLAY_SERIAL.println(F("MSG|PATTERN LOAD ERROR"));
      return;
    }
    mark59(4);activePattern[track] = pattern;
    patch808Pattern(track);mark59(5);
    grooveListOrigin[track] = transportTickAbsolute;
    memset(lastTriggeredCycle[track],255,sizeof(lastTriggeredCycle[track]));
    memset(fineLastTriggeredCycle[track],255,sizeof(fineLastTriggeredCycle[track]));
    for(auto &d:deferredRoll)if(d.track==track)d.active=false;
  }
  mark59(6);if (selectAfter) {
    selectedTrack = track;
    selectedStep = -1;
  }
  if (track == selectedTrack) {
    ensureSelectedTrackVisible();
    beatDraftBars = sequenceBars;
    ensurePitchVisible(defaultMidiNote[selectedTrack]);
    clampViewStepOffset();
    displayedPattern = pattern + 1;
  }
  neoDisplayDirty = true;
  displayValueDirty = true;
  displayBpmDirty = true;
}

FLASHMEM void requestTrackPattern(uint8_t track, uint8_t pattern) {
  if (track >= TRACK_COUNT || pattern >= PATTERN_COUNT) return;
  patternHasNotes(track, pattern); // Refresh metadata for an inactive SD pattern.
  if (playing && pattern != activePattern[track]) {
    if (prepareGrooveBridge(track, pattern)) DISPLAY_SERIAL.println(F("MSG|PATTERN PREPARED"));
    // A multi-track pad operation must service due notes between preparations.
    // Never recurse when a request already comes from the musical tick loop.
    if(!clockContext830){updateTransport();serviceNoteOffs();}
  } else {
    clearGrooveBridge(track);
    pendingPattern[track] = -1;
    if (!playing) grooveTrackPhase[track] = 0;
    switchTrackPattern(track, pattern, false);
  }
  neoDisplayDirty = true;
  displayValueDirty = true;
  displayBpmDirty = true;
}

void changeMidiChannelFromKy(int32_t rawDelta) {
  uint8_t track = linkOwner12(selectedTrack,LINK_MOUT12);
  int next = (int)trackMidiChannel[track] + (rawDelta > 0 ? 1 : -1);
  next = clampInt((trackMidiChannel[track])+(rawDelta>0?1:-1),0,15);
  if (next == trackMidiChannel[track]) return;
  (pidiRoutesDirty931|=1u<<(track), trackMidiChannel[track] = (uint8_t)next);
  displayValueDirty = true;
}

void changePitchBendFromKy(int32_t rawDelta) {
  uint8_t track = selectedTrack;
  if(rawDelta)pitchBendStepShown44=false;
  int32_t amount = rawDelta * pitchBendIncrement44();
  if(indNoteEdit50(4,amount))return;
  int32_t next = (int32_t)trackPitchBend[track] + amount;
  next = WRAP23(trackPitchBend[track],amount,-8192,8191);
  if (next == trackPitchBend[track]) return;
  trackPitchBend[track] = (int16_t)next;
  displayValueDirty = true;
}

bool currentEditIsPLock() {
  if (selectedStep < 0) return false;
  if (selectedTrack >= TRACK_COUNT) return false;
  if (selectedVoice>=noteCountAtStep(selectedTrack,(uint16_t)selectedStep)) return false;
  return true;
}

void propagateDefaultToUnlockedSteps(uint8_t, EditMode) {}

uint16_t nextMusicalGate(uint16_t current, int32_t delta) {
  if (delta > 0) {
    for (uint8_t i = 0; i < GATE_MUSICAL_COUNT; i++) {
      if (GATE_MUSICAL_VALUES[i] > current) return GATE_MUSICAL_VALUES[i];
    }
    return GATE_TICK_MAX;
  }

  if (delta < 0) {
    for (int8_t i = GATE_MUSICAL_COUNT - 1; i >= 0; i--) {
      if (GATE_MUSICAL_VALUES[i] < current) return GATE_MUSICAL_VALUES[i];
    }
    return GATE_TICK_MIN;
  }

  return current;
}

uint16_t applyGateDeltaToValue(uint16_t current, int32_t delta) {
  if (gateEditMode == GATE_EDIT_MUSICAL) {int next=nextMusicalGate(current,delta);return clampInt(next,GATE_TICK_MIN,GATE_TICK_MAX);}
  int32_t inc = (gateEditMode == GATE_EDIT_24T) ? TICKS_PER_STEP : 1;
  int32_t v = (int32_t)current + (delta > 0 ? inc : -inc);
  return (uint16_t)clampInt(v,GATE_TICK_MIN,GATE_TICK_MAX);
}

void applyDeltaToSingleStep(uint8_t t, uint16_t s, EditMode mode, int32_t delta) {
  if (mode == EDIT_VEL) {
    int v = WRAP23(stepVel[t][s],delta,1,127);
    stepVel[t][s] = (uint8_t)v;
    stepLockVel[t][s] = true;
  } else if (mode == EDIT_PITCH) {
    uint8_t voice = (t == selectedTrack && selectedStep == (int16_t)s) ? selectedVoice : 0;
    int v = WRAP23(noteAtVoice(t,s,voice),delta,0,127);
    lockPitch817(t,s,voice,v);
    if (voice == 0) {
      stepMidiNote[t][s] = (uint8_t)v;
      stepLockPitch[t][s] = true;
    } else if (voice <= stepExtraNoteCount[t][s]) {
      stepExtraMidiNote[t][s][voice - 1] = (uint8_t)v;
    }
  } else if (mode == EDIT_GATE) {
    stepGateTicks[t][s] = applyGateDeltaToValue(stepGateTicks[t][s], delta);
    stepLockGate[t][s] = true;
  } else if (mode == EDIT_TIME) {
    int v = WRAP23(stepMicroTick[t][s],delta,MICRO_TICK_MIN,MICRO_TICK_MAX);
    stepMicroTick[t][s] = (int16_t)v;
    stepLockTime[t][s] = true;
  }
}

void applyDeltaToSelectedSteps(EditMode mode, int32_t delta) {
  for (uint8_t t = 0; t < TRACK_COUNT; t++) {
    for (uint16_t s = 0; s < STEP_COUNT; s++) {
      if (stepSelected[t][s]) applyDeltaToSingleStep(t, s, mode, delta);
    }
  }
}

void replacePitchRow(uint8_t track, uint8_t oldPitch, uint8_t newPitch) {
  if (track >= TRACK_COUNT || oldPitch == newPitch) return;
  for (uint16_t step = 0; step < trackStepCount(track); step++) {
    int8_t existingNew = findVoiceAtStep(track, step, newPitch);
    for (int8_t voice = (int8_t)noteCountAtStep(track, step) - 1; voice >= 0; voice--) {
      if (noteAtVoice(track, step, (uint8_t)voice) != oldPitch) continue;
      if (existingNew >= 0 && existingNew != voice) {
        removeVoiceAtStep(track, step, (uint8_t)voice);
      } else if (voice == 0) {
        stepMidiNote[track][step] = newPitch;
        stepLockPitch[track][step] = true;
        existingNew = 0;
      } else {
        stepExtraMidiNote[track][step][voice - 1] = newPitch;
        existingNew = voice;
      }
    }
  }
  if (defaultMidiNote[track] == oldPitch || monoMeasureRowsView(track)) defaultMidiNote[track] = newPitch;
  activePitchRow = newPitch;
  activePitchTrack = track;
  rebuildCompactPitchRows();
}

void setSelectedStepsToValue(EditMode mode, int value) {
  for (uint8_t track = 0; track < TRACK_COUNT; track++) {
    for (uint16_t step = 0; step < STEP_COUNT; step++) {
      if (stepSelected[track][step]) setDirectValueOnStep(track, step, mode, value);
    }
  }
}

int clampDirectNumericValue(EditMode mode, long value) {
  if (mode == EDIT_VEL) return clampInt((int)value, 1, 127);
  if (mode == EDIT_PITCH) return clampInt((int)value, 0, 127);
  if (mode == EDIT_GATE) return clampInt((int)value, GATE_TICK_MIN, GATE_TICK_MAX);
  return clampInt((int)value, MICRO_TICK_MIN, MICRO_TICK_MAX);
}

void setDirectValueOnStep(uint8_t track, uint16_t step, EditMode mode, int value) {
  if (track >= TRACK_COUNT || step >= STEP_COUNT) return;

  if (mode == EDIT_VEL) {
    stepVel[track][step] = (uint8_t)value;
    stepLockVel[track][step] = true;
  } else if (mode == EDIT_PITCH) {
    uint8_t voice = (track == selectedTrack && selectedStep == (int16_t)step) ? selectedVoice : 0;
    if (voice == 0) {
      stepMidiNote[track][step] = (uint8_t)value;
      stepLockPitch[track][step] = true;
    } else if (voice <= stepExtraNoteCount[track][step]) {
      stepExtraMidiNote[track][step][voice - 1] = (uint8_t)value;
    }
  } else if (mode == EDIT_GATE) {
    stepGateTicks[track][step] = (uint16_t)value;
    stepLockGate[track][step] = true;
  } else {
    stepMicroTick[track][step] = (int16_t)value;
    stepLockTime[track][step] = true;
  }
}

void setDirectDefaultValue(uint8_t track, EditMode mode, int value) {
  if (track >= TRACK_COUNT) return;

  if (mode == EDIT_VEL) defaultVel[track] = (uint8_t)value;
  else if (mode == EDIT_PITCH) defaultMidiNote[track] = (uint8_t)value;
  else if (mode == EDIT_GATE) defaultGateTicks[track] = (uint16_t)value;
  else defaultMicroTick[track] = (int16_t)value;
}

int applyGateDraft(int baseValue) {
  int value = baseValue;
  for (uint8_t i = 0; i < gateDraftCount; ++i) {
    const GateDraftSegment &s = gateDraftSegments[i];
    if (s.unit == GATE_EDIT_MUSICAL) {
      uint32_t count = min((uint64_t)GATE_MUSICAL_COUNT + 1, s.delta >= 0 ? (uint64_t)s.delta : (uint64_t)(-(int64_t)s.delta));
      while (count--) value = nextMusicalGate(value, s.delta >= 0 ? 1 : -1);
    } else {
      int64_t next = (int64_t)value + (int64_t)s.delta * (s.unit == GATE_EDIT_24T ? TICKS_PER_STEP : 1);
      value = next < GATE_TICK_MIN ? GATE_TICK_MIN : next > GATE_TICK_MAX ? GATE_TICK_MAX : (int)next;
    }
  }
  return value;
}

#include "DraftWrap23.h"

int applyPendingDeltaToValue(EditMode mode, int baseValue, int32_t delta) {
  if (mode == EDIT_GATE) {
    if (gateEditMode != GATE_EDIT_MUSICAL) {
      int64_t value = (int64_t)baseValue + (int64_t)delta * (gateEditMode == GATE_EDIT_24T ? TICKS_PER_STEP : 1);
      return value < GATE_TICK_MIN ? GATE_TICK_MIN : value > GATE_TICK_MAX ? GATE_TICK_MAX : (int)value;
    }
    uint16_t value = (uint16_t)baseValue;
    uint32_t count = min((uint64_t)GATE_MUSICAL_COUNT + 1, delta >= 0 ? (uint64_t)delta : (uint64_t)(-(int64_t)delta));
    int32_t direction = delta >= 0 ? 1 : -1;
    while (count-- > 0) value = applyGateDeltaToValue(value, direction);
    return value;
  }
  return clampDirectNumericValue(mode, (long)baseValue + delta);
}

bool pendingSelectionContains(uint8_t track, uint16_t step, uint8_t voice) {
  if (!pendingSelectionEdit.active) return false;
  if (pendingSelectionEdit.range) return selectedNoteObject(track, step, voice);
  if (pendingSelectionEdit.track != track) return false;
  return pendingSelectionEdit.step == (int16_t)step && pendingSelectionEdit.voice == voice;
}

int pendingEditedValue(uint8_t track, uint16_t step, uint8_t voice, EditMode mode, int baseValue) {
  PendingSelectionField &field = pendingSelectionEdit.field[(uint8_t)mode];
  if (!field.touched || !pendingSelectionContains(track, step, voice)) return baseValue;
  return field.sameValue ? field.target : draftValue23(mode,baseValue);
}

uint8_t visualNoteAtVoice(uint8_t track, uint16_t step, uint8_t voice) {
  int baseValue = noteAtVoice(track, step, voice);
  return (uint8_t)pendingEditedValue(track, step, voice, EDIT_PITCH, baseValue);
}

FLASHMEM bool noteChangedInCurrentSelection(uint8_t track, uint16_t step, uint8_t voice) {
  if (!pendingSelectionContains(track, step, voice)) return false;
  for (uint8_t m = 0; m < EDIT_MODE_COUNT; ++m) {
    EditMode mode = (EditMode)m;
    int base = mode == EDIT_VEL ? effectiveVel(track, step, voice)
      : mode == EDIT_PITCH ? noteAtVoice(track, step, voice)
      : mode == EDIT_GATE ? effectiveGateTicks(track, step, voice) : effectiveMicroTick(track, step, voice);
    if (pendingEditedValue(track, step, voice, mode, base) != base) return true;
  }
  TrackRollSettings base = configuredRollSettings(track, step, voice);
  TrackRollSettings edited = pendingDisplayedRoll(track, step, voice, base);
  for (uint8_t f = 0; f < 6; ++f)
    if (rollFieldValue(base, f) != rollFieldValue(edited, f)) return true;
  return false;
}

struct Detent40 {int original=0;uint32_t at=0;bool holding=false;};
static Detent40 detent40[EDIT_MODE_COUNT];
FLASHMEM void initialisePendingSelectionEdit() {
  for(auto&d:detent40)d={};
  selectionIndicatorsPending931=true;
  gateDraftCount = 0;
  memset(draftCount23,0,sizeof(draftCount23));
  memset(&pendingSelectionEdit, 0, sizeof(pendingSelectionEdit));
  pendingSelectionEdit.active = true;
  pendingSelectionEdit.track = selectedTrack;
  pendingSelectionEdit.range = hasSelectedSteps();
  pendingSelectionEdit.sourcePitch = selectionPitch;
  pendingSelectionEdit.step = selectedStep;
  pendingSelectionEdit.voice = selectedVoice;
}

bool pendingTargetStillSelected() {
  if (!pendingSelectionEdit.active) return false;
  if (pendingSelectionEdit.range) return selectButtonActive || selectionHasActiveNotes();
  if (pendingSelectionEdit.track != selectedTrack) return false;
  return selectedStep == pendingSelectionEdit.step && selectedVoice == pendingSelectionEdit.voice &&
         selectedStep >= 0 && selectedVoice<noteCountAtStep(selectedTrack,(uint16_t)selectedStep);
}

bool stageSelectedParamDelta(EditMode mode, int32_t delta) {
  if (!delta) return false;
  bool range = selectionHasActiveNotes();
  bool single = currentEditIsPLock();
  if (!range && !single) return false;
  if (pendingSelectionEdit.active && !pendingTargetStillSelected()) commitPendingSelectionEdit();
  if (!pendingSelectionEdit.active) initialisePendingSelectionEdit();

  PendingSelectionField &field = pendingSelectionEdit.field[(uint8_t)mode];
  bool sameValue = mode!=EDIT_TIME && range && multiEditSameForTrack(selectedTrack);
  auto&stop40=detent40[(uint8_t)mode];
  if(stop40.holding){if(millis()-stop40.at<(uint32_t)max(1.0f,tickDurationMs()*384))return true;stop40.holding=false;}
  if (!field.touched) {
    int s=selectedStep,v=selectedVoice;
    if(range){for(uint16_t k=0;k<STEP_COUNT;++k){int voice=selectedRangeVoice(selectedTrack,k);if(voice>=0){s=k;v=voice;break;}}}
    if(s>=0)stop40.original=selectedStepValue(selectedTrack,s,v,mode);
    // Keep the field inactive until its original value has been sampled.
    field.sameValue = sameValue;
    field.delta = 0;
    field.target = 0;
    if (sameValue) {
      int baseValue = 0;
      if (!selectedStepsHaveCommonValue(mode, baseValue)) {
        for (uint16_t step = 0; step < STEP_COUNT; step++) {
          int8_t voice = selectedRangeVoice(selectedTrack, step);
          if (voice >= 0) {
            baseValue = selectedStepValue(selectedTrack, step, (uint8_t)voice, mode);
            break;
          }
        }
      }
      field.target = baseValue;
    }
    field.touched = true;
  }

  int before40=field.sameValue?field.target:draftValue23(mode,stop40.original);
  int32_t effectiveDelta = mode == EDIT_GATE ? (delta > 0 ? 1 : -1) : delta;
  if(!field.sameValue&&!draftAdd23(mode,effectiveDelta)){commitPendingSelectionEdit();return stageSelectedParamDelta(mode,delta);}
  if (mode == EDIT_GATE && !field.sameValue) {
    if (!gateDraftCount || gateDraftSegments[gateDraftCount-1].unit != gateEditMode) {
      if (gateDraftCount >= 24) { DISPLAY_SERIAL.println(F("MSG|VALIDATE GATE DRAFT")); return false; }
      gateDraftSegments[gateDraftCount++] = {0, gateEditMode};
    }
    gateDraftSegments[gateDraftCount-1].delta += effectiveDelta;
  }
  if (field.sameValue) field.target = mode==EDIT_GATE?applyGateDeltaToValue(field.target,effectiveDelta):WRAP23(field.target,effectiveDelta,mode==EDIT_VEL?1:mode==EDIT_TIME?MICRO_TICK_MIN:0,mode==EDIT_TIME?MICRO_TICK_MAX:127);
  else field.delta += effectiveDelta;
  int after40=field.sameValue?field.target:draftValue23(mode,stop40.original);
  if((before40<stop40.original&&after40>=stop40.original)||(before40>stop40.original&&after40<=stop40.original)){
    field.delta=0;field.target=stop40.original;draftCount23[(uint8_t)mode]=0;if(mode==EDIT_GATE)gateDraftCount=0;
    stop40.at=millis();stop40.holding=true;
  }

  if (range && multipleSelectedNotes() && !field.sameValue) {
    selectionEditDelta = field.delta - effectiveDelta;
    selectionEditDeltaMode = mode;
    selectionEditDelta += effectiveDelta;
    selectionEditDeltaVisible = true;
  } else {
    clearSelectionEditDisplay();
  }
  return true;
}

bool stageSelectedParamValue(EditMode mode, int value, bool valueIsDelta) {
  if (valueIsDelta) return stageSelectedParamDelta(mode, value);
  if (!selectionHasActiveNotes() && !currentEditIsPLock()) return false;
  if (pendingSelectionEdit.active && !pendingTargetStillSelected()) commitPendingSelectionEdit();
  if (!pendingSelectionEdit.active) initialisePendingSelectionEdit();
  PendingSelectionField &field = pendingSelectionEdit.field[(uint8_t)mode];
  field.touched = true;
  field.sameValue = true;
  field.delta = 0;
  field.target = clampDirectNumericValue(mode, value);
  if(mode==EDIT_PITCH){activePitchTrack=selectedTrack;activePitchRow=field.target;ensurePitchVisible(activePitchRow);}
  clearSelectionEditDisplay();
  return true;
}

void setCommittedStepValue(uint8_t track, uint16_t step, uint8_t voice,
                           EditMode mode, int value) {
  if(mode==EDIT_PITCH)lockPitch817(track,step,voice,value);
  int16_t fine = fineIndexForVoice(track, step, voice);
  if (fine >= 0) {
    FineNoteEvent &event = fineNote[track][fine];
    if (mode == EDIT_VEL) {
      event.vel = (uint8_t)value;
      event.pxMask |= PX_VEL;
    } else if (mode == EDIT_PITCH) {
      event.note = (uint8_t)value;
      event.pxMask |= PX_PITCH;
    } else if (mode == EDIT_GATE) {
      event.gate = (uint16_t)value;
      event.pxMask |= PX_GATE;
    } else {
      event.micro = (int16_t)value;
      event.pxMask |= PX_TIME;
    }
    return;
  }
  if (mode == EDIT_PITCH) {
    if (voice == 0) {
      stepMidiNote[track][step] = (uint8_t)value;
      stepLockPitch[track][step] = true;
    } else if (voice <= stepExtraNoteCount[track][step]) {
      stepExtraMidiNote[track][step][voice - 1] = (uint8_t)value;
    }
    return;
  }
  if(voice&&voice<=stepExtraNoteCount[track][step]){auto&x=stepExtra14[track][step][voice-1];if(mode==EDIT_VEL)x.vel=value;else if(mode==EDIT_GATE)x.gate=value;else x.micro=value;return;}
  setDirectValueOnStep(track, step, mode, value);
}

int rollFieldValue(const TrackRollSettings &roll, uint8_t field) {
  if (field == 0) return roll.start;
  if (field == 1) return roll.speed;
  if (field == 2) return roll.accel;
  if (field == 3) return roll.expo;
  if (field == 4) return roll.curve;
  return roll.level;
}

int clampRollFieldValue(uint8_t field, int value) {
  if (field == 0) return clampInt(value, 0, 1536);
  if (field == 1) return clampInt(value, 1, TICKS_PER_STEP);
  if (field == 2 || field == 4) return clampInt(value, -127, 127);
  if (field == 3) return clampInt(value, 0, 102);
  return clampInt(value, 0, 127);
}

void setRollFieldValue(TrackRollSettings &roll, uint8_t field, int value) {
  value = clampRollFieldValue(field, value);
  if (field == 0) roll.start = (uint16_t)value;
  else if (field == 1) roll.speed = (uint16_t)value;
  else if (field == 2) roll.accel = (int16_t)value;
  else if (field == 3) roll.expo = (uint8_t)value;
  else if (field == 4) roll.curve = (int16_t)value;
  else roll.level = (uint8_t)value;
}

bool stageSelectedRollDelta(uint8_t rollField, int delta) {
  bool range = selectionHasActiveNotes();
  bool single = currentEditIsPLock();
  if ((!range && !single) || rollField > 5) return false;
  if (pendingSelectionEdit.active && !pendingTargetStillSelected()) commitPendingSelectionEdit();
  if (!pendingSelectionEdit.active) initialisePendingSelectionEdit();

  PendingSelectionField &field = pendingSelectionEdit.roll[rollField];
  bool sameValue = range && multiEditSameForTrack(selectedTrack);
  if (!field.touched) {
    field.touched = true;
    field.sameValue = sameValue;
    field.delta = 0;
    field.target = 0;
    if (sameValue) {
      for (uint16_t step = 0; step < STEP_COUNT; step++) {
        int8_t voice = selectedRangeVoice(selectedTrack, step);
        if (voice >= 0) {
          field.target = rollFieldValue(configuredRollSettings(selectedTrack, step, (uint8_t)voice), rollField);
          break;
        }
      }
    }
  }
  if(!field.sameValue&&!draftAdd23(4+rollField,delta)){commitPendingSelectionEdit();return stageSelectedRollDelta(rollField,delta);}
  if (field.sameValue) field.target = rollField==0?clampRollFieldValue(0,field.target+delta):WRAP23(field.target,delta,clampRollFieldValue(rollField,-32768),clampRollFieldValue(rollField,32767));
  else field.delta += delta;
  return true;
}

TrackRollSettings pendingDisplayedRoll(uint8_t track, uint16_t step, uint8_t voice,
                                       TrackRollSettings base) {
  if (!pendingSelectionContains(track, step, voice)) return base;
  for (uint8_t fieldIndex = 0; fieldIndex < 6; fieldIndex++) {
    PendingSelectionField &field = pendingSelectionEdit.roll[fieldIndex];
    if (!field.touched) continue;
    int value = field.sameValue ? field.target
                                : draftValue23(4+fieldIndex,rollFieldValue(base,fieldIndex));
    setRollFieldValue(base, fieldIndex, value);
  }
  return base;
}

void discardPendingSelectionEdit() {
  freeDraftFinish25(false);
  selectionIndicatorsPending931=true;
  memset(&pendingSelectionEdit, 0, sizeof(pendingSelectionEdit));
  clearSelectionEditDisplay();
  displayValueDirty = true;
  neoDisplayDirty = true;
}

FLASHMEM bool prepareSelectedChordVoices() {
  if (!explicitNoteSelection || !pendingSelectionEdit.range) return true;
  bool sharedFields = pendingSelectionEdit.field[EDIT_VEL].touched ||
                      pendingSelectionEdit.field[EDIT_GATE].touched ||
                      pendingSelectionEdit.field[EDIT_TIME].touched;
  for (uint8_t field = 0; field < 6; field++) sharedFields |= pendingSelectionEdit.roll[field].touched;
  if (!sharedFields) return true;
  for (uint8_t track = 0; track < TRACK_COUNT; track++) {
    uint16_t needed = 0, free = 0;
    for (uint8_t i = 0; i < FINE_EVENTS_PER_TRACK; i++) if (!fineNote[track][i].active) free++;
    for (uint16_t step = 0; step < STEP_COUNT; step++)
      if (stepSelected[track][step] && selectedBaseMask[track][step]) needed += stepExtraNoteCount[track][step];
    if (needed > free) { DISPLAY_SERIAL.println(F("MSG|EVENT POOL FULL")); return false; }
  }
  for (uint8_t track = 0; track < TRACK_COUNT; track++) {
    for (uint16_t step = 0; step < STEP_COUNT; step++) {
      if (!stepSelected[track][step] || !selectedBaseMask[track][step]) continue;
      uint8_t count = stepExtraNoteCount[track][step];
      int16_t focusedFine = -1;
      for (uint8_t voice = 1; voice <= count; voice++) {
        FineNoteEvent event = {};
        captureNoteRecord(track, step, voice, event);
        event.pxMask |= PX_PITCH;
        for (uint8_t i = 0; i < FINE_EVENTS_PER_TRACK; i++) {
          if (fineNote[track][i].active) continue;
          fineNote[track][i] = event;
          fineLastTriggeredCycle[track][i]=UINT32_MAX;
          selectedFineNote[track][i] = (selectedBaseMask[track][step] & (1u << voice)) ? 1
            : ((selectedBaseMask[track][step] & (1u << (voice + 4))) ? 2 : 0);
          if (selectedTrack == track && selectedStep == (int16_t)step && selectedVoice == voice) focusedFine = i;
          break;
        }
      }
      stepExtraNoteCount[track][step] = 0;
      selectedBaseMask[track][step] &= 0x11;
      if (focusedFine >= 0) {
        selectedFineIndex = focusedFine;
        selectedVoice = voiceForFineIndex(track, step, (uint8_t)focusedFine);
      }
    }
  }
  return true;
}

FLASHMEM void commitPendingSelectionEdit() {
  grooveFinish60(true);
  if(!freeDraftFinish25(true))return;
  selectionIndicatorsPending931=true;
  if (!pendingSelectionEdit.active) return;
  if(pendingSelectionEdit.range&&!explicitNoteSelection){
    memset(selectedBaseMask,0,sizeof(selectedBaseMask));memset(selectedFineNote,0,sizeof(selectedFineNote));
    for(uint8_t t=0;t<TRACK_COUNT;++t)for(uint16_t st=0;st<trackStepCount(t);++st)for(uint8_t v=0;v<noteCountAtStep(t,st);++v)if(selectedNoteObject(t,st,v)){int16_t f=fineIndexForVoice(t,st,v);if(f>=0)selectedFineNote[t][f]=1;else selectedBaseMask[t][st]|=1u<<v;}
    explicitNoteSelection=true;
  }
  if (!prepareSelectedChordVoices()) return;
  discardNoteRedo();
  uint8_t track = pendingSelectionEdit.track;
  for (uint8_t modeIndex = 0; modeIndex < EDIT_MODE_COUNT; modeIndex++) {
    PendingSelectionField &field = pendingSelectionEdit.field[modeIndex];
    if (!field.touched) continue;
    EditMode mode = (EditMode)modeIndex;
    if (pendingSelectionEdit.range) {
      for (uint8_t track = 0; track < TRACK_COUNT; track++) {
      for (uint16_t step = 0; step < STEP_COUNT; step++) {
        if (!stepSelected[track][step]) continue;
        for (uint8_t voice = 0; voice < noteCountAtStep(track, step); voice++) {
        if (!selectedNoteObject(track, step, voice)) continue;
        int baseValue = mode == EDIT_VEL ? effectiveVel(track, step, voice)
                      : mode == EDIT_PITCH ? noteAtVoice(track, step, voice)
                      : mode == EDIT_GATE ? effectiveGateTicks(track, step, voice)
                                          : effectiveMicroTick(track, step, voice);
        int value = field.sameValue ? field.target
                                    : draftValue23(mode,baseValue);
        setCommittedStepValue(track, step, voice, mode, value);
        }
      }
      }
    } else if (pendingSelectionEdit.step >= 0 &&
               pendingSelectionEdit.voice<noteCountAtStep(track,(uint16_t)pendingSelectionEdit.step)) {
      uint16_t step = (uint16_t)pendingSelectionEdit.step;
      uint8_t voice = pendingSelectionEdit.voice;
      int baseValue = mode == EDIT_VEL ? effectiveVel(track, step, voice)
                    : mode == EDIT_PITCH ? noteAtVoice(track, step, voice)
                    : mode == EDIT_GATE ? effectiveGateTicks(track, step, voice)
                                        : effectiveMicroTick(track, step, voice);
      int value = field.sameValue ? field.target
                                  : draftValue23(mode,baseValue);
      setCommittedStepValue(track, step, voice, mode, value);
    }
  }

  for (uint8_t fieldIndex = 0; fieldIndex < 6; fieldIndex++) {
    PendingSelectionField &field = pendingSelectionEdit.roll[fieldIndex];
    if (!field.touched) continue;
    if (pendingSelectionEdit.range) {
      for (uint8_t track = 0; track < TRACK_COUNT; track++) {
      for (uint16_t step = 0; step < STEP_COUNT; step++) {
        if (!stepSelected[track][step]) continue;
        for (uint8_t voice = 0; voice < noteCountAtStep(track, step); voice++) {
        if (!selectedNoteObject(track, step, voice)) continue;
        TrackRollSettings roll = configuredRollSettings(track, step, voice);
        int value = field.sameValue ? field.target
                                    : draftValue23(4+fieldIndex,rollFieldValue(roll,fieldIndex));
        int16_t fine = fineIndexForVoice(track, step, voice);
        if (fine >= 0) {
          fineNote[track][fine].roll = roll;
          setRollFieldValue(fineNote[track][fine].roll, fieldIndex, value);
          fineNote[track][fine].pxMask |= (uint16_t)(1u << (PX_ROLL_SHIFT + fieldIndex));
        } else {
          setRollFieldValue(roll,fieldIndex,value);
          rollSet906(track,step,roll,stepRollLockMask[track][step]|(1u << fieldIndex));
        }
        }
      }
      }
    } else if (pendingSelectionEdit.step >= 0 &&
               pendingSelectionEdit.voice<noteCountAtStep(track,(uint16_t)pendingSelectionEdit.step)) {
      uint16_t step = (uint16_t)pendingSelectionEdit.step;
      uint8_t voice = pendingSelectionEdit.voice;
      TrackRollSettings roll = configuredRollSettings(track, step, voice);
      int value = field.sameValue ? field.target
                                  : draftValue23(4+fieldIndex,rollFieldValue(roll,fieldIndex));
      int16_t fine = fineIndexForVoice(track, step, voice);
      if (fine >= 0) {
        fineNote[track][fine].roll = roll;
        setRollFieldValue(fineNote[track][fine].roll, fieldIndex, value);
        fineNote[track][fine].pxMask |= (uint16_t)(1u << (PX_ROLL_SHIFT + fieldIndex));
      } else {
        setRollFieldValue(roll,fieldIndex,value);
          rollSet906(track,step,roll,stepRollLockMask[track][step]|(1u << fieldIndex));
      }
    }
  }

  memset(&pendingSelectionEdit, 0, sizeof(pendingSelectionEdit));
  scheduleConfiguredRollApply();
  clearSelectionEditDisplay();
  displayValueDirty = true;
  neoDisplayDirty = true;
}

FLASHMEM void beginNumericEntry(char firstDigit) {
  if (grooveEditMode || (patternMode && selectButtonActive)) return;
  numericEntryActive = true;
  numericEntryLen = 0;
  numericEntryDigits[0] = '\0';
  numericEntryTarget = lastNumericTarget;
  numericEntryTrack = selectedTrack;

  if (numericEntryTarget == NUMERIC_TARGET_PARAM) {
    numericEntryMode = editMode;
    numericEntryStep = selectedStep;

    if (hasSelectedSteps()) {
      numericEntryScope = NUMERIC_SCOPE_SELECTION;
    } else if (currentEditIsPLock()) {
      numericEntryScope = NUMERIC_SCOPE_PLOCK;
    } else {
      numericEntryScope = NUMERIC_SCOPE_DEFAULT;
      numericEntryStep = -1;
    }
  } else {
    numericEntryScope = NUMERIC_SCOPE_DEFAULT;
    numericEntryStep = -1;
  }

  numericEntryDigits[numericEntryLen++] = firstDigit;
  numericEntryDigits[numericEntryLen] = '\0';
  displayValueDirty = true;
}

void appendNumericDigit(char digit) {
  if (!numericEntryActive) {
    beginNumericEntry(digit);
    return;
  }

  if (numericEntryLen >= sizeof(numericEntryDigits) - 1) return;
  numericEntryDigits[numericEntryLen++] = digit;
  numericEntryDigits[numericEntryLen] = '\0';
  displayValueDirty = true;
}

void finishNumericEntry(bool commitValue) {
  if (!numericEntryActive) return;
  if (commitValue && numericEntryLen > 0) discardNoteRedo();

  if (commitValue && numericEntryLen > 0) {
    long entered = atol(numericEntryDigits);

    if (numericEntryTarget == NUMERIC_TARGET_BPM) {
      int value = clampInt((int)entered, (int)BPM_MIN, (int)BPM_MAX);
      bpm = (float)value;
      updateInternalClockPeriod();
      displayBpmDirty = true;
    } else if (numericEntryTarget == NUMERIC_TARGET_TRACK) {
      selectEditingTrack((uint8_t)(clampInt((int)entered, 1, TRACK_COUNT) - 1));
      selectedStep = -1;
      ensureSelectedTrackVisible();
      neoDisplayDirty = true;
    } else if (numericEntryTarget == NUMERIC_TARGET_CHANNEL) {
      (pidiRoutesDirty931=0xffff, trackMidiChannel[linkOwner12(numericEntryTrack,LINK_MOUT12)] = (uint8_t)(clampInt((int)entered, 1, 16) - 1));
    } else if (numericEntryTarget == NUMERIC_TARGET_PITCH_BEND) {
      int value = clampInt((int)entered, 0, 8191);
      trackPitchBend[numericEntryTrack] = (int16_t)value;
    } else {
      int value = clampDirectNumericValue(numericEntryMode, entered);

      if (numericEntryScope == NUMERIC_SCOPE_SELECTION) {
        if (!selectionHasActiveNotes()) {
          // Une plage vide est une vraie selection, mais elle n'est pas une
          // destination d'edition et ne cree aucun historique Undo.
        } else if (!multiEditSameForTrack(numericEntryTrack)) {
          // KEEP : la saisie est un delta, comme une rotation. Elle ne
          // remplace donc jamais les differences propres a chaque note.
          stageSelectedParamValue(numericEntryMode, entered, true);
        } else {
          stageSelectedParamValue(numericEntryMode, value, false);
        }
      } else if (numericEntryScope == NUMERIC_SCOPE_PLOCK && numericEntryStep >= 0) {
        stageSelectedParamValue(numericEntryMode, value, false);
      } else {
        setDirectDefaultValue(numericEntryTrack, numericEntryMode, value);
      }

      neoDisplayDirty = true;
    }
  }

  numericEntryActive = false;
  numericEntryLen = 0;
  numericEntryDigits[0] = '\0';
  displayValueDirty = true;
  displayBpmDirty = true;
}

void handleNumericKey(char key) {
  if (fileBrowserMode == FILE_BROWSER_SAVE) {
    storageNameKey(key);
    return;
  }

  if(!numericEnabled906)return;
  // 8A1 - A/B/C/D restent reserves : UNDO et REDO ont leurs boutons physiques.
  if (key >= '0' && key <= '9') {
    appendNumericDigit(key);
    return;
  }

  if (key == '#') {
    finishNumericEntry(true);
    return;
  }

  if (key == '*') {
    finishNumericEntry(false);
    return;
  }

  // C/D restent reserves hors saisie et navigateur de fichiers.
}

void editTrackRollFromKy(uint8_t kyIndex, int32_t rawDelta) {
  if (kyIndex > 5 || rawDelta == 0) return;
  discardNoteRedo();
  bool pLockEdit = currentEditIsPLock();
  int delta = (kyIndex==3||kyIndex==4)?rawDelta*(curveFine41?1:10):rollDelta819(kyIndex,(int)rawDelta)*(kyIndex==0&&!rollFine60?96:1);
  if(kyIndex==3&&!rollModeChange60){int current=displayedRollForTrack(selectedTrack).expo;if(current>=101)delta=(current==101?102:101)-current;}
  if(!pLockEdit&&!selectionHasActiveNotes()&&indEdit50(IND_ROLL50,selectedTrack,kyIndex,rollFieldValue(trackRoll[selectedTrack],kyIndex),delta,clampRollFieldValue(kyIndex,-32768),clampRollFieldValue(kyIndex,32767)))return;
  if (hasSelectedSteps() && !selectionHasActiveNotes()) {
    // Selection vide : aucun changement et aucun Undo.
  } else if (selectionHasActiveNotes() || pLockEdit) {
    stageSelectedRollDelta(kyIndex, delta);
    markSelectedHeldPadEdited();
  } else {
    TrackRollSettings &roll = trackRoll[selectedTrack];
    setRollFieldValue(roll, kyIndex, kyIndex==0?clampRollFieldValue(0,roll.start+delta):WRAP23(rollFieldValue(roll,kyIndex),delta,clampRollFieldValue(kyIndex,-32768),clampRollFieldValue(kyIndex,32767)));
    scheduleConfiguredRollApply();
  }
  trackRollLastEditedField = kyIndex;
  lastNeoRefreshMs = millis() - NEO_MIN_REFRESH_MS;
  displayValueDirty = true;
}

FLASHMEM void applyNoteVelGateTimeDelta(uint8_t kyIndex, int32_t rawDelta) {
  if (rawDelta != 0) discardNoteRedo();
  setEditModeFromKyTrack(kyIndex);
  if (editMode == EDIT_PITCH) uiEditTarget = UI_EDIT_NOTE;
  else if (editMode == EDIT_VEL) uiEditTarget = UI_EDIT_VEL;
  else if (editMode == EDIT_GATE) uiEditTarget = UI_EDIT_GATE;
  else uiEditTarget = UI_EDIT_TIME;
  lastNumericTarget = NUMERIC_TARGET_PARAM;

  if (false) { // MOV has its own K7 control in VA01.40
    moveSelectedNotesByTicks(selectedTrack, rawDelta*(int32_t)max(1UL,windowTickSpan()/16));
    neoDisplayDirty = true;
    lastNeoRefreshMs = millis() - NEO_MIN_REFRESH_MS;
    displayValueDirty = true;
    return;
  }

  int32_t delta = rawDelta;
  uint8_t track = selectedTrack;


  if(!currentEditIsPLock()&&!selectionHasActiveNotes()&&indNoteEdit50(kyIndex-2,rawDelta*(kyIndex==4&&gateEditMode!=GATE_EDIT_1T?96:1)))return;
  if (editMode == EDIT_PITCH && hasSelectedSteps() && selectionHasActiveNotes()) {
    // 1A2 : une selection prime sur le sous-mode SEL/NOT. NOT transpose les
    // notes selectionnees et la ligne de focus suit exactement le brouillon.
    if (stageSelectedParamDelta(EDIT_PITCH, delta)) {
      selectionPitch = (uint8_t)clampInt((int)selectionPitch + (int)delta, 0, 127);
      selectionPitchValid = true;
      activePitchRow = selectionPitch;
      activePitchTrack = track;
      ensurePitchVisible(activePitchRow);
      activeLineWhiteUntilMs = millis() + ACTIVE_LINE_WHITE_MS;
    }
  } else if (false) { // NOT always changes pitch; preview is independent.
    if (hasSelectedSteps()) {
      // SEL deplace seulement la ligne cible. Un brouillon precedent est
      // d'abord valide sur son ancienne ligne afin de ne jamais changer de
      // cible en cours de transaction.
      if (pendingSelectionEdit.active || (lockDraftAny26()&&!selectButtonActive)) commitPendingSelectionEdit();
      selectionPitch = (uint8_t)clampInt((int)selectionPitch + (int)delta, 0, 127);
      selectionPitchValid = true;
      activePitchRow = selectionPitch;
    } else {
      uint8_t basePitch = activePitchTrack == track && activePitchRow <= 127
                            ? activePitchRow : defaultMidiNote[track];
      activePitchRow = (uint8_t)clampInt((int)basePitch + (int)delta, 0, 127);
      defaultMidiNote[track] = activePitchRow;
    }
    activePitchTrack = track;
    ensurePitchVisible(activePitchRow);
    activeLineWhiteUntilMs = millis() + ACTIVE_LINE_WHITE_MS;
  } else if (hasSelectedSteps() && !selectionHasActiveNotes()) {
    // Selection vide : aucune valeur par defaut et aucune note ne changent.
  } else if (selectionHasActiveNotes() || currentEditIsPLock()) {
    stageSelectedParamDelta(editMode, delta);
    markSelectedHeldPadEdited();
  } else if (editMode == EDIT_VEL) {
    defaultVel[track] = (uint8_t)WRAP23(defaultVel[track],delta,1,127);
  } else if (editMode == EDIT_PITCH) {
    defaultMidiNote[track] = (uint8_t)WRAP23(defaultMidiNote[track],delta,0,127);
    activePitchRow = defaultMidiNote[track];
    activePitchTrack = track;
  } else if (editMode == EDIT_GATE) {
    defaultGateTicks[track] = applyGateDeltaToValue(defaultGateTicks[track], delta);
  } else {
    defaultMicroTick[track] = (int16_t)WRAP23(defaultMicroTick[track],delta,MICRO_TICK_MIN,MICRO_TICK_MAX);
  }

  if (editMode == EDIT_PITCH) {
    uint8_t visiblePitch = hasSelectedSteps() && selectionPitchValid ? selectionPitch
                         : selectedStep >= 0 ? visualNoteAtVoice(track, (uint16_t)selectedStep, selectedVoice)
                         : (activePitchTrack == track && activePitchRow <= 127 ? activePitchRow
                                                                              : defaultMidiNote[track]);
    ensurePitchVisible(visiblePitch);
    if (delta && notePreviewForTrack(track)) {BlockScope21 audition41(blockPlaying21<0?0:blockIndex21());triggerNoteAudition(track, (uint8_t)getCurrentEditedValue());}
  }

  neoDisplayDirty = true;
  lastNeoRefreshMs = millis() - NEO_MIN_REFRESH_MS;
  displayValueDirty = true;
}

FLASHMEM bool applyParamDeltaFromKy(uint8_t kyIndex, int32_t rawDelta) {
 deleteCancel55();
 struct DeleteRotation55 {bool restore;DeleteRotation55():restore(deleteNumeric55&&deleteButtonActive){if(restore)deleteButtonActive=false;}~DeleteRotation55(){if(restore)deleteButtonActive=true;}} deleteRotation55;
  if(patchNoteOffsetEdit44(kyIndex,rawDelta))return true;
  WrapInput23 wrapInput;
  if(kyIndex==4&&rawDelta)gateIncrementShown15=false;
 if(rawDelta)alertAction08();
  if(kbKyR10(kyIndex,rawDelta,false))return true;
  NeoInputScope932 neoScope;
  if(kyIndex==8&&stepContext25()){if(rawDelta){bool held=false;for(auto&panel:neoPadDown)for(bool down:panel)held|=down;if(held)return true;stepLayout25=clampInt(stepLayout25+(rawDelta>0?1:-1),0,3);neoDisplayDirty=displayValueDirty=true;DISPLAY_SERIAL.printf("MSG|STEP TRIX %u\n",stepLayout25+1);}return true;}
  if(patternMode&&!grooveEditMode&&kyIndex==0&&!menuNavigationOnly()&&!modUi805&&!quickView903){programEdit939(rawDelta,false);return true;}
  if(remixMode&&quickView903!=1&&quickView903!=3&&quickView903!=6)return remixKy911(kyIndex,rawDelta);
  if(grooveEditMode&&!menuNavigationOnly())return editGrooveFromKy(kyIndex,rawDelta);
  if(!quickWorking903&&quickKy903(kyIndex,rawDelta))return true;
  if(kyIndex==6&&!quickView903&&!grooveEditMode&&!patchBank808&&!menuNavigationOnly()&&!deleteButtonActive){moveEdit40(rawDelta);return true;}
  if(kyIndex==0&&trackContext815()){
    if(!rawDelta)return false;
    return true; // Track volume removed; use VEL or patch parameters.
    return true;
  }
  if (quickWorking903 && mod805Ky(kyIndex, modUi805==MOD_DELAY806?rawDelta:(rawDelta ? (rawDelta>0?1:-1)*(kyBtnDown[kyIndex]?10:1) : 0))) return true;
  if (quickWorking903 && !patchUi808 && patch808Ky(kyIndex, rawDelta)) return true;
  mod805Invalidate();
  if (patternMode && !grooveEditMode && !menuNavigationOnly() && !deleteButtonActive && pattern805Edit(kyIndex, rawDelta)) return true;
  if (grooveEditMode) return editGrooveFromKy(kyIndex, rawDelta);
  if(kyIndex==4&&freeGateEdit11(rawDelta))return true;
  if (patternMode && selectButtonActive && kyIndex != 0) {
    if (kyIndex != 0 || !selectedPatternCount()) return false;
    int base = pendingPatternGroove;
    if (base < 0) {
      base = 0;
      for (uint8_t t = 0; t < TRACK_COUNT; t++) for (uint8_t p = 0; p < PATTERN_COUNT; p++)
        if (patternSelected[t][p]) { base = patternGroove[t][p]; goto grooveBaseFound; }
    }
    grooveBaseFound:
    // Rotary order OFF, LIS, A01..D16, while preserving the stored suffix.
    int ordinal = base == GROOVE_USE_LIST ? 1 : base ? base + 1 : 0;
    ordinal = clampInt(ordinal + rawDelta, 0, GROOVE_COUNT + 1);
    pendingPatternGroove = ordinal == 1 ? GROOVE_USE_LIST : ordinal > 1 ? ordinal - 1 : 0;
    sendGroovePacket(); neoDisplayDirty = displayValueDirty = true;
    return true;
  }
  if (deleteButtonActive && kyIndex != 0) return false;
  if (rawDelta == 0) return false;
  if (numericEntryActive) return false;
  bool anyTrackMenu = trackMenuActive || trackResetConfirmActive || trackAutoMenuActive ||
                      trackMultiMenuActive || trackPreviewMenuActive || trackPlayMenuActive ||
                      trackColorMenuActive || trackViewMenuActive || trackDrumWarningActive ||
                      trackGateLengthPromptActive;
  if (menuNavigationOnly() && !anyTrackMenu) return false;
  if (anyTrackMenu && kyIndex != 0 && !(quickWorking903 && quickView903==1 && kyIndex==1)) return false;

  switch (kyIndex) {
    case 0: // TRK
      return false; // Direct TRK navigation is KY1 mode + held KY1/Neo column.
    case 1: // CHA
      uiEditTarget = UI_EDIT_CHANNEL;
      lastNumericTarget = NUMERIC_TARGET_CHANNEL;
      changeMidiChannelFromKy(rawDelta);
      return true;
    case 2: // NOT
    case 3: // VEL
    case 4: // GAT
    case 5: // TIM
      applyNoteVelGateTimeDelta(kyIndex, rawDelta);
      return true;
    case 6: // MOV, fixed one tick / sixteenth quantum
      moveEdit40(rawDelta);return true;
    case 7: // PBN
      uiEditTarget = UI_EDIT_PITCH_BEND;
      lastNumericTarget = NUMERIC_TARGET_PITCH_BEND;
      changePitchBendFromKy(rawDelta);
      return true;

    case 8: // K9 = Roll Level
      if(lockDisplay817(LOCK_ROLL1000,selectedTrack,1,rollExtra1000(selectedTrack).v[1]))rollEdit1000(2,rawDelta);else editTrackRollFromKy(5, rawDelta);
      return true;
    case 9: // K10: pre-roll modulation retriggers.
      // Retrigger filters now belong to each modulation on EP.
      return true;
    case 10: // K11 = Roll Start
    case 11: // K12 = Roll Speed
    case 12: // K13 = Roll Accel
    case 13: // K14 = Roll Expo
    case 14: // K15 = Roll Curve
      editTrackRollFromKy(kyIndex - 10, rawDelta);
      return true;
    case 15: // K16: roll velocity exponent.
      rollEdit1000(0,rawDelta);return true;
  }
  return false;
}

FLASHMEM void toggleEditFastFromKy(uint8_t kyIndex, uint32_t heldMs) {
  if(kyIndex==8){rollClick1000(1);return;}
  if(kyIndex==15)return;
  if(kyIndex==9)return;
  if (kyIndex == 0) {
    return;
  }
  if (kyIndex == 6 || kyIndex == 7) {
    uiEditTarget = UI_EDIT_PITCH_BEND;
    lastNumericTarget = NUMERIC_TARGET_PITCH_BEND;
    pitchBendStep44=(pitchBendStep44+1)%3;
    pitchBendStepShown44=true;
    displayValueDirty = true;
    return;
  }
  if (kyIndex < 2 || kyIndex > 5) return;

  setEditModeFromKyTrack(kyIndex);
  if (editMode == EDIT_PITCH) uiEditTarget = UI_EDIT_NOTE;
  else if (editMode == EDIT_VEL) uiEditTarget = UI_EDIT_VEL;
  else if (editMode == EDIT_GATE) uiEditTarget = UI_EDIT_GATE;
  else uiEditTarget = UI_EDIT_TIME;
  lastNumericTarget = NUMERIC_TARGET_PARAM;
  if (editMode == EDIT_PITCH) {
    /* Note preview is switchable during selection. */
    trackNotePreviewMode[selectedTrack] = notePreviewForTrack(selectedTrack) ? 0 : 1;
    DISPLAY_SERIAL.println(notePreviewForTrack(selectedTrack) ? "MSG|PREVIEW ON" : "MSG|PREVIEW OFF");
    displayValueDirty = false;
    return;
  }
  if (editMode == EDIT_GATE) {
    if (heldMs >= 700) {
      uint16_t zoomGate = zoomLevel >= 0
                            ? max((uint16_t)1, (uint16_t)(48u >> zoomLevel))
                            : min((uint16_t)GATE_TICK_MAX, (uint16_t)(48u << (-zoomLevel)));
      if (hasSelectedSteps() && !selectionHasActiveNotes()) {
        // Rien a modifier : l'affichage VAL indiquera ???.
      } else if (selectionHasActiveNotes() || currentEditIsPLock()) {
        stageSelectedParamValue(EDIT_GATE, zoomGate, false);
      } else {
        defaultGateTicks[selectedTrack] = zoomGate;
      }
      displayValueDirty = true;
      return;
    }
    toggleTimeUnit38();gateIncrementShown15=true;
    displayValueDirty = true;
    return;
  }

  if (editMode == EDIT_TIME) {
    timeEncoderMoveMode = false;
    DISPLAY_SERIAL.print(F("MODE,TIME,"));
    DISPLAY_SERIAL.println(timeEncoderMoveMode ? F("MOV") : F("TIM"));
    DISPLAY_SERIAL.print(F("FIELD,5,T,"));
    DISPLAY_SERIAL.println(timeEncoderMoveMode ? F("MOV") : F("TIM"));
  }
  displayValueDirty = true;
}

FLASHMEM void sendKyHelp(uint8_t kyIndex, bool show) {
  DISPLAY_SERIAL.print(F("HELP,"));
  if (!show) {
    DISPLAY_SERIAL.print(F("OFF,"));
    DISPLAY_SERIAL.println(kyIndex);
    return;
  }
  DISPLAY_SERIAL.print(kyIndex);
  DISPLAY_SERIAL.print(',');
  if(quickView903==1){const char*names[]={"Note","Velocity","Gate","Pitch Bend","BLK1 MIDI out","BLK1 Channel","BLK1 Timing","BLK1 Groove list","BLK2 MIDI out","BLK2 Channel","BLK2 Timing","BLK2 Groove list","BLK3 MIDI out","BLK3 Channel","BLK3 Timing","BLK3 Groove list"};DISPLAY_SERIAL.println(names[kyIndex]);}else DISPLAY_SERIAL.println(KY_FULL_NAME[kyIndex]);
}

void serviceKyButtonHelp() {
 lockHoldService26();
 if(controller940)return;uint8_t gateKy21=quickView903==1?2:4;
  if (menuNavigationOnly() || modUi805 || patchUi808 || patchBank808) return;
  // 1A3 : afficher la valeur GAT des que le maintien devient fonctionnel,
  // sans attendre le relachement qui applique encore la valeur liee au zoom.
  if (!kyBtnDown[gateKy21] || gateHoldValueShown || kyRotatedWhileDown[gateKy21]) return;
  if ((uint32_t)(millis() - kyBtnDownAtMs[gateKy21]) < 700) return;
  gateHoldValueShown = true;
  editMode = EDIT_GATE;
  uiEditTarget = UI_EDIT_GATE;
  lastNumericTarget = NUMERIC_TARGET_PARAM;
  sendDisplayValuePacket();
}

void scrollStepView(int32_t rawDelta) {
  if (rawDelta == 0) return;
  int32_t delta = -rawDelta; // Roy : horaire = positif sur les tests precedents
  int32_t next = (int32_t)viewTickOffset + (delta > 0 ? TICKS_PER_STEP : -TICKS_PER_STEP);
  uint32_t maxOffset = activeSequenceTicks() > windowTickSpan() ? activeSequenceTicks() - windowTickSpan() : 0;
  next = clampInt((int)next, 0, (int)maxOffset);
  if ((uint32_t)next == viewTickOffset) return;
  viewTickOffset = (uint32_t)next;
  neoDisplayDirty = true;
  displayBpmDirty = true;
}

void changeZoom(int32_t rawDelta) {
  if (rawDelta == 0) return;
  int32_t delta = -rawDelta; // Roy : horaire = positif
  int8_t oldZoom = zoomLevel;

  if (delta > 0 && zoomLevel < 5) zoomLevel++;
  else if (delta < 0 && zoomLevel > -2) zoomLevel--;

  if (zoomLevel != oldZoom) {
    clampViewStepOffset();
    neoDisplayDirty = true;
    displayBpmDirty = true;
  }
}

void applyBpmDelta(int32_t rawDelta) {
  if (rawDelta == 0) return;
  discardNoteRedo();
  if (numericEntryActive) return;
  lastNumericTarget = NUMERIC_TARGET_BPM;
  int32_t delta = rawDelta; // sens logique deja corrige dans handleRoy
  float oldBpm = bpm;
  bpm = WRAP23((int)lroundf(bpm),delta,(int)BPM_MIN,(int)BPM_MAX);
  if (bpm == oldBpm) return;

  updateInternalClockPeriod();

  if (DEBUG_SERIAL) {
    Serial.print(F("BPM = "));
    Serial.println(bpm, 0);
  }
  displayBpmDirty = true;
}

void clearTrackStep(uint8_t track, uint16_t step) {
  mixerValid930&=~(1u<<track);
  if(!lockMoving817)for(uint8_t v=0;v<noteCountAtStep(track,step);++v)lockRemoveNote817(track,step,v);
  for (uint8_t index = 0; index < FINE_EVENTS_PER_TRACK; index++) {
    if (fineNote[track][index].active && fineNote[track][index].step == step) {
      fineNote[track][index].active = false;
    }
  }
  stepActive[track][step] = false;
  stepExtraNoteCount[track][step] = 0;
  stepVel[track][step] = indNote50(track,1);
  stepMidiNote[track][step] = indNote50(track,0);
  stepGateTicks[track][step] = indNote50(track,2);
  stepLogicalMicro[track][step] = 0;
  stepMicroTick[track][step] = indNote50(track,3);
  stepLockVel[track][step] = false;
  stepLockPitch[track][step] = false;
  stepLockGate[track][step] = false;
  stepLockTime[track][step] = false;
  stepSelected[track][step] = false;
  rollSet906(track,step,emptyRoll906,0);
}

bool captureNoteRecord(uint8_t track, uint16_t step, uint8_t voice, FineNoteEvent &event) {
  if (track >= TRACK_COUNT || step >= STEP_COUNT || voice >= noteCountAtStep(track, step)) return false;
  int16_t fine = fineIndexForVoice(track, step, voice);
  if (fine >= 0) {
    event = fineNote[track][fine];
    return event.active;
  }

  event.active = true;
  event.step = step;
  event.logicalMicro = stepLogicalMicro[track][step];
  event.micro = stepMicroTick[track][step];
  event.note = noteAtVoice(track, step, voice);
  event.vel = effectiveVel(track,step,voice);
  event.gate = effectiveGateTicks(track,step,voice);
  event.micro = effectiveMicroTick(track,step,voice);
  event.pxMask = (uint16_t)((stepLockVel[track][step] ? PX_VEL : 0) |
                            ((voice > 0 || stepLockPitch[track][step]) ? PX_PITCH : 0) |
                            (stepLockGate[track][step] ? PX_GATE : 0) |
                            (stepLockTime[track][step] ? PX_TIME : 0) |
                            ((uint16_t)stepRollLockMask[track][step] << PX_ROLL_SHIFT));
  event.roll = rollRead906(track,step);
  return true;
}

bool sameRollSettings(const TrackRollSettings &left, const TrackRollSettings &right) {
  return left.start == right.start && left.speed == right.speed && left.accel == right.accel &&
         left.expo == right.expo && left.curve == right.curve && left.level == right.level;
}

bool sameNoteRecord(const FineNoteEvent &left, const FineNoteEvent &right) {
  return left.active && right.active && left.step == right.step &&
         left.logicalMicro == right.logicalMicro && left.micro == right.micro &&
         left.note == right.note && left.vel == right.vel && left.gate == right.gate &&
         left.pxMask == right.pxMask && sameRollSettings(left.roll, right.roll);
}

void rememberDeletedNote(uint8_t track, uint16_t step, uint8_t voice) {
  FineNoteEvent event = {};
  if (!captureNoteRecord(track, step, voice, event)) return;
  discardNoteRedo();
  if (deletedNoteHistoryCount >= NOTE_UNDO_LEVELS) {
    lockUndoShift817();
    for (uint8_t index = 1; index < NOTE_UNDO_LEVELS; index++) {
      deletedNoteHistory[index - 1] = deletedNoteHistory[index];
    }
    deletedNoteHistoryCount = NOTE_UNDO_LEVELS - 1;
    deletedNoteHistoryCursor = deletedNoteHistoryCount;
  }
  lockUndoCapture817(deletedNoteHistoryCount,track,step,voice);
  DeletedNoteUndoRecord &record = deletedNoteHistory[deletedNoteHistoryCount++];
  record.valid = true;
  record.restored = false;
  record.track = track;
  record.pattern = activePattern[track];
  record.originalVoice = voice;
  record.restoredFineIndex = -1;
  record.event = event;
  deletedNoteHistoryCursor = deletedNoteHistoryCount;
}

void discardNoteRedo() {
  if (deletedNoteHistoryCursor >= deletedNoteHistoryCount) return;
  for (uint8_t index = deletedNoteHistoryCursor; index < deletedNoteHistoryCount; index++) {
    deleteFree23(index);deletedNoteHistory[index].valid = false;
  }
  deletedNoteHistoryCount = deletedNoteHistoryCursor;
}

bool undoContextMatches(const DeletedNoteUndoRecord &record) {
  return record.valid && record.track < TRACK_COUNT && selectedTrack == record.track &&
         activePattern[record.track] == record.pattern;
}

bool logicalTickAlreadyHasNote(uint8_t track, const FineNoteEvent &wanted) {
  uint8_t count = noteCountAtStep(track, wanted.step);
  for (uint8_t voice = 0; voice < count; voice++) {
    FineNoteEvent current = {};
    if (!captureNoteRecord(track, wanted.step, voice, current)) continue;
    if (current.logicalMicro == wanted.logicalMicro && current.note == wanted.note) return true;
    if (trackPlayMode[linkOwner12(track,LINK_POLY12)] == TRACK_PLAY_MONO && current.logicalMicro == wanted.logicalMicro) return true;
  }
  return false;
}

bool restoreDeletedNote(DeletedNoteUndoRecord &record) {
  uint8_t track = record.track;
  const FineNoteEvent &event = record.event;
  if (event.step >= STEP_COUNT || logicalTickAlreadyHasNote(track, event)) return false;

  if (!stepActive[track][event.step]) {
    clearTrackStep(track, event.step);
    stepActive[track][event.step] = true;
    stepMidiNote[track][event.step] = event.note;
    stepVel[track][event.step] = event.vel;
    stepGateTicks[track][event.step] = event.gate;
    stepLogicalMicro[track][event.step] = event.logicalMicro;
    stepMicroTick[track][event.step] = event.micro;
    stepLockVel[track][event.step] = (event.pxMask & PX_VEL) != 0;
    stepLockPitch[track][event.step] = (event.pxMask & PX_PITCH) != 0;
    stepLockGate[track][event.step] = (event.pxMask & PX_GATE) != 0;
    stepLockTime[track][event.step] = (event.pxMask & PX_TIME) != 0;
    rollSet906(track,event.step,event.roll,(uint8_t)((event.pxMask >> PX_ROLL_SHIFT) & 0x3F));
    record.restoredFineIndex = -1;
    return true;
  }

  for (uint8_t index = 0; index < FINE_EVENTS_PER_TRACK; index++) {
    if (fineNote[track][index].active) continue;
    fineNote[track][index] = event;
    fineNote[track][index].active = true;
    record.restoredFineIndex = index;
    return true;
  }
  return false;
}

bool removeRestoredNote(DeletedNoteUndoRecord &record) {
  uint8_t track = record.track;
  uint16_t step = record.event.step;
  int16_t fine = record.restoredFineIndex;
  if (fine >= 0 && fine < FINE_EVENTS_PER_TRACK && fineNote[track][fine].active) {
    if (!sameNoteRecord(fineNote[track][fine], record.event)) return false;
    uint8_t voice = voiceForFineIndex(track, step, (uint8_t)fine);
    if (voice == 255) return false;
    removeVoiceAtStep(track, step, voice);
    return true;
  }

  FineNoteEvent current = {};
  if (!captureNoteRecord(track, step, 0, current) || !sameNoteRecord(current, record.event)) return false;
  removeVoiceAtStep(track, step, 0);
  return true;
}

void refreshAfterNoteHistory() {
  invalidateStepTriggers();
  neoDisplayDirty = true;
  displayValueDirty = true;
  displayBpmDirty = true;
}

FLASHMEM void performUndo() {
  if(deleteHistory23(false))return;
  if (deletedNoteHistoryCursor == 0) {
    DISPLAY_SERIAL.println(F("MSG|NOTHING TO UNDO"));
    return;
  }
  DeletedNoteUndoRecord &record = deletedNoteHistory[deletedNoteHistoryCursor - 1];
  if (!undoContextMatches(record) || record.restored) {
    DISPLAY_SERIAL.println(F("MSG|UNDO OTHER PATTERN"));
    return;
  }
  if (!lockUndoRoom817(deletedNoteHistoryCursor-1)||!(record.originalVoice==255||restoreDeletedNote(record))) {
    DISPLAY_SERIAL.println(F("MSG|UNDO IMPOSSIBLE"));
    return;
  }
  lockUndoInstall817(deletedNoteHistoryCursor-1);freeInvalidate11();
  record.restored = true;
  deletedNoteHistoryCursor--;
  refreshAfterNoteHistory();
  DISPLAY_SERIAL.println(F("MSG|UNDO NOTE"));
}

FLASHMEM void performRedo() {
  if(deleteHistory23(true))return;
  if (deletedNoteHistoryCursor >= deletedNoteHistoryCount) {
    DISPLAY_SERIAL.println(F("MSG|NOTHING TO REDO"));
    return;
  }
  DeletedNoteUndoRecord &record = deletedNoteHistory[deletedNoteHistoryCursor];
  if (!undoContextMatches(record) || !record.restored) {
    discardNoteRedo();
    DISPLAY_SERIAL.println(F("MSG|NOTHING TO REDO"));
    return;
  }
  if(record.originalVoice==255){uint8_t idx=deletedNoteHistoryCursor;for(uint16_t k=0;k<lockUndoCount817[idx];++k){auto&l=lockUndo817[idx][k];int at=lockFind817(l.track,l.pattern,l.tick,l.note,l.kind,l.target,l.parameter);if(at>=0)noteLocks817[at]=noteLocks817[--lockCount817];}freeInvalidate11();}
  if (record.originalVoice!=255&&!removeRestoredNote(record)) {
    record.valid = false;
    discardNoteRedo();
    DISPLAY_SERIAL.println(F("MSG|REDO IMPOSSIBLE"));
    return;
  }
  record.restored = false;
  deletedNoteHistoryCursor++;
  refreshAfterNoteHistory();
  DISPLAY_SERIAL.println(F("MSG|REDO NOTE"));
}

void copyTrackStep(uint8_t track, uint16_t destination, uint16_t source) {
  stepActive[track][destination] = stepActive[track][source];
  stepVel[track][destination] = stepVel[track][source];
  stepMidiNote[track][destination] = stepMidiNote[track][source];
  stepExtraNoteCount[track][destination] = stepExtraNoteCount[track][source];
  memcpy(stepExtra14[track][destination],stepExtra14[track][source],sizeof(stepExtra14[track][destination]));
  memcpy(stepExtraMidiNote[track][destination], stepExtraMidiNote[track][source],
         sizeof(stepExtraMidiNote[track][destination]));
  stepGateTicks[track][destination] = stepGateTicks[track][source];
  stepLogicalMicro[track][destination] = stepLogicalMicro[track][source];
  stepMicroTick[track][destination] = stepMicroTick[track][source];
  stepLockVel[track][destination] = stepLockVel[track][source];
  stepLockPitch[track][destination] = stepLockPitch[track][source];
  stepLockGate[track][destination] = stepLockGate[track][source];
  stepLockTime[track][destination] = stepLockTime[track][source];
  stepSelected[track][destination] = stepSelected[track][source];
  rollSet906(track,destination,rollRead906(track,source),stepRollLockMask[track][source]);
}

void applyTrackLengthIncrease(uint16_t newBars) {
  uint8_t track = selectedTrack;
  uint16_t oldSteps = trackStepCount(track);
  newBars = (uint16_t)clampInt(newBars, trackBars[track], MAX_BARS);
  uint16_t newSteps = newBars * STEPS_PER_BAR;
  for (uint16_t step = oldSteps; step < newSteps; step++) clearTrackStep(track, step);
  trackBars[track] = newBars;
  sequenceBars = newBars;
  loopEndStep = max(loopEndStep, longestTrackStepCount());
  syncLegacyTrackLoopBounds();
  beatDraftBars = newBars;
  neoDisplayDirty = true;
  displayBpmDirty = true;
}

FLASHMEM void beginBarFillPrompt() {
  if (beatDraftBars <= sequenceBars) return;
  pendingOldBars = sequenceBars;
  pendingNewBars = beatDraftBars;
  barFillPromptActive = true;
  beatEditActive = false;
  char line[21];
  snprintf(line, sizeof(line), "ADD %u TO %u BARS", pendingOldBars, pendingNewBars);
  const char* labels[8] = {"EMPTY", "AUTO", nullptr, nullptr,
                           nullptr, nullptr, nullptr, nullptr};
  char top[21], bottom[21];
  makeEightButtonLines(top, bottom, labels);
  sendMenuScreen("FILL NEW MEASURES?", line, top, "");
}

FLASHMEM void sendBarDeletePrompt() {
  char line0[21], line1[21];
  snprintf(line0, sizeof(line0), "DELETE %u/%u BARS", barDeleteCount, barDeleteOriginalBars);
  snprintf(line1, sizeof(line1), "BARS %u TO %u", barDeleteStartBar + 1,
           barDeleteStartBar + barDeleteCount);
  sendMenuScreen(line0, line1, "", "");
}

FLASHMEM void beginBarDeletePrompt() {
  if (beatDraftBars >= sequenceBars) return;
  barDeletePromptActive = true;
  barDeleteOriginalBars = sequenceBars;
  barDeleteTargetBars = beatDraftBars;
  barDeleteCount = sequenceBars - beatDraftBars;
  // Par defaut, toujours supprimer la difference a la fin.
  barDeleteStartBar = barDeleteTargetBars;
  beatEditActive = false;
  sendBarDeletePrompt();
}

void moveBarDeleteWindow(int32_t delta) {
  if (!barDeletePromptActive || delta == 0) return;
  int maximum = max(0, (int)barDeleteOriginalBars - (int)barDeleteCount);
  barDeleteStartBar = (uint16_t)clampInt((int)barDeleteStartBar + (delta > 0 ? 1 : -1), 0, maximum);
  sendBarDeletePrompt();
}

FLASHMEM void commitBarDelete() {
  if (!barDeletePromptActive) return;
  uint8_t track = selectedTrack;
  if (track >= TRACK_COUNT) {
    barDeletePromptActive = false;
    showGameScreen();
    return;
  }
  sendMenuScreen("PROCESSING", "PLEASE WAIT", "", "");
  uint16_t startBar = barDeleteStartBar;
  if (startBar + barDeleteCount > barDeleteOriginalBars) startBar = barDeleteOriginalBars - barDeleteCount;
  uint16_t startStep = startBar * STEPS_PER_BAR;
  uint16_t removedSteps = barDeleteCount * STEPS_PER_BAR;
  uint16_t oldSteps = barDeleteOriginalBars * STEPS_PER_BAR;
  for (uint16_t destination = startStep; destination + removedSteps < oldSteps; destination++) {
    copyTrackStep(track, destination, destination + removedSteps);
  }
  for (uint16_t step = oldSteps - removedSteps; step < oldSteps; step++) clearTrackStep(track, step);
  trackBars[track] = barDeleteTargetBars;
  sequenceBars = trackBars[track];
  uint16_t longestSteps = longestTrackStepCount();
  loopStartStep = min(loopStartStep, longestSteps);
  loopEndStep = (uint16_t)clampInt(loopEndStep, loopStartStep, longestSteps);
  syncLegacyTrackLoopBounds();
  beatDraftBars = sequenceBars;
  beatEditLength = false;
  barDeletePromptActive = false;
  pendingViewSource = VIEW_SRC_LENGTH;
  clampViewStepOffset();
  measureTick = localTickForTrack(track);
  currentStep = (uint16_t)(measureTick / TICKS_PER_STEP);
  showGameScreen();
  neoDisplayDirty = true;
  displayBpmDirty = true;
}

FLASHMEM void sendPatternModeEditor() {
  sendMenuScreen("PATTERN CHANGE", patternModeDraftNext ? "NEXT" : "CONT", "",
                 "NEXT waits for the end; CONT keeps the current position");
}

FLASHMEM void openPatternModeEditor() {
  patternModeEditActive = true;
  patternModeDraftNext = patternChangeNext;
  sendPatternModeEditor();
}

FLASHMEM void commitPatternModeEditor() {
  patternChangeNext = patternModeDraftNext;
  patternModeEditActive = false;
  showGameScreen();
  DISPLAY_SERIAL.println(patternChangeNext ? F("MSG|PATT NEXT") : F("MSG|PATT CONT"));
}

void reportDebugRoy(uint8_t encoderIndex, int32_t position, int32_t delta,
                    bool buttonEvent, bool pressed) {
  char line0[21], line1[21], line2[21], line3[21], small0[17], small1[17];
  snprintf(line0, sizeof(line0), "ROY TEST R%u", encoderIndex + 1);
  snprintf(line1, sizeof(line1), "I2C 0x%02X ENC%u", ROY_ADDR, encoderIndex);
  snprintf(line2, sizeof(line2), "POS%ld DELTA%+ld", (long)position, (long)delta);
  snprintf(line3, sizeof(line3), "SW%u %s BE1 BACK", ROY_SWITCH_PIN[encoderIndex],
           buttonEvent ? (pressed ? "ON" : "OFF") : "-");
  snprintf(small0, sizeof(small0), "R%u I2C0x%02X", encoderIndex + 1, ROY_ADDR);
  if (buttonEvent) snprintf(small1, sizeof(small1), "SW%u %s", ROY_SWITCH_PIN[encoderIndex], pressed ? "ON" : "OFF");
  else snprintf(small1, sizeof(small1), "P%ld D%+ld", (long)position, (long)delta);
  sendDebugDisplay(line0, line1, line2, line3, small0, small1);

  Serial.print(F("DBG,ROY,R=")); Serial.print(encoderIndex + 1);
  Serial.print(F(",ADDR=0x")); Serial.print(ROY_ADDR, HEX);
  Serial.print(F(",SWPIN=")); Serial.print(ROY_SWITCH_PIN[encoderIndex]);
  Serial.print(F(",POS=")); Serial.print(position);
  Serial.print(F(",DELTA=")); Serial.print(delta);
  if (buttonEvent) {
    Serial.print(F(",BUTTON=")); Serial.println(pressed ? 1 : 0);
  } else {
    Serial.println();
  }
}

bool validateSettingsWithRoy(uint8_t logicalRoy) {
 if(oovMenu16){if(oovRoy16==(int8_t)logicalRoy)oovClose16(true);return true;}
 if(trackPage933){trackRoy933(logicalRoy,0,true);return true;}
 if(assignOpen932){if(logicalRoy==0&&assignDirty60())assignChoice932(assignReplace60);return true;}
 if(settingsOpen&&settingsMenuLevel==SETTINGS_COLOR_EDIT){colorsClick11(logicalRoy);return true;}
  if(settingsOpen&&settingsMenuLevel==SETTINGS_MIXER_EDIT915){save902();settingsMenuLevel=SETTINGS_COLOR_ITEMS;settingsEditingRoy=-1;sendSettingsPage();return true;}
  if (!settingsOpen || settingsEditingRoy != (int8_t)logicalRoy) return false;
  if (settingsMenuLevel == SETTINGS_EDIT) commitSelectedSetting();
  else if (settingsMenuLevel == SETTINGS_PICO_EDIT) commitSelectedPicoSetting();
  else if (settingsMenuLevel == SETTINGS_COLOR_EDIT) commitSelectedColor();
  else if (settingsMenuLevel == SETTINGS_ROY_EDIT) commitSelectedRoySetting();
  else if (settingsMenuLevel == SETTINGS_SEQ_EXTRA_EDIT) commitSelectedSeqExtraSetting();
  else if (settingsMenuLevel == SETTINGS_TRANSPORT_EDIT) commitTransportSetting();
  else return false;
  settingsEditingRoy = -1;
  return true;
}

uint32_t roySwitchBits821=0xFFFFFFFF;bool royBulkValid821=false;
FLASHMEM void royEdge831(uint8_t encoderIndex,bool pressed) { if(pressed)presentationTouch43();
 if(pinTestActive33)return;
 if(debugTestMode==DEBUG_TEST_CV47){if(pressed)cvDebugRoy47(3-encoderIndex,0,true);return;}
 EpRefreshR10 refresh; if(kbMenuR10){if(pressed)kbRoyR10(3-encoderIndex,0,true);return;}
 if(controller940){if(encoderIndex==3&&pressed)controllerEmit940("CUT",0,1);return;}
  uint32_t now=millis();
    uint8_t logicalRoy = 3 - encoderIndex;
    if(patMenu46&&!patchUi808){if(pressed)patRoy46(logicalRoy,0,true);return;}
    if(writeMenuActive){if(pressed&&logicalRoy==0)applyWriteToSequence();return;}
    if(lockHoldEdge26(16+logicalRoy,pressed))return;
    if(sleepMenu43){if(pressed&&logicalRoy==0){sleepSave43();sleepMenu43=false;sendSettingsPage();}return;}
    if(screenMapMenu){if(pressed&&logicalRoy==2)screenMapClose(true);return;}
    if(linkMenu12||trackMidiMenu902)return;
    inputLog815(I815_ROY,logicalRoy,pressed,I815_STABLE);
    InputScope815 input(I815_ROY,logicalRoy,pressed);
    if (!settingsOpen && !modUi805 && !patchUi808 && deleteButtonActive) return;
    if (settingsOpen && settingsMenuLevel == SETTINGS_ROY_EDIT) {
      debugRoyTestEncoder = logicalRoy;
      if (pressed && (uint32_t)(now - roySwLastToggleMs[encoderIndex]) >= royFunctionGuardMs) {
        roySwLastToggleMs[encoderIndex] = now;
        debugRoyTestClick = true;
      } else {
        debugRoyTestClick = false;
      }
      sendSettingsPage();
      return;
    }
    if (buttonSettingsLiveTest()) {
      if (pressed && logicalRoy == 0) commitSelectedSetting();
      else if (pressed && logicalRoy == 1) backSettingsMenu();
      return;
    }
    if(debugTestMode==DEBUG_TEST_CV47){if(pressed)cvDebugRoy47(logicalRoy,0,true);return;}
    if (debugTestMode == DEBUG_TEST_ROY) {
      reportDebugRoy(encoderIndex, royEncoderPos[encoderIndex], 0, true,
                     pressed);
      return;
    }
    if (debugTestMode != DEBUG_TEST_NONE) {
      bool royExitTest = debugTestMode == DEBUG_TEST_BE ||
                         debugTestMode == DEBUG_TEST_BUTTONS ||
                         debugTestMode == DEBUG_TEST_TRANSPORT ||
                         debugTestMode == DEBUG_TEST_CLIPBOARD ||
                         debugTestMode == DEBUG_TEST_SCREENS;
      if (pressed && logicalRoy == 0 && royExitTest) {
        stopDebugTest();
        sendSettingsPage();
      }
      return;
    }
    if (pressed && (modUi805 || patchUi808 || (uint32_t)(now - roySwLastToggleMs[encoderIndex]) >= royFunctionGuardMs)) {
      roySwLastToggleMs[encoderIndex] = now;alertAction08();inputActivity811=now;

      if(midiInMenu17)return;
      if(trackPage933){trackRoy933(logicalRoy,0,true);return;}
      if (delayLabelRoy07(logicalRoy,0,true) || itemRoy809(logicalRoy,0,true) || patch808Roy(logicalRoy,0,true) || mod806Roy(logicalRoy,0,true)) {
        // Live modulation UI owns every Roy, including unused controls.
      } else if (logicalRoy == 0 && clipboardMenuActive) {
        clipboardCopyScope[clipboardMenuSlot] = (ClipboardCopyScope)clipboardMenuRecentIndex;
        captureTrackIntoClipboard(clipboardMenuSlot);
        closeClipboardMenu();
      } else if (logicalRoy == 0 && saveMenuActive) {
        confirmSaveMenu();
      } else if (logicalRoy == 0 && fileBrowserMode != FILE_BROWSER_NONE) {
        handleFileBrowserScreenButton(0);
      } else if (logicalRoy == 0 && settingsOpen && !assignOpen932 &&
                 (settingsMenuLevel == SETTINGS_CATEGORIES || settingsMenuLevel == SETTINGS_ITEMS)) {
        openOrCommitSettingsSelection();
      } else if (songMode && !settingsOpen && !loadTypeMenuActive && !writeMenuActive) {
        if (logicalRoy == 2) { loopEditStart = !loopEditStart; songState(); }
      } else if (barDeletePromptActive) {
        if (logicalRoy == 1) commitBarDelete();
      } else if (patternModeEditActive) {
        if (logicalRoy == 3) commitPatternModeEditor();
      } else if (trackGateLengthPromptActive) {
        if (logicalRoy == 0) applyTrackPlayModeConversion();
      } else if (settingsOpen) {
        validateSettingsWithRoy(logicalRoy);
      } else if (trackColorMenuActive) {
        if (trackMenuEditingRoy == (int8_t)logicalRoy) {
          // Les couleurs par piste ont ete retirees.
          trackMenuEditingRoy = -1;
          saveSettingsToEEPROM();
          neoDisplayDirty = true;
          sendTrackMenu();
        }
      } else if(trackResetConfirmActive){if(logicalRoy==0&&trackInitApply57())showGameScreen();
      } else if (trackMenuActive || trackAutoMenuActive || trackMultiMenuActive ||
                 trackPreviewMenuActive || trackPlayMenuActive || trackViewMenuActive) {
        // Les autres valeurs directes du menu Track restent sur les BE.
      } else if (menuNavigationOnly()) {
        // Tout clic sans fonction de navigation est absorbe dans tous les menus.
      } else {
        if (!selectButtonActive && !shiftButtonActive && cancelStepSelection()) return;
        if (logicalRoy == 0) {
          roy1EditsZoom = !roy1EditsZoom;
          displayBpmDirty = true;
        } else if (logicalRoy == 1) {
          if (!beatEditLength) {
            beatEditActive = true;
            beatEditLength = true;
            beatOriginalBars = sequenceBars;
            beatDraftBars = sequenceBars;
          } else if (beatDraftBars < sequenceBars) {
            beginBarDeletePrompt();
          } else if (beatDraftBars > sequenceBars) {
            beginBarFillPrompt();
          } else {
            beatEditActive = false;
            beatEditLength = false;
          }
          displayBpmDirty = true;
        // Each loop boundary has its own additive edit quantum.
        } else if (logicalRoy == 2) {
          loopBarMode817[0] = !loopBarMode817[0];
          displayBpmDirty = true;
        } else if (logicalRoy == 3) {
          loopBarMode817[1] = !loopBarMode817[1];
          displayBpmDirty = true;
        }
      }
    }
}
FLASHMEM void handleRoySwitch(uint8_t encoderIndex) {
  if (!royOk) return;
  if (encoderIndex >= ROY_ENCODER_COUNT || (pc831.royMask&(1u<<(3-encoderIndex))) || (pc831.royDown&(1u<<(3-encoderIndex)))) return;

  uint32_t now = millis();
  bool raw = royBulkValid821 ? (roySwitchBits821&(1UL<<ROY_SWITCH_PIN[encoderIndex]))!=0 : roy.digitalRead(ROY_SWITCH_PIN[encoderIndex]);

  if (raw != roySwLastRawState[encoderIndex]) {
    inputLog815(I815_ROY,3-encoderIndex,raw==LOW,I815_RAW);
    roySwLastRawState[encoderIndex] = raw;
    roySwLastRawChangeMs[encoderIndex] = now;
  }

  if ((uint32_t)(now - roySwLastRawChangeMs[encoderIndex]) < roySwitchDebounceMs) return;

  if (raw != roySwStableState[encoderIndex]) {
    roySwStableState[encoderIndex] = raw;
    royEdge831(encoderIndex, raw == LOW);
  }
}

FLASHMEM void royTurn831(uint8_t e,int32_t uiDelta,int32_t pos,int32_t delta) { if(uiDelta||delta)presentationTouch43();
 if(pinTestActive33){if(e==3&&uiDelta)pinTestTurn33(uiDelta);return;}
 if(debugTestMode==DEBUG_TEST_CV47){if(uiDelta)cvDebugRoy47(3-e,uiDelta,false);return;}
 WrapInput23 wrapInput;
 if(uiDelta||delta){alertAction08();if(delayIncrementShown925){delayIncrementShown925=0;delayMenuDirty817=modDisplayDirty810=displayValueDirty=true;}}
 EpRefreshR10 refresh; if(delayLabelRoy07(3-e,uiDelta,false)||kbRoyR10(3-e,uiDelta,false))return;
  uint8_t logicalRoy=3-e; int32_t logicalDelta=-uiDelta;
  if(debugTestMode==DEBUG_TEST_CV47){if(uiDelta)cvDebugRoy47(logicalRoy,uiDelta,false);return;}
  if(uiDelta)lockHoldTurn26(16+logicalRoy);
  if(uiDelta)presentationTouch43();
  if(sleepMenu43){if(logicalRoy==0&&uiDelta){sleepDraft43=constrain((int)sleepDraft43+(uiDelta>0?1:-1),0,60);sleepRender43();}return;}
  if(screenMapMenu){screenMapTurn(logicalRoy,uiDelta);return;}
  if(oovMenu16){if(uiDelta)oovTurn16(logicalRoy,uiDelta);return;}
  if(controller940){if(logicalRoy==0&&uiDelta)controllerEmit940("CUR",0,uiDelta);return;}
  if(linkMenu12){if(logicalRoy==0&&uiDelta){if(linkMask12[selectedTrack]){DISPLAY_SERIAL.println("MSG|UNLINK BEFORE SOURCE");return;}linkDraft12=clampInt((int)linkDraft12+(uiDelta>0?1:-1),0,15);linkRender12();}return;}
  if(patMenu46&&!patchUi808){patRoy46(logicalRoy,uiDelta,false);return;}
  if(midiInMenu17){midiInRoy17(logicalRoy,uiDelta);return;}
  if(trackPage933){trackRoy933(logicalRoy,uiDelta,false);return;}
  if(trackMenuActive&&!trackMidiMenu902&&!trackResetConfirmActive&&!trackAutoMenuActive&&!trackMultiMenuActive&&!trackPreviewMenuActive&&!trackPlayMenuActive&&!trackColorMenuActive&&!trackViewMenuActive&&logicalRoy==1){selectEditingTrack(clampInt((int)selectedTrack+(uiDelta>0?1:-1),0,15));sendTrackMenu();return;}
  if(trackMidiMenu902)return;

      // Pendant une saisie au digipad, on absorbe les mouvements sans les
      // appliquer. La position reste synchronisee, donc aucun saut a la fin.
      if (!modUi805 && !patchUi808 && (numericEntryActive || deleteButtonActive) && fileBrowserMode!=FILE_BROWSER_SAVE) return;

      if(itemChoosing809){if(uiDelta)itemRoy809(logicalRoy,uiDelta,false);}
      else if(patchUi808){if(uiDelta)patch808Roy(logicalRoy,uiDelta,false);}
      else if(modUi805){if(uiDelta)mod806Roy(logicalRoy,uiDelta,false);}
      else if (settingsOpen && settingsMenuLevel == SETTINGS_ROY_EDIT) {
        if (logicalDelta != 0) {
          debugRoyTestEncoder = logicalRoy;
          debugRoyTestDelta = logicalDelta;
          debugRoyTestValue += logicalDelta;
          debugRoyTestClick = false;
          sendSettingsPage();
        }
      } else if (buttonSettingsLiveTest()) {
        if (logicalRoy == 0 && uiDelta != 0) {
          settingsEditingRoy = logicalRoy;
          editSelectedSetting(uiDelta);
        }
      } else if (debugTestMode == DEBUG_TEST_ROY) {
        reportDebugRoy(e, pos, delta, false, false);
      } else if (debugTestMode != DEBUG_TEST_NONE) {
        // Un test materiel ne doit jamais modifier les reglages musicaux.
      } else if (logicalDelta == 0) {
        // Accumulation jusqu'au prochain cran logique.
      } else if (trackGateLengthPromptActive) {
        if (logicalRoy == 0) {
          int change = uiDelta > 0 ? 1 : -1;
          trackGateLengthDraft = (uint16_t)WRAP23(trackGateLengthDraft,change,GATE_TICK_MIN,GATE_TICK_MAX);
          sendGateLengthPrompt();
        }
      } else if (trackDrumWarningActive) {
        // La confirmation DRUM utilise uniquement CANC/YES.
      } else if (logicalRoy == 0 && clipboardMenuActive) {
        int next = (int)clipboardMenuRecentIndex + (uiDelta > 0 ? 1 : -1);
        while (next < 0) next += 4;
        clipboardMenuRecentIndex = (uint8_t)(next % 4);
        clipboardCopyScope[clipboardMenuSlot] = (ClipboardCopyScope)clipboardMenuRecentIndex;
        sendClipboardMenu();
      } else if (logicalRoy == 0 && saveMenuActive) {
        moveSaveMenu(uiDelta > 0 ? 1 : -1);
      } else if (fileBrowserMode == FILE_BROWSER_SAVE && logicalRoy<2) {
        storageNameTurn(logicalRoy,uiDelta>0?1:-1);
      } else if (logicalRoy == 0 && fileBrowserMode == FILE_BROWSER_LOAD) {
        cycleBrowserFile(uiDelta > 0 ? 1 : -1);
        renderFileBrowser();
      } else if (logicalRoy == 1 && fileBrowserMode == FILE_BROWSER_SAVE) {
        // OB parcourt les noms existants, le plus recent etant l'index zero.
        cycleBrowserFile(uiDelta > 0 ? 1 : -1);
        renderFileBrowser();
      } else if (logicalRoy == 0 && clipboardPasteMenuActive) {
        uint8_t slot = clipboardPasteMenuSlot;
        clipboardPasteMode[slot] = clipboardPasteMode[slot] == CLIP_PASTE_ERASE
                                     ? CLIP_PASTE_OVERDUB : CLIP_PASTE_ERASE;
        sendClipboardPasteMenu(slot);
      } else if (logicalRoy == 0 && settingsOpen && !assignOpen932 &&
                 (settingsMenuLevel == SETTINGS_CATEGORIES || settingsMenuLevel == SETTINGS_ITEMS)) {
        moveSettingsSelection(uiDelta > 0 ? 1 : -1);
      } else if (logicalRoy == 3 && !patternModeEditActive &&
                 (settingsOpen || saveMenuActive || loadTypeMenuActive || writeMenuActive ||
                  fileBrowserMode != FILE_BROWSER_NONE || clipboardMenuActive ||
                  clipboardPasteMenuActive || trackMenuActive || trackResetConfirmActive ||
                  trackAutoMenuActive || trackMultiMenuActive || trackPreviewMenuActive ||
                  trackPlayMenuActive || trackColorMenuActive || trackViewMenuActive ||
                  barFillPromptActive || barDeletePromptActive) &&
                 scrollMenuText(uiDelta)) {
        // Roy4 est reserve au texte lorsqu'une explication depasse 20 caracteres.
      } else if (barDeletePromptActive) {
        if (logicalRoy == 1) moveBarDeleteWindow(uiDelta);
      } else if (patternModeEditActive) {
        if (logicalRoy == 3) {
          patternModeDraftNext = uiDelta > 0;
          sendPatternModeEditor();
        }
      } else if(assignOpen932){if(uiDelta)assignTurn932(logicalRoy,uiDelta);
      } else if(settingsOpen&&settingsMenuLevel==SETTINGS_COLOR_EDIT){if(uiDelta)colorsTurn932(logicalRoy,uiDelta);
      } else if(settingsOpen&&settingsMenuLevel==SETTINGS_MIXER_EDIT915){
        if(uiDelta){modeColors902[logicalRoy]=clampInt(modeColors902[logicalRoy]+(uiDelta>0?1:-1),0,NEO_PALETTE_COUNT-1);neoDisplayDirty=true;sendSettingsPage();}
      } else if (settingsOpen) {
        // Une valeur simple appartient toujours a Roy1. Les autres Roy ne
        // peuvent donc jamais modifier accidentellement le meme brouillon.
        if (logicalRoy == 0) {
          settingsEditingRoy = logicalRoy;
          if (settingsMenuLevel == SETTINGS_EDIT) editSelectedSetting(uiDelta);
          else if (settingsMenuLevel == SETTINGS_PICO_EDIT) editSelectedPicoSetting(uiDelta);
          else if (settingsMenuLevel == SETTINGS_COLOR_EDIT) editSelectedColor(uiDelta);
          else if (settingsMenuLevel == SETTINGS_ROY_EDIT) editSelectedRoySetting(uiDelta, logicalRoy);
          else if (settingsMenuLevel == SETTINGS_SEQ_EXTRA_EDIT) editSelectedSeqExtraSetting(uiDelta, logicalRoy);
          else if (settingsMenuLevel == SETTINGS_TRANSPORT_EDIT) editTransportSetting(uiDelta, logicalRoy);
        }
      } else if (trackColorMenuActive) {
        if (logicalRoy == 0) {
          trackMenuEditingRoy = logicalRoy;
          int nextRole = (int)trackColorRole + (uiDelta > 0 ? 1 : -1);
          trackColorRole = (uint8_t)clampInt(nextRole, 0, NEO_COLOR_REMIX);
          trackColorDraft = -1;
          sendTrackColorMenu();
        } else if (logicalRoy == 1) {
          trackMenuEditingRoy = logicalRoy;
          int next = (int)trackColorDraft + (uiDelta > 0 ? 1 : -1);
          trackColorDraft = (int8_t)clampInt(next, -1, NEO_PALETTE_COUNT - 1);
          sendTrackColorMenu();
        }
      } else if (trackMenuActive || trackResetConfirmActive || trackAutoMenuActive || trackMultiMenuActive ||
                 trackPreviewMenuActive || trackPlayMenuActive || trackViewMenuActive) {
        // Les autres menus Track utilisent les BE; absorber les rotations musicales.
      } else if (saveMenuActive || loadTypeMenuActive || writeMenuActive ||
                 fileBrowserMode != FILE_BROWSER_NONE || clipboardMenuActive ||
                 clipboardPasteMenuActive || barFillPromptActive) {
        // Dans un menu, aucun Roy non affecte ne doit modifier BPM, zoom,
        // position, boucle ou pattern en arriere-plan.
      } else if (songMode) {
        songRoy(logicalRoy,uiDelta>0?1:-1);
      } else {
        if (logicalRoy == 0) { // Roy1 partage BPM et zoom
          if (roy1EditsZoom) {
            zoomAroundSelection(uiDelta > 0 ? 1 : -1, VIEW_SRC_ROY1_ZOOM);
          } else {
            applyBpmDelta(uiDelta);
          }
        } else if (logicalRoy == 1) { // Roy2 = mesure affichee
          if (beatEditLength) {
            int magnitude = abs((int)uiDelta);
            int jump = magnitude >= 8 ? 16 : (magnitude >= 4 ? 4 : 1);
            uint16_t nextBars = (uint16_t)WRAP23(beatDraftBars,uiDelta>0?jump:-jump,1,MAX_BARS);
            beatDraftBars = nextBars;
            displayBpmDirty = true;
          } else {
            uint32_t navigationSpan = max(1UL,windowTickSpan()/2);
            int segment = viewTickOffset / max((uint32_t)1, navigationSpan);
            int magnitude = abs((int)uiDelta);
            int jump = magnitude >= 8 ? 16 : (magnitude >= 4 ? 4 : 1);
            uint32_t sequenceTicks = activeSequenceTicks();
            uint32_t visibleSpan = monoMeasureRowsView(selectedTrack)
                                     ? navigationSpan * 4u : navigationSpan;
            uint32_t maxOffset = sequenceTicks > visibleSpan ? sequenceTicks - visibleSpan : 0;
            int maxSegment = (int)(maxOffset / max((uint32_t)1, navigationSpan));
            segment = clampInt(segment + (uiDelta > 0 ? jump : -jump), 0, maxSegment);
            viewTickOffset = (uint32_t)segment * navigationSpan;
            pendingViewSource = VIEW_SRC_ROY2;
            clampViewStepOffset();
            neoDisplayDirty = true;
            displayBpmDirty = true;
          }
        } else if (logicalRoy == 2 || logicalRoy == 3) {
          uint8_t bound=logicalRoy-2;
          int change=(uiDelta>0?1:-1)*(loopBarMode817[bound]?16:1);
          if(!bound)loopStartStep=WRAP23(loopStartStep,change,1,loopEndStep);
          else loopEndStep=WRAP23(loopEndStep,change,loopStartStep,longestTrackStepCount());
          syncLegacyTrackLoopBounds();displayBpmDirty=true;
        }
      }
}
FLASHMEM void handleRoy() {
  WrapInput23 wrapInput;
  if (!royOk) return;

  uint32_t now = millis();
  if ((uint32_t)(now - royLastPollMs) < ROY_POLL_INTERVAL_MS) return;
  royLastPollMs = now;
  uint32_t mask821=0;for(uint8_t i=0;i<ROY_ENCODER_COUNT;++i)mask821|=1UL<<ROY_SWITCH_PIN[i];
  roySwitchBits821=roy.digitalReadBulk(mask821);royBulkValid821=true;
  if(optim31&16)serviceRealtimeBetweenNeoPanels();

  for (uint8_t e = 0; e < ROY_ENCODER_COUNT; e++) {
    int32_t pos = roy.getEncoderPosition(e);
    if(optim31&16)serviceRealtimeBetweenNeoPanels();
    if(sdBusy905()||(pc831.royMask&(1u<<(3-e)))){royEncoderPos[e]=pos;continue;}
    int32_t delta = pos - royEncoderPos[e];
    if (delta != 0) {
      royEncoderPos[e] = pos;
      royRotationAccumulator[e] = (int8_t)clampInt((int)royRotationAccumulator[e] + (int)delta, -64, 64);
      int32_t valueDelta = royRotationAccumulator[e] / max((uint8_t)1, royPulsesPerValue);
      royRotationAccumulator[e] -= valueDelta * max((uint8_t)1, royPulsesPerValue);
      uint8_t logicalRoy = 3 - e;
      int32_t logicalDelta = valueDelta * royRotationDirection;
      // Une seule convention physique pour tous les Roy. uiDelta reprend le
      // sens valide par l'edition de longueur des mesures.
      int32_t uiDelta = -logicalDelta;

      royTurn831(e,uiDelta,pos,delta);
    }

    handleRoySwitch(e);
  }
}

// ============================================================
// PICO UART PARSER
// Format attendu : P1/P2,E,index,value,delta ou P1/P2,B,index,pressed
// ============================================================
void sendPicoDefaultsToPort(HardwareSerial &port, uint8_t role);

struct PicoRxStats {
  uint32_t encoderEvents;
  uint32_t sequenceGaps;
  uint32_t recoveredSteps;
  uint32_t badLines;
  uint16_t maxBacklog;
  int32_t lastValue[8];
  bool valueKnown[8];
};

PicoRxStats picoRxStats[2] = {};

void resetPicoRxStats() {
  memset(picoRxStats, 0, sizeof(picoRxStats));
}

bool picoRecoveryEnabled() {
  if (settingsOpen && settingsMenuLevel == SETTINGS_PICO_EDIT &&
      settingsItemSelection == 3) return settingsDraftValue != 0;
  return picoRecoveryMode != 0;
}

uint8_t kyFromPicoIndex(uint8_t picoNumber, int indexFromMsg) {
  // Les logs Pico confirment un format brut 0..7.
  // Important : ne PAS accepter 1..8 ici, sinon index 1 devient doublon de index 0.
  if (indexFromMsg < 0 || indexFromMsg > 7) return 255;

  if (picoNumber == 1) return (uint8_t)indexFromMsg;       // KY1..KY8  -> 0..7
  if (picoNumber == 2) return (uint8_t)(8 + indexFromMsg); // KY9..KY16 -> 8..15
  return 255;
}

void reportDebugKy40(uint8_t picoNumber, uint8_t kyIndex, int32_t value,
                     int delta, bool buttonEvent, bool pressed) {
  uint8_t localIndex = kyIndex % 8;
  PicoRxStats &rx = picoRxStats[picoNumber - 1];
  char line0[21], line1[21], line2[21], line3[21], small0[17], small1[17];
  snprintf(line0, sizeof(line0), "KY40 TEST K%02u", kyIndex + 1);
  snprintf(line1, sizeof(line1), "PICO%u IDX%u", picoNumber, localIndex);
  snprintf(line2, sizeof(line2), "E%lu G%lu Q%u",
           (unsigned long)rx.encoderEvents, (unsigned long)rx.sequenceGaps,
           rx.maxBacklog);
  if (buttonEvent) snprintf(line2, sizeof(line2), "BOUTON %s", pressed ? "PRESSE" : "RELACHE");
  else snprintf(line2, sizeof(line2), "VAL%ld DELTA%+d", (long)value, delta);
  snprintf(line3, sizeof(line3), "BE1 BACK");
  snprintf(small0, sizeof(small0), "K%02u P%u I%u", kyIndex + 1, picoNumber, localIndex);
  if (buttonEvent) snprintf(small1, sizeof(small1), "SW%u %s", KY40_SW_PIN[localIndex], pressed ? "ON" : "OFF");
  else snprintf(small1, sizeof(small1), "V%ld D%+d", (long)value, delta);
  sendDebugDisplay(line0, line1, line2, line3, small0, small1);

  Serial.print(F("DBG,KY40,K=")); Serial.print(kyIndex + 1);
  Serial.print(F(",PICO=")); Serial.print(picoNumber);
  Serial.print(F(",IDX=")); Serial.print(localIndex);
  Serial.print(F(",CLK=")); Serial.print(KY40_CLK_PIN[localIndex]);
  Serial.print(F(",DT=")); Serial.print(KY40_DT_PIN[localIndex]);
  Serial.print(F(",SWPIN=")); Serial.print(KY40_SW_PIN[localIndex]);
  if (buttonEvent) {
    Serial.print(F(",BUTTON=")); Serial.println(pressed ? 1 : 0);
  } else {
    Serial.print(F(",VALUE=")); Serial.print(value);
    Serial.print(F(",DELTA=")); Serial.println(delta);
  }
}

FLASHMEM void handlePicoLine(uint8_t picoNumber, char* line) {
  WrapInput23 wrapInput;
  if(sdBusy905())return;
  if (line == nullptr || line[0] == '\0') return;

  // Un Pico KY qui vient de redemarrer reste PX et redemande son role toutes
  // les secondes. Cela evite de perdre l'unique configuration du Teensy si
  // elle a ete envoyee pendant le delai de boot USB du Pico.
  if (strcmp(line, "PX,H") == 0) {
    memset(picoRxStats[picoNumber - 1].valueKnown, 0,
           sizeof(picoRxStats[picoNumber - 1].valueKnown));
    if (picoNumber == 1) sendPicoDefaultsToPort(PICO1_SERIAL, 1);
    else if (picoNumber == 2) sendPicoDefaultsToPort(PICO2_SERIAL, 2);
    return;
  }

  char* token0 = strtok(line, ",");
  char* token1 = strtok(nullptr, ",");
  char* token2 = strtok(nullptr, ",");
  char* token3 = strtok(nullptr, ",");
  char* token4 = strtok(nullptr, ",");

  if (!token0 || !token1) {
    picoRxStats[picoNumber - 1].badLines++;
    return;
  }

  char eventType = token1[0];
  if (eventType == 'C') return;
  if (!token2) {
    picoRxStats[picoNumber - 1].badLines++;
    return;
  }
  int index = atoi(token2);
  uint8_t kyIndex = kyFromPicoIndex(picoNumber, index);
  if (kyIndex >= TRACK_COUNT) {
    picoRxStats[picoNumber - 1].badLines++;
    return;
  }

  if(!pc831.injecting && ((pc831.kyMask&(1u<<kyIndex)) || (eventType=='B'&&(pc831.kyDown&(1u<<kyIndex)))))return;
  if(debugTestMode==DEBUG_TEST_CV47&&eventType=='B'){kyBtnDown[kyIndex]=false;return;}
  if (eventType == 'B') {
    if(token3&&atoi(token3))presentationTouch43();
    if(controller940){kyBtnDown[kyIndex]=false;return;}
    NeoInputScope932 neoScope; // KY edits the displayed Neo/QV context, independently of EP.
    int pressed = token3 ? atoi(token3) : 0;if(pressed)alertAction08();
    // Release must clear the physical latch even when the current editor consumes it.
    struct Release23 {uint8_t key;bool release;~Release23(){if(release){kyBtnDown[key]=false;kyRotatedWhileDown[key]=false;if(kyHelpShown[key])sendKyHelp(key,false);kyHelpShown[key]=false;}}} release23{kyIndex,!pressed};
    InputScope815 input(I815_KY,kyIndex,pressed!=0);
    if(deleteKey55(kyIndex,pressed!=0))return;
    if(!quickView903&&!patchBank808&&!grooveEditMode&&!menuNavigationOnly()&&!keyboardEnabled823&&kyIndex==6){
      bool click41=!pressed&&kyBtnDown[kyIndex]&&!kyRotatedWhileDown[kyIndex];kyBtnDown[kyIndex]=pressed;
      if(pressed){kyRotatedWhileDown[kyIndex]=false;kyBtnDownAtMs[kyIndex]=millis();}
      if(click41){moveUnit42=(moveUnit42+1)%4;moveFine40=false;moveUnitShown40=true;displayValueDirty=true;}return;
    }
    if(!quickView903&&!patchBank808&&!patternMode&&!grooveEditMode&&!menuNavigationOnly()&&!keyboardEnabled823&&kyIndex==2){
      bool click41=!pressed&&kyBtnDown[kyIndex]&&!kyRotatedWhileDown[kyIndex];kyBtnDown[kyIndex]=pressed;
      if(pressed){kyRotatedWhileDown[kyIndex]=false;kyBtnDownAtMs[kyIndex]=millis();}
      if(click41)toggleEditFastFromKy(2,0);return;
    }

    if(rollClick60(kyIndex,pressed!=0))return;
    if(curveKey41(kyIndex)){
      bool click41=!pressed&&kyBtnDown[kyIndex]&&!kyRotatedWhileDown[kyIndex];kyBtnDown[kyIndex]=pressed;
      if(pressed){kyRotatedWhileDown[kyIndex]=false;kyBtnDownAtMs[kyIndex]=millis();}
      if(click41){curveFine41=!curveFine41;displayValueDirty=modDisplayDirty810=true;DISPLAY_SERIAL.println(curveFine41?"MSG|CURVE STEP 1":"MSG|CURVE STEP 10");}return;
    }
    if(grooveEditMode&&patternMode&&(kyIndex==14||kyIndex==15)){bool click59=!pressed&&kyBtnDown[kyIndex]&&!kyRotatedWhileDown[kyIndex];kyBtnDown[kyIndex]=pressed;if(pressed)kyRotatedWhileDown[kyIndex]=false;if(click59){if(kyIndex==15)grooveAllOff59=!grooveAllOff59;else grooveTrackOff59^=1u<<selectedTrack;mod805Invalidate();invalidateStepTriggers();neoDisplayDirty=displayValueDirty=true;grooveListDisplay40();}return;}
    if(lockHoldEdge26(kyIndex,pressed!=0))return;
    if(kyIndex==15&&grooveEditMode&&!patternMode&&!menuNavigationOnly()){bool click=!pressed&&kyBtnDown[kyIndex]&&!kyRotatedWhileDown[kyIndex];kyBtnDown[kyIndex]=pressed;if(pressed)kyRotatedWhileDown[kyIndex]=false;if(click){grooveView16=!grooveView16;sendGroovePacket();}return;}
    if(kyIndex==8&&stepContext25()){if(pressed){bool held=false;for(auto&panel:neoPadDown)for(bool down:panel)held|=down;if(held)return;stepLayout25=(stepLayout25+1)%4;neoDisplayDirty=displayValueDirty=true;DISPLAY_SERIAL.printf("MSG|STEP TRIX %u\n",stepLayout25+1);}return;}

    if(kbKeyR10(kyIndex,pressed!=0))return;
    if(quickView903==1&&debugTestMode==DEBUG_TEST_NONE&&blockClick21(kyIndex,pressed!=0))return;
    if(lfoClick932(kyIndex,pressed!=0))return;
    if(cvFineClick47(kyIndex,pressed!=0))return;
    if(!quickView903&&modUi805&&modUi805!=MOD_DELAY806&&modSelected806<64&&modTimingKey36(modSelected806,kyIndex)){
      if(!pressed&&kyBtnDown[kyIndex]&&!kyRotatedWhileDown[kyIndex])modTimingClick36(modSelected806,kyIndex);
      kyBtnDown[kyIndex]=pressed!=0;if(pressed){kyBtnDownAtMs[kyIndex]=millis();kyRotatedWhileDown[kyIndex]=false;}return;
    }
    if(!quickView903&&modUi805&&modUi805!=MOD_DELAY806&&modSelected806<64&&kyIndex==8){
      if(!pressed&&kyBtnDown[kyIndex]&&!kyRotatedWhileDown[kyIndex]){
        auto&m=mod806Unit(modSelected806);
        if(kyIndex==11){m.sustain=lfoEnvelopeOn27(m)?80:126;modPresetDirty07[modSelected806]=storageDirty931=true;mod805Invalidate();}
        else{int start=modDepthSlot806;for(uint8_t n=1;n<=8;++n){uint8_t slot=(start+n+8)%8;if(m.outputs[slot].enabled){modDepthSlot806=slot;modDestCursor807=0;for(uint8_t j=0;j<slot;++j)if(m.outputs[j].enabled)++modDestCursor807;break;}}}
        modDisplayDirty810=displayValueDirty=true;
      }
      kyBtnDown[kyIndex]=pressed!=0;if(pressed){kyBtnDownAtMs[kyIndex]=millis();kyRotatedWhileDown[kyIndex]=false;}return;
    }

    if(quickView903==1 && kyIndex==4 && debugTestMode==DEBUG_TEST_NONE && !deleteButtonActive){
      if(pressed){
        if(!kyBtnDown[kyIndex]){kyBtnDown[kyIndex]=true;kyBtnDownAtMs[kyIndex]=millis();kyRotatedWhileDown[kyIndex]=false;kyHelpShown[kyIndex]=false;gateHoldValueShown=false;}
      }else if(kyBtnDown[kyIndex]){
        uint32_t held=millis()-kyBtnDownAtMs[kyIndex];
        bool click=!kyRotatedWhileDown[kyIndex]&&!kyHelpShown[kyIndex]&&!numericEntryActive;
        if(kyHelpShown[kyIndex])sendKyHelp(kyIndex,false);
        kyBtnDown[kyIndex]=kyRotatedWhileDown[kyIndex]=kyHelpShown[kyIndex]=false;gateHoldValueShown=false;
        if(click)quickGateClick20(held);
      }
      return;
    }
    if((remixMode&&quickView903!=3&&quickView903!=6)||(remixKeys911&(1u<<kyIndex))){remixKey911(kyIndex,pressed!=0);return;}
    if (quickView903>=2 || patchBank808) {
      if(!pressed&&kyBtnDown[kyIndex]&&!kyRotatedWhileDown[kyIndex])quickModClick07(kyIndex);
      if(quickView903==3&&pressed&&!kyBtnDown[kyIndex])delayQuickKey925(kyIndex);
      if(quickView903==2&&!pressed&&kyBtnDown[kyIndex]&&!kyRotatedWhileDown[kyIndex]){if(rollKey26(kyIndex)==11)rollClick1000(1);if(rollKey26(kyIndex)==15)rollClick1000(4);}
      kyBtnDown[kyIndex]=pressed!=0;
      if(pressed){kyBtnDownAtMs[kyIndex]=millis();kyRotatedWhileDown[kyIndex]=false;}
      return;
    }
    if(quickView903==1&&kyIndex==8){if(!pressed&&kyBtnDown[kyIndex]&&!kyRotatedWhileDown[kyIndex])rollClick1000(1);kyBtnDown[kyIndex]=pressed!=0;if(pressed)kyRotatedWhileDown[kyIndex]=false;return;}
    if (debugTestMode == DEBUG_TEST_KY40) {
      reportDebugKy40(picoNumber, kyIndex, 0, 0, true, pressed != 0);
      return;
    }
    if (debugTestMode != DEBUG_TEST_NONE) return;
    if(grooveEditMode&&!patternMode&&kyIndex==4){bool click=!pressed&&kyBtnDown[kyIndex]&&!kyRotatedWhileDown[kyIndex];kyBtnDown[kyIndex]=pressed;if(pressed)kyRotatedWhileDown[kyIndex]=false;if(click){grooveGateTens51=!grooveGateTens51;DISPLAY_SERIAL.println(grooveGateTens51?"MSG|GAT % STEP 10":"MSG|GAT % STEP 1");}return;}

    if(!quickView903&&!patchBank808&&!patternMode&&!grooveEditMode&&!menuNavigationOnly()&&kyIndex==6){
      bool click=!pressed&&kyBtnDown[kyIndex]&&!kyRotatedWhileDown[kyIndex];kyBtnDown[kyIndex]=pressed;
      if(pressed)kyRotatedWhileDown[kyIndex]=false;
      if(click){moveUnit42=(moveUnit42+1)%4;moveFine40=false;moveUnitShown40=true;displayValueDirty=true;}return;
    }
    if (menuNavigationOnly() || deleteButtonActive || (grooveEditMode && !patternMode) || (patternMode && selectButtonActive && !grooveEditMode && !modUi805 && !quickView903 && kyIndex != 0)) return;
    uint32_t now = millis();

    if (pressed) {
      if (!kyBtnDown[kyIndex]) {
        kyBtnDown[kyIndex] = true;
        kyBtnDownAtMs[kyIndex] = now;
        kyRotatedWhileDown[kyIndex] = false;
        kyHelpShown[kyIndex] = false;
        if (kyIndex == 4) gateHoldValueShown = false;
      }
    } else {
      if (kyBtnDown[kyIndex]) {
        uint32_t heldMs = now - kyBtnDownAtMs[kyIndex];
        kyBtnDown[kyIndex] = false;
        bool rotated = kyRotatedWhileDown[kyIndex];
        bool helpWasShown = kyHelpShown[kyIndex];
        if (helpWasShown) sendKyHelp(kyIndex, false);
        kyRotatedWhileDown[kyIndex] = false;
        kyHelpShown[kyIndex] = false;
        if (kyIndex == 4) gateHoldValueShown = false;

        // The Pico already debounces both edges using the configured B value.
        // Do not reject a complete click because its UART frames arrived together.
        if (!rotated && !helpWasShown && !numericEntryActive) {
          kyBtnLastToggleMs[kyIndex] = now;
          if (grooveEditMode && patternMode && selection805GrooveClick(kyIndex)) return;
          if (kyIndex == 0 && patternMode && !grooveEditMode) { programEdit939(0,true); return; }
          if (kyIndex == 0 && !grooveEditMode) { toggleEditFastFromKy(0, heldMs); return; }
          if (grooveEditMode && patternMode) {
            if(kyIndex==15&&listCount44(listTrack40)<BLOCK_LISTS21){blockListEdit21=listCount44(listTrack40)++;memset(listBank44(listTrack40)[blockListEdit21],0,sizeof(listBank44(listTrack40)[0]));listAssign40(listTrack40,listPattern40,blockListEdit21+1);++modEpoch805;storageDirty931=true;}
            if (kyIndex < GROOVE_LIST_SIZE) grooveListRepeatEditing ^= 1u << kyIndex;
            sendGrooveListPacket();
            return;
          }
          if (patternMode) {
            (pidiStateDirty931=true, trackMuted[kyIndex] = !trackMuted[kyIndex]);
            neoDisplayDirty = true;
            displayValueDirty = true;
            return;
          }
          if (kyIndex == 15) return;
          // K3 est l'unique validation de l'edition temporaire NOT/SEL.
          // Les autres clics K (vitesse, mode Gate, raccourci Gate/zoom...)
          // restent disponibles pendant une selection et ne la quittent pas.
          if (kyIndex == 2 && !selectButtonActive && !shiftButtonActive && (hasSelectedSteps() || selectedStep >= 0 ||
                               pendingSelectionEdit.active)) {
            // 1A2/1A3 : la selection est deja en NOT. Un clic valide et
            // quitte directement; Annuler reste le seul chemin de restauration.
            cancelStepSelection();
            displayValueDirty = false;
            return;
          }
          toggleEditFastFromKy(kyIndex, heldMs);
        }
      }
    }
    return;
  }

  if (eventType == 'E') {
    lockHoldTurn26(kyIndex);
    if (!token3 || !token4) {
      picoRxStats[picoNumber - 1].badLines++;
      return;
    }
    int32_t value = atol(token3);
    if (kyBtnDown[kyIndex]) {
      kyRotatedWhileDown[kyIndex] = true;
      if (kyHelpShown[kyIndex]) {
        sendKyHelp(kyIndex, false);
        kyHelpShown[kyIndex] = false;
      }
    }
    int messageDelta = atoi(token4);
    int delta = messageDelta;
    uint8_t localIndex = (uint8_t)index;
    PicoRxStats &rx = picoRxStats[picoNumber - 1];
    rx.encoderEvents++;

    if (rx.valueKnown[localIndex]) {
      int32_t absoluteDelta = value - rx.lastValue[localIndex];
      bool plausibleContinuation = absoluteDelta >= -32 && absoluteDelta <= 32;
      if (plausibleContinuation && absoluteDelta != messageDelta) {
        rx.sequenceGaps++;
        int32_t missing = absoluteDelta - messageDelta;
        rx.recoveredSteps += (uint32_t)abs((int)missing);
      }

      // Une difference raisonnable permet de rattraper les trames perdues et
      // d'ignorer un eventuel doublon. Une grande difference indique plutot
      // un redemarrage du Pico : resynchroniser sans provoquer un gros saut.
      if (picoRecoveryEnabled() && plausibleContinuation) {
        delta = (int)absoluteDelta;
      }
    }
    rx.lastValue[localIndex] = value;
    rx.valueKnown[localIndex] = true;
    if(delta)presentationTouch43();
    if(controller940){if(delta)controllerEmit940("KY",kyIndex,delta);return;}

    // Pendant le reglage d'un Roy, les KY40 saisissent le brouillon afin que
    // les quatre Roy restent entierement disponibles comme objets du test.
    if (settingsOpen && settingsMenuLevel == SETTINGS_ROY_EDIT) {
      editRoySettingFromKy40(delta);
      return;
    }

    if (debugTestMode == DEBUG_TEST_KY40) {
      reportDebugKy40(picoNumber, kyIndex, value, delta, false, false);
      return;
    }
    if (debugTestMode != DEBUG_TEST_NONE) return;

    // Chaque Pico reconstruit deja un cran mecanique complet. Le Teensy ne doit
    // pas diviser une seconde fois ce delta, sinon des crans semblent ignores.
    int logicalDelta = normalKyDelta819(delta);
    if(kyBtnDown[kyIndex]&&!curveKey41(kyIndex)&&!(kyIndex==0&&trackContext815()))logicalDelta*=10;

    bool parameterChanged = applyParamDeltaFromKy(kyIndex, logicalDelta);
    if (logicalDelta != 0 && parameterChanged && !modUi805 && !patchBank808) {
      displayValueDirty = true;
    }
    return;
  }

  picoRxStats[picoNumber - 1].badLines++;
}

void servicePicoSerial(HardwareSerial &port, uint8_t picoNumber, char* buffer, uint8_t &pos) {
  PicoRxStats &rx = picoRxStats[picoNumber - 1];
  int backlog = port.available();
  if (backlog > rx.maxBacklog) rx.maxBacklog = (uint16_t)min(backlog, 65535);

  // Les UART KY40 sont prioritaires : vider completement les tampons a chaque
  // passage au lieu de laisser seulement 48 caracteres par tour de loop().
  unsigned inputBudget34=256;
  while (port.available() > 0 && inputBudget34--) {
    char c = (char)port.read();

    if (c == '\r') continue;

    if (c == '\n') {
      buffer[pos] = '\0';
      handlePicoLine(picoNumber, buffer);
      serviceRealtimeBetweenNeoPanels();pidiService827();
      pos = 0;
      buffer[0] = '\0';
      continue;
    }

    if (pos < 79) {
      buffer[pos++] = c;
    } else {
      pos = 0;
      buffer[0] = '\0';
      rx.badLines++;
    }
  }
}

void servicePicos() {
  servicePicoSerial(PICO1_SERIAL, 1, pico1Line, pico1LinePos);
  servicePicoSerial(PICO2_SERIAL, 2, pico2Line, pico2LinePos);
}

void handleDisplayInputLine(char* line) {
  if(line&&!strncmp(line,"BUSY910,READY,",14)){busyAck910=strtoul(line+14,nullptr,10);return;}
  if(fullBusy910)return;
  if(sdBusy905())return;
  if (!line) return;
  // Picran owns the physical LCD rendering and sends the exact final rows.
  // Relay only those stable UKD1 packets to the Windows USB serial port.
  if (strncmp(line, "UKD1|", 5) == 0) {
    if (Serial) Serial.println(line);
    return;
  }
  if(controller940)return;
  if (strncmp(line, "PICO_LCD_TEST", 13) == 0) {
    Serial.print(F("DBG,SCREENS,"));
    Serial.println(line);
    return;
  }
  if (strncmp(line, "PICO_LCD_READY", 14) == 0) {
    screenMapSend();
    DISPLAY_SERIAL.printf("SYNKOZ,%u\n",synkozEnabled933);
    display821.invalidate();memset(modPreviousLcd806,0,sizeof(modPreviousLcd806));remixInvalidateDisplay915();
    // Le Pico ecran vient de redemarrer : renvoyer tout l'etat sans imposer
    // de reset au Teensy ni aux Pico KY.
    if (debugTestMode != DEBUG_TEST_NONE) startDebugTest(debugTestMode);
    else if (fileBrowserMode != FILE_BROWSER_NONE) renderFileBrowser();
    else if (loadTypeMenuActive) sendLoadTypeMenu();
    else if (saveMenuActive) sendSaveMenu();
    else if (kbMenuR10) kbRenderMenuR10();
    else if (settingsOpen) sendSettingsPage();
    else if (writeMenuActive) sendWriteMenu();
    else if (trackMultiMenuActive) sendTrackMultiMenu();
    else if (trackPreviewMenuActive) sendTrackPreviewMenu();
    else if (trackViewMenuActive) sendTrackViewMenu();
    else if (trackPlayMenuActive) sendTrackPlayMenu();
    else if (trackMenuActive) sendTrackMenu();
    else if (clipboardMenuActive) sendClipboardMenu();
    else if (clipboardPasteMenuActive) sendClipboardPasteMenu(clipboardPasteMenuSlot);
    else showGameScreen();
    sendDisplayValuePacket();
    sendDisplayBpmPacket();
    DISPLAY_SERIAL.print(F("CFG,DLY,"));
    DISPLAY_SERIAL.println(settingsValue[0][3]);
    sendSelectionIndicatorsToPicran();
    displayValueDirty = false;
    displayBpmDirty = false;
    return;
  }
  if(pc831.keyMask&&!pc831.injecting)return;
  if (strncmp(line, "KEY,", 4) != 0) return;
  if (line[4] == '\0' || line[5] != '\0') return;
  if (debugTestMode == DEBUG_TEST_KEYPAD) {
    char line0[21], small0[17];
    snprintf(line0, sizeof(line0), "KEYPAD TEST KEY %c", line[4]);
    snprintf(small0, sizeof(small0), "KEYPAD KEY %c", line[4]);
    sendDebugDisplay(line0, "PICO GP12..GP19", "MATRIX 4x4 OK",
                     "BE1 BACK", small0, "EVENEMENT RECU");
    Serial.print(F("DBG,KEYPAD,KEY=")); Serial.println(line[4]);
    return;
  }
  if (debugTestMode != DEBUG_TEST_NONE) return;
  if (menuNavigationOnly() && fileBrowserMode != FILE_BROWSER_SAVE) return;
  handleNumericKey(line[4]);
}

void serviceDisplayInput() {
  uint8_t charsThisLoop = 0;
  while (DISPLAY_SERIAL.available() > 0 && charsThisLoop < PICO_MAX_CHARS_PER_LOOP) {
    charsThisLoop++;
    char c = (char)DISPLAY_SERIAL.read();

    if (c == '\r') continue;

    if (c == '\n') {
      displayRxLine[displayRxLinePos] = '\0';
      handleDisplayInputLine(displayRxLine);
      displayRxLinePos = 0;
      displayRxLine[0] = '\0';
      continue;
    }

    if (displayRxLinePos < sizeof(displayRxLine) - 1) {
      displayRxLine[displayRxLinePos++] = c;
    } else {
      displayRxLinePos = 0;
      displayRxLine[0] = '\0';
    }
  }
}

FLASHMEM void sendPicoDefaultsToPort(HardwareSerial &port, uint8_t role) {
  const uint8_t page = 1;
  port.print(F("I,")); port.println(role);
  port.print(F("D,"));
  port.println(clampInt(settingsValue[page][2], PICO_DETENT_MIN, PICO_DETENT_MAX));
  port.print(F("R,")); port.println(clampInt(settingsValue[page][1], 0, 80));
  port.print(F("B,")); port.println(clampInt(picoButtonDebounceMs, 0, 120));
  port.println(F("?"));
}

FLASHMEM void sendPicoDefaults() {
  sendPicoDefaultsToPort(PICO1_SERIAL, 1);
  sendPicoDefaultsToPort(PICO2_SERIAL, 2);
}

// GP4 orange covers every selection, including NOT, empty cells and Groove.
// Extend this common predicate whenever a future selection type is introduced.
bool anySelectionIndicatorActive() {
  if(deleteButtonActive)return false;
  return selectButtonActive || shiftButtonActive || columnSelectionMode || selectionMode ||
         selectedStep >= 0 || pendingSelectionEdit.active ||
         hasSelectedSteps() || selectedPatternCount() != 0 || grooveStepMask != 0;
}

FLASHMEM uint8_t modeLedState() {
  return (anySelectionIndicatorActive() ? 1u : 0u) |
         (remixMode ? 2u : 0u) | ((patternMode&&!grooveEditMode) ? 4u : 0u) |
         ((grooveNoticeDuration26?((millis()-grooveNoticeAt26)/(max(1UL,grooveNoticeDuration26/32)))%2==0:grooveEditMode) ? 8u : 0u) |
         ((deleteButtonActive || songDeleteArmed || (recordDelete51&&recordArmed823())) ? 16u : 0u) |
         (indEditMode52 ? 32u : 0u);
}

FLASHMEM void sendSelectionIndicatorsToPicran() {
  selectionIndicatorsPending931=false;
  const uint8_t state = mappedLedState932(modeLedState());
  static uint8_t sent931=255;
  if(state==sent931)return;sent931=state;
  DISPLAY_SERIAL.print(F("MODELEDS,"));
  // Keep the first four fields compatible; append DELETE and SONG.
  for (uint8_t i = 0; i < 6; ++i) {
    if (i) DISPLAY_SERIAL.print(',');
    DISPLAY_SERIAL.print((state >> i) & 1u);
  }
  DISPLAY_SERIAL.println();
}

DMAMEM char usbControlLine[900] = {0}; // PC Song/name commands; bounded parser below.
uint16_t usbControlLinePos = 0;

FLASHMEM void printKyRxStats() {
  Serial.print(F("KYRX,E1=")); Serial.print(picoRxStats[0].encoderEvents);
  Serial.print(F(",E2=")); Serial.print(picoRxStats[1].encoderEvents);
  Serial.print(F(",G1=")); Serial.print(picoRxStats[0].sequenceGaps);
  Serial.print(F(",G2=")); Serial.print(picoRxStats[1].sequenceGaps);
  Serial.print(F(",REC1=")); Serial.print(picoRxStats[0].recoveredSteps);
  Serial.print(F(",REC2=")); Serial.print(picoRxStats[1].recoveredSteps);
  Serial.print(F(",BAD1=")); Serial.print(picoRxStats[0].badLines);
  Serial.print(F(",BAD2=")); Serial.print(picoRxStats[1].badLines);
  Serial.print(F(",Q1=")); Serial.print(picoRxStats[0].maxBacklog);
  Serial.print(F(",Q2=")); Serial.print(picoRxStats[1].maxBacklog);
  Serial.print(F(",RCVR=")); Serial.println(picoRecoveryMode);
}

FLASHMEM void printRollStats() {
  for (uint8_t track = 0; track < TRACK_COUNT; track++) {
    uint16_t activeCount = 0;
    uint16_t rollCount = 0;
    for (uint16_t step = 0; step < trackStepCount(track); step++) {
      if (!stepActive[track][step]) continue;
      activeCount++;
      TrackRollSettings roll = configuredRollSettings(track, step);
      if (roll.start > 0 && roll.level > 0) rollCount++;
    }
    Serial.print(F("ROLLSTAT,T=")); Serial.print(track + 1);
    Serial.print(F(",NOTES=")); Serial.print(activeCount);
    Serial.print(F(",ROLL=")); Serial.print(rollCount);
    Serial.print(F(",DEFSTART=")); Serial.print(trackRoll[track].start);
    Serial.print(F(",DEFSPEED=")); Serial.print(trackRoll[track].speed);
    Serial.print(F(",DEFLEVEL=")); Serial.println(trackRoll[track].level);
  }
}

// 13B1 - USB regression exercise. Only a completely empty track is borrowed;
// its cell data, settings, undo history and view are restored before returning.
FLASHMEM void runSelectionSelfCheck() {
  if (grooveEditMode || anyLiveClipboardPreview() || playing || currentMenuPageRef() != PAGE_A || hasSelectedSteps() || selectedStep >= 0 ||
      selectButtonActive || shiftButtonActive || deleteButtonActive || pendingSelectionEdit.active || patternMode || remixMode) {
    Serial.println(F("SELCHECK,BUSY")); return;
  }
  for (uint8_t n = 0; n < NEO_COUNT; n++) for (uint8_t p = 0; p < 16; p++)
    if (neoPadDown[n][p]) { Serial.println(F("SELCHECK,PAD_HELD")); return; }
  uint8_t t = 255;
  for (uint8_t track = 0; track < TRACK_COUNT; track++) {
    bool empty = true;
    for (uint16_t step = 0; step < STEP_COUNT; step++) if (stepActive[track][step]) { empty = false; break; }
    for (uint8_t i = 0; i < FINE_EVENTS_PER_TRACK; i++) if (fineNote[track][i].active) empty = false;
    if (empty) { t = track; break; }
  }
  if (t == 255) { Serial.println(F("SELCHECK,NO_EMPTY_TRACK")); return; }
  struct CellBackup {
    uint8_t vel, note, count, extra[EXTRA_NOTES_PER_STEP], rollMask;
    uint16_t gate; int16_t micro; int8_t logical;
    bool lv, lp, lg, lt; TrackRollSettings roll;
  } backup[3];
  for (uint8_t step = 0; step < 3; step++) {
    CellBackup &c = backup[step];
    c.vel = stepVel[t][step]; c.note = stepMidiNote[t][step]; c.count = stepExtraNoteCount[t][step];
    memcpy(c.extra, stepExtraMidiNote[t][step], sizeof(c.extra));
    c.gate = stepGateTicks[t][step]; c.micro = stepMicroTick[t][step]; c.logical = stepLogicalMicro[t][step];
    c.lv = stepLockVel[t][step]; c.lp = stepLockPitch[t][step]; c.lg = stepLockGate[t][step]; c.lt = stepLockTime[t][step];
    c.roll = rollRead906(t,step); c.rollMask = stepRollLockMask[t][step];
  }
  DeletedNoteUndoRecord history[NOTE_UNDO_LEVELS];
  memcpy(history, deletedNoteHistory, sizeof(history));
  uint8_t historyCount = deletedNoteHistoryCount, historyCursor = deletedNoteHistoryCursor;
  uint8_t oldTrack = selectedTrack, oldBottom = chromaticBottomNote;
  uint8_t oldPitch = activePitchRow, oldPitchTrack = activePitchTrack;
  uint8_t oldViewStart = viewStartTrack, oldVoice = selectedVoice;
  int16_t oldFine = selectedFineIndex;
  uint16_t oldBars = trackBars[t], oldSequenceBars = sequenceBars;
  uint16_t oldStep = currentStep; uint32_t oldMeasure = measureTick;
  int8_t oldZoom = zoomLevel, oldNoteView = trackNoteViewMode[linkOwner12(t,LINK_VIEW12)];
  int8_t oldMultiEdit = trackMultiEditMode[t];
  uint32_t oldOffset = viewTickOffset;
  TrackPlayMode oldPlayMode = trackPlayMode[linkOwner12(t,LINK_POLY12)];
  bool oldPitchMode = pitchEncoderSelectMode, oldLastValid = lastNoteValid;
  bool oldGridValid = lastGridTickValid; uint32_t oldGridTick = lastGridTick;
  selectedTrack = t; trackBars[t] = sequenceBars = 1; zoomLevel = 0; viewTickOffset = 0;
  chromaticBottomNote = 60; trackNoteViewMode[linkOwner12(t,LINK_VIEW12)] = NOTE_VIEW_CHROMATIC; (pidiRoutesDirty931|=1u<<(t), trackPlayMode[linkOwner12(t,LINK_POLY12)] = TRACK_PLAY_MULTI);
  trackMultiEditMode[t] = 0;
  uint8_t savedGrooveAssignment = patternGroove[t][activePattern[t]];
  patternGroove[t][activePattern[t]] = 0;
  uint32_t failures = 0;
  bool oldSdNotice = sdNeoNoticeActive;
  sdNeoNoticeActive = true;
  if (currentMenuPageRef() != PAGE_A || menuNavigationOnly()) failures |= 65536;
  dispatchStableButtonPress(EDIT_BUTTON_BITS[EDIT_BUTTON_SELECT]);
  onPadEdge(0, 0, SEESAW_KEYPAD_EDGE_RISING);
  onPadEdge(0, 0, SEESAW_KEYPAD_EDGE_FALLING);
  if (stepActive[t][0] || !selectedEmptyCell(t, 0, 63) || !selectButtonActive) failures |= 1;
  if (!sdNeoNoticeActive) failures |= 131072;
  onPadEdge(0, 0, SEESAW_KEYPAD_EDGE_RISING);
  onPadEdge(0, 0, SEESAW_KEYPAD_EDGE_FALLING);
  if (hasSelectedSteps() || !selectButtonActive) failures |= 2;
  dispatchStableButtonPress(EDIT_BUTTON_BITS[EDIT_BUTTON_SELECT]);
  onPadEdge(0, 0, SEESAW_KEYPAD_EDGE_RISING);
  if (!sdNeoNoticeActive || stepActive[t][0]) failures |= 262144;
  onPadEdge(0, 0, SEESAW_KEYPAD_EDGE_FALLING);
  if (sdNeoNoticeActive || !stepActive[t][0] || selectedStep >= 0 || hasSelectedSteps()) failures |= 524288;
  leaveStepSelection(false);
  clearTrackStep(t, 0);
  dispatchStableButtonPress(EDIT_BUTTON_BITS[EDIT_BUTTON_SELECT]);
  for (uint8_t step = 0; step < 3; step++) {
    grooveFirstNote26(t,step);
    stepActive[t][step] = true; stepMidiNote[t][step] = step == 1 ? 62 : 63;
    stepLockPitch[t][step] = true; stepExtraNoteCount[t][step] = 0;
    stepLogicalMicro[t][step] = 0; stepMicroTick[t][step] = 0;
    stepVel[t][step] = 90; stepLockVel[t][step] = true;
  }
  stepExtraNoteCount[t][0] = 2; stepExtraMidiNote[t][0][0] = 61; stepExtraMidiNote[t][0][1] = 60;
  onPadEdge(0, 0, SEESAW_KEYPAD_EDGE_RISING);
  onPadEdge(0, 0, SEESAW_KEYPAD_EDGE_FALLING);
  onPadEdge(0, 5, SEESAW_KEYPAD_EDGE_RISING);
  onPadEdge(0, 5, SEESAW_KEYPAD_EDGE_FALLING);
  toggleNotePadSelection(t, 0, 1);
  if (!selectedNoteObject(t, 0, 0) || !selectedNoteObject(t, 0, 1) || !selectedNoteObject(t, 1, 0) ||
      !stepActive[t][0] || !stepActive[t][1]) failures |= 4;
  stageSelectedParamDelta(EDIT_PITCH, 1);
  stageSelectedParamDelta(EDIT_VEL, 1);
  if (noteAtVoice(t, 0, 0) != 63 || noteAtVoice(t, 1, 0) != 62) failures |= 8;
  dispatchStableButtonPress(EDIT_BUTTON_BITS[EDIT_BUTTON_SELECT]);
  if (selectButtonActive || hasSelectedSteps() || noteAtVoice(t, 0, 0) != 64 ||
      noteAtVoice(t, 0, 1) != 62 || noteAtVoice(t, 1, 0) != 63) failures |= 16;
  if (effectiveVel(t, 0, 0) != 91 || effectiveVel(t, 0, 1) != 91 ||
      effectiveVel(t, 0, 2) != 90 || noteAtVoice(t, 0, 2) != 60) failures |= 32768;
  dispatchStableButtonPress(EDIT_BUTTON_BITS[EDIT_BUTTON_SELECT]);
  toggleNotePadSelection(t, 0, 0); toggleNotePadSelection(t, 0, 0);
  if (hasSelectedSteps() || !selectButtonActive || !selectionMode) failures |= 32;
  toggleNotePadSelection(t, 0, 0); stageSelectedParamDelta(EDIT_PITCH, 2);
  p23BackOrRecover();
  if (selectButtonActive || shiftButtonActive || hasSelectedSteps() || noteAtVoice(t, 0, 0) != 64) failures |= 64;
  stepMidiNote[t][0] = 63; stepMidiNote[t][1] = 62;
  handleEditButtonPress(EDIT_BUTTON_SHIFT);
  handleEditButtonPress(EDIT_BUTTON_SELECT_ALL);
  if (shiftButtonActive || selectButtonActive || hasSelectedSteps()) failures |= 128;
  // A normal note deletion uses the actual Neo edge route and must undo.
  onPadEdge(0, 0, SEESAW_KEYPAD_EDGE_RISING); onPadEdge(0, 0, SEESAW_KEYPAD_EDGE_FALLING);
  if (findVoiceAtStep(t, 0, 63) >= 0) failures |= 256;
  performUndo();
  if (findVoiceAtStep(t, 0, 63) < 0) failures |= 512;
  leaveStepSelection(false);
  handleTransportButtonEdge(6, true);
  dispatchStableButtonPress(EDIT_BUTTON_BITS[EDIT_BUTTON_SELECT]);
  onPadEdge(0, 0, SEESAW_KEYPAD_EDGE_RISING); onPadEdge(0, 0, SEESAW_KEYPAD_EDGE_FALLING);
  onPadEdge(0, 4, SEESAW_KEYPAD_EDGE_RISING); onPadEdge(0, 4, SEESAW_KEYPAD_EDGE_FALLING);
  if (!patternMode || selectedPatternCount() != 2) failures |= 1024;
  onPadEdge(0, 0, SEESAW_KEYPAD_EDGE_RISING); onPadEdge(0, 0, SEESAW_KEYPAD_EDGE_FALLING);
  onPadEdge(0, 4, SEESAW_KEYPAD_EDGE_RISING); onPadEdge(0, 4, SEESAW_KEYPAD_EDGE_FALLING);
  if (selectedPatternCount() || !selectButtonActive) failures |= 2048;
  dispatchStableButtonPress(EDIT_BUTTON_BITS[EDIT_BUTTON_SELECT]); handleTransportButtonEdge(6, true);
  (pidiStateDirty931=true, playing = internalClockRunning = true);
  handleTransportButtonEdge(2, true);
  if (playing || internalClockRunning || patternMode || editActionForBit(29) >= 0) failures |= 4096;
  startDebugTest(DEBUG_TEST_BUTTONS);
  char release[] = "DATA,Y1,0,0,0,0,0,Y2,0,0,0,0,0";
  char click[] = "DATA,Y1,0,0,0,0,1,Y2,0,0,0,0,0";
  handleP23Line(release); handleP23Line(click);
  if (debugTestMode != DEBUG_TEST_NONE) failures |= 8192;
  stopDebugTest(); showGameScreen();
  startDebugTest(DEBUG_TEST_KY23);
  char release2[] = "DATA,Y1,0,0,0,0,0,Y2,0,0,0,0,0";
  char click2[] = "DATA,Y1,0,0,0,0,1,Y2,0,0,0,0,0";
  handleP23Line(release2); handleP23Line(click2);
  if (debugTestMode != DEBUG_TEST_KY23) failures |= 16384;
  stopDebugTest(); showGameScreen();
  leaveStepSelection(false);
  // Provisional note deletion: several presses, repeat, cancel and commit.
  clearTrackStep(t, 0); clearTrackStep(t, 1); clearTrackStep(t, 2);
  stepActive[t][0] = stepActive[t][1] = true;
  stepMidiNote[t][0] = 63; stepMidiNote[t][1] = 62;
  stepLockPitch[t][0] = stepLockPitch[t][1] = true;
  stepLogicalMicro[t][0] = stepLogicalMicro[t][1] = 0;
  handleEditButtonPress(EDIT_BUTTON_DELETE);
  onPadEdge(0, 0, SEESAW_KEYPAD_EDGE_RISING); onPadEdge(0, 0, SEESAW_KEYPAD_EDGE_FALLING);
  onPadEdge(0, 5, SEESAW_KEYPAD_EDGE_RISING); onPadEdge(0, 5, SEESAW_KEYPAD_EDGE_FALLING);
  onPadEdge(0, 0, SEESAW_KEYPAD_EDGE_RISING); onPadEdge(0, 0, SEESAW_KEYPAD_EDGE_FALLING);
  uint16_t deletedHead = 0; uint8_t deletedVoice = 0;
  Serial.printf("SELCHECK,DELETE_CONTEXT,T=%u,SEL=%u,VIEW=%u,OFFSET=%lu,BOTTOM=%u,MICRO=%d,PAT=%u,CTX=%u,MASK0=%u,MASK1=%u\n",
    t, selectedTrack, viewStartTrack, (unsigned long)viewTickOffset, chromaticBottomNote,
    defaultMicroTick[t], patternMode, deletePatternContext, selectedBaseMask[t][0], selectedBaseMask[t][1]);
  if (!deleteButtonActive || !pendingDeleteNote(t, 0, 0) || !pendingDeleteNote(t, 1, 0) ||
      visibleNoteAtPad(t, 63, 0, 0, deletedHead, &deletedVoice, true) || !stepActive[t][0]) failures |= 1048576;
  p23BackOrRecover();
  if (deleteButtonActive || !stepActive[t][0] || !stepActive[t][1] || hasSelectedSteps()) failures |= 2097152;
  handleEditButtonPress(EDIT_BUTTON_DELETE);
  onPadEdge(0, 0, SEESAW_KEYPAD_EDGE_RISING);
  handleEditButtonPress(EDIT_BUTTON_DELETE);
  onPadEdge(0, 0, SEESAW_KEYPAD_EDGE_FALLING);
  if (deleteButtonActive || stepActive[t][0] || !stepActive[t][1]) failures |= 4194304;
  clearTrackStep(t, 0); clearTrackStep(t, 1); clearTrackStep(t, 2);
  stepActive[t][0] = true; stepMidiNote[t][0] = 63;
  stepLockPitch[t][0] = true; stepLogicalMicro[t][0] = 0;
  for (uint8_t i = 0; i < NOTE_UNDO_LEVELS + 1 && i < FINE_EVENTS_PER_TRACK; i++) {
    fineNote[t][i] = {}; fineNote[t][i].active = true; fineNote[t][i].step = 0;
    fineNote[t][i].note = 63; fineNote[t][i].vel = 70; fineNote[t][i].gate = 12;
    fineNote[t][i].pxMask = PX_PITCH | PX_VEL | PX_GATE;
  }
  uint8_t zoneCount = noteCountAtStep(t, 0);
  handleEditButtonPress(EDIT_BUTTON_DELETE);
  onPadEdge(0, 0, SEESAW_KEYPAD_EDGE_RISING); onPadEdge(0, 0, SEESAW_KEYPAD_EDGE_FALLING);
  for (uint8_t v = 0; v < zoneCount; v++) if (!pendingDeleteNote(t, 0, v)) failures |= 33554432;
  p23BackOrRecover();
  if (noteCountAtStep(t, 0) != zoneCount || deleteButtonActive) failures |= 33554432;
  handleEditButtonPress(EDIT_BUTTON_DELETE);
  onPadEdge(0, 0, SEESAW_KEYPAD_EDGE_RISING); onPadEdge(0, 0, SEESAW_KEYPAD_EDGE_FALLING);
  handleEditButtonPress(EDIT_BUTTON_DELETE);
  if (stepActive[t][0] || noteCountAtStep(t, 0)) failures |= 67108864;
  if (storageWriteAvailable) {
    uint8_t fixturePattern = 255;
    for (uint8_t pat = 0; pat < PATTERN_COUNT; pat++) {
      char path[40], backupPath[40]; internalPatternPath(t, pat, path); deleteBackupPath(t, pat, backupPath);
      if (pat != activePattern[t] && !patternBank[t][pat].initialized && !SD.exists(path) && !SD.exists(backupPath)) {
        fixturePattern = pat; break;
      }
    }
    if (fixturePattern != 255) {
      StoredTrackPattern savedMeta = patternBank[t][fixturePattern];
      char path[40]; internalPatternPath(t, fixturePattern, path);
      if (!saveTrackPattern(t, fixturePattern)) failures |= 134217728;
      else {
        patternMode = true;
        handleEditButtonPress(EDIT_BUTTON_DELETE);
        patternSelected[t][fixturePattern] = true;
        // An unavailable SD refuses validation and keeps the full cancel path.
        storageWriteAvailable = false;
        if (finishDeleteMode(true) || !deleteButtonActive || !SD.exists(path)) failures |= 134217728;
        storageWriteAvailable = true;
        p23BackOrRecover();
        if (!SD.exists(path)) failures |= 134217728;
        handleEditButtonPress(EDIT_BUTTON_DELETE); patternSelected[t][fixturePattern] = true;
        if (!finishDeleteMode(true) || SD.exists(path) || patternBank[t][fixturePattern].initialized) failures |= 134217728;
        if (deleteButtonActive) finishDeleteMode(false);
      }
      SD.remove(path); patternBank[t][fixturePattern] = savedMeta;
      patternMode = false;
    } else Serial.println(F("SELCHECK,PATTERN_FILE_SKIPPED_NO_FREE_SLOT"));
  } else Serial.println(F("SELCHECK,PATTERN_FILE_SKIPPED_NO_SD"));
  // A pattern delete can always be cancelled without touching the SD file.
  patternMode = true;
  handleEditButtonPress(EDIT_BUTTON_DELETE);
  onPadEdge(0, 0, SEESAW_KEYPAD_EDGE_RISING); onPadEdge(0, 0, SEESAW_KEYPAD_EDGE_FALLING);
  if (!patternSelected[0][patternRowOffset] || !deleteButtonActive) failures |= 8388608;
  p23BackOrRecover(); patternMode = false;
  if (deleteButtonActive || selectedPatternCount()) failures |= 16777216;
  // Fixed queue arithmetic, Roll relative timing, saturation, and future boundary.
  uint32_t bridgeFailures = 0;
  bool bridgeBusy = false;
  for (uint8_t tr = 0; tr < TRACK_COUNT; ++tr) bridgeBusy |= grooveBridgeReady[tr];
  if (!bridgeBusy) {
    clearGrooveBridge(t);
    TrackRollSettings testRoll = {12, 4, 0, 0, 0, 80};
    if (!addBridgeNote(t, 1, -2, 1536, 67, 100, 24, 500, testRoll)) bridgeFailures |= 1;
    const int16_t wanted[4] = {-2, -14, -10, -6};
    uint16_t found = 0;
    for (const auto &e : grooveBridgeEvents) if (e.kind && e.track == t) {
      if (found >= 4 || e.offset != wanted[found] || e.note != 67 || e.gate != 24 || e.bend != 500) bridgeFailures |= 2;
      ++found;
    }
    if (found != 4) bridgeFailures |= 4;
    clearGrooveBridge(t);
    testRoll.start = 2; testRoll.speed = 1;
    if (!addBridgeNote(t, 2, 1539, 1536, 67, 100, 24, 0, testRoll)) bridgeFailures |= 8;
    found = 0;
    for (const auto &e : grooveBridgeEvents) if (e.kind && e.track == t) { if (e.offset < 1 || e.offset > 3) bridgeFailures |= 16; ++found; }
    if (found != 3) bridgeFailures |= 16;
    clearGrooveBridge(t);
    for (uint16_t i = 0; i < GROOVE_BRIDGE_CAPACITY; ++i)
      if (!addBridgeOccurrence(t, 1, -2, 0, 60, 90, 24, 0)) bridgeFailures |= 32;
    if (addBridgeOccurrence(t, 1, -2, 0, 60, 90, 24, 0)) bridgeFailures |= 64;
    clearGrooveBridge(t);
    uint64_t deadline = transportTickAbsolute + 4 * TICKS_PER_STEP;
    if (nextBridgeBoundary(t, deadline) <= deadline) bridgeFailures |= 128;
    grooveBridgeReady[t] = grooveBridgeSwitched[t] = true;
    grooveBridgeFirstEnd[t] = deadline;
    uint8_t deferredPattern = (activePattern[t] + 1) % PATTERN_COUNT;
    if (prepareGrooveBridge(t, deferredPattern) || pendingPattern[t] != deferredPattern || !grooveBridgeReady[t]) bridgeFailures |= 4096;
    clearGrooveBridge(t); pendingPattern[t] = -1;
  } else bridgeFailures |= 256;
  uint32_t extendedFailures = 0;
  clearTrackStep(t, 0); clearTrackStep(t, 1); clearTrackStep(t, 2);
  for (uint8_t cell = 0; cell < 2; cell++) {
    stepActive[t][cell] = true; stepMidiNote[t][cell] = 63;
    stepLockPitch[t][cell] = true; stepVel[t][cell] = 90; stepLockVel[t][cell] = true;
    stepGateTicks[t][cell] = 24; stepLockGate[t][cell] = true;
    stepLogicalMicro[t][cell] = 0; stepMicroTick[t][cell] = 0; stepLockTime[t][cell] = true;
  }
  runV714Fixtures(t);
  dispatchStableButtonPress(EDIT_BUTTON_BITS[EDIT_BUTTON_SELECT]);
  onPadEdge(0, 0, SEESAW_KEYPAD_EDGE_RISING); onPadEdge(0, 0, SEESAW_KEYPAD_EDGE_FALLING);
  onPadEdge(0, 1, SEESAW_KEYPAD_EDGE_RISING); onPadEdge(0, 1, SEESAW_KEYPAD_EDGE_FALLING);
  stageSelectedParamDelta(EDIT_VEL, 10);
  onPadEdge(0, 0, SEESAW_KEYPAD_EDGE_RISING); onPadEdge(0, 0, SEESAW_KEYPAD_EDGE_FALLING);
  if (!exiledNoteObject(t, 0, 0) || audibleVel(t, 0, 0) != 90 || audibleVel(t, 1, 0) != 100) extendedFailures |= 1;
  stageSelectedParamDelta(EDIT_VEL, 3);
  onPadEdge(0, 0, SEESAW_KEYPAD_EDGE_RISING); onPadEdge(0, 0, SEESAW_KEYPAD_EDGE_FALLING);
  if (exiledNoteObject(t, 0, 0) || audibleVel(t, 0, 0) != 103 || effectiveVel(t, 0, 0) != 90) extendedFailures |= 2;
  onPadEdge(0, 0, SEESAW_KEYPAD_EDGE_RISING);
  neoPressStartMs[0][0] = millis() - SELECT_VALIDATE_HOLD_MS - 1;
  serviceNeoSelectionHolds();
  onPadEdge(0, 0, SEESAW_KEYPAD_EDGE_FALLING);
  if (!selectButtonActive || selectedNoteObject(t, 0, 0) || exiledNoteObject(t, 0, 0) ||
      effectiveVel(t, 0, 0) != 103 || !selectedNoteObject(t, 1, 0)) extendedFailures |= 4;
  p23BackOrRecover();
  if (effectiveVel(t, 0, 0) != 103 || effectiveVel(t, 1, 0) != 90) extendedFailures |= 8;
  // The last exiled object retains the common draft, even with no active mask.
  dispatchStableButtonPress(EDIT_BUTTON_BITS[EDIT_BUTTON_SELECT]);
  onPadEdge(0, 1, SEESAW_KEYPAD_EDGE_RISING); onPadEdge(0, 1, SEESAW_KEYPAD_EDGE_FALLING);
  stageSelectedParamDelta(EDIT_PITCH, 2);
  toggleNotePadSelection(t, 1, 0); // move back to base position for a controlled exile fixture
  setNoteObjectSelected(t, 1, 0, true); exileNoteObject(t, 1, 0);
  if (hasSelectedSteps() || !pendingTargetStillSelected() || audibleNoteAtVoice(t, 1, 0) != 63) extendedFailures |= 16;
  setNoteObjectSelected(t, 1, 0, true);
  if (audibleNoteAtVoice(t, 1, 0) != 65) extendedFailures |= 16;
  p23BackOrRecover();
  int8_t savedSheet[GROOVE_STEPS][5]; memcpy(savedSheet, grooveOffset[GROOVE_COUNT-1], sizeof(savedSheet));
  uint8_t oldEditedGroove = editedGroove;
  memset(grooveOffset[GROOVE_COUNT-1], 0, sizeof(savedSheet)); editedGroove = GROOVE_COUNT;
  toggleGrooveEditor();
  onPadEdge(0, 0, SEESAW_KEYPAD_EDGE_RISING); onPadEdge(0, 0, SEESAW_KEYPAD_EDGE_FALLING);
  onPadEdge(2, 0, SEESAW_KEYPAD_EDGE_RISING); onPadEdge(2, 0, SEESAW_KEYPAD_EDGE_FALLING);
  if (grooveStepMask != ((1u << 0) | (1u << 8))) extendedFailures |= 32;
  applyParamDeltaFromKy(2, 5); applyParamDeltaFromKy(3, 3);
  applyParamDeltaFromKy(4, 7); applyParamDeltaFromKy(5, -2); applyParamDeltaFromKy(6, 5);
  float grooveTestBpm = bpm; applyParamDeltaFromKy(7, 10);
  if (bpm != grooveTestBpm || grooveOffset[GROOVE_COUNT-1][0][4] != 5) extendedFailures |= 8192;
  int8_t zoomBefore = zoomLevel; zoomAroundSelection(1, VIEW_SRC_ROY1_ZOOM);
  if (zoomLevel != zoomBefore || grooveOffset[GROOVE_COUNT-1][0][0] != 5 || grooveOffset[GROOVE_COUNT-1][8][3] != -2 || grooveOffset[GROOVE_COUNT-1][1][0]) extendedFailures |= 64;
  p23BackOrRecover();
  patternGroove[t][activePattern[t]] = GROOVE_COUNT;
  if (grooveDelta(t, 16, 0) != 5 || grooveDelta(t, 24, 3) != -2 || grooveDelta(t, 17, 0)) extendedFailures |= 128;
  if (audibleNoteAtVoice(t, 0, 0) != 68 || visualNoteAtVoice(t, 0, 0) != 63 ||
      audibleVel(t, 0, 0) != 106 || audibleGateTicks(t, 0, 0) != 31 || audibleMicroTick(t, 0, 0) != -2) extendedFailures |= 256;
  // Exercise the actual UKB4 reader on a newly owned inactive slot only.
  bool bridgeFileTest = false;
  if (storageWriteAvailable && !bridgeBusy) {
    for (uint8_t pat = 0; pat < PATTERN_COUNT; ++pat) {
      char path[40]; internalPatternPath(t, pat, path);
      if (pat == activePattern[t] || patternBank[t][pat].initialized || SD.exists(path)) continue;
      StoredTrackPattern meta = patternBank[t][pat];
      uint8_t assignment = patternGroove[t][pat];
      patternGroove[t][pat] = GROOVE_COUNT;
      clearGrooveBridge(t);
      bool saved = saveTrackPattern(t, pat);
      if (!saved || !readIncomingBridge(t, pat)) bridgeFailures |= 512;
      else {
        bridgeFileTest = true; bool mainFound = false;
        for (const auto &e : grooveBridgeEvents) if (e.kind == 1 && e.track == t && e.offset == -2 &&
            e.note == 68 && e.velocity == 106 && e.gate == 31 && e.bend == 500) mainFound = true;
        if (!mainFound) bridgeFailures |= 1024;
      }
      clearGrooveBridge(t);
      if (!SD.remove(path)) bridgeFailures |= 2048;
      patternGroove[t][pat] = assignment; patternBank[t][pat] = meta;
      break;
    }
  }
  Serial.print(F("GROOVEBRIDGECHECK,")); Serial.print(bridgeFailures ? F("FAIL") : F("PASS"));
  Serial.print(F(",MASK=")); Serial.print(bridgeFailures);
  Serial.print(F(",SD=")); Serial.println(bridgeFileTest ? F("PASS") : F("SKIPPED"));
  TrackRollSettings rollBefore = effectiveRollSettings(t, 0, 0);
  if (!sameRollSettings(rollBefore, audibleRollSettings(t, 0, 0))) extendedFailures |= 512;
  patternGroove[t][activePattern[t]] = 0;
  patternMode = true; dispatchStableButtonPress(EDIT_BUTTON_BITS[EDIT_BUTTON_SELECT]);
    patternSelected[t][activePattern[t]] = true; applyParamDeltaFromKy(0, GROOVE_COUNT + 1);
  if (playbackGroove(t, activePattern[t]) != GROOVE_COUNT || patternGroove[t][activePattern[t]]) extendedFailures |= 1024;
  p23BackOrRecover();
  if (patternGroove[t][activePattern[t]] || pendingPatternGroove >= 0) extendedFailures |= 2048;
  dispatchStableButtonPress(EDIT_BUTTON_BITS[EDIT_BUTTON_SELECT]); patternSelected[t][activePattern[t]] = true;
  applyParamDeltaFromKy(0, GROOVE_COUNT + 1); dispatchStableButtonPress(EDIT_BUTTON_BITS[EDIT_BUTTON_SELECT]);
  if (patternGroove[t][activePattern[t]] != GROOVE_COUNT) extendedFailures |= 4096;
  patternMode = false;
  patternGroove[t][activePattern[t]] = savedGrooveAssignment;
  memcpy(grooveOffset[GROOVE_COUNT-1], savedSheet, sizeof(savedSheet)); editedGroove = oldEditedGroove;
  grooveEditMode = false; grooveStepMask = 0; sendGroovePacket();
  Serial.print(extendedFailures ? F("GROOVEEXILECHECK,FAIL,MASK=") : F("GROOVEEXILECHECK,PASS,MASK=")); Serial.println(extendedFailures);
  // Restore the borrowed inactive cells, including stored locks/parameters.
  for (uint8_t step = 0; step < 3; step++) {
    CellBackup &c = backup[step];
    clearTrackStep(t, step);
    stepVel[t][step] = c.vel; stepMidiNote[t][step] = c.note; stepExtraNoteCount[t][step] = c.count;
    memcpy(stepExtraMidiNote[t][step], c.extra, sizeof(c.extra));
    stepGateTicks[t][step] = c.gate; stepMicroTick[t][step] = c.micro; stepLogicalMicro[t][step] = c.logical;
    stepLockVel[t][step] = c.lv; stepLockPitch[t][step] = c.lp; stepLockGate[t][step] = c.lg; stepLockTime[t][step] = c.lt;
    rollSet906(t,step,c.roll,c.rollMask);
  }
  memcpy(deletedNoteHistory, history, sizeof(history));
  deletedNoteHistoryCount = historyCount; deletedNoteHistoryCursor = historyCursor;
  selectedTrack = oldTrack; chromaticBottomNote = oldBottom; activePitchRow = oldPitch; activePitchTrack = oldPitchTrack;
  viewStartTrack = oldViewStart; selectedVoice = oldVoice; selectedFineIndex = oldFine;
  trackBars[t] = oldBars; sequenceBars = oldSequenceBars; currentStep = oldStep; measureTick = oldMeasure;
  zoomLevel = oldZoom; trackNoteViewMode[linkOwner12(t,LINK_VIEW12)] = oldNoteView; viewTickOffset = oldOffset; (pidiRoutesDirty931|=1u<<(t), trackPlayMode[linkOwner12(t,LINK_POLY12)] = oldPlayMode);
  trackMultiEditMode[t] = oldMultiEdit;
  pitchEncoderSelectMode = oldPitchMode; lastNoteValid = oldLastValid;
  lastGridTickValid = oldGridValid; lastGridTick = oldGridTick;
  p23Y1SwitchWasPressed = p23Y2SwitchWasPressed = false;
  sdNeoNoticeActive = oldSdNotice;
  neoDisplayDirty = displayValueDirty = displayBpmDirty = true;
  Serial.print(failures ? F("SELCHECK,FAIL,MASK=") : F("SELCHECK,PASS,MASK=")); Serial.println(failures);
}

#include "V8Checks.h"

void runChecks805();
void runFullCheck805();
#include "Perf812.h"
#include "Diag814Commands.h"
#include "Checks815.h"
void agendaStatus822();
#include "GrooveCheck16.h"
void checks42();
#include "Bench29.h"
FLASHMEM void handleUsbControlLine(char *line) {
 if(!strcmp(line,"SONGSCREEN59")){songPcSeen59=millis();songPcPresent59=true;return;}
 if(!strcmp(line,"GRAPH59?")){graphPreview59();return;}
 if(!strcmp(line,"STARTUP35?")){File f=sdAvailable?SD.open("/URBANKAOZ/PRJ/default.UKP",FILE_READ):File();FullProjectHeader h{};bool header=f&&f.read((uint8_t*)&h,sizeof(h))==sizeof(h);Serial.printf("STARTUP35,MODE=%u,SD=%u,EXISTS=%u,HEADER=%u,VERSION=%u,EXPECTED=%u,FOLLOW=%u\n",startupMode811,sdAvailable,(bool)f,header,header?h.version:0,FULL_PROJECT_VERSION,headFollow);f.close();return;}
 if(!strcmp(line,"EDIT34?")){uint8_t t=selectedTrack;Serial.printf("EDIT34,T=%u,QVT=%u,SELECT=%u,DRAFT=%u,MODE=%u,VEL=%d,GAT=%d,PLAY=%u\n",t,quickTrack903,selectionHasActiveNotes(),pendingSelectionEdit.active,editMode,blockNoteDisplay21(1),blockNoteDisplay21(2),playing);for(uint16_t s=0;s<trackStepCount(t);++s)if(stepSelected[t][s])for(uint8_t v=0;v<noteCountAtStep(t,s);++v)if(selectedNoteObject(t,s,v)){Serial.printf("EDITVOICE34,S=%u,V=%u,BASEVEL=%u,PLAYVEL=%u,BASEGAT=%u,PLAYGAT=%u\n",s,v,effectiveVel(t,s,v),audibleVel(t,s,v),effectiveGateTicks(t,s,v),audibleGateTicks(t,s,v));return;}return;}
 if(!strcmp(line,"PINTEST33?")){pinTestStatus33();return;}
  if(benchCommand29(line))return;
 if(!strcmp(line,"CHECKGROOVE16")){checkGroove16();return;}
 if(!strcmp(line,"GROOVE16?")){Serial.printf("GROOVE16,SHEET=%u,SMO=%u,WID=%u,VIW=%u,LOW=%u,HIGH=%u,OOV=%u,FORMAT=%u\n",editedGroove,grooveShape16[editedGroove-1][0],grooveShape16[editedGroove-1][1],grooveView16,oovLow16,oovHigh16,oovMenu16,FULL_PROJECT_VERSION);return;}
  if(!strcmp(line,"EXP14?")){Serial.printf("EXP14,MAX=%u,FACTOR100=%ld\n",expMaximum14,(long)expFactor14(100));return;}
  if(!strcmp(line,"NOTESTAT15")){uint16_t count=0;uint16_t tracks[16]={};for(const auto&n:activeNotes)if(n.active){++count;if(n.track<16)++tracks[n.track];}Serial.printf("NOTESTAT15,COUNT=%u,PEAK=%u,REJECTED=%lu,CAPACITY=%u\n",count,activeNotePeak15,activeNoteRejects15,MAX_ACTIVE_NOTES);for(uint8_t t=0;t<16;++t)if(tracks[t])Serial.printf("NOTEACTIVE15,T=%u,N=%u\n",t,tracks[t]);pidiStats827();midiStats810();return;}
  if(!strncmp(line,"NOTE14,",7)){Serial.printf("CONTEXT14,QV=%u,PAT=%u,RMX=%u,ITEM=%u,CHOOSING=%u,MENU=%u,SELECTED=%u,ACTIVE=%u\n",quickView903,patternMode,remixMode,itemKind809,itemChoosing809,menuNavigationOnly(),hasSelectedSteps(),selectionHasActiveNotes());unsigned t,s;if(sscanf(line+7,"%u,%u",&t,&s)==2&&t<16&&s<1024){Serial.printf("NOTE14,T=%u,S=%u,COUNT=%u,DEFAULT=%u/%u/%u/%d\n",t,s,noteCountAtStep(t,s),defaultMidiNote[t],defaultVel[t],defaultGateTicks[t],defaultMicroTick[t]);for(uint8_t v=0;v<noteCountAtStep(t,s);++v)Serial.printf("VOICE14,V=%u,N=%u,VEL=%u,GAT=%u,TIM=%d\n",v,noteAtVoice(t,s,v),effectiveVel(t,s,v),effectiveGateTicks(t,s,v),effectiveMicroTick(t,s,v));}return;}
  if(!strcmp(line,"BUSYTEST910")){if(playing){Serial.println("BUSYTEST910,BUSY");return;}bool ok=beginFullBusy910();uint32_t at=millis();while(ok&&millis()-at<1800){display821.service();serviceDisplayInput();pidiService827();}endFullBusy910();Serial.println(ok?"BUSYTEST910,PASS,NO_SD_WRITE":"BUSYTEST910,FAIL");return;}
  if(!strcmp(line,"DELAYSTAT924")){pidiDelayStatus924();return;}
  if(!strcmp(line,"REMIXSTAT")){remixStatus911();remixEditStatus912();return;}
  if(!strcmp(line,"CHECK913")){remixChecks913();return;}
  if(!strcmp(line,"CHECK912")){remixChecks912();return;}
  if(!strcmp(line,"CHECK42")){checks42();return;}
  if(!strcmp(line,"CHECK911")){remixChecks911();return;}
  if(!strcmp(line,"CHECK906")){checks906();return;}
  if(!strcmp(line,"MEM906")){memory906();return;}
  if(!strcmp(line,"CHECK905")){checks905();return;}
  if(recoveryCommand08(line))return;
  if(sdCommand905(line))return;
  if(analyzerCommand28(line))return;if(pcCommand831(line))return;
  if(sdBusy905()&&strcmp(line,"UISTAT"))return;
  if(!strcmp(line,"MIDIIN17?")){midiInStatus17();return;}
  if(!strcmp(line,"CHECKBUSY17")){
    if(playing||fullBusy910){Serial.println("CHECKBUSY17,BUSY");return;}
    bool ok=beginFullBusy910(true);uint32_t at=millis();
    if(ok)while(millis()-at<900){display821.service();serviceNoteOffs();pidiService827();}
    endFullBusy910();Serial.println(ok?"CHECKBUSY17,PASS":"CHECKBUSY17,FAIL");return;
  }
  if(!strcmp(line,"LATSTAT830")){if(playing){Serial.println("LAT830,STOP_BEFORE_REPORT");return;}status830();return;}
  if(!strcmp(line,"LATCLEAR830")){resetStats830();Serial.println("LAT830,CLEARED");return;}
  if(!strcmp(line,"CHECK830")){checks830();return;}
  if(!strcmp(line,"AGENDASTAT822")){agendaStatus822();return;}
  if(!strcmp(line,"OPTSTAT821")){
    Serial.printf("OPT821,QUEUED=%lu,SENT=%lu,REPLACED=%lu,DUP=%lu,DROP=%lu,NEO_WRITE=%lu,NEO_AVOID=%lu,SLICE_MAX_US=%lu,BACKLOG_MAX=%lu,TICKS=%lu,OVERRUN=%lu\n",display821.queued,display821.sent,display821.replaced,display821.duplicates,display821.overflow,neoWrites821,neoAvoided821,transportMax821,transportBacklog821,(uint32_t)pendingClockTicks,(uint32_t)clockOverruns821);return;
  }
  if(!strcmp(line,"CHECK819")){checks819();return;}
  if(!strcmp(line,"SAVESTAT07")){Serial.printf("SAVE07,ACTIVE=%u,OK=%u,PHASE=%u,STEP=%u,MS=%lu\n",saveProbe07.active,saveProbe07.ok,saveProbe07.phase,saveProbe07.step,(unsigned long)saveProbe07.elapsed);return;}
  if(!strcmp(line,"CLOCKSTAT05")){Serial.printf("CLOCK05,MAX_CYCLES=%lu,PC=%08lX,IRQ=%lu\n",(unsigned long)clockProbeMax05,(unsigned long)clockProbePc05,(unsigned long)clockProbeIrq05);return;}
  if(!strcmp(line,"PLAYBACKSTAT")){playbackStats828();return;}
  if(!strcmp(line,"CHECK823")){keyboardCheck823();return;}
  if(!strcmp(line,"CHECK820")){recordCheck820();return;}
  if(!strcmp(line,"RECSTAT820")){Serial.printf("REC820,ARMED=%u,SONG=%u,PLAY=%u\n",songEngineActive?songRecord:recordingArmed,songEngineActive,playing);return;}
  if(!strcmp(line,"PATCHSTAT819")){Serial.printf("PATCHSTAT819,DIR=%d,TRACK=%u,BANK=%u,PAGE=%lu\n",effectiveKy40Setting(3),selectedTrack,patchBank808,(unsigned long)patchPage808);return;}
  if(!strcmp(line,"CHECK817FILES")){checksFiles817();return;}
  if(!strcmp(line,"SAVECHECK45")){saveCheck45(false);return;}
  if(!strcmp(line,"LOCKSTAT11")){uint16_t free=0;for(uint16_t i=0;i<lockCount817;++i)free+=noteLocks817[i].note>=128;Serial.printf("LOCK11,COUNT=%u,FREE=%u,EMPTYSEL=%u,GAT=%d,AUTOSAVE=OFF\n",lockCount817,free,selectedEmptyCount,selectedEmptyCount?freeGateValue11():0);for(uint16_t i=0;i<lockCount817;++i){auto&l=noteLocks817[i];if(l.note>=128)Serial.printf("FREE11,T=%u,P=%u,TICK=%ld,PITCH=%u,KIND=%u,PARAM=%u,VALUE=%ld,GAT=%lu\n",l.track,l.pattern,(long)l.tick,l.note&127,l.kind,l.parameter,(long)l.value,(unsigned long)l.duration);}return;}
  if(!strcmp(line,"CHECK817")){checks817();return;}
  if(!strcmp(line,"CHECK816")){checks816();return;}
  if(!strcmp(line,"CHECK818")){checks818();return;}
  if(!strcmp(line,"PATCHCHECK818")){patchCheck818();return;}
  if(!strcmp(line,"BUTTONSTAT")){Serial.printf("BUTTON816,PRESS=%u,RELEASE=%u,ROY=%u,KY40=%u,KY23=%u\n",hc165PressDebounceMs,hc165ReleaseDebounceMs,roySwitchDebounceMs,picoButtonDebounceMs,ky23Debounce816);return;}
  if(!strncmp(line,"MIDI15,",7)){int t,c,n,v,g,mask;if(sscanf(line+7,"%d,%d,%d,%d,%d,%d",&t,&c,&n,&v,&g,&mask)==6&&!playing&&t>=0&&t<16&&c>=0&&c<16&&n>=0&&n<128&&v>0&&v<128&&g>=1&&g<=10000&&mask>=1&&mask<=255){MidiScope902 route(mask,t);bool ok=scheduleNoteOffAtUs(c,n,micros()+g*1000u,false,false,t);if(ok)ok=noteOnNow(c,n,v);Serial.printf("MIDI15,%s\n",ok?"OK":"REJECTED");}else Serial.println("MIDI15,INVALID");return;}
  if(!strcmp(line,"CHECK15EMPTY")){checksEmpty15();return;}
  if(!strcmp(line,"CHECK917")){checks917();return;}
  if(!strcmp(line,"CHECK916")){checks916();return;}
  if(!strcmp(line,"CHECK915")){checks915();return;}
  if(!strcmp(line,"STAT915")){status915();return;}
  if(!strcmp(line,"CHECK904")){checks904();return;}
  if(!strcmp(line,"STABLESTAT")){statusStability904();return;}
  if(!strcmp(line,"CHECK903")){checks903();return;}
  if(!strcmp(line,"QUICKSTAT")){Serial.printf("QUICK903,VIEW=%u,ITEM=%lu,TRACK=%u,PAGE=%lu,CUT_NOTES=%u,CUT_DELAY=%u,PLAYING=%u,HC_BITS=%u\n",quickView903,quickItem903,quickTrack903+1,quickPatchPage903,cutNotes903,cutDelay903,playing,HC165_BIT_COUNT);return;}
  if(!strcmp(line,"CHECK902")){checks902();return;}
  if(!strcmp(line,"CHECK815")){checks815();return;}
  if(diagCommand814(line)){perfSkip812=true;return;}
  if(perfCommand812(line))return;
  if(!strcmp(line,"CHECK805")){runChecks805();return;}
  if(!strcmp(line,"FULLCHECK805")){runFullCheck805();return;}
  if(!strcmp(line,"MODCHECK807")){mod806Checks();return;}
  if(!strncmp(line,"FULLDIAG808,",12)) {char path[128];snprintf(path,sizeof(path),"/URBANKAOZ/PRJ/%s",line+12);Serial.print("FULLDIAG808,");Serial.println(full805Validate(path)?"PASS":"FAIL");return;}
  if(!strcmp(line,"CHECK808")){checks808();return;}
  if(!strcmp(line,"MIDISTAT")){midiStats810();return;}
  if(!strcmp(line,"MIDICLEAR")){midiClearStats810();return;}
  if(!strcmp(line,"CHECK811")){checks811();return;}
  if(!strcmp(line,"CHECK810")){checks810();return;}
  if(!strcmp(line,"CHECK809")){itemChecks809();return;}
  if(!strcmp(line,"HOME805")&&!playing){showGameScreen();return;}
  if(!strncmp(line,"RESTORE805,",11)&&!playing){
    const char* name=line+11; bool valid=*name!=0;
    for(const char* c=name;*c;++c)if(!((*c>='A'&&*c<='Z')||(*c>='0'&&*c<='9')||*c=='_'||*c=='.'))valid=false;
    if(strstr(name,"..")||strlen(name)>32)valid=false;
    char path[80];snprintf(path,sizeof(path),"/URBANKAOZ/PRJ/%s",name);
    bool ok=valid&&loadFullProject(path);showGameScreen();songSnapshot();Serial.println(ok?"RESTORE805,PASS":"RESTORE805,FAIL");return;
  }
  if(!strcmp(line,"V8CHECK")){runV8Check();return;}
  if (!strncmp(line,"SONG,",5)) { songCommand(line+5); return; }
  int d, r, b, recovery;
  if (sscanf(line, "KYCFG,%d,%d,%d,%d", &d, &r, &b, &recovery) == 4) {
    settingsValue[1][2] = (int16_t)clampInt(d, PICO_DETENT_MIN, PICO_DETENT_MAX);
    settingsValue[1][1] = (int16_t)clampInt(r, 0, 80);
    picoButtonDebounceMs = (int16_t)clampInt(b, 0, 120);
    picoRecoveryMode = (int16_t)clampInt(recovery, 0, 1);
    resetPicoRxStats();
    saveSettingsToEEPROM();
    sendPicoDefaults();
    Serial.print(F("KYCFG OK,D=")); Serial.print(settingsValue[1][2]);
    Serial.print(F(",R=")); Serial.print(settingsValue[1][1]);
    Serial.print(F(",B=")); Serial.print(picoButtonDebounceMs);
    Serial.print(F(",RCVR=")); Serial.println(picoRecoveryMode);
  } else if (sscanf(line, "UIDLY,%d", &d) == 1) {
    settingsValue[3][0] = (int16_t)clampInt(d, 0, 1000);
    saveSettingsToEEPROM();
    Serial.print(F("UIDLY OK,")); Serial.println(settingsValue[3][0]);
  } else if (strcmp(line, "KYSTAT") == 0) {
    printKyRxStats();
  } else if (strcmp(line, "PIDISTAT") == 0) {
    pidiStats827();
  } else if (strcmp(line, "ROLLSTAT") == 0) {
    printRollStats();
  } else if (strcmp(line, "VIEWLOG") == 0) {
    printViewLog();
  } else if (strcmp(line, "VIEWCLR") == 0) {
    viewLogWrite = 0;
    viewLogUsed = 0;
    Serial.println(F("VIEWLOG CLEARED"));
  } else if (strcmp(line, "PATCHSTAT10") == 0) {
    patchStats10();
  } else if(!strcmp(line,"CHECKSTORAGE18")){checksStorage18();
  } else if(!strcmp(line,"STORAGE18?")){Serial.printf("STORAGE18,KIND=%u,EP=%u,KB=%u,CLIP=%u,CHOICE=%u,DESC=%s\n",itemKind809,modUi805,kbMenuR10,saveMenuClipboardMode,saveMenuSelection,fileBrowserMode?storageDescription18():"NONE");
  } else if (strcmp(line, "TRACKSTAT12") == 0) {
    Serial.printf("TRACKSTAT12,T=%u,LINK=%u,SRC=%u,MOUT=%u,CH=%u,CENTER=%ld,SPLIT=%u,CUT=%u,TRACKMENU=%u,LINKMENU=%u,MOUTMENU=%u,STEP=%d,ZOOM=%d,MOV=%lu\n",selectedTrack+1,linkMask12[selectedTrack],linkSource12[selectedTrack]+1,outMask902(selectedTrack),midiChannelForTrack(selectedTrack)+1,(long)kbCenter12,kbSettings13(selectedTrack).split,!!(delayCut12&(1u<<linkOwner12(selectedTrack,LINK_DLY12))),trackMenuActive,linkMenu12,trackMidiMenu902,selectedStep,zoomLevel,(unsigned long)max(1UL,windowTickSpan()/16));
  } else if (strcmp(line, "UISTAT") == 0) {
    Serial.print(F("UISTAT,FW=")); Serial.print(APP_VERSION); Serial.print(F(",REV=")); Serial.print(APP_VERSION);
    Serial.print(F(",PAGE=")); Serial.print((uint8_t)currentMenuPageRef());
    Serial.print(F(",STOP=")); Serial.print(menuNavigationOnly());
    Serial.print(F(",PLAY=")); Serial.print(playing);
    Serial.print(F(",DEBUG=")); Serial.print((uint8_t)debugTestMode);
    Serial.print(F(",SELECT=")); Serial.print(selectButtonActive);
    Serial.print(F(",SHIFT=")); Serial.print(shiftButtonActive);
    Serial.print(F(",DELETE=")); Serial.print(deleteButtonActive);
    Serial.print(F(",GROOVE=")); Serial.print(grooveEditMode);
    Serial.print(F(",SD=")); Serial.print(sdAvailable);
    Serial.print(F(",SDWRITE=")); Serial.print(storageWriteAvailable);
    Serial.print(F(",SDNEO=")); Serial.println(sdNeoNoticeActive);
  } else if (strcmp(line, "HCRAW") == 0) {
    uint64_t state = read74HC165();
    Serial.print(F("HCRAW,BITS="));
    for (int8_t bit = HC165_BIT_COUNT - 1; bit >= 0; bit--) {
      Serial.print((state >> bit) & 1ULL);
    }
    Serial.print(F(",NAV_BITS="));
    for(int8_t bit=15;bit>=0;--bit)Serial.print((navigationRaw49>>bit)&1);
    Serial.print(F(",PHYSICAL_BITS=72,DATA=")); Serial.println(digitalRead(HC165_PIN_DATA));
  } else if (strcmp(line, "HCINIT") == 0) {
    initTransportButtons();
    Serial.println(F("HCINIT,OK,PINS=26/27/28"));
  } else if (strcmp(line, "SELCHECK") == 0) {
    runSelectionSelfCheck();
  } else if (strcmp(line, "UICHECK") == 0) {
    // Diagnostic reproductible a l'accueil arrete, meme sans carte SD.
    if (playing || currentMenuPageRef() != PAGE_A) {
      Serial.println(F("UICHECK,BUSY"));
      return;
    }
    bool ok = true;
    showParaScreen();
    settingsCategoryPage = 1;
    // Page 2 : <, BUTTON, COPY, DEBUG. Le meme mapping alimente les BE.
    openSettingsCategoryByBe(3);
    ok &= settingsOpen && settingsPage == 6 && settingsMenuLevel == SETTINGS_ITEMS;
    screenUiAwaitRelease = false;
    handleScreenButtonPress(SCREEN_BUTTON_BITS[5]); // BTNS est le sixieme item.
    ok &= debugTestMode == DEBUG_TEST_BUTTONS;
    serviceSettingsPageRetry();
    ok &= debugTestMode == DEBUG_TEST_BUTTONS && !settingsPageRetryPending;
    p23BackOrRecover();
    ok &= debugTestMode == DEBUG_TEST_NONE && settingsPage == 6 && settingsMenuLevel == SETTINGS_ITEMS;
    p23BackOrRecover();
    ok &= settingsMenuLevel == SETTINGS_CATEGORIES && settingsCategoryPage == 1;
    p23BackOrRecover();
    ok &= currentMenuPageRef() == PAGE_A;
    sendTrackMenu();
    sendTrackMultiMenu();
    p23BackOrRecover();
    ok &= currentMenuPageRef() == PAGE_AC;
    p23BackOrRecover();
    p23BackOrRecover();
    ok &= currentMenuPageRef() == PAGE_A && !playing;
    Serial.println(ok ? F("UICHECK,PASS,BTNS,RETRY,BACK,TRACK,HOME") : F("UICHECK,FAIL"));
  }
}

void serviceSelectionIndicators() {
  if(!selectionIndicatorsPending931)return;
  sendSelectionIndicatorsToPicran();
}

void serviceUsbControl() {
  static bool discard831=false;unsigned budget831=160;
  while (budget831-- && Serial.available() > 0) {
    char c = (char)Serial.read();
    if (c == '\r') continue;
    if (c == '\n') {
      usbControlLine[usbControlLinePos] = '\0';
      if (!discard831 && usbControlLinePos > 0) handleUsbControlLine(usbControlLine);
      discard831=false;
      usbControlLinePos = 0;
    } else if(discard831) { continue;
    } else if (usbControlLinePos < sizeof(usbControlLine) - 1) {
      usbControlLine[usbControlLinePos++] = c;
    } else {
      usbControlLinePos = 0;discard831=true;
    }
  }
}


// ============================================================
// TRANSPORT PAR 74HC165
// ============================================================
void updateRoySelectionIndicators() {
  if (!royPixelsReady) return;
  for (uint8_t pixel = 0; pixel < 4; pixel++) royPixels.setPixelColor(pixel, 0);
  if (selectButtonActive) {
    royPixels.setPixelColor(ROY_PIXEL_SELECT_OA, royPixels.Color(0, 95, 45));
  }
  if (shiftButtonActive) {
    royPixels.setPixelColor(ROY_PIXEL_SHIFT_OB, royPixels.Color(95, 45, 0));
  }
  royPixels.show();
}

int8_t editActionForBit(uint8_t bit) {
  for (uint8_t action = 0; action < EDIT_BUTTON_COUNT; action++) {
    if (EDIT_BUTTON_BITS[action] == bit) return (int8_t)action;
  }
  return -1;
}

void deleteBackupPath(uint8_t track, uint8_t pattern, char path[40]) {
  snprintf(path, 40, "/URBANKAOZ/PAT/.T%02u_P%02u.DEL", track + 1, pattern + 1);
}

bool restoreDeletePatternFiles() {
  bool restored = true;
  for (uint8_t t = 0; t < TRACK_COUNT; t++) for (uint8_t p = 0; p < PATTERN_COUNT; p++) {
    if (!(deleteRenamedPatterns[t] & (1u << p))) continue;
    char path[40], backup[40]; internalPatternPath(t, p, path); deleteBackupPath(t, p, backup);
    if (SD.rename(backup, path)) deleteRenamedPatterns[t] &= ~(1u << p);
    else restored = false;
  }
  return restored;
}

FLASHMEM bool finishDeleteMode(bool commit) {
  if (!deleteButtonActive) return false;
  if(deleteNumeric55){deleteNumeric55=false;deleteButtonActive=false;deleteCancel55();sendSelectionIndicatorsToPicran();neoDisplayDirty=displayValueDirty=true;return true;}
  if(deleteLocksMode15){deleteLocksMode15=false;deleteButtonActive=false;selection805Changed();return true;}
  if (!restoreDeletePatternFiles()) {
    DISPLAY_SERIAL.println(F("MSG|RESTORE SD THEN J1")); return false;
  }
  if (commit && deletePatternContext) {
    // Prepare all file removals by renaming. On error, restore every rename
    // before returning; RAM patterns have not yet been changed.
    bool ok = true;
    if (selectedPatternCount() && !storageWriteAvailable) ok = false;
    for (uint8_t t = 0; t < TRACK_COUNT && ok; t++) for (uint8_t p = 0; p < PATTERN_COUNT && ok; p++) {
      if (!patternSelected[t][p]) continue;
      char path[40], backup[40]; internalPatternPath(t, p, path); deleteBackupPath(t, p, backup);
      if (SD.exists(backup)) { ok = false; break; }
      if (SD.exists(path)) {
        ok = SD.rename(path, backup);
        if (ok) deleteRenamedPatterns[t] |= (1u << p);
      }
    }
    if (!ok) {
      bool restored = restoreDeletePatternFiles();
      DISPLAY_SERIAL.println(restored ? F("MSG|DELETE SD ERROR - J1 CANCEL") : F("MSG|DELETE BACKUP .DEL SD ERROR"));
      return false;
    }
    allNotesOff();
    for (uint8_t t = 0; t < TRACK_COUNT; t++) for (uint8_t p = 0; p < PATTERN_COUNT; p++) {
      if (!patternSelected[t][p]) continue;
      if (activePattern[t] == p) for (uint16_t step = 0; step < STEP_COUNT; step++) clearTrackStep(t, step);
      if (pendingPattern[t] == (int8_t)p) pendingPattern[t] = -1;
      mod806DetachPattern(t,p);
      pattern805Saved(t,p);
      patternBank[t][p].initialized = false;
      patternBank[t][p].hasNotes = false;patternBank[t][p].bars=1;patternLinks911[t][p]=0;
      if(activePattern[t]==p){trackBars[t]=1;trackLoopStartStep[t]=1;trackLoopEndStep[t]=16;}
      if(lastPatternValid&&lastPatternTrack==t&&lastPatternIndex==p)lastPatternValid=false;
      patternGroove[t][p] = 0;
      memset(grooveLists[t][p], 0, sizeof(grooveLists[t][p]));
      if (deleteRenamedPatterns[t] & (1u << p)) {
        char backup[40]; deleteBackupPath(t, p, backup);
        if (!SD.remove(backup)) Serial.println(F("DELETE,STALE_BACKUP"));
        deleteRenamedPatterns[t] &= ~(1u << p);
      }
    }
    // Deletion history must not revive notes belonging to deleted patterns.
    deletedNoteHistoryCount = deletedNoteHistoryCursor = 0;
  } else if (commit) {
    freeDeleteSelected11();
    allNotesOff();
    for (uint8_t t = 0; t < TRACK_COUNT; t++) for (uint16_t step = 0; step < STEP_COUNT; step++) {
      for (int voice = (int)noteCountAtStep(t, step) - 1; voice >= 0; voice--) {
        if (!selectedNoteObject(t, step, (uint8_t)voice)) continue;
        rememberDeletedNote(t, step, (uint8_t)voice);
        removeVoiceAtStep(t, step, (uint8_t)voice);
      }
    }
  }
  deleteButtonActive = false;
  clearStepSelection(); clearPatternSelection();
  selectedStep = selectedFineIndex = -1;
  invalidateStepTriggers();
  neoDisplayDirty = displayValueDirty = true;
  DISPLAY_SERIAL.println(commit ? F("MSG|DELETE VALIDATED") : F("MSG|DELETE CANCELLED"));
  return true;
}


#include "GrooveUndo50.h"
FLASHMEM void handleEditButtonPress(uint8_t action) {
  if(grooveDelete50(action))return;
  if(action==EDIT_BUTTON_SELECT&&selectButtonActive&&trackMenuActive&&trackPage933==4){if(leaveStepSelection(true))programMenu939();return;}

  selectionIndicatorsPending931=true;
  if(action==EDIT_BUTTON_DELETE&&!deleteButtonActive&&(selectButtonActive||quickView903||modUi805==MOD_OBJECT806||modUi805==MOD_DELAY806)&&!remixMode&&!songMode&&!grooveEditMode){deleteButtonActive=true;deleteNumeric55=true;deleteLocksMode15=false;deleteCancel55();neoDisplayDirty=displayValueDirty=true;sendSelectionIndicatorsToPicran();return;}
  if(action==EDIT_BUTTON_DELETE&&deleteButtonActive&&deleteNumeric55){finishDeleteMode(true);return;}
  if(action==EDIT_BUTTON_DELETE&&!deleteButtonActive&&!remixMode&&!songMode&&!grooveEditMode){
    if(patternMode&&selectedPatternCount()){deletePatternContext=true;deleteButtonActive=true;if(!finishDeleteMode(true)){deleteButtonActive=false;return;}leaveStepSelection(false);selection805Changed();return;}
    if(!patternMode&&selectedEmptyCount&&freeDeleteSelected11()){if(selectionHasActiveNotes())deleteStepSelection();selection805Changed();return;}
    if(!patternMode&&selectionHasActiveNotes()){deleteStepSelection();selection805Changed();return;}
  }
  if(action==EDIT_BUTTON_DELETE&&patchDelete819())return;
  if (grooveEditMode && action != EDIT_BUTTON_SHIFT && action != EDIT_BUTTON_SELECT_ALL && action != EDIT_BUTTON_SELECT) return;
  if (debugTestMode != DEBUG_TEST_NONE) return;
  if (menuNavigationOnly()) return;
  // Delete remains available alongside selection and other edit actions.
  switch (action) {
    case EDIT_BUTTON_SHIFT:
      selection805ToggleShift(); return;
    case EDIT_BUTTON_SELECT_ALL:
      selection805All(); return;
    case EDIT_BUTTON_UNDO:
      performUndo();
      break;
    case EDIT_BUTTON_REDO:
      performRedo();
      break;
    case EDIT_BUTTON_SELECT: {
      bool enable = !selectButtonActive;
      if (!enable || shiftButtonActive) { if(!freeDraftFinish25(true))return;if (!leaveStepSelection(true)) return; }
      else {
        commitPendingSelectionEdit();
        clearStepSelection();
        clearPatternSelection();
        selectedStep = -1;
      }
      for (uint8_t n = 0; n < NEO_COUNT; n++) for (uint8_t p = 0; p < 16; p++)
        if (neoPadDown[n][p]) neoPadEdited[n][p] = true;
      selectButtonActive = enable;
      shiftButtonActive = false;
      selectionMode = enable;
      selectionAnchorValid = patternSelectionAnchorValid = false;
      DISPLAY_SERIAL.println(enable ? F("MSG|SELECT ON") : F("MSG|SELECT OFF"));
      break;
    }
    case EDIT_BUTTON_DELETE:
      if (deleteButtonActive) finishDeleteMode(true);
      else {
        if (selectButtonActive || pendingSelectionEdit.active) {
          DISPLAY_SERIAL.println(F("MSG|FINISH SELECT FIRST")); return;
        }
        leaveStepSelection(false);
        clearStepSelection(); clearPatternSelection();
        deletePatternContext = patternMode;
        deleteButtonActive = true;
        explicitNoteSelection = true;
        selectedStep = -1;
        for (uint8_t n = 0; n < NEO_COUNT; n++) for (uint8_t p = 0; p < 16; p++)
          if (neoPadDown[n][p]) neoPadEdited[n][p] = true;
        DISPLAY_SERIAL.println(F("MSG|DELETE ON"));
      }
      break;
  }
  updateRoySelectionIndicators();
  sendSelectionIndicatorsToPicran();
  Serial.print(F("EDITMODE,SELECT=")); Serial.print(selectButtonActive ? 1 : 0);
  Serial.print(F(",SHIFT=")); Serial.print(shiftButtonActive ? 1 : 0);
  Serial.print(F(",DELETE=")); Serial.println(deleteButtonActive ? 1 : 0);
  neoDisplayDirty = true;
  displayValueDirty = true;
}

FLASHMEM void dispatchStableButtonPress(uint8_t bit) {
 if(bit<HC165_BIT_COUNT)presentationTouch43();
 if(bit==53&&debugTestMode==DEBUG_TEST_NONE&&!sdBusy905()&&!controller940){bool on=!modMixer939;leavePadModesA01(false);modMixer939=trackChord815=on;neoDisplayDirty=true;sendSelectionIndicatorsToPicran();return;}
 if(bit>=56&&bit<64){static const uint8_t phys51[]={4,5,6,7,12,13,14,15};navigationPress49(phys51[bit-56]);return;}
 alertAction08();
 if(bit==15&&debugTestMode==DEBUG_TEST_NONE&&!sdBusy905()){indToggle52();return;}
 if(controller940){if(bit==0)controllerEmit940("SELECT",0,1);else if(bit==7)controllerEmit940("COLUMN",0,1);else if(bit==6)controllerEmit940("ALL",0,1);return;}
  // Delete belongs to the visible EP editor; never arm the Neo surface too.

  NeoInputScope932 neoScope(bit<12||bit>=24);
  if(sdBusy905())return;
  if(bit==EDIT_BUTTON_BITS[EDIT_BUTTON_UNDO]){leaveStepSelection(false);selectButtonActive=selectionMode=shiftButtonActive=columnSelectionMode=false;clearPatternSelection();selection805Changed();}
  if(debugTestMode==DEBUG_TEST_NONE&&(bit==3||bit==4||bit==9||bit==10||(bit>=12&&bit<=15)))recordExitA01();if(debugTestMode==DEBUG_TEST_NONE&&bit==14){toggleDelayMixer930();return;}
  if(debugTestMode==DEBUG_TEST_NONE&&(bit==3||bit==4||bit==9||bit==10||bit==11)&&delayMixer930){delayMixer930=false;neoDisplayDirty=true;}
  if(debugTestMode==DEBUG_TEST_NONE&&remixButton911(bit))return;
  if(debugTestMode==DEBUG_TEST_NONE&&patternLinkButton911(bit))return;
  if(debugTestMode==DEBUG_TEST_NONE&&quickButton903(bit))return;
  InputScope815 input(I815_HC,bit,true);
  if(bit==11&&debugTestMode==DEBUG_TEST_NONE&&!itemKind809&&!itemChoosing809&&!numericEntryActive){keyboardToggle823();return;}
  if(recordOctave820(bit))return;
  if(bit==5&&recordArmed823()&&!settingsOpen){recordClose820();recordDelete51=!recordDelete51;selectionIndicatorsPending931=true;neoDisplayDirty=true;sendSelectionIndicatorsToPicran();return;}
  // Unused inputs of the existing six-register chain; do not shift legacy bits.
  if(bit>=12&&bit<=15){
    if(bit<=13&&trackStorageOpen12(bit==12))return;
    if(bit<=13&&itemOpen809(bit==12))return;
    if(itemKind809||itemChoosing809)return;
    while(modUi805)mod805Back();while(patchUi808)patch808Back();
    if(bit==12)openSaveMenu();else if(bit==13)openLoadBrowser();else if(bit==15)patch808Toggle(1);
    return;
  }
  // Neo mode changes do not close the independent EP editor.

  if(bit==EDIT_BUTTON_BITS[EDIT_BUTTON_UNDO]&&(autoUndo46()||moveHistory939(false)||mod807Undo()))return; if(bit==EDIT_BUTTON_BITS[EDIT_BUTTON_REDO]&&moveHistory939(true))return;
  if(modUi805&&bit>=16&&bit<=23){
    if(bit==EDIT_BUTTON_BITS[EDIT_BUTTON_DELETE])return;
    else handleScreenButtonPress(bit);
    return;
  }
  if (bit == 9) { if(!menuNavigationOnly())songSetMode(!songMode); return; }
  if (bit == 7) { selection805ToggleColumn(); return; }
  if(patchUi808==4&&bit==EDIT_BUTTON_BITS[EDIT_BUTTON_DELETE]){nameDeleteR8();return;}
  if(fileBrowserMode==FILE_BROWSER_SAVE&&bit==EDIT_BUTTON_BITS[EDIT_BUTTON_DELETE]){storageNameDelete();return;}
  if(songMode&&bit==EDIT_BUTTON_BITS[EDIT_BUTTON_SELECT]&&!menuNavigationOnly()){
    shiftButtonActive=columnSelectionMode=false;selectButtonActive=!selectButtonActive;selectionMode=selectButtonActive;songExportArmed=false;songState();sendSelectionIndicatorsToPicran();return;
  }
  if(songMode&&bit==EDIT_BUTTON_BITS[EDIT_BUTTON_DELETE]&&!menuNavigationOnly()){
    songDeleteArmed=!songDeleteArmed;songState();return;
  }
  if(songMode&&(bit==3||bit==4||bit==10)){songLeaveSurface();if(bit==10){patternMode=true;neoDisplayDirty=displayValueDirty=displayBpmDirty=true;sendSelectionIndicatorsToPicran();return;}}
  // Physical HC165 bits, not the historical row/column button labels.
  if (bit == 3 || bit == 4 || bit == 10) {
    if (debugTestMode != DEBUG_TEST_NONE || menuNavigationOnly()) return;
    deleteButtonActive=false;
    if(selectButtonActive||pendingSelectionEdit.active){if(!leaveStepSelection(false))return;}
    if (shiftButtonActive || columnSelectionMode || selectionMode) leaveStepSelection(false);
    selectButtonActive=selectionMode=shiftButtonActive=columnSelectionMode=false;
    clearPatternSelection();
    if(bit!=10){quickView903=0;DISPLAY_SERIAL.println("CUSTOM,OFF");}
    patternPasteTargetHeld = patternPasteTargetConsumed = false;
    grooveStepMask = 0;
    keyboardEnabled823=false;trackChord815=false;delayMixer930=modMixer939=false;bool nextRemix=bit==4&&!remixMode,nextGroove=bit==3&&!grooveEditMode;
    if(remixMode){(pidiStateDirty931=true, remixMode=false);remixLeave911();}
    if(bit==3&&grooveEditMode){grooveFinish60(true);if(patternMode){patternMode=false;}else{patternMode=true;grooveListFocus40(selectedTrack,activePattern[selectedTrack]);sendGrooveListPacket();}}
    else if(bit==10){grooveEditMode=false;patternMode=!patternMode;}
    else {patternMode=false;grooveEditMode=nextGroove;(pidiStateDirty931=true, remixMode=nextRemix);if(nextGroove){grooveView16=false;editedGroove=grooveSheet35(selectedTrack);uint8_t id=blocks21[selectedTrack][0].groove;blockListEdit21=id?id-1:0;memset(grooveAssigned805,0,sizeof(grooveAssigned805));}}
    if(remixMode){if(!remixEnter911())(pidiStateDirty931=true, remixMode=false);}else remixLeave911();
    neoDisplayDirty = displayValueDirty = displayBpmDirty = true;
    sendGroovePacket();
    sendSelectionIndicatorsToPicran();
    return;
  }
  int8_t editAction = editActionForBit(bit);
  if (editAction >= 0) handleEditButtonPress((uint8_t)editAction);
  else handleScreenButtonPress(bit);
}

uint64_t read74HC165() {
  uint64_t value = 0;

  digitalWrite(HC165_PIN_LATCH, LOW);
  delayMicroseconds(5);
  digitalWrite(HC165_PIN_LATCH, HIGH);
  delayMicroseconds(5);

  for (uint8_t i = 0; i < 56; i++) {
    value <<= 1;
    if (digitalRead(HC165_PIN_DATA)) value |= 1ULL;

    digitalWrite(HC165_PIN_CLOCK, HIGH);
    delayMicroseconds(2);
    digitalWrite(HC165_PIN_CLOCK, LOW);
    delayMicroseconds(2);
  }

  navigationRaw49=0;
  for(uint8_t i=0;i<16;++i){
    navigationRaw49=(uint16_t)((navigationRaw49<<1)|(digitalRead(HC165_PIN_DATA)?1:0));
    digitalWrite(HC165_PIN_CLOCK,HIGH);delayMicroseconds(2);
    digitalWrite(HC165_PIN_CLOCK,LOW);delayMicroseconds(2);
  }
  return value;
}

#include "Navigation49.h"

void invalidateStepTriggers() {
  lockEventInvalidate30();
  mixerValid930=0;
  for (uint8_t track = 0; track < TRACK_COUNT; track++) {
    for (uint16_t step = 0; step < STEP_COUNT; step++) {
      lastTriggeredCycle[track][step] = UINT32_MAX;
    }
    for (uint8_t fine = 0; fine < FINE_EVENTS_PER_TRACK; fine++) {
      fineLastTriggeredCycle[track][fine] = UINT32_MAX;
    }
  }
}

void jumpPlaybackToMeasureStep(uint16_t measureIndex, uint16_t step) {
  if (measureIndex >= sequenceBars || step >= STEPS_PER_BAR) return;

  resetGrooveBridges();
  // Equivalent du saut direct de GhostR : on remplace la position maitre,
  // on abandonne l'ancienne echeance et la lecture continue depuis ici.
  allNotesOff();
  currentStep = measureIndex * STEPS_PER_BAR + step;
  lastDisplayedStep = 0xFFFF;
  transportTickAbsolute = (uint64_t)currentStep * TICKS_PER_STEP;
  measureTick = localTickForTrack(selectedTrack);
  evaluateCurrentTickOnNextClock = true;
  pendingClockTicks = 0;
  invalidateStepTriggers();

  // Force le rafraichissement sans attendre la cadence normale des Neo.
  neoDisplayDirty = true;
  lastNeoRefreshMs = millis() - NEO_MIN_REFRESH_MS;

  Serial.print(F("REMIX JUMP: mesure "));
  Serial.print((uint16_t)measureIndex + 1);
  Serial.print(F(" step "));
  Serial.println((uint16_t)step + 1);
}

void setTransportPositionToMeasureStart() {
  resetGrooveBridges();
  allNotesOff();
  transportTickAbsolute = 0;
  measureTick = 0;
  currentStep = 0;
  evaluateCurrentTickOnNextClock = true;
  pendingClockTicks = 0;
  invalidateStepTriggers();
  neoDisplayDirty = true;
  displayBpmDirty = true;
}

void setTransportPositionToBar(uint16_t bar) {
  if (sequenceBars == 0) return;
  resetGrooveBridges();
  uint16_t firstLoopBar = (loopStartStep - 1) / STEPS_PER_BAR;
  uint16_t lastLoopBar = (loopEndStep - 1) / STEPS_PER_BAR;
  bar = (uint16_t)clampInt(bar, firstLoopBar, lastLoopBar);
  allNotesOff();
  currentStep = bar * STEPS_PER_BAR;
  transportTickAbsolute = (uint64_t)currentStep * TICKS_PER_STEP;
  measureTick = localTickForTrack(selectedTrack);
  evaluateCurrentTickOnNextClock = true;
  pendingClockTicks = 0;
  invalidateStepTriggers();
  // Transport never moves the Neo editing window.
  neoDisplayDirty = true;
  displayBpmDirty = true;
}

void transportReturnToSequenceStart() {
  cycleCounter = 0;
  setTransportPositionToMeasureStart();
  Serial.println(F("TRANSPORT: RETOUR DEBUT SEQUENCE"));
}

void transportPreviousMeasure() {
  TransportScope903 scope(4);
  uint16_t currentBar = currentStep / STEPS_PER_BAR;
  uint32_t tickInBar = measureTick % ((uint32_t)STEPS_PER_BAR * TICKS_PER_STEP);
  uint16_t targetBar = currentBar;
  if (tickInBar < PPQN && currentBar > 0) targetBar = currentBar - 1;
  setTransportPositionToBar(targetBar);
  Serial.print(F("TRANSPORT: MP -> mesure "));
  Serial.println(targetBar + 1);
}

void transportNextMeasure() {
  TransportScope903 scope(5);
  uint16_t currentBar = currentStep / STEPS_PER_BAR;
  uint16_t lastLoopBar = (loopEndStep - 1) / STEPS_PER_BAR;
  uint16_t targetBar = currentBar < lastLoopBar ? currentBar + 1 : currentBar;
  setTransportPositionToBar(targetBar);
  Serial.print(F("TRANSPORT: MS -> mesure "));
  Serial.println(targetBar + 1);
}

uint32_t transportTicksToMs(uint16_t ticks) {
  return max((uint32_t)1, (uint32_t)((float)ticks * tickDurationMs()));
}

#include "Transport40.h"

void serviceTransportMeasureRepeat() {transportRepeat40();return;
  if(controller940)return;
  if (!songEngineActive && menuNavigationOnly() && !modUi805 && !patchUi808 && !itemKind809) return;
  uint32_t now = millis();
  for (uint8_t side = 0; side < 2; side++) {
    uint8_t buttonIndex = side == 0 ? 4 : 5;
    if (!transportButtonStable[buttonIndex] || transportMeasureRepeatAtMs[side] == 0) continue;
    if ((int32_t)(now - transportMeasureRepeatAtMs[side]) < 0) continue;
    if(songEngineActive)songTransport(buttonIndex,true);
    else if (side == 0) transportPreviousMeasure();
    else transportNextMeasure();
    transportMeasureRepeatAtMs[side] = now + transportTicksToMs(transportRepeatRateTicks);
  }
}

void handleTransportButtonEdge(uint8_t buttonIndex, bool pressed) {
 if(buttonIndex<8&&pressed)presentationTouch43();
 if(pinTestActive33)return;
 if(controller940){if(pressed&&buttonIndex>=3&&buttonIndex<=5)controllerEmit940("TRAN",buttonIndex,1);return;}
  selectionIndicatorsPending931=true;
  if(buttonIndex==2&&!pressed)pidiStopEdge931(false);
  if(sdBusy905())return;
  TransportScope903 scope(buttonIndex);if(debugTestMode==DEBUG_TEST_NONE&&pressed&&buttonIndex!=0&&buttonIndex!=1&&buttonIndex!=2)programAll28();
  if(debugTestMode==DEBUG_TEST_NONE&&buttonIndex>=6){if(pressed&&stableTransport904)toggleTransport903(buttonIndex);return;}
  InputScope815 input(I815_HC,TRANSPORT_BUTTON_BITS[buttonIndex],pressed);
  if(pressed){alertAction08();inputActivity811=millis();}
  if (buttonIndex == 2 && pressed && debugTestMode!=DEBUG_TEST_CV47) autosave805Request();
  if (debugTestMode == DEBUG_TEST_TRANSPORT) {
    static const char* BUTTON_NAME[TRANSPORT_BUTTON_COUNT] = {
      "REC", "PLAY", "STOP", "RETURN", "MP", "MS", "DELAY", "NOTES"
    };
    char line0[21], line1[21], line2[21], small0[17], small1[17];
    snprintf(line0, sizeof(line0), "TRANS TEST %s", BUTTON_NAME[buttonIndex]);
    snprintf(line1, sizeof(line1), "HC165 BIT%u", TRANSPORT_BUTTON_BITS[buttonIndex]);
    snprintf(line2, sizeof(line2), "INDEX%u %s", buttonIndex, pressed ? "ON" : "OFF");
    snprintf(small0, sizeof(small0), "TRANS %s", BUTTON_NAME[buttonIndex]);
    snprintf(small1, sizeof(small1), "B%u %s", TRANSPORT_BUTTON_BITS[buttonIndex], pressed ? "ON" : "OFF");
    sendDebugDisplay(line0, line1, line2, "", small0, small1);
    Serial.print(F("DBG,TRANSPORT,NAME=")); Serial.print(BUTTON_NAME[buttonIndex]);
    Serial.print(F(",INDEX=")); Serial.print(buttonIndex);
    Serial.print(F(",HC165_BIT=")); Serial.print(TRANSPORT_BUTTON_BITS[buttonIndex]);
    Serial.print(F(",PRESSED=")); Serial.println(pressed ? 1 : 0);
    return;
  }
  if(debugTestMode==DEBUG_TEST_CV47){if(buttonIndex!=1&&buttonIndex!=2)return;}
  else if (debugTestMode != DEBUG_TEST_NONE) return;
  if(buttonIndex==2&&pressed)pidiStopEdge931(true);
  if(buttonIndex==2&&pressed){remixAudition911.active=false;remixFinishTake913();modStop811();}
  // Timestamp only: no polling and no wait in the transport loop.
  // Hold state reset at controller mode transitions.
  if(pressed && buttonIndex!=1)playHeld923=false;
  if(buttonIndex==1){
    if(pressed){playHeld923=playing;playAt923=millis();playThreshold923=(uint32_t)(120000.0f/bpm);}
    else {bool resume=playHeld923&&!playing&&(uint32_t)(millis()-playAt923)>playThreshold923;playHeld923=false;if(resume)pressed=true;}
  }
  if(buttonIndex==2&&pressed)synkozStop933();
  if(buttonIndex==2&&pressed){transportCancel40();pendingApply40();}
  if(buttonIndex==3&&pressed){transportCancel40();pendingApply40();}
  if(buttonIndex==3){
    if(pressed&&!returnHeld41){returnHeld41=true;returnWasPlaying41=playing;playing=internalClockRunning=false;allNotesOff();if(songEngineActive)songSeek((uint64_t)(songStartBar-1)*1536);else {setTransportPositionToBar((loopStartStep-1)/16);transportTickAbsolute=(uint64_t)(loopStartStep-1)*96;currentStep=loopStartStep-1;measureTick=localTickForTrack(selectedTrack);}}
    else if(!pressed&&returnHeld41){returnHeld41=false;playing=internalClockRunning=returnWasPlaying41;}
    pidiStateDirty931=true;return;
  }
  if(buttonIndex==4||buttonIndex==5){transportBrowse40(buttonIndex,pressed);return;}
  if(buttonIndex==0){recordButton46(pressed);return;}
  if (songTransport(buttonIndex, pressed)) return;
  if (menuNavigationOnly() && !modUi805 && !patchUi808 && !itemKind809 && buttonIndex!=2 && debugTestMode!=DEBUG_TEST_CV47) return;
  if (pressed && buttonIndex!=0 && !selectButtonActive && !shiftButtonActive) cancelStepSelection();
  switch (buttonIndex) {
    case 0: // REC : action directe a l'appui
      if (pressed) {
        recordToggle820();
        Serial.print(F("TRANSPORT: REC "));
        Serial.println((remixMode?remixRecord913:recordingArmed) ? F("ON") : F("OFF"));
      }
      break;

    case 1: // PLAY / PAUSE : reprise au tick exact
      if (pressed) {
        if(!playing)patch808Play();
        (pidiStateDirty931=true, playing = !playing);
        internalClockRunning = playing;
        noInterrupts();
        pendingClockTicks = 0;
        interrupts();
        if (!playing) allNotesOff();
        Serial.print(playing ? F("TRANSPORT: PLAY tick=") : F("TRANSPORT: PAUSE tick="));
        Serial.println(measureTick);
      }
      break;

    case 2: // STOP : action directe a l'appui
      if (pressed) {
        stopClipboardPreview(true, true);
        (pidiStateDirty931=true, playing = false);
        internalClockRunning = false;
        noInterrupts();
        pendingClockTicks = 0;
        interrupts();
        allNotesOff();
        transportMeasureWasPlaying[0] = transportMeasureWasPlaying[1] = false;
        transportMeasureRepeatAtMs[0] = transportMeasureRepeatAtMs[1] = 0;
        cycleCounter = 0;
        setTransportPositionToMeasureStart();
        Serial.println(F("TRANSPORT: STOP"));
      }
      break;

    case 3: // RETOUR : action directe a l'appui
      if (pressed) transportReturnToSequenceStart();
      break;

    case 4: // MP : conserve exactement l'etat PLAY/PAUSE precedent
      if (pressed) {
        transportMeasureWasPlaying[0] = playing;
        (pidiStateDirty931=true, playing = false);
        internalClockRunning = false;
        noInterrupts(); pendingClockTicks = 0; interrupts();
        allNotesOff();
        transportPreviousMeasure();
        transportMeasureRepeatAtMs[0] = millis() + transportTicksToMs(transportRepeatWaitTicks);
      } else {
        transportMeasureRepeatAtMs[0] = 0;
        (pidiStateDirty931=true, playing = transportMeasureWasPlaying[0]);
        internalClockRunning = playing;
        Serial.println(playing ? F("TRANSPORT: MP -> CONTINUE") : F("TRANSPORT: MP -> STAY PAUSED"));
      }
      break;

    case 5: // MS : conserve exactement l'etat PLAY/PAUSE precedent
      if (pressed) {
        transportMeasureWasPlaying[1] = playing;
        (pidiStateDirty931=true, playing = false);
        internalClockRunning = false;
        noInterrupts(); pendingClockTicks = 0; interrupts();
        allNotesOff();
        transportNextMeasure();
        transportMeasureRepeatAtMs[1] = millis() + transportTicksToMs(transportRepeatWaitTicks);
      } else {
        transportMeasureRepeatAtMs[1] = 0;
        (pidiStateDirty931=true, playing = transportMeasureWasPlaying[1]);
        internalClockRunning = playing;
        Serial.println(playing ? F("TRANSPORT: MS -> CONTINUE") : F("TRANSPORT: MS -> STAY PAUSED"));
      }
      break;


  }
}

void serviceTransportButtons() {
  uint32_t now = millis();
  if ((uint32_t)(now - lastHc165PollMs) < hc165PollMs) return;
  lastHc165PollMs = now;
  if(grooveEditMode&&!patternMode){bool active51=playing&&playbackGroove(selectedTrack,activePattern[selectedTrack])==editedGroove;static int sent51=-1;if(sent51!=(int)active51){sent51=active51;DISPLAY_SERIAL.printf("GACTIVE51,%u\n",active51);}}

  uint64_t state = readButtons904();
  const uint8_t navPhysical51[]={4,5,6,7,12,13,14,15};
  state|=0xff00000000000000ULL;for(uint8_t i=0;i<8;++i)if(!(navigationRaw49&(1u<<navPhysical51[i])))state&=~(1ULL<<(56+i));
  state=(state|pc831.hcMask)&~pc831.hcDown;
  if(assignOpen932&&!controller940){assignCapture932(state);return;}
  if(debugTestMode==DEBUG_TEST_NONE)state=mapButtons932(state);

  if(diagActive814){if(inputLastScan815){uint32_t gap=now-inputLastScan815;if(gap>inputMaxScan815)inputMaxScan815=gap;}inputLastScan815=now;}

  // Releve temporaire V5.19 : toutes les cartes de boutons restent utilisables,
  // et chaque transition stable est annoncee sur l'USB pour mapper les 8 touches ecran.
  for (uint8_t bit = 0; bit < HC165_BIT_COUNT; bit++) {
    bool rawPressed = (((state >> bit) & 1ULL) == 0ULL);
    if (!hc165ProbeInitialised) {
      hc165ProbeStable[bit] = rawPressed;
      hc165ProbeLastRaw[bit] = rawPressed;
      hc165ProbeRawChangedMs[bit] = now;
      continue;
    }

    if (rawPressed != hc165ProbeLastRaw[bit]) {
      inputLog815(I815_HC,bit,rawPressed,I815_RAW);
      hc165ProbeLastRaw[bit] = rawPressed;
      hc165ProbeRawChangedMs[bit] = now;
    }

    uint16_t probeStableMs = rawPressed ? hc165PressDebounceMs : hc165ReleaseDebounceMs;
    if (rawPressed != hc165ProbeStable[bit] &&
        (uint32_t)(now - hc165ProbeRawChangedMs[bit]) >= probeStableMs) {
      hc165ProbeStable[bit] = rawPressed;selectionIndicatorsPending931=true;
      inputLog815(I815_HC,bit,rawPressed,I815_STABLE);
      Serial.print(F("HC165,"));
      Serial.print(rawPressed ? F("PRESS,B") : F("RELEASE,B"));
      Serial.println(bit);
      if (debugTestMode == DEBUG_TEST_BUTTONS) {
        reportDebugButton(bit, rawPressed);
        continue;
      }
      if (buttonSettingsLiveTest()) {
        reportLiveButtonSetting(bit, rawPressed);
        continue;
      }
      if(bit==5&&!recordArmed823()&&!settingsOpen&&!controller940&&!songMode&&!remixMode&&!grooveEditMode){deleteEdge60(rawPressed);continue;}
      if (rawPressed) {
        dispatchStableButtonPress(bit);
      }
    }
  }
  hc165ProbeInitialised = true;

  if (screenUiAwaitRelease) {
    bool anyScreenButtonPressed = false;
    for (uint8_t i = 0; i < 8; i++) {
      if (hc165ProbeStable[SCREEN_BUTTON_BITS[i]]) {
        anyScreenButtonPressed = true;
        break;
      }
    }
    if (!anyScreenButtonPressed) screenUiAwaitRelease = false;
  }

  for (uint8_t i = 0; i < TRANSPORT_BUTTON_COUNT; i++) {
    if (editActionForBit(TRANSPORT_BUTTON_BITS[i]) >= 0) continue;
    bool rawPressed = (((state >> TRANSPORT_BUTTON_BITS[i]) & 1ULL) == 0ULL);

    if (!hc165Initialised) {
      transportButtonStable[i] = rawPressed;
      transportButtonLastRaw[i] = rawPressed;
      transportButtonRawChangedMs[i] = now;
      continue;
    }

    if (rawPressed != transportButtonLastRaw[i]) {
      transportButtonLastRaw[i] = rawPressed;
      transportButtonRawChangedMs[i] = now;
    }

    uint16_t requiredStableMs = rawPressed ? hc165PressDebounceMs : hc165ReleaseDebounceMs;
    if (rawPressed != transportButtonStable[i] &&
        (uint32_t)(now - transportButtonRawChangedMs[i]) >= requiredStableMs) {
      transportButtonStable[i] = rawPressed;
      handleTransportButtonEdge(i, rawPressed);
    }
  }

  for (uint8_t i = 0; i < CLIPBOARD_BUTTON_COUNT; i++) {
    bool rawPressed = (((state >> CLIPBOARD_BUTTON_BITS[i]) & 1ULL) == 0ULL);

    if (!hc165Initialised) {
      clipboardButtonStable[i] = rawPressed;
      clipboardButtonLastRaw[i] = rawPressed;
      clipboardButtonRawChangedMs[i] = now;
      continue;
    }

    if (rawPressed != clipboardButtonLastRaw[i]) {
      clipboardButtonLastRaw[i] = rawPressed;
      clipboardButtonRawChangedMs[i] = now;
    }

    uint16_t requiredStableMs = rawPressed ? hc165PressDebounceMs : hc165ReleaseDebounceMs;
    if (rawPressed != clipboardButtonStable[i] &&
        (uint32_t)(now - clipboardButtonRawChangedMs[i]) >= requiredStableMs) {
      clipboardButtonStable[i] = rawPressed;
      handleClipboardButtonEdge(i, rawPressed);
    }
  }

  hc165Initialised = true;
}

FLASHMEM void initTransportButtons() {
  pinMode(HC165_PIN_LATCH, OUTPUT);
  pinMode(HC165_PIN_CLOCK, OUTPUT);
  pinMode(HC165_PIN_DATA, INPUT);
  digitalWrite(HC165_PIN_LATCH, HIGH);
  digitalWrite(HC165_PIN_CLOCK, LOW);

  hc165Initialised = false;
  serviceTransportButtons();
  serviceTransportMeasureRepeat();
}

// ============================================================
// TRANSPORT MIDI
// ============================================================
void triggerStep(uint8_t track, uint16_t step) {
  // V4.80 : les valeurs sont capturees au moment exact du Note On.
  // Le Note Off est planifie tout de suite avec une duree figÃƒÆ’Ã†â€™Ãƒâ€ Ã¢â‚¬â„¢ÃƒÆ’Ã¢â‚¬Â ÃƒÂ¢Ã¢â€šÂ¬Ã¢â€žÂ¢ÃƒÆ’Ã†â€™ÃƒÂ¢Ã¢â€šÂ¬Ã‚Â ÃƒÆ’Ã‚Â¢ÃƒÂ¢Ã¢â‚¬Å¡Ã‚Â¬ÃƒÂ¢Ã¢â‚¬Å¾Ã‚Â¢ÃƒÆ’Ã†â€™Ãƒâ€ Ã¢â‚¬â„¢ÃƒÆ’Ã‚Â¢ÃƒÂ¢Ã¢â‚¬Å¡Ã‚Â¬Ãƒâ€¦Ã‚Â¡ÃƒÆ’Ã†â€™ÃƒÂ¢Ã¢â€šÂ¬Ã…Â¡ÃƒÆ’Ã¢â‚¬Å¡Ãƒâ€šÃ‚Â©e : une edition ulterieure
  // de Gate/Pitch/Vel/Time ne modifie pas cette note deja lancee.
  uint8_t count = baseVoiceCountAtStep(track, step);
  if (trackPlayMode[linkOwner12(track,LINK_POLY12)] == TRACK_PLAY_MONO && count > 1) count = 1;
  uint8_t emittedNotes[EXTRA_NOTES_PER_STEP + 1];
  uint8_t emittedCount = 0;
  for (uint8_t voice = 0; voice < count; voice++) {
    if (previewSuppressesVoice(track, step, voice)) continue;
    uint8_t note = audibleNoteAtVoice(track, step, voice);
    bool duplicate = false;
    for (uint8_t emitted = 0; emitted < emittedCount; emitted++) {
      if (emittedNotes[emitted] == note) {
        duplicate = true;
        break;
      }
    }
    if (duplicate) continue;
    emittedNotes[emittedCount++] = note;
    uint8_t vel = audibleVel(track, step, voice);
    uint32_t gateUs = (uint32_t)clampInt((int)((float)audibleGateTicks(track, step, voice) * tickDurationMs() * 1000.0f), 1000, 20000000);
    triggerTrackNoteWithFixedGate(track, note, vel, gateUs, 0,(int32_t)step*96+logicalMicroTick(track,step,voice),noteAtVoice(track,step,voice));
  }
}

void triggerFineEvent(uint8_t track, uint16_t step, uint8_t voice) {
  uint32_t epoch38=modEpoch805;
  if(blockIndex21()==0)ioNote38(track,pendingEditedValue(track,step,voice,EDIT_PITCH,noteAtVoice(track,step,voice)),pendingEditedValue(track,step,voice,EDIT_VEL,effectiveVel(track,step,voice)),pendingEditedValue(track,step,voice,EDIT_GATE,effectiveGateTicks(track,step,voice)),false);
  if(recordErasePitch820(track,noteAtVoice(track,step,voice))){removeVoiceAtStep(track,step,voice);mod805Invalidate();neoDisplayDirty=true;return;}
  if (previewSuppressesVoice(track, step, voice)) return;
  if(modEpoch805!=epoch38)mod808FlushMidi(songEngineActive?songTick:transportTickAbsolute,4);
  uint8_t vel = audibleVel(track, step, voice);
  uint32_t gateUs = (uint32_t)((float)audibleGateTicks(track, step, voice) * tickDurationMs() * 1000.0f);
  gateUs = (uint32_t)clampInt((int)gateUs, 1000, 20000000);
  triggerTrackNoteWithFixedGate(track, audibleNoteAtVoice(track, step, voice), vel, gateUs, 0,(int32_t)step*96+logicalMicroTick(track,step,voice),noteAtVoice(track,step,voice));
}

void internalClockISR() {
  if(internalClockRunning)clockEnqueue830(micros());
}

uint32_t internalClockPeriodUs() {
  float period = 60000000.0f / bpm / (float)PPQN;
  if (period < 50.0f) period = 50.0f;
  return (uint32_t)(period + 0.5f);
}

void updateInternalClockPeriod() {
  internalClockTimer.update(internalClockPeriodUs());
}

uint8_t rollVelocity(const TrackRollSettings &roll, uint16_t repeatIndex) {
  int value = (int)roll.level + (int)roll.curve * repeatIndex;
  return (uint8_t)clampInt(value, 1, 127);
}

bool writeUsesSelection() {
  return selectionHasActiveNotes() ||
         (selectedStep >= 0 && selectedVoice<noteCountAtStep(selectedTrack,(uint16_t)selectedStep));
}

bool writeTargetsVoice(uint8_t track, uint16_t step, uint8_t voice, bool selectedOnly) {
  if (!selectedOnly) return true;
  if (selectionHasActiveNotes()) return selectedNoteObject(track, step, voice);
  return track == selectedTrack && selectedStep == (int16_t)step && selectedVoice == voice;
}

void setVoiceTimeOffset(uint8_t track, uint16_t step, uint8_t voice, int16_t offset,
                        bool locked) {
  int16_t fine = fineIndexForVoice(track, step, voice);
  if (fine >= 0) {
    fineNote[track][fine].micro = offset;
    if (locked) fineNote[track][fine].pxMask |= PX_TIME;
    else fineNote[track][fine].pxMask &= (uint16_t)~PX_TIME;
  } else {
    stepMicroTick[track][step] = offset;
    stepLockTime[track][step] = locked;
  }
}

void disableVoiceRoll(uint8_t track, uint16_t step, uint8_t voice) {
  int16_t fine = fineIndexForVoice(track, step, voice);
  if (fine >= 0) {
    fineNote[track][fine].roll.level = 0;
    fineNote[track][fine].pxMask |= (uint16_t)(1u << (PX_ROLL_SHIFT + 5));
  } else {
    auto r906=rollRead906(track,step);r906.level=0;rollSet906(track,step,r906,stepRollLockMask[track][step]|(1u<<5));
  }
}

FLASHMEM uint16_t writeTimePass(uint8_t track, bool selectedOnly, int8_t wantedSign) {
  uint16_t moved = 0;
  static FineNoteEvent sourceEvent[MAX_NOTES_PER_STEP + FINE_EVENTS_PER_TRACK];
  static uint32_t destinationTick[MAX_NOTES_PER_STEP + FINE_EVENTS_PER_TRACK];
  static uint8_t sourceVoice[MAX_NOTES_PER_STEP + FINE_EVENTS_PER_TRACK];
  int first = wantedSign > 0 ? (int)trackStepCount(track) - 1 : 0;
  int end = wantedSign > 0 ? -1 : (int)trackStepCount(track);
  int direction = wantedSign > 0 ? -1 : 1;
  for (int stepIndex = first; stepIndex != end; stepIndex += direction) {
    uint16_t step = (uint16_t)stepIndex;
    uint8_t captured = 0;
    uint8_t voices = noteCountAtStep(track, step);
    for (uint8_t voice = 0; voice < voices; voice++) {
      if (!writeTargetsVoice(track, step, voice, selectedOnly)) {
        continue;
      }
      int16_t offset = effectiveMicroTick(track, step, voice);
      if ((wantedSign > 0 && offset <= 0) || (wantedSign < 0 && offset >= 0)) {
        continue;
      }
      if (captured >= MAX_NOTES_PER_STEP + FINE_EVENTS_PER_TRACK) break;
      FineNoteEvent event = {};
      if (!captureNoteRecord(track, step, voice, event)) continue;
      int32_t writtenTick = (int32_t)step * TICKS_PER_STEP +
                            logicalMicroTick(track, step, voice) + offset;
      if (writtenTick < 0 || writtenTick >= (int32_t)trackSequenceTicks(track)) {
        continue;
      }
      event.note = noteAtVoice(track, step, voice);
      event.vel = effectiveVel(track, step, voice);
      event.gate = effectiveGateTicks(track, step, voice);
      event.micro = 0;
      event.pxMask |= PX_TIME;
      sourceEvent[captured] = event;
      destinationTick[captured] = (uint32_t)writtenTick;
      sourceVoice[captured] = voice;
      captured++;
    }
    // 1B3 : retrait complet du lot source avant toute insertion. Une note
    // nouvellement posee ne peut donc jamais etre relue par cette passe.
    for (int index = (int)captured - 1; index >= 0; index--) {
      removeVoiceAtStep(track, step, sourceVoice[index]);
    }
    for (uint8_t index = 0; index < captured; index++) {
      placeClipboardEvent(track, destinationTick[index], sourceEvent[index]);
      if (selectedOnly) {
        uint16_t destinationStep;
        int16_t destinationMicro;
        tickToStepAndMicro(track, destinationTick[index], destinationStep, destinationMicro);
        if (destinationStep < STEP_COUNT) stepSelected[track][destinationStep] = true;
      }
      moved++;
    }
  }
  return moved;
}

#include "Move939.h"
#include "Delete23.h"
uint16_t moveSelectedNotesByTicks(uint8_t track, int32_t deltaTicks) {return moveCircular939(track,deltaTicks);}

FLASHMEM uint16_t writeTimeToComposition(uint8_t track, bool selectedOnly) {
  uint16_t changed = writeTimePass(track, selectedOnly, 1);
  changed += writeTimePass(track, selectedOnly, -1);

  // Les notes deja exactement sur leur position logique doivent elles aussi
  // cesser de suivre un TIM de piste lors d'un WRITE partiel.
  for (uint16_t step = 0; step < trackStepCount(track); step++) {
    for (uint8_t voice = 0; voice < noteCountAtStep(track, step); voice++) {
      if (!writeTargetsVoice(track, step, voice, selectedOnly)) continue;
      if (effectiveMicroTick(track, step, voice) == 0) {
        setVoiceTimeOffset(track, step, voice, 0, selectedOnly && defaultMicroTick[track] != 0);
      }
    }
  }

  if (!selectedOnly) {
    defaultMicroTick[track] = 0;
    for (uint16_t step = 0; step < trackStepCount(track); step++) {
      for (uint8_t voice = 0; voice < noteCountAtStep(track, step); voice++) {
        setVoiceTimeOffset(track, step, voice, 0, false);
      }
    }
  }
  return changed;
}

FLASHMEM uint16_t writeRollToComposition(uint8_t track, bool selectedOnly) {
  uint16_t created = 0;
  for (uint16_t step = 0; step < trackStepCount(track); step++) {
    uint8_t voice = 0;
    while (voice < noteCountAtStep(track, step)) {
      if (!writeTargetsVoice(track, step, voice, selectedOnly)) {
        voice++;
        continue;
      }
      TrackRollSettings roll = configuredRollSettings(track, step, voice);
      if (roll.start == 0 || (roll.level == 0 && !rollExtra1000(track,activePattern[track],(int32_t)step*96+logicalMicroTick(track,step,voice),noteAtVoice(track,step,voice)).v[1])) {
        voice++;
        continue;
      }
      FineNoteEvent source = {};
      if (!captureNoteRecord(track, step, voice, source)) {
        voice++;
        continue;
      }
      int32_t targetTick = (int32_t)step * TICKS_PER_STEP + audibleMicroTick(track, step, voice)-defaultMov41[track][0];
      source.note = noteAtVoice(track, step, voice);
      source.vel = effectiveVel(track, step, voice);
      source.gate = effectiveGateTicks(track, step, voice);
      disableVoiceRoll(track, step, voice);

      const int32_t origin38=targetTick;int32_t relativeTick = targetTick - roll.start;
      targetTick-=rollStop09(roll,rollExtra1000(track,activePattern[track],(int32_t)step*96+logicalMicroTick(track,step,voice),noteAtVoice(track,step,voice)));
      int32_t spacing = clampInt(roll.speed, 1, TICKS_PER_STEP);
      int32_t acceleration256 = (int32_t)roll.accel * 256;
      for(uint16_t repeat = 0; repeat < MAX_PREROLL_NOTES && relativeTick < targetTick; repeat++) {
        for(uint8_t side939=0;side939<2;++side939){if((side939==0&&rollDirection60(track,activePattern[track],lockTick817(track,step,voice),noteAtVoice(track,step,voice))==1)||(side939==1&&rollDirection60(track,activePattern[track],lockTick817(track,step,voice),noteAtVoice(track,step,voice))==0))continue;int32_t writeTick939=side939?2*origin38-relativeTick:relativeTick;
        if (writeTick939 >= 0 && writeTick939 < (int32_t)trackSequenceTicks(track)) {
          FineNoteEvent generated = source;
          auto x=rollExtra1000(track,activePattern[track],(int32_t)step*96+logicalMicroTick(track,step,voice),source.note);
          generated.vel=rollVelocity1000(roll,x,source.vel,repeat);generated.note=rollPitch1000(x,source.note,repeat);
          generated.gate = effectiveGateTicks(track, step, voice);
          generated.micro = 0;
          generated.roll.start = 0;generated.roll.level = 0;
          generated.pxMask |= PX_PITCH | PX_VEL | PX_GATE | PX_TIME |
                              (uint16_t)(1u << (PX_ROLL_SHIFT + 5));
          placeClipboardEvent(track, (uint32_t)writeTick939, generated);
          if (selectedOnly) {
            uint16_t destinationStep;
            int16_t destinationMicro;
            tickToStepAndMicro(track, (uint32_t)writeTick939, destinationStep, destinationMicro);
            if (destinationStep < STEP_COUNT) stepSelected[track][destinationStep] = true;
          }
          created++;
        }
        }
        if (spacing >= targetTick - relativeTick) break;
        relativeTick += spacing;
        rollAdvance60(spacing,acceleration256,roll.expo,repeat);
      }
      voice++;
    }
  }
  return created;
}

FLASHMEM void sendWriteMenu() {
 const char*names[7]={"TIM","MOV","ROL","DLY","ENV","LFO","PLX"};char text[7][6];const char*labels[8]={};
 for(uint8_t i=0;i<7;++i){snprintf(text[i],6,"%s%c",names[i],writeMask41&(1u<<i)?'+':'-');labels[i]=text[i];}
 char top[21],bottom[21];makeEightButtonLines(top,bottom,labels);sendMenuScreen("WRITE", "NEO: SELECT TRACKS",top,bottom);
}
#include "GrooveUi16.h"
FLASHMEM void openWriteMenu() {
 writeSelect46();
 leavePadModesA01(true);modRemove806=false;stopNoteAuditions();stopClipboardPreview(true,true);
 showGameScreen();modUi805=patchUi808=quickView903=0;patchBank808=grooveEditMode=patternMode=remixMode=false;delayMixer930=false;
 writeMenuActive=true;writeMask41=0;writeTracks41=1u<<selectedTrack;writeRollEnabled=writeTimeEnabled=false;
 sendWriteMenu();neoDisplayDirty=displayValueDirty=true;
}
#include "WriteDelay41.h"
#include "WriteAuto46.h"
FLASHMEM void applyWriteToSequence() {
 if(!writeMenuActive)return;
 if(!writeTracks41||!(writeMask41&127)){DISPLAY_SERIAL.println("MSG|SELECT TRACKS AND FIELDS");return;}
 if(!writePrepare46())return;
 if((writeMask41&8)&&(!writeDelayPrepare41()||!writeDelayRoom41())){DISPLAY_SERIAL.print("MSG|");DISPLAY_SERIAL.println(writeError41);writeEchoFree41();return;}
 writeCommit46();
 uint8_t previous=selectedTrack;uint32_t timed=0,moved=0,rolled=0;
 for(uint8_t t=0;t<16;++t)if(writeTracks41&(1u<<t)){
  selectedTrack=t;if(!writeSelection46){clearStepSelection();selectedStep=-1;}
  if(writeMask41&1)timed+=writeTimeToComposition(t,writeSelection46);
  if((writeMask41&2)&&defaultMov41[t][0]){
   allTrackNotesSelected=true;explicitNoteSelection=true;
   for(uint16_t st=0;st<trackStepCount(t);++st)for(uint8_t v=0;v<noteCountAtStep(t,st);++v)setNoteObjectSelected(t,st,v,true);
   uint16_t count=moveCircular939(t,defaultMov41[t][0]);
   if(count||defaultMov41[t][0]%(int32_t)trackSequenceTicks(t)==0){moved+=count;defaultMov41[t][0]=0;moveFinish41(true);}clearStepSelection();
  }
  if(writeMask41&4)rolled+=writeRollToComposition(t,writeSelection46);
 }
 uint16_t delayed=writeEchoCount41;
 if(writeMask41&8){if(!writeDelayRoom41(false)){DISPLAY_SERIAL.print("MSG|");DISPLAY_SERIAL.println(writeError41);writeEchoFree41();selectedTrack=previous;return;}writeDelayCommit41();}
 writeEchoFree41();
 selectedTrack=previous;clearStepSelection();selectedStep=-1;writeMenuActive=false;storageDirty931=true;
 invalidateStepTriggers();neoDisplayDirty=displayValueDirty=displayBpmDirty=true;showGameScreen();
 char result[64];snprintf(result,sizeof(result),"WRITE TIM %lu MOV %lu ROL %lu DLY %u",(unsigned long)timed,(unsigned long)moved,(unsigned long)rolled,delayed);DISPLAY_SERIAL.print("MSG|");DISPLAY_SERIAL.println(result);
}

void triggerRollNote(uint8_t track, uint16_t step, uint8_t voice, uint8_t velocity, uint16_t repeat=0, int32_t rollOffset44=0) {
  auto x=rollExtra1000(track,activePattern[track],(int32_t)step*96+logicalMicroTick(track,step,voice),noteAtVoice(track,step,voice));
  velocity=rollVelocity1000(audibleRollSettings(track,step,voice),x,audibleVel(track,step,voice),repeat);
  if(!velocity)return;
  if (pendingDeleteNote(track, step, voice)) return;
  uint32_t gateUs = (uint32_t)((float)audibleGateTicks(track, step, voice) * tickDurationMs() * 1000.0f);
  gateUs = (uint32_t)clampInt((int)gateUs, 1000, 20000000);
  rollEvent44(track,audibleRollSettings(track,step,voice),x,rollOffset44,(int32_t)step*96+logicalMicroTick(track,step,voice),audibleGateTicks(track,step,voice),rollDirection60(track,activePattern[track],lockTick817(track,step,voice),noteAtVoice(track,step,voice)));
  rollTrigger817=true;
  triggerTrackNoteWithFixedGate(track, rollPitch1000(x,audibleNoteAtVoice(track, step, voice),repeat), velocity, gateUs, 0,(int32_t)step*96+logicalMicroTick(track,step,voice),noteAtVoice(track,step,voice));
  rollTrigger817=false;rollHoldUs44=0;
}

void processPreRollAtCurrentTick(uint8_t track, uint16_t step, uint8_t voice,
                                 int32_t targetTick, uint32_t trackTick) {
  TrackRollSettings roll = audibleRollSettings(track, step, voice);
  if (roll.start == 0 || (roll.level == 0 && !rollExtra1000(track,activePattern[track],(int32_t)step*96+logicalMicroTick(track,step,voice),noteAtVoice(track,step,voice)).v[1])) return;

  const int32_t origin38=targetTick;int32_t relativeTick = targetTick - roll.start;
      targetTick-=rollStop09(roll,rollExtra1000(track,activePattern[track],(int32_t)step*96+logicalMicroTick(track,step,voice),noteAtVoice(track,step,voice)));
  int32_t spacing = clampInt(roll.speed, 1, TICKS_PER_STEP);
  int32_t acceleration256 = (int32_t)roll.accel * 256;
  for(uint16_t repeat = 0; repeat < MAX_PREROLL_NOTES && relativeTick < targetTick; repeat++) {
    int32_t wrappedTick = relativeTick;
    while (wrappedTick < 0) wrappedTick += trackSequenceTicks(track);
    while (wrappedTick >= (int32_t)trackSequenceTicks(track)) wrappedTick -= trackSequenceTicks(track);
    if (rollDirection60(track,activePattern[track],lockTick817(track,step,voice),noteAtVoice(track,step,voice))!=1&&(uint32_t)wrappedTick == trackTick && !bridgeSuppressWrapped(track, relativeTick))
      triggerRollNote(track, step, voice, rollVelocity(roll, repeat),repeat,relativeTick-origin38);
    int32_t post=2*origin38-relativeTick,postWrapped=((post%(int32_t)trackSequenceTicks(track))+(int32_t)trackSequenceTicks(track))%(int32_t)trackSequenceTicks(track);if(rollDirection60(track,activePattern[track],lockTick817(track,step,voice),noteAtVoice(track,step,voice))!=0&&(uint32_t)postWrapped==trackTick&&!bridgeSuppressWrapped(track,post))triggerRollNote(track,step,voice,rollVelocity(roll,repeat),repeat,post-origin38);

    if (spacing >= targetTick - relativeTick) break;
    relativeTick += spacing;
    rollAdvance60(spacing,acceleration256,roll.expo,repeat);
  }
}

#include "GrooveListPlayback.h"

FLASHMEM void runV714Fixtures(uint8_t t) {
#include "V714Checks.h"
}

DMAMEM uint8_t fineHead821[STEP_COUNT],fineNext821[FINE_EVENTS_PER_TRACK];
#include "Agenda822.h"
void processEventsAtCurrentTick() {

  for (uint8_t track = 0; track < TRACK_COUNT; track++) {
    profileTrack830=track;Scope830 measured(P830_TRACK);
    uint32_t trackTick = localTickForTrack(track);
    if(remixTrack911(track,trackTick))continue;
    if (!grooveBridgeReady[track] && pendingPattern[track] >= 0 && !selectButtonActive && !deleteButtonActive)
      prepareGrooveBridge(track, (uint8_t)pendingPattern[track]);
    if (grooveBridgeReady[track] && !grooveBridgeSwitched[track] &&
        (pendingPattern[track] < 0 || activePattern[track] != grooveBridgeSource[track])) clearGrooveBridge(track);
    if (grooveBridgeReady[track] && !grooveBridgeSwitched[track] && pendingPattern[track] >= 0) {
      if (deleteButtonActive || selectButtonActive) {
        // Editing takes priority. Do not run a prepared transition during an
        // uncommitted selection; give it a fresh complete preparation window.
        grooveBridgeAt[track] = nextBridgeBoundary(track, transportTickAbsolute + 4 * TICKS_PER_STEP);
        ++bridgeEpoch49[track];
      } else if (transportTickAbsolute >= grooveBridgeAt[track]) {
        uint8_t nextPattern = (uint8_t)pendingPattern[track];
        pendingPattern[track] = -1;
        switchTrackPattern(track, nextPattern, false);
        if (activePattern[track] == nextPattern) {
          grooveTrackPhase[track] = globalLoopedTick();
          grooveListOrigin[track] = transportTickAbsolute;
          grooveBridgeSwitched[track] = true;
          ++bridgeEpoch49[track];
          grooveBridgeFirstEnd[track] = nextBridgeBoundary(track, transportTickAbsolute);bridgeFirstCycleUntil51[track]=grooveBridgeFirstEnd[track];
          trackTick = localTickForTrack(track);
        } else clearGrooveBridge(track);
      }
    }
    if (!deleteButtonActive && !selectButtonActive) playGrooveBridgeEvents(track);
    lastTrackLocalTick[track] = trackTick;
    if(!trackSilent818(track))freePlayTrack11(track,trackTick);
    if (trackSilent818(track)||tickExpired830()) continue;
    blockProcessTrack21(track,trackTick);continue;
    if (effectiveGrooveAssignment(track, activePattern[track]) == GROOVE_USE_LIST) {
      processGrooveListTrack(track, trackTick);
      continue;
    }
    uint32_t trackLength = max((uint32_t)1, trackSequenceTicks(track));
    uint32_t globalLoopLength = max((uint32_t)1,
      (uint32_t)(loopEndStep - loopStartStep + 1) * TICKS_PER_STEP);
    uint32_t triggerCycleLength = min(trackLength, globalLoopLength);
    uint32_t trackCycle = (uint32_t)transportTickAbsolute;
    uint16_t stepCount821=trackStepCount(track);
    const uint32_t* due822=agendaDue822(track,trackTick);
    if(agendaNext822(due822,0,stepCount821)>=stepCount821)continue;
    memset(fineHead821,255,stepCount821);
    for(int i=FINE_EVENTS_PER_TRACK-1;i>=0;--i){const auto&e=fineNote[track][i];if(!e.active||e.step>=stepCount821)continue;fineNext821[i]=fineHead821[e.step];fineHead821[e.step]=i;}
    for (uint16_t step = agendaNext822(due822,0,stepCount821); step < stepCount821; step=agendaNext822(due822,step+1,stepCount821)) {
      if (stepActive[track][step]) {
      int32_t eventTick = (int32_t)step * TICKS_PER_STEP + audibleMicroTick(track, step, 0);
      processPreRollAtCurrentTick(track, step, 0, eventTick, trackTick);
      bool skipBase = bridgeSuppressWrapped(track, eventTick);
      while (eventTick < 0) eventTick += trackSequenceTicks(track);
      while (eventTick >= (int32_t)trackSequenceTicks(track)) eventTick -= trackSequenceTicks(track);
      if (!skipBase && (uint32_t)eventTick == trackTick && lastTriggeredCycle[track][step] != trackCycle) {
        lastTriggeredCycle[track][step] = trackCycle;
        triggerStep(track, step);
      }
      }

      uint8_t voice=baseVoiceCountAtStep(track,step);
      for(uint8_t fine=fineHead821[step];fine!=255;fine=fineNext821[fine],++voice){
        int32_t fineTick = (int32_t)step * TICKS_PER_STEP + audibleMicroTick(track, step, voice);
        processPreRollAtCurrentTick(track, step, voice, fineTick, trackTick);
        bool skipFine = bridgeSuppressWrapped(track, fineTick);
        while (fineTick < 0) fineTick += trackSequenceTicks(track);
        while (fineTick >= (int32_t)trackSequenceTicks(track)) fineTick -= trackSequenceTicks(track);
        if (!skipFine && (uint32_t)fineTick == trackTick && fineLastTriggeredCycle[track][fine] != trackCycle) {
          fineLastTriggeredCycle[track][fine] = trackCycle;
          triggerFineEvent(track, step, voice);
        }
      }
    }
  }
}

void processClockTick() {
  if(controller940)return;
  if(synkozClock933())return;
  if(playing)recordAdvance820();
  if (songEngineActive) { songClockTick(); return; }
  processClipboardPreviewTick();
  if (!playing) return;

  if (evaluateCurrentTickOnNextClock) {
    evaluateCurrentTickOnNextClock = false;
  } else {
    transportTickAbsolute++;
  }

  uint32_t masterLoopTick = globalLoopedTick();
  measureTick = localTickForTrack(selectedTrack);
  if ((masterLoopTick % ((uint32_t)STEPS_PER_BAR * TICKS_PER_STEP)) == 0) {
    patternBeatBlinkUntilMs = millis() + 120;
    if (patternMode) neoDisplayDirty = true;
    DISPLAY_SERIAL.println(F("BAR"));
  }

  uint16_t newStep = (uint16_t)(measureTick / TICKS_PER_STEP);
  if (newStep != currentStep) {
    currentStep = newStep;
    playbackStepCounter++;
    serviceDeferredRollAudio();
    neoDisplayDirty = true;
  }

  if (headFollowForTrack(selectedTrack)) {
    uint32_t span = windowTickSpan();
    if (measureTick < viewTickOffset || measureTick >= viewTickOffset + span) {
      viewTickOffset = (measureTick / span) * span;
      pendingViewSource = VIEW_SRC_HEAD;
      clampViewStepOffset();
      neoDisplayDirty = true;
      displayBpmDirty = true;
    }
  }

  recordErase820();
  processEventsAtCurrentTick();
}

void servicePatternBlink() {
  static uint32_t refresh55=0;if(patternMode&&(uint32_t)(millis()-refresh55)>=30){refresh55=millis();neoDisplayDirty=true;}
  static bool lastPulse805 = false;
  bool pulse805 = millis() % max(1UL, (uint32_t)(60000.0f / bpm)) < max(1UL, (uint32_t)(30000.0f / bpm));
  if (pulse805 != lastPulse805) { lastPulse805 = pulse805; if (patternMode && !songMode && lastPatternValid && !selectedPatternCount()) neoDisplayDirty = true; }
  if (patternBeatBlinkUntilMs == 0) return;
  if ((int32_t)(millis() - patternBeatBlinkUntilMs) < 0) return;
  patternBeatBlinkUntilMs = 0;
  if (patternMode) neoDisplayDirty = true;
}

void updateTransport() {
 if(pinTestActive33)return;
  if(transportBusy821)return;
  transportBusy821=true;Scope830 measured(P830_TRANSPORT);uint32_t started=micros();uint8_t processed=0;
  agendaBegin822();
  DiagScope814 diag(D814_TRANSPORT,pendingClockTicks);
  transportBacklog821=max(transportBacklog821,(uint32_t)pendingClockTicks);
  // Preserve every queued tick; yield between ticks so inputs and Note Off run.
  // One unusually expensive tick is measured, never silently skipped.
  while(processed<16){
    noInterrupts();bool ready=clockTake830();interrupts();
    if(!ready)break;
    clockContext830=true;uint32_t age=tickAge830();maxTickAge830=max(maxTickAge830,age);
    if(age>LATE_LIMIT830)++lateTicks830;
    uint32_t benchBegan29=benchActive29?micros():0;
    {Scope830 tick(P830_TICK);processClockTick();}
    if(benchActive29){benchTickSample29(micros()-benchBegan29);if(++benchTicks29>=benchTickLimit29)benchStop29();}
    clockContext830=false;profileTrack830=255;++processed;serviceNoteOffs();
    // Actual elapsed slice time, including agenda preparation. A single tick
    // can still exceed this budget; expired ticks take the state-only path.
    if((uint32_t)(micros()-started)>=2000)break;
  }
  agendaEnd822();
  if(processed)++transportSlices821;
  transportMax821=max(transportMax821,(uint32_t)(micros()-started));transportBusy821=false;
}

// ============================================================
// INIT
// ============================================================
FLASHMEM void initPattern() {
  memset(stepExtraMidiNote,0,sizeof(stepExtraMidiNote));
  for (uint8_t channel = 0; channel < 16; channel++) midiChannelPitchBendSent[channel] = INT16_MIN;
  for (uint8_t track = 0; track < TRACK_COUNT; track++) {
    memset(fineNote[track], 0, sizeof(fineNote[track]));
    for (uint8_t fine = 0; fine < FINE_EVENTS_PER_TRACK; fine++) fineLastTriggeredCycle[track][fine] = UINT32_MAX;
    activePattern[track] = 0;
    (pidiStateDirty931=true, trackMuted[track] = false);
    defaultMov41[track][0]=0;trackBars[track] = 1;
    trackLoopStartStep[track] = 1;
    trackLoopEndStep[track] = STEPS_PER_BAR;
    lastTrackLocalTick[track] = 0;
    pendingPattern[track] = -1;
    trackNotePreviewMode[track] = -1;
    trackNoteViewMode[linkOwner12(track,LINK_VIEW12)] = -1;
    for (uint8_t role = 0; role < NEO_COLOR_ROLE_COUNT; role++) {
      // Aucun tableau couleur par piste a initialiser.
    }
    (pidiRoutesDirty931|=1u<<(track), trackPlayMode[linkOwner12(track,LINK_POLY12)] = TRACK_PLAY_MULTI);(pidiRoutesDirty931|=1u<<(track), trackOut902[track]=1);
    for (uint8_t pattern = 0; pattern < PATTERN_COUNT; pattern++) patternBank[track][pattern].initialized = false;
    defaultVel[track] = 100;
    defaultMidiNote[track] = 36 + track;
    trackViewTickOffset[track] = 0;
    trackZoomLevel[track] = 0;
    trackActivePitchRow[track] = defaultMidiNote[track];
    defaultGateTicks[track] = TICKS_PER_STEP / 2; // 48 ticks par defaut
    defaultMicroTick[track] = 0;defaultMov41[track][0]=0;
    midiInRelease17(track);midiInChannel17[track]=0;
  (pidiRoutesDirty931|=1u<<(track), trackMidiChannel[track] = track);
    trackPitchBend[track] = 0;
    trackRoll[track].start = 24;
    trackRoll[track].speed = 24;
    trackRoll[track].accel = 0;
    trackRoll[track].expo = expDefault15[0];
    trackRoll[track].curve = 0;
    trackRoll[track].level = 0;

    for (uint16_t step = 0; step < STEP_COUNT; step++) {
      rollSet906(track,step,emptyRoll906,0);
      stepActive[track][step] = false;
      stepVel[track][step] = indNote50(track,1);
      stepMidiNote[track][step] = indNote50(track,0);
      stepExtraNoteCount[track][step] = 0;
      stepGateTicks[track][step] = indNote50(track,2);
      stepLogicalMicro[track][step] = 0;
      stepMicroTick[track][step] = indNote50(track,3);
      stepLockVel[track][step] = false;
      stepLockPitch[track][step] = false;
      stepLockGate[track][step] = false;
      stepLockTime[track][step] = false;
      stepSelected[track][step] = false;
      lastTriggeredCycle[track][step] = UINT32_MAX;
    }
  }

  for (uint16_t i = 0; i < MAX_ACTIVE_NOTES; i++) {
    activeNotes[i].active = false;
    activeNotes[i].preview = false;
    activeNotes[i].audition = false;
    activeNotes[i].track = 255;
    activeNotes[i].previewSlot = 255;
  }

  for (uint8_t i = 0; i < TRACK_COUNT; i++) {
    kyBtnDown[i] = false;
    kyBtnDownAtMs[i] = 0;
    kyBtnLastToggleMs[i] = 0;
    kyRotatedWhileDown[i] = false;
    kyHelpShown[i] = false;
  }

  for (uint8_t n = 0; n < NEO_COUNT; n++) {
    for (uint8_t pad = 0; pad < 16; pad++) {
      neoPressStartMs[n][pad] = 0;
      neoPadDown[n][pad] = false;
      neoPadEdited[n][pad] = false;
      neoPadCreatedOnPress[n][pad] = false;
    }
  }

  for (uint8_t slot = 0; slot < CLIPBOARD_COUNT; slot++) {
    memset(clipboardFine[slot], 0, sizeof(clipboardFine[slot]));
    clipboardFineCount[slot] = 0;
    releaseRemixClipboard915(slot);clipboardSlots[slot].valid = false;
    clipboardSlots[slot].type = CLIP_TYPE_NONE;
    clipboardSlots[slot].sourceTrack = 0;
    clipboardSlots[slot].startStep = 0;
    clipboardSlots[slot].lengthSteps = STEP_COUNT;
    clipboardSelectedTypes[slot] = CLIP_TYPE_SEQUENCE;
    clipboardPasteMode[slot] = CLIP_PASTE_ERASE;
    clipboardPasteDown[slot] = false;
    clipboardPasteHoldMenu[slot] = false;
    clipboardLivePreview[slot].active = false;
  }
  clipboardMenuActive = false;
  clipboardPasteMenuActive = false;
  clipboardPreviewActive = false;
  clearClipboardPreviewQueue();
  saveMenuActive = false;
  sequenceBars = trackBars[selectedTrack];
  loopStartStep = 1;
  loopEndStep = longestTrackStepCount();
  syncLegacyTrackLoopBounds();
  beatDraftBars = sequenceBars;
  chromaticBottomNote = defaultMidiNote[selectedTrack];

  // Demarrage volontairement vide : aucune note creee au boot.
}

FLASHMEM void initNeos() {
  DISPLAY_SERIAL.println(F("MODELEDS,1,1,1,1,1,1"));
  for (uint8_t n = 0; n < NEO_COUNT; n++) {
    if (n >= NEO_TEST_ACTIVE_COUNT) {
      neoAvailable[n] = false;
      Serial.print(F("Neo "));
      Serial.print(n + 1);
      Serial.println(F(" ignore pour le test presse-papiers"));
      continue;
    }
    Serial.print(F("Initialisation Neo "));
    Serial.print(n + 1);
    Serial.print(F(" adresse 0x"));
    Serial.print(NEO_ADDR[n], HEX);
    Serial.print(F(" ... "));

    bool ok = neo[n]->begin(NEO_ADDR[n]);
    neoAvailable[n] = ok;
    Serial.println(ok ? F("OK") : F("ERREUR"));

    if (ok) {
      neo[n]->pixels.setBrightness(NEO_GLOBAL_BRIGHTNESS);
      registerNeoCallbacks(n);
      for (uint8_t pad = 0; pad < 16; pad++) {
        neoPixel821(n,pad, colorFor(n, 0, 0, 0));
      }
      neo[n]->pixels.show();

    }
  }
  for (uint8_t n = 0; n < NEO_COUNT; ++n) if (neoAvailable[n]) {
    for (uint8_t p = 0; p < 16; ++p) neoPixel821(n,p, colorFor(n, 60, 60, 60));
    neo[n]->pixels.show();
  }
  delay(1000);
  for (uint8_t n = 0; n < NEO_COUNT; ++n) if (neoAvailable[n]) {
    for (uint8_t p = 0; p < 16; ++p) neoPixel821(n,p, 0);
    neo[n]->pixels.show();
  }
  updateRoySelectionIndicators();
  sendSelectionIndicatorsToPicran();
}

FLASHMEM void initRemoteDisplay() {
  DISPLAY_SERIAL.begin(DISPLAY_BAUD);
  delay(50);
  DISPLAY_SERIAL.print(F("HELLO,UrbanKaoz,"));
  DISPLAY_SERIAL.println(APP_BUILD_ID);
  displayValueDirty = true;
  displayBpmDirty = true;
  refreshOledValue(true);
  refreshOledBpm(true);
  showGameScreen();
}

FLASHMEM void initStorage() {
  sdAvailable = SD.begin(BUILTIN_SDCARD);
  storageWriteAvailable = false;
  if (!sdAvailable) {
    sdNeoNoticeActive = true;
    return;
  }
  SD.mkdir("/URBANKAOZ");
  SD.mkdir("/URBANKAOZ/NPX");
  SD.mkdir("/URBANKAOZ/SEQ");
  SD.mkdir("/URBANKAOZ/PAT");
  SD.mkdir("/URBANKAOZ/PRJ");

  const char* testPath = "/URBANKAOZ/.WRITE_TEST";
  SD.remove(testPath);
  File testFile = SD.open(testPath, FILE_WRITE);
  if (testFile) {
    const char* marker = "URBANKAOZ_OK";
    size_t written = testFile.print(marker);
    testFile.close();
    File verifyFile = SD.open(testPath, FILE_READ);
    char verify[13] = {};
    size_t readCount = verifyFile ? verifyFile.readBytes(verify, 12) : 0;
    if (verifyFile) verifyFile.close();
    storageWriteAvailable = written == 12 && readCount == 12 && strcmp(verify, marker) == 0;
  }
  SD.remove(testPath);
  sdNeoNoticeActive = !storageWriteAvailable;
}

void resetSessionPatternsEmpty() {
  resetGrooveBridges();
  selectedTrack = 0;
  selectedStep = -1;
  compactRowOffset = 0;
  for (uint8_t track = 0; track < TRACK_COUNT; track++) {
    activePattern[track] = 0;
    pendingPattern[track] = -1;
    defaultMov41[track][0]=0;trackBars[track] = 1;
    trackLoopStartStep[track] = 1;
    trackLoopEndStep[track] = STEPS_PER_BAR;
    lastTrackLocalTick[track] = 0;
    trackViewTickOffset[track] = 0;
    trackZoomLevel[track] = 0;
    trackActivePitchRow[track] = defaultMidiNote[track];
    for (uint8_t pattern = 0; pattern < PATTERN_COUNT; pattern++) {
      patternBank[track][pattern].initialized = false;
      patternBank[track][pattern].hasNotes = false;
    }
    for (uint16_t step = 0; step < STEP_COUNT; step++) {
      stepActive[track][step] = false;
      stepSelected[track][step] = false;
      stepVel[track][step] = indNote50(track,1);
      stepMidiNote[track][step] = indNote50(track,0);
      stepGateTicks[track][step] = indNote50(track,2);
      stepLogicalMicro[track][step] = 0;
      stepMicroTick[track][step] = indNote50(track,3);
      stepLockVel[track][step] = false;
      stepLockPitch[track][step] = false;
      stepLockGate[track][step] = false;
      stepLockTime[track][step] = false;
      rollSet906(track,step,emptyRoll906,0);
      lastTriggeredCycle[track][step] = UINT32_MAX;
    }
  }
  sequenceBars = 1;
  loopStartStep = 1;
  loopEndStep = STEPS_PER_BAR;
  syncLegacyTrackLoopBounds();
  beatDraftBars = 1;
  displayedPattern = 1;
  zoomLevel = 0;
  viewTickOffset = 0;
  chromaticBottomNote = defaultMidiNote[0];
  invalidateStepTriggers();
  neoDisplayDirty = true;
  displayValueDirty = true;
  displayBpmDirty = true;
}

FLASHMEM void loadDefaultProjectOrStartEmpty() {
  const char* defaultPath = "/URBANKAOZ/PRJ/default.UKP";
  char latest[96]={};const char*path=defaultPath;
  if(startupMode811==2){if(startupLatest811(latest,sizeof(latest)))path=latest;else path="";}
  bool loaded=false;
  if(startupMode811!=0&&sdAvailable&&path[0]&&SD.exists(path)){
    File probe=SD.open(path,FILE_READ);FullProjectHeader header{};bool read=probe&&probe.read((uint8_t*)&header,sizeof(header))==sizeof(header);probe.close();
    if(read&&header.magic==FULL_PROJECT_MAGIC&&header.version!=FULL_PROJECT_VERSION&&header.version!=33&&header.version!=34)Serial.printf("STARTUP811,INCOMPATIBLE,VERSION=%u,EXPECTED=%u\n",header.version,FULL_PROJECT_VERSION);
    loaded=loadFullProject(path);
  }
  if (loaded) {
    bpm = (float)settingsValue[6][2];
    updateInternalClockPeriod();
    Serial.print("STARTUP811,LOADED,");Serial.println(path);
    return;
  }
  internalPatternSessionFresh = true;
  resetSessionPatternsEmpty();
  bpm = (float)settingsValue[6][2];
  (pidiStateDirty931=true, playing = false);
  internalClockRunning = false;
  pendingClockTicks = 0;
  setTransportPositionToMeasureStart();
  updateInternalClockPeriod();
  Serial.printf("STARTUP811,EMPTY,REASON=%s\n",startupMode811==0?"MODE_EMPTY":!sdAvailable?"SD_UNAVAILABLE":!path[0]||!SD.exists(path)?"FILE_MISSING":"LOAD_REJECTED");
}

FLASHMEM void showStorageAlertIfNeeded() {
  // 15A1 - The existing startup hook now refreshes the Neo notice only.
  if (sdNeoNoticeActive) neoDisplayDirty = true;
}

void dismissSdNoticeAfterNoteCreation() {
  if (!sdNeoNoticeActive) return;
  sdNeoNoticeActive = false;
  neoDisplayDirty = true;
  lastNeoRefreshMs = millis() - NEO_MIN_REFRESH_MS;
}

bool sdNoticePixel(uint8_t neoIndex, uint8_t pad) {
  // Centered 3x4 letters on the two middle panels; the fourth column is blank.
  static const uint8_t letters[2][4] = {{7, 4, 3, 6}, {6, 5, 5, 6}};
  if (neoIndex < 1 || neoIndex > 2 || pad >= 16 || (pad % 4) >= 3) return false;
  return (letters[neoIndex - 1][pad / 4] & (1u << (2 - pad % 4))) != 0;
}

FLASHMEM void initRoy() {
  royOk = roy.begin(ROY_ADDR);
  if (royOk) {
    if (royPixels.begin(ROY_ADDR)) {
      // 8A6 - Test visuel au demarrage : chaque Roy doit s'allumer seul,
      // rouge, vert, bleu puis blanc, avant extinction complete.
      royPixels.setBrightness(35);
      for (uint8_t pixel = 0; pixel < 4; pixel++) royPixels.setPixelColor(pixel, 0);
      royPixels.show();
      const uint32_t colors[4] = {
        royPixels.Color(255, 0, 0), royPixels.Color(0, 255, 0),
        royPixels.Color(0, 0, 255), royPixels.Color(255, 255, 255)
      };
      for (uint8_t pixel = 0; pixel < 4; pixel++) royPixels.setPixelColor(pixel, colors[pixel]);
      royPixels.show();
      royPixelsReady = true;
      // Les Roy restent allumes pendant le test Neo, puis reprennent leur etat.
    }
    for (uint8_t e = 0; e < ROY_ENCODER_COUNT; e++) {
      roy.pinMode(ROY_SWITCH_PIN[e], INPUT_PULLUP);
      royEncoderPos[e] = roy.getEncoderPosition(e);
      roySwStableState[e] = roy.digitalRead(ROY_SWITCH_PIN[e]);
      roySwLastRawState[e] = roySwStableState[e];
      roySwLastRawChangeMs[e] = millis();
      roySwLastToggleMs[e] = 0;
    }
  }
}

FLASHMEM void printBootInfo() {
  Serial.println();
  Serial.print(F("=== UrbanKaoz_Tee_"));
  Serial.print(APP_BUILD_ID);
  Serial.println(F(" ==="));
  Serial.println(F("Boutons ecran : haut B23/B20/B22/B21, bas B16/B19/B17/B18"));
  Serial.println(F("Neo physique 1->4 : 0x2F / 0x2E / 0x32 / 0x30"));
  Serial.println(F("Pico ecran = Serial2 / Teensy pins 8 TX2, 7 RX2 optionnel"));
  Serial.print(F("Roy 0x49 -> "));
  Serial.println(royOk ? F("OK") : F("MISSING"));
  Serial.println(F("Pico1 = Serial1 / Teensy pins 0-1"));
  Serial.println(F("Pico2 = Serial6 / Teensy pins 25-24"));
  Serial.print(F("Nano P23 = Serial5 RX pin "));
  Serial.print(P23_RX_PIN_INFO);
  Serial.println(F(" (Y1 menu/home, Y2 Neo zoom)"));
  Serial.print(F("MIDI DIN OUT = "));
  Serial.print(F("Serial8 TX pin "));
  Serial.println(MIDI_TX_PIN_INFO);
  Serial.println(F("Track 1..16 = MIDI CH configurable, defaut CH1..16"));
  Serial.println(F("74HC165 transport : A1=B27 REC, A2=B24 PLAY/PAUSE, A3=B29 STOP, A4=B31 NORMAL"));
  Serial.println(F("74HC165 transport : B1=B26 RETOUR, B2=B25 MP, B3=B28 MS, B4=B30 REMIX"));
  Serial.println(F("74HC165 presse-papiers : B32..B47, 4 lignes COPY/SELECT/PASTE/PREVIEW"));
  Serial.print(F("Carte SD sauvegardes NPX/SEQ/PAT : "));
  if (!sdAvailable) Serial.println(F("ABSENTE"));
  else Serial.println(storageWriteAvailable ? F("WRITE OK") : F("WRITE ERROR"));
  Serial.println(F("Neo = vue COMPACTE par defaut, CHROMATIQUE disponible global/piste"));
  Serial.println(F("Fond blanc piste selectionnee supprime"));
  Serial.println(F("KY1..4 = TRK CHA NOT VEL; KY5..8 = GAT TIM PBN BPM"));
  Serial.println(F("KY9..12 = RST RSP RAC REX; KY13..16 = RCV RLV VST VTR"));
  Serial.println(F("Clic PBN = fin / 10 / 100"));
  Serial.println(F("Clic TIM = TIM/MOV; clic GAT = MUS/24t/1t; aucun x10 global"));
  Serial.println(F("KY edit = valeurs par defaut de la piste selectionnee"));
  Serial.println(F("Selection + KY = brouillon; K3 valide, J1 annule"));
  Serial.println(F("Preview note = DEFAULT/ON/OFF par piste"));
  Serial.println(F("Mode stabilite : DEBUG_SERIAL=false, refresh Neo limite, affichage delegue Serial2"));
  Serial.println(F("Timing : PPQN=384, 1 step=96 ticks, microtiming -48/+48"));
  Serial.println(F("TEST CLOCK : IntervalTimer interne -> processClockTick(), sans calcul flottant micros()"));
  Serial.println(F("Snapshot mesure retire : Note Off fige au declenchement"));
  Serial.println(F("Les valeurs par defaut ne sont plus recopies dans les steps a chaque cran"));
  Serial.println(F("Notes : Note On + Note Off planifies ensemble, gate fige au declenchement"));
  Serial.println(F("Selection piste/pas uniquement par appui Neo"));
  Serial.println(F("Toutes les rotations KY1..KY16 ont une affectation directe fixe"));
  Serial.println(F("Maintien note existante = selection; maintien vide + autre pad = plage vide"));
  Serial.println(F("TEST TEMPORAIRE : A4=NORMAL, B4=REMIX, Neo1 pads 1..16 = sauts mesure 1"));
  Serial.println(F("Step court vide = cree au relachement"));
  Serial.println(F("Step court actif = supprime + LED OFF"));
  Serial.println(F("Mode selection : appui step = toggle selection, maintien + autre step = plage"));
  Serial.println(F("Roy1 piste, Roy2 position/longueur, Roy3 boucle, Roy4 pattern"));
  Serial.println(F("Digipad A/B/C/D reserves; UNDO/REDO sur boutons physiques"));
  Serial.println(F("Diagnostic vue USB : VIEWLOG / VIEWCLR"));
  Serial.println(F("Une note neuve ne peut pas etre selectionnee par son maintien initial"));
  Serial.println(F("Edition selection = un seul snapshot UNDO lors de la validation"));
  Serial.println(F("MAIN ACCESS = NOTHING / PAUSE / STOP"));
  Serial.println(F("BPM de demarrage charge depuis SETTINGS > SEQ > TRNS > BPM (defaut 115)"));
}

#include "SongEngine.h"
#include "Selection805.h"
#include "Pattern805.h"
#include "Clipboard15.h"
#include "Mod806Engine.h"
#include "Patch808Model.h"
#include "Patch808Ui.h"
#include "Midi810.h"
#include "Mod810.h"
#include "Mod810Ui.h"
#include "Storage809.h"
#include "ModPreset07.h"
#include "PatchLibrary10.h"
#include "PatchList819.h"
#include "Name809.h"
#include "NameKeyboardR8.h"
#include "Names12.h"
#include "Names07.h"
#include "PLocks817.h"
#include "FreeLocks11.h"
#include "Roll1000.h"
#include "Mixer938.h"
#include "Track818.h"
#include "Delivery905.h"
#include "PatchSeed818.h"
#include "Factory07.h"
#include "Program939.h"
#include "Full805.h"
#include "Settings811.h"
#include "Playback828.h"
#include "Settings816.h"
#include "Checks816.h"
#include "Checks805.h"
#include "Mod806Checks.h"
#include "Checks808.h"
#include "Checks809.h"
#include "Checks810.h"
#include "Checks811.h"
#include "Keyboard823.h"
#include "Pidi827.h"
#include "PatchRecall54.h"

#include "PinTest33.h"
#include "DelayLabel07.h"
#include "Record820.h"
#include "KeyboardR10.h"
#include "ModesA01.h"
#include "Mixer930.h"
#include "Checks820.h"
#include "Checks823.h"
#include "Checks830.h"
#include "Checks902.h"
#include "Quick903.h"
#include "Checks903.h"
#include "Stability904.h"
#include "Ui910.h"
#include "Remix911.h"
#include "Remix913.h"
#include "RemixClipboard915.h"
#include "RemixStorage911.h"
#include "RemixChecks911.h"
#include "RemixChecks913.h"
#include "Checks915.h"
#include "Checks916.h"

#include "Evolution932.h"
#include "Ui60.h"
#include "PatchMetadata37.h"
#include "Io38.h"
#include "CvJoy46.h"
#include "CvDebug47.h"
#include "Automation46.h"
#include "Ind50.h"
#include "AutoClip46.h"
#include "Evolution933.h"
#include "FxUi32.h"
#include "GrooveStorage32.h"

FLASHMEM void setup() {
  midiInInit17();
  kbResetR10();loadPrefs932();loadPrefs933();kbPrefsLoadR10();
  // EXTMEM storage is NOLOAD: never interpret a previous boot's pointer.
  for(auto&clip:clipboardSlots)clip.remix=nullptr;
  memset(recordPads820,0,sizeof(recordPads820));
  keyboardLightsClear823();
  memset(grooveLists, 0, sizeof(grooveLists));
  memset(grooveOffset, 0, sizeof(grooveOffset));grooveDefaults16();oovLoad16();
  resetGrooveBridges();
  Serial.begin(115200);

  PICO1_SERIAL.addMemoryForRead(pico1RxExtra, sizeof(pico1RxExtra));
  PICO2_SERIAL.addMemoryForRead(pico2RxExtra, sizeof(pico2RxExtra));
  PICO1_SERIAL.begin(PICO_BAUD);
  PICO2_SERIAL.begin(PICO_BAUD);
  P23_SERIAL.addMemoryForRead(p23RxExtra811,sizeof(p23RxExtra811));
  P23_SERIAL.begin(P23_BAUD);
  pidiInit827();

  Wire.begin();
  Wire.setClock(400000);

  settingsLoad811();
  mod805Init();patch808Init();
  initPattern();
  if (!loadSettingsFromEEPROM()) saveSettingsToEEPROM();
  screenMapLoad();screenMapSend();sleepLoad43();patchRecallLoad54();
  load902();loadTransport903();loadStability904();
  settingsLoad816();lockSettingsLoad817();
  internalClockRunning = playing;
  internalClockTimer.begin(internalClockISR, internalClockPeriodUs());
  initTransportButtons();
  initRemoteDisplay();
  initStorage();
  patchCatalogScan10();
  full805RecoverFiles();
  loadDelayDefaults932();
  loadDefaultProjectOrStartEmpty();applyDelayDefaults932();
  // Le diagnostic TRK2 de V7 est termine : aucun doublage n'a ete observe.
  // Revenir au comportement nominal sans pretendre qu'un correctif cible de
  // doublage a ete applique.
  selectedTrack = 0;
  viewStartTrack = selectedTrack;
  measureTick = localTickForTrack(selectedTrack);
  chromaticBottomNote = defaultMidiNote[selectedTrack];
  initRoy();
  selection805Reset();
  initNeos();
  // Reprendre les broches seulement apres que toutes les bibliotheques et tous
  // les peripheriques ont termine leur initialisation. Le sketch HC165 seul
  // fonctionne avec exactement ces niveaux et ces temporisations.
  initTransportButtons();
  showStorageAlertIfNeeded();
  sendPicoDefaults();
  sendSelectionIndicatorsToPicran();
  updateNeoPixels();
  printBootInfo();
  songInit();
  resetStats830();
  // Peripheral startup screens may latch navigation-only. Always finish at home,
  // including when no SD/default project is available.
  showGameScreen();
  // Factory catalogue is installed once by patchSeed818; deleted files stay deleted.
  if(stableQuick904)quickChoose903(1);
  // VA01.16: the patch library is read-only at boot; no factory installation/move.
  autosave807Baseline();
  autosaveReady805 = true;
}

// ROY / V9.32: Never add periodic state scans, refresh retries, resends or
// automatic resynchronization as "safety". Explicit user authorization is
// required for each targeted recovery. Keep Note Off and nonblocking queues.
void loop() {
  uiService60();
 if(!playing&&!sdBusy905())compactIdle59();
 pinTestService33();
  service932();synkozService933();
  pidiService827();
  remotePrepare54(songEngineActive?songTick:transportTickAbsolute);
  if(memoryFault906){memoryFault906=false;DISPLAY_SERIAL.println("MSG|ROLL MEMORY FULL");Serial.println("MEM906,ROLL_ALLOCATION_FAILED");}
  diagService814();
  display821.service();
  DiagScope814 diagLoop(D814_LOOP,pendingClockTicks);
  if(diagActive814&&diagQueuedTrack814>=0){
    uint8_t track=diagQueuedTrack814;diagQueuedTrack814=-1;
    if(!menuNavigationOnly()&&!patternMode&&!songMode)selectTrackWithPreview814(track);
    else Serial.println("DIAG814,TRK,REJECTED_CONTEXT");
  }
  perfBegin812((playing?1:0)|(modUi805?2:0)|(patchUi808?4:0)|(patternMode?8:0)|(songMode?16:0));
  // Priorite absolue aux deux flux KY40 avant les traitements I2C, SD et UI.
  PERF812(1,servicePicos());
  PERF812(2,serviceP23()); // Drain joystick edges before slower display/I2C work.
  pcService831();
  PERF812(3,serviceUsbControl());
  PERF812(4,songService());
  PERF812(5,serviceSelectionIndicators());

  PERF812(6,serviceTransportButtons());pidiStopService931();
  keyboardService823();
  PERF812(7,serviceClipboardUi());
  PERF812(8,updateTransport());
  PERF812(9,serviceNoteOffs());
  followTrack817();
  PERF812(10,servicePatternBlink());
  if (activeLineWhiteUntilMs != 0 && (int32_t)(millis() - activeLineWhiteUntilMs) >= 0) {
    activeLineWhiteUntilMs = 0;
    neoDisplayDirty = true;
  }

  // Deuxieme vidage apres les traitements potentiellement bloquants.
  PERF812(11,servicePicos());
  PERF812(12,serviceKyButtonHelp());
  PERF812(13,serviceP23());
  PERF812(14,serviceP23Debug());
  PERF812(15,serviceDisplayInput());
  PERF812(16,serviceNeoButtons());
  // Bound musical latency between the two measured blocking I2C input batches.
  updateTransport();serviceNoteOffs();pidiDrain827();
  PERF812(17,serviceNeoSelectionHolds());
  PERF812(18,handleRoy());sleepService43();
  updateTransport();serviceNoteOffs();pidiDrain827();
  // Settings are sent once at the initiating event.
  serviceMenuScroll905();
  PERF812(20,serviceViewAudit());

  updateTransport();
  static uint32_t measure08=UINT32_MAX;uint32_t bar08=transportTickAbsolute/(96u*16u);if(bar08!=measure08){measure08=bar08;displayValueDirty=true;}
  static int sdShown08=-1;int sdNow08=sdAvailable?1:0;if(sdShown08!=sdNow08){sdShown08=sdNow08;DISPLAY_SERIAL.printf("SD08,%u\n",sdNow08);}
  PERF812(21,updateNeoPixels());
  PERF812(22,refreshOledValue());
  PERF812(23,refreshOledBpm());
  grooveNoticeService26();
  deleteService55();quickHoldService55();recordService46();updateTransport();serviceNoteOffs();pidiDrain827();
  PERF812(24,mod805Service());updateTransport();serviceNoteOffs();pidiDrain827();
  PERF812(25,patch808Service());PERF812(26,realtimeService810());
  updateTransport();serviceNoteOffs();pidiDrain827();PERF812(27,nameSync809());
  // VA01.16: autosave removed; no background project writes.
  sdService905();recoveryService08();
  remixService911();
  remixDisplay911();
  display821.service();
  perfEnd812();
}
#include "Checks817.h"

#include "Checks818.h"
#include "Checks819.h"

#include "Numeric917.h"
#include "DeleteLocks15.h"
#include "LockHold26.h"
#include "Delete55.h"

#include "Checks917.h"
#include "Pc831.h"
#include "Sd905.h"
#include "TrackLink12.h"
#include "TrackPreset12.h"
#include "Storage18.h"
#include "Content18.h"
#include "ObjectCleanup45.h"
#include "MidiIn17.h"
#include "Recovery08.h"
#include "Checks18.h"
#include "Checks905.h"
#include "Checks906.h"







#include "Checks15.h"


#include "Blocks21.h"
#include "ModRouting43.h"
#include "Remote54.h"
#include "DelayTargetsUi55.h"
#include "AutoPlayback46.h"
#include "AutoEdit46.h"





#include "Checks42.h"



#include "TrackInit57.h"


