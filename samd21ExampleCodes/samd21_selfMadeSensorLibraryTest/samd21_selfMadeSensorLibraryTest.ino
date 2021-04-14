#include <init.h>
#include <Sensors.h>

Sensors sensor(6);
float altitude;
float rotRate;
float latitude;
float longitude; 
float battVolt;

void setup() {
  // put your setup code here, to run once:
  Serial2.begin(9600); 
  while (!Serial2)
  {
    Serial2.print("xbee aint starting");
  };
  sensor.init();
}

void loop() {
  // put your main code here, to run repeatedly:
  altitude = sensor.getAlt();
  rotRate = sensor.getRotRate();
  latitude = sensor.getLat();
  longitude = sensor.getLong();
  battVolt = sensor.getBattVoltage();
  
  Serial2.println(rotRate);
  delay(1000);
}
