#pragma once
bool contentSave18(uint8_t,uint8_t,bool,char*,size_t);
bool contentSaveClip18(uint8_t,char*,size_t);
bool contentLoadPattern18(const char*);
bool contentLoadClip18(const char*,uint8_t);
bool contentPaste18(uint8_t);
void contentClearClip18(uint8_t);
void contentCaptureClip18(uint8_t,uint8_t,uint16_t);
bool contentPreview18(const char*);
bool contentPreviewTick18(uint8_t,uint8_t,uint32_t,uint32_t);
void contentPreviewClear18();
uint8_t contentClipKind18(uint8_t);
bool contentEffect18(uint8_t);

void startPatternPreview36(uint8_t);
void checksStorage18();
