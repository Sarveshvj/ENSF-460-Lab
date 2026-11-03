/*
 * File:   isr.c
 * Author: Sarvesh Vettrivelan
 *
 * Created on October 22, 2026, 11:59 PM
 */

#include "isr.h"

volatile uint8_t state = MODE0;

// ISR.c
void initCN(void) {
    TRISBbits.TRISB7 = 1;      // RB7 as input
    CNPU2bits.CN23PUE = 1;     // pull-up on CN23
    CNEN2bits.CN23IE = 1;      // RB7 may trigger CN interrupt
    
    IPC4bits.CNIP = 6;         // priority for interrupt
    IFS1bits.CNIF = 0;         // clear flag
    IEC1bits.CNIE = 1;         // enable CN interrupt
}


void __attribute__((interrupt, no_auto_psv)) _CNInterrupt(void){
    __delay_ms(20);   // wait for bounce to settle
    if (PORTBbits.RB7 == 0) {  // confirm still pressed
        // wait for release
        while(PORTBbits.RB7 == 0){}
        __delay_ms(20);  // debounce release

        // now toggle state
        if (state == MODE0) state = MODE1;
        else state = MODE0;
    }
    IFS1bits.CNIF = 0;
}