/*
 * File:   setmode.c
 * Author: user
 *
 * Created on October 23, 2026, 11:59 PM
 */

#include "config.h"
#include "timers.h"
#include "countdown.h"
#include "setmode.h"
#include "IOs.h"

static uint8_t pend = 0;                 /* a new press is waiting to see if more buttons join */
static uint8_t g1 = 0, g2 = 0, g3 = 0;   /* the button levels of that waiting press */

/* Let the teammates' code act on these levels, then keep the 50 ms repeat tick
 * in step with what it decided (running while a repeating button is held). */
static void process(uint8_t pb1, uint8_t pb2, uint8_t pb3)
{
    IOsetButtons(pb1, pb2, pb3);
    IOcheck();                           /* may call Timer_Start() */

    if (state == ST_SET && button >= 1 && button <= 5)
        T3_Start(T3_PR_50MS);
    else
        T3_Stop();                       /* nothing to repeat, or the countdown has started */
}

void SetMode_OnButtons(uint8_t pb1, uint8_t pb2, uint8_t pb3)
{
    uint8_t n = pb1 + pb2 + pb3;         /* how many buttons are down */

    if (pend) {                          /* the buttons changed during the wait */
        if (n == 0) {                    /* all released inside the wait: it was a tap */
            pend = 0;
            T3_Stop();
            process(g1, g2, g3);         /* do the press ... */
            process(0, 0, 0);            /* ... and then the release */
        } else if (n == 3) {             /* all three are down: nothing more can join */
            pend = 0;
            T3_Stop();
            process(pb1, pb2, pb3);
        } else {                         /* the combination changed: wait again */
            g1 = pb1; g2 = pb2; g3 = pb3;
            T3_Start(T3_PR_150MS);
        }
        return;
    }

    if (button == 0 && (n == 1 || n == 2)) {   /* a new press: wait to see if more buttons join */
        pend = 1;
        g1 = pb1; g2 = pb2; g3 = pb3;
        T3_Start(T3_PR_150MS);
        return;
    }

    process(pb1, pb2, pb3);
}

void SetMode_OnRepeat(void)
{
    if (pend) {                          /* the wait is over and nothing changed */
        pend = 0;
        process(g1, g2, g3);             /* act on the lone press now */
        return;
    }

    IOhold();                            /* teammates' code: one 50 ms step */
    if (state == ST_SET && button >= 1 && button <= 5)
        T3_Start(T3_PR_50MS);            /* still held: schedule the next step */
}

