#pragma once
uint8_t stepRaw25(uint8_t p){uint8_t x=p%4,y=p/4;switch(stepLayout25){case 1:return x*4+y;case 2:return (3-y)*4+x;case 3:return (3-x)*4+y;default:return p;}}
uint8_t stepLogical25(uint8_t p){for(uint8_t i=0;i<16;++i)if(stepRaw25(i)==p)return i;return p;}
void stepPixel25(uint8_t n,uint8_t p,uint32_t c){neoPixel821(n,stepRaw25(p),c);}
