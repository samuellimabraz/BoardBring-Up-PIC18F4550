// -----------------------------------------------------------------------
//  * File: lcd.h
//      Header of library for using lcd 1602A
//
//  * Author: Samuel Lima Braz
// ----------------------------------------------------------------------- 

#ifndef LCD_H
	#define LCD_H

    void lcdInit(void);
	void lcdCommand(unsigned char comando);
	void lcdData(unsigned char caracter);
	void lcdString(const char *palavra);
    void lcdPosition(unsigned char linha, unsigned char coluna);
    void lcdInt(int val, int n);
    void CreateCustomSymbol(unsigned char symbolIndex, const unsigned char *symbolData);
    void lcdDisplayCustomSymbol(const unsigned char symbol[][8], unsigned char position);
#endif
