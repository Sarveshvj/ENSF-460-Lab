

#include <xc.h>
#include "IOs.h"

#define PB1 (!PORTBbits.RB7)
#define PB2 (!PORTAbits.RA4)
#define PB3 (!PORTBbits.RB4)


void IOinit(void) {

    AD1PCFG = 0xFFFF; 

    // Set push buttons inputs
    TRISBbits.TRISB7 = 1; // PB1
    TRISAbits.TRISA4 = 1; // PB2
    TRISBbits.TRISB4 = 1; // PB3

    //  pull-up 
    CNPU2bits.CN23PUE = 1; // PB1
    CNPU1bits.CN0PUE = 1;  // PB2
    CNPU1bits.CN1PUE = 1;  // PB3

}
