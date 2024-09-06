/*******************************************************************
    Copyright (C) 2009 FreakLabs
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions
    are met:

    1. Redistributions of source code must retain the above copyright
       notice, this list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright
       notice, this list of conditions and the following disclaimer in the
       documentation and/or other materials provided with the distribution.
    3. Neither the name of the the copyright holder nor the names of its contributors
       may be used to endorse or promote products derived from this software
       without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE THE COPYRIGHT HOLDERS AND CONTRIBUTORS ``AS IS'' AND
    ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
    ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
    FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
    DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
    OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
    HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
    LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
    OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
    SUCH DAMAGE.

    Originally written by Christopher Wang aka Akiba.
    Please post support questions to the FreakLabs forum.

*******************************************************************/
/*!
    \file 
    \ingroup


*/
/**************************************************************************/
#ifndef TIANCHIBI_H
#define TIANCHIBI_H

// For handling Arduino 1.0 compatibility and backwards compatibility
#if ARDUINO >= 100
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif

//#include "chibiUsrCfg.h"
#include "src/chb_eeprom.h"
#include "src/chb_spi.h"
#include "src/chb.h"
#include "src/chb_drvr.h"
//#include "src/Robonii_coms.h"
#include "Remote_Common.h"

#define BROADCAST_ADDR 0xFFFF


#define EEP_ROBONI_BOARD_NAME_LENGTH	128*2
#define EEP_ROBONI_BOARD_NAME			EEP_ROBONI_BOARD_NAME_LENGTH+1




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




void chibiEEP_Write_BoardName( char *name,char length);
void chibiEEP_Read_BoarName( char *name,char *length);



void chibiInit();
void chibiSetShortAddr(uint16_t addr);
uint16_t chibiGetShortAddr();
void chibiSetIEEEAddr(uint8_t *ieee_addr);
void chibiGetIEEEAddr(uint8_t *ieee_addr);
uint8_t chibiRegRead(uint8_t addr);
void chibiRegWrite(uint8_t addr, uint8_t val);
uint8_t chibiTx(uint16_t addr, uint8_t *data, uint8_t len);
uint8_t chibiTxRaw( uint8_t *data, uint8_t len);
uint8_t chibiDataRcvd();
uint8_t chibiGetData(uint8_t *data);
uint8_t chibiGetDataRaw(uint8_t *data);
uint8_t chibiGetRSSI();
uint16_t chibiGetSrcAddr();
uint8_t chibiSetChannel(uint8_t channel);
uint8_t chibiGetPartID();
void chibiSleepRadio(uint8_t enb);
void chibiCmdInit(uint32_t speed);
void chibiCmdPoll();
void chibiCmdAdd(char *name, void (*func)(int argc, char **argv));
uint32_t chibiCmdStr2Num(char *str, uint8_t base);

void chibiAesInit(uint8_t *key);
uint8_t chibiAesEncrypt(uint8_t len, uint8_t *plaintext, uint8_t *ciphertext);
uint8_t chibiAesDecrypt(uint8_t len, uint8_t *plaintext, uint8_t *ciphertext);
void chibiSetDataRate(uint8_t rate);
uint8_t chibiGetRand();
void chibiSetMode(uint8_t mode);

void chibiAesTest(uint8_t *key);

#if (FREAKDUINO_LONG_RANGE == 1)    
    void chibiHighGainModeEnable();
    void chibiHighGainModeDisable();
#endif



void chb_eeprom_write(U16 addr, U8 *buf, U16 size);
void chb_eeprom_read(U16 addr, U8 *buf, U16 size);
void eeprom_read(unsigned int addr, char *buf, unsigned int size);
void eeprom_write(unsigned int addr, char *buf, unsigned int size);

static void eep_write_byte( unsigned int addr, char value );
static unsigned char eep_read_byte(unsigned int addr);
//-------------------------------------------------------------------------
void chb_spi_init();
U8 chb_xfer_byte(U8 data);
char spi_read_byte(char addr);
char spi_xfer_byte(char data);
void spi_init();


uint8_t Test1();

unsigned char Robonii_MakeMessage(unsigned char *txBuffer, unsigned char *data, unsigned int dataLentgh);
unsigned char Robonii_Parse_Msg(unsigned char *data, unsigned char DataLength);
void Robonii_sendRemoteMessagePairedAvailable();
void Robonii_sendRemoteMessageControlTest();
void Robonii_Print_Msg();
unsigned char Robonii_Remote_Key_Pressed();
void TxRemoteMessage();

void SetBigNumberDisplay(uint8_t amount, display_state_t displayState);
void SetSmallNumberDisplay(uint8_t amount, display_state_t displayState);
void SetBigNumberSpecial(unsigned char char1, unsigned char char2, display_state_t displayState);
void SetSmallNumberSpecial(unsigned char char1, unsigned char char2, display_state_t displayState);
void SetRemoteBars(uint8_t amount, display_state_t displayState);
void SetRemoteBullets(uint8_t amount, display_state_t displayState);
void SetRemoteLEDs(uint8_t amount, display_state_t displayState);
void set_lcd(lcd_e lcdSegment, display_state_t displayState);

#endif

