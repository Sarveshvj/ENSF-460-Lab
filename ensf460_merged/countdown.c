/*
 * File:   countdown.c
 * Author: Sarvesh Vettrivelan
 *
 * Created on October 22, 2026, 8:55 AM
 */

#include "config.h"
#include "events.h"
#include "timers.h"
#include "display.h"
#include "buttons.h"
#include "countdown.h"

volatile state_t state   = ST_SET;
volatile uint8_t minutes = 0;
volatile uint8_t seconds = 0;

void Countdown_Init(void)
{
    LED1_TRIS = 0;  
    LED2_TRIS = 0;
    LED1_LAT  = 0;  
    LED2_LAT  = 0;
    state   = ST_SET;
    minutes = 0;
    seconds = 0;
}

/* 00:00 reached -> alarm */
static void finish(void)
{
    T1_Stop();
    state = ST_FIN;
    LED1_LAT = 1;                        // solid ON
    LED2_LAT = 0;
    T1_Config(T1_PR_100MS);              // re-use T1 as the rapid-blink timebase
    T1_Start();
    print_time("FIN", 0, 0, " - ALARM"); // ASCII hyphen instead of the en dash
}

void Countdown_Clear(void)
{
    T1_Stop();
    T3_Stop();
    f_tick = 0;
    LED1_LAT = 0;
    LED2_LAT = 0;
    minutes = 0;
    seconds = 0;
    T1_Config(T1_PR_1S);                 // back to the 1s configuration
    state = ST_SET;
    print_time("CLR", 0, 0, "");
}

void Countdown_TogglePause(void)
{
    if (state == ST_RUN) {
        T1_Stop();                       // TMR1 keeps its value -> resume is exact
        state = ST_PAUSED;
    } else if (state == ST_PAUSED) {
        state = ST_RUN;
        T1_Start();
    }
}

void Countdown_AckAlarm(void)
{
    T1_Stop();
    f_tick = 0;
    LED1_LAT = 0;
    LED2_LAT = 0;
    T1_Config(T1_PR_1S);                 // Timer1 was on its 100ms setting
    state = ST_SET;
}

void Timer_Start(void)
{
    if (minutes == 0 && seconds == 0) {  // corner case: nothing to count
        print_time("SET", 0, 0, "");     // stay in ST_SET
        return;
    }
    f_tick = 0;
    Buttons_IgnoreHeldPB3();             // a PB3 already held must not act
    T3_Stop();                           // T3 returns to long-press duty
    T1_Config(T1_PR_1S);
    LED1_LAT = 1;                        // blink starts with 1 s ON
    LED2_LAT = 0;
    state = ST_RUN;
    T1_Start();
    print_time("CNT", minutes, seconds, "");
}

void Countdown_OnTick(void)
{
    if (state == ST_RUN) {
        LED1_LAT ^= 1;                   // 1 s on / 1 s off
        if (seconds > 0) {
            seconds--;
        } else if (minutes > 0) {
            minutes--;
            seconds = 59;
        }
        if (minutes == 0 && seconds == 0) finish();
        else                              print_time("CNT", minutes, seconds, "");
    } else if (state == ST_FIN) {
        LED2_LAT ^= 1;                   // rapid blink every 100ms
    }
    // ST_PAUSED / ST_SET: stale tick, ignore
}
