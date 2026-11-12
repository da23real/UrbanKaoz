#pragma once
uint8_t startupMode811=1;
bool autosaveEnabled811=false;
uint16_t autosaveMinutes811=5;
uint32_t inputActivity811=0;
void settingsLoad811();void settingsSave811();
void settingsOpen811(bool startup);void settingsRender811();
bool settingsRoy811(uint8_t roy,int32_t delta,bool click);
bool startupLatest811(char*path,size_t size);
bool patchAssigned811(uint32_t id);
void checks811();

extern uint16_t configDraft811[2];
