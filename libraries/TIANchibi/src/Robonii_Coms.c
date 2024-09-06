#include "src/Robonii_coms.h"
#include "TianChibi.h"
#include "HardwareSerial.h"
#include "Remote_Common.h"

#ifndef ROBONII_REMOTE_COMS_C
#define ROBONII_REMOTE_COMS_C

unsigned char Robonii_Remote_Key_Pressed()
{

if (Robonii_Remote_New_Message)
{
Robonii_Remote_New_Message = 0;
return Robonii_Remote_Keys;

}

return 0;
}

unsigned char Robonii_Service_Remote_Message(unsigned char *data)
{
 Robonii_Remote_Battery = data[2];
Robonii_Remote_Keys = data[1];

Robonii_Remote_LF = Robonii_Remote_Keys & REMOTE_LEFT_FORWARD;
Robonii_Remote_FR = Robonii_Remote_Keys &REMOTE_LEFT_BACKWARD;
 Robonii_Remote_RF = Robonii_Remote_Keys &REMOTE_RIGHT_FORWARD;
 Robonii_Remote_RR = Robonii_Remote_Keys &REMOTE_RIGHT_BACKWARD;
 Robonii_Remote_LT = Robonii_Remote_Keys &REMOTE_LEFT_TRIGGER;
 Robonii_Remote_RT = Robonii_Remote_Keys &REMOTE_RIGHT_TRIGGER;
 Robonii_Remote_Robonii_Remote_DubleTap = Robonii_Remote_Keys & REMOTE_FRONT_DOUBLE_TAP;
 
 if (Robonii_Remote_Keys==0)
 {
 Robonii_Remote_No_Key_Pressed = REMOTE_NONE;
 Robonii_Remote_Keys = REMOTE_NONE;
 }else
  {
  Robonii_Remote_No_Key_Pressed = 0;
  }
 


Robonii_Remote_New_Message = 1;
}


unsigned char Robonii_Parse_Msg(byte *data, unsigned char DataLength)
{
	MsgLength = data[0];
	if (DataLength != MsgLength)
	{
		return PARSE_ERROR_Length;
	}
	MsgCounter=data[3];
	MsgPANID = data[5] * 256 + data[4];
	
	unsigned char i=0;
	MsgSrcMac=0;
	MsgDstMac=0;
	for (i=8;i>0;i--)
	{
		MsgSrcMac = MsgSrcMac*256;
		MsgSrcMac = MsgSrcMac+data[5+i];
		MsgDstMac = MsgDstMac*256;
		MsgDstMac = MsgDstMac+data[5+8+i]	;
	}

	MsgDataHdr = data[22];
	MsgDataLength = data[23];
	MsgDataCounter = data[24];
	MsgDataDevType = data[25];
	MsgDataCRC = data[26];
	MsgDataptr = &data[27];
	
	unsigned char i3 = 0;
	for (i=27;i<MsgLength;i++)
	{
	MsgData[i3++] = data[i];
	}
	
	unsigned char CRC_TEST = Robonii_CalculateCRC(data,DataLength-1);
	
	if (CRC_TEST != data[DataLength-1])
	{
		//return PARSE_ERROR_CRC;
	}
	
	
	if ((MsgDataHdr== 0xA0)&&(MsgDataDevType==ROBONII_REMOTE_TYPE))
	{
		//Decode Remote Message
		Robonii_Service_Remote_Message(MsgDataptr);
		
	}
	
	
	if ((Robonii_Auto_Paired_Reply)&&(MsgDataHdr== 0xA5))
	{
		if ((MsgDstMac == OwnMac)||(PairedMac==0)||(OwnMac==0))
		{
			//Remote Request a header reply message from me
			#ifdef ROBONII_TEST			
			Robonii_Print_Msg();
			#endif			
			if ((PairedMac==0))
			{
				PairedMac = MsgSrcMac;
			#ifdef ROBONII_TEST			
				unsigned char macbuf[9];
				Serial.print("Don't have paired MAC. Taking this one...Own MAC: ");
				Robonii_MAC_to_char(OwnMac,macbuf);
				Serial.println("");
				Serial.print("Paired MAC: ");
				Robonii_MAC_to_char(PairedMac,macbuf);
				Serial.println("");
			#endif	
			}
			if ((OwnMac==0))
			{
				OwnMac=MsgDstMac;
			#ifdef ROBONII_TEST			
					unsigned char macbuf2[9];
					Serial.print("Don't have MAC. Taking this one... Own MAC: ");
					Robonii_MAC_to_char(OwnMac,macbuf2);
					Serial.println("");
					Serial.print("Paired MAC: ");
					Robonii_MAC_to_char(PairedMac,macbuf2);
					Serial.println("");
			#endif	
				
			}
			
			Robonii_sendRemoteMessagePairedAvailable();
		}
	}
	
	
	return PARSE_SUCSES;
}


