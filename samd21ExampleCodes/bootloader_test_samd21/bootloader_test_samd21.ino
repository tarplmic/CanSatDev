#include <SPI.h>

#define BMP_SCK 13 //26 on micro
#define BMP_MISO 12 //28
#define BMP_MOSI 11 //25
#define BMP_CS 10 //27
#define SEALEVELPRESSURE_HPA (1013.25)

#include <Adafruit_Sensor.h>
#include "Adafruit_BMP3XX.h"

Adafruit_BMP3XX bmp; // I2C
//Adafruit_BMP3XX bmp(BMP_CS, BMP_MOSI, BMP_MISO,  BMP_SCK);

const int ledPin =  6;
const int delayNum = 200;

// Barom: GNG - GND, VIN -3.3V, SCK -A5 (I2C SCL), SDO - 12 (DO of sd), SDI - A4 (I2C SDA)
//open log: BLK - gnd, GND - gnd, VCC - power, TX - 16 (pin on micro), RX - 15 (pin on micro), GRN - reset
void setup() {
  Serial1.begin(115200); 
  while (!Serial1);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  Serial1.println("first test");

  if (!bmp.begin()) {
    Serial1.println("Could not find a valid BMP3 sensor, check wiring!");
    while (1);
  }
  bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_8X);
  bmp.setPressureOversampling(BMP3_OVERSAMPLING_4X);
  bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);
  
  Serial1.println("bmp worked");
  
}

void loop() {
  //digitalWrite(ledPin, HIGH);
  //delay(delayNum);
  //digitalWrite(ledPin, LOW);
  //delay(delayNum);
  //Serial1.println("test");

  if (! bmp.performReading()) {
    Serial1.println("Failed to perform reading :(");
    return;
  }
  //Serial.print("Temperature = ");
  float tem = bmp.temperature;

  //Serial.print("Pressure = ");
  float pres = bmp.pressure / 100.0;

  //Serial.print("Approx. Altitude = ");
  float alt = bmp.readAltitude(SEALEVELPRESSURE_HPA);

  writeSD(tem, pres, alt);
  
  delay(100);
  
}

void writeSD(float temperature, float pressure, float altitude){
  //Serial.print(temperature);
  time_t timestamp = millis(); 

    //Serial.print("Writing to test.txt...");
    Serial1.print(timestamp);
    Serial1.print(',');
    Serial1.print(temperature);
    Serial1.print(',');
    Serial1.print(pressure);
    Serial1.print(',');
    Serial1.println(altitude);
  // close the file:
    //Serial.println("done.");
}
