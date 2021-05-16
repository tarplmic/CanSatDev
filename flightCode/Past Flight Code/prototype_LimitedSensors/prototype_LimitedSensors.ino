//GND TELEMTRY FORMAT
//teamId,missionTime,packetCount,packetType,mode,sp1Released,sp2Released,altitude,temp,voltage,gpsTime,gpsLat,gpsLong,gpsAlt,gpsSats,flightStage,sp1PacketCount,sp2PacketCount,lastCommand
//GND COMMAND FORMAT
//CMD,2617,CX,PING

//INCLUDE ALL HEADER FILES NEEDED
#include <init.h> //includes necessary libraries and initializes data vars
#include "SAMDTimerInterrupt.h"
#include "SAMD_ISR_Timer.h"
#include <LimitedSensors.h> //initialize and get data from sensors
#include <SerialDefinitions.h> //initialize Serial2 and Serial3
#include <TimeFunctions.h> //function to grab time (in terms of arduino millis)
#include <Servo.h>

//DEFINE SENSORS
LimitedSensors sensors(6);
TimeFunctions timeFunctions;
Servo myServo; 

SAMDTimer ITimer0(TIMER_TC3);
SAMD_ISR_Timer ISR_Timer;

#define HW_TIMER_INTERVAL_MS          50L

#define TIMER_INTERVAL_getSensData             100L
#define TIMER_INTERVAL_writeToXbee             1000L
#define TIMER_INTERVAL_readXbee                400L
#define TIMER_INTERVAL_altCheck                500L

//container sensor vars, alt correction, and othe global vars are defined in init.h
//DEFINE PAYLOAD SENSOR VARS FOR RELAY

//DEFINE RECEIVING DATA VARS
const byte numChars = 32;
char receivedChars[numChars];
boolean newData = false;

//container sensor vars, alt correction, and othe global vars are defined in init.h
//DEFINE PAYLOAD SENSOR VARS FOR RELAY
int sensorDivisor = 0;
float tempSum = 0.0;
//OTHER GLOBAL VARS TO DEFINE
int incomingByte = 0;

float deltaAlt[10];
float previousAlt = 0;
float currentAlt = 0; 
int currentAltPos = 0;


void setup() {
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);

  Serial1.begin(115200); //openlog
  while (!Serial1) { Serial1.println("open log aint starting"); };
  Serial1.println("begin test");
  Serial2.begin(9600); 
  while (!Serial2){ Serial1.print("xbee aint starting"); };
  Serial3.begin(9600);
  while (!Serial3){Serial1.print("xbee not starting");}; 
  Serial2.println("past serial begin");

  sensors.init();
  
  Serial2.println("past sensor init");

  if (ITimer0.attachInterruptInterval(HW_TIMER_INTERVAL_MS * 1000, TimerHandler)){
    int lastMillis = millis();
    Serial.println("Starting  ITimer OK, millis() = " + String(lastMillis));
  }
  else{
    Serial.println("Can't set ITimer correctly. Select another freq. or interval");
  }
  ISR_Timer.setInterval(TIMER_INTERVAL_getSensData, getSensData);
  //ISR_Timer.setInterval(TIMER_INTERVAL_writeToXbee, printToXbee);
  ISR_Timer.setInterval(TIMER_INTERVAL_readXbee, readXbee);
  ISR_Timer.setInterval(TIMER_INTERVAL_altCheck, altitudeCheck);
  
}

void loop() {
  
}

void altitudeCheck(){ 
  //previous and curr alt, 
  Serial2.println(alt);
  float total;
  float averageDeltaAlt;
  for(int i = 0; i < 10; i++){
    total += deltaAlt[i];
  }
  averageDeltaAlt = total / 10;
  Serial2.println("averageDeltaAlt");
  Serial2.println(averageDeltaAlt);
  /*if(averageDeltaAlt < 0 && alt <= 500 && flightStage >= #){
    //release payload 1
   */
  
}

void getSensData(){
  tem = sensors.getTemp();
  tempSum += tem;
  alt = sensors.getAlt();
  sensorDivisor += 1;

  previousAlt = currentAlt;
  currentAlt = alt;
  deltaAlt[currentAltPos] = currentAlt - previousAlt;
  if(currentAltPos = 9){
    currentAltPos = 0;
  }else{
    currentAltPos++;
  }
  
}
void printToXbee(){
  missionTime = timeFunctions.getTime();
  int ts = millis();
 // Serial2.println(String(ts));
  float temperature = tempSum / sensorDivisor;
  tempSum = 0;
  sensorDivisor = 0;
  Serial2.println(String(teamId) + "," + missionTime + "," + String(packetCount) + "," + packetType + "," + mode + "," + sp1Released + "," + sp2Released + "," + String(alt) + "," + String(tem) +
                  "," + String(voltage) + "," + gpsTime + "," + String(gpsLat) + "," + String(gpsLong) + "," + String(gpsAlt) + "," + String(gpsSats) + "," + String(flightStage) + "," + String(sp1PacketCount) + "," +
                  String(sp2PacketCount) + "," + lastCommand);
  packetCount += 1;
}

void readXbee(){
  recvWithStartEndMarkers();
  showNewData();
}

void TimerHandler(void)
{
  ISR_Timer.run();
}

void recvWithStartEndMarkers() {
    static boolean recvInProgress = false;
    static byte ndx = 0;
    char startMarker = '<';
    char endMarker = '>';
    char rc;
 
    while (Serial2.available() > 0 && newData == false) {
        rc = Serial2.read();

        if (recvInProgress == true) {
            if (rc != endMarker) {
                receivedChars[ndx] = rc;
                ndx++;
                if (ndx >= numChars) {
                    ndx = numChars - 1;
                }
            }
            else {
                receivedChars[ndx] = '\0'; // terminate the string
                recvInProgress = false;
                ndx = 0;
                newData = true;
            }
        }

        else if (rc == startMarker) {
            recvInProgress = true;
        }
    }
}

void showNewData() {
    if (newData == true) {
        String stringVersionReceivedChars;
        stringVersionReceivedChars = receivedChars;
        if(stringVersionReceivedChars == "CMD,2617,CX,PING"){
          Serial2.println(",PING_RECIEVED,");
        }else if(stringVersionReceivedChars == "CMD,2617,CX,RELEASE"){
          Serial2.println(",RELEASE CMD RECIEVED,");
          //activate servos to release
         // sensors.releaseServo1();
        }
        newData = false;
    }
}
