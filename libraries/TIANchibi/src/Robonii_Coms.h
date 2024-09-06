#ifndef ROBONII_COMS_H

#define ROBONII_COMS_H

#include "types.h"
#include <avr/io.h>
#include "Remote_Common.h"

#define ROBONII_TEST			

#define REMOTE_DATA_SIZE  (REMOTE_MESSAGE_ARRAY_SIZE / 4) + 1
#define ZIGBEE_DATA_START_OFFSET  1

//#define REMOTE_MESSAGE_ARRAY_SIZE	(LCD_MAX_INDEX + REMOTE_LED_MAX_INDEX)
#define REMOTE_MESSAGE_ARRAY_SIZE	(64 + 6)

#define PARSE_SUCSES 	0
#define PARSE_ERROR		1
#define PARSE_ERROR_Length	5
#define PARSE_ERROR_CRC		6

#define ROBONII_ROBOT_TYPE		2
#define ROBONII_REMOTE_TYPE		1

#define REMOTE_LEFT_BACKWARD	(0x02)
#define REMOTE_LEFT_FORWARD		(0x01)
#define REMOTE_LEFT_TRIGGER		(0x04)
#define REMOTE_RIGHT_BACKWARD	(0x10)
#define REMOTE_RIGHT_FORWARD	(0x08)
#define REMOTE_RIGHT_TRIGGER	(0x20)
#define REMOTE_FRONT_DOUBLE_TAP	(0x40)		
#define REMOTE_NONE  (0x80)

/*
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

*/
  //8B 16 0 0 1 D5 25 0
 uint64_t OwnMac =    0x25D5010000168B;
 
 //1 0 0 0 0 D5 25 0
 uint64_t PairedMac = 0x25D50000000001;

 unsigned char Robonii_Auto_Paired_Reply = 1;
 
 
 
unsigned char Robonii_MakeMessage(unsigned char *txBuffer, unsigned char *data, unsigned int dataLentgh);
void Robonii_SetPairedMac(uint64_t NewMac);
void Robonii_SetOwnMac(uint64_t NewMac);
unsigned char Robonii_Parse_Msg(unsigned char *data, unsigned char DataLength);
unsigned char Robonii_CalculateCRC(uint8_t *data, uint8_t length);
void Robonii_sendRemoteMessagePairedAvailable();
void Robonii_sendRemoteMessageControlTest();
void Robonii_Print_Msg();
void Robonii_addMAc(uint64_t MacToAdd,unsigned char *data, unsigned char *pos);
void Robonii_MAC_to_char(uint64_t mac, unsigned char *DestinationBuf);

unsigned char Robonii_Remote_Key_Pressed();


unsigned int Robonii_PANID = 0x22FE;


unsigned int MsgPANID=0;
uint64_t MsgSrcMac=0;	// Message Source Mac
uint64_t MsgDstMac=0;	// Message Destination Mac
unsigned char MsgLength = 0;
unsigned char MsgCounter = 0;
unsigned char MsgCRC = 0;

unsigned char MsgDataHdr = 0;
unsigned char MsgDataLength = 0;
unsigned char MsgDataCounter = 0;
unsigned char MsgDataDevType = 0;
unsigned char MsgDataCRC = 0;
byte MsgData[127];
byte *MsgDataptr;
 
unsigned char Robonii_Remote_Battery = 0;
unsigned char Robonii_Remote_Keys = 0;
unsigned char Robonii_Remote_LF = 0;
unsigned char Robonii_Remote_FR = 0;
unsigned char Robonii_Remote_RF = 0;
unsigned char Robonii_Remote_RR = 0;
unsigned char Robonii_Remote_LT = 0;
unsigned char Robonii_Remote_RT = 0;
unsigned char Robonii_Remote_Robonii_Remote_DubleTap = 0;
unsigned char Robonii_Remote_New_Message = 0;
unsigned char Robonii_Remote_No_Key_Pressed = 0;

void SetBigNumberDisplay(uint8_t amount, display_state_t displayState);
void SetSmallNumberDisplay(uint8_t amount, display_state_t displayState);
unsigned char GetLCDNumberTranslation( unsigned char number );

void SetBigNumberSpecial(unsigned char char1, unsigned char char2, display_state_t displayState);
void SetSmallNumberSpecial(unsigned char char1, unsigned char char2, display_state_t displayState);
void SetRemoteBars(uint8_t amount, display_state_t displayState);
void SetRemoteBullets(uint8_t amount, display_state_t displayState);
void SetRemoteLEDs(uint8_t amount, display_state_t displayState);
void set_lcd(lcd_e lcdSegment, display_state_t displayState);







#endif