/* Chibi for Arduino, Example 3
This example is the same as the previous examples except that it 
changes the channel to one that is not occupied by any 802.11 Wi-Fi
device. Its basically just to show how to change the channel.
*/

#include <Tianchibi.h>

byte msg[] = "Hello World";

byte buf[CHB_MAX_PAYLOAD];
char boardName[20];
char nameLen = 0;

void setup()
{
 // Serial.begin(57600);
  pinMode(13, OUTPUT);     
  digitalWrite(13, LOW);    // set the LED off
  // Init the chibi wireless stack
  chibiInit();
  
  // set the channel to channel 20. channel 20 is out of band of
  // 802.11 wi-fi channels
  chibiSetChannel(20);
  
  //chibiEEP_Write_BoardName("MaX 6",6);
  //chibiEEP_Write_BoardName("Sam 5",6);
  
  chibiEEP_Read_BoarName(boardName,&nameLen);
  
  
}

unsigned char RemoteMsgLen=0;
unsigned int counter = 0;
unsigned int DoOnce = 0;

void loop()
{ 
  // We're going to automatically calculate the length
  // of the message string using strlen(). We add 1 to the
  // length to handle the terminating character '/0'.
  // We're also sending a broadcast so any node in listening
  // range will hear the message. 
  
  unsigned char TextLen = strlen((const char*)msg);
  int tint = (int) TextLen;
 // chibiTx(BROADCAST_ADDR, msg, TextLen+1);
 // Serial.print("Text Length: ");
 // Serial.println(tint);
 // chibiTx(BROADCAST_ADDR, msg, 24);
Serial.print("-");
counter++;

if (counter>64)
{
 counter=0;
 Serial.println("64");
}
  // We're going to print anything we receive in case there are others 
  // transmitting.
  if (chibiDataRcvd() == true)
  {
     RemoteMsgLen=chibiGetDataRaw(buf);
  if ((buf[22] == 0xA5)||(buf[22] == 0xA7))
  {
    Serial.println(" ++++++++ A7 +++++++++++++");
     Serial.print((char *)boardName);
    Serial.print(": ");
    Serial.println((char *)buf);
    
    
    printRemoteMsg(buf,RemoteMsgLen);
    counter=0;
  }
   
   
  }

  // delay half a second between transmission
  delay(10);
  digitalWrite(13, !digitalRead(13));    // set the LED off
 // PORTK.DIRSET = 0x05; //TIAN
 //sendRemoteMessage();
 //sendRemoteMessageSequence(counter);
 
 if (DoOnce<1)
 {
   DoOnce++;
   sendRemoteMessagePairedAvailableToets(0x11);
 }else
 {
   sendRemoteMessageSequence(counter);
 }
 
 
 
//digitalWrite(13, 0);    // set the LED off
}

void printRemoteMsg(byte *buf, unsigned char len)
{
  int i=0;
  Serial.println(" ");
  Serial.print("Lentgh size: ");
    Serial.print(len);
  //  Serial.print("     RSSI Value:");
   // Serial.println((int)chibiGetRSSI());
  for (i=0;i<len;i++)
  {
  // Serial.print("Char: ");
  // Serial.print(i);
   Serial.print(" ");
   Serial.print((int)buf[i], HEX); 
  // Serial.print(" : ");
  // Serial.println((int)buf[i]);
  }
  Serial.println(" ----");
  if (buf[22] == 0xA5)
  {
    Serial.println(" ++++++++ A7 +++++++++++++");
  }
  
}


void sendRemoteMessagePairedAvailable(unsigned char counter)
{
  byte buf2[100];
  
  buf2[0] = 0x1C;
  buf2[1] = 0x61;
  buf2[2] = 0xCC;
  buf2[3] = 0x50;
  buf2[3] = counter;
  
  buf2[4] = 0xFE;
  buf2[5] = 0x22;
  
  buf2[6] = 0x8B;
  buf2[7] = 0x16;
  buf2[8] = 0x00;
  buf2[9] = 0x00;
  buf2[10] = 0x01;
  buf2[11] = 0xd5;
  buf2[12] = 0x25;
  buf2[13] = 0x00;
  
  buf2[14] = 0x01;
  buf2[15] = 0x00;
  buf2[16] = 0x00;
  buf2[17] = 0x00;
  buf2[18] = 0x00;
  buf2[19] = 0xd5;
  buf2[20] = 0x25;
  buf2[21] = 0x00;
  /*
  buf2[22] = 0xA5;
  buf2[23] = 0x00;
  buf2[24] = 0x2D;
  
  buf2[25] = 0x02;
  buf2[26] = 0x00;
  buf2[27] = 0x21;
  */
  unsigned char i=22;
   buf2[i++] = 0xA7;
  buf2[i++] = 0x00;
  buf2[i++] = 0xE3;
  
  buf2[i++] = 0x02;
  buf2[i++] = 0x00;
  //buf2[i++] = 0xA3;
  unsigned char crc = CalculateCRC(buf2,i);
  buf2[i++] = crc;
  
  /*
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
  unsigned char crc = CalculateCRC(buf2,i);
  buf2[i++] = crc;*/
  
  
  
  
  
  
  //chibiTx( BROADCAST_ADDR,buf2, 28);
  chibiTxRaw( buf2, i);
}

