/*
 * File:   display.c
 * Author: Sarvesh Vettrivelan
 *
 * Created on October 22, 2026, 12:00 AM
 */

#include "display.h"
#include "UART2.h"

void Display_Init(void)
{
    InitUART2();
}

void print_time(const char *tag, uint8_t m, uint8_t s, const char *tail)
{
    char buf[48];
    uint8_t i = 0;

    buf[i++] = '\r';
    while (*tag)  buf[i++] = *tag++;
    buf[i++] = ' ';
    buf[i++] = '0' + (m / 10);
    buf[i++] = '0' + (m % 10);
    buf[i++] = 'm';  buf[i++] = ' ';  buf[i++] = ':';  buf[i++] = ' ';
    buf[i++] = '0' + (s / 10);
    buf[i++] = '0' + (s % 10);
    buf[i++] = 's';
    while (*tail) buf[i++] = *tail++;
    while (i < 40) buf[i++] = ' ';       /* pad so shorter text overwrites longer (also the group-info line) */
    buf[i] = '\0';
    Disp2String(buf);
}

void print_line(const char *text)
{
    char buf[60];
    uint8_t i = 0;

    buf[i++] = '\r';
    while (*text && i < 50) buf[i++] = *text++;
    while (i < 40) buf[i++] = ' ';
    buf[i] = '\0';
    Disp2String(buf);
}
