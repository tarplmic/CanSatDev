#include <arduino-timer.h>
#include <init.h> //includes necessary libraries and initializes data vars
#include <Sensors.h> //initialize and get data from sensors
#include <SerialDefinitions.h> //initialize Serial2 and Serial3
#include <TimeFunctions.h> //function to grab time (in terms of arduino millis)

Sensors sensors(6);
TimeFunctions timeFunctions;
Timer<10> timer;

void setup() {
  // put your setup code here, to run once:
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);
  
  Serial2.begin(9600); 
  while (!Serial2)
  {
    Serial2.print("xbee aint starting");
  };
  Serial2.println("test");

  sensors.init();
  //timer.every(1000, function_to_call);
  timer.every(1000, getSensData);
  timer.every(1000, printToXbee);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  timer.tick();
}

bool function_to_call(void *argument) {
    int ts = millis();
    Serial2.println("tick " + String(ts));
    return true;
}

bool getSensData(void *argument){
    tem = sensors.getTemp();
    alt = sensors.getAlt();
    gpsLat = sensors.getLat();
    gpsLong = sensors.getLong();
    voltage = sensors.getBattVoltage();
}

bool printToXbee(void *argument){
  missionTime = timeFunctions.getTime();
  int ts = millis();
  Serial2.println(String(ts));
  Serial2.println(String(teamId) + "," + missionTime + "," + String(packetCount) + "," + packetType + "," + mode + "," + sp1Released + "," + sp2Released + "," + String(alt) + "," + String(tem) +
                  "," + String(voltage) + "," + gpsTime + "," + String(gpsLat) + "," + String(gpsLong) + "," + String(gpsAlt) + "," + String(gpsSats) + "," + String(flightStage) + "," + String(sp1PacketCount) + "," +
                  String(sp2PacketCount) + "," + lastCommand);
}
