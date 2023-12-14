// -----------------------------------------------------------------------
//  * File: event.h
//      Header of library for handling system events
//
//  * Author: Samuel Lima Braz
// ----------------------------------------------------------------------- 

#ifndef EVENT_H
#define	EVENT_H

// Events 
enum{
    EV_UP,
    EV_DOWN,
    EV_LEFT,
    EV_RIGHT,
    EV_BACK,
    EV_ALARM,
    EV_NOEVENT
};

// Main functions of event handling
void eventInit(void);
unsigned char eventRead(void);

#endif	/* EVENT_H */

