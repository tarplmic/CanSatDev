#include "PayloadInit.h"

#define ledPin 6

// Communication
const byte numChars = 40;
char receivedChars[numChars];
boolean newData = false;

String thermTempStr;

int openLogPacketCount = 0; 

// Science Payload 1
String packetType = "SP1";

// Sensors
Adafruit_BMP3XX bmp; 
Adafruit_BNO055 bno;

// Flight stage commands
String command = "";
bool sendTelem = true;
bool collectData = true;

// Delays
const long sensorDelayNum = 10; 
long sensorDelayStart;
const long printDelayNum = 100;
long printDelayStart;
const long sendDelayNum = 1000;
long sendDelayStart; 
const long readDelayNum = 10;
long readDelayStart;
unsigned long currentTs;
const int altCheckDelayNum = 500;
int altCheckDelayStart;

// Set index
int sampleIndex = 0;

//set delta alt variables
float previousAlts[10];
float deltaAlt[10];
float previousAlt;
float currentAlt; 
int deltaAltSampleIndex = 0;
float openLogAverageDeltaAlt;

void setup() {

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);

  Serial1.begin(9600);
  while(!Serial1);
  
  Serial1.println("OpenLog started");
  
  Serial2.begin(9600); 
  while (!Serial2){
    Serial1.println("XBee not starting");
  };

  Serial2.println("TESTTT");
  
  analogReference(AR_DEFAULT); //for thermistor

  //start bmp
  /*if (!bmp.begin_I2C()) {
    Serial1.println("Could not find a valid BMP3 sensor, check wiring!");
    Serial2.println("Could not find a valid BMP3 sensor, check wiring!");
    while (1);
  }*/

  if (!bmp.begin_SPI(BMP_CS, BMP_SCK, BMP_MISO, BMP_MOSI)) {
    Serial1.println("Could not find a valid BMP3 sensor, check wiring!");
    Serial2.println("Could not find a valid BMP3 sensor, check wiring!");
    while (1);
  }
  
  bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_8X);
  bmp.setPressureOversampling(BMP3_OVERSAMPLING_4X);
  bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);

  //start bno
  if(!bno.begin())
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial1.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    Serial2.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }

  bno.setExtCrystalUse(true);

  //initialize timers
  altCheckDelayStart = millis();
  sensorDelayStart = millis();
  printDelayStart = millis();
  sendDelayStart = millis();
  readDelayStart = millis();

  bmp.performReading(); //dummy read
}

void loop() {
  
    currentTs = millis();

    /*READ XBEE*/
    if((currentTs - readDelayStart) > readDelayNum){
     /*command = readXBee();
     if (command == "CMD,2617,SP1X,ON"){
        sendTelem = true;
        Serial2.println("RECIEVEED SP1X ON");
     }else if (command == "CMD,2617,SP1X,OFF"){
        sendTelem = false;
        Serial2.println(sendTelem);
        Serial2.println("RECIEVEED SP1X OFF");
        readDelayStart = millis();
     }
     */
     recvWithStartEndMarkers();
     showNewData();
     readDelayStart = millis();
   }
    
    /*COLLECT DATA*/
    

    /*SAMPLE SENSORS*/
    if((currentTs - sensorDelayStart) >= sensorDelayNum){

      /*CASE: RESET LIST*/
      if (sampleIndex == NUMSAMPLES){
        sampleIndex = 0; 
      }
      
      /*BMP DATA*/
      bmp.performReading();
      pres = bmp.pressure / 100.0; //Pa
      bmpTemperature = bmp.temperature;
      bmpSamples[sampleIndex] = pres;
      
      /*BNO DATA*/
      sensors_event_t angVelocityData; 
      bno.getEvent(&angVelocityData, Adafruit_BNO055::VECTOR_GYROSCOPE);
      
      bnoXSamples[sampleIndex] = angVelocityData.gyro.x;
      bnoYSamples[sampleIndex] = angVelocityData.gyro.y;
      bnoSamples[sampleIndex] = angVelocityData.gyro.z;         
      
      /*THERMISTOR DATA*/       
      thermSamples[sampleIndex] = analogRead(THERMISTORPIN);

      /*RESET INDEX*/
      sampleIndex++;
      
      /*RESET TIMER*/
      sensorDelayStart = millis();
    }

    /*CREATE PACKETS*/    
    if (currentTs - printDelayStart >= printDelayNum){
        
        /*AVERAGE SAMPLES*/
        float presSum = 0;
        float rotation_xSum = 0;
        float rotation_ySum = 0;
        float rotation_zSum = 0;
        float thermSum = 0;
        float presAvg = 0;
        float resistanceAvg = 0;
        
        for (int i=0; i < NUMSAMPLES; i++) {
           presSum += bmpSamples[i];
           rotation_xSum += bnoXSamples[i];
           rotation_ySum += bnoYSamples[i];
           rotation_zSum += bnoSamples[i];
           thermSum += thermSamples[i];
        }
  
        presAvg = presSum/NUMSAMPLES;
        alt = 44330*(1 - pow((presAvg/SEALEVELPRESSURE_HPA), (1/5.255))) + altCorrection;

        //CALCULATE DELTA ALT
        previousAlt = currentAlt;
        currentAlt = alt;
        if(!(currentAlt > 1000 || currentAlt < 0)){
          deltaAlt[deltaAltSampleIndex] = currentAlt - previousAlt; 
          previousAlts[deltaAltSampleIndex] = currentAlt;
        
          if(deltaAltSampleIndex == 9){
            deltaAltSampleIndex = 0;
          }else{
            deltaAltSampleIndex++;
          }
        }else{
          Serial2.println("THROWING OUT ALTITUDE: OUT OF RANGE: " + String(currentAlt));
          Serial1.println("THROWING OUT ALTITUDE: OUT OF RANGE: " + String(currentAlt));
        }
        
        rotation_x = rotation_xSum/NUMSAMPLES;
        rotation_y = rotation_ySum/NUMSAMPLES;
        rotation_z = rotation_zSum/NUMSAMPLES;
        resistanceAvg = thermSum/NUMSAMPLES;
  
        // convert the value to resistance
        resistanceAvg = 1023 / resistanceAvg - 1;
        resistanceAvg = SERIESRESISTOR / resistanceAvg;
          
        // calculate temperature from resistance
        thermTemp = ((log(resistanceAvg / SERIESRESISTOR))/BCOEFFICIENT) + 1.0 / (TEMPERATURENOMINAL + 273.15);
        thermTemp = (1.0 / thermTemp) - 273.15; 
  
        // round to nearest tenth
        thermTemp = float(int(thermTemp * 10 + 0.5))/10;
        thermTempStr = String(thermTemp);
        thermTempStr.remove(thermTempStr.length() - 1);
        
        /*CREATE AND PRINT PACKET*/
        String packet = openLogPacket(missionTime, alt, thermTempStr, rotation_x, rotation_y, rotation_z, openLogAverageDeltaAlt);
        writeSD(packet);
        
        /*RESET PRINT TIMER*/
        printDelayStart = millis();

      }
      if((currentTs - altCheckDelayStart) > altCheckDelayNum){
       calculateDeltaAlt();
       altCheckDelayStart = millis();
     }


    /*SEND PACKET*/
    if((currentTs - sendDelayStart >= sendDelayNum) && sendTelem == true){
      String XBEEWrite = xbeePacket(teamID, missionTime, packetCount, packetType, alt, String(bmpTemperature), rotation_z, openLogAverageDeltaAlt);
      writeXBee(XBEEWrite);  
      sendDelayStart = millis();
    }

}

