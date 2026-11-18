/*
 * File:   main.c
 * Author: Sarvesh Vettrivelan
 *
 * Created on October 22, 2026, 8:55 AM
 */

#ifndef CONFIG_H
#define CONFIG_H

#include "xc.h"
#include <stdint.h>

// LED
#define LED1_LAT      LATBbits.LATB9
#define LED1_TRIS     TRISBbits.TRISB9
#define LED2_LAT      LATAbits.LATA6       
#define LED2_TRIS     TRISAbits.TRISA6      

// Push buttons: active-low, internal pull-ups
#define PB1_PRESSED() (PORTBbits.RB7 == 0)      // PB1 = RB7 = CN23
#define PB2_PRESSED() (PORTAbits.RA4 == 0)      // PB2 = RA4 = CN0 
#define PB3_PRESSED() (PORTBbits.RB4 == 0)      // PB3 = RB4 = CN1 

// Low power
#define Idle()  __asm__ volatile ("pwrsav #1")

// Timing constants (Fcy = 4 MHz)
#define T1_PR_1S      15624u                    // 4MHz/256 = 15625 - 1 = 15624 counts = 1 s (the 1 s countdown tick)
#define T1_PR_100MS   1561u                     // 4MHz/256 = (15625 * 0.1s) - 1 = 1561.5 counts = 100ms  (LED2 rapid blink) 
#define T2_PR_20MS    9999u                     // 4MHz/8 = (500kHz * 0.02) - 1 = 9999 counts = 20 ms  (button debounce window)
#define T3_PR_3S      46874u                    // 4MHz/256 = (15625 * 3) - 1 -> 46874 counts = 3.000 s  (PB3 long-press threshold)
#define T3_PR_50MS    780u                      // 4MHz/256 = (15625 * 0.050) - 1 -> 780 counts = 49.98 ms (set-time hold-to-repeat tick)
#define T3_PR_150MS   2343u                     // 4MHz/256 = (15625 * 0.150) - 1 -> 2343 counts = 150.0 ms (wait to see if another button joins)

#endif 
