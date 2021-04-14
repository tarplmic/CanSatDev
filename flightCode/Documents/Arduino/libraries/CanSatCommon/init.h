//arduino files
#include <Wire.h>
#include <Arduino.h>   // required before wiring_private.h
#include "wiring_private.h" // pinPeripheral() function
#include "Adafruit_BMP3XX.h"
#include <Adafruit_BNO055.h>
#include <SparkFun_u-blox_GNSS_Arduino_Library.h>
//our own files (located in Documents/Arduino/libraries/CanSatCommon)

//PIN DEFINITIONS
#define ledPin 6
#define BATTERYSENSORPIN A1  //PB08, pin 7

//CONSTANT VALUE DEFINITIONS
#define SEALEVELPRESSURE_HPA (1013.25)
#define PI 3.1415926535897932384626433832795

//DEFINE CONTAINER SENSOR VARS
const int teamId = 2617;
String missionTime = "00:00:00"; //hh:mm:ss
int packetCount = 0; //total transmitted packets from container to ground
const String packetType = "C";
String mode = "F"; //F or S
String sp1Released = "N"; //N or R
String sp2Released = "N"; //N or R
float alt = 0;
float tem = 0;
float voltage = 0; 
String gpsTime = "00:00:00";
float gpsLat = 0;
float gpsLong = 0;
float gpsAlt = 0;
int gpsSats = 0; 
int flightStage = 0; //operating state of the software, softwareState
int sp1PacketCount = 0;
int sp2PacketCount = 0;
String lastCommand = "test"; //cmd echo

//INITIALIZE ALT CORRECTION
int altCorrection = 0;

//OTHER GLOBAL VARS TO DEFINE
float pres;
float analogReading;
float analogVoltage;
