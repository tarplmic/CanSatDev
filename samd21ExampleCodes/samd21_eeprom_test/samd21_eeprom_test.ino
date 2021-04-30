#include <init.h> //includes necessary libraries and initializes data vars
#include <Sensors.h> //initialize and get data from sensors
#include <SerialDefinitions.h> //initialize Serial2 and Serial3
#include <TimeFunctions.h> //function to grab time (in terms of arduino millis)
#include <FlashStorage.h>

Sensors sensors(6);
FlashStorage(flightStageFlash, int);
FlashStorage(altCorrectionFlash, int);

const int readDelayNum = 10;
int readDelayStart;

//OTHER GLOBAL VARS TO DEFINE
int incomingByte = 0;

//DEFINE RECEIVING DATA VARS
const byte numChars = 40;
char receivedChars[numChars];
boolean newData = false;

void setup() {
  // put your setup code here, to run once:
  Serial2.begin(9600); 
  while (!Serial2){ Serial1.print("xbee aint starting"); };
  Serial2.println("test");
  
  Serial2.println(altCorrection);
  // Read the content of "my_flash_store" and assign it to "number"
  altCorrection = altCorrectionFlash.read();
  Serial2.println(altCorrection);
  
}

int currentTs;
void loop() {
   currentTs = millis();
  // put your main code here, to run repeatedly:
  if((currentTs - readDelayStart) > readDelayNum){
     recvWithStartEndMarkers();
     showNewData();
     readDelayStart = millis();
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
          Serial2.println("CMD_2617_CX_PING");
          
        }else if(stringVersionReceivedChars == "CMD,2617,CX,RELEASE"){
          Serial2.println("RELEASE_CMD_RECIEVED");

        }else if(stringVersionReceivedChars.substring(0, 28) == "CMD,2617,CX,SETALTCORRECTION"){
          Serial2.println(stringVersionReceivedChars);
          String sentAltCorrect = stringVersionReceivedChars.substring(29);
          altCorrection = sentAltCorrect.toInt();
          Serial2.println("ALT CORRECTION RECIEVED: " + altCorrection);
          
          altCorrectionFlash.write(altCorrection);
        }
        newData = false;
    }
}
