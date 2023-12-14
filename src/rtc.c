// -----------------------------------------------------------------------
//  * File: rtc.c
//      Library for communication with the MCP7940M RTC via I2C
// 
//  * Author: Samuel Lima Braz
// -----------------------------------------------------------------------
 

#include "i2c.h"
#include "rtc.h"

//endereço do dispositivo, deslocado por causa do bit de RW
#define DS1307_CTRL_ID (0x6f<<1)
#define I2C_WRITE 0
#define I2C_READ  1

int dec2bcd(int value) {
	return ((value / 10 * 16) + (value % 10));
}
int bcd2dec(int value) {
	return ((value / 16 * 10) + (value % 16));
}
void rtcInit(void) {
	i2cInit();
}
void rtcStartClock(void) {
	rtcWriteData(0x80, 0x0);
}

void rtcWriteData(unsigned char value, int address) {
	i2cWriteByte(1,0, DS1307_CTRL_ID|I2C_WRITE);
	i2cWriteByte(0,0,address);
	i2cWriteByte(0,1,value);
}
int rtcReadData(int address) {
	int result;
	i2cWriteByte(1,0,DS1307_CTRL_ID | I2C_WRITE);
	i2cWriteByte(0,0,address);
	i2cWriteByte(1,0, DS1307_CTRL_ID | I2C_READ);
	result = i2cReadByte(1,1);
	return result;
}

void rtcStopWatch(void) {
    setSeconds(0);
    setMinutes(0);
    setHours(0);
    rtcStartClock();
}
