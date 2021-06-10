//GND TELEMTRY FORMAT
//teamId,missionTime,packetCount,packetType,mode,sp1Released,sp2Released,altitude,temp,voltage,gpsTime,gpsLat,gpsLong,gpsAlt,gpsSats,flightStage,sp1PacketCount,sp2PacketCount,lastCommand
//GND COMMAND FORMAT
//CMD,2617,CX,PING

//INCLUDE ALL HEADER FILES NEEDED
#include <init.h> //includes necessary libraries and initializes data vars
#include <Sensors.h> //initialize and get data from sensors
#include <SerialDefinitions.h> //initialize Serial2 and Serial3
#include <TimeFunctions.h> //function to grab time (in terms of arduino millis)

//DEFINE SENSORS
Sensors sensors(6);
TimeFunctions timeFunctions;
Servo myServo1;
Servo myServo2;

//DEFINE DELAY VARS
const int sensorDelayNum = 100;
int sensorDelayStart;
const int printDelayNum = 1000;
int printDelayStart;
const int readDelayNum = 10;
int readDelayStart;
const int altCheckDelayNum = 500;
int altCheckDelayStart;
const int gpsDelayNum = 1000;
int gpsDelayStart;

//container sensor vars, alt correction, and othe global vars are defined in init.h
//DEFINE PAYLOAD SENSOR VARS FOR RELAY

//DEFINE FLASH VARIABLES 
const int DO_WRITE_TO_FLASH = 1;
FlashStorage(flightStageFlash, int);
FlashStorage(altCorrectionFlash, int);

int doSendData = 1;

int openLogPacketCount = 0;
float openLogAverageDeltaAlt;

void setup() {
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);

  Serial1.begin(115200); //openlog
  while (!Serial1) { Serial1.println("open log aint starting"); };
  Serial1.println("begin test");
  Serial2.begin(9600); 
  while (!Serial2){ Serial1.print("xbee aint starting"); };
  Serial2.println("STARTING CONTAINER SOFTWARE");
  Serial1.println("past serial begin");

  if(DO_WRITE_TO_FLASH){
    Serial1.println("ABOUT TO READ INITIAL VARS FROM FLASH");
    altCorrection = altCorrectionFlash.read();

    flightStageFromFlash = flightStageFlash.read();
    if(flightStageFromFlash == 0){
      flightStage = "rising";
    }else if(flightStageFromFlash == 1){
      flightStage = "falling";
    }
    
    Serial1.println("alt Correction");
    Serial1.println(altCorrection);
    Serial1.println("flight stage");
    Serial1.println(flightStage);
  }
  
  sensors.init();
  float dummy = sensors.getTemp();
  dummy = sensors.getPressure();
  dummy = sensors.getTemp();
  dummy = sensors.getPressure();

  Serial2.println("Initialized Sensors");
  Serial1.println("past sensor init");
  Serial1.println("Time, Alt, Temp, Voltage, gpsTime, Lat, Long, gpsAlt, gpsSats, flightStage, lastCommand, altCorrection");
  
  sensorDelayStart = millis();
  printDelayStart = millis();
  readDelayStart = millis();
  altCheckDelayStart = millis();
  gpsDelayStart = millis();

  //delay(10000);
  //sensors.stopCamera();
  
}

int x = 0;
int currentTs;

