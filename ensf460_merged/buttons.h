
#ifndef BUTTONS_H
#define BUTTONS_H

void Buttons_Init(void);            // pins, pull-ups, CN interrupts
void Buttons_OnDebounced(void);     // main(): f_deb set (20 ms after an edge)
void Buttons_OnT3(void);            /* main(): f_long set (3 s long press, or
                                       the set-time auto-repeat while ST_SET)*/
void Buttons_IgnoreHeldPB3(void);   /* a PB3 press already in progress is
                                       ignored (called when the timer starts)  */

#endif /* BUTTONS_H */