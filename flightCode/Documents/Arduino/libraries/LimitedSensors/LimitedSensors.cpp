#include "Arduino.h"
#include "LimitedSensors.h"
#include "Adafruit_BMP3XX.h"
//#include "init.h"
//#include <SerialDefinitions.h>

LimitedSensors::LimitedSensors(int ledPin)
{
    //pinMode(ledPin, OUTPUT);
    _pin = ledPin;
    //digitalWrite(_pin, HIGH);

    SEALEVELPRESSURE = 1013.25;
}

void LimitedSensors::init()
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

}

float LimitedSensors::getAlt()
{
    if (!bmp.performReading()) {
      return 0;
    }
    float tem = bmp.temperature; //celcius
    float pres = bmp.pressure / 100.0F; //Pa
    float alt = 44330*(1 - pow((pres/SEALEVELPRESSURE), (1/5.255)));
    return alt;
}

float LimitedSensors::getTemp()
{
    if (!bmp.performReading()) {
      return 0;
    }
    float tem = bmp.temperature; //celcius
    return tem;
}

float LimitedSensors::getRotRateX(){
    imu::Vector<3> rotSpeed = bno.getVector(Adafruit_BNO055::VECTOR_GYROSCOPE);
    float gyrox = rotSpeed.x();
    return gyrox;
}

float LimitedSensors::getRotRateY(){
    imu::Vector<3> rotSpeed = bno.getVector(Adafruit_BNO055::VECTOR_GYROSCOPE);
    float gyroy = rotSpeed.y();
    return gyroy;
}

float LimitedSensors::getRotRateZ(){
    imu::Vector<3> rotSpeed = bno.getVector(Adafruit_BNO055::VECTOR_GYROSCOPE);
    float gyroz = rotSpeed.z();
    return gyroz;
}



