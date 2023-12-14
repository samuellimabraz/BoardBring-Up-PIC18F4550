// -----------------------------------------------------------------------
//  * File: lcd.c
//      Library for using lcd 1602A
//
//  * Author: Samuel Lima Braz
// ----------------------------------------------------------------------- 

#include "pic18f4520.h"
#include "delay.h"
#include "lcd.h"

#define RS 1
#define EN 2

void lcdInit(void) {
    BitClr(TRISC, RS); // Set RC1 as output
    BitClr(TRISC, EN); // Set RC2 as output
    TRISD = 0x00; // Set PORTD as output
    delay_ms(5);

    BitClr(PORTD, 0);
    BitClr(PORTD, 1);
    BitClr(PORTD, 2);
    BitClr(PORTD, 3);

    lcdCommand(0x33);
    lcdCommand(0x32);
    lcdCommand(0x28); // 4-bit mode, 2 lines, 5x8 font
    lcdCommand(0x06); // Set entry mode: left to right, no shift
    lcdCommand(0x0E); // Turn on display, cursor, and no blink
    lcdCommand(0x01); // Clear the LCD
    delay_ms(5);
}

// Enable pulse
void pulseEnable() {
    BitSet(PORTC, EN);
    delay_ms(3);
    BitClr(PORTC, EN);
    delay_ms(3);
}

void lcdCommand(unsigned char command) {
    BitClr(PORTC, RS); // Set command mode
    PORTD = command >> 4; // Send high nibble of command to PORTD
    pulseEnable();
    PORTD = command & 0x0F; // Send low nibble of command to PORTD
    pulseEnable();
    if (command == 0x01) {
        delay_ms(4);
    } else {
        delay_ms(1);
    }
}

// Display a character in ldc
void lcdData(unsigned char character) {
    BitSet(PORTC, RS); // Set data mode
    PORTD = character >> 4; // Send high nibble of character to PORTD
    pulseEnable();
    PORTD = character & 0x0F; // Send low nibble of character to PORTD
    pulseEnable();

    for (volatile int i = 0; i < 150; i++);
}

// Display the string in 16x2 screen
void lcdString(const char *msg) {
    int i;

    for (i = 0; msg[i] != '\0'; i++) {
        if (i == 16)
            lcdCommand(0xC0);
        if (i == 32)
            lcdCommand(0x02);
        lcdData(msg[i]);
    }
}

// Places the cursor at the indicated position
void lcdPosition(unsigned char line, unsigned char column) {
    if (line == 0) 
        lcdCommand(0x80 + (column % 16));
    if (line == 1) 
        lcdCommand(0xC0 + (column % 16));
}

// Displays the int number with n digits
void lcdInt(int value, int n) {
    int i = 1;
    for (unsigned char j = 1; j < n; j++) {
        i *= 10;
    }
    if (value < 0) {
        value = value * (-1);
        lcdData('-');
    }
    while (i > 0) {
        lcdData((value / i) % 10 + 48);
        i /= 10;
    }
}

// Create a symbol in LCD
void CreateCustomSymbol(unsigned char symbolIndex, const unsigned char *symbolData) {
    // Calculate the memory position for the custom symbol
    // Memory in the range 0 to 7 and offset by 8 bits
    unsigned char symbolAddress = (symbolIndex & 0x07) * 8;

    // Send the command to set the CGRAM address
    lcdCommand(0x40 | symbolAddress);

    // Send the symbol data to CGRAM
    for (unsigned char i = 0; i < 8; i++) {
        lcdData(symbolData[i]);
    }
}

void lcdDisplayCustomSymbol(const unsigned char symbol[][8], unsigned char position) {
    for (unsigned char i = 0, column = position; i < 6; i++, column++) {
        // Create the custom symbol
        CreateCustomSymbol(i, symbol[i]);
        // Print the custom symbol on the LCD display
        if (column == 16)
            column = position;
        lcdPosition(i / 3, column);
        lcdData(i);
    }
}

