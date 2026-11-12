#pragma once
#include <stdint.h>
namespace scrmap {
static const uint8_t address[4]={0x27,0x26,0x25,0x23};
inline bool valid(const uint8_t *r){for(unsigned i=0;i<4;++i)if(r[i]>4)return false;return true;}
inline bool duplicate(const uint8_t *r,unsigned i){if(i>=4||!r[i])return false;for(unsigned j=0;j<4;++j)if(j!=i&&r[i]==r[j])return true;return false;}
inline bool anyDuplicate(const uint8_t *r){for(unsigned i=0;i<4;++i)if(duplicate(r,i))return true;return false;}
inline uint8_t checksum(const uint8_t *r){uint8_t c=0xB7;for(unsigned i=0;i<4;++i)c=(uint8_t)(c*33+r[i]);return c;}
inline bool parse(const char *s,uint8_t *out){uint8_t v[4];for(unsigned i=0;i<4;++i){if(*s<'0'||*s>'4')return false;v[i]=*s++-'0';if(i<3){if(*s++!=',')return false;}else if(*s)return false;}for(unsigned i=0;i<4;++i)out[i]=v[i];return true;}
}
