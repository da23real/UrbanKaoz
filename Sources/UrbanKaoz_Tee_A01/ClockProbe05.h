#pragma once
// Temporary timing probe: RAM counters only, no printing in timed paths.
static volatile uint32_t clockProbeMax05=0,clockProbePc05=0,clockProbeIrq05=0;
__attribute__((noinline)) uint32_t clockProbeRead05(){
  uint32_t started=ARM_DWT_CYCCNT;
  uint32_t value=micros();
  uint32_t cycles=ARM_DWT_CYCCNT-started;
  if(cycles>clockProbeMax05){clockProbeMax05=cycles;clockProbePc05=(uint32_t)__builtin_return_address(0);clockProbeIrq05=SCB_ICSR&0x1FF;}
  return value;
}
#define micros clockProbeRead05
