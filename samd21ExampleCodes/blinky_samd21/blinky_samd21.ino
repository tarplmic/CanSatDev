#include <Adafruit_BNO055.h>

Adafruit_BNO055 bno = Adafruit_BNO055(55); //on same i2c pins as bmp

const int ledPin =  6;
const int buzzerPin = 28; //pin 33 on samd

void setup() {
  // put your setup code here, to run once:
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);
  pinMode(buzzerPin, OUTPUT);

  /*Serial1.begin(115200); //openlog
  while (!Serial1) { Serial1.println("open log aint starting"); };
  Serial1.println("begin test");

  if(!bno.begin())
   {
       while(1){
           Serial1.println("bno sucks");
       };
   }
   delay(10);
   bno.setExtCrystalUse(true);*/
    
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(ledPin, HIGH);
  digitalWrite(buzzerPin, HIGH);
  delay(500);
  
  digitalWrite(ledPin, LOW);
  digitalWrite(buzzerPin, LOW);
  delay(500);

  /*imu::Vector<3> rotSpeed = bno.getVector(Adafruit_BNO055::VECTOR_GYROSCOPE);
  float gyrox = rotSpeed.x();
  float gyroy = rotSpeed.y();
  float gyroz = rotSpeed.z();

  Serial1.println(String(gyrox) + "," + String(gyroy) + "," + String(gyroz));*/
}
