/* 
 * File:   IOs.h
 * Author: Steven Wu
 *
 * Created on October 19, 2026, 11:33 PM
 */

#ifndef IOS_H
#define IOS_H

extern volatile unsigned int minutes;
extern volatile unsigned int seconds;

void IOinit(void);
void showTime(void);
void IOcheck(void);
void IOhold(void);

#endif