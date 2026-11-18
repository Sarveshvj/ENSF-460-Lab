/*
 * File:   buttons.c
 * Author: Sarvesh Vettrivelan
 *
 * Created on October 22, 2026, 8:55 AM
 */


#include "config.h"
#include "events.h"
#include "timers.h"
#include "countdown.h"
#include "setmode.h"
#include "buttons.h"

static uint8_t pb3_prev     = 0;         // last debounced PB3 level
static uint8_t pb3_down     = 0;         // a PB3 press is in progress 
static uint8_t pb3_consumed = 0;         // press already acted on / cancelled  

void Buttons_Init(void)
{
    TRISBbits.TRISB7 = 1;                // PB1
    TRISAbits.TRISA4 = 1;                // PB2
    TRISBbits.TRISB4 = 1;                // PB3

    CNPU2bits.CN23PUE = 1;               // pull-ups: PB1
    CNPU1bits.CN0PUE  = 1;               //           PB2
    CNPU1bits.CN1PUE  = 1;               //           PB3

    CNEN2bits.CN23IE  = 1;               // CN interrupts on all three
    CNEN1bits.CN0IE   = 1;
    CNEN1bits.CN1IE   = 1;

    IFS1bits.CNIF = 0;
    IEC1bits.CNIE = 1;
}

void Buttons_IgnoreHeldPB3(void)
{
    pb3_consumed = 1;
}

void Buttons_OnDebounced(void)
{
    uint8_t pb1 = PB1_PRESSED();
    uint8_t pb2 = PB2_PRESSED();
    uint8_t pb3 = PB3_PRESSED();

    if (state == ST_FIN && (pb1 || pb2)) {   // PB1/PB2 while the alarm is on: the brief says setting
        Countdown_AckAlarm();                // a time clears the alarm, so stop it (state becomes ST_SET)
    }                                        // and let the set-time code below handle the press
    
    if (state == ST_SET) {               // set-time
        SetMode_OnButtons(pb1, pb2, pb3);
        pb3_prev = pb3;
        pb3_down = 0;
        return;
    }

    if (pb1 || pb2) {                    // combo while running: cancel PB3 gesture
        pb3_consumed = 1;
        T3_Stop();
        if (!pb3) pb3_down = 0;
        pb3_prev = pb3;
        return;
    }

    if (pb3 && !pb3_prev) {              // PB3 PRESS
        pb3_down = 1;
        pb3_consumed = 0;
        if (state == ST_FIN) {           // acknowledge the alarm immediately
            Countdown_Clear();
            pb3_consumed = 1;
        } else {                         // start the 3 s long-press measurement
            T3_Start(T3_PR_3S);
        }
    } else if (!pb3 && pb3_prev) {       // PB3 RELEASE
        T3_Stop();
        if (pb3_down && !pb3_consumed)   // released before 3 s -> short press
            Countdown_TogglePause();
        pb3_down = 0;
    }
    pb3_prev = pb3;
}

void Buttons_OnT3(void)
{
    if (state == ST_SET) {               // T3 belongs to the set-time repeat
        SetMode_OnRepeat();
        return;
    }
    if (pb3_down && !pb3_consumed && (state == ST_RUN || state == ST_PAUSED)) {
        Countdown_Clear();               // prints CLR 00m : 00s
        pb3_consumed = 1;                // the release must NOT also pause/resume
    }
}