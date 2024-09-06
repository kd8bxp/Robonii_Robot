#include <Robonii_LIB.h>

/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.
 
  This example code is in the public domain.
 */

unsigned char x;
void setup() {                
  // initialize the digital pin as an output.
  // Pin 13 has an LED connected on most Arduino boards:
  init_LED();    
  
   set_ROBONII_LED(D3_LED,ROBONII_LED_RED);  // set the LED on
  delay(1500);              // wait for a 1.5 second
  set_ROBONII_LED(D3_LED,ROBONII_LED_GREEN);   // set the LED on
  delay(1500);              // wait for a 1.5 second
  set_ROBONII_LED(D3_LED,ROBONII_LED_YELLOW);   // set the LED on
  delay(1500);              // wait for a 1.5 second
  set_ROBONII_LED(D3_LED,ROBONII_LED_OFF);   // set the LED off
  delay(1500);              // wait for a 1.5 second
  
  
  x=D1_Green;
}

void loop() {
  set_ROBONII_LED(x,ROBONII_LED_RED);  // set the LED on
  delay(500);              // wait for a 0.5 second
  set_ROBONII_LED(x,ROBONII_LED_GREEN);   // set the LED on
  delay(500);              // wait for a 0.5 second
  set_ROBONII_LED(x,ROBONII_LED_YELLOW);   // set the LED on
  delay(500);              // wait for a 0.5 second
  set_ROBONII_LED(x,ROBONII_LED_OFF);   // set the LED off
  delay(500);              // wait for a 0.5 second
  
  x= x+2;  // each LED is controlled by 2 pins. One pin controls red, the other controls yellow.
  if (x>=D5_Red)
  {
    x=D1_Green;
    knight_Rider(400);
  }
}






