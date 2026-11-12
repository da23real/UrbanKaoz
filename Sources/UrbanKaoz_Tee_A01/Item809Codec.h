#pragma once
// Portable standalone objects. The whole file is validated off-line before installation.
// Presets embed the complete definition, not a project-local numeric ID or just a name.
struct Item809 {
  char name07[8]={};CcEnvelope941 cc941={};uint8_t kind=0; PatchDef808 *def=nullptr; PatchContext808 values={0,-1,nullptr};
  int16_t early50=0;ModUnit805 curve={}; ModExtra810 extra={}; uint16_t rat=8; Delay805 delay={}; DelayExtra817 delayExtra={0,16,1};DelayValues907 delay907={};
};
FLASHMEM void itemFree809(Item809 &s){
  if(s.def){while(s.def->params){auto q=s.def->params;s.def->params=q->next;extmem_free(q);}extmem_free(s.def);s.def=nullptr;}
  patch808FreeValues(s.values.values);
}
FLASHMEM bool itemName809(const char*s,uint8_t maxLen,uint8_t minLen=1){
  size_t n=strlen(s);if(n<minLen||n>maxLen)return false;
  for(;*s;++s)if(!((*s>='a'&&*s<='z')||(*s>='A'&&*s<='Z')||(*s>='0'&&*s<='9')||*s==' '||*s=='_'||*s=='-'))return false;
  return true;
}
FLASHMEM bool itemNumber809(const char*s,uint32_t maxValue,uint32_t&v){
  if(!s||!*s)return false;v=0;
  for(;*s;++s){if(*s<'0'||*s>'9')return false;uint8_t digit=*s-'0';if(v>maxValue/10||(v==maxValue/10&&digit>maxValue%10))return false;v=v*10+digit;}return true;
}
FLASHMEM bool itemLine809(File&f,char (&line)[128]){
  size_t n=0;while(f.available()){uint8_t c;if(f.read(&c,1)!=1)return false;if(c=='\n'){line[n]=0;return true;}if(c=='\r')continue;if(c<32||c>126||n==127)return false;line[n++]=(char)c;}return false;
}
FLASHMEM uint8_t itemFields809(char*line,char**fields,uint8_t maxFields){
  uint8_t n=0;fields[n++]=line;for(char*p=line;*p;++p)if(*p==','){*p=0;if(n==maxFields)return 0;fields[n++]=p+1;}return n;
}
FLASHMEM bool itemSame809(const PatchDef808*a,const PatchDef808*b){
  if(!a||!b||strcmp(a->name,b->name)||a->noteOffset44!=b->noteOffset44)return false;
  auto x=a->params,y=b->params;
  for(;x&&y;x=x->next,y=y->next)if(x->number!=y->number||x->kind!=y->kind||x->bits!=y->bits||strcmp(x->name,y->name)||strcmp(x->shortName,y->shortName))return false;
  return !x&&!y;
}
FLASHMEM bool itemRead809(File&f,Item809&s,uint8_t expected){
  char line[128],*fields[12];uint32_t v[11]={};
  if(!itemLine809(f,line)||itemFields809(line,fields,12)!=3||strcmp(fields[0],PSTR("UKITEM"))||(strcmp(fields[1],PSTR("1"))&&strcmp(fields[1],PSTR("2"))&&strcmp(fields[1],PSTR("3"))&&strcmp(fields[1],PSTR("4"))&&strcmp(fields[1],PSTR("5"))&&strcmp(fields[1],PSTR("6"))&&strcmp(fields[1],PSTR("7"))&&strcmp(fields[1],PSTR("8"))&&strcmp(fields[1],PSTR("9"))&&strcmp(fields[1],PSTR("10"))&&strcmp(fields[1],PSTR("11"))&&strcmp(fields[1],PSTR("12"))&&strcmp(fields[1],PSTR("13"))&&strcmp(fields[1],PSTR("14"))&&strcmp(fields[1],PSTR("15"))&&strcmp(fields[1],PSTR("16"))&&strcmp(fields[1],PSTR("17"))&&strcmp(fields[1],PSTR("18"))&&strcmp(fields[1],PSTR("19"))&&strcmp(fields[1],PSTR("20"))&&strcmp(fields[1],PSTR("21")))||!itemNumber809(fields[2],5,v[0])||v[0]!=expected)return false;
  bool version58=!strcmp(fields[1],"21");bool version55=!strcmp(fields[1],"19")||!strcmp(fields[1],"20");bool version50=version58||!strcmp(fields[1],"18");bool version21=version55||!strcmp(fields[1],"14");bool version42=version50||!strcmp(fields[1],"17");bool version27=version42||!strcmp(fields[1],"13");bool version08=version21||!strcmp(fields[1],"12");bool version07=!strcmp(fields[1],"16")||version27||!strcmp(fields[1],"11");bool version941=version07||!strcmp(fields[1],PSTR("10"));bool version930=version08||!strcmp(fields[1],PSTR("9"));bool version926=version930||!strcmp(fields[1],PSTR("8"));bool version924=version926||!strcmp(fields[1],PSTR("7"));bool version923=version924||!strcmp(fields[1],PSTR("6"));bool version907=version923||!strcmp(fields[1],PSTR("5"));bool version817=version907||!strcmp(fields[1],PSTR("3"));bool modern=strcmp(fields[1],PSTR("1"))!=0;s.kind=expected;
  if(expected==ITEM_PATCH809||expected==ITEM_PRESET809){
    s.def=patch808Allocate<PatchDef808>();if(!s.def)return false;
    if(!itemLine809(f,line)||itemFields809(line,fields,12)!=2||strcmp(fields[0],PSTR("NAME"))||!itemName809(fields[1],48))return false;
    strcpy(s.def->name,fields[1]);auto tail=&s.def->params;uint32_t count=0;bool valuesStarted=false;
    while(itemLine809(f,line)){
      if(!strcmp(line,PSTR("END")))return f.available()==0;
      uint8_t n=itemFields809(line,fields,12);
      if(n==2&&!strcmp(fields[0],"NOTEOFFSET")&&!valuesStarted){
        const char*number=fields[1];bool minus=*number=='-';if(minus)++number;if(!itemNumber809(number,127,v[0]))return false;s.def->noteOffset44=minus?-(int)v[0]:(int)v[0];
      }else if(n==6&&!strcmp(fields[0],PSTR("P"))&&!valuesStarted){
        if(!itemNumber809(fields[1],2,v[0])||!itemNumber809(fields[2],65535,v[1])||!itemNumber809(fields[3],14,v[2])||
           (v[2]!=7&&v[2]!=14)||(v[0]!=1&&(v[1]>127||v[2]!=7))||!itemName809(fields[4],6)||!itemName809(fields[5],3,1))return false;
        auto q=patch808Allocate<PatchParam808>();if(!q)return false;*tail=q;tail=&q->next;
        q->id=++count;q->kind=v[0];q->number=v[1];q->bits=v[2];strcpy(q->name,fields[4]);strcpy(q->shortName,fields[5]);
      }else if(n==3&&!strcmp(fields[0],PSTR("V"))&&expected==ITEM_PRESET809){
        valuesStarted=true;
        if(!itemNumber809(fields[1],count,v[0])||!v[0]||!itemNumber809(fields[2],65535,v[1]))return false;
        auto q=patch808ParamAt(s.def,v[0]-1);uint16_t unused;
        if(!q||(v[1]!=65535&&v[1]>(q->bits==14?16383u:127u))||patch808Get(s.values,v[0],unused)||!patch808Set(s.values,v[0],v[1]))return false;
      }else return false;
    }return false;
  }
  if(!itemLine809(f,line))return false;uint8_t n=itemFields809(line,fields,12);
  if(expected==ITEM_ENV809||expected==ITEM_LFO809){
    if(n!=11||strcmp(fields[0],PSTR("CURVE")))return false;
    const uint32_t limits[10]={modern?65535u:999u,modern?65535u:999u,modern?65535u:999u,modern?65535u:999u,version42?LFO_MAX42:modern?65535u:3072u,127,2,modern?4u:3u,3072,version58?8u:1u};
    for(uint8_t i=0;i<10;++i)if(!itemNumber809(fields[i+1],limits[i],v[i]))return false;
    if(!v[4]||!v[8])return false;
    s.curve.hold=v[0];s.curve.attack=v[1];s.curve.decay=v[2];s.curve.release=v[3];s.curve.rate=v[4];s.curve.sustain=v[5];s.curve.curve=v[6];s.curve.shape=v[7];s.rat=v[8];
    if(expected==ITEM_LFO809&&!version27)s.curve.sustain=80;
    // The last field is reserved for forward-compatible format validation, not routing.
    if(version58){if(v[9]>8)return false;s.curve.roll58=v[9]%3;s.curve.delay58=v[9]/3;}else if(v[9])return false;
    if(modern){
      if(!itemLine809(f,line)||itemFields809(line,fields,12)!=6||strcmp(fields[0],PSTR("EXTRA")))return false;
      int8_t curves[3];
      for(uint8_t i=0;i<3;++i){const char*p=fields[i+1];bool negative=*p=='-';if(negative||*p=='+')++p;if(!itemNumber809(p,100,v[i]))return false;curves[i]=negative?-(int)v[i]:v[i];}
      if(!itemNumber809(fields[4],version07?6:2,v[3])||!itemNumber809(fields[5],360,v[4]))return false;
      s.extra={curves[0],curves[1],curves[2],(uint8_t)v[3],(uint16_t)v[4]};
    }else modMigrateUnit810(s.curve,s.extra,expected==ITEM_LFO809);
    if(version941){if(!itemLine809(f,line)||itemFields809(line,fields,12)!=5||strcmp(fields[0],"CCIN"))return false;const uint32_t ranges[4]={1,15,127,1};for(uint8_t i=0;i<4;++i)if(!itemNumber809(fields[i+1],ranges[i],v[i]))return false;s.cc941={(uint8_t)v[0],(uint8_t)v[1],(uint8_t)v[2],(uint8_t)v[3]};}
  if(version07){if(!itemLine809(f,line)||itemFields809(line,fields,12)!=2||strcmp(fields[0],"NAME")||!itemName809(fields[1],7,0))return false;strcpy(s.name07,fields[1]);}if(version50&&expected==ITEM_ENV809){if(!itemLine809(f,line)||itemFields809(line,fields,12)!=2||strcmp(fields[0],"EARLY")||!itemNumber809(fields[1],384,v[0]))return false;s.early50=-(int)v[0];}
  }else if(expected==ITEM_FX809){
    if(n!=5||strcmp(fields[0],PSTR("DELAY"))||!itemNumber809(fields[1],3072,v[0])||!v[0]||!itemNumber809(fields[2],16,v[1])||!itemNumber809(fields[3],127,v[2])||!itemNumber809(fields[4],1,v[3]))return false;
    s.delay={(uint16_t)v[0],(uint8_t)v[1],(uint8_t)v[2],(uint8_t)v[3]};
    if(version817){if(!itemLine809(f,line)||itemFields809(line,fields,12)!=4||strcmp(fields[0],PSTR("DELAYEX"))||!itemNumber809(fields[1],3072,v[0])||!itemNumber809(fields[2],16,v[1])||!itemNumber809(fields[3],1,v[2]))return false;s.delayExtra={(uint16_t)v[0],(uint8_t)v[1],(uint8_t)v[2]};}
    if(version907){if(!itemLine809(f,line)||itemFields809(line,fields,12)!=5||strcmp(fields[0],"DELAY907"))return false;
     int8_t values[3];for(uint8_t i=0;i<3;++i){const char*q=fields[i+1];bool neg=*q=='-';if(neg||*q=='+')++q;if(!itemNumber809(q,i==2&&neg?128:127,v[i]))return false;values[i]=neg?-(int)v[i]:v[i];}
     if(!itemNumber809(fields[4],2,v[3]))return false;s.delay907={values[0],values[1],values[2],(uint8_t)v[3]};
    }else s.delay907={(int8_t)s.delay.velocity,0,0,2};
    if(version923){
      if(!itemLine809(f,line)||itemFields809(line,fields,12)!=4||strcmp(fields[0],"DELAY923"))return false;
      const char*q=fields[1];bool neg=*q=='-';if(neg||*q=='+')++q;
      if(!itemNumber809(q,127,v[0])||!itemNumber809(fields[2],100,v[1])||!itemNumber809(fields[3],100,v[2]))return false;
      s.delay907.accel=neg?-(int)v[0]:v[0];s.delay907.axp=v[1];s.delay907.oxp=v[2];
    }
    if(version924){if(!itemLine809(f,line)||itemFields809(line,fields,12)!=3||strcmp(fields[0],"DELAY924")||!itemNumber809(fields[1],1,v[0])||!itemNumber809(fields[2],127,v[1]))return false;s.delay907.source=v[0];s.delay907.chain=v[1];}
    if(version926){if(!itemLine809(f,line)||itemFields809(line,fields,12)!=3||strcmp(fields[0],"VEL926"))return false;
char*end;
long a=strtol(fields[1],&end,10);
if(*end||a<0||a>127)return false;
long b=strtol(fields[2],&end,10);
if(*end||b< -127||b>127)return false;
s.delay907.fixedVelocity926=a;
s.delay907.relativeVelocity926=b;
}else {if(s.delay907.mode==1)s.delay907.fixedVelocity926=s.delay907.velocity;
else s.delay907.relativeVelocity926=s.delay907.velocity;
}
    if(version930){if(!itemLine809(f,line)||itemFields809(line,fields,12)!=2||strcmp(fields[0],"EDIT930")||!itemNumber809(fields[1],1,v[0]))return false;s.delay907.edited930=v[0];}else s.delay907.edited930=1;
  if(version08){int g,x;if(!itemLine809(f,line)||sscanf(line,"GATE08,%d,%d",&g,&x)!=2||g< -127||g>127||x<0||x>100)return false;s.delay907.gat08=g;s.delay907.gex08=x;}
  if(version21){unsigned b;if(!itemLine809(f,line)||sscanf(line,"BLK21,%u",&b)!=1||b>2)return false;s.delay907.block21=b;}
  if(!version55)s.delay907.targets55[0].kind=delay55::NDO;
  if(version55){for(auto&t:s.delay907.targets55){int k,p,c,num,id,slot,step,base,exp,track;if(!itemLine809(f,line)||sscanf(line,"TARGET55,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",&k,&p,&c,&num,&id,&slot,&step,&base,&exp,&track)!=10||k<0||k>255||p<0||p>7||c<0||c>15||num<0||num>16383||id<0||id>63||slot<0||slot>7||step< -32767||step>32767||base< -8192||base>16383||exp<0||exp>100||track<0||track>255)return false;t={(int16_t)step,(int16_t)base,(uint16_t)num,(uint16_t)id,(uint8_t)k,(uint8_t)p,(uint8_t)c,(uint8_t)slot,(uint8_t)exp,(uint8_t)track};if(!delayTargetValid55(t))return false;}}
  }else return false;
  return itemLine809(f,line)&&!strcmp(line,PSTR("END"))&&!f.available();
}
FLASHMEM bool itemWrite809(File&f,const Item809&s){
  bool ok=true;char line[128];auto put=[&](const char*x){size_t n=strlen(x);if(ok)ok=f.write((const uint8_t*)x,n)==n;};
  snprintf(line,sizeof(line),PSTR("UKITEM,%u,%u\n"),s.kind==ITEM_FX809?20:s.kind==ITEM_LFO809?21:s.kind==ITEM_ENV809?(ioUnit38(s.curve)?16:21):(s.kind==ITEM_PATCH809||s.kind==ITEM_PRESET809)?19:4,s.kind);put(line);
  if(s.kind==ITEM_PATCH809||s.kind==ITEM_PRESET809){
    if(!s.def)return false;snprintf(line,sizeof(line),PSTR("NAME,%s\n"),s.def->name);put(line);if(s.def->noteOffset44){snprintf(line,sizeof(line),"NOTEOFFSET,%d\n",s.def->noteOffset44);put(line);}
    for(auto q=s.def->params;q;q=q->next){snprintf(line,sizeof(line),PSTR("P,%u,%u,%u,%s,%s\n"),q->kind,q->number,q->bits,q->name,q->shortName);put(line);}
    if(s.kind==ITEM_PRESET809){uint32_t index=0;for(auto q=s.def->params;q;q=q->next){++index;uint16_t value;if(patch808Get(s.values,q->id,value)){snprintf(line,sizeof(line),PSTR("V,%lu,%u\n"),(unsigned long)index,value);put(line);}}}
  }else if(s.kind==ITEM_ENV809||s.kind==ITEM_LFO809){const auto&m=s.curve;
    snprintf(line,sizeof(line),PSTR("CURVE,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u\n"),m.hold,m.attack,m.decay,m.release,m.rate,m.sustain,m.curve,m.shape,s.rat,m.roll58+3*m.delay58);put(line);
    snprintf(line,sizeof(line),PSTR("EXTRA,%d,%d,%d,%u,%u\n"),s.extra.attackCurve,s.extra.decayCurve,s.extra.releaseCurve,s.extra.mode,s.extra.phase);put(line);snprintf(line,sizeof(line),"CCIN,%u,%u,%u,%u\n",s.cc941.enabled,s.cc941.channel,s.cc941.number,s.cc941.invert);put(line);snprintf(line,sizeof(line),"NAME,%s\n",s.name07);put(line);if(s.kind==ITEM_ENV809&&!ioUnit38(s.curve)){snprintf(line,sizeof(line),"EARLY,%u\n",(unsigned)-s.early50);put(line);}
  }else if(s.kind==ITEM_FX809){snprintf(line,sizeof(line),PSTR("DELAY,%u,%u,%u,%u\n"),s.delay.ticks,s.delay.repeats,s.delay.velocity,s.delay.enabled);
put(line);
snprintf(line,sizeof(line),PSTR("DELAYEX,%u,%u,%u\n"),s.delayExtra.wait,s.delayExtra.output,s.delayExtra.rolls);
put(line);
snprintf(line,sizeof(line),"DELAY907,%d,%d,%d,%u\n",s.delay907.velocity,s.delay907.note,s.delay907.scdo,s.delay907.mode);
put(line);
snprintf(line,sizeof(line),"DELAY923,%d,%u,%u\n",s.delay907.accel,s.delay907.axp,s.delay907.oxp);
put(line);
snprintf(line,sizeof(line),"DELAY924,%u,%u\n",s.delay907.source,s.delay907.chain);
put(line);
snprintf(line,sizeof(line),"VEL926,%d,%d\n",s.delay907.fixedVelocity926,s.delay907.relativeVelocity926);
put(line);
snprintf(line,sizeof(line),"EDIT930,%u\n",s.delay907.edited930);
put(line);snprintf(line,sizeof(line),"GATE08,%d,%u\n",s.delay907.gat08,s.delay907.gex08);put(line);snprintf(line,sizeof(line),"BLK21,%u\n",s.delay907.block21);put(line);for(auto&t:s.delay907.targets55){snprintf(line,sizeof(line),"TARGET55,%u,%u,%u,%u,%u,%u,%d,%d,%u,%u\n",t.kind,t.port,t.channel,t.number,t.identity,t.slot,t.step,t.base,t.exp,t.track);put(line);}
}
  else return false;put(PSTR("END\n"));return ok;
}
FLASHMEM bool itemPresetValues809(const Item809&s,PatchDef808*d,PatchContext808&out){
  if(!itemSame809(s.def,d))return false;out.patch=d->id;out.channel=-1;out.values=nullptr;
  for(auto v=s.values.values;v;v=v->next){auto q=patch808ParamAt(d,v->parameter-1);if(!q||!patch808Set(out,q->id,v->value)){patch808FreeValues(out.values);return false;}}return true;
}