void sendRemoteMessagePairedAvailableToets(unsigned char counter)
{
  byte buf2[100];
    byte bufTX[100];
  unsigned char bufTXLenght;
 /* 
  buf2[0] = 0x1C;
  buf2[1] = 0x61;
  buf2[2] = 0xCC;
  buf2[3] = 0x50;
  buf2[3] = counter;
  
  buf2[4] = 0xFE;
  buf2[5] = 0x22;
  
  buf2[6] = 0x8B;
  buf2[7] = 0x16;
  buf2[8] = 0x00;
  buf2[9] = 0x00;
  buf2[10] = 0x01;
  buf2[11] = 0xd5;
  buf2[12] = 0x25;
  buf2[13] = 0x00;
  
  buf2[14] = 0x01;
  buf2[15] = 0x00;
  buf2[16] = 0x00;
  buf2[17] = 0x00;
  buf2[18] = 0x00;
  buf2[19] = 0xd5;
  buf2[20] = 0x25;
  buf2[21] = 0x00;
  /*
  buf2[22] = 0xA5;
  buf2[23] = 0x00;
  buf2[24] = 0x2D;
  
  buf2[25] = 0x02;
  buf2[26] = 0x00;
  buf2[27] = 0x21;
  */
  unsigned char i=0;
   buf2[i++] = 0xA7;
  buf2[i++] = 0x00;
  buf2[i++] = 0xE3;
  
  buf2[i++] = 0x02;
  buf2[i++] = 0x00;
  

 bufTXLenght= MakeMessage(bufTX,buf2,i);
 
  chibiTxRaw( bufTX, bufTXLenght);
  //buf2[i++] = 0xA3;
 // unsigned char crc = CalculateCRC(buf2,i);
  //buf2[i++] = crc;
  
  /*
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
  unsigned char crc = CalculateCRC(buf2,i);
  buf2[i++] = crc;*/
  
  
  
  
  
  
  //chibiTx( BROADCAST_ADDR,buf2, 28);
 
}

/*
void printRemoteMsg(byte *buf, unsigned char len)
{
  int i=0;
  Serial.println(" ");
  Serial.print("Lentgh size: ");
    Serial.print(len);
  //  Serial.print("     RSSI Value:");
   // Serial.println((int)chibiGetRSSI());
  for (i=0;i<len;i++)
  {
  // Serial.print("Char: ");
  // Serial.print(i);
   Serial.print(" ");
   Serial.print((int)buf[i], HEX); 
  // Serial.print(" : ");
  // Serial.println((int)buf[i]);
  }
  Serial.println(" ----");
  if (buf[22] == 0xA5)
  {
    Serial.println(" ++++++++ A7 +++++++++++++");
  }
  
}

*/
void sendRemoteMessageSequence(unsigned char counter)
{
  byte buf2[100];
  
  buf2[0] = 0x2F;
  buf2[1] = 0x61;
  buf2[2] = 0xCC;
  //buf2[3] = 0xCD;
  buf2[3] = counter;
  
  buf2[4] = 0xFE;
  buf2[5] = 0x22;
  
  buf2[6] = 0x8B;
  buf2[7] = 0x16;
  buf2[8] = 0x00;
  buf2[9] = 0x00;
  buf2[10] = 0x01;
  buf2[11] = 0xd5;
  buf2[12] = 0x25;
  buf2[13] = 0x00;
  
  buf2[14] = 0x01;
  buf2[15] = 0x00;
  buf2[16] = 0x00;
  buf2[17] = 0x00;
  buf2[18] = 0x00;
  buf2[19] = 0xd5;
  buf2[20] = 0x25;
  buf2[21] = 0x00;
  /*
  buf2[22] = 0xA5;
  buf2[23] = 0x00;
  buf2[24] = 0x2D;
  
  buf2[25] = 0x02;
  buf2[26] = 0x00;
  buf2[27] = 0x21;
  */
  unsigned char i=22;
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
  unsigned char crc = CalculateCRC(buf2,i);
  buf2[i++] = crc;
  
  
  
  
  
  
  //chibiTx( BROADCAST_ADDR,buf2, 28);
  chibiTxRaw( buf2, i);
}