void loop() {
  currentTs = millis();
  
  //INTERVAL TO GET SENSOR DATA
  if((currentTs - sensorDelayStart) > sensorDelayNum){
    //ACCUIRE ALL RAW SENSOR DATA AND ADD TO ARRAYS
    tem = sensors.getTemp();
    pres = sensors.getPressure();
    //pres = fakeData[x] / 100;
    //x++;
    bmpAltSamples[sampleIndex] = 44330*(1 - pow((pres/SEALEVELPRESSURE_HPA), (1/5.255))) - altCorrection;
    voltageSamples[sampleIndex] = sensors.getBattVoltage();

    //PERFORM AVERAGING
    float totalAltitudes = 0;
    float totalVoltages = 0;
    for(int i = 0; i < 10; i++){
      totalAltitudes += bmpAltSamples[i];
      totalVoltages += voltageSamples[i];
    }
    alt = totalAltitudes / 10;
    voltage = totalVoltages / 10;
    
    //CALCULATE DELTA ALT
    previousAlt = currentAlt;
    currentAlt = alt;
    if(!(currentAlt > (1000) || currentAlt < (0))){
      deltaAlt[deltaAltSampleIndex] = currentAlt - previousAlt; 
      previousAlts[deltaAltSampleIndex] = currentAlt;
    
      if(deltaAltSampleIndex == 9){
        deltaAltSampleIndex = 0;
      }else{
        deltaAltSampleIndex++;
      }
    }else{
      Serial2.println("THROWINGOUTALTITUDE:OUTOFRANGE:" + String(currentAlt));
      Serial1.println("THROWING OUT ALTITUDE: OUT OF RANGE:" + String(currentAlt));
    }

    //INCREMENT DATS SAMPLE INDEX
    if(sampleIndex == 9){
      sampleIndex = 0;
    }else{
      sampleIndex++;
    }

    Serial1.println(String(millis()) + "," + String(openLogPacketCount) + "," + String(alt) + "," + String(tem) +
                  "," + String(voltage) + "," + gpsTime + "," + String(gpsLat) + "," + String(gpsLong) + "," + String(gpsAlt) + "," + 
                  String(gpsSats) + "," + String(flightStage) + "," + lastCommand + "," + altCorrection + "," + String(openLogAverageDeltaAlt));
    openLogPacketCount++;

    sensorDelayStart = millis();
  }

  //INTERVAL TO GET GPS DATA
  if((currentTs - gpsDelayStart) > gpsDelayNum){
    gpsLat = sensors.getLat();
    gpsLong = sensors.getLong();
    gpsAlt = sensors.getGPSAlt();
    gpsTime = sensors.getGPSTime();
    gpsSats = sensors.getNumSats();
    gpsDelayStart = millis();

  }
  
  //INTERVAL TO PRINT TO XBEE
  if((currentTs - printDelayStart) > printDelayNum){
    printToXbee();
    printDelayStart = millis();
  }

  //INTERVAL TO READ FROM SERIAL DEVICES
  if((currentTs - readDelayStart) > readDelayNum){
     recvWithStartEndMarkers();
     showNewData();
     readDelayStart = millis();
   }

   if((currentTs - altCheckDelayStart) > altCheckDelayNum){
     altitudeCheck();
     altCheckDelayStart = millis();
   }
}

void printToXbee(){
  if(doSendData){
    missionTime = timeFunctions.getTime();
  
    Serial2.println(String(teamId) + "," + missionTime + "," + String(packetCount) + "," + packetType + "," + mode + "," + sp1Released + "," + sp2Released + "," + String(alt) + "," + String(tem) +
                  "," + String(voltage) + "," + gpsTime + "," + String(gpsLat) + "," + String(gpsLong) + "," + String(gpsAlt) + "," + String(gpsSats) + "," + String(flightStage) + "," + String(sp1PacketCount) + "," +
                  String(sp2PacketCount) + "," + lastCommand + "," + altCorrection + "," + String(openLogAverageDeltaAlt));
    packetCount += 1;
  }
}

