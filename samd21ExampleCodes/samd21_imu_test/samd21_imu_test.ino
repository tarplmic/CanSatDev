#include <Wire.h>
#include <Arduino.h>   // required before wiring_private.h
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include "wiring_private.h" // pinPeripheral() 
#include "Adafruit_BMP3XX.h"

#define PIN_SERIAL_RX (38) //microcontrollers RX, Xbee TX
#define PIN_SERIAL_TX (22) //microcontroller's TX, xbee RX
#define PAD_SERIAL_RX (SERCOM_RX_PAD_1)
#define PAD_SERIAL_TX (UART_TX_PAD_0)

#define BNO_SDA_PIN 4
#define BNO_SCL_PIN 3
#define PERIPH_WIRE sercom2
#define WIRE_IT_HANDLER SERCOM2_Handler

Uart Serial2 (&sercom4, PIN_SERIAL_RX, PIN_SERIAL_TX, PAD_SERIAL_RX, PAD_SERIAL_TX );
void SERCOM4_Handler()
{
  Serial2.IrqHandler();
}

//Adafruit_BNO055 bno = Adafruit_BNO055(55);
Adafruit_BMP3XX bmp;

//TwoWire myWire(&sercom2, BNO_SDA_PIN, BNO_SCL_PIN);

void setup() {
  // put your setup code here, to run once:
  pinPeripheral(PIN_SERIAL_RX, PIO_SERCOM);
  pinPeripheral(PIN_SERIAL_TX, PIO_SERCOM);
  
  Serial2.begin(9600);
  Serial2.println("trying out imu");
  
  if (!bmp.begin()) {
    Serial2.println("Could not find a valid BMP3 sensor, check wiring!");
    while (1);
  }
  bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_8X);
  bmp.setPressureOversampling(BMP3_OVERSAMPLING_4X);
  bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);
  
  //if(!bno.begin())
  //{
    //Serial2.println("bno sucks");
    //while(1);
  //}
  delay(1000);
  //bno.setExtCrystalUse(true);
 }

void loop() {
  // put your main code here, to run repeatedly:
  //sensors_event_t event;
  //bno.getEvent(&event);
  if (!bmp.performReading()) {
    Serial2.println("Failed to perform reading :(");
    //return;
  }
  Serial2.print("X: ");
  //Serial2.print(event.orientation.x, 4);
  Serial2.print("\tY: ");
  //Serial2.print(event.orientation.y, 4);
  Serial2.print("\tZ: ");
  //Serial2.print(event.orientation.z, 4);
  Serial2.println("");
  Serial2.print("\ttemp: ");
  Serial.println(bmp.temperature);
  delay(100);
}
