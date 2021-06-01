#ifndef Sensors_h
#define Sensors_h

#include "Arduino.h"
#include "Adafruit_BMP3XX.h"
#include <Adafruit_BNO055.h>
#include <SparkFun_u-blox_GNSS_Arduino_Library.h>
#include <Servo.h>

class Sensors
{
  SFE_UBLOX_GNSS myGPS;
  Adafruit_BMP3XX bmp;
  Adafruit_BNO055 bno = Adafruit_BNO055(55); //on same i2c pins as bmp
  Servo myServo1;
  Servo myServo2;
  public:
    Sensors(int pin);
    void init();
    float getPressure();
    // float* getRotRate();
    // float getRotRateX();
    // float getRotRateY();
    // float getRotRateZ();
    float getAccX();
    float getAccY();
    float getAccZ();
    float getLat();
    float getLong();
    float getGPSAlt();
    String getGPSTime();
    int getNumSats();
    float getBattVoltage();
    float getTemp();
    void releaseServo1();
    void releaseServo2();
    void stopCamera();
    float SEALEVELPRESSURE;
    unsigned char BATTERYSENSORPIN;
    int CAMERAPIN;
  private:
    int _pin;
};

#endif