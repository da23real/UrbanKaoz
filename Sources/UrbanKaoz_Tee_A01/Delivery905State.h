#pragma once
static bool saveCollision905=false,saveOverwrite905=false;
static char saveTarget905[64]={},saveWork905[64]={};
void toggleTrackView905();
void renderSaveCollision905();
bool prepareSave905();
bool finishSave905(bool ok,char*path,size_t size);
void saveCollisionButton905(uint8_t index);
bool sdCommand905(char*line);
bool sdBusy905();
void sdService905();
void checks905();
