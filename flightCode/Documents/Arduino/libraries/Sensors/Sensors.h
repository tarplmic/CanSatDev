#ifndef Sensors_h
#define Sensors_h

#include "Arduino.h"
#include "Adafruit_BMP3XX.h"
#include <Adafruit_BNO055.h>
#include <SparkFun_u-blox_GNSS_Arduino_Library.h>

class Sensors
{
  SFE_UBLOX_GNSS myGPS;
  Adafruit_BMP3XX bmp;
  Adafruit_BNO055 bno = Adafruit_BNO055(55); //on same i2c pins as bmp
  public:
    Sensors(int pin);
    void init();
    float getAlt();
    float getRotRate();
    float getLat();
    float getLong();
    float getBattVoltage();
    float getTemp();
    float SEALEVELPRESSURE;
    unsigned char BATTERYSENSORPIN;
  private:
    int _pin;
};

#endif