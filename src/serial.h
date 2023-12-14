// -----------------------------------------------------------------------
//  * File: serial.h
//      Header of library for serial communication (USART) in PIC18F4550
//
//  * Author: Samuel Lima Braz
// ----------------------------------------------------------------------- 


#ifndef SERIAL_H
	#define SERIAL_H

	void serialSend(unsigned char c);
    void serialString(char msg[]);
    void serialClear(void);
    void serialInt(int val, int n);
	unsigned char serialRead(void);
	void serialInit(void);

#endif