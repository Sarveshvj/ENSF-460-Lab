/* 
 * File: IOs.h
 * Author: Sarvesh Vettrivelan, Steven Wu, Paolo Abad
 * Revision history: 1.0
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef IOS_H
#define	IOS_H

#include <xc.h> // include processor files - each processor file is guarded.  
#define LED1 LATBbits.LATB9

// Function Declerations
void IOinit(void); 
void IOcheck(void);

#endif	/* IOS_H */

