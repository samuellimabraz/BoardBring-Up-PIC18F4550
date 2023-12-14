// -----------------------------------------------------------------------
//  * File: led.c
//      Library for LED's on pic18f4550
//
//  * Author: Samuel Lima Braz
// ----------------------------------------------------------------------- 


#include "led.h"
#include "delay.h"
#include "pic18f4520.h"

#define	RA4 4
#define	RA5 5
#define	RE0 0
#define	RE1 1

void ledInit(void) {

    TRISA = 0xCF;   // Configure RA4 and RA5 as outputs
    TRISE = 0xFC;   // Configure RE0 and RE1 as outputs
    PORTA |= (1 << RA4) | (1 << RA5);   // Set RA4 and RA5 pins high in PORTA
    PORTE |= (1 << RE0) | (1 << RE1);   // Set RE0 and RE1 pins high in PORTE
    ADCON1 = 0b00001111;    // Configure all analog inputs as digital

}

// Turn on led
void ledOn(unsigned char led) {
    switch (led) {
        case 1:
            BitClr(PORTA, RA4); // Set RA4 pin low
            break;
        case 2:
            BitClr(PORTA, RA5); // Set RA5 pin low
            break;
        case 3:
            BitClr(PORTE, RE0); // Set RE0 pin low
            break;
        case 4:
            BitClr(PORTE, RE1); // Set RE1 pin low
            break;
        default:
            break;
    }
}

// Turn off led
void ledOff(unsigned char led) {
    switch (led) {
        case 1:
            BitSet(PORTA, RA4); // Set RA4 pin high
            break;
        case 2:
            BitSet(PORTA, RA5); // Set RA5 pin high
            break;
        case 3:
            BitSet(PORTE, RE0); // Set RE0 pin high
            break;
        case 4:
            BitSet(PORTE, RE1); // Set RE1 pin high
            break;
        default:
            break;
    }
}

// Blink all 4 LEDs 
void ledBlink(int time) {
    unsigned char led;
    
    delay_ms(time);
    for (led = 1; led <= 4; led++) {
        ledOn(led);
    }
    delay_ms(time);
    for (led = 1; led <= 4; led++) {
        ledOff(led);
    }
}

// Turn LED on and off one by one
void ledSnake(void) {
    unsigned char led;
    for (led = 1; led <= 4; led++) {
        ledOn(led);
        delay_ms(500);
    }
    for (led = 1; led <= 4; led++) {
        ledOff(led);
        delay_ms(500);
    }   
}
