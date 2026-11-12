#pragma once
bool full805Restored = false, autosaveReady805 = false, autosavePending805 = false;
bool autosaveBusy805 = false;
char full805ForcedPath[96] = {};
uint32_t full805Generation = 0;
bool full805Append(File &file);
bool full805Read(File &file,bool verified=false);
bool full805Validate(const char *path);
void autosave805Request();
void autosave805Service();
void full805RecoverFiles();
void autosave807Baseline();
void newProject807();
void openNewProject807();

