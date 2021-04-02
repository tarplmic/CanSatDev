#include <Arduino.h>   // required before wiring_private.h
#include "wiring_private.h" // pinPeripheral() function

#define PIN_SERIAL_RX (38) //microcontrollers RX, Xbee TX
#define PIN_SERIAL_TX (22) //microcontroller's TX, xbee RX
#define PAD_SERIAL_RX (SERCOM_RX_PAD_1)
#define PAD_SERIAL_TX (UART_TX_PAD_0)

const int ledPin =  6;

Uart Serial2 (&sercom4, PIN_SERIAL_RX, PIN_SERIAL_TX, PAD_SERIAL_RX, PAD_SERIAL_TX );
void SERCOM4_Handler()
{
  Serial2.IrqHandler();
}
  
void setup() {
  //Serial1.begin(115200);
  //Serial1.begin(9600);
 // while(!Serial1); 
  //Serial1.println("trying out xbee");
  pinPeripheral(PIN_SERIAL_RX, PIO_SERCOM);
  pinPeripheral(PIN_SERIAL_TX, PIO_SERCOM);
  
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  
  Serial2.begin(9600); 
  while (!Serial2);

}

void loop() {
  Serial2.println("test");
  digitalWrite(ledPin, HIGH);
  //Serial1.print("tried xbee ");
  delay(1000);
  digitalWrite(ledPin, LOW);
  delay(1000);
  
}
