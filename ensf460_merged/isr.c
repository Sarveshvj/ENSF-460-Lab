/*
 * File:   isr.c
 * Author: Sarvesh Vettrivelan
 *
 * Created on October 22, 2026, 11:59 PM
 */

#include "config.h"
#include "events.h"

volatile uint8_t f_tick = 0;
volatile uint8_t f_deb  = 0;
volatile uint8_t f_long = 0;

void __attribute__((interrupt, no_auto_psv)) _T1Interrupt(void)   // 1s while RUN, 100ms in FIN
{
    IFS0bits.T1IF = 0;
    f_tick = 1;
}

void __attribute__((interrupt, no_auto_psv)) _T2Interrupt(void)   // debounce done
{
    IFS0bits.T2IF = 0;
    T2CONbits.TON = 0;                                            // runs once per button
    f_deb = 1;
}

void __attribute__((interrupt, no_auto_psv)) _T3Interrupt(void)   // 3s long press 
{
    IFS0bits.T3IF = 0;
    T3CONbits.TON = 0;                                            // runs once per button
    f_long = 1;
}

void __attribute__((interrupt, no_auto_psv)) _CNInterrupt(void)
{
    IFS1bits.CNIF = 0;
    /* The first edge starts the 20 ms window; bounce edges inside it are
     * ignored. The button levels are sampled ONCE when the window ends */
    if (!T2CONbits.TON) {
        TMR2 = 0;
        IFS0bits.T2IF = 0;
        T2CONbits.TON = 1;
    }
}