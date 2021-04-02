//arduino files
#include <Wire.h>
#include <Arduino.h>   // required before wiring_private.h
#include "wiring_private.h" // pinPeripheral() function
#include "Adafruit_BMP3XX.h"
#include <Adafruit_BNO055.h>
#include <SparkFun_u-blox_GNSS_Arduino_Library.h>
//our own files (located in Documents/Arduino/libraries/CanSatCommon)
#include <SerialDefinitions.h>

//PIN DEFINITIONS
#define ledPin 6
#define BATTERYSENSORPIN A1  //PB08, pin 7

//CONSTANT VALUE DEFINITIONS
#define SEALEVELPRESSURE_HPA (1013.25)
#define PI 3.1415926535897932384626433832795
