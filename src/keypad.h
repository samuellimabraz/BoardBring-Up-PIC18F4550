// -----------------------------------------------------------------------
//  * File: keypad.h
//      Header of library for Matrix keyboard operation
//
//  * Author: Samuel Lima Braz
// ----------------------------------------------------------------------- 

#ifndef KEYPAD_H
	#define KEYPAD_H

	unsigned char kpRead(void);
	void kpDebounce(void);
	void kpInit(void);

#endif