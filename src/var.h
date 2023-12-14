// -----------------------------------------------------------------------
//  * File: var.h
//      Header of library for monitoring and storing system variables
//
//  * Author: Samuel Lima Braz
// ----------------------------------------------------------------------- 

#ifndef VAR_H
    #define	VAR_H

    void varInit(void);
    
    // Buffer functions
    void addBuffer(char valor);
    void resetBuffer(void);
    void executeProtocol(void);

    // Auxiliary functions
    unsigned char strcmp(const char s[], const char p[], unsigned char l, unsigned char j);
    int clipInt(int valor, int inf, int sup);
    
    // Get and set for state variables
    unsigned char getState(void);
    void setState(unsigned char newState);
    
    unsigned char getPreviousState(void);
    void setPreviousState(unsigned char state);

    int getSmTime(void);
    void setSmTime(int newTime);
    
    int getAlarmLevel(unsigned char alarmNumber, unsigned char level);
    void setAlarmLevel(unsigned char alarmNumber, unsigned char level, int newAlarmLevel);

    char getLanguage(void);
    void setLanguage(unsigned char newLanguage);

    void setAdc1(unsigned int valor);
    unsigned int getAdc1(void);
    
    void setAdc2(unsigned int valor);
    unsigned int getAdc2(void);
    
    // Mask for rtc functions 
    int getRtcSeconds(void);
    int getRtcMinutes(void);
    int getRtcHours(void);

#endif	/* VAR_H */
