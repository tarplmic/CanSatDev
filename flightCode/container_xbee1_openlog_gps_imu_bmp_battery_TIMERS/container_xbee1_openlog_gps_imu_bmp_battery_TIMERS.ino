//GND TELEMTRY FORMAT
//teamId,missionTime,packetCount,packetType,mode,sp1Released,sp2Released,altitude,temp,voltage,gpsTime,gpsLat,gpsLong,gpsAlt,gpsSats,flightStage,sp1PacketCount,sp2PacketCount,lastCommand
//GND COMMAND FORMAT
//CMD,2617,CX,PING

//INCLUDE ALL HEADER FILES NEEDED
#include <init.h> //includes necessary libraries and initializes data vars
#include "SAMDTimerInterrupt.h"
#include "SAMD_ISR_Timer.h"
#include <Sensors.h> //initialize and get data from sensors
#include <SerialDefinitions.h> //initialize Serial2 and Serial3
#include <TimeFunctions.h> //function to grab time (in terms of arduino millis)

//#include <SparkFun_u-blox_GNSS_Arduino_Library.h>

//DEFINE SENSORS
//SFE_UBLOX_GNSS myGPS;
Sensors sensors(6);
TimeFunctions timeFunctions;

SAMDTimer ITimer0(TIMER_TC3);
SAMD_ISR_Timer ISR_Timer;

#define HW_TIMER_INTERVAL_MS          50L

#define TIMER_INTERVAL_getSensData             100L
#define TIMER_INTERVAL_writeToXbee             1000L
#define TIMER_INTERVAL_readXbee                400L
#define TIMER_INTERVAL_altCheck                500L

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

//DEFINE RECEIVING DATA VARS
const byte numChars = 32;
char receivedChars[numChars];
boolean newData = false;

const int gpsDelayNum = 2000;
int gpsDelayStart;

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
  //ISR_Timer.setInterval(TIMER_INTERVAL_getSensData, getSensData);
  //ISR_Timer.setInterval(TIMER_INTERVAL_writeToXbee, printToXbee);
  //ISR_Timer.setInterval(TIMER_INTERVAL_readXbee, readXbee);
  //ISR_Timer.setInterval(TIMER_INTERVAL_readXbee, readPayloadXBee);
  //ISR_Timer.setInterval(TIMER_INTERVAL_altCheck, altitudeCheck);

  gpsDelayStart = millis();
  
}

void loop() {
  int currentTs = millis();
  if((currentTs - gpsDelayStart) > gpsDelayNum){
    Serial2.println((currentTs - gpsDelayStart));
    gpsLat = sensors.getLat();
    gpsLong = sensors.getLong();
    gpsAlt = sensors.getGPSAlt();
    gpsDelayStart = millis();
    
  }
  
}

void altitudeCheck(){ 
  float total;
  float averageDeltaAlt;
  for(int i = 0; i < 10; i++){
    total += deltaAlt[i];
  }
  averageDeltaAlt = total / 10;
  /*Serial2.println("averageDeltaAlt");
  Serial2.println(averageDeltaAlt);
  Serial2.println("alt");
  Serial2.println(alt);*/
  
}

void getSensData(){
  tem = sensors.getTemp();
  tempSum += tem;
  pres = sensors.getPressure();
  alt = 44330*(1 - pow((pres/SEALEVELPRESSURE_HPA), (1/5.255)));
  
  /*gpsLat = sensors.getLat();
  gpsLong = sensors.getLong();*/
  
  voltage = sensors.getBattVoltage();
  sensorDivisor += 1;

  previousAlt = currentAlt;
  currentAlt = alt;
  deltaAlt[currentAltPos] = currentAlt - previousAlt;
  if(currentAltPos == 9){
    currentAltPos = 0;
  }else{
    currentAltPos++;
  }
  
}
void printToXbee(){
  missionTime = timeFunctions.getTime();
  //int ts = millis();
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
          sensors.releaseServo1();
        }
        newData = false;
    }
}

void writePayloadXBee(String packet){

  int packetLength = packet.length();

  //calculate chexum, get packet in bytes
  long chexum = 0x10 + 0x01 + 0x13 + 0xa2 + 0x41 + 0xba + 0x07 + 0x85 + 0xff + 0xfe;
  byte buffer[packet.length() + 1];
  packet.getBytes(buffer, packet.length() + 1);

  // strings have extra 0 at end
  for (int i=0; i < packet.length(); i++){
      chexum += buffer[i];
  }
  
  Serial3.write(0x7e); //Start delimiter
  Serial3.write((byte)0x0); //Length
  Serial3.write(packetLength + 14); //Length
  Serial3.write(0x10); //Frame type
  Serial3.write(0x01); //Frame ID
  //64-bit address
  Serial3.write((byte)0x0);
  Serial3.write(0x13);
  Serial3.write(0xa2);
  Serial3.write((byte)0x0);
  Serial3.write(0x41);
  Serial3.write(0xba);
  Serial3.write(0x07);
  Serial3.write(0x85);
  //16-bit address - reserved 
  Serial3.write(0xff);
  Serial3.write(0xfe);
  Serial3.write((byte)0x0);//broadcast radius
  Serial3.write((byte)0x0); //transmit options
  //Data & Checksum
   for (int i=0; i < packet.length(); i++){
    Serial3.write(buffer[i]);
   }
  //Checksum
  Serial3.write(0xff - (chexum & 0xff));
}

void readPayloadXBee(){
  String incomingData = "";
  
  if (Serial3.available() >= 16){
    //Serial2.println("serial 3 avail");
    /*while(Serial3.available() > 0){
      Serial2.println(String(Serial3.read()));
    }*/
    
    if(Serial3.read() == 0x7E){
      for (int i = 1; i<15; i++){
        byte discardByte = Serial3.read();
      }
      while(Serial3.available() > 0){
        char dataIn = Serial3.read();
        /*if (dataIn == 0x7E){
          break;
        }*/
        //else {
          incomingData += dataIn; 
        //}
      }
      incomingData.remove(incomingData.length()-1);
    }
  } 
  if (incomingData != ""){
    Serial2.println(incomingData);
  }
}
