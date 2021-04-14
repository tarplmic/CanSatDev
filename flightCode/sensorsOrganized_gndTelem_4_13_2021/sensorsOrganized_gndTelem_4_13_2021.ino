//GND TELEMTRY FORMAT
//teamId,missionTime,packetCount,packetType,mode,sp1Released,sp2Released,altitude,temp,voltage,gpsTime,gpsLat,gpsLong,gpsAlt,gpsSats,flightStage,sp1PacketCount,sp2PacketCount,lastCommand
//GND COMMAND FORMAT
//CMD,2617,CX,PING

//INCLUDE ALL HEADER FILES NEEDED
#include <init.h> 
#include <Sensors.h>

//DEFINE SENSORS
Sensors sensor(6);

//DEFINE DELAY VARS
const int sensorDelayNum = 100;
int sensorDelayStart;
const int printDelayNum = 1000;
int printDelayStart;
const int readDelayNum = 10;
int readDelayStart;

//INITIALIZE ALT CORRECTION
int altCorrection = 0;

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

//DEFINE PAYLOAD SENSOR VARS FOR RELAY

//OTHER GLOBAL VARS TO DEFINE
float pres;
float analogReading;
float analogVoltage;
int incomingByte = 0;
#define Po 10.1325

//DEFINE RECEIVING DATA VARS
const byte numChars = 32;
char receivedChars[numChars];
boolean newData = false;

void setup() {
  //TURN ON LED
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);

  //INITIALIZE SERIALS
  Serial1.begin(9600); //openlog
  while (!Serial1)
  {
   Serial1.println("open log aint starting");
  };
  //pinPeripheral(PIN_SERIAL_RX2, PIO_SERCOM);
  //pinPeripheral(PIN_SERIAL_TX2, PIO_SERCOM);
  Serial2.begin(9600); 
  while (!Serial2)
  {
    Serial2.print("xbee aint starting");
  };

  sensor.init();
  
  //START TIMER VARS
  sensorDelayStart = millis();
  printDelayStart = millis();
  
}

void loop() {
  int currentTs = millis();
  
  //INTERVAL TO GET SENSOR DATA
  if((currentTs - sensorDelayStart) > sensorDelayNum){
    tem = sensor.getTemp();
    alt = sensor.getAlt();
    gpsLat = sensor.getLat();
    gpsLong = sensor.getLong();
    voltage = sensor.getBattVoltage();
    
    sensorDelayStart = millis();

  }
  
  //INTERVAL TO PRINT TO SERIAL DEVICES
  if((currentTs - printDelayStart) > printDelayNum){
    writeOpenLogXbees(teamId, missionTime, packetCount, packetType, mode, sp1Released, sp2Released, alt, tem, voltage, gpsTime, gpsLat, gpsLong, gpsAlt, gpsSats, flightStage, sp1PacketCount, sp2PacketCount, lastCommand);
    printDelayStart = millis();
  }

  //INTERVAL TO READ FROM SERIAL DEVICES
  if((currentTs - readDelayStart) > readDelayNum){
     recvWithStartEndMarkers();
     showNewData();
     readDelayStart = millis();

   }
}

void writeOpenLogXbees(int teamId, String missionTime, int packetCount, String packetType, String mode, String sp1Released, String sp2Released, float altitude, float temperature, 
                        float voltage, String gpsTime, float lat, float lng, float gpsAlt, int gpsSats, int flightStage, int sp1PacketCount, int sp2PacketCount, String lastCommand){
  //time_t timestamp = millis(); 
  //in real case use gps mission time
  unsigned long runMillis= millis();
  unsigned long allSeconds=millis()/1000;
  int runHours= allSeconds/3600;
  int secsRemaining=allSeconds%3600;
  int runMinutes=secsRemaining/60;
  int runSeconds=secsRemaining%60;
  
  missionTime = String(runHours) + ":" + String(runMinutes) + ":" + String(runSeconds);

  printToSerials(String(teamId) + "," + missionTime + "," + String(packetCount) + "," + packetType + "," + mode + "," + sp1Released + "," + sp2Released + "," + String(altitude) + "," + String(temperature) +
                  "," + String(voltage) + "," + gpsTime + "," + String(lat) + "," + String(lng) + "," + String(gpsAlt) + "," + String(gpsSats) + "," + String(flightStage) + "," + String(sp1PacketCount) + "," +
                  String(sp2PacketCount) + "," + lastCommand);

  Serial1.println();
}

void printToSerials(String msg){
  Serial2.println(msg);
  Serial1.println(msg);
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
          Serial2.println("PING_RECIEVED");
        }
        newData = false;
    }
}
