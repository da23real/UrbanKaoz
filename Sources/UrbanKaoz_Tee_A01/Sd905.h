#pragma once
// Request/reply transfer protocol, bounded chunks, one replayable response.
// No project save/load occurs here. Files are changed only by explicit commands.
static bool sdLease905=false,sdUpload905=false,sdReplace905=false;
static File sdFile905;
static uint32_t sdSeen905=0,sdId905=0,sdRequestHash905=0,sdOffset905=0,sdCrc905=0xffffffffu;
static char sdReply905[600]={},sdTarget905[192]={},sdTemp905[192]={};
FLASHMEM bool sdBusy905(){return sdLease905;}
FLASHMEM uint32_t crc905(uint32_t crc,const uint8_t*p,size_t n){while(n--){crc^=*p++;for(unsigned i=0;i<8;++i)crc=(crc>>1)^(0xedb88320u&-(crc&1));}return crc;}
FLASHMEM int unhex905(char c){return c>='0'&&c<='9'?c-'0':c>='A'&&c<='F'?c-'A'+10:c>='a'&&c<='f'?c-'a'+10:-1;}
FLASHMEM int decode905(const char*s,uint8_t*out,size_t cap){size_t n=strlen(s);if(n%2||n/2>cap)return -1;for(size_t i=0;i<n;i+=2){int a=unhex905(s[i]),b=unhex905(s[i+1]);if(a<0||b<0)return -1;out[i/2]=(a<<4)|b;}return n/2;}
FLASHMEM void hex905(const uint8_t*p,size_t n,char*out){static const char digits[]="0123456789ABCDEF";for(size_t i=0;i<n;++i){out[2*i]=digits[p[i]>>4];out[2*i+1]=digits[p[i]&15];}out[n*2]=0;}
FLASHMEM bool path905(const char*hex,char*out,size_t cap){int n=decode905(hex,(uint8_t*)out,cap-1);if(n<1)return false;out[n]=0;if(out[0]!='/'||strlen(out)!=(size_t)n)return false;for(int i=0;i<n;++i)if((uint8_t)out[i]<32||strchr("\\:*?\"<>|",out[i]))return false;
 const char*p=out+1;while(*p){const char*e=strchr(p,'/');size_t len=e?(size_t)(e-p):strlen(p);if(!len||(len==1&&p[0]=='.')||(len==2&&p[0]=='.'&&p[1]=='.'))return false;if(!e)break;p=e+1;if(!*p)return false;}return true;}
