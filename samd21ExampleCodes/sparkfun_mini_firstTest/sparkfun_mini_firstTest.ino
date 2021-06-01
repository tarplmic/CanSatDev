#include "Arduino.h"
const int ledPin =  13;
#include "Adafruit_BMP3XX.h"
#include <SparkFun_u-blox_GNSS_Arduino_Library.h>

Adafruit_BMP3XX bmp;
//SFE_UBLOX_GNSS myGPS;

#define BMP_SCK 13 //26 on micro
#define BMP_MISO 12 //28
#define BMP_MOSI 11 //25
#define BMP_CS 10 //27

void setup() {
  // put your setup code here, to run once:
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);
  
  SerialUSB.begin(9600); 
  while(!SerialUSB);
  SerialUSB.println("beginnning");

  Serial1.begin(9600); //xbee
  while (!Serial1) { SerialUSB.println("xbee aint starting"); };
  SerialUSB.println("after serial init");

  /*if (!bmp.begin_I2C()) {
      while (1){
          SerialUSB.println("bmp is shit");
      };
      SerialUSB.println("bmp didnt work 1");
  }*/
  if (!bmp.begin_SPI(BMP_CS, BMP_SCK, BMP_MISO, BMP_MOSI)) {
    SerialUSB.println("Could not find a valid BMP3 sensor, check wiring!");
    while (1);
  }
  bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_8X);
  bmp.setPressureOversampling(BMP3_OVERSAMPLING_4X);
  bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);

  /*Wire.begin();
  SerialUSB.println("after wire begin");
  if (myGPS.begin() == false)
  {
      //Serial.println(F("u-blox GNSS module not detected at default I2C address. Please check wiring. Freezing."));
      while (1){
        SerialUSB.println("gps no work");
      };
  }*/
  
  SerialUSB.println("end setup");
}

void loop() {

  Serial1.println("test!");
  SerialUSB.println("test!");
  
  // put your main code here, to run repeatedly:
  digitalWrite(ledPin, HIGH);
  delay(1000);
  digitalWrite(ledPin, LOW);
  delay(1000);

  /*float longitude = myGPS.getLongitude(50);
  SerialUSB.println(longitude);*/

  if (!bmp.performReading()) {
    SerialUSB.println("bmp not get reading");
  }
  float tem = bmp.temperature; //celcius
  float pres = bmp.pressure / 100.0F; //Pa
  float alt = 44330*(1 - pow((pres/1013.25), (1/5.255)));

  SerialUSB.println(alt);
  
}
