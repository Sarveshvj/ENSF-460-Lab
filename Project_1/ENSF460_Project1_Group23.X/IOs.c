

#include <xc.h>
#include <stdio.h>
#include "IOs.h"
#include "UART2.h"

#define PB1 (!PORTBbits.RB7)
#define PB2 (!PORTAbits.RA4)
#define PB3 (!PORTBbits.RB4)

#define RESET_HOLDTIME 30   // 30 ticks = 1.5s to hold for reset (PB1 + PB2)
#define LED1 LATBbits.LATB9 // LED 1

volatile unsigned int minutes = 0;
volatile unsigned int seconds = 0;
volatile unsigned char cnFlag = 0;
volatile unsigned char t2Flag = 0;
volatile unsigned char t1Ticks = 0; // ticks to be handled
volatile unsigned char running = 0; // is counter active

unsigned char button = 0;
unsigned int holdTime = 0;

void IOinit(void)
{

    AD1PCFG = 0xFFFF;

    // set buttons inputs
    TRISBbits.TRISB7 = 1;
    TRISAbits.TRISA4 = 1;
    TRISBbits.TRISB4 = 1;

    // pull-ups
    CNPU2bits.CN23PUE = 1;
    CNPU1bits.CN0PUE = 1;
    CNPU1bits.CN1PUE = 1;

    // CN interrupts
    CNEN2bits.CN23IE = 1;
    CNEN1bits.CN0IE = 1;
    CNEN1bits.CN1IE = 1;

    // LED1
    TRISBbits.TRISB9 = 0;

    (void)PORTA;
    (void)PORTB;

    IPC4bits.CNIP = 4;
    IFS1bits.CNIF = 0;
    IEC1bits.CNIE = 1;

    T1CON = 0;
    T1CONbits.TCKPS = 1;
    PR1 = 31249;
    TMR1 = 0;
    IPC0bits.T1IP = 2;
    IFS0bits.T1IF = 0;
    IEC0bits.T1IE = 1;

    T2CON = 0;
    T2CONbits.TCKPS = 1;
    PR2 = 1562;
    TMR2 = 0;
    IPC1bits.T2IP = 3;
    IFS0bits.T2IF = 0;
    IEC0bits.T2IE = 1;
}

void __attribute__((interrupt, no_auto_psv)) _T1Interrupt(void)
{
    IFS0bits.T1IF = 0;
    t1Ticks++;
}

void __attribute__((interrupt, no_auto_psv)) _CNInterrupt(void)
{
    IFS1bits.CNIF = 0;
    cnFlag = 1;
    TMR2 = 0;
    T2CONbits.TON = 1;
}

void __attribute__((interrupt, no_auto_psv)) _T2Interrupt(void)
{
    IFS0bits.T2IF = 0;
    t2Flag = 1;
}

static void showCount(const char *mode)
{
    char str[45];
    sprintf(str, "\r%s %02um : %02us                    ", mode, minutes, seconds);
    Disp2String(str);
}

void showTime(void)
{

    showCount("SET");
}

void startTimer(void)
{
    if (minutes == 0 && seconds == 0)
    { // no time
        return;
    }
    running = 1;
    LED1 = 1;
    t1Ticks = 0;
    TMR1 = 0;
    IFS0bits.T1IF = 0;
    T1CONbits.TON = 1;
    showCount("CNT");
}

void updateTimer(void)
{
    if (!running)
        return;

    if (seconds > 0)
    {
        seconds--;
    }
    else if (minutes > 0)
    {
        minutes--;
        seconds = 59;
    }
    if (minutes == 0 && seconds == 0)
    { // end of timer
        T1CONbits.TON = 0;
        running = 0;
        LED1 = 1;
        Disp2String("\rFIN 00m : 00s - ALARM        ");
    }
    else
    {
        LED1 = !LED1;
        showCount("CNT");
    }
}

void showGroupInfo(void)
{
    Disp2String("\r2026 ENSF460 L01 - Group 23                    ");
}

void IOcheck(void)
{
    if (running)
    { // ignore buttons while timer is on
        T2CONbits.TON = 0;
        return;
    }

    // All three PB pressed (show group info)
    if (PB1 && PB2 && PB3)
    {
        if (button != 6)
        {
            showGroupInfo();
            button = 6;
            holdTime = 0;
        }
    }

    // PB1 and PB2 (start / reset)
    else if (PB1 && PB2 && !PB3)
    {
        if (button == 0 || button == 1 || button == 2)
        {
            button = 5;
            holdTime = 0;
        }
    }

    // PB3 and PB1 (second decrement)
    else if (PB3 && PB1 && !PB2)
    {
        if (button == 0)
        {
            if (seconds == 0)
            {
                seconds = 59;
            }
            else
            {
                seconds--;
            }
            showTime();
            button = 3;
            holdTime = 0;
        }
    }

    // PB3 and PB2 (minute decrement)
    else if (PB3 && PB2 && !PB1)
    {
        if (button == 0)
        {
            if (minutes > 0)
            {
                minutes--;
            }
            showTime();
            button = 4;
            holdTime = 0;
        }
    }

    // PB1 is pressed
    else if (PB1 && !PB2 && !PB3)
    {

        if (button == 0)
        {
            LED1 = 0;

            seconds++;

            if (seconds >= 60)
            {
                seconds = 0;
            }

            showTime();
            button = 1;
            holdTime = 0;
        }
    }

    // PB2 is pressed
    else if (PB2 && !PB1 && !PB3)
    {

        if (button == 0)
        {
            LED1 = 0;

            if (minutes < 59)
            {
                minutes++;
                showTime();
            }

            button = 2;
            holdTime = 0;
        }
    }

    else if (PB3 && !PB1 && !PB2)
    { // Only PB3 held, stop repeats
        if (button >= 1 && button <= 5)
        {
            button = 0;
            holdTime = 0;
        }
    }

    // No button pressed
    else
    {
        if (!PB1 && !PB2 && !PB3)
        {
            if (button == 5 && holdTime < RESET_HOLDTIME)
            {
                startTimer(); // timer start
            }
            if (button == 6 && holdTime < RESET_HOLDTIME)
            {
                showTime();
            }
            button = 0;
            holdTime = 0;
            T2CONbits.TON = 0;
        }
    }
}

void IOhold(void)
{

    if (1 <= button && button <= 5)
    { // for combos that need repeats
        holdTime++;

        if (button == 5 && holdTime >= RESET_HOLDTIME)
        {
            minutes = 0;
            seconds = 0;
            showTime();
            button = 7; //  max button value, wait for reset
        }

        // Increase every 300ms
        if (holdTime % 6 == 0)
        {

            if (button == 1)
            {

                // PB1 increase a seconds
                if (holdTime >= 40)
                {
                    seconds += 5;
                }
                else
                {
                    seconds++;
                }

                if (seconds >= 60)
                {
                    seconds -= 60;
                }

                showTime();
            }

            else if (button == 2)
            {

                // PB2 increase a minutes
                if (minutes < 59)
                {
                    minutes++;
                    showTime();
                }
            }

            else if (button == 3)
            {                                                      // decrement seconds PB3 + PB1
                unsigned int decrement = (holdTime >= 40) ? 5 : 1; // decrement amount, -5 after ~2s
                seconds = (seconds + 60 - decrement) % 60;         // keeps within range 0-59
                showTime();
            }
            else if (button == 4)
            { // decrement minutes PB3 + PB2
                if (minutes > 0)
                {
                    minutes--;
                    showTime();
                }
            }
        }
    }
}