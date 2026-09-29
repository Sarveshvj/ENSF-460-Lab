/*
 * File:   IOs.c
 * Author: user
 *
 * Created on September 27, 2026, 10:25 PM
 */

#include "IOs.h"
#include "TimeDelay.h"

#define PB1 (!PORTBbits.RB7)   // active low
#define PB2 (!PORTBbits.RB4)
#define PB3 (!PORTAbits.RA4)

void IOinit(void) {    
    AD1PCFG = 0xFFFF; /* keep this line as it sets I/O pins that can also be analog to be digital */
    
    TRISBbits.TRISB9 = 0;   // Set LED1(RB9) as output
    TRISAbits.TRISA6 = 0;   // Set LED2(RA6) as output
    
    TRISBbits.TRISB7 = 1;    // Set RB7 as input (PB1)
    TRISBbits.TRISB4 = 1;    // Set RB4 as input (PB2)
    TRISAbits.TRISA4 = 1;    // Set RA4 as input (PB3)
    
    CNPU2bits.CN23PUE = 1;   // Set RB7(PB1) as Pull-up
    CNPU1bits.CN1PUE = 1;    // Set RB4(PB2) as Pull-up
    CNPU1bits.CN0PUE = 1;    // Set RA4(PB3) as Pull-up

        
}


void IOcheck(void) {
    //When PB1 and PB2 are pressed together
    if (PB1 && PB2) {
        LED1 = 1; // LED1 turns ON
        delay_ms(1);
        LED1 = 0; // LED1 turns OFF
        delay_ms(1);
        
    }
    
    //When PB1 is pressed
    else if(PB1){
        LED1 = 1;   // LED turns on
        delay_ms(250);
        LED1 = 0;   // LED turns off  
        delay_ms(250);
    }
    
    //When PB2 is pressed
    else if(PB2){
        LED1 = 1;   // LED turns on
        delay_ms(1000);
        LED1 = 0;   // LED turns off  
        delay_ms(1000);
    }

    //When PB3 is pressed
    else if(PB3){
        LED1 = 1;   // LED turns on
        delay_ms(6000);
        LED1 = 0;   // LED turns off  
        delay_ms(6000);
    }

    // When no button is pressed
    else{
        LED1 = 0;                                // LED turns off     
    }

}