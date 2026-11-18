/*
 * File:   timers.c
 * Author: Sarvesh Vettrivelan
 *
 * Created on October 22, 2026, 11:58 PM
 */


#include "config.h"
#include "events.h"
#include "timers.h"

/* Timer1: internal clock (TCS = 0), prescaler 1:256, period set by caller.
 * Left STOPPED (TON = 0); caller starts it. */
void T1_Config(uint16_t pr)
{
    T1CON = 0;
    T1CONbits.TCKPS = 3;                 // 1:256
    TMR1 = 0;
    PR1  = pr;
    IFS0bits.T1IF = 0;
    IEC0bits.T1IE = 1;
}

void T1_Start(void) { T1CONbits.TON = 1; }
void T1_Stop(void)  { T1CONbits.TON = 0; }
void T1_Reset(void) { TMR1 = 0; }

void T3_Start(uint16_t pr)
{
    T3CONbits.TON = 0;
    PR3 = pr;
    TMR3 = 0;
    IFS0bits.T3IF = 0;
    f_long = 0;
    T3CONbits.TON = 1;
}

void T3_Stop(void)
{
    T3CONbits.TON = 0;
    f_long = 0;
}

void Timers_Init(void)
{
    // T2: 20 ms debounce, one-shot (its ISR turns it off)
    T2CON = 0;
    T2CONbits.TCKPS = 1;                 // 1:8
    PR2 = T2_PR_20MS;
    TMR2 = 0;
    IFS0bits.T2IF = 0;
    IEC0bits.T2IE = 1;

    // T3: one-shot, 1:256 prescaler (period given in T3_Start)
    T3CON = 0;
    T3CONbits.TCKPS = 3;
    PR3 = T3_PR_3S;
    TMR3 = 0;
    IFS0bits.T3IF = 0;
    IEC0bits.T3IE = 1;

    T1_Config(T1_PR_1S);                 // T1 stays off until Timer_Start()
}

