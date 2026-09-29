/* 
 * File: TimeDelay.h  
 * Author: 
 * Comments:
 * Revision history: 1.0
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef TIMERDELAY_H
#define	TIMERDELAY_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h>


// Function Declerations
void delay_ms(uint16_t time_ms);



#endif	/* TIMERDELAY_H */

