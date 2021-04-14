
#include <init.h>  


Adafruit_BNO055 bno = Adafruit_BNO055(55);
//Adafruit_BMP3XX bmp;

//TwoWire myWire(&sercom2, BNO_SDA_PIN, BNO_SCL_PIN);

void setup() {
  
  Serial2.begin(9600);
  Serial2.println("trying out imu");
  
  /*if (!bmp.begin()) {
    Serial2.println("Could not find a valid BMP3 sensor, check wiring!");
    while (1);
  }
  bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_8X);
  bmp.setPressureOversampling(BMP3_OVERSAMPLING_4X);
  bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);*/
  
  if(!bno.begin())
  {
    Serial2.println("bno sucks");
    while(1);
  }
  delay(1000);
  bno.setExtCrystalUse(true);
 }

void loop() {
  // put your main code here, to run repeatedly:
  //if (!bmp.performReading()) {
   // Serial2.println("Failed to perform reading :(");
    //return;
  //}

  imu::Vector<3> rotSpeed = bno.getVector(Adafruit_BNO055::VECTOR_GYROSCOPE);
//raw value is degrees per second, we want rotations per minute
//radians per second to rotations per minute (60/(2*PI))
//degress per second to rotations per minute (60/360)
  float gyrox = rotSpeed.x();
  float gyroy = rotSpeed.y();
  float gyroz = rotSpeed.z();
  Serial2.print("X: ");
  Serial2.print(gyrox);
  Serial2.print("     Y: ");
  Serial2.print(gyroy);
  Serial2.print("     Z: ");
  Serial2.print(gyroz);
  Serial2.println("");
  //Serial2.print("\ttemp: ");
  //Serial.println(bmp.temperature);
  delay(500);
}
