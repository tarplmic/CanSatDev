#include <Wire.h>
#include <SparkFun_u-blox_GNSS_Arduino_Library.h>
#include <Arduino.h>   // required before wiring_private.h
#include "wiring_private.h" // pinPeripheral() function

#define PIN_SERIAL_RX (38) //microcontrollers RX, Xbee TX
#define PIN_SERIAL_TX (22) //microcontroller's TX, xbee RX
#define PAD_SERIAL_RX (SERCOM_RX_PAD_1)
#define PAD_SERIAL_TX (UART_TX_PAD_0)

const int ledPin =  6;

SFE_UBLOX_GNSS myGPS;

Uart Serial2 (&sercom4, PIN_SERIAL_RX, PIN_SERIAL_TX, PAD_SERIAL_RX, PAD_SERIAL_TX );
void SERCOM4_Handler()
{
  Serial2.IrqHandler();
}

void setup() {
  // put your setup code here, to run once:
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);
  
  pinPeripheral(PIN_SERIAL_RX, PIO_SERCOM);
  pinPeripheral(PIN_SERIAL_TX, PIO_SERCOM);

  Serial2.begin(9600); 
  while (!Serial2)
  {
    Serial1.print("xbee aint starting");
  };
  
  Serial2.println("beginning of setup");

  Wire.begin();
  if (myGPS.begin() == false)
  {
    Serial.println(F("u-blox GNSS module not detected at default I2C address. Please check wiring. Freezing."));
    while (1);
  }
  //myGPS.setNMEAOutputPort(Serial2);

}

void loop() {
  // put your main code here, to run repeatedly:
  float latitude = myGPS.getLatitude();
  latitude = latitude / 10000000;

  float longitude = myGPS.getLongitude();
  longitude = longitude / 10000000;

  Serial2.println("Lat:    Long: ");
  Serial2.print(latitude);
  Serial2.print(", ");
  Serial2.println(longitude);
  
  delay(100);

}