void Robonii_SetAutoPairedReply(unsigned char NewSetting)
{
	Robonii_Auto_Paired_Reply =NewSetting;
}


void Robonii_SetOwnMac(uint64_t NewMac)
{
	OwnMac =NewMac;
}

void Robonii_SetPairedMac(uint64_t NewMac)
{
	OwnMac =NewMac;
}

/**
 * \brief Calculate CRC.
 *
 * This function is used to calculate the CRC for a message with a length smaller than 255 chars.
 * \param  *data The data pointer that points to a memory location.
 * \param  length The length of the data.
 * \return unsigned char The byte containing the CRC is returned.
*/
unsigned char Robonii_CalculateCRC(uint8_t *data, uint8_t length)
{
	unsigned char crcCalc = 0x00;
	for(uint8_t i=0; i< length; i++)
		crcCalc ^= data[i];

	return crcCalc;
}

void Robonii_addMAc(uint64_t MacToAdd,unsigned char *data, unsigned char *pos)
{
  int i = 0;
  
  for (i=0; i<8;i++)
  {
    data[(*pos)++]= MacToAdd%256;
    
    MacToAdd = MacToAdd/256;
  }
}

unsigned char Robonii_MsgCounter = 2;
unsigned char Robonii_MakeMessage(unsigned char *txBuffer, unsigned char *data, unsigned int dataLentgh)
{
  //8B 16 0 0 1 D5 25 0
 //uint64_t OwnMac =    0x25D5010000168B;
 
 //1 0 0 0 0 D5 25 0
// uint64_t PairedMac = 0x25D50000000001;
 
 //unsigned int Robonii_PANID = 0x22FE;
 
  byte buf2[127];
  unsigned char i=1;
  
  txBuffer[0] = 22+dataLentgh+1; //Header + data + crc
  txBuffer[i++] = 0x61;
  txBuffer[i++] = 0xCC;
  txBuffer[i++] =Robonii_MsgCounter++;
  
  
  txBuffer[i++] = Robonii_PANID%256;
  txBuffer[i++] = Robonii_PANID/256;
  
   Robonii_addMAc(OwnMac,txBuffer,&i);
  Robonii_addMAc(PairedMac,txBuffer,&i);
 
  
  int dataCount = 0;
  for (dataCount=0;dataCount<dataLentgh;dataCount++)
  {
  txBuffer[i++] =data[dataCount]  ;
  }
  
  unsigned char crc = Robonii_CalculateCRC(buf2,i);
  txBuffer[i++] = crc;
  return i;
}


void Robonii_sendRemoteMessagePairedAvailable()
{
  byte buf2[100];
  byte bufTX[100];
 
  unsigned char i=0;
   buf2[i++] = 0xA7;
  buf2[i++] = 0x00;
  buf2[i++] = 0xE3;
  
  buf2[i++] = 0x02;
  buf2[i++] = 0x00;
  //buf2[i++] = 0xA3;
 // unsigned char crc = CalculateCRC(buf2,i);
 // buf2[i++] = crc;
  unsigned char TXLength = 0;
 TXLength= Robonii_MakeMessage(bufTX,buf2,i);


  chibiTxRaw( bufTX, TXLength);
}

void Robonii_sendRemoteMessageControlTest()
{
  byte buf2[100];
  byte bufTX[100];
 
  unsigned char i=0;
   buf2[i++] = 0xA0;
  buf2[i++] = 0x13;
  buf2[i++] = 0xA3;
  
  buf2[i++] = 0x02;
  buf2[i++] = 0x5E;
  buf2[i++] = 0x0A;
  
   buf2[i++] = 0x51;
  buf2[i++] = 0x55;
  buf2[i++] = 0x15;
  
   buf2[i++] = 0x10;
  buf2[i++] = 0x00;
  buf2[i++] = 0x00;
  
   buf2[i++] = 0x04;
  buf2[i++] = 0x00;
  buf2[i++] = 0x11;
  
   buf2[i++] = 0x55;
  buf2[i++] = 0x54;
  buf2[i++] = 0x54;
  
    buf2[i++] = 0x45;
  buf2[i++] = 0x55;
  buf2[i++] = 0x54;
  
   buf2[i++] = 0x50;
  buf2[i++] = 0x01;
  buf2[i++] = 0x00;
  
 // buf2[i++] = 0xB9;
 // unsigned char crc = CalculateCRC(buf2,i);
 // buf2[i++] = crc;
  
  
   unsigned char TXLength = 0;
 TXLength= Robonii_MakeMessage(bufTX,buf2,i);
  chibiTxRaw( bufTX, TXLength);
}

