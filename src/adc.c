// -----------------------------------------------------------------------
//  * File: adc.c
//      Library of analog to digital converter for the pic18f4550
//
//  * Author: Samuel Lima Braz
// ----------------------------------------------------------------------- 

#include "pic18f4520.h"
#include "adc.h"

void adcInit(void) {
    BitSet(TRISA, 1); // Set RA1 as input
    BitSet(TRISA, 2); // Set RA2 as input
    BitSet(TRISE, 2); // Set RE2 as input
    ADCON0 = 0b00000001; // Set ADC module ON and channel 0 selected
    ADCON1 = 0b00001100; // Set RA2 and RA1 as analog inputs/Voltage reference
    ADCON2 = 0b10101010; // Configure ADC conversion clock and result format
}

unsigned int adcRead(unsigned char channel) {
    unsigned int convertedValue;
    
    if (channel == 1) {
        ADCON1 = 0b00001101; // Set RA1 as analog input
        ADCON0 = 0b00000101; // Select channel 1
    } else if (channel == 2) {
        ADCON1 = 0b00001100; // Set RA2 as analog input
        ADCON0 = 0b00001001; // Select channel 2
    } else if (channel == 3) {
        ADCON1 = 0b00000111; // Set RE2 as analog input
        ADCON0 = 0b00011101; // Select channel 3
    }

    ADCON0 |= 0b00000010; // Start conversion

    while (BitTst(ADCON0, 1)); // Wait for conversion to complete

    convertedValue = ADRESH; // Receive high byte of converted value
    convertedValue <<= 8; // Shift to higher position
    convertedValue += ADRESL; // Receive low byte of converted value

    ADCON1 = 0b00001111; // Set all pins as digital I/O

    return convertedValue; // Return the converted value
}


