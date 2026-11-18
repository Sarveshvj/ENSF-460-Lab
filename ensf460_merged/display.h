#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>

void Display_Init(void);   /* wraps your lab's InitUART2(): 9600 8N1 at 8 MHz (U2BRG = 103 in your driver) */

/* Prints "\r<tag> MMm : SSs<tail>" padded with spaces, e.g.
 *   print_time("CNT", 5, 8, "")           -> CNT 05m : 08s
 *   print_time("FIN", 0, 0, " - ALARM")   -> FIN 00m : 00s - ALARM
 * '\r' returns to the start of the SAME line, so no new line is ever sent. */
void print_time(const char *tag, uint8_t m, uint8_t s, const char *tail);

/* Prints a raw single-line message (e.g. the group-info line). */
void print_line(const char *text);

#endif /* DISPLAY_H */