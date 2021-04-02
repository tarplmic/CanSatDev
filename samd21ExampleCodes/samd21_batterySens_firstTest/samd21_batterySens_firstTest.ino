#include <Arduino.h>   // required before wiring_private.h
#include "wiring_private.h" // pinPeripheral() function

#define PIN_SERIAL_RX (38) //microcontrollers RX, Xbee TX
#define PIN_SERIAL_TX (22) //microcontroller's TX, xbee RX
#define PAD_SERIAL_RX (SERCOM_RX_PAD_1)
#define PAD_SERIAL_TX (UART_TX_PAD_0)

#define BATTERYSENSORPIN A1  //PB08, pin 7
  

 Uart Serial2 (&sercom4, PIN_SERIAL_RX, PIN_SERIAL_TX, PAD_SERIAL_RX, PAD_SERIAL_TX );
void SERCOM4_Handler()
{
  Serial2.IrqHandler();
}
 
void setup(void) {
  pinPeripheral(PIN_SERIAL_RX, PIO_SERCOM);
  pinPeripheral(PIN_SERIAL_TX, PIO_SERCOM);

  Serial2.begin(9600); 
  while (!Serial2)
  {
    Serial1.print("xbee aint starting");
  };
  Serial2.println("about to test battery voltage sensor");
  // connect AREF to 3.3V and use that as VCC, less noisy!
  analogReadResolution(12);
  analogReference(AR_DEFAULT);
}
 
void loop(void) {
  float analogReading = analogRead(BATTERYSENSORPIN);
  float analogVoltage = (analogReading * 3.3) / 4095;
  float voltage = analogVoltage * ((33000 + 10000) / 10000);
  Serial2.print(analogReading);
  Serial2.print(", ");
  Serial2.print(analogVoltage);
  Serial2.print(", ");
  Serial2.println(voltage);
 
  delay(1000);
}
