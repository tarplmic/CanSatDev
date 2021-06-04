#ifndef Sensors_h
#define Sensors_h

#include "Arduino.h"
#include "Adafruit_BMP3XX.h"
#include <Adafruit_BNO055.h>

class LimitedSensors
{
  Adafruit_BMP3XX bmp;
  Adafruit_BNO055 bno = Adafruit_BNO055(55); //on same i2c pins as bmp
  public:
    LimitedSensors(int pin);
    void init();
    float getAlt();
    float getTemp();
    float getRotRateX();
    float getRotRateY();
    float getRotRateZ();
    float SEALEVELPRESSURE;
  private:
    int _pin;
};

#endif