#include <Arduino.h>   // required before wiring_private.h
#include "wiring_private.h" // pinPeripheral() function
#include "PayloadSerialDefinitions.h"

#define THERMISTORPIN 16  // which analog pin to connect   
#define SERIESRESISTOR 10000  // the value of the 'other' resistor
#define TEMPERATURENOMINAL 25 // temp. for nominal resistance (almost always 25 C)
#define BCOEFFICIENT 3950 // The beta coefficient of the thermistor (usually 3000-4000)

//int samples[NUMSAMPLES];
 
void setup(void) {

  Serial2.begin(9600); 
  while (!Serial2)
  {
    Serial1.print("xbee aint starting");
  };
  Serial2.println("about to test thermistor");
  // connect AREF to 3.3V and use that as VCC, less noisy!
  
  analogReference(AR_DEFAULT);
}
 
void loop(void) {
  float analogReading = analogRead(THERMISTORPIN);
  Serial2.println("analog reading:");
  Serial2.println(analogReading);
  float resistance = 1023 / analogReading - 1;
  resistance = SERIESRESISTOR / resistance;
  Serial2.println("resistance:");
  Serial2.println(resistance);

  float thermTemp = ((log(resistance / SERIESRESISTOR))/BCOEFFICIENT) + 1.0 / (TEMPERATURENOMINAL + 273.15);
  thermTemp = (1.0 / thermTemp) - 273.15; 

  Serial2.println("thermTemp:");
  Serial2.println(thermTemp);
  //uint8_t i;
  //float average;
  
  // take N samples in a row, with a slight delay
  /*for (i=0; i< NUMSAMPLES; i++) {
   samples[i] = analogRead(THERMISTORPIN);
   delay(10);
  }
 
  // average all the samples out
  average = 0;
  for (i=0; i< NUMSAMPLES; i++) {
     average += samples[i];
  }
  average /= NUMSAMPLES;
 
  Serial2.print("Average analog reading "); 
  Serial2.println(average);
  // convert the value to resistance
  average = 1023 / average - 1;
  average = SERIESRESISTOR / average;
 
  Serial2.print("Thermistor resistance "); 
  Serial2.println(average);*/
 
  delay(1000);
}