String xbeePacket(const int teamID, String missionTime, int &packetCount, String packetType, float alt, String thermTemp, float rotationZ, float avgDeltaAlt){
  
    unsigned long runMillis= millis();
    unsigned long allSeconds=millis()/1000;
    int runHours= allSeconds/3600;
    int secsRemaining=allSeconds%3600;
    int runMinutes=secsRemaining/60;
    int runSeconds=secsRemaining%60;
    
    missionTime = String(runHours) + ":" + String(runMinutes) + ":" + String(runSeconds);
    
    packetCount += 1;
    
    altStr = String(alt);
    altStr = altStr.substring(0, altStr.length() - 1);
    temStr = String(thermTemp);
    
    String message = "<" + String(teamID) + "," + "," + "," + packetType + "," + altStr + "," + temStr + "," + String(rotationZ) + "," + String(avgDeltaAlt) + ">"; 

    return message;
}

String openLogPacket(String missionTime, float alt, String thermTemp, float rotationX, float rotationY, float rotationZ, float avgDeltaAlt){
  
    unsigned long runMillis= millis();
    unsigned long allSeconds=millis()/1000;
    int runHours= allSeconds/3600;
    int secsRemaining=allSeconds%3600;
    int runMinutes=secsRemaining/60;
    int runSeconds=secsRemaining%60;
    
    missionTime = String(runHours) + ":" + String(runMinutes) + ":" + String(runSeconds);
    
    openLogPacketCount += 1;
    
    String message = String(teamID) + "," + runMillis + "," + String(openLogPacketCount) + "," + packetType + "," + String(alt) + "," + thermTemp + "," + String(rotationX) + "," + String(rotationY) + "," + String(rotationZ) + "," + String(avgDeltaAlt); 

    return message;
}

void calculateDeltaAlt(){ 
  //calculate the average change in altitude of the past 10 delta altitudes 
  float total;
  float averageDeltaAlt;
  for(int i = 0; i < 10; i++){
    total += deltaAlt[i];
  }
  averageDeltaAlt = total / 10;
  openLogAverageDeltaAlt = averageDeltaAlt;
}


void writeSD(String packet){
  Serial1.println(packet);
  //Serial1.println("");
}

void writeXBee(String packet){

  Serial2.println(packet);
}

String readXBee(){
  
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

    String msg = receivedChars;

    Serial1.println(msg);
    Serial1.println("command received");
    return msg;
  
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

    if (stringVersionReceivedChars == "CMD,2617,SP1X,ON"){
        sendTelem = true;
        Serial2.println("RECIEVEED SP1X ON");
     }else if (stringVersionReceivedChars == "CMD,2617,SP1X,OFF"){
        sendTelem = false;
        Serial2.println(sendTelem);
        Serial2.println("RECIEVEED SP1X OFF");
     }
    
    newData = false;
    }
}
