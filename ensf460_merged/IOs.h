/* 
 * File: IOs.h
 * Author: Sarvesh Vettrivelan, Steven Wu, Paolo Abad
 * Revision history: 1.0
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef IOS_H
#define	IOS_H

/* Set-time code (teammates). Called from setmode.c while state == ST_SET.
 * minutes, seconds and state are declared in countdown.h (defined in countdown.c).
 * The countdown, the ISRs, the timers and IOinit() of the old IOs.c are gone:
 * they now live in countdown.c, isr.c, timers.c and buttons.c. */

void IOsetButtons(unsigned char pb1, unsigned char pb2, unsigned char pb3);  // levels IOcheck() will use, 1 = pressed
void showTime(void);            // prints the SET line 
void showGroupInfo(void);       // prints the group line (all three buttons held)  
void IOcheck(void);             // one debounced change of PB1/PB2/PB3  
void IOhold(void);              // one 50 ms hold step (Timer3 tick): repeat, reset

extern unsigned char button;     /* 0 none, 1 PB1, 2 PB2, 3 PB3+PB1, 4 PB3+PB2,
                                    5 PB1+PB2, 6 all three, 7 reset done*/
extern unsigned int  holdTime;   // number of 50 ms ticks the button has been held

#endif	/* IOS_H */

