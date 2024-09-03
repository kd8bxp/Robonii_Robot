//#include <avrlibdefs.h>
//#include <avrlibtypes.h>
#include <Driver_RFID.h>

#include <xTimer.h>
#include <dma_driver.h>
#include <Robonii_Sound.h>

#include <Robonii_Motor.h>

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
  
  init_LED();
  
  //Inititialize the Timer to excecute every 1ms (1000 * 1us)
  TimerD1A.initialize(1000);
  
  Robonii_Sounds.InitSound();  
  
  RFID.rfid_init();

//Enable RFID
  e_rfid_state ee = RFID_ENABLED_ON;
  RFID.rfid_set_state(ee);
  
    Robonii_Motors.InitMotor();
  // Init the chibi wireless stack
  chibiInit();
  
  // set the channel to channel 20. channel 20 is out of band of
  // 802.11 wi-fi channels
  chibiSetChannel(20);
  
 
  
  chibiEEP_Read_BoarName(boardName,&nameLen);
  if ((nameLen==0) || (nameLen==255))
  {
     //chibiEEP_Write_BoardName("MaX 6",6);  // Store Robonii name in EEPROM
     //chibiEEP_Write_BoardName("Sam 5",6);  // Store Robonii name in EEPROM if not done before
     Serial.println("Writing name in EEPROM:  ");
  }
  
  chibiEEP_Read_BoarName(boardName,&nameLen);
   Serial.print("Robonii Name:  ");
   Serial.println((char *)boardName);
}

unsigned char RemoteMsgLen=0;
unsigned int counter = 0;
unsigned int DoOnce = 0;

unsigned char NoRxCounter = 0;

void loop()
{ 
 

Serial.print("-");
counter++;

if (counter>100)
{
 counter=0;
 Serial.println("100");
}

if (RFID.rfid_flag_tag_data_updated)
  {
    //Tag Read - Print tag ID to USB
    unsigned char rfid_TAG[10];
    RFID.GetRFID(rfid_TAG);
    digitalWrite(D5_Red, !digitalRead(D5_Red));    // set the LED on/off
   Serial.print("TAG Read: ");
    Serial.print(rfid_TAG[0]);
    Serial.print(rfid_TAG[1]);
    Serial.print(rfid_TAG[2]);
    Serial.print(rfid_TAG[3]);
    Serial.println(rfid_TAG[4]);    
    
  
  //Sounds to play : 1-20
  if (rfid_TAG[4] == 2)
  {
    //Play only if certain tag
    Robonii_Sounds.PlaySound(1,3);  
    delay(1000);  
  }
  }

  // We're going to print anything we receive in case there are others 
  // transmitting.
  if (chibiDataRcvd() == true)
  {
     RemoteMsgLen=chibiGetDataRaw(buf);  // Add RAW message to buffer.  This is used for the ROBONII protocol.
     
     
     // Parse the Robonii message. This will: 
               // -send a "paired device availbe" if "Robonii_Auto_Paired_Reply" configured in ".h" file
               // -learn own and paired MAC address if not set, 
               // -parse the remote keys.
     Robonii_Parse_Msg(buf, RemoteMsgLen); 
     
     
  if ((buf[22] == 0xA0))
  {
  //  Serial.println(" ++++ A0 +++++++");
   // Serial.print((char *)boardName);
    Serial.print(": ");
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
 
 
   //sendRemoteMessageSequence(counter);
   
   //Robonii_sendRemoteMessageControlTest();//WORKS - Normal startup leds;;
   SetRemoteLEDs(2,DS_ON);
   SetRemoteLEDs(3,DS_SLOW_FLICKER);
   SetRemoteBullets(3,DS_ON);
   SetSmallNumberDisplay(22,DS_ON);
   SetBigNumberDisplay(88, DS_SLOW_FLICKER);
   
   set_lcd(LCD_FACE_HAPPY,DS_ON);
   set_lcd(LCD_FACE,DS_ON);
   set_lcd(LCD_BASE_STATION,DS_ON);
   
   SetRemoteBars(4,DS_ON);
   
   TxRemoteMessage();
    
   

 
 unsigned char Keys = Robonii_Remote_Key_Pressed();
 Serial.println("Keys: ");
 Serial.println(Keys,HEX);
 if (Keys != REMOTE_NONE)
 {
   if ((Keys != 0))
   {
     NoRxCounter = 0;
   }
 
   if ((Keys & REMOTE_LEFT_FORWARD))
   {
     digitalWrite(D2_Green, LED_ON);    // set the LED off
    // activateMotorWithTimeStop(Robonii_Motor_Left,90);
   }else
   {
     digitalWrite(D2_Green, LED_OFF);    // set the LED off
   }
   
   if ((Keys & REMOTE_LEFT_BACKWARD))
   {
     digitalWrite(D2_Red, LED_ON);    // set the LED off
    // activateMotorWithTimeStop(Robonii_Motor_Left,-90);
   }else
   {
     digitalWrite(D2_Red, LED_OFF);    // set the LED off
     
   }
   
   if ((Keys & REMOTE_LEFT_TRIGGER))
   {
     digitalWrite(D3_Green, LED_ON);    // set the LED on
     
   }else
   {
     
     digitalWrite(D3_Green, LED_OFF);    // set the LED off
   }
   
   if ((Keys & REMOTE_RIGHT_BACKWARD))
   {
     activateMotorWithTimeStop(Robonii_Motor_Right,-90);
     digitalWrite(D4_Red, LED_ON);    // set the LED off
   }else
   {
     digitalWrite(D4_Red, LED_OFF);    // set the LED off
   }
   
   if ((Keys & REMOTE_RIGHT_FORWARD))
   {
     activateMotorWithTimeStop(Robonii_Motor_Right,90);
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
    NoRxCounter = 0;
    
    digitalWrite(D3_Red, !digitalRead(D3_Red));    // set the LED on/off
  }
  
}

 if (NoRxCounter>50)
 {
   clear_ROBONII_LEDs();
   Robonii_Motors.Brake(); // Stop if no remote message received in 1s, else will drive for ever if remote is off...
   
 }else
 {
    NoRxCounter++;
 }
//digitalWrite(13, 0);    // set the LED off
}










void stopLeftMotor()
{
  //Robonii_Sounds.PlaySound(1,3);  
   Robonii_Motor_Left.Brake();
}

void stopRightMotor()
{
  //Robonii_Sounds.PlaySound(1,3);  
   Robonii_Motor_Right.Brake();
}

void activateMotorWithTimeStop(Robonii_Motors_Driver motor, char power)
{
  //motor.SetSpeed(power);
  
  if (motor.motorSide == Motor_Left)
  {
     Robonii_Motor_Left.SetSpeed(power);
    TimerD1A.attachInterrupt1(stopLeftMotor,1000); // Will stop motors if no new motor command received from remote for 1s...
  }
  
  if (motor.motorSide == Motor_Right)
  {
   Robonii_Motor_Right.SetSpeed(power);
    TimerD1A.attachInterrupt2(stopRightMotor,1000); // Will stop motors if no new motor command received from remote for 1s...
  }
  
 
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



  
  

