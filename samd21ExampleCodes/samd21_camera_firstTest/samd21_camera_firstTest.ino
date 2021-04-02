#include <Arduino.h>   // required before wiring_private.h
#include "wiring_private.h" // pinPeripheral() function

#define PIN_SERIAL_RX (37) //microcontrollers RX, Xbee TX
#define PIN_SERIAL_TX (35) //microcontroller's TX, xbee RX
#define PAD_SERIAL_RX (SERCOM_RX_PAD_1)
#define PAD_SERIAL_TX (UART_TX_PAD_0)

const int cameraPin = 43; //pa02, pin 3

Uart Serial3 (&sercom1, PIN_SERIAL_RX, PIN_SERIAL_TX, PAD_SERIAL_RX, PAD_SERIAL_TX );
void SERCOM1_Handler()
{
  Serial3.IrqHandler();
}

void setup() {
  // put your setup code here, to run once:
  //pinMode(cameraPin, OUTPUT);
  
  //digitalWrite(cameraPin, LOW); 
  //delay(1000);

  pinPeripheral(PIN_SERIAL_RX, PIO_SERCOM);
  pinPeripheral(PIN_SERIAL_TX, PIO_SERCOM);

  Serial3.begin(9600); 
  while (!Serial3)
  {
    Serial1.print("xbee aint starting");
  };
}

void loop() {
  // put your main code here, to run repeatedly:
  /*digitalWrite(cameraPin, HIGH);
  delay(50);
  digitalWrite(cameraPin, LOW);
   
  delay(5000);               
 
  digitalWrite(cameraPin, HIGH);
  delay(50);
  digitalWrite(cameraPin, LOW); */
  Serial3.println("test"); 

  delay(1000);
}
