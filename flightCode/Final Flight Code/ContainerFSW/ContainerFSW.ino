//GND TELEMTRY FORMAT
//teamId,missionTime,packetCount,packetType,mode,sp1Released,sp2Released,altitude,temp,voltage,gpsTime,gpsLat,gpsLong,gpsAlt,gpsSats,flightStage,sp1PacketCount,sp2PacketCount,lastCommand
//GND COMMAND FORMAT
//CMD,2617,CX,PING

//INCLUDE ALL HEADER FILES NEEDED
#include <init.h> //includes necessary libraries and initializes data vars
#include <Sensors.h> //initialize and get data from sensors
#include <SerialDefinitions.h> //initialize Serial and Serial3
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
const int readDelayNum = 30;
int readDelayStart;
const int altCheckDelayNum = 1000;
int altCheckDelayStart;
const int gpsDelayNum = 1000;
int gpsDelayStart;
const int simGotDataCheckNum = 1300;
int simGotDataCheckStart;

//container sensor vars, alt correction, and othe global vars are defined in init.h
//DEFINE PAYLOAD SENSOR VARS FOR RELAY

//DEFINE FLASH VARIABLES 
const int DO_WRITE_TO_FLASH = 0;
FlashStorage(flightStageFlash, int);
FlashStorage(altCorrectionFlash, int);
FlashStorage(packetCountFlash, int);
FlashStorage(sendTelemFlash, int);
FlashStorage(flightModeFlash, int);

