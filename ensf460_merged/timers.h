#ifndef TIMERS_H
#define TIMERS_H

#include <stdint.h>

void Timers_Init(void);          // configures T1/T2/T3, all left stopped

void T1_Config(uint16_t pr);     // Changes Timer1 PR1 while the timer is stopped.
void T1_Start(void);             // TMR1 is NOT cleared: resume is exact
void T1_Stop(void);              // TMR1 keeps its value (pause)
void T1_Reset(void);             // TMR1 is cleared (TMR1 = 0)

void T3_Start(uint16_t pr);      // restart one-shot with new PR3
void T3_Stop(void);              // stop and drop any pending f_long

#endif /* TIMERS_H */