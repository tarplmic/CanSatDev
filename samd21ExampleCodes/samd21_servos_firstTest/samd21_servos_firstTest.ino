#include <Arduino.h>   // required before wiring_private.h
#include "wiring_private.h" // pinPeripheral() function
#include <Servo.h>
#include <init.h>

Servo myServo; 
int pos = 0;

void setup() {
  
  // put your setup code here, to run once:
  delay(2000);
  myServo.attach(9);
  //myServo.write(115);

  //delay(1000);

  myServo.write(140);
  //delay(2000);

  //myServo.write(115);

  
  delay(1000);
  /*
  for (pos = 0; pos <= 90; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myServo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  for (pos = 90; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    myServo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }*/
   myServo.detach();
}

void loop() {
}
