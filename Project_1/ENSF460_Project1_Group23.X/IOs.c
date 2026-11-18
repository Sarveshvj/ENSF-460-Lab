

#include <xc.h>
#include <stdio.h>
#include "IOs.h"
#include "UART2.h"

#define PB1 (!PORTBbits.RB7)
#define PB2 (!PORTAbits.RA4)
#define PB3 (!PORTBbits.RB4)

#define REPEAT_TICKS 6  // 6 x 50 ms = 300 ms
#define FAST_TICKS 40   // 40 x 50 ms = 2 seconds

volatile unsigned int minutes = 0;
volatile unsigned int seconds = 0;

unsigned char button = 0;
unsigned int holdTime = 0;

void IOinit(void) {

    AD1PCFG = 0xFFFF;

    //set buttons inputs
    TRISBbits.TRISB7 = 1;
    TRISAbits.TRISA4 = 1;
    TRISBbits.TRISB4 = 1;

    //pull-ups
    CNPU2bits.CN23PUE = 1;
    CNPU1bits.CN0PUE = 1;
    CNPU1bits.CN1PUE = 1;

    //CN interrupts
    CNEN2bits.CN23IE = 1;
    CNEN1bits.CN0IE = 1;
    CNEN1bits.CN1IE = 1;

    (void)PORTA;
    (void)PORTB;

    IPC4bits.CNIP = 4;
    IFS1bits.CNIF = 0;
    IEC1bits.CNIE = 1;
}


void showTime(void) {

    char str[45];

    sprintf(str, "\rSET %02um : %02us                    ",minutes, seconds);

    Disp2String(str);
}

void IOcheck(void) {
    
    if (!PB1 && !PB2 && !PB3) {
        button = 0;
        holdTime = 0;
        T2CONbits.TON = 0; 
    }
    
    
    //PB1 is pressed
    else if(PB1 && !PB2 && !PB3) {

        if (button == 0) {
            seconds++;
            if (seconds >= 60){
                seconds = 0;
            }
            showTime();
            button = 1;
            holdTime = 0;
        }
        else if (button != 1) {
            button = 3;
            holdTime = 0;
            T2CONbits.TON = 0;
        }
    }

    //PB2 is pressed
    else if(PB2 && !PB1 && !PB3) {
        if (button == 0) {
            if (minutes < 59) {
                minutes++;
                showTime();
            }
            button = 2;
            holdTime = 0;
        }
        else if (button != 2) {
            button = 3; // another button was already held
            holdTime = 0;
            T2CONbits.TON = 0;
        }
        
    }

    //No button pressed
    else {
         // other code goes here-----------------------------------//
        button = 0;
        holdTime = 0;
        T2CONbits.TON = 0;
    }
}

void IOhold(void) {

    if (button != 1 && button != 2) return;

    holdTime++;
    if (holdTime % REPEAT_TICKS != 0) return;

    if (button == 1) {
        if (holdTime >= FAST_TICKS) seconds += 5;
        else seconds++;

        if (seconds >= 60) seconds -= 60;
        showTime();
    }
    else if (button == 2) {
        if (minutes < 59) {
            minutes++;
            showTime();
        }
    }
}