int doSendData = 0;

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
  Serial3.begin(9600);
  while (!Serial3){ Serial.print("xbee2 aint starting"); };
  Serial.begin(9600); 
  while (!Serial){ Serial1.print("xbee aint starting"); };
  Serial.println("start test");
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

    flightModeFromFlash = flightModeFlash.read();
    if(flightModeFromFlash == 0){
      mode = "F";
    }else if(flightModeFromFlash == 1){
      mode = "S";
    }
    
    packetCount = packetCountFlash.read();
    doSendData = sendTelemFlash.read();
    
    Serial2.println("alt Correction");
    Serial2.println(altCorrection);
    Serial2.println("flight stage");
    Serial2.println(flightStage);
    Serial2.println("packetCount");
    Serial2.println(packetCount);
    Serial2.println("doSendData");
    Serial2.println(doSendData);
  }
  
  sensors.init();
  float dummy = sensors.getTemp();
  dummy = sensors.getPressure();
  dummy = sensors.getTemp();
  dummy = sensors.getPressure();

  Serial2.println("Initialize Sensors");
  
  Serial1.println("past sensor init");
  Serial1.println("Time, Alt, Temp, Voltage, gpsTime, Lat, Long, gpsAlt, gpsSats, flightStage, lastCommand, altCorrection");
  
  sensorDelayStart = millis();
  printDelayStart = millis();
  readDelayStart = millis();
  altCheckDelayStart = millis();
  gpsDelayStart = millis();
  gpsDelayStart = millis();
  simGotDataCheckStart = millis();

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

    if(mode == "F"){
      pres = sensors.getPressure();
      //pres = fakeData[x] / 100;
      //x++;
      //Serial.println(44330*(1 - pow((pres/SEALEVELPRESSURE_HPA), (1/5.255))));
      bmpAltSamples[sampleIndex] = 44330*(1 - pow((pres/SEALEVELPRESSURE_HPA), (1/5.255))) - altCorrection;

      if(altDivisor < 10){
        altDivisor++;
      }
    }

    tem = sensors.getTemp();
    voltageSamples[sampleIndex] = sensors.getBattVoltage();
    //rawRotRateX[sampleIndex] = sensors.getRotRateX();
    //rawRotRateY[sampleIndex] = sensors.getRotRateY();
    //rawRotRateZ[sampleIndex] = sensors.getRotRateZ();
    accx = sensors.getAccX();
    accy = sensors.getAccY();
    accz = sensors.getAccZ();

    //PERFORM AVERAGING
    float totalAltitudes = 0;
    float totalVoltages = 0;
    //float totalRotRateX = 0;
    //float totalRotRateY = 0;
    //float totalRotRateZ = 0;
    for(int i = 0; i < 10; i++){
      totalAltitudes += bmpAltSamples[i];
      totalVoltages += voltageSamples[i];
      //totalRotRateX += rawRotRateX[i];
      //totalRotRateY += rawRotRateY[i];
      //totalRotRateZ += rawRotRateZ[i];
    }
    if(mode == "F"){
      alt = totalAltitudes / altDivisor;
    }
    voltage = totalVoltages / 10;
    //rotRate[0] = totalRotRateX / 10;
    //rotRate[1] = totalRotRateY / 10;
    //rotRate[2] = totalRotRateZ / 10;
    
    //CALCULATE DELTA ALT HERE IF IN FLIGHT MODE
    if(mode == "F"){
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
        Serial2.println("THROWING OUT ALTITUDE: OUT OF RANGE: " + String(currentAlt));
        Serial1.println("THROWING OUT ALTITUDE: OUT OF RANGE: " + String(currentAlt));
      }
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

  //INTERVAL TO ASSUME WE ARE AT THE LAST ALTITUDE RECEIVED FROM GND IF IN SIM MODE AND HAVE NOT RECEIVED DATA FROM GND IN MORE THAN __ SECONDS
  /*if(mode == "S"){
    if((currentTs - simGotDataCheckStart) > simGotDataCheckNum){
      //Serial.println("assuming at same altitude as before: " + String(currentAlt));
      
      bmpAltSamples[simPresSampleIndex] = currentAlt;

      if(recFirstSimp) //if we have already received atleast one simp command, can increment altDivisor until it reaches 9
      {
        if(altDivisor < 10){
          altDivisor++;
        }
      }

      if(simPresSampleIndex == 9){
        simPresSampleIndex = 0;
      }else{
        simPresSampleIndex++;
      }

      previousAlt = currentAlt;
      currentAlt = calculatedAlt;

      mostRecentSimDeltaAlt = currentAlt - previousAlt;
      
      simGotDataCheckStart = millis();
    }
  }*/

  //INTERVAL TO GET GPS DATA
  if((currentTs - gpsDelayStart) > gpsDelayNum){
    gpsLat = sensors.getLat();
    gpsLong = sensors.getLong();
    gpsAlt = sensors.getGPSAlt();
    gpsTime = sensors.getGPSTime();
    gpsSats = sensors.getNumSats();
    gpsDelayStart = millis();
  }
  
  //INTERVAL TO PRINT TO SERIAL DEVICES
  if((currentTs - printDelayStart) > printDelayNum){
    printToXbee();
    printDelayStart = millis();
  }

  //INTERVAL TO READ FROM SERIAL DEVICES
  if((currentTs - readDelayStart) > readDelayNum){
     recvWithStartEndMarkers();
     showNewData();
     //delay(10);
     recvWithStartEndMarkers2();
     showNewData2();
     recvWithStartEndMarkers3();
     showNewData3();
     readDelayStart = millis();
   }

   if((currentTs - altCheckDelayStart) > altCheckDelayNum){
     altitudeCheck();
     altCheckDelayStart = millis();
   }

   //MAY NEED A SEPERATE FUNCTINO TO CALL TO CHECK ALTITUDE IN SIM MODE IF WANT TO DO AN ALITUDE CHECK ONLY EVERY SECOND
}

