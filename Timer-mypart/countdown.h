#ifndef COUNTDOWN_H
#define COUNTDOWN_H

#include <stdint.h>

typedef enum { ST_SET = 0, ST_RUN, ST_PAUSED, ST_FIN } state_t;

/* Shared with the set-time code: it edits minutes/seconds while ST_SET. */
extern volatile state_t state;
extern volatile uint8_t minutes;
extern volatile uint8_t seconds;

void Countdown_Init(void);           // LED pins, state = ST_SET, 00:00         
void Timer_Start(void);              // set-time code calls this on PB1+PB2 tap 
void Countdown_OnTick(void);         // main() calls this when f_tick is set   
void Countdown_TogglePause(void);    // short PB3 press                        
void Countdown_Clear(void);          // long PB3 press, or PB3 in ST_FIN       

#endif /* COUNTDOWN_H */