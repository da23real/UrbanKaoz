#pragma once
// HEAD follows the transport loop START/END; zoom only affects brightness.
uint64_t playheadTick24(){return songEngineActive?songTick:globalLoopedTick();}
uint8_t playheadBrightness24(uint8_t track,uint8_t neoIndex,uint8_t pad){
 if(!playing)return 0;
 uint8_t position=(playheadTick24()/TICKS_PER_STEP)%64;
 uint8_t pixel=(pad/4)*16+neoIndex*4+pad%4;
 uint8_t distance=(position+64-pixel)%64;
 if(distance){if(!headTrail25||distance>3||(headTrail25==2&&pixel/16!=position/16))return 0;return (uint8_t)(40/distance);}
 uint32_t local=localTickForTrack(track);
 uint32_t span=windowTickSpan()*(monoMeasureRowsView(track)?4u:1u);
 bool visible=local>=viewTickOffset&&(uint64_t)local<(uint64_t)viewTickOffset+span;
 return visible?100:50;
}
