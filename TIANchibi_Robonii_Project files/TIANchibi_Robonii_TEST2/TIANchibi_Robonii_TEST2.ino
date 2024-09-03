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
   //sendRemoteMessagePairedAvailable(0x11);
   Robonii_sendRemoteMessagePairedAvailable();
 }else
 {
   //sendRemoteMessageSequence(counter);
   Robonii_sendRemoteMessageControlTest();
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



  
  

