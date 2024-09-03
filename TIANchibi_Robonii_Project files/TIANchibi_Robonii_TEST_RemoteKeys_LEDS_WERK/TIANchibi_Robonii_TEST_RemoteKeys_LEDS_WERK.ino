#include <Robonii_LIB.h>

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
 /*
  pinMode(13, OUTPUT);   
pinMode(D1_Green, OUTPUT);   
pinMode(D2_Green, OUTPUT);   
pinMode(D3_Green, OUTPUT);   
pinMode(D4_Green, OUTPUT);   
pinMode(D5_Green, OUTPUT); 

pinMode(D5_Red, OUTPUT); 
pinMode(D4_Red, OUTPUT); 
pinMode(D3_Red, OUTPUT); 
pinMode(D2_Red, OUTPUT); 
pinMode(D1_Red, OUTPUT); 

  
  digitalWrite(13, LOW);    // set the LED off
  */
  init_LED();
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
     
     Robonii_Parse_Msg(buf, RemoteMsgLen);
     
     
  if ((buf[22] == 0xA0))
  {
  //  Serial.println(" ++++ A0 +++++++");
   // Serial.print((char *)boardName);
    //Serial.print(": ");
   // Serial.println((char *)buf);
    
    
   // printRemoteMsg(buf,RemoteMsgLen);
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
  // Robonii_sendRemoteMessagePairedAvailable();
 }else
 {
   //sendRemoteMessageSequence(counter);
   
   //Robonii_sendRemoteMessageControlTest();//WORKS - Normal startup leds;;
   SetRemoteLEDs(2,DS_ON);
   SetRemoteLEDs(3,DS_SLOW_FLICKER);
   SetRemoteBullets(3,DS_ON);
   SetSmallNumberDisplay(22,DS_ON);
    SetBigNumberDisplay(88, DS_SLOW_FLICKER);
   TxRemoteMessage();
   
   
   
 }
 
 unsigned char Keys = Robonii_Remote_Key_Pressed();
 Serial.println("Keys: ");
 Serial.println(Keys,HEX);
 if (Keys != REMOTE_NONE)
 {
 
   if ((Keys & REMOTE_LEFT_FORWARD))
   {
     digitalWrite(D2_Green, LED_ON);    // set the LED off
   }else
   {
     digitalWrite(D2_Green, LED_OFF);    // set the LED off
   }
   
   if ((Keys & REMOTE_LEFT_BACKWARD))
   {
     //digitalWrite(D2_Red, LED_ON);    // set the LED off
     set_ROBONII_LED(D2_LED,ROBONII_LED_RED);  // set the LED on
   }else
   {
     //digitalWrite(D2_Red, LED_OFF);    // set the LED off
     set_ROBONII_LED(D2_LED,ROBONII_LED_OFF);
   }
   
   if ((Keys & REMOTE_LEFT_TRIGGER))
   {
     //digitalWrite(D3_Green, LED_ON);    // set the LED on
     set_ROBONII_LED(D3_LED,ROBONII_LED_GREEN);
   }else
   {
     set_ROBONII_LED(D3_LED,ROBONII_LED_OFF);
     //digitalWrite(D3_Green, LED_OFF);    // set the LED off
   }
   
   if ((Keys & REMOTE_RIGHT_BACKWARD))
   {
     digitalWrite(D4_Red, LED_ON);    // set the LED off
   }else
   {
     digitalWrite(D4_Red, LED_OFF);    // set the LED off
   }
   
   if ((Keys & REMOTE_RIGHT_FORWARD))
   {
     digitalWrite(D4_Green, LED_ON);    // set the LED off
   }else
   {
     digitalWrite(D4_Green, LED_OFF);    // set the LED off
   }
   
   if ((Keys & REMOTE_RIGHT_TRIGGER))
   {
     digitalWrite(D5_Red, LED_ON);    // set the LED off
   }else
   {
     digitalWrite(D5_Red, LED_OFF);    // set the LED off
   }
 }else
{
  if (Keys == REMOTE_NONE)
  {
    clear_ROBONII_LEDs();
  }
  
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



  
  

