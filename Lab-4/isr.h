/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef ISR_H
#define	ISR_H

#include <xc.h>

#include <stdint.h>

#define MODE0 0
#define MODE1 1

extern volatile uint8_t state;
void initCN(void);

#endif	/* ISR_H */

