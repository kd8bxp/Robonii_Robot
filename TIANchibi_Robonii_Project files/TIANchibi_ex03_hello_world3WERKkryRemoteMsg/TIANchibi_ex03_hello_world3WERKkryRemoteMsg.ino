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

void loop()
{ 
  // We're going to automatically calculate the length
  // of the message string using strlen(). We add 1 to the
  // length to handle the terminating character '/0'.
  // We're also sending a broadcast so any node in listening
  // range will hear the message. 
  
  unsigned char TextLen = strlen((const char*)msg);
  int tint = (int) TextLen;
  chibiTx(BROADCAST_ADDR, msg, TextLen+1);
  Serial.print("Text Length: ");
  Serial.println(tint);
  //chibiTx(BROADCAST_ADDR, msg, 24);

  // We're going to print anything we receive in case there are others 
  // transmitting.
  if (chibiDataRcvd() == true)
  {
    chibiGetData(buf);
    Serial.print((char *)boardName);
    Serial.print(": ");
    Serial.println((char *)buf);
  }

  // delay half a second between transmission
  delay(500);
  digitalWrite(13, !digitalRead(13));    // set the LED off
 // PORTK.DIRSET = 0x05; //TIAN
}
