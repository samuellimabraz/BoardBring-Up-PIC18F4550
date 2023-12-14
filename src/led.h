// -----------------------------------------------------------------------
//  * File: led.h
//      Header of library for LED's on pic18f4550
//
//  * Author: Samuel Lima Braz
// ----------------------------------------------------------------------- 


#ifndef LED_H
    #define LED_H
    
    // Main functions of LEDs
    void ledInit(void);
    void ledOn(unsigned char led);
    void ledOff(unsigned char led);
    
    // Entertainment functions
    void ledBlink(int time);
    void ledSnake(void);

#endif
