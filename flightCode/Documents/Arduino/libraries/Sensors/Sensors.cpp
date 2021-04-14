#include "Arduino.h"
#include "Sensors.h"
#include "Adafruit_BMP3XX.h"
//#include "init.h"
//#include <SerialDefinitions.h>

Sensors::Sensors(int ledPin)
{
    pinMode(ledPin, OUTPUT);
    _pin = ledPin;
    digitalWrite(_pin, HIGH);

    SEALEVELPRESSURE = 1013.25;
    BATTERYSENSORPIN = A1;  //PB08, pin 7
}

void Sensors::init()
{
    if (!bmp.begin()) {
        while (1){
            digitalWrite(_pin, LOW);
        };
    }
    bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_8X);
    bmp.setPressureOversampling(BMP3_OVERSAMPLING_4X);
    bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);
    
    if(!bno.begin())
    {
        while(1){
            digitalWrite(_pin, LOW);
        };
    }
    delay(10);
    bno.setExtCrystalUse(true);

    Wire.begin();
    if (myGPS.begin() == false)
    {
        //Serial.println(F("u-blox GNSS module not detected at default I2C address. Please check wiring. Freezing."));
        while (1){
            digitalWrite(_pin, LOW);
        };
    }

    analogReadResolution(12);
    analogReference(AR_DEFAULT);
}

float Sensors::getAlt()
{
    if (!bmp.performReading()) {
      return 0;
    }
    float tem = bmp.temperature; //celcius
    float pres = bmp.pressure / 100.0F; //Pa
    float alt = 44330*(1 - pow((pres/SEALEVELPRESSURE), (1/5.255)));
    return alt;
}

float Sensors::getTemp()
{
    if (!bmp.performReading()) {
      return 0;
    }
    float tem = bmp.temperature; //celcius
    return tem;
}

float Sensors::getRotRate(){
    imu::Vector<3> rotSpeed = bno.getVector(Adafruit_BNO055::VECTOR_GYROSCOPE);
    float gyrox = rotSpeed.x();
    float gyroy = rotSpeed.y();
    float gyroz = rotSpeed.z();
    return gyroz;
}

float Sensors::getLat(){
    float gpsLat = myGPS.getLatitude();
    gpsLat = gpsLat / 10000000;
    return gpsLat;
}

float Sensors::getLong(){
    float gpsLong = myGPS.getLongitude();
    gpsLong = gpsLong / 10000000;
    return gpsLong;
}

float Sensors::getBattVoltage(){
    float analogReading = analogRead(BATTERYSENSORPIN);
    float analogVoltage = (analogReading * 3.3) / 4095;
    float voltage = analogVoltage * ((33000 + 10000) / 10000);
    return voltage;
}