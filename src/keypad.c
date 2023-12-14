// -----------------------------------------------------------------------
//  * File: keypad.c
//      Library for Matrix keyboard operation
//
//  * Author: Samuel Lima Braz
// ----------------------------------------------------------------------- 


#include "keypad.h"
#include "pic18f4520.h"

static unsigned char value = 0x00;

unsigned char kpRead(void) {
    return value;
}

// Debounce for keypad buttons
void kpDebounce(void) {
    unsigned char i;
    static unsigned char time = 0;
    static unsigned char newValue = 0x00;
    static unsigned char oldValue = 0x00;

    value = 0x00;

    for (i = 0; i < 4; i++) {
        if (BitTst(PORTD, (i + 4))) {
            BitSet(newValue, i);
        } else {
            BitClr(newValue, i);
        }
    }

    if (BitTst(PORTB, 2)) {
        BitSet(newValue, 4);
    } else {
        BitClr(newValue, 4);
    }

    if (oldValue == newValue) {
        time--;
    } else {
        time = 1;
        oldValue = newValue;
    }
    if (time == 0) {
        value = oldValue;
    }
}

void kpInit(void) {
    TRISD = 0xF0; // Configure RD4 to RD7 as inputs
    TRISB = 0x04; // Configure RB2 as input
    BitSet(INTCON2, 7); //  All PORTB pull-ups are disabled
    ADCON1 = 0b00001111; // Configure all RB pins as digital inputs

#ifdef PIC18F4550
    SPPCFG = 0x00; // SFR not present on PIC18F4520
#endif
}

