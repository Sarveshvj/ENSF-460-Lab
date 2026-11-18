/*
 * File:   main.c
 * Author: Sarvesh Vettrivelan
 *
 * Created on October 22, 2026, 8:55 AM
 */


// FBS
#pragma config BWRP = OFF               // Table Write Protect Boot (Boot segment may be written)
#pragma config BSS = OFF                // Boot segment Protect (No boot program Flash segment)

// FGS
#pragma config GWRP = OFF               // General Segment Code Flash Write Protection bit (General segment may be written)
#pragma config GCP = OFF                // General Segment Code Flash Code Protection bit (No protection)

// FOSCSEL
#pragma config FNOSC = FRC              // Oscillator Select (Fast RC oscillator (FRC))
#pragma config IESO = OFF               // Internal External Switch Over bit (Internal External Switchover mode disabled (Two-Speed Start-up disabled))

// FOSC
#pragma config POSCMOD = NONE           // Primary Oscillator Configuration bits (Primary oscillator disabled)
#pragma config OSCIOFNC = ON            // CLKO Enable Configuration bit (CLKO output disabled; pin functions as port I/O)
#pragma config POSCFREQ = HS            // Primary Oscillator Frequency Range Configuration bits (Primary oscillator/external clock input frequency greater than 8 MHz)
#pragma config SOSCSEL = SOSCHP         // SOSC Power Selection Configuration bits (Secondary oscillator configured for high-power operation)
#pragma config FCKSM = CSECMD           // Clock Switching and Monitor Selection (Clock switching is enabled, Fail-Safe Clock Monitor is disabled)

// FWDT
#pragma config WDTPS = PS32768          // Watchdog Timer Postscale Select bits (1:32,768)
#pragma config FWPSA = PR128            // WDT Prescaler (WDT prescaler ratio of 1:128)
#pragma config WINDIS = OFF             // Windowed Watchdog Timer Disable bit (Standard WDT selected; windowed WDT disabled)
#pragma config FWDTEN = OFF             // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))

// FPOR
#pragma config BOREN = BOR3             // Brown-out Reset Enable bits (Brown-out Reset enabled in hardware; SBOREN bit disabled)
#pragma config PWRTEN = ON              // Power-up Timer Enable bit (PWRT enabled)
#pragma config I2C1SEL = PRI            // Alternate I2C1 Pin Mapping bit (Default location for SCL1/SDA1 pins)
#pragma config BORV = V18               // Brown-out Reset Voltage bits (Brown-out Reset set to lowest voltage (1.8V))
#pragma config MCLRE = ON               // MCLR Pin Enable bit (MCLR pin enabled; RA5 input pin disabled)

// FICD
#pragma config ICS = PGx2               // ICD Pin Placement Select bits (PGC2/PGD2 are used for programming and debugging the device)

// FDS
#pragma config DSWDTPS = DSWDTPSF       // Deep Sleep Watchdog Timer Postscale Select bits (1:2,147,483,648 (25.7 Days))
#pragma config DSWDTOSC = LPRC          // DSWDT Reference Clock Select bit (DSWDT uses LPRC as reference clock)
#pragma config RTCOSC = SOSC            // RTCC Reference Clock Select bit (RTCC uses SOSC as reference clock)
#pragma config DSBOREN = ON             // Deep Sleep Zero-Power BOR Enable bit (Deep Sleep BOR enabled in Deep Sleep)
#pragma config DSWDTEN = ON             // Deep Sleep Watchdog Timer Enable bit (DSWDT enabled)

 /* ENSF 460 Project 1 - Count Down Timer
 * main.c - initialisation and the Idle() event loop. No application logic.
 *
 * Module map:
 *   config.h     pins, clock, timing constants
 *   isr.c        all ISRs (set flags only)          events.h
 *   timers.c     Timer1/2/3 configuration           timers.h
 *   display.c    UART terminal output               display.h
 *   buttons.c    CN setup, debounce, PB3 gestures   buttons.h
 *   countdown.c  RUN / PAUSED / FIN / CLEAR         countdown.h
 *   setmode.c    passes button events and the 50ms 
 *                repeat tick to the set-time code   setmode.h
 *   IOs.c        set-time code: PB1/PB2 increment, PB3+PB1/PB2
 *                decrement, hold-to-repeat, start, reset, group info   IOs.h */

#include "config.h"
#include "clkChange.h"                   
#include "events.h"
#include "timers.h"
#include "display.h"
#include "buttons.h"
#include "countdown.h"

int main(void)
{
    newClk(8);                           
    AD1PCFG = 0xFFFF;                    // all pins digital

    Countdown_Init();                    // LEDs, state = ST_SET 
    Display_Init();                      // UART2, 9600 8N1
    Timers_Init();
    Buttons_Init();                      // last: enables the CN interrupt

    print_time("SET", 0, 0, "");

    while (1) {
        Idle();                          // CPU sleeps until ANY interrupt

        // tick first, so a tick that raced with a pause is not lost
        if (f_tick) { f_tick = 0; Countdown_OnTick(); }
        if (f_deb)  { f_deb  = 0; Buttons_OnDebounced(); }
        if (f_long) { f_long = 0; Buttons_OnT3(); }
    }
    return 0;
}
