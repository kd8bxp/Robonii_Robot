#include "Robonii_LIB.h"
#include <Arduino.h> 

void init_LED()
{
   
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
  
  unsigned char i;
  for (i=D1_Green; i<=D5_Red; i++)
  {
    digitalWrite(i, LED_OFF);   
  }
  
}

void init_Bumpers()
{
  pinMode(Bumper_Front, INPUT); 
  pinMode(Bumper_Rear, INPUT); 
}

void set_ROBONII_LED(unsigned char led_Nr, LED_e led_Status)
{
if (led_Nr <=5)
{
led_Nr = ((led_Nr-1)*2)+D1_Green;
}
 if (led_Status==ROBONII_LED_RED)
 {
   digitalWrite(led_Nr+1, LED_ON);
   digitalWrite(led_Nr, LED_OFF);   
 }
 if (led_Status==ROBONII_LED_GREEN)
 {
   digitalWrite(led_Nr+1, LED_OFF);
   digitalWrite(led_Nr, LED_ON);   
 }
 if (led_Status==ROBONII_LED_YELLOW)
 {
   digitalWrite(led_Nr, LED_ON);
   if ((led_Nr%2)==0)
   {
     digitalWrite(led_Nr+1, LED_ON);   
   }else
   {
     digitalWrite(led_Nr-1, LED_ON);   
   }
 }
 if (led_Status==ROBONII_LED_OFF)
 {
   digitalWrite(led_Nr, LED_OFF);
   digitalWrite(led_Nr+1, LED_OFF);   
 }
  
}

void clear_ROBONII_LEDs()
{
unsigned char i=0;

  for (i=D1_Green; i<=D5_Green+1; i++)
  {    
    set_ROBONII_LED(i,ROBONII_LED_OFF);   // set the LED off
  }

}

void knight_Rider(unsigned char Delay_ms1)
{
 unsigned char i=0;

  for (i=1; i<=5; i++)
  {
    set_ROBONII_LED(i,ROBONII_LED_RED);
    delay(Delay_ms1);  
    set_ROBONII_LED(i,ROBONII_LED_OFF);   // set the LED off
  }
 
  for (i=5; i>=1; i--)
  {
    set_ROBONII_LED(i,ROBONII_LED_RED);
    delay(Delay_ms1);  
    set_ROBONII_LED(i,ROBONII_LED_OFF);   // set the LED off
  }
 delay(Delay_ms1);  
}




