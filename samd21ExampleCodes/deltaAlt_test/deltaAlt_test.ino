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

//DEFINE SENSORS
Sensors sensors(6);
TimeFunctions timeFunctions;

SAMDTimer ITimer0(TIMER_TC3);
SAMD_ISR_Timer ISR_Timer;

#define HW_TIMER_INTERVAL_MS          50L

#define TIMER_INTERVAL_getSensData             1000L
#define TIMER_INTERVAL_writeToXbee             1000L
#define TIMER_INTERVAL_readXbee                400L
#define TIMER_INTERVAL_altCheck                1000L

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


float fakeData[138] = {100833
,100833
,100833
,100833
,100833
,100833
,100833
,100833
,100833
,100833
,100833
,100833
,100833
,98522
,97353
,96081
,95049
,94231
,93621
,93137
,92812
,92578
,92645
,92868
,92756
,92924
,92913
,93059
,93115
,93216
,93250
,93384
,93441
,97539
,93655
,93700
,93790
,93880
,93948
,94072
,94378
,94231
,94299
,94378
,94503
,94617
,94651
,94810
,96334
,94947
,95015
,95118
,95278
,95323
,98264
,95495
,95587
,95678
,95782
,95862
,96127
,96023
,96069
,96150
,96207
,96288
,96357
,96438
,96519
,96623
,96669
,96738
,96831
,96900
,96981
,97121
,97179
,97190
,97295
,97341
,97458
,97598
,97621
,97563
,97609
,97691
,97761
,97796
,97843
,97889
,97936
,98018
,98393
,104180
,98287
,98240
,98393
,98393
,98440
,98522
,98616
,98639
,98698
,98745
,98816
,98875
,98957
,99075
,99512
,99264
,99299
,100523
,99406
,99489
,99571
,99702
,99749
,99809
,99880
,99963
,100034
,100129
,100368
,100463
,101927
,100487
,100582
,100606
,100726
,100762
,100821
,100893
,100893
,100893
,100893
,100893
,100893
,100893
};



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
  //ISR_Timer.setInterval(TIMER_INTERVAL_readXbee, readXbee);
  //ISR_Timer.setInterval(TIMER_INTERVAL_readXbee, readPayloadXBee);
  ISR_Timer.setInterval(TIMER_INTERVAL_altCheck, altitudeCheck);
  
}

void loop() {
  
}

int x = 0;

void altitudeCheck(){ 
  //previous and curr alt, 
  //Serial2.println(alt);
  float total;
  float averageDeltaAlt;
  for(int i = 0; i < 10; i++){
    total += deltaAlt[i];
    //Serial2.println(deltaAlt[i]);
    //delay(20);
  }
  averageDeltaAlt = total / 10;
  Serial2.println("averageDeltaAlt");
  Serial2.println(averageDeltaAlt);
  Serial2.println("alt");
  Serial2.println(alt);
  /*if(averageDeltaAlt < 0 && alt <= 500 && flightStage >= #){
    //release payload 1
   */
  
}

void getSensData(){
  //alt = sensors.getAlt();
  /*if (alt < 10) {
    alt++;
  }else {
    alt--;
  }*/
  
  //pres = sensors.getPressure();
  pres = fakeData[x];
  alt = 44330*(1 - pow((pres/101325), (1/5.255)));
  x++;
  
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

void TimerHandler(void)
{
  ISR_Timer.run();
}
