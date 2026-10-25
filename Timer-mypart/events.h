#ifndef EVENTS_H
#define EVENTS_H

#include <stdint.h>

extern volatile uint8_t f_tick;   // Timer1 expired (1 s while RUN, 100 ms in FIN)
extern volatile uint8_t f_deb;    // Timer2: 20 ms debounce window elapsed
extern volatile uint8_t f_long;   // Timer3 expired (3 s long press)

#endif