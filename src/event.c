// -----------------------------------------------------------------------
//  * File: event.c
//      Library for handling system events
//
//  * Author: Samuel Lima Braz
// ----------------------------------------------------------------------- 


#include "event.h"
#include "keypad.h"
#include "serial.h"
#include "adc.h"
#include "var.h"
#include "pic18f4520.h"

static unsigned int key_ant; // Previous state of the keypad
static unsigned char isCommand; // Flag to indicate if a command protocol is being entered

// Init devices and variables
void eventInit(void) {
    kpInit(); // Initialize keypad
    key_ant = isCommand = 0;
    serialInit(); // Initialize serial communication
    adcInit(); // Initialize ADC
}

// Define and publish events
unsigned char eventRead(void) {
    unsigned char key = kpRead(); // Read current state of the keypad
    char serialkey = serialRead(); // Read character from serial communication
    unsigned char ev = EV_NOEVENT; // Event value initialized as no event
   
    // Trigger an alarm event if ADC value is outside alarm thresholds
    setAdc1(adcRead(1));
    setAdc2(adcRead(3));
    
    if ((getAdc1() > getAlarmLevel(1, 1) || getAdc1() < getAlarmLevel(1, 0)) ||
        (getAdc2() > getAlarmLevel(2, 1) || getAdc2() < getAlarmLevel(2, 0))) {
        ev = EV_ALARM;
    }
    
    // Set the protocol command flag if the received character is '/'
    if (serialkey == '/') {
        isCommand = 1; 
    }

    if (!isCommand) {
        // Handle events based on received serial characters
        if (serialkey == 'w' || serialkey == 'W') {
            ev = EV_UP;
        }

        if (serialkey == 's' || serialkey == 'S') {
            ev = EV_DOWN;
        }

        if (serialkey == 'd' || serialkey == 'D') {
            ev = EV_RIGHT;
        }

        if (serialkey == 'a' || serialkey == 'A') {
            ev = EV_LEFT;
        }

        if (serialkey == 'f' || serialkey == 'F') {
            ev = EV_BACK;
        }
    } else {
        // Finish protocol
        if (serialkey == 13) {
            isCommand = 0; // Reset the command flag
            executeProtocol(); // Execute the command in the buffer
            resetBuffer(); // Reset the command buffer
        }else if (serialkey != 0) {
            // Handle protocol command input
            addBuffer(serialkey); // Add character to the command buffer
        }
    }
    
    // Handle events based on keypad input
    if (key != key_ant) {
        if (BitTst(key, 0)) {
            ev = EV_RIGHT;
        }

        if (BitTst(key, 1)) {
            ev = EV_UP;
        }

        if (BitTst(key, 2)) {
            ev = EV_DOWN;
        }

        if (BitTst(key, 3)) {
            ev = EV_LEFT;
        }
        
        if (BitTst(key, 4)) {
            ev = EV_BACK;
        }
    }
    key_ant = key; // Update the previous keypad state

    return ev; // Return the event value
}