void sendRemoteMessage()
{
  byte buf2[100];
  
  buf2[0] = 0x2F;
  buf2[1] = 0x61;
  buf2[2] = 0xCC;
  buf2[3] = 0xCD;
  buf2[4] = 0xFE;
  buf2[5] = 0x22;
  
  buf2[6] = 0x8B;
  buf2[7] = 0x16;
  buf2[8] = 0x00;
  buf2[9] = 0x00;
  buf2[10] = 0x01;
  buf2[11] = 0xd5;
  buf2[12] = 0x25;
  buf2[13] = 0x00;
  
  buf2[14] = 0x01;
  buf2[15] = 0x00;
  buf2[16] = 0x00;
  buf2[17] = 0x00;
  buf2[18] = 0x00;
  buf2[19] = 0xd5;
  buf2[20] = 0x25;
  buf2[21] = 0x00;
  /*
  buf2[22] = 0xA5;
  buf2[23] = 0x00;
  buf2[24] = 0x2D;
  
  buf2[25] = 0x02;
  buf2[26] = 0x00;
  buf2[27] = 0x21;
  */
  unsigned char i=22;
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
  
  buf2[i++] = 0xB9;
  
  
  
  
  
  
  
  
  //chibiTx( BROADCAST_ADDR,buf2, 28);
  chibiTxRaw( buf2, i);
}


void sendRemoteMessageWerk()
{
  /*
  byte buf2[100];
  
  buf2[0] = 0x2F;
  buf2[1] = 0x61;
  buf2[2] = 0xCC;
  buf2[3] = 0xCD;
  buf2[4] = 0xFE;
  buf2[5] = 0x22;
  
  buf2[6] = 0x8B;
  buf2[7] = 0x16;
  buf2[8] = 0x00;
  buf2[9] = 0x00;
  buf2[10] = 0x01;
  buf2[11] = 0xd5;
  buf2[12] = 0x25;
  buf2[13] = 0x00;
  
  buf2[14] = 0x01;
  buf2[15] = 0x00;
  buf2[16] = 0x00;
  buf2[17] = 0x00;
  buf2[18] = 0x00;
  buf2[19] = 0xd5;
  buf2[20] = 0x25;
  buf2[21] = 0x00;
  /*
  buf2[22] = 0xA5;
  buf2[23] = 0x00;
  buf2[24] = 0x2D;
  
  buf2[25] = 0x02;
  buf2[26] = 0x00;
  buf2[27] = 0x21;
  */
  /*
  unsigned char i=22;
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
  
  buf2[i++] = 0xB9;
  
  
  
  
  
  
  
  
  //chibiTx( BROADCAST_ADDR,buf2, 28);
  chibiTxRaw( buf2, i);
  */
}

/**
 * \brief Calculate CRC.
 *
 * This function is used to calculate the CRC for a message with a length smaller than 255 chars.
 * \param  *data The data pointer that points to a memory location.
 * \param  length The length of the data.
 * \return unsigned char The byte containing the CRC is returned.
*/
unsigned char CalculateCRC(uint8_t *data, uint8_t length)
{
	unsigned char crcCalc = 0x00;
	for(uint8_t i=0; i< length; i++)
		crcCalc ^= data[i];

	return crcCalc;
}


void addMAc(uint64_t MacToAdd,unsigned char *data, unsigned char *pos)
{
  int i = 0;
  
  for (i=0; i<8;i++)
  {
    data[(*pos)++]= MacToAdd%256;
    
    MacToAdd = MacToAdd/256;
  }
}


unsigned char MsgCounter = 2;
unsigned char MakeMessage(unsigned char *txBuffer, unsigned char *data, unsigned int dataLentgh)
{
  //8B 16 0 0 1 D5 25 0
 uint64_t OwnMac =    0x25D5010000168B;
 
 //1 0 0 0 0 D5 25 0
 uint64_t PairedMac = 0x25D50000000001;
 
 unsigned int PANID = 0x22FE;
 
  byte buf2[127];
  unsigned char i=1;
  
  txBuffer[0] = 22+dataLentgh+1; //Header + data + crc
  txBuffer[i++] = 0x61;
  txBuffer[i++] = 0xCC;
  txBuffer[i++] =MsgCounter++;
  
  
  txBuffer[i++] = PANID%256;
  txBuffer[i++] = PANID/256;
  
   addMAc(OwnMac,txBuffer,&i);
  addMAc(PairedMac,txBuffer,&i);
 
  
  int dataCount = 0;
  for (dataCount=0;dataCount<dataLentgh;dataCount++)
  {
  txBuffer[i++] =data[dataCount]  ;
  }
  
  unsigned char crc = CalculateCRC(buf2,i);
  txBuffer[i++] = crc;
  return i;
}

