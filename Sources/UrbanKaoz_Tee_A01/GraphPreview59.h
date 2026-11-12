#pragma once
// Monochrome 128x64 preview: two sampled traces plus note/start markers.
// No runtime modulation state is advanced by this diagnostic view.
FLASHMEM void graphPreview59(){
 uint8_t a[128],b[128];memset(a,255,sizeof(a));memset(b,255,sizeof(b));const char*name="NONE";int note=-1,start=-1;
 auto pixel=[](float v){return (uint8_t)clampInt((int)lroundf(60-v*48),12,60);};
 if((quickView903==4||quickView903==5)&&quickItem903<64){QuickContext903 context;quickObject903();auto m=mod806Unit(quickItem903);auto e=modExtra810[quickItem903];uint16_t k=quickItem903;
 e.attackCurve=lockDisplay817(LOCK_EXTRA817,k,0,e.attackCurve);e.decayCurve=lockDisplay817(LOCK_EXTRA817,k,1,e.decayCurve);e.releaseCurve=lockDisplay817(LOCK_EXTRA817,k,2,e.releaseCurve);e.phase=lockDisplay817(LOCK_EXTRA817,k,4,e.phase);
 m.attack=lockDisplay817(LOCK_MOD817,k,quickView903==5?2:1,m.attack);m.decay=lockDisplay817(LOCK_MOD817,k,quickView903==5?3:2,m.decay);m.sustain=lockDisplay817(LOCK_MOD817,k,quickView903==5?4:3,m.sustain);
 if(quickView903==4){name="ENV";m.release=lockDisplay817(LOCK_MOD817,k,4,m.release);int hold=lockDisplay817(LOCK_MOD817,k,0,holdBase50(k,m));float gate=max(1,hold+(int)m.attack+(int)m.decay)+max(1,(int)m.decay/2);float begin=min(0,hold),span=max(1.0f,gate+m.release-begin);note=(int)(-begin/span*127);
 for(int x=0;x<128;++x){float age=begin+x*span/127;float v=age<=gate?heldSigned50(age,m,e,hold):heldSigned50(gate,m,e,hold)*(1-curve810((age-gate)/max(1,(int)m.release),e.releaseCurve));a[x]=pixel(v);}
 }else{name="LFO";m.shape=lockDisplay817(LOCK_MOD817,k,1,m.shape);m.rate=lockDisplay817(LOCK_MOD817,k,0,m.rate);for(int x=0;x<128;++x){float p=fmodf(x/127.0f*1536/max(1,(int)m.rate)+e.phase/360.0f,1.0f),v=m.shape==1?(p<.5f?1:-1):m.shape==2?2*p-1:m.shape==3?1-2*p:m.shape==4?sinf(p*6.28318530718f):p<.5f?4*p-1:3-4*p;a[x]=pixel((v+1)*.5f);}}
 }else if(quickView903==2){name="ROLL";auto r=displayedRollForTrack(quickTrack903);auto rx=rollExtra1000(quickTrack903);note=64;start=64-(int)min(60.0f,r.start*60.0f/384);int32_t spacing=clampInt(r.speed,1,96),accel=r.accel*256;for(int x=0;x<128;++x){int repeat=x/8;a[x]=pixel(spacing/96.0f);b[x]=pixel(rollVelocity1000(r,rx,defaultVel[quickTrack903],repeat)/127.0f);if(x%8==7){rollAdvance60(spacing,accel,r.expo,repeat);}}}

 Serial.printf("GRAPH59,%s,%d,%d,",name,note,start);for(auto v:a)Serial.printf("%02X",v);Serial.print(',');for(auto v:b)Serial.printf("%02X",v);Serial.println();
}
