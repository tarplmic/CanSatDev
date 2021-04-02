#include <Arduino.h>   // required before wiring_private.h
#include "wiring_private.h" // pinPeripheral() function
#include <Servo.h>
#include <init.h>


Servo myServo; 
int pos = 0;



void setup() {
  pinPeripheral(PIN_SERIAL_RX3, PIO_SERCOM_ALT);
  pinPeripheral(PIN_SERIAL_TX3, PIO_SERCOM_ALT);
  Serial3.begin(9600); 
  while (!Serial3)
  {
    Serial1.print("xbee aint starting");
  };
  Serial3.println("TEST");
  
  // put your setup code here, to run once:
  /*myServo.attach(15);
  myServo.write(0);
  //myServo2.write(90);
  for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myServo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    myServo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }*/
}

void loop() {
  Serial3.println("test");
}
