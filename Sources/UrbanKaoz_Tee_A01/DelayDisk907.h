#pragma once
FLASHMEM bool delayStageAdd907(uint16_t key,DelayValues907 value){
 for(auto&t:value.targets55)if(!delayTargetValid55(t))return false;
 if(key>=2048||value.gat08==-128||value.gex08>100||value.edited930>1||value.mode>2||value.fixedVelocity926<0||value.relativeVelocity926==-128||value.velocity==-128||value.note==-128||value.accel==-128||value.axp>100||value.oxp>100||!value.length||value.length>3072||value.wait>3072||value.repeats>16||value.output>16||value.block21>2||value.rolls>1||value.enabled>1||value.source>1||(value.chain&~((1u<<(key/256))-1)))return false;
 for(auto*n=delayStage907;n;n=n->next)if(n->key==key)return false;
 auto*n=(DelayNode907*)extmem_malloc(sizeof(DelayNode907));if(!n)return false;*n={delayStage907,key,value};delayStage907=n;return true;
}
FLASHMEM bool delayDiskWrite907(File&f){uint16_t count=0;for(auto*n=delayNodes907;n;n=n->next)++count;
 count|=0xFE00;
 if(f.write((uint8_t*)&count,2)!=2)return false;
 for(auto*n=delayNodes907;n;n=n->next){auto value=n->value;for(auto&t:value.targets55)if(t.kind==5||t.kind==6){if(t.identity>=64||!modUnits805[t.identity].enabled)return false;t.identity=modDiskKey931(t.identity);}if(f.write((uint8_t*)&n->key,2)!=2||f.write((uint8_t*)&value,sizeof(value))!=sizeof(value))return false;}return true;
}
FLASHMEM bool delayDiskRead907(File&f){delayClear907(delayStage907);uint16_t count=0;
 if(f.read((uint8_t*)&count,2)!=2)return false;bool targets55=(count&0xFE00)==0xFE00;bool gate08=(count&0xFC00)==0xFC00;bool marked=(count&0xF800)==0xF800,independent=count&0x2000,multi=count&0x4000,extended=count&0x8000;delayProjectFile925=count&0x1000;count&=targets55?0x01ff:gate08?0x03ff:marked?0x07ff:0x0fff;if(count>(marked?64:2048))return false;
 for(uint16_t i=0;i<count;++i){uint16_t key;DelayValues907 value{};value.axp=value.oxp=0;
  if(f.read((uint8_t*)&key,2)!=2||f.read((uint8_t*)&value,targets55?sizeof(value):gate08?offsetof(DelayValues907,targets55):marked?20:independent?19:multi?17:extended?7:4)!=(targets55?sizeof(value):gate08?offsetof(DelayValues907,targets55):marked?20:independent?19:multi?17:extended?7:4)){delayClear907(delayStage907);
return false;
}if(!targets55)value.targets55[0].kind=delay55::NDO;if(targets55)for(auto&t:value.targets55)if(t.kind==5||t.kind==6){if(t.identity>=512||fullMap931[t.identity]<0)return false;t.identity=fullMap931[t.identity];}if(!independent){if(value.mode==1)value.fixedVelocity926=value.velocity;
else value.relativeVelocity926=value.velocity;
}if(!marked)value.edited930=1;
/* historical objects: no reliable edit history */if(!delayStageAdd907(key,value)){delayClear907(delayStage907);
return false;
}}return true;

}
