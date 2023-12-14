// -----------------------------------------------------------------------
//  * File: delay.c
//      Library for auxiliary delays
//
//  * Author: Samuel Lima Braz
// ----------------------------------------------------------------------- 


#include "delay.h"

void delay_ms(int time_ms) {

    unsigned char j, k;
    
    for (int i = 0; i < time_ms; i++)
    {
        for (j = 0; j < 10; j++) {

            for (k = 0; k < 4; k++);
            
        }
    }
}

void delay_s(int time_s) {

    unsigned char j, k, m;

    for (int i = 0; i < time_s; i++)
    {
        for (j = 0; j < 200; j++) {

            for (k = 0; k < 150; k++){
            
                for (m = 0; m < 2; m++);
            }
        }
    }
}