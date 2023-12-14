// -----------------------------------------------------------------------
//  * File: pwm.h
//      Header of library for PIC18F4550 PWM outputs access
//
//  * Author: Samuel Lima Braz
// ----------------------------------------------------------------------- 

#ifndef PWM_H
	#define PWM_H

	void pwmInit(void);
    void pwmSet(unsigned char porcento);
    void pwmFrequency(unsigned int freq);

#endif