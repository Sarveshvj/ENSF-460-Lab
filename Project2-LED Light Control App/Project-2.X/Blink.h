/* 
 * File:   Blink.h
 * Author: Sarvesh Vettrivelan
 */

#ifndef BLINK_H
#define BLINK_H

#include <xc.h>
#include <stdint.h>

void Blink_Init(void);           // Setup Timer3 for 500ms
void Blink_Start(uint8_t mode);  // 1 = ON MODE blink, 2 = OFF MODE blink
void Blink_Stop(void);           // Stop blinking

#endif /* BLINK_H */
