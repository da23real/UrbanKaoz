#pragma once
// Side-effect-free checks executable on the real Teensy via SONG,CHECK.
FLASHMEM void songSelfCheck() {
  unsigned failed=0,checks=0;
  auto check=[&](bool condition){++checks;if(!condition)++failed;};
  check(storageNameValid("PRISE_01"));check(storageNameValid(""));check(storageNameValid("abcdefghijklmnop"));
  check(!storageNameValid("abcdefghijklmnopq"));check(!storageNameValid("../default"));check(!storageNameValid("nom,commande"));
  check(songSliceContains(2,1536,2,1536,3072));check(songSliceContains(2,3071,2,1536,3072));
  check(!songSliceContains(2,3072,2,1536,3072));check(!songSliceContains(1,2000,2,1536,3072));
  check(!songSliceContains(2,1535,2,1536,3072));
  check(freeZoneTick(112,0,384)==11136); // bar 8, step 5
  check(freeZoneTick(112,16,1920)==11136); // trimmed source: same absolute position
  check(freeZoneTick(112,0,377)==11129); // TIM -7 is not quantized
  check(freeZoneTick(0,0,-7)==-7); // caller must refuse, never silently snap to zero
  check(sdFolderPathValid("/URBANKAOZ"));
  check(sdFolderPathValid("/URBANKAOZ/Test 03"));
  check(!sdFolderPathValid("/URBANKAOZX/Test"));
  check(!sdFolderPathValid("/URBANKAOZ/../PAT"));
  check(!sdFolderPathValid("/URBANKAOZ//Test"));
  check(!sdFolderPathValid("/URBANKAOZ/Test,"));
  check(sdProtected("/URBANKAOZ/PAT"));
  check(!sdProtected("/URBANKAOZ/Temporary"));
  SongClip copies[8];for(unsigned i=0;i<8;++i)copies[i]={(uint32_t)(112+i*16),0,16,0,0,true};
  copies[3].used=false;unsigned remaining=0;for(auto &copy:copies)if(copy.used)++remaining;
  check(remaining==7&&copies[2].used&&copies[4].used);
  SongClip c={80,0,64,3,2,true};
  check(songClipValid(c,64));
  c.length=16;check(songClipValid(c,64)&&c.position==80);
  c.offset=32;c.length=32;check(songClipValid(c,64)&&c.position==80);
  c.offset=33;check(!songClipValid(c,64));
  c.offset=0;c.length=0;check(!songClipValid(c,64));
  c.length=112;check(songClipValid(c,112)); // arbitrary seven-bar source
  c.length=1024;check(songClipValid(c,1024));
  c.position=SONG_MAX_STEP;check(!songClipValid(c,1024));
  c={80,0,16,3,2,true};SongClip d={96,0,16,3,1,true};
  check(!songClipsOverlap(c,d));d.position=95;check(songClipsOverlap(c,d));
  d.track=4;check(!songClipsOverlap(c,d));d.track=3;d.used=false;check(!songClipsOverlap(c,d));
  const uint32_t bar=1536;
  check(songLoopPosition(5ULL*bar,true,6,8,bar)==5ULL*bar);
  check(songLoopPosition(8ULL*bar-1,true,6,8,bar)==8ULL*bar-1);
  check(songLoopPosition(8ULL*bar,true,6,8,bar)==5ULL*bar);
  check(songLoopPosition(11ULL*bar,true,6,8,bar)==5ULL*bar);
  check(songLoopPosition(8ULL*bar,false,6,8,bar)==8ULL*bar);
  check(songLoopPosition(10000000000ULL,false,6,8,bar)==10000000000ULL);
  check(songLoopPosition(6ULL*bar,true,6,6,bar)==5ULL*bar);
  ZLEvent z={0,1,96,15,0,60,100};check(zlValid(z));z.b=0;check(!zlValid(z));z.kind=1;check(zlValid(z));z.track=16;check(!zlValid(z));
  z={((uint64_t)SONG_MAX_STEP*96)-1,1,1,0,0,60,100};check(zlValid(z));z.gate=2;check(!zlValid(z));
  songSend("CHECK,%s,%u,%u",failed?"FAIL":"PASS",checks,failed);
}
