/*
 * File:   TimeDelay.c
 * Author: user
 *
 * Created on September 27, 2026, 10:26 PM
 */


#include "TimeDelay.h"
volatile uint16_t ms_count = 0;

void delay_ms(uint16_t time_ms){
    if (time_ms == 0) return;
    
    //T1CON config
    T1CON = 0;
    T1CONbits.TCS = 0; // use internal clock
    T1CONbits.TCKPS = 0; // set prescalar
    T1CONbits.TSIDL = 0; // timer continues to run in Idle
    
    // Timer 1 interrupt config
    IPC0bits.T1IP = 3; // priority
    IFS0bits.T1IF = 0; // clear flag
    IEC0bits.T1IE = 1; // enable timer interrupt
    
    PR1 = 249;
    TMR1 = 0; // value starts from 0
    
    ms_count = 0;    
    T1CONbits.TON = 1; // turns the timer 1 ON
    
    while(ms_count < time_ms){
        
        Idle();
    }
    
    T1CONbits.TON = 0; // turns the timer 1 off
    IEC0bits.T1IE = 0;    // stop Timer1 from interrupting
    return;
    
}
