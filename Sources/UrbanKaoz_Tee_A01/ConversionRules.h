#pragma once
#include <stdint.h>
#include <string.h>
// No quantization: translate source ticks by the clip's position minus trim.
inline int64_t freeZoneTick(uint32_t position, uint16_t offset, int32_t sourceTick) {
  return (int64_t)position * 96 - (int64_t)offset * 96 + sourceTick;
}
inline bool songSliceContains(uint8_t track,uint64_t tick,uint8_t selected,uint64_t begin,uint64_t end){return track==selected&&tick>=begin&&tick<end;}
inline bool sdFolderPathValid(const char *p) {
  if (strncmp(p,"/URBANKAOZ",10) || (p[10] && p[10]!='/') || strlen(p)>40) return false;
  if (strstr(p,"..") || strstr(p,"//")) return false;
  for (const char *s=p;*s;++s) if (!((*s>='a'&&*s<='z')||(*s>='A'&&*s<='Z')||(*s>='0'&&*s<='9')||*s=='/'||*s=='_'||*s=='-'||*s==' ')) return false;
  return strlen(p)==10 || p[strlen(p)-1]!='/';
}
