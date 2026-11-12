#pragma once
// Synthetic fixtures only. Never reads/captures/restores the user's project.
FLASHMEM void checksStorage18(){
 if(playing||fileBrowserMode||itemKind809){Serial.println("CHECKSTORAGE18,BUSY");return;}
 unsigned pass=0,fail=0;auto ck=[&](bool ok,const char*label){Serial.printf("CHECKSTORAGE18,%s,%s\n",ok?"PASS":"FAIL",label);ok?++pass:++fail;};
 auto*d=(ScopeData18*)extmem_malloc(sizeof(ScopeData18));auto*read=(ScopeData18*)extmem_malloc(sizeof(ScopeData18));
 auto*c=(Content18*)extmem_malloc(sizeof(Content18));auto*loaded=(Content18*)extmem_malloc(sizeof(Content18));auto*im=(PatternImage805*)extmem_malloc(sizeof(PatternImage805));
 if(!d||!read||!c||!loaded||!im){if(d)extmem_free(d);if(read)extmem_free(read);if(c)extmem_free(c);if(loaded)extmem_free(loaded);if(im)extmem_free(im);Serial.println("CHECKSTORAGE18,MEMORY");return;}
 for(uint8_t kind=ITEM_KB18;kind<=ITEM_DELAYS18;++kind){new(d)ScopeData18{};d->kind=kind;d->origin=15;d->kb.split=2;d->kb.zone[3].range=-7;
  if(kind==ITEM_DELAYS18){d->present=15;for(uint8_t j=0;j<4;++j){d->delays[j].length=96*(j+1);d->delays[j].chain=(1u<<j)-1;}}
  if(kind==ITEM_ENVS18||kind==ITEM_LFOS18){d->roots=1;d->mods[0].unit.enabled=1;d->mods[0].unit.rate=96;d->mods[0].kind=kind==ITEM_LFOS18;strcpy(d->mods[0].name,"FIXTURE");}
  File f(new RamFile08);ck(scopeWrite18(f,*d),"scope_write_RAM");f.seek(0);ck(scopeRead18(f,*read,kind)&&!memcmp(d,read,sizeof(*d)),"scope_roundtrip");f.seek(0);ck(!scopeRead18(f,*read,kind==ITEM_KB18?ITEM_DELAYS18:ITEM_KB18),"scope_wrong_kind_refused");f.close();
 }
 new(c)Content18{};new(loaded)Content18{};memset(im,0,sizeof(*im));im->bars=1;im->active[0]=true;im->note[0]=60;im->vel[0]=100;im->gate[0]=777;im->extraCount[0]=3;for(uint8_t v=0;v<3;++v){im->extraNote[0][v]=64+v;im->extra14[0][v]={uint16_t(500+v),int16_t(v-2),uint8_t(80+v)};}
 c->kind=3;c->count=1;c->flags=1;auto&e=c->entries[0];e.track=15;e.pattern=15;e.length=16;e.image=packPattern906(*im);e.count=2;e.locks=(NoteLock817*)extmem_malloc(2*sizeof(NoteLock817));
 if(e.image&&e.locks){e.locks[0]={0,99,15,7,15,15,60,LOCK_CC817,0};e.locks[1]={96,77,15,8,15,15,188,LOCK_CC817,432};
  File f(new RamFile08);ck(contentWrite18(f,*c),"content_write_RAM");f.seek(0);ck(contentRead18(f,*loaded),"content_read_RAM");ck(loaded->entries[0].count==2&&loaded->entries[0].locks&&loaded->entries[0].locks[1].duration==432,"free_PLX_gate");ck(unpackPattern906(loaded->entries[0].image,*im)&&im->extra14[0][2].gate==502,"chord_independent_parameters");contentFree18(*loaded);f.close();
  // A disposable SD fixture verifies the actual File/SD transport. Refuse
  // every pre-existing name; delete only this newly created synthetic file.
  char path[80];snprintf(path,sizeof(path),"/URBANKAOZ/TEST18/T%08lX.TMP",(unsigned long)micros());
  bool made=sdAvailable&&storageWriteAvailable&&!SD.exists(path)&&(SD.exists("/URBANKAOZ/TEST18")||SD.mkdir("/URBANKAOZ/TEST18"));
  if(made){f=SD.open(path,FILE_WRITE_BEGIN);made=(bool)f;bool ok=made&&contentWrite18(f,*c);f.flush();f.close();ck(ok,"synthetic_SD_write");if(ok){f=SD.open(path,FILE_READ);ck(f&&contentRead18(f,*loaded),"synthetic_SD_read");f.close();contentFree18(*loaded);}if(made)ck(SD.remove(path),"synthetic_SD_file_removed");}
  else ck(false,"synthetic_SD_available");
 }else ck(false,"synthetic_allocations");
 contentFree18(*c);contentFree18(*loaded);extmem_free(c);extmem_free(loaded);extmem_free(d);extmem_free(read);extmem_free(im);
 Serial.printf("CHECKSTORAGE18,RESULT,PASS=%u,FAIL=%u\n",pass,fail);
}
