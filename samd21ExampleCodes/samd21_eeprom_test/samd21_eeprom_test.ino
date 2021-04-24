#include <init.h> //includes necessary libraries and initializes data vars
#include <Sensors.h> //initialize and get data from sensors
#include <SerialDefinitions.h> //initialize Serial2 and Serial3
#include <TimeFunctions.h> //function to grab time (in terms of arduino millis)
#include <FlashStorage.h>

Sensors sensors(6);
FlashStorage(flightStageFlash, int);

void setup() {
  // put your setup code here, to run once:
  Serial2.begin(9600); 
  while (!Serial2){ Serial1.print("xbee aint starting"); };
  Serial2.println("test");
  flightStageFlash.write(0);
  int number;

  // Read the content of "my_flash_store" and assign it to "number"
  number = flightStageFlash.read();
  Serial2.println(number);
  
}

void loop() {
  // put your main code here, to run repeatedly:

}
