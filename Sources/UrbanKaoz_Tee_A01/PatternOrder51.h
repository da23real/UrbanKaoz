#pragma once
FLASHMEM void orderReset51(){for(uint8_t t=0;t<16;++t)for(uint8_t p=0;p<16;++p)patternOrder51[t][p]=p;orderReady51=true;}
FLASHMEM uint8_t patternAtRow51(uint8_t t,uint8_t row){if(!orderReady51)orderReset51();return t<16&&row<16?patternOrder51[t][row]:row;}
FLASHMEM uint8_t patternRow51(uint8_t t,uint8_t p){if(!orderReady51)orderReset51();for(uint8_t row=0;row<16;++row)if(patternOrder51[t][row]==p)return row;return p;}
