#include <Arduino.h>   // required before wiring_private.h
#include "wiring_private.h" // pinPeripheral() function
#include <init.h> 
#include <SerialDefinitions.h>

const int cameraPin = 25; //pa02, pin 3

void setup() {
  // put your setup code here, to run once:
  //pinMode(cameraPin, OUTPUT);
  Serial2.begin(9600); 
  while (!Serial2){ Serial1.print("xbee aint starting"); };
  Serial2.println("beginning");
  
 // delay(100);
  digitalWrite(cameraPin, HIGH); 
  delay(1000);

}

void loop() {
  // put your main code here, to run repeatedly:
  //delay(1000);
  digitalWrite(cameraPin, LOW); 
  delay(700);
  digitalWrite(cameraPin, HIGH);
  delay(5000);              
  digitalWrite(cameraPin, LOW);
  delay(700);
  digitalWrite(cameraPin, HIGH);
  delay(1000);


  Serial2.println("test"); 

  delay(1000);
}
