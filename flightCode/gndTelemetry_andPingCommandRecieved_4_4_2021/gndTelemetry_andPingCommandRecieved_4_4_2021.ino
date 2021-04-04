//GND TELEMTRY FORMAT
//teamId,missionTime,packetCount,packetType,mode,sp1Released,sp2Released,altitude,temp,voltage,gpsTime,gpsLat,gpsLong,gpsAlt,gpsSats,flightStage,sp1PacketCount,sp2PacketCount,lastCommand
//GND COMMAND FORMAT
//CMD,2617,CX,PING

//INCLUDE ALL HEADER FILES NEEDED
#include <init.h> 

//DEFINE SENSORS
Adafruit_BMP3XX bmp; // I2C
Adafruit_BNO055 bno = Adafruit_BNO055(55); //on same i2c pins as bmp
SFE_UBLOX_GNSS myGPS;

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

//DEFINE RECEIVING DATA VARS
const byte numChars = 32;
char receivedChars[numChars];
boolean newData = false;

void setup() {
  //TURN ON LED
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);

  //INITIALIZE SERIALS
  Serial1.begin(115200); //openlog
  while (!Serial1)
  {
    Serial1.println("open log aint starting");
  };
  pinPeripheral(PIN_SERIAL_RX, PIO_SERCOM);
  pinPeripheral(PIN_SERIAL_TX, PIO_SERCOM);
  Serial1.begin(9600); 
  while (!Serial1)
  {
    Serial1.print("xbee aint starting");
  };

  //INITIALIZE BMP
  if (!bmp.begin()) {
    while (1){
      Serial1.println("Could not find a valid BMP3 sensor, check wiring!");
    };
  }
  bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_8X);
  bmp.setPressureOversampling(BMP3_OVERSAMPLING_4X);
  bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);

  //INITIALIZE BNO
  if(!bno.begin())
  {
    while(1){
       Serial1.println("bno sucks");
    };
  }
  delay(10);
  bno.setExtCrystalUse(true);

  //INITIALIZE GPS
  Wire.begin();
  if (myGPS.begin() == false)
  {
    Serial.println(F("u-blox GNSS module not detected at default I2C address. Please check wiring. Freezing."));
    while (1);
  }

  //INITALIZE BATTERY SENSOR
  analogReadResolution(12);
  analogReference(AR_DEFAULT);
  
  //START TIMER VARS
  sensorDelayStart = millis();
  printDelayStart = millis();
  
}

void loop() {
  int currentTs = millis();
  
  //INTERVAL TO GET SENSOR DATA
  if((currentTs - sensorDelayStart) > sensorDelayNum){
    if (! bmp.performReading()) {
      Serial1.println("Failed to perform reading :(");
      Serial1.println("Failed to perform reading :(");
      return;
    }
    
    //BMP DATA
    tem = bmp.temperature; //celcius
    pres = bmp.pressure; //Pa
    alt = bmp.readAltitude(SEALEVELPRESSURE_HPA) + altCorrection; //meters
    //GPS DATA
    gpsLat = myGPS.getLatitude();
    gpsLat = gpsLat / 10000000;
    gpsLong = myGPS.getLongitude();
    gpsLong = gpsLong / 10000000;
    //BATTERY DATA
    analogReading = analogRead(BATTERYSENSORPIN);
    analogVoltage = (analogReading * 3.3) / 4095;
    voltage = analogVoltage * ((33000 + 10000) / 10000);

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
  Serial1.print(msg);
  Serial1.print(msg);
}

void recvWithStartEndMarkers() {
    static boolean recvInProgress = false;
    static byte ndx = 0;
    char startMarker = '<';
    char endMarker = '>';
    char rc;
 
    while (Serial1.available() > 0 && newData == false) {
        rc = Serial1.read();

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
          Serial1.println("PING_RECIEVED");
        }
        newData = false;
    }
}