void Robonii_MAC_to_char(uint64_t mac, unsigned char *DestinationBuf)
{
unsigned char i=0;
unsigned char i2=0;
Robonii_addMAc(mac,DestinationBuf, &i);

for (i=0; i<8;i++)
{
Serial.print(DestinationBuf[i], HEX);
Serial.print(" ");
}

DestinationBuf[8] = 0;
}

void Robonii_Print_Msg()
{
unsigned char macbuf[9];
Serial.print("Own MAC: ");
Robonii_MAC_to_char(OwnMac,macbuf);
Serial.println("");
Serial.print("Paired MAC: ");
Robonii_MAC_to_char(PairedMac,macbuf);
Serial.println("");

Serial.print("Dest MAC: ");
Robonii_MAC_to_char(MsgDstMac,macbuf);
Serial.println("");
Serial.print("Source MAC: ");
Robonii_MAC_to_char(MsgSrcMac,macbuf);
Serial.println("");

Serial.print("Msg Header: ");
Serial.println(MsgDataHdr, HEX);
Serial.print("Msg Data Lenght: ");
Serial.println(MsgDataLength, HEX);
Serial.print("Msg Counter: ");
Serial.println(MsgDataCounter, HEX);
Serial.print("Msg Device Type: ");
Serial.println(MsgDataDevType, HEX);
Serial.print("Msg data CRC: ");
Serial.println(MsgDataCRC, HEX);
Serial.print("Data: ");
unsigned char i = 0;
for (i=0; i<MsgDataLength;i++)
{
Serial.print(MsgData[i], HEX);
Serial.print(" ");
}
Serial.println("--");

}


//-------------Remote Display functions---------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------
/** Display functionality */
/////// LCD segments and LED transmit with Zigbee message ////////////
/// 4 LCD/LED states are transmitted per byte so the transmit
/// process entails working with multiples of 4 for contructing 
/// bit mask, bitshifts, etc. The LCD states are placed before the LED states
/// in the zigbee packet and thus is the LED transmit/retrieval dependant of the LCD 
/// last/max index (LCD_MAX_INDEX). Should an additional lcd segment/led ever be
/// added will it only be neccesary to an entry to either the lcd_e/remote_led_e enums
/// in remote.h file order to keep it compatible with the remote code.
    ///////////////////////////////////////////////////////////////////////
	
unsigned char remoteStateArray[REMOTE_DATA_SIZE];	//keep state of the LCD segments and LEDs	
	
void set_lcd(lcd_e lcdSegment, display_state_t displayState)
{
  uint8_t bitShifts = (lcdSegment % 4) * 2;
  uint8_t mask = 0b00000011 << bitShifts;
  uint8_t arrayIndex = lcdSegment / 4;
  uint8_t lcdState = (displayState << bitShifts) & mask;
  remoteStateArray[arrayIndex] &= ~mask; //1st clear what ever is stored in the 2 bits
  remoteStateArray[arrayIndex] |= lcdState;   //then add the new 2 bits of display data
}

void set_remote_led(remote_led_e remoteLed, display_state_t displayState)
{
  uint8_t bitShifts = ((remoteLed + LCD_MAX_INDEX) % 4) * 2;
  uint8_t mask = 0b00000011 << bitShifts;
  uint8_t arrayIndex = (remoteLed + LCD_MAX_INDEX) / 4;
  uint8_t ledState = (displayState << bitShifts) & mask;
  remoteStateArray[arrayIndex] &= ~mask;  //1st clear what ever is stored in the 2 bits
  remoteStateArray[arrayIndex] |= ledState;  //then add the new 2 bits of display data
}

void ClearRemoteDisplay()
{
	memset(&remoteStateArray[0], 0x00, REMOTE_DATA_SIZE);
}

