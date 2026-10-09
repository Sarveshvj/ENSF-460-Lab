/*
 * File:   IOs.c
 * Author: Sarvesh Vettrivelan, Steven Wu, Paolo Abad
 *
 * Created on October 6, 2026, 10:25 PM
 */

#include "IOs.h"
#include "UART2.h"

// Timer3 at 1:64 -> 3906.25 Hz
#define T3_025s 977
#define T3_1s   3906
#define T3_3s   11719

#define PB1 (!PORTBbits.RB7)   // active low
#define PB2 (!PORTBbits.RB4)
#define PB3 (!PORTAbits.RA4)

#define LED_OFF 0
#define LED_ON  1

static uint8_t lastState = 0xFF;   // forces the first IOcheck() to print

void IOinit(void) {    
    AD1PCFG = 0xFFFF; /* keep this line as it sets I/O pins that can also be analog to be digital */
    
    TRISBbits.TRISB9 = 0;   // Set LED1(RB9) as output
    LED1 = 0;               // Starts off
    
    TRISBbits.TRISB7 = 1;    // Set RB7 as input (PB1)
    TRISBbits.TRISB4 = 1;    // Set RB4 as input (PB2)
    TRISAbits.TRISA4 = 1;    // Set RA4 as input (PB3)
    
    CNPU2bits.CN23PUE = 1;   // Set RB7(PB1) as Pull-up
    CNPU1bits.CN1PUE = 1;    // Set RB4(PB2) as Pull-up
    CNPU1bits.CN0PUE = 1;    // Set RA4(PB3) as Pull-up
    
    CNEN1bits.CN0IE = 1;     // RA4 may trigger CN interrupt
    CNEN1bits.CN1IE = 1;     // RB4 may trigger CN interrupt
    CNEN2bits.CN23IE = 1;    // RB7 may trigger CN interrupt

    IPC4bits.CNIP = 6;       // priority for interrupt
    IFS1bits.CNIF = 0;       // Flag for CN
    IEC1bits.CNIE = 1;       // Enable CN to reach CPU
}

static void led(uint16_t period) {
    T3CONbits.TON = 0;
    
    if(period == LED_ON || period == LED_OFF){
        LED1 = period;
    }
    else {
        PR3 = period;
        TMR3 = 0;
        IFS0bits.T3IF = 0;
        LED1 = 1;
        T3CONbits.TON = 1;   
    }

}

static void displayState(char *msg)
{
    Disp2String("\r                           \r");
    Disp2String(msg);
}

void IOcheck(void) {
    uint8_t s = (PB1 ? 1:0) | (PB2 ? 2 : 0) | (PB3 ? 4:0);
    
    if(s == lastState) return;
    lastState = s;
    
    switch (s) {
        case 0: led(LED_OFF);        displayState("Nothing pressed"); break;
        case 1: led(T3_025s);        displayState("PB1 is pressed"); break;
        case 2: led(T3_1s);          displayState("PB2 is pressed"); break;
        case 4: led(T3_3s);          displayState("PB3 is pressed"); break;
        case 3: led(LED_ON);         displayState("PB1 and PB2 are pressed"); break;
        case 5: led(LED_ON);         displayState("PB1 and PB3 are pressed"); break;
        case 6: led(LED_ON);         displayState("PB3 and PB2 are pressed"); break;
        case 7: led(LED_ON);         displayState("All PBs pressed"); break;
    }        
}