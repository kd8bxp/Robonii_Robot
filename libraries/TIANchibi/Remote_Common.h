#ifndef REMOTE_COMMON_H
#define REMOTE_COMMON_H


// Segment array indexes for production lcd.
typedef enum
{
  LCD_ROBOT=                   0,
  LCD_LEFT_TRIGGER=            1,
  LCD_BAR4=                    2,
  LCD_BAR3=                    3,
  LCD_BAR2=                    4,
  LCD_BAR1=                    5,
  LCD_BULLET4=                 6,
  LCD_BULLET3=                 7,
  LCD_BULLET2=                 8,
  LCD_BULLET1=                 9,
  LCD_REMOTE=                 10,
  LCD_TAG1=                    11,
  LCD_LEVEL=                   12,
  LCD_TAG2=                    13,
  LCD_AUTO=                    14,
  LCD_TAG3=                    15,
  LCD_SUN =                    16,
  LCD_FACE =                   17,
  LCD_FACE_HAPPY=              18,
  LCD_FACE_SAD=                19,
  LCD_BOMB =                   20,
  LCD_REPAIR =                 21,
  LCD_GAME =                   22,
  LCD_SFX_TAG =                23,
  LCD_TEAM1 =                  24,
  LCD_HOST =                   25,
  LCD_TEAM2 =                  26,
  LCD_JOIN =                   27,
  LCD_ENERGY_TAG =             28,
  LCD_BASE_STATION =           29,
  LCD_WAIT =                   30,
  LCD_START =                  31,
  LCD_SS_3A =                  32,
  LCD_END =                    33,
  LCD_SS_4A =                  34,
  LCD_RIGHT_TRIGGER =          35,
  LCD_SS_3F  =                 36,
  LCD_SS_3E  =                 37,
  LCD_SS_3G  =                 38,
  LCD_SS_3D  =                 39,
  LCD_SS_3B  =                 40,
  LCD_SS_3C  =                 41,
  LCD_SS_4F  =                 42,
  LCD_SS_4E  =                 43,
  LCD_SS_4G  =                 44,
  LCD_SS_4D  =                 45,
  LCD_SS_4B  =                 46,
  LCD_SS_4C  =                 47,
  LCD_SS_2C  =                 48,
  LCD_SS_2B  =                 49,
  LCD_SS_2G  =                 50,
  LCD_SS_2A  =                 51,
  LCD_SS_2E  =                 52,
  LCD_SS_2F  =                 53,
  LCD_SS_1C  =                 54,
  LCD_SS_1B  =                 55,
  LCD_SS_1G  =                 56,
  LCD_SS_1A  =                 57,
  LCD_SS_1E  =                 58,
  LCD_SS_1F  =                 59,
  LCD_SS_B   =                 60,
  LCD_SS_A   =                 61,
  LCD_SS_1D  =                 62,
  LCD_SS_2D  =                 63,

  //Last/max index placeholder
  LCD_MAX_INDEX,
} lcd_e;

//Led array indexes
typedef enum
{ 
  REMOTE_LED1 = 0,
  REMOTE_LED2,
  REMOTE_LED3,
  REMOTE_LED4,
  REMOTE_LED5,
  REMOTE_LED6,

  //Last/max index placeholder
  REMOTE_LED_MAX_INDEX,
} remote_led_e;


// Typedef Defines LCD Display State
typedef enum
{
	DS_OFF	= 0,
	DS_ON	= 1,
	DS_SLOW_FLICKER	= 2,
	DS_FAST_FLICKER = 3,
} display_state_t;

typedef enum
{
	ZIGBEE_NONE = 0,
	ZIGBEE_APPLICATION = 10,
	ZIGBEE_GGI = 11,
  ZIGBEE_APPLICATION2 = 12,
  ZIGBEE_APPLICATION3 = 13,
  ZIGBEE_APPLICATION4 = 14,
  ZIGBEE_APPLICATION5 = 15,
} zigbeeMessageType_t;

#endif