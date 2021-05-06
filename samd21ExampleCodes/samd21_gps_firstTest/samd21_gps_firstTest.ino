#include <Wire.h>
#include <SparkFun_u-blox_GNSS_Arduino_Library.h>
#include <Arduino.h>   // required before wiring_private.h
#include "wiring_private.h" // pinPeripheral() function
#include <SerialDefinitions.h> //initialize Serial2 and Serial3

#define PIN_SERIAL_RX (24) //microcontrollers RX, Xbee TX
#define PIN_SERIAL_TX (23) //microcontroller's TX, xbee RX
#define PAD_SERIAL_RX (SERCOM_RX_PAD_1)
#define PAD_SERIAL_TX (UART_TX_PAD_0)

const int ledPin =  6;

SFE_UBLOX_GNSS myGPS;
long lastTime = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);

  Serial2.begin(9600); 
  while (!Serial2){Serial1.print("xbee aint starting");};
  Serial2.println("beginning of setup");

  Wire.begin();
  if (myGPS.begin() == false)
  {
    Serial2.println(F("u-blox GNSS module not detected at default I2C address. Please check wiring. Freezing."));
    while (1);
  }
  //myGPS.setNMEAOutputPort(Serial2);
  myGPS.setI2COutput(COM_TYPE_UBX);

}

void loop() {
  // put your main code here, to run repeatedly:

  if (millis() - lastTime > 1000)
  {
    lastTime = millis();
    
    int times = millis();
    Serial2.println("before long: " + String(times));
    float longitude = myGPS.getLongitude(50);
    longitude = longitude / 10000000;
    times = millis();
    Serial2.println("after long: " + String(times));
    //Serial2.println(millis());
    Serial2.println("Long: ");
    Serial2.println(longitude);
    
    times = millis();
    Serial2.println("before lat: " + String(times));
    float latitude = myGPS.getLatitude(50);
    latitude = latitude / 10000000;
    times = millis();
    Serial2.println("after lat: " + String(times));
    Serial2.println("Lat: ");
    Serial2.print(latitude);
  }

}
