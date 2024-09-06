#ifndef MinibloqRobonii_h
#define MinibloqRobonii_h

#include <pitches.h>
//To use in Arduinio. type:  #include <MinibloqRobonii.h>    at the top of your sketch
//#include <avrlibdefs.h>
//#include <avrlibtypes.h>
//#include <Robonii_Motor.h>
//#include <Robonii_Sound.h>

#define motor0 Robonii_Motor_Left
#define motor1 Robonii_Motor_Right

#define serial0 Serial
#define serial1 Serial
#define serial2 Serial

#define setSpeed SetSpeed
#define BuzzerPin Sound_Pin
#define toneWithDelay tone

#define initBoard Robonii_Motors.InitMotor



#endif
