// -----------------------------------------------------------------------
//  * File: stateMachine.c
//      Library for state machine logic, handling state transitions and actions
//
//  * Author: Samuel Lima Braz
// ----------------------------------------------------------------------- 


#include "stateMachine.h"
#include "event.h"
#include "var.h"
#include "output.h"

// Type definition for activity functions, a function pointer
typedef void (*ActivityFunction)(const unsigned char event);

// Structure definition for a state
typedef struct {
    unsigned char masterState; // Superior state, where can always return
    unsigned char previousState; // Previous state
    unsigned char nextState; // Next State
    ActivityFunction activity; // State action function
} State;

// List of states, with yours transition states and activity functions
State stateMachine[] = {
    {STATE_MENU,    STATE_LANGUAGE,    STATE_ALARM1LOW,    menuActivity},   // STATE_MENU
    {STATE_MENU,    STATE_MENU,        STATE_ALARM1HIGH,   alarmActivity},  // STATE_ALARM1LOW
    {STATE_MENU,    STATE_ALARM1LOW,   STATE_ALARM2LOW,    alarmActivity},  // STATE_ALARM1HIGH
    {STATE_MENU,    STATE_ALARM1HIGH,  STATE_ALARM2HIGH,   alarmActivity},  // STATE_ALARM2LOW
    {STATE_MENU,    STATE_ALARM2LOW,   STATE_TIME,         alarmActivity},  // STATE_ALARM2HIGH
    {STATE_MENU,    STATE_ALARM2HIGH,  STATE_LANGUAGE,     timeActivity},   // STATE_TIME
    {STATE_MENU,    STATE_TIME,        STATE_MENU,         languageActivity},   // STATE_LANGUAGE
    {STATE_MENU,    STATE_WARNING,     STATE_WARNING,      handleWarningState}, // STATE_WARNING
};

// Initializes the state machine
void smInit(void) {
    setState(STATE_MENU);
}

// Main loop of the state machine
void smLoop(void) {
    unsigned char event = eventRead(); 
    
    
    // Check if the state is valid
    if ((getState() > STATE_END) || (getState() < STATE_MENU)) {
        handleInvalidState();
    } else{
     
        // Check if it's an alarm event
        if (event == EV_ALARM) {
            // Records the previous state of alarm generation
            if (getState() != STATE_WARNING)
                setPreviousState(getState());
            // Set to Alarm state
            setState(STATE_WARNING);
        }

        // Get the data of the current state
        State *currentStateData = &stateMachine[getState()];

        // Execute the activity function for the current state
        currentStateData->activity(event);

        /* State machine transitions 
         * The state machine pattern is: 
         *  EV_LEFT :   go to previous state
         *  EV_RIGHT :  go to next state                                   
         *  EV_BACK :   go to master state
         */
        if (event == EV_LEFT) {
            setState(currentStateData->previousState);
        }
        if (event == EV_RIGHT) {
            setState(currentStateData->nextState);
        }
        if (event == EV_BACK) {
            setState(currentStateData->masterState);
        }
    }
    // Print output
    outputPrint(getState(), getLanguage());
}

// Activity function for the menu state
void menuActivity(const unsigned char event) {
    // No specific activity for the menu state
    return;
}


/* Activity function for the all alarm state 
 * Select the alarm number and level 
 * according to the current state
 */
void alarmActivity(const unsigned char event) {
    switch (getState()) {
        case STATE_ALARM1LOW:
            handleAlarmState(event, 1, 0); // alarm 1 low state
            break;
        case STATE_ALARM1HIGH:
            handleAlarmState(event, 1, 1); // alarm 1 high state
            break;
        case STATE_ALARM2LOW:
            handleAlarmState(event, 2, 0); // alarm 2 low state
            break;
        case STATE_ALARM2HIGH:
            handleAlarmState(event, 2, 1); // alarm 2 high state
            break;
    }
}

/* Auxiliary function to manipulate the alarm values in each state
 * In all cases, EV_UP = increase 1 on alarm level
 *               EV_DOWN = decrease 1 on alarm level
 */
void handleAlarmState(const unsigned char event, unsigned char alarmNumber, unsigned char level) {
    if (event == EV_UP) {
        setAlarmLevel(alarmNumber, level, (getAlarmLevel(alarmNumber, level) + 1));
    }
    if (event == EV_DOWN) {
        setAlarmLevel(alarmNumber, level, (getAlarmLevel(alarmNumber, level) - 1));
    }
}

// Activity function for the time state
void timeActivity(const unsigned char event) {
    if (event == EV_UP) {
        setSmTime(getSmTime() + 1);
    }
    if (event == EV_DOWN) {
        setSmTime(getSmTime() - 1);
    } 
}

// Activity function for the language state
void languageActivity(const unsigned char event) {
    // Execute activity
    if (event == EV_UP) {
        setLanguage(getLanguage() + 1);
    }
    if (event == EV_DOWN) {
        setLanguage(getLanguage() - 1);
    }
}

// Activity function for handling the warning state
void handleWarningState(const unsigned char event) {
    if (event != EV_ALARM) {
        setState(getPreviousState());
    }
}

// Function for handling an invalid state
void handleInvalidState() {
    setState(STATE_MENU);
}
