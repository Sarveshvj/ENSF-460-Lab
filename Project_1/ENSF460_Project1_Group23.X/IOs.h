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
extern volatile unsigned char cnFlag;
extern volatile unsigned char t2Flag;
extern volatile unsigned char t1Ticks;

void IOinit(void);
void showTime(void);
void startTimer(void);
void updateTimer(void);
void showGroupInfo(void);
void IOcheck(void);
void IOhold(void);

#endif