void altitudeCheck(){ 

  //calculate the average change in altitude of the past 10 delta altitudes 
  float total;
  float averageDeltaAlt;
  float firstDeltaAltMin;
  float secondDeltaAltMin;
  int fs1ReqNum;
  
  firstDeltaAltMin = -1.0;
  secondDeltaAltMin = -0.75;
  fs1ReqNum = 7;

  for(int i = 0; i < 10; i++){
    total += deltaAlt[i];
  }
  averageDeltaAlt = total / 10;
    
  openLogAverageDeltaAlt = averageDeltaAlt;

 if(averageDeltaAlt < firstDeltaAltMin && flightStage != "falling" && FS1reqCounter == 0){ //need to hit -1.0 atleast one time to start the check if we are falling
    FS1reqCounter++;
    Serial2.println("FS1reqCounterincremented");
    
  }else if(averageDeltaAlt < secondDeltaAltMin && flightStage != "falling"){ //has to be atleast -0.75 five times after it was initially -1.0
    FS1reqCounter++;
    Serial2.println("FS1reqCounterincremented");
    
  }else{//so we did not meet the requirement consecutively and we have not transitioned yet
    if(FS1reqCounter != 0){
      FS1reqCounter = 0;
      Serial2.println("FS1reqCounterresetto0");
    }
  }

  if(FS1reqCounter >= fs1ReqNum){ //if we meet requirments 6 times in a row (for three seconds since altitudeCheck is called every 500ms)
    Serial2.println("transition_to_flight_stage_1");
    Serial2.println(averageDeltaAlt);
    Serial1.println("transition to flight stage 1");
    Serial1.println(averageDeltaAlt);
    
    flightStage = "falling";

    if(DO_WRITE_TO_FLASH){
      Serial2.println("WARNING:ABOUTTOWRITETOFLASH:FLIGHTSTAGE");
      flightStageFlash.write(1);
    }
  }

  int shouldDeploy1 = 1;
  int shouldDeploy2 = 1;
  //if we are falling 
  if(flightStage == "falling"){
    
    for(int i = 0; i < 10; i++){
      if (!(previousAlts[i] <= (515))){
        shouldDeploy1 = 0;
      }
      if(!(previousAlts[i] <= (415))){
        shouldDeploy2 = 0;
      }
    }

    if(shouldDeploy1){ //if it completed that for loop and shouldDeploy is still 1, increment the counter
      deploySP1reqCounter++;
    }else{ //if it did not meet the requirement, do not increment the counter
      deploySP1reqCounter = 0;
    }
    if(shouldDeploy2){//if it completed that for loop and shouldDeploy is still 1, increment the counter
      deploySP2reqCounter++;
    }else{//if it did not meet the requirement, do not increment the counter
      deploySP2reqCounter = 0;
    }
    
    if(shouldDeploy1 && sp1Released == "N" && deploySP1reqCounter > 2){
      Serial2.println("DEPLOYPAYLOAD1");
      Serial1.println("DEPLOY PAYLOAD 1");
      sensors.releaseServo1();
      sp1Released = "R";
    }
    if(shouldDeploy2 && sp2Released == "N" && deploySP2reqCounter > 2){
      Serial2.println("DEPLOYPAYLOAD2");
      Serial1.println("DEPLOY PAYLOAD 2");
      sensors.releaseServo2();
      sp2Released = "R";
    }

    //CHECK IF WE HAVE LANDED AFTER FALLING 
    if(abs(averageDeltaAlt) <= 0.14 && alt < 50){
      Serial2.println("landedIncremented");
      landedReqCounter++;
    }else{
      if(landedReqCounter != 0){
        Serial2.println("landedReset");
        landedReqCounter = 0;
      }
    }
    
    if(landedReqCounter > 2 && buzzerIsOn == 0){ //if we meet requirements to say we've landed
      //turn on buzzer
      Serial2.println("TURN ON BUZZER!!!");
      sensors.startBuzzer();
      buzzerIsOn = 1;
      
    }
   
  }
  
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
          lastCommand = "PING";
          
        }else if(stringVersionReceivedChars == "CMD,2617,CX,RELEASE"){
          Serial2.println("RELEASE_CMD_RECIEVED");
          lastCommand = "RELEASE";
          
          //activate servos to release
          sensors.releaseServo1();
          sensors.releaseServo2();

        }else if(stringVersionReceivedChars.substring(0, 28) == "CMD,2617,CX,SETALTCORRECTION"){
          Serial2.println("altcommandrecieved");
          
          String sentAltCorrect = stringVersionReceivedChars.substring(29);
          altCorrection = sentAltCorrect.toInt();
          lastCommand = "SETALTCORRECTION";

          if(DO_WRITE_TO_FLASH){
            Serial2.println("WARNING:ABOUTTOWRITETOFLASH:ALTCORRECTION");
            altCorrectionFlash.write(altCorrection);
          }
          
        }else if(stringVersionReceivedChars == "CMD,2617,CX,START_BUZZER"){
          lastCommand = "START_BUZZER";
          sensors.startBuzzer();
          
        }else if(stringVersionReceivedChars == "CMD,2617,CX,STOP_BUZZER"){
          lastCommand = "STOP_BUZZER";
          sensors.stopBuzzer();
          
         }else if(stringVersionReceivedChars == "CMD,2617,CX,CLEARFLASH"){
          Serial2.println("receivedcommandtoclearflash");
            altCorrection = 0;
            flightStage = "rising";
            FS1reqCounter = 0;
            deploySP1reqCounter = 0;
            deploySP2reqCounter = 0;
            landedReqCounter = 0;
            packetCount = 0;
            buzzerIsOn = 0;
            
            sp1Released = "N";
            sp2Released = "N";
            
            sensors.stopBuzzer();
            
            if(DO_WRITE_TO_FLASH){
              lastCommand = "CLEARFLASH";
              Serial2.println("WARNING:ABOUTTORESETFLASHVALUES");
              // clear values
              flightStageFlash.write(0);
              altCorrectionFlash.write(0);
            }
              
        }else if(stringVersionReceivedChars == "CMD,2617,CX,ON"){
              Serial2.println("recievedoncommand");
              lastCommand = "CXON";
              doSendData = 1;
              
        }else if(stringVersionReceivedChars == "CMD,2617,CX,OFF"){
          Serial2.println("recieved off command");
          lastCommand = "CXOFF";
          doSendData = 0;
          
        }else if(stringVersionReceivedChars == "CMD,2617,CX,ZEROSERVOS"){
          Serial2.println("recieved zero servos command");
          lastCommand = "ZEROSERVOS";
          myServo2.attach(9); //pin 9 arduino is pin 12 samd
          myServo2.write(0);
          delay(100);
          myServo2.detach();
          myServo1.attach(15); //pin 15 arduino is pin 7 samd
          myServo1.write(0);
          delay(100);
          myServo1.detach();
        }
        
        newData = false;
    }
}
