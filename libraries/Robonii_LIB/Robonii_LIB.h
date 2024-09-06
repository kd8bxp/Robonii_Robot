
#ifndef Robonii_LIB_H
#define Robonii_LIB_H

#include "Robonii.h"

typedef enum
{
ROBONII_LED_OFF =0,
ROBONII_LED_RED =1,
ROBONII_LED_GREEN=2,
ROBONII_LED_YELLOW=3
}LED_e;

#define D1_LED	D1_Green
#define D2_LED	D2_Green
#define D3_LED	D3_Green
#define D4_LED	D4_Green
#define D5_LED	D5_Green

#define LED_ON 		LOW
#define LED_OFF 	HIGH



void set_ROBONII_LED(unsigned char led_Nr, LED_e led_Status);
void init_Bumpers();
void init_LED();
void knight_Rider(unsigned char Delay_ms1);
void clear_ROBONII_LEDs();


#endif