FLASHMEM bool number905(const char*s,uint32_t&v){if(!s||!*s)return false;uint64_t n=0;for(;*s;++s){if(*s<'0'||*s>'9')return false;n=n*10+*s-'0';if(n>UINT32_MAX)return false;}v=n;return true;}
FLASHMEM void sdAbort905(){sdFile905.close();if(sdUpload905&&sdTemp905[0])SD.remove(sdTemp905);sdUpload905=false;sdTemp905[0]=0;sdLease905=false;}
FLASHMEM void sdService905(){if(sdLease905&&(!Serial||millis()-sdSeen905>15000))sdAbort905();}
FLASHMEM bool sdCommand905(char*line){
 if(strncmp(line,"SD1,",4))return false;
 uint32_t hash=crc905(0xffffffffu,(uint8_t*)line,strlen(line));
 char*parts[7];unsigned count=0;char*save=nullptr;for(char*p=strtok_r(line+4,",",&save);p&&count<7;p=strtok_r(nullptr,",",&save))parts[count++]=p;
 uint32_t id=0;if(count<2||!number905(parts[0],id)||!id){Serial.println("SD1,0,ERR,INVALID");return true;}
 sdSeen905=millis();
 if(id==sdId905&&hash==sdRequestHash905){Serial.println(sdReply905);return true;}
 sdId905=id;sdRequestHash905=hash;snprintf(sdReply905,sizeof(sdReply905),"SD1,%lu,ERR,INVALID",(unsigned long)id);
 auto reply=[&](const char*status,const char*value){snprintf(sdReply905,sizeof(sdReply905),"SD1,%lu,%s,%s",(unsigned long)id,status,value);};
 const char*op=parts[1];char path[192]={};uint32_t offset=0;
 if(count==2&&!strcmp(op,"CLOSE")){sdAbort905();reply("OK","CLOSED");}
 else if(playing){reply("ERR","STOP_PLAYBACK");}
 else if(!sdAvailable){reply("ERR","NO_SD");}
 else if(count==2&&!strcmp(op,"OPEN")){
  if(sdLease905||fileBrowserMode!=FILE_BROWSER_NONE||saveMenuActive||loadTypeMenuActive||itemKind809||itemChoosing809)reply("ERR","BUSY");
  else {sdLease905=true;reply("OK","READY");}
 }
 else if(!sdLease905)reply("ERR","NO_SESSION");
 else if(count==3&&!strcmp(op,"LIST")&&path905(parts[2],path,sizeof(path))){
  if(sdUpload905)reply("ERR","BUSY");else {sdFile905.close();sdFile905=SD.open(path);if(sdFile905&&sdFile905.isDirectory())reply("OK","LIST");else {sdFile905.close();reply("ERR","NOT_DIRECTORY");}}
 }
 else if(count==2&&!strcmp(op,"NEXT")&&sdFile905&&sdFile905.isDirectory()){
  File entry=sdFile905.openNextFile();if(!entry)reply("END","LIST");else if(strlen(entry.name())>255){entry.close();reply("ERR","NAME_TOO_LONG");}else {char encoded[513];hex905((const uint8_t*)entry.name(),strlen(entry.name()),encoded);snprintf(sdReply905,sizeof(sdReply905),"SD1,%lu,ENTRY,%c,%llu,%s",(unsigned long)id,entry.isDirectory()?'D':'F',(unsigned long long)entry.size(),encoded);entry.close();}
 }
 else if(count==3&&!strcmp(op,"GET")&&path905(parts[2],path,sizeof(path))){
  if(sdUpload905)reply("ERR","BUSY");else {sdFile905.close();sdFile905=SD.open(path,FILE_READ);if(sdFile905&&!sdFile905.isDirectory()){snprintf(sdReply905,sizeof(sdReply905),"SD1,%lu,SIZE,%llu",(unsigned long)id,(unsigned long long)sdFile905.size());}else {sdFile905.close();reply("ERR","NOT_FILE");}}
 }
 else if(count==3&&!strcmp(op,"READ")&&!sdUpload905&&sdFile905&&!sdFile905.isDirectory()&&number905(parts[2],offset)){
  uint8_t data[256];if(offset>sdFile905.size()||!sdFile905.seek(offset))reply("ERR","OFFSET");else {int n=sdFile905.read(data,sizeof(data));if(n<0)reply("ERR","READ");else {char encoded[513];hex905(data,n,encoded);snprintf(sdReply905,sizeof(sdReply905),"SD1,%lu,DATA,%lu,%08lX,%s",(unsigned long)id,(unsigned long)offset,(unsigned long)~crc905(0xffffffffu,data,n),encoded);}}
 }
 else if(count==4&&!strcmp(op,"PUT")&&path905(parts[2],path,sizeof(path))&&(!strcmp(parts[3],"0")||!strcmp(parts[3],"1"))){
  if(sdUpload905)reply("ERR","BUSY");else if(!storageWriteAvailable)reply("ERR","READ_ONLY");else if(SD.exists(path)&&!strcmp(parts[3],"0"))reply("ERR","EXISTS");else {
   sdFile905.close();File target=SD.open(path);bool directory=target&&target.isDirectory();target.close();
   if(directory)reply("ERR","IS_DIRECTORY");else {
    strcpy(sdTarget905,path);sdReplace905=!strcmp(parts[3],"1");char*slash=strrchr(path,'/');slash[1]=0;
    bool available=false;for(unsigned i=0;i<100;++i){snprintf(sdTemp905,sizeof(sdTemp905),"%sUKSD%02u.TMP",path,i);if(!SD.exists(sdTemp905)){available=true;break;}}
    if(available)sdFile905=SD.open(sdTemp905,FILE_WRITE_BEGIN);
    if(!available||!sdFile905)reply("ERR","CREATE");else {sdUpload905=true;sdOffset905=0;sdCrc905=0xffffffffu;reply("OK","UPLOAD");}
   }
  }
 }
 else if(count==4&&!strcmp(op,"WRITE")&&sdUpload905&&number905(parts[2],offset)){
  uint8_t data[256];int n=decode905(parts[3],data,sizeof(data));
  if(n<=0||offset!=sdOffset905)reply("ERR","OFFSET_OR_DATA");else if(sdFile905.write(data,n)!=(size_t)n){reply("ERR","WRITE");sdAbort905();}
  else {sdOffset905+=n;sdCrc905=crc905(sdCrc905,data,n);reply("OK","WRITTEN");}
 }
 else if(count==4&&!strcmp(op,"COMMIT")&&sdUpload905&&number905(parts[2],offset)){
  uint32_t crc=0;uint8_t bytes[4];int n=decode905(parts[3],bytes,4);for(int i=0;i<n;++i)crc=(crc<<8)|bytes[i];
  if(n!=4||offset!=sdOffset905||crc!=~sdCrc905)reply("ERR","CHECKSUM");else {
   sdFile905.flush();bool ok=sdFile905.size()==sdOffset905;sdFile905.close();char backup[200]={};bool moved=false;
   if(ok&&SD.exists(sdTarget905)){
    snprintf(backup,sizeof(backup),"%s.BAK",sdTemp905);
    ok=sdReplace905&&!SD.exists(backup)&&SD.sdfs.rename(sdTarget905,backup);moved=ok;
   }
   if(ok)ok=SD.sdfs.rename(sdTemp905,sdTarget905);
   if(!ok){if(moved)SD.sdfs.rename(backup,sdTarget905);reply("ERR","COMMIT");}
   else {if(moved)SD.remove(backup);sdUpload905=false;sdTemp905[0]=0;reply("OK","COMMITTED");}
  }
 }
 else if(count==3&&!strcmp(op,"DELETE")&&!sdUpload905&&path905(parts[2],path,sizeof(path))){
  sdFile905.close();File f=SD.open(path);bool regular=f&&!f.isDirectory();f.close();reply(regular&&storageWriteAvailable&&SD.remove(path)?"OK":"ERR",regular?"DELETE":"NOT_FILE");
 }
 else if(count==4&&!strcmp(op,"RENAME")&&!sdUpload905&&path905(parts[2],path,sizeof(path))){
  char dest[192];if(path905(parts[3],dest,sizeof(dest))){sdFile905.close();File f=SD.open(path);bool regular=f&&!f.isDirectory();f.close();reply(regular&&storageWriteAvailable&&!SD.exists(dest)&&SD.sdfs.rename(path,dest)?"OK":"ERR","RENAME");}
 }
 Serial.println(sdReply905);return true;
}
