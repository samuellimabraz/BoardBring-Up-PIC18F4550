#include "config.h"
#include "pic18f4520.h"
#include "timer.h"
#include "keypad.h"
#include "event.h"
#include "var.h"
#include "stateMachine.h"
#include "output.h"
#include "rtc.h"


void main(void) {
     
    // Libraries init
    timerInit();
    rtcInit();
    varInit();
    eventInit();
    outputInit();
    smInit();
    rtcStopWatch();
               
    for (;;) {
        // Timed loop
        timerReset(getSmTime());
        
        // Board infrastructure
        kpDebounce();
        
        // State Machine
        smLoop();
        
        timerWait();

    }
}