/*--TxRemoteMessage----------------------------------------------------
 Description : This function receives a single remote message by collecting
 		a message from the incoming buffer.
 Parameters  : None.
 Return      : Bool - returns true when a valid message has been transmitted.
*/
void TxRemoteMessage()
{
 byte TXbuf[127];
  // Tx the message.
	unsigned char tempMessage[REMOTE_DATA_SIZE + ZIGBEE_DATA_START_OFFSET + 1+5];
	//unsigned char tempMessage[REMOTE_DATA_SIZE + ZIGBEE_DATA_START_OFFSET + 1];
	
		
	tempMessage[0] = 0xA0;
  tempMessage[1] = 0x13;
  tempMessage[2] = 0xA3;
  
  tempMessage[3] = 0x02;
  tempMessage[4] = 0x5E; //CRC
	
	tempMessage[0+5] = ZIGBEE_APPLICATION;
  #warning TN: Large stack frame for local copy of message - discard!!!
	//memcpy(&tempMessage[ZIGBEE_DATA_START_OFFSET], &remoteStateArray[0], REMOTE_DATA_SIZE);
	memcpy(&tempMessage[ZIGBEE_DATA_START_OFFSET+5], &remoteStateArray[0], REMOTE_DATA_SIZE);
//  PORTJ.OUTTGL = PIN7_bm;
//  PORTD.OUTSET = PIN5_bm;
  
  //Zigbee multi channel - Add next channel so that remote can find robot when jumping to another channel
  //tempMessage[REMOTE_DATA_SIZE + ZIGBEE_DATA_START_OFFSET] = robot_zigbee_channel;     
	tempMessage[REMOTE_DATA_SIZE + ZIGBEE_DATA_START_OFFSET] = 0;  //TIAN To Check
	
 // acks_disabled_flag = true;
	//TalkToPairedDevice( tempMessage, REMOTE_DATA_SIZE + ZIGBEE_DATA_START_OFFSET + 1, PRIORITY_LOW_RETRIES); //send LCD and led info to remote to be displayed  //Outgoing buffer for zigbee
  //Robonii_MakeMessage(TXbuf, (byte*) tempMessage,REMOTE_DATA_SIZE + ZIGBEE_DATA_START_OFFSET + 1);
  //if (clear_remote_disp_array == true)
  //  ClearRemoteDisplay();	
  tempMessage[4] =  Robonii_CalculateCRC(&tempMessage[5],REMOTE_DATA_SIZE + ZIGBEE_DATA_START_OFFSET );
     unsigned char TXLength = 0;
 TXLength=  Robonii_MakeMessage(TXbuf, (byte*) tempMessage,REMOTE_DATA_SIZE + ZIGBEE_DATA_START_OFFSET +5);
 
  chibiTxRaw( TXbuf, TXLength);
  
}

/*--GetLCDNumberTranslation--------------------------------------------
 Description : This function return a char translation for LCD segments.
 Parameters  : char number - The char that needs to be displayed on the LCD.
 Return      : char - The translated char for the LCD.
*/
unsigned char SegmentLookup[16] PROGMEM = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x67,0x77,0x7C,0x39,0x5E,0x79};
                                           //0,  1,   2,   3,   4,   5,   6,   7,   8,   9,   A,    B,   C,   D,   E
unsigned char GetLCDNumberTranslation( unsigned char number )
{
	unsigned char cRemoteLCD = 0x00;
	number = number & 0x0F;
	cRemoteLCD  = pgm_read_byte(&SegmentLookup[number]);
	return cRemoteLCD;
}

//display amount on the 'small numbers'
void SetSmallNumberDisplay(uint8_t amount, display_state_t displayState)
{
	//check if the score is over 100
	if (amount >= 100)
	{	
    amount = amount - 100;
    set_lcd(LCD_SS_A, displayState);
    set_lcd(LCD_SS_B, displayState);
	}
	else
	{
		set_lcd(LCD_SS_A, DS_OFF);
    set_lcd(LCD_SS_B, DS_OFF);
	}
  
  unsigned char lsbSevenSegTranslatedVal = GetLCDNumberTranslation(amount % 10);
  unsigned char msbSevenSegTranslatedVal = GetLCDNumberTranslation(amount / 10);
  SetSmallNumberSpecial(msbSevenSegTranslatedVal, lsbSevenSegTranslatedVal, displayState);
}

//display amount on the 'big numbers'
void SetBigNumberDisplay(uint8_t amount, display_state_t displayState)
{
  unsigned char lsbSevenSegTranslatedVal = GetLCDNumberTranslation(amount % 10);  
  unsigned char msbSevenSegTranslatedVal = GetLCDNumberTranslation(amount / 10);
  SetBigNumberSpecial(msbSevenSegTranslatedVal, lsbSevenSegTranslatedVal, displayState);
}

