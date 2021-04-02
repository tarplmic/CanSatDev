#include <Wire.h> //need for i2c to GNSS
#include <SparkFun_u-blox_GNSS_Arduino_Library.h>
#include <Arduino.h>   // required before wiring_private.h
#include "wiring_private.h" // pinPeripheral() function

#define PIN_Serial2_RX (38) //microcontrollers RX, Xbee TX
#define PIN_Serial2_TX (22) //microcontroller's TX, xbee RX
#define PAD_Serial2_RX (SERCOM_RX_PAD_1)
#define PAD_Serial2_TX (UART_TX_PAD_0)

Uart Serial2 (&sercom4, PIN_Serial2_RX, PIN_Serial2_TX, PAD_Serial2_RX, PAD_Serial2_TX );
void SERCOM4_Handler()
{
  Serial2.IrqHandler();
}

const int ledPin =  6;

SFE_UBLOX_GNSS myGPS;


void setup() {
  // put your setup code here, to run once:
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);
  
  pinPeripheral(PIN_Serial2_RX, PIO_SERCOM);
  pinPeripheral(PIN_Serial2_TX, PIO_SERCOM);

  Serial2.begin(9600); 
  while (!Serial2)
  {
    Serial2.print("xbee aint starting");
  };
  
  Serial2.println("beginning of setup");

  Wire.begin();
  if (myGPS.begin() == false)
  {
    Serial2.println(F("u-blox GNSS module not detected at default I2C address. Please check wiring. Freezing."));
    while (1);
  }
  //myGPS.setNMEAOutputPort(Serial2);

}

void loop()
{
  //Query module only every second. Doing it more often will just cause I2C traffic.
  //The module only responds when a new position is available
  long latitude = myGPS.getLatitude();
  Serial2.print(F("Lat: "));
  Serial2.print(latitude);

  long longitude = myGPS.getLongitude();
  Serial2.print(F(" Long: "));
  Serial2.print(longitude);
  Serial2.print(F(" (degrees * 10^-7)"));

  /*long altitude = myGPS.getAltitude();
  Serial2.print(F(" Alt: "));
  Serial2.print(altitude);
  Serial2.print(F(" (mm)"));*/

  /*byte SIV = myGPS.getSIV();
  Serial2.print(F(" SIV: "));
  Serial2.print(SIV);*/
  if (myGPS.getDateValid() == true)
  {
    Serial2.println();
    Serial2.print(myGPS.getYear());
    Serial2.print("-");
    Serial2.print(myGPS.getMonth());
    Serial2.print("-");
    Serial2.print(myGPS.getDay());
  }else{
    Serial2.println("date is not valid");
  }

  if (myGPS.getTimeValid() == true)
  {
    Serial2.print(" ");
    Serial2.print(myGPS.getHour());
    Serial2.print(":");
    Serial2.print(myGPS.getMinute());
    Serial2.print(":");
    Serial2.print(myGPS.getSecond());
  }else{
    Serial2.println("time is not valid");
  }

  Serial2.println();

  delay(1000);
  
}

/*Lat: 347248889 Long: -866387680 (degrees * 10^-7)
2021-3-28 23:18:43  Time is valid  Date is valid*/
