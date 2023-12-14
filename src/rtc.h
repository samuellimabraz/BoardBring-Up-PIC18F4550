// -----------------------------------------------------------------------
//  * File: rtc.h
//      Header of library for communication with the MCP7940M RTC via I2C
//    
//  * Author: Samuel Lima Braz
// ----------------------------------------------------------------------- 

#ifndef RTC_h
#define RTC_h

//definição dos endereços
#define SEC     0
#define MIN     1
#define HOUR    2
#define WEEKDAY 3
#define DAY     4
#define MONTH   5
#define YEAR    6

	void rtcInit(void);
	void rtcStartClock(void);
	void rtcStopClock(void);
	int dec2bcd(int value);
	int bcd2dec(int value);
	void rtcWriteData(unsigned char value, int address);
	int rtcReadData(int address);
    void rtcStopWatch(void);

//funções de leitura/escrita simplificadas
#define getSeconds()  (bcd2dec(rtcReadData(SEC)& 0x7f))
#define getMinutes()  (bcd2dec(rtcReadData(MIN)& 0x7f))
#define getHours()    (bcd2dec(rtcReadData(HOUR)& 0x5f))
#define getWeekDay()  (bcd2dec(rtcReadData(WEEKDAY)& 0x07))
#define getDays()     (bcd2dec(rtcReadData(DAY)& 0x5f))
#define getMonths()   (bcd2dec(rtcReadData(MONTH)& 0x3f))
#define getYears()    (bcd2dec(rtcReadData(YEAR)& 0xff))

#define setSeconds(v) (rtcWriteData((unsigned char)dec2bcd(v),SEC))
#define setMinutes(v) (rtcWriteData((unsigned char)dec2bcd(v),MIN))
#define setHours(v)   (rtcWriteData((unsigned char)dec2bcd(v),HOUR))
#define setWeekDay(v) (rtcWriteData(dec2bcd(v),WEEKDAY))
#define setDays(v)    (rtcWriteData(dec2bcd(v),DAY))
#define setMonths(v)  (rtcWriteData(dec2bcd(v),MONTH))
#define setYears(v)   (rtcWriteData(dec2bcd(v),YEAR))

#endif


