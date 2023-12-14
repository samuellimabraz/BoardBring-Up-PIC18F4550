// -----------------------------------------------------------------------
//  * File: var.c
//      Library for monitoring and storing system variables
//
//  * Author: Samuel Lima Braz
// ----------------------------------------------------------------------- 

#include "var.h"
#include "rtc.h"
#include "adc.h"

// Addrees for memory variables
#define ALARM1LOW 0x20
#define ALARM1HIGH 0x22
#define ALARM2LOW 0x24
#define ALARM2HIGH 0x26
#define LNG 0X28
#define PRSTATE 0x29


// Variables to be stored
static unsigned char state;     // Current state of the system
static unsigned char language;  // Selected language
static int smTime;              // State machine loop time
static unsigned int adc1;       // ADC1 value
static unsigned int adc2;       // ADC2 value

static char buffer[15];         // Buffer to storing protocol message
static int cont;            // Buffer counter

static const char alarmProtocol[4] = "/ALM"; // Alarm init protocol
static const char * linguageProtocol[3] = {"/LPTB", "/LING", "/LGER"}; // Language protocols

// Init variables with standard value
void varInit(void) {
    state = cont = 0;
    
    smTime = 1000;
    
    setAlarmLevel(1, 0, getAlarmLevel(1, 0));
    setAlarmLevel(1, 1, getAlarmLevel(1, 1));
    setAlarmLevel(2, 0, getAlarmLevel(2, 0));
    setAlarmLevel(2, 1, getAlarmLevel(2, 1));
    
    resetBuffer();
}

// Add char in protocol command buffer
void addBuffer(char valor) {
    if (cont >= 15) {
        cont=0;
    }
    buffer[cont] = valor;
    cont++;
}

// Memset 0 in buffer
void resetBuffer(void) {
    unsigned char i;
    for (i = 0; i < 14; i++) {
        buffer[i] = 48;
    }
    buffer[14] = '\0';
    cont = 0;
}

// Verify the protocol and execute the event
void executeProtocol(void) {
    unsigned char i;
    int value;
    
    // Language protocol |  /LXXX
    // compare character by character and set the math language
    if (buffer[1] == 'L') {
        for (i = 0; i < 3; i++) {
            if (strcmp(buffer, linguageProtocol[i], 2, 4)) {
                setLanguage(i);
                break;
            }
        }
    }
    
    // Alarm protocol | /ALMXX[XXXX]
    // Get value in last characters and match the adc  
    // /ALM1H, /ALM1L -> ADC1 high and low 
    // /ALM2H, /ALM2L -> ADC2 high and low 
    if (strcmp(buffer, alarmProtocol, 0, 3)) {
        value = ((buffer[6]-48)*1000 + (buffer[7]-48)*100 + (buffer[8]-48)*10 + (buffer[9]-48));
        
        if (buffer[4] == '1' && buffer[5] == 'H') 
            setAlarmLevel(1, 1, value);
        if (buffer[4] == '1' && buffer[5] == 'L')
            setAlarmLevel(1, 0, value);
        if (buffer[4] == '2' && buffer[5] == 'H')
            setAlarmLevel(2, 1, value);
        if (buffer[4] == '2' && buffer[5] == 'L')
            setAlarmLevel(2, 0, value); 
    }
}

// Compare two char vectors in [i, j] interval
unsigned char strcmp(const char s[], const char p[], unsigned char l, unsigned char j) {
    unsigned char i, flag = 1;
    for (i = l; i <= j; i++) {
        if (s[i] != p[i]) {
            flag = 0;
            break;
        }
    }
    return flag;
}

// Clip the value within the specified range
int clipInt(int valor, int inf, int sup) {
    return (valor > sup) ? sup : (valor < inf) ? inf : valor;
}

/* Getter and Setter - state */
unsigned char getState(void) {
    return state;
}

void setState(unsigned char newState) {
    state = newState;
}

/* Getter and Setter - Previous state
 * This value is used to return the alarm screen.
*/
unsigned char getPreviousState(void) {
    return (unsigned char)rtcReadData(PRSTATE);
}

void setPreviousState(unsigned char state) {
    rtcWriteData(state, PRSTATE);
}

/* Getter and Setter - time */
int getSmTime(void) {
    return smTime;
}

void setSmTime(int newTime) {
    newTime = clipInt(newTime, 0, 5000);
    smTime = newTime;
}

/*  Getter and Setter - ADC x x ;
 *               alarmNumber  level
 *  ADC 1 LOW  =    1           0;  address = 0x20, 0x21
 *  ADC 1 HIGH =    1           1;  address = 0x22, 0x23
 *  ADC 2 LOW  =    2           0;  address = 0x24, 0x25
 *  ADC 2 HIGH =    2           1;  address = 0x26, 0x27
*/
int getAlarmLevel(unsigned char alarmNumber, unsigned char level) {
    int address = 0;
    
    // Select memory adress
    switch (alarmNumber) {
        case 1:
            address = (level == 0) ? ALARM1LOW : (level == 1) ? ALARM1HIGH : 0;
            break;
        case 2:
            address = (level == 0) ? ALARM2LOW : (level == 1) ? ALARM2HIGH : 0;
            break;
    }
    
    if (address != 0) {
        // Reads the value's two addresses, separated into MSB and LSB
        return ((rtcReadData(address + 1) << 8) + rtcReadData(address));
    } else {
        // Handle invalid alarm number or level
        return -1;
    }
}

void setAlarmLevel(unsigned char alarmNumber, unsigned char level, int newAlarmLevel) {
    int address = 0;
    
    // Select memory adress
    switch (alarmNumber) {
        case 1:
            address = (level == 0) ? ALARM1LOW : (level == 1) ? ALARM1HIGH : 0;
            break;
        case 2:
            address = (level == 0) ? ALARM2LOW : (level == 1) ? ALARM2HIGH : 0;
            break;
    }
    
    if (address != 0) {
        // Limit value between 0 - 1000 
        newAlarmLevel = clipInt(newAlarmLevel, 0, 1000);
        
        // Controls the threshold values so that they make sense
        // The low level must be less than the upper, 
        // and the upper greater than the low
        if( ((level == 0) && (newAlarmLevel >= getAlarmLevel(alarmNumber, 1))) ||
            ((level == 1) && (newAlarmLevel <= getAlarmLevel(alarmNumber, 0))) ) {
            newAlarmLevel = getAlarmLevel(alarmNumber, level);
        }
        
        // Write in memory in 2 address for 16 bits number
        rtcWriteData(newAlarmLevel & 0xFF, address);
        rtcWriteData((newAlarmLevel >> 8) & 0xFF, address + 1);
        
    } else {
        // Handle invalid alarm number or level
        return;
    }
}

/* Getter and Setter -  Language ; address = 0x28  */
char getLanguage(void) {    
    return rtcReadData(LNG)%3;
}

void setLanguage(unsigned char newLanguage) {
    // There are only 3 languages, 
    // using modulo to avoid setting an incorrect value
    rtcWriteData(newLanguage % 3, LNG);
}

/* Getter and Setter - adc1 */
void setAdc1(unsigned int valor) {
    adc1 = valor;
}

unsigned int getAdc1(void) {
    return adc1;
}

/* Getter and Setter - adc2 */
void setAdc2(unsigned int valor) {
    adc2 = valor;
}

unsigned int getAdc2(void) {
    return adc2;
}

/* Getter and Setter - rtc time */
int getRtcSeconds(void) {
    return getSeconds();
}

int getRtcMinutes(void) {
    return getMinutes();
}

int getRtcHours(void) {
    return getHours();
}