void printToXbee(){
  if(doSendData){
    missionTime = timeFunctions.getTime();

    altStr = String(alt);
    altStr = altStr.substring(0, altStr.length() - 1);
    temStr = String(tem);
    temStr = temStr.substring(0, temStr.length() - 1);
    gpsLatStr = String(gpsLat);
    gpsLatStr = gpsLatStr.substring(0, 2);
    gpsLatStr += ".";
    gpsLatStr += String(gpsLat).substring(2, 6);
    gpsLongStr = String(gpsLong);
    gpsLongStr = gpsLongStr.substring(0, 3);
    gpsLongStr += ".";
    gpsLongStr += String(gpsLong).substring(3, 6);
    gpsAltStr = String(gpsAlt);
    gpsAltStr = gpsAltStr.substring(0, gpsAltStr.length() - 1);
    
    Serial2.println(String(teamId) + "," + missionTime + "," + String(packetCount) + "," + packetType + "," + mode + "," + sp1Released + "," + sp2Released + "," + altStr + "," + temStr +
                  "," + String(voltage) + "," + gpsTime + "," + gpsLatStr + "," + gpsLongStr + "," + gpsAltStr + "," + String(gpsSats) + "," + String(flightStage) + "," + String(sp1PacketCount) + "," +
                  String(sp2PacketCount) + "," + lastCommand + "," + altCorrection + "," + String(openLogAverageDeltaAlt) + "," + String(accx) + "," + String(accy) + "," + String(accz));
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
  
  if(mode == "F"){
    firstDeltaAltMin = -1.0;
    secondDeltaAltMin = -0.75;
    fs1ReqNum = 4;
  
    for(int i = 0; i < 10; i++){
      total += deltaAlt[i];
    }
    averageDeltaAlt = total / 10;
    
  }else{ //if mode == "S"
    firstDeltaAltMin = -5.0;
    secondDeltaAltMin = -3.0;
    fs1ReqNum = 4;
    
    /*for(int i = 0; i < 3; i++){
      total += simDeltaAlt[i];
    }
    averageDeltaAlt = total / 3;*/

    averageDeltaAlt = mostRecentSimDeltaAlt;
    
  }
  
  //Serial.println("avg delta alt: " + String(averageDeltaAlt));
  openLogAverageDeltaAlt = averageDeltaAlt;

  if(averageDeltaAlt < firstDeltaAltMin && flightStage != "falling" && FS1reqCounter == 0){ //need to hit -1.0 atleast one time to start the check if we are falling
    FS1reqCounter++;
    Serial2.println("FS1reqCounter incremented");
    
  }else if(averageDeltaAlt < secondDeltaAltMin && flightStage != "falling"){ //has to be atleast -0.75 five times after it was initially -1.0
    FS1reqCounter++;
    Serial2.println("FS1reqCounter incremented");
    
  }else{//so we did not meet the requirement consecutively and we have not transitioned yet
    if(FS1reqCounter != 0){
      FS1reqCounter = 0;
      Serial2.println("FS1reqCounter reset to 0");
    }
  }

  if(FS1reqCounter >= fs1ReqNum){ //if we meet requirments 6 times in a row (for three seconds since altitudeCheck is called every 500ms)
    Serial2.println("transition to flight stage 1");
    Serial2.println(averageDeltaAlt);
    Serial1.println("transition to flight stage 1");
    Serial1.println(averageDeltaAlt);
    
    flightStage = "falling";

    if(DO_WRITE_TO_FLASH){
      Serial2.println("WARNING: ABOUT TO WRITE TO FLASH: FLIGHT STAGE");
      flightStageFlash.write(1);
    }
  }

  int shouldDeploy1 = 1;
  int shouldDeploy2 = 1;
  //if we are falling 
  if(flightStage == "falling"){
    if(mode == "F"){
      for(int i = 0; i < 10; i++){
        if (!(previousAlts[i] <= (500))){
          shouldDeploy1 = 0;
        }
        if(!(previousAlts[i] <= (400))){
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
    
   }else{//if we are in sim mode
    if (!(alt <= 500 && previousAlt <= 500)){ //if the previous alt and current are not both under 500, do not deploy, reset req counter
      shouldDeploy1 = 0;
      deploySP1reqCounter = 0;
      
    }else{ //if the previous alt and current are both under 500, increment req counter
      deploySP1reqCounter++;
    }
    
    if(!(alt <= 400 && previousAlt <= 400)){//if the previous alt and current are not both under 400, do not deploy, reset req counter
      shouldDeploy2 = 0;
      deploySP2reqCounter = 0;
      
    }else{//if the previous alt and current are both under 400, increment req counter
      deploySP2reqCounter++;
    }
      
   }
   
   if(shouldDeploy1 && sp1Released == "N" && deploySP1reqCounter > 2){
      Serial2.println("DEPLOY PAYLOAD 1");
      Serial1.println("DEPLOY PAYLOAD 1");
      sensors.releaseServo1();
      sp1Released = "R";
    }
    if(shouldDeploy2 && sp2Released == "N" && deploySP2reqCounter > 2){
      Serial2.println("DEPLOY PAYLOAD 2");
      Serial1.println("DEPLOY PAYLOAD 2");
      sensors.releaseServo2();
      sp2Released = "R";
    }
   
  }
  /*Serial.println("averageDeltaAlt");
  Serial.println(averageDeltaAlt);
  Serial.println("alt");
  Serial.println(alt);*/
  
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
          Serial2.println("CMD_2617_CX_PING");
          lastCommand = "PING";
          
        }else if(stringVersionReceivedChars == "CMD,2617,CX,RELEASE"){
          //Serial.println("RELEASE_CMD_RECIEVED");
          lastCommand = "RELEASE";
          
          //activate servos to release
          sensors.releaseServo1();
          sensors.releaseServo2();

        }else if(stringVersionReceivedChars.substring(0, 28) == "CMD,2617,CX,SETALTCORRECTION"){
          //Serial.println("alt command recieved");
          
          String sentAltCorrect = stringVersionReceivedChars.substring(29);
          altCorrection = sentAltCorrect.toInt();
          lastCommand = "SETALTCORRECTION";

          if(DO_WRITE_TO_FLASH){
            Serial2.println("WARNING: ABOUT TO WRITE TO FLASH: ALT CORRECTION");
            altCorrectionFlash.write(altCorrection);
          }
          
        }else if(stringVersionReceivedChars == "CMD,2617,CX,CLEARFLASH"){
          //Serial.println("received command to clear flash");
            altCorrection = 0;
            flightStage = "rising";
            doSendData = 0;
            packetCount = 0;
            mode = "F";
            if(DO_WRITE_TO_FLASH){
              lastCommand = "CLEARFLASH";
              Serial2.println("WARNING: ABOUT TO RESET FLASH VALUES");
              // clear values
              flightStageFlash.write(0);
              altCorrectionFlash.write(0);
              sendTelemFlash.write(0);
              packetCountFlash.write(0);
              flightModeFlash.write(0);
            }
              
        }else if(stringVersionReceivedChars == "CMD,2617,CX,ON"){
              //Serial.println("recieved on command");
              lastCommand = "CXON";
              doSendData = 1;
              if(DO_WRITE_TO_FLASH){
                Serial2.println("WARNING: ABOUT TO WRITE TO FLASH: doSendTelem");
                sendTelemFlash.write(doSendData);
              }
              
        }else if(stringVersionReceivedChars == "CMD,2617,CX,OFF"){
          //Serial.println("recieved off command");
          lastCommand = "CXOFF";
          doSendData = 0;

          if(DO_WRITE_TO_FLASH){
            Serial2.println("WARNING: ABOUT TO WRITE TO FLASH: doSendTelem");
            sendTelemFlash.write(doSendData);
          }
        }else if (stringVersionReceivedChars == "CMD,2617,SP1X,ON"){
          lastCommand = "SP1X_ON";
          Serial3.println("<CMD,2617,SP1X,ON>");
          
        }else if (stringVersionReceivedChars == "CMD,2617,SP1X,OFF"){
          lastCommand = "SP1X_OFF";
          Serial3.println("<CMD,2617,SP1X,OFF>");
          
        }else if (stringVersionReceivedChars == "CMD,2617,SP2X,ON"){
          lastCommand = "SP2X_ON";
          Serial.println("<CMD,2617,SP2X,ON>");
          
        }else if (stringVersionReceivedChars == "CMD,2617,SP2X,OFF"){
          lastCommand = "SP2X_OFF";
          Serial.println("<CMD,2617,SP2X,OFF>");
          
          
        }else if(stringVersionReceivedChars == "CMD,2617,CX,ZEROSERVOS"){
          //Serial.println("recieved zero servos command");
          lastCommand = "ZEROSERVOS";
          myServo2.attach(9); //pin 9 arduino is pin 12 samd
          myServo2.write(0);
          delay(100);
          myServo2.detach();
          myServo1.attach(15); //pin 15 arduino is pin 7 samd
          myServo1.write(0);
          delay(100);
          myServo1.detach();
          
        }else if(stringVersionReceivedChars == "CMD,2617,SIM,ENABLE"){
          //Serial.println("recieved sim enable command");
          simEnableRec = 1;
          lastCommand = "SIM_ENABLE";
          
        }else if(stringVersionReceivedChars == "CMD,2617,SIM,ACTIVATE"){
          //Serial.println("recieved sim activate command");
          if(simEnableRec){
            Serial2.println("SWITCH TO SIMULATION MODE");
            mode = "S";

            lastCommand = "SIM_ACTIVATE";

            //clear altitude samples from sensor 
            memset(bmpAltSamples, 0, sizeof(bmpAltSamples));
            altDivisor = 1;
            deltaAltSampleIndex = 0;
            currentAlt = 0;
            flightStage = "rising";
            altCorrection = 0;
            deploySP1reqCounter = 0;
            deploySP2reqCounter = 0;

            if(DO_WRITE_TO_FLASH){
              Serial2.println("WARNING: ABOUT TO RESET FLASH VALUES");
              // clear values
              flightStageFlash.write(0);
              altCorrectionFlash.write(0);
              flightModeFlash.write(1);
            }
            
          }
          
        }else if(stringVersionReceivedChars == "CMD,2617,SIM,DISABLE"){
          //Serial.println("recieved sim disable command");
          mode = "F";
          if(DO_WRITE_TO_FLASH){
            Serial2.println("WARNING: ABOUT TO WRITE TO FLASH: flightModeFlash");
            flightModeFlash.write(0);
          }
          memset(bmpAltSamples, 0, sizeof(bmpAltSamples));
          altDivisor = 0;
          recFirstSimp = false;
          deltaAltSampleIndex = 0;
          simEnableRec = 0;
          flightStage = "rising";
          altCorrection = 0;
          deploySP1reqCounter = 0;
          deploySP2reqCounter = 0;

          if(DO_WRITE_TO_FLASH){
              Serial2.println("WARNING: ABOUT TO RESET FLASH VALUES");
              // clear values
              flightStageFlash.write(0);
              altCorrectionFlash.write(0);
              flightModeFlash.write(0);
            }

          lastCommand = "SIM_DISABLE";
          
        }else if(stringVersionReceivedChars.substring(0, 13) == "CMD,2617,SIMP"){
          if(mode == "S"){
            simGotDataCheckStart = millis(); //reset got data check (if we actually received each simp data packet from ground once a second, this gotDataCheck wouldn't execute)

            //before, could get away with not checking if data is errenous before adding as an alitude to be averaged, but because in sim mode we only get alt data every second, one erroneous could screw up our average alot
            //need to check that the altiude is not less than 0 or above 1000 before adding
            
            pres = stringVersionReceivedChars.substring(14).toFloat();
            calculatedAlt = 44330*(1 - pow((pres/101325), (1/5.255)));
            
            if(recFirstSimp) //if we have already received atleast one simp command, can increment altDivisor until it reaches 9
            {
              if(altDivisor < 10){
                altDivisor++;
              }
              calculatedAlt -= altCorrection;
              
            }else{
              altCorrection = calculatedAlt;
              if(DO_WRITE_TO_FLASH){
                Serial2.println("WARNING: ABOUT TO WRITE TO FLASH: ALT CORRECTION");
                altCorrectionFlash.write(altCorrection);
              }
            }

            if(!recFirstSimp){ //if this is the first simp command we've received, say that the altiude does make sense (need a baseline)
              altMakesSense = 1;
              
            }else{
              //add some code to determine if the altiude makes sense based on the previous alt, current alt, and delta between the two and either do or dont set altMakesSense
              //previous alt at his point is still the previous altitude and calculatedAlt is the new one. mostRecentSimDeltaAlt would be the previous delta alt, can calculate new delta alt with prev and calculated
              //if(fishyAltFlag)
              //somehow compare fishyDeltaAlt (previous delta alt with weird data), mostRecentSimDeltaAlt (previous previous delta alt (before weird data), and the current delta (calculatedAlt - fishyAlt)
              //fishyAltFlag = 0
              //either setaltMakesSense to 0 or 1 depending on result of comparison  //actually may not need to do this b/c going to add in next line
              //if altMakesSense //if we see it does make sense to add, add it. if not, dont do anything with data (throw out)
              //add the fishyDeltaAlt to samples and make mostRecentSimDeltaAlt = fishyDeltaAlt
              //altMakesSense = 0 //reset altMakesSense so that one line later it will actually be comparing new data and deciding wether to add that
              //
              //if abs value delta alt > 100 //interest is first peaked, add to temp variable to store fishy data fishyAlt
              //set fishyAltFlag = 1
              //fishyAlt = calculatedAlt
              //fishyDeltaALt = fishyAlt - previousAlt //previous alt will stay as the previous alt (not changed to fishyAlt the next time through)
              //altmakesSense = 0 //wouldnt acallt have to do this


              //if delta alt > threshold
              //dont make most recSimDeltaAltIt
              //dont set global alt variable to it

              //for now until we decide what to do with this, keep as always being 1 (always add the pressure reading)
              altMakesSense = 1;
              
            }
            
            if(!(calculatedAlt > (1000) || calculatedAlt < (0))){
              if(altMakesSense){
                /*bmpAltSamples[simPresSampleIndex] = calculatedAlt;

                if(simPresSampleIndex == 9){
                  simPresSampleIndex = 0;
                }else{
                  simPresSampleIndex++;
                }*/
                alt = calculatedAlt;
  
                previousAlt = currentAlt;
                currentAlt = calculatedAlt;
  
                mostRecentSimDeltaAlt = currentAlt - previousAlt; 
  
                /*simDeltaAlt[deltaAltSampleIndex] = currentAlt - previousAlt; 
                previousAlts[deltaAltSampleIndex] = currentAlt;
              
                if(deltaAltSampleIndex == 2){
                  deltaAltSampleIndex = 0;
                }else{
                  deltaAltSampleIndex++;
                }*/
  
                //Serial.println("curr: " + String(currentAlt) + " prev: " + String(previousAlt) + " delta: " + String(currentAlt - previousAlt));
  
                recFirstSimp = true; //make true because we have recieved atleast one simp command 
              }
            }

            altMakesSense = 0; //reset altMakesSense
          }
        }
        
        newData = false;
    }
}

void recvWithStartEndMarkers2() {
    static boolean recvInProgress2 = false;
    static byte ndx = 0;
    char startMarker = '<';
    char endMarker = '>';
    char rc;
    
    while (Serial3.available() > 0 && newData2 == false) {
        rc = Serial3.read();

        if (recvInProgress2 == true) {
            if (rc != endMarker) {
                receivedChars2[ndx] = rc;
                ndx++;
                if (ndx >= numChars2) {
                    ndx = numChars2 - 1;
                }
            }
            else {
                receivedChars2[ndx] = '\0'; // terminate the string
                recvInProgress2 = false;
                ndx = 0;
                newData2 = true;
            }
        }

        else if (rc == startMarker) {
            recvInProgress2 = true;
        }
    }
}

void showNewData2() {
  String firstPart;
  String secondPart;
  String middlePart;
  String sendToGnd;
  missionTime = timeFunctions.getTime();
  if (newData2 == true) {
        String stringVersionReceivedChars;
        stringVersionReceivedChars = receivedChars2;
        /*2617,,,S1,53,38.9,-0.03,-0.02*/
        firstPart = stringVersionReceivedChars.substring(0, 4);
        secondPart = stringVersionReceivedChars.substring(7);
        middlePart = "," + String(missionTime) + "," + String(packetCount) + ",";
        sendToGnd = firstPart + middlePart + secondPart;
        Serial2.println(sendToGnd);
        packetCount += 1;
        sp1PacketCount += 1;
        
        newData2 = false;
    }
}

void recvWithStartEndMarkers3() {
    static boolean recvInProgress3 = false;
    static byte ndx = 0;
    char startMarker = '<';
    char endMarker = '>';
    char rc;

    while (Serial.available() > 0 && newData3 == false) {
        rc = Serial.read();
        //Serial2.println("in serial avail");

        if (recvInProgress3 == true) {
            if (rc != endMarker) {
                receivedChars3[ndx] = rc;
                ndx++;
                if (ndx >= numChars3) {
                    ndx = numChars3 - 1;
                }
            }
            else {
                receivedChars3[ndx] = '\0'; // terminate the string
                recvInProgress3 = false;
                ndx = 0;
                newData3 = true;
            }
        }

        else if (rc == startMarker) {
            recvInProgress3 = true;
        }
    }
}

void showNewData3() {
  String firstPart;
  String secondPart;
  String middlePart;
  String sendToGnd;
  
  missionTime = timeFunctions.getTime();
  if (newData3 == true) {
        String stringVersionReceivedChars;
        stringVersionReceivedChars = receivedChars3;
        /*2617,,,S1,53,38.9,-0.03,-0.02*/
        firstPart = stringVersionReceivedChars.substring(0, 4);
        secondPart = stringVersionReceivedChars.substring(7);
        middlePart = "," + String(missionTime) + "," + String(packetCount) + ",";
        sendToGnd = firstPart + middlePart + secondPart;
        Serial2.println(sendToGnd);
        packetCount += 1;
        sp2PacketCount += 1;
        
        newData3 = false;
    }
}

/*
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
    //Serial.println("serial 3 avail");
    //while(Serial3.available() > 0){
      //Serial.println(String(Serial3.read()));
    //}
    
    if(Serial3.read() == 0x7E){
      for (int i = 1; i<15; i++){
        byte discardByte = Serial3.read();
      }
      while(Serial3.available() > 0){
        char dataIn = Serial3.read();
        //if (dataIn == 0x7E){
          //break;
        //}
        //else {
          incomingData += dataIn; 
        //}
      }
      incomingData.remove(incomingData.length()-1);
    }
  } 
  if (incomingData != ""){
    Serial.println(incomingData);
  }
}
*/
