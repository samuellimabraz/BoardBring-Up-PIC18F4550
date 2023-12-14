// -----------------------------------------------------------------------
//  * File: pwm.c
//      Library for PIC18F4550 PWM outputs access
//
//  * Author: Samuel Lima Braz
// ----------------------------------------------------------------------- 

#include "pic18f4520.h"
#include "pwm.h"

void pwmInit(void) {
    BitClr(TRISB, 3);        // Configure RB3 as output
    T2CON |= 0b00000011;    // Configure timer 2 prescaler as 1:16
    BitSet(T2CON, 2);       // Enable timer 2
    CCP2CON |= 0b00001100;  // Configure CCP2 as PWM
}

void pwmSet(unsigned char percent) {
    unsigned int value = ((unsigned int)percent) * (PR2 + 1);
    value /= 25;
    // Ensure it has only 10 bits
    value &= 0x03FF;
    // The first 8 bits are placed in CCPR2L
    CCPR2L = value >> 2;
    // The last two bits are placed at position 5 and 4 of CCP2CON
    CCP2CON |= (value & 0x0003) << 4;
}

void pwmFrequency(unsigned int freq) {
    PR2 = (125000 / freq) - 1;
}


