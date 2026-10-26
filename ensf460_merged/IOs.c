/*
 * File:   IOs.c
 * Author: Sarvesh Vettrivelan, Steven Wu, Paolo Abad
 *
 * Created on October 6, 2026, 10:25 PM
 */

#include <xc.h>
#include "IOs.h"
#include "display.h"     // print_time(), print_line()
#include "countdown.h"   // minutes, seconds, Timer_Start()

/* The button levels IOcheck() acts on. setmode.c supplies them with
 * IOsetButtons(); 1 means pressed. (These replace the three
 * (!PORTxbits.Rxx) macros, so the code below is unchanged.) */
static unsigned char in1 = 0, in2 = 0, in3 = 0;
#define PB1 (in1)
#define PB2 (in2)
#define PB3 (in3)

#define RESET_HOLDTIME 30   // 30 ticks = 1.5s to hold for reset (PB1 + PB2)

/* MERGE NOTES (what changed):
 *  1. minutes and seconds are no longer defined here. They are defined once in
 *     countdown.c, because the countdown uses the same two variables.
 *  2. cnFlag, t2Flag, t1Ticks and running are gone, and so are the three ISRs,
 *     IOinit(), startTimer() and updateTimer(). Their jobs are done by isr.c,
 *     timers.c, buttons.c and countdown.c. Having two _CNInterrupt or two
 *     _T1Interrupt functions would not link.
 *  3. showTime() and showGroupInfo() print through display.c, which keeps every
 *     message on one line and pads it so a shorter message erases a longer one.
 *  4. startTimer() is now Timer_Start().
 *  5. T2CONbits.TON = 0 (which stopped the old repeat timer) is gone. The 50 ms
 *     repeat tick is Timer3, started and stopped in setmode.c.
 *  6. PB1, PB2 and PB3 no longer read PORTA/PORTB. They use the levels given by
 *     IOsetButtons(), so setmode.c can wait a short time to see whether another
 *     button joins before IOcheck() acts (pressing all three buttons must not
 *     first add a second).
 *  7. "if (running)" and the two "LED1 = 0" lines are gone. IOcheck() is only
 *     called while the program is in set mode, and the LEDs are already off
 *     there (the alarm is stopped before this code sees a PB1/PB2 press).
 */

unsigned char button = 0;
unsigned int holdTime = 0;

void IOsetButtons(unsigned char pb1, unsigned char pb2, unsigned char pb3)
{
    in1 = pb1;
    in2 = pb2;
    in3 = pb3;
}

void showTime(void)
{
    print_time("SET", minutes, seconds, "");
}

void showGroupInfo(void)
{
    print_line("2026 ENSF 460 L01 - Group 23");
}

void IOcheck(void)
{
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
                Timer_Start(); // timer start (stays in set mode if the time is 00:00)
            }
            if (button == 6 && holdTime < RESET_HOLDTIME)
            {
                showTime();
            }
            button = 0;
            holdTime = 0;
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
