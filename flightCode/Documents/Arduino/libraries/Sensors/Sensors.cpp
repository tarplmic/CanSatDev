#include "Arduino.h"
#include "Sensors.h"

Sensors::Sensors(int ledPin)
{
    //pinMode(ledPin, OUTPUT);
    _pin = ledPin;
    //digitalWrite(_pin, HIGH);

    SEALEVELPRESSURE = 1013.25;
    BATTERYSENSORPIN = A2;  //PB08, pin 8
    CAMERAPIN = 43; //pa02, pin 3
    BUZZERPIN = 28; //pa24, pin 33
}

void Sensors::init()
{
    //pinMode(CAMERAPIN, OUTPUT);
    //digitalWrite(CAMERAPIN, LOW);

    if (!bmp.begin_I2C()) {
        while (1){
            digitalWrite(_pin, LOW);
        };
    }
    bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_8X);
    bmp.setPressureOversampling(BMP3_OVERSAMPLING_4X);
    bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);
    
    /*if(!bno.begin())
    {
        while(1){
             digitalWrite(_pin, LOW);
        };
    }
    delay(10);
    bno.setExtCrystalUse(true);*/

    Wire.begin();
    DS3231_init(DS3231_CONTROL_INTCN);

    Wire.begin();
    if (myGPS.begin() == false)
    {
        //Serial.println(F("u-blox GNSS module not detected at default I2C address. Please check wiring. Freezing."));
        while (1){
            digitalWrite(_pin, LOW);
        };
    }

    analogReadResolution(12);
    analogReference(AR_DEFAULT);

    pinMode(BUZZERPIN, OUTPUT);
    digitalWrite(BUZZERPIN, HIGH);

    // digitalWrite(CAMERAPIN, HIGH);
    // delay(50);
    // digitalWrite(CAMERAPIN, LOW);
}

float Sensors::getPressure()
{
    if (!bmp.performReading()) {
      return 0;
    }
    float tem = bmp.temperature; //celcius
    float pres = bmp.pressure / 100.0F; //Pa
    float alt = 44330*(1 - pow((pres/SEALEVELPRESSURE), (1/5.255)));
    return pres;
}

float Sensors::getTemp()
{
    if (!bmp.performReading()) {
      return 0;
    }
    float tem = bmp.temperature; //celcius
    return tem;
}

//never updates, stays 0,0,0
// float* Sensors::getRotRate(){
//     imu::Vector<3> rotSpeed = bno.getVector(Adafruit_BNO055::VECTOR_GYROSCOPE);
//     float gyrox = rotSpeed.x();
//     float gyroy = rotSpeed.y();
//     float gyroz = rotSpeed.z();
//     float rotRate[3] = {gyrox, gyroy, gyroz};
//     return rotRate;
// }

// float Sensors::getRotRateX(){
//     imu::Vector<3> rotSpeed = bno.getVector(Adafruit_BNO055::VECTOR_GYROSCOPE);
//     float gyrox = rotSpeed.x();
//     return gyrox;
// }

// float Sensors::getRotRateY(){
//     imu::Vector<3> rotSpeed = bno.getVector(Adafruit_BNO055::VECTOR_GYROSCOPE);
//     float gyroy = rotSpeed.y();
//     return gyroy;
// }

// float Sensors::getRotRateZ(){
//     imu::Vector<3> rotSpeed = bno.getVector(Adafruit_BNO055::VECTOR_GYROSCOPE);
//     float gyroz = rotSpeed.z();
//     return gyroz;
// }

float Sensors::getAccX(){
    imu::Vector<3> acc = bno.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER);
    float accx = acc.x();
    return accx;
}

float Sensors::getAccY(){
    imu::Vector<3> acc = bno.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER);
    float accy = acc.y();
    return accy;
}

float Sensors::getAccZ(){
    imu::Vector<3> acc = bno.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER);
    float accz = acc.z();
    return accz;
}

float Sensors::getLat(){
    float latitude = myGPS.getLatitude(50);
    //latitude = latitude / 10000000;
    
    //return 4;
    return latitude;
}

float Sensors::getLong(){
    float longitude = myGPS.getLongitude(50);
    //longitude = longitude / 10000000;
    return longitude;
    //return 2;
    //return myGPS.getSIV();
}

float Sensors::getGPSAlt(){
    float altitude = myGPS.getAltitude(50);
    return altitude / 1000;
}

String Sensors::getGPSTime(){
    String gpsTime = "";
    if (myGPS.getDateValid(50) == true)
    {
        gpsTime += myGPS.getYear(50);
        gpsTime += "-";
        gpsTime += myGPS.getMonth(50);
        gpsTime += "-";
        gpsTime += myGPS.getDay(50);
        gpsTime += "-";
    }else{
        gpsTime += "notvalid:";
    }

    if (myGPS.getTimeValid(50) == true)
    {
        gpsTime += myGPS.getHour(50);
        gpsTime += ":";
        gpsTime += myGPS.getMinute(50);
        gpsTime += ":";
        gpsTime += myGPS.getSecond(50);
    }else{
        gpsTime += "notvalid";
    }
    return gpsTime;
}

int Sensors::getNumSats(){
    return myGPS.getSIV(50);
}

float Sensors::getBattVoltage(){
    float analogReading = analogRead(BATTERYSENSORPIN);
    float analogVoltage = (analogReading * 3.3) / 4095;
    float voltage = analogVoltage * ((33000 + 10000) / 10000);
    return voltage;
}

void Sensors::releaseServo1(){
    delay(100);
    myServo1.attach(15); //pin 15 arduino is pin 7 samd
    //myServo1.write(110);
    //delay(100);
    myServo1.write(0);
    delay(1000);
    myServo1.detach();
}

void Sensors::releaseServo2(){
    delay(100);
    myServo2.attach(9); //pin 9 arduino is pin 12 samd
    //myServo2.write(80);
    //delay(100);
    myServo2.write(0);
    delay(1000);
    myServo2.detach();
}

void Sensors::stopCamera(){
    digitalWrite(CAMERAPIN, HIGH);
    delay(50);
    digitalWrite(CAMERAPIN, LOW);
}

String Sensors::getMissionTime(){
    struct ts t; 
    String time = "";
    DS3231_get(&t);
    time += t.hour;
    time += ":";
    time += t.min;
    time += ":";
    time += t.sec;
    return time;

}

void Sensors::setMissionTime(int hour, int min, int sec){
    struct ts t; 
    t.hour = hour; 
    t.min = min;
    t.sec = sec;
 
    DS3231_set(t); 
}

void Sensors::stopBuzzer(){
    digitalWrite(BUZZERPIN, HIGH);

}

void Sensors::startBuzzer(){
    digitalWrite(BUZZERPIN, LOW);
}