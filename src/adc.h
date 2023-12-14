// -----------------------------------------------------------------------
//  * File: adc.h
//      Header of library of analog to digital converter for the pic18f4550
//
//  * Author: Samuel Lima Braz
// ----------------------------------------------------------------------- 

#ifndef	ADC_H
    #define ADC_H
    
    // Main functions of digital analog converter
    void adcInit(void);
    unsigned int adcRead(unsigned char saida);
    
#endif