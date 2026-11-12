#pragma once
// Explicit project transfer. This file never writes to the SD card.
class RamFile08:public FileImpl {
 uint8_t*data=nullptr;uint32_t used=0,at=0,capacity=0;bool opened=true;
 public:
 ~RamFile08(){if(data)extmem_free(data);}
 size_t read(void*b,size_t n)override{n=min(n,(size_t)(used-at));memcpy(b,data+at,n);at+=n;return n;}
 size_t write(const void*b,size_t n)override{
  if(n>8388608u-at)return 0;uint32_t end=at+n;
  if(end>capacity){uint32_t cap=min(8388608u,max(end,max(65536u,capacity*2)));auto*p=(uint8_t*)extmem_realloc(data,cap);if(!p)return 0;data=p;capacity=cap;}
  memcpy(data+at,b,n);at=end;used=max(used,at);return n;
 }
 int available()override{return used-at;}int peek()override{return at<used?data[at]:-1;}
 void flush()override{}bool truncate(uint64_t n=0)override{if(n>used)return false;used=n;at=min(at,used);return true;}
 bool seek(uint64_t pos,int mode)override{uint64_t next=mode==SeekCur?at+pos:mode==SeekEnd?used+pos:pos;if(next>used)return false;at=next;return true;}
 uint64_t position()override{return at;}uint64_t size()override{return used;}void close()override{opened=false;}
 bool isOpen()override{return opened;}const char*name()override{return "PC recovery";}bool isDirectory()override{return false;}
 File openNextFile(uint8_t=0)override{return File();}void rewindDirectory()override{}
};
static File recoveryFile08;
static uint32_t recoverySeen08=0,recoveryId08=0,recoveryHash08=0;
static char recoveryReply08[600]={};
static bool recoveryUploading08=false;
static PackedPattern906* recoveryPatterns08[16][16]={};
static StoredTrackPattern recoveryMetadata08[16][16]={};
FLASHMEM void recoveryClearStages08(){for(auto&row:recoveryPatterns08)for(auto*&p:row){if(p)extmem_free(p);p=nullptr;}}
FLASHMEM bool recoveryStage08(File f){
 recoveryClearStages08();memset(recoveryMetadata08,0,sizeof(recoveryMetadata08));f.seek(sizeof(FullProjectHeader));
 for(uint8_t t=0;t<16;++t)for(uint8_t p=0;p<16;++p){uint32_t bytes=0,magic=0;uint16_t bars=0;uint8_t notes=0;
  if(f.read((uint8_t*)&bytes,4)!=4)goto fail;
  if(!bytes)continue;
  if(f.read((uint8_t*)&magic,4)!=4||f.read((uint8_t*)&bars,2)!=2||(magic>=0x554B4232&&f.read(&notes,1)!=1)||!readPatternPayload906(f,magic,bars,patternWork906))goto fail;
  bool has=false;for(bool a:patternWork906.active)has|=a;for(auto&e:patternWork906.fine)has|=e.active;
  recoveryMetadata08[t][p]={true,bars,has};
  recoveryPatterns08[t][p]=packPattern906(patternWork906);if(!recoveryPatterns08[t][p])goto fail;
 }return true;
 fail:recoveryClearStages08();return false;
}
FLASHMEM void recoveryInstall08(){for(uint8_t t=0;t<16;++t)for(uint8_t p=0;p<16;++p){auto*v=recoveryPatterns08[t][p];if(!v)continue;installPattern906(t,p,v);recoveryPatterns08[t][p]=nullptr;patternDirty805[t][p]=true;patternBank[t][p]=recoveryMetadata08[t][p];}}
FLASHMEM bool recoveryWritePattern08(File&f,const PatternImage805&i){
 uint32_t size=7+sizeof(i.active)+sizeof(i.vel)+sizeof(i.note)+sizeof(i.gate)+sizeof(i.micro)+sizeof(i.lockVel)+sizeof(i.lockPitch)+sizeof(i.lockGate)+sizeof(i.lockTime)+sizeof(i.roll)+sizeof(i.rollMask)+sizeof(i.extraCount)+sizeof(i.extraNote)+sizeof(i.logical)+sizeof(i.fine)+sizeof(i.extra14)+sizeof(i.mov),magic=0x554B4236;uint8_t notes=1;
 bool ok=f.write((uint8_t*)&size,4)==4&&f.write((uint8_t*)&magic,4)==4&&f.write((uint8_t*)&i.bars,2)==2&&f.write(&notes,1)==1;
 #define REC08(field) ok=ok&&f.write((const uint8_t*)i.field,sizeof(i.field))==sizeof(i.field);
 REC08(active) REC08(vel) REC08(note) REC08(gate) REC08(micro) REC08(lockVel) REC08(lockPitch) REC08(lockGate) REC08(lockTime) REC08(roll) REC08(rollMask) REC08(extraCount) REC08(extraNote) REC08(logical) REC08(fine) REC08(extra14) REC08(mov)
 #undef REC08
 return ok;
}
FLASHMEM bool recoveryCapture08(){
 if(!internalPatternSessionFresh){File directory=SD.open("/URBANKAOZ/PAT");if(!sdAvailable||!directory||!directory.isDirectory())return false;}
 recoveryFile08.close();recoveryUploading08=false;recoveryFile08=File(new RamFile08);
 FullProjectHeader h;fillFullProjectHeader(h);bool ok=recoveryFile08.write((uint8_t*)&h,sizeof(h))==sizeof(h);
 for(uint8_t t=0;t<16&&ok;++t)for(uint8_t p=0;p<16&&ok;++p){
  if(p==activePattern[t]){pattern805CaptureLive(t,patternWork906);ok=recoveryWritePattern08(recoveryFile08,patternWork906);}
  else if(patterns906[t][p]){ok=unpackPattern906(patterns906[t][p],patternWork906)&&recoveryWritePattern08(recoveryFile08,patternWork906);}
  else if(patternBank[t][p].initialized||(!internalPatternSessionFresh&&sdAvailable&&patternHasNotes(t,p))){ok=sdAvailable&&pattern805Read(t,p,patternWork906)&&recoveryWritePattern08(recoveryFile08,patternWork906);}
  else {uint32_t zero=0;ok=recoveryFile08.write((uint8_t*)&zero,4)==4;}
 }
 ok=ok&&recoveryFile08.write((uint8_t*)grooveOffset,sizeof(grooveOffset))==sizeof(grooveOffset)&&recoveryFile08.write((uint8_t*)grooveShape16,sizeof(grooveShape16))==sizeof(grooveShape16)&&recoveryFile08.write((uint8_t*)patternGroove,sizeof(patternGroove))==sizeof(patternGroove)&&recoveryFile08.write((uint8_t*)grooveLists,sizeof(grooveLists))==sizeof(grooveLists)&&full805Append(recoveryFile08);
 if(ok){recoveryFile08.seek(0);ok=full805ValidateFile08(recoveryFile08);}
 if(!ok)recoveryFile08.close();return ok;
}
FLASHMEM bool recoveryText15(File&f){
 f.seek(0);String header=f.readStringUntil('\n');header.trim();f.seek(0);
 if(header=="URBANKAOZ_PAT_V6")return applyPatternFile("",&f);
 if(header!="URBANKAOZ_SEQ_V3"||!loadFileIntoClipboard("",loadPreviewBuffer,loadPreviewFine,&loadPreviewFineCount,&f)||!loadPreviewBuffer.valid)return false;
 uint8_t t=selectedTrack;auto&c=loadPreviewBuffer;clearTrackPatternData(t);lockClearPattern817(t,activePattern[t]);
 defaultMidiNote[t]=c.defaultNote;defaultVel[t]=c.defaultVel;defaultGateTicks[t]=c.defaultGateTicks;defaultMicroTick[t]=c.defaultMicroTick;
 for(uint16_t s=0;s<STEP_COUNT;s++){stepActive[t][s]=c.active[s];stepMidiNote[t][s]=c.note[s];stepVel[t][s]=c.vel[s];stepGateTicks[t][s]=c.gateTicks[s];stepMicroTick[t][s]=c.microTick[s];stepLockPitch[t][s]=c.lockPitch[s];stepLockVel[t][s]=c.lockVel[s];stepLockGate[t][s]=c.lockGate[s];stepLockTime[t][s]=c.lockTime[s];}
 memcpy(fineNote[t],loadPreviewFine,sizeof(fineNote[t]));trackBars[t]=max((uint16_t)1,(uint16_t)((c.lengthSteps+15)/16));trackLoopStartStep[t]=1;trackLoopEndStep[t]=trackStepCount(t);clearStepSelection();pendingSelectionEdit={};selectedStep=-1;invalidateStepTriggers();freeInvalidate11();neoDisplayDirty=displayValueDirty=true;return true;
}
FLASHMEM bool recoveryCommand08(char*line){
 if(strncmp(line,"RAM08,",6))return false;
 uint32_t hash=crc905(0xffffffffu,(uint8_t*)line,strlen(line));char*parts[5];unsigned count=0;char*context=nullptr;
 for(char*p=strtok_r(line+6,",",&context);p&&count<5;p=strtok_r(nullptr,",",&context))parts[count++]=p;
 uint32_t id=0,n=0;if(count<2||!number905(parts[0],id)||!id){Serial.println("RAM08,0,ERR,INVALID");return true;}
 recoverySeen08=millis();if(id==recoveryId08&&hash==recoveryHash08){Serial.println(recoveryReply08);return true;}
 recoveryId08=id;recoveryHash08=hash;
 auto reply=[&](const char*s,const char*v){snprintf(recoveryReply08,sizeof(recoveryReply08),"RAM08,%lu,%s,%s",(unsigned long)id,s,v);};
 const char*op=parts[1];reply("ERR","INVALID");
 if(!strcmp(op,"CLOSE")){recoveryFile08.close();recoveryUploading08=false;reply("OK","CLOSED");}
 else if(playing)reply("ERR","STOP_PLAYBACK");
 else if(sdBusy905())reply("ERR","BUSY");
 else if(!strcmp(op,"CAPTURE")){if(pendingSelectionEdit.active)reply("ERR","VALIDATE_SELECTION");else if(recoveryCapture08())snprintf(recoveryReply08,sizeof(recoveryReply08),"RAM08,%lu,SIZE,%lu",(unsigned long)id,(unsigned long)recoveryFile08.size());else reply("ERR","PROJECT_CAPTURE_FAILED");}
 else if(!strcmp(op,"READ")&&count==3&&recoveryFile08&&!recoveryUploading08&&number905(parts[2],n)&&n<recoveryFile08.size()){
  uint8_t b[256];recoveryFile08.seek(n);int got=recoveryFile08.read(b,sizeof(b));char hex[513];hex905(b,got,hex);snprintf(recoveryReply08,sizeof(recoveryReply08),"RAM08,%lu,DATA,%lu,%08lX,%s",(unsigned long)id,(unsigned long)n,(unsigned long)~crc905(0xffffffffu,b,got),hex);
 }
 else if(!strcmp(op,"UPLOAD")){recoveryFile08.close();recoveryFile08=File(new RamFile08);recoveryUploading08=true;reply("OK","READY");}
 else if(!strcmp(op,"WRITE")&&count==4&&recoveryUploading08&&number905(parts[2],n)&&n==recoveryFile08.size()){
  uint8_t b[256];int got=decode905(parts[3],b,sizeof(b));if(got>0&&recoveryFile08.write(b,got)==(size_t)got)reply("OK","WRITTEN");else reply("ERR","MEMORY_OR_DATA");
 }
 else if(!strcmp(op,"APPLYTEXT15")&&count==3&&recoveryUploading08&&number905(parts[2],n)&&n==recoveryFile08.size()){
  bool ok=recoveryText15(recoveryFile08);reply(ok?"OK":"ERR",ok?"RESTORED":"INVALID_PATTERN");recoveryUploading08=false;if(ok){showGameScreen();songSnapshot();}
 }
 else if(!strcmp(op,"APPLY")&&count==3&&recoveryUploading08&&number905(parts[2],n)&&n==recoveryFile08.size()){
  recoveryFile08.seek(0);bool ok=loadFullFile08(recoveryFile08,true);recoveryClearStages08();reply(ok?"OK":"ERR",ok?"RESTORED":"INVALID_PROJECT");if(ok){recoveryUploading08=false;showGameScreen();songSnapshot();}
 }
 Serial.println(recoveryReply08);return true;
}
void recoveryService08(){if(recoveryFile08&&millis()-recoverySeen08>60000){recoveryFile08.close();recoveryUploading08=false;}}
