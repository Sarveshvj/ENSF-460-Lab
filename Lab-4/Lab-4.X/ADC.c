/*
 * File:   ADC.c
 * Author: user
 *
 * Created on November 2, 2026, 4:50 PM
 */


#include "xc.h"

uint16_t do_ADC(void){
    uint16_t ADCvalue;
    
    TRISAbits.TRISA2 = 1;        // Input
    AD1PCFGbits.PCFG4 = 0;       // AN4 analog
    
    // ADC configuration
    AD1CON1 = 0;
    AD1CON2 = 0;
    AD1CON3 = 0;
     
    AD1CON2bits.VCFG = 0;        // AVDD AVSS references
    AD1CON3bits.ADRC = 0;        // System clock
    AD1CON3bits.ADCS = 2;        // ADC clock
    AD1CON3bits.SAMC = 10;       // Sample time

    AD1CON1bits.FORM = 0;        // Integer output
    AD1CON1bits.SSRC = 7;        // Auto conversion

    AD1CHSbits.CH0SA = 4;        // Select AN4
    
    
    AD1CON1bits.ADON = 1;
    AD1CON1bits.SAMP=1;
    
    while(AD1CON1bits.DONE==0){}
    
    ADCvalue = ADC1BUF0;
    
    AD1CON1bits.ADON=0;
    
    return (ADCvalue);    
}
