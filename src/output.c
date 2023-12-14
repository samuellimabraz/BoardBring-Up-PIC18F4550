// -----------------------------------------------------------------------
//  * File: output.c
//      Library for system interface, based on LCD ans serial screens
//
//  * Author: Samuel Lima Braz
// ----------------------------------------------------------------------- 

#include "output.h"
#include "stateMachine.h"
#include "lcd.h"
#include "serial.h"
#include "led.h"
#include "var.h"
#include <stdio.h>

#define NUM_IDIOMAS 3 // Number of supported languages
#define MAX_CHARACTERS 16 // Maximum number of characters for messages

// Struct for a screen with 2 messages
typedef struct {
    char msg1[NUM_IDIOMAS][MAX_CHARACTERS+1]; // Array of message 1 in different languages
    char msg2[MAX_CHARACTERS+1]; // Message 2
} Screen;

// Definition of screens
Screen screens[STATE_END] = {
    {{"Bem vindo!   ",    "Welcome!     ",    "Willkommen!  "},    ""}, // Welcome screen
    {{"Alarme ADC 1    ", "ADC 1 alarm     ", "ADC 1 alarm     "}, "                "}, // ADC 1 alarm screen
    {{"Alarme ADC 1    ", "ADC 1 alarm     ", "ADC 1 alarm     "}, "                "}, 
    {{"Alarme ADC 2    ", "ADC 2 alarm     ", "ADC 2 alarm     "}, "                "}, // ADC 2 alarm screen
    {{"Alarme ADC 2    ", "ADC 2 alarm     ", "ADC 2 alarm     "}, "                "},
    {{"Alterar tempo   ", "Change time     ", "Zeit andern     "}, "                "}, // Change time screen
    {{"Alterar idioma  ", "Change language ", "Andere Sprache  "}, "                "}, // Change language screen
    {{"ALERTA!         ", "ATTENTION!      ", "AUFMERKSAMKEIT! "}, "                "} // Alarm warning screen
};

// Language text
static const char languages[NUM_IDIOMAS][16] = {
    "Portuguese      ", 
    "English         ", 
    "Deutsch         "
};

// Map for custom sybol of Unifei logo
static const unsigned char unifeiLogo[6][8] = {
	{0x01, 0x03, 0x03, 0x0E, 0x1C, 0x18, 0x08, 0x08}, //0,0
	{0x11, 0x1F, 0x00, 0x01, 0x1F, 0x12, 0x14, 0x1F}, //0,1
	{0x10, 0x18, 0x18, 0x0E, 0x07, 0x03, 0x02, 0x02}, //0,2
	{0x08, 0x18, 0x1C, 0x0E, 0x03, 0x03, 0x01, 0x00}, //1,0
	{0x12, 0x14, 0x1F, 0x08, 0x00, 0x1F, 0x11, 0x00}, //1,1
	{0x02, 0x03, 0x07, 0x0E, 0x18, 0x18, 0x10, 0x00}  //1,2
};

// Shows a screen on lcd and serial 
void displayScreen(Screen *screen, int language) {
    lcdPosition(0, 0);
    lcdString(screen->msg1[language]);
    lcdPosition(1, 0);
    lcdString(screen->msg2);
    
    serialClear();
    serialString(screen->msg1[language]);
    serialString(screen->msg2);
}

// Screen content control
void outputPrint(int screenIndex, int language) {
    Screen *screen = &screens[screenIndex];
    
    // Defines the property of the screens and their actions
    switch (screenIndex) {
        case STATE_MENU:
            // Print Unifei logo in LCD
            lcdDisplayCustomSymbol(unifeiLogo, 13);
            snprintf(screen->msg2, sizeof(screen->msg2), "%02d:%02d:%02d     ", getRtcHours(), getRtcMinutes(), getRtcSeconds());
            break;
  
        case STATE_ALARM1LOW:
            snprintf(screen->msg2, sizeof(screen->msg2), ">%04d       %04d", getAlarmLevel(1, 0), getAlarmLevel(1, 1));
            break;
            
        case STATE_ALARM1HIGH:
            snprintf(screen->msg2, sizeof(screen->msg2), "%04d       >%04d", getAlarmLevel(1, 0), getAlarmLevel(1, 1));
            break;
            
        case STATE_ALARM2LOW:
            snprintf(screen->msg2, sizeof(screen->msg2), ">%04d       %04d", getAlarmLevel(2, 0), getAlarmLevel(2, 1));
            break;
            
        case STATE_ALARM2HIGH:
            snprintf(screen->msg2, sizeof(screen->msg2), "%04d       >%04d", getAlarmLevel(2, 0), getAlarmLevel(2, 1));
            break;
            
        case STATE_TIME:
            snprintf(screen->msg2, sizeof(screen->msg2), "%04d            ", getSmTime());
            break;
            
        case STATE_LANGUAGE:
            snprintf(screen->msg2, sizeof(screen->msg2), "%s", languages[getLanguage()]);
            break;  
        
        case STATE_WARNING:
            snprintf(screen->msg2, sizeof(screen->msg2), "A1:%04d  A2:%04d ", getAdc1(), getAdc2());
            ledBlink(600);
            break;
    }
    // Display screen in defined language
    displayScreen(screen, language);
}

// Init the devices
void outputInit(void) {
    lcdInit();
    serialInit();
    ledInit();
}

