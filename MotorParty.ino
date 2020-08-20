// Adafruit Motor shield library
// copyright Adafruit Industries LLC, 2009
// this code is public domain, enjoy!

#include <AFMotor.h>
#include <Servo.h> 

// DC motor on M2
//AF_DCMotor motor1(1);
//AF_DCMotor motor2(2);
AF_DCMotor motor3(3);
AF_DCMotor motor4(4);

// DC hobby servo


void setup() {
  Serial.begin(9600);           // set up Serial library at 9600 bps
  Serial.println("Motor party!");
  
 
   
  // turn on motor #2
  //motor1.setSpeed(200);
  //motor2.setSpeed(200);
  motor3.setSpeed(200);
  motor4.setSpeed(200);
 // motor1.run(RELEASE);
  //motor2.run(RELEASE);
  motor3.run(RELEASE);
  motor4.run(RELEASE);

}

int i;

// Test the DC motor, stepper and servo ALL AT ONCE!
void loop() {
  //motor1.run(FORWARD);
  //motor2.run(FORWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);

//  for (i=0; i<255; i++) {
//  //  motor1.setSpeed(i); 
//    motor2.setSpeed(i);  
//    motor3.setSpeed(i);  
//    motor4.setSpeed(i);  
//    delay(3);
// }

}
