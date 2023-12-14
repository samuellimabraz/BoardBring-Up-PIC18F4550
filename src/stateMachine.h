// -----------------------------------------------------------------------
//  * File: stateMachine.h
//      Header of library for state machine logic, handling state transitions and actions
//
//  * Author: Samuel Lima Braz
// ----------------------------------------------------------------------- 

#ifndef STATEMACHINE_H
#define	STATEMACHINE_H


// States of state machine
enum {
    STATE_MENU,
    STATE_ALARM1LOW,
    STATE_ALARM1HIGH,
    STATE_ALARM2LOW,
    STATE_ALARM2HIGH,
    STATE_TIME,
    STATE_LANGUAGE,
    STATE_WARNING,
    STATE_END
};

// Main functions of the state machine
void smInit(void);
void smLoop(void);

// Auxiliary functions for state activity
void menuActivity(const unsigned char evento);

void alarmActivity(const unsigned char event);
void handleAlarmState(const unsigned char event, unsigned char alarmNumber, unsigned char level);

void timeActivity(const unsigned char evento);
void languageActivity(const unsigned char evento);
void handleWarningState(const unsigned char evento);
void handleInvalidState(void);


#endif	/* STATEMACHINE_H */

