/* 
 * File:   PWM.h
 * Author: Sarvesh Vettrivelan
 */

#ifndef PWM_H
#define PWM_H

#include <xc.h>
#include <stdint.h>

// Global: current duty cycle 0-100%, read by UART to send to Python
extern volatile uint8_t dutyCycle;

void PWM_Init(void);             // Setup timer + LED pins
void PWM_SetDuty(uint8_t pct);   // Set duty cycle 0-100
void PWM_SetActiveLED(uint8_t led); // 1 = LED1 (RB9), 2 = LED2 (RA6)
void PWM_Stop(void);             // Turn off PWM, drive pin LOW

#endif /* PWM_H */
