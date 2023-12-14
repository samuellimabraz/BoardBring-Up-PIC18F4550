// -----------------------------------------------------------------------
//  * File: serial.c
//      Library for serial communication in PIC18F4550
//
//  * Author: Samuel Lima Braz
// ----------------------------------------------------------------------- 


#include "serial.h"
#include "pic18f4520.h"

void serialSend(unsigned char c) {
    while (!BitTst(PIR1, 4)); // Wait for the register to become available
    TXREG = c; // Place the value to be sent
}

void serialString(char msg[]) {
    int i;
    for (i = 0; msg[i] != '\0'; i++) {
        serialSend(msg[i]);
    }
}

// Displays the int number with n digits
void serialInt(int value, int n) {
    int i = 1;
    for (unsigned char j = 1; j < n; j++) {
        i *= 10;
    }
    if (value < 0) {
        value = value * (-1);
        serialSend('-');
    }
    while (i > 0) {
        serialSend((value / i) % 10 + 48);
        i /= 10;
    }
}

// Move the cursor back to the beginning of the line,
// to overwrite it with new contents
void serialClear(void) {
    serialString("                                                ");
    serialSend('\r');
}

unsigned char serialRead(void) {
    char resp = 0;

    if (BitTst(RCSTA, 1)) // Check for overrun error and reset the serial
    {
        BitClr(RCSTA, 4);
        BitSet(RCSTA, 4);
    }

    if (BitTst(PIR1, 5)) // Check if there is any value available
    {
        resp = RCREG; // Return the value
    }
    return resp; // Return zero
}

void serialInit(void) {
    TXSTA = 0b00101100; // Configure the serial data transmission
    RCSTA = 0b10010000; // Configure the serial data reception
    BAUDCON = 0b00001000; // Configure the serial speed system
    SPBRGH = 0b00000000; // Configure for 56k
    SPBRG = 0b00100010; // Configure for 56k
    BitSet(TRISC, 6); // Data reception pin
    BitSet(TRISC, 7); // Data transmission pin
    
    serialClear();
}