//display amount on the 'big numbers'
void SetBigNumberSpecial(unsigned char char1, unsigned char char2, display_state_t displayState)
{
  //left seven segment
  set_lcd(LCD_SS_3A, char1 & 0x01 ? displayState : DS_OFF);
  set_lcd(LCD_SS_3B, char1 & 0x02 ? displayState : DS_OFF);
  set_lcd(LCD_SS_3C, char1 & 0x04 ? displayState : DS_OFF);
  set_lcd(LCD_SS_3D, char1 & 0x08 ? displayState : DS_OFF);
  set_lcd(LCD_SS_3E, char1 & 0x10 ? displayState : DS_OFF);
  set_lcd(LCD_SS_3F, char1 & 0x20 ? displayState : DS_OFF);
  set_lcd(LCD_SS_3G, char1 & 0x40 ? displayState : DS_OFF);

  //right seven segment (LSB number)
  set_lcd(LCD_SS_4A, char2 & 0x01 ? displayState : DS_OFF);
  set_lcd(LCD_SS_4B, char2 & 0x02 ? displayState : DS_OFF);
  set_lcd(LCD_SS_4C, char2 & 0x04 ? displayState : DS_OFF);
  set_lcd(LCD_SS_4D, char2 & 0x08 ? displayState : DS_OFF);
  set_lcd(LCD_SS_4E, char2 & 0x10 ? displayState : DS_OFF);
  set_lcd(LCD_SS_4F, char2 & 0x20 ? displayState : DS_OFF);
  set_lcd(LCD_SS_4G, char2 & 0x40 ? displayState : DS_OFF);
}

//display amount on the 'small numbers'
void SetSmallNumberSpecial(unsigned char char1, unsigned char char2, display_state_t displayState)
{
  //left seven segment
  set_lcd(LCD_SS_1A, char1 & 0x01 ? displayState : DS_OFF);
  set_lcd(LCD_SS_1B, char1 & 0x02 ? displayState : DS_OFF);
  set_lcd(LCD_SS_1C, char1 & 0x04 ? displayState : DS_OFF);
  set_lcd(LCD_SS_1D, char1 & 0x08 ? displayState : DS_OFF);
  set_lcd(LCD_SS_1E, char1 & 0x10 ? displayState : DS_OFF);
  set_lcd(LCD_SS_1F, char1 & 0x20 ? displayState : DS_OFF);
  set_lcd(LCD_SS_1G, char1 & 0x40 ? displayState : DS_OFF);

  //right seven segment
  set_lcd(LCD_SS_2A, char2 & 0x01 ? displayState : DS_OFF);
  set_lcd(LCD_SS_2B, char2 & 0x02 ? displayState : DS_OFF);
  set_lcd(LCD_SS_2C, char2 & 0x04 ? displayState : DS_OFF);
  set_lcd(LCD_SS_2D, char2 & 0x08 ? displayState : DS_OFF);
  set_lcd(LCD_SS_2E, char2 & 0x10 ? displayState : DS_OFF);
  set_lcd(LCD_SS_2F, char2 & 0x20 ? displayState : DS_OFF);
  set_lcd(LCD_SS_2G, char2 & 0x40 ? displayState : DS_OFF);
}

//set bars on remote
void SetRemoteBars(uint8_t amount, display_state_t displayState)
{			
	set_lcd(LCD_BAR4, (amount >= 4) ? displayState : DS_OFF);
	set_lcd(LCD_BAR3, (amount >= 3) ? displayState : DS_OFF);
	set_lcd(LCD_BAR2, (amount >= 2) ? displayState : DS_OFF);
	set_lcd(LCD_BAR1, (amount >= 1) ? displayState : DS_OFF);
}

//set bullets on Remote
void SetRemoteBullets(uint8_t amount, display_state_t displayState)
{
	set_lcd(LCD_BULLET4, (amount >= 4) ? displayState : DS_OFF);
	set_lcd(LCD_BULLET3, (amount >= 3) ? displayState : DS_OFF);
	set_lcd(LCD_BULLET2, (amount >= 2) ? displayState : DS_OFF);
	set_lcd(LCD_BULLET1, (amount >= 1) ? displayState : DS_OFF);
}

//set leds on remote
void SetRemoteLEDs(uint8_t amount, display_state_t displayState)
{
	set_remote_led(REMOTE_LED1, (amount >= 1) ? displayState : DS_OFF);
	set_remote_led(REMOTE_LED2, (amount >= 2) ? displayState : DS_OFF);
	set_remote_led(REMOTE_LED3, (amount >= 3) ? displayState : DS_OFF);
	set_remote_led(REMOTE_LED4, (amount >= 4) ? displayState : DS_OFF);
  set_remote_led(REMOTE_LED5, (amount >= 5) ? displayState : DS_OFF);
  set_remote_led(REMOTE_LED6, (amount >= 6) ? displayState : DS_OFF);
}


#endif


