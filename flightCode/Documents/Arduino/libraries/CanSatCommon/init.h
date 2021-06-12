//arduino files
#include <Wire.h>
#include <Arduino.h>   // required before wiring_private.h
#include "wiring_private.h" // pinPeripheral() function
#include "Adafruit_BMP3XX.h"
#include <Adafruit_BNO055.h>
#include <FlashStorage.h>
//#include <SparkFun_u-blox_GNSS_Arduino_Library.h>
//our own files (located in Documents/Arduino/libraries/CanSatCommon)

//PIN DEFINITIONS
#define ledPin 6
#define BATTERYSENSORPIN A2 //PB09, pin 8

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
float alt;
float tem = 0;
float voltage = 0; 
String gpsTime = "00:00:00";
float gpsLat = 0;
float gpsLong = 0;
float gpsAlt = 0;
int gpsSats = 0; 
String flightStage = "rising"; //operating state of the software, softwareState
int sp1PacketCount = 0;
int sp2PacketCount = 0;
String lastCommand = "test"; //cmd echo

String altStr = "";
String temStr = "";
String gpsLatStr = "";
String gpsLongStr = "";
String gpsAltStr = "";

float accx = 0.000;
float accy = 0.000;
float accz = 0.000;

int flightStageFromFlash = 0;
int flightModeFromFlash = 0;

//INITIALIZE ALT CORRECTION
int altCorrection = 0;

//OTHER GLOBAL VARS TO DEFINE
float pres;
float analogReading;
float analogVoltage;
//float rotRate[3];
int simEnableRec = 0;
int altDivisor = 0;

//DEFINE RECEIVING DATA VARS
const byte numChars = 40;
char receivedChars[numChars];
boolean newData = false;

const byte numChars2 = 50;
char receivedChars2[numChars2];
boolean newData2 = false;

const byte numChars3 = 50;
char receivedChars3[numChars3];
boolean newData3 = false;

float previousAlts[10];
float deltaAlt[10];
float simDeltaAlt[3];
float mostRecentSimDeltaAlt;
float previousAlt;
float currentAlt; 
int deltaAltSampleIndex = 0;
int FS1reqCounter = 0;
int FS0reqCounter = 0;
int deploySP1reqCounter = 0;
int deploySP2reqCounter = 0;
int landedReqCounter = 0;
int buzzerIsOn = 0;

int sampleIndex = 0;
int simPresSampleIndex = 0;
bool recFirstSimp = false;
float calculatedAlt;
int altMakesSense = 0;

bool recFirstSP1Packet = false;
bool recFirstSP2Packet = false;
// String sp1DataStart = "";
// String sp2DataStart = "";

float bmpAltSamples[10];
float voltageSamples[10];
