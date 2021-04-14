#include <Arduino.h>   // required before wiring_private.h
#include "wiring_private.h" // pinPeripheral() function
#include <HardwareSerial.h>

#define PIN_SERIAL_RX2 (38) //microcontrollers RX, Xbee TX
#define PIN_SERIAL_TX2 (22) //microcontroller's TX, xbee RX
#define PAD_SERIAL_RX2 (SERCOM_RX_PAD_1)
#define PAD_SERIAL_TX2 (UART_TX_PAD_0)

#define PIN_SERIAL_RX3 (35) //microcontrollers RX, Xbee TX
#define PIN_SERIAL_TX3 (37) //microcontroller's TX, xbee RX
#define PAD_SERIAL_RX3 (SERCOM_RX_PAD_1)
#define PAD_SERIAL_TX3 (UART_TX_PAD_0)

//Serial1 already defined in arduino 

Uart Serial2 (&sercom2, PIN_SERIAL_RX2, PIN_SERIAL_TX2, PAD_SERIAL_RX2, PAD_SERIAL_TX2 );
void SERCOM2_Handler()
{
  Serial2.IrqHandler();
}

Uart Serial3 (&sercom1, PIN_SERIAL_RX3, PIN_SERIAL_TX3, PAD_SERIAL_RX3, PAD_SERIAL_TX3);
void SERCOM1_Handler()
{
  Serial3.IrqHandler();
}
