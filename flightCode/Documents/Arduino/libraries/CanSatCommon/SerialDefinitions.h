#include <Arduino.h>   // required before wiring_private.h
#include "wiring_private.h" // pinPeripheral() function
#include <HardwareSerial.h>

#define PIN_SERIAL_RX2 (24) //microcontrollers RX, Xbee TX, PB11, 20
#define PIN_SERIAL_TX2 (23) //microcontroller's TX, xbee RX, PB10, 19
#define PAD_SERIAL_RX2 (SERCOM_RX_PAD_3)
#define PAD_SERIAL_TX2 (UART_TX_PAD_2)

#define PIN_SERIAL_RX3 (35) //microcontrollers TX, Xbee RX, PA16, 25
#define PIN_SERIAL_TX3 (37) //microcontroller's RX, xbee TX, PA17, 26
#define PAD_SERIAL_RX3 (SERCOM_RX_PAD_1)
#define PAD_SERIAL_TX3 (UART_TX_PAD_0)

// #define PIN_SERIAL_RX4 (2) //microcontrollers TX, Xbee RX, PA14, 23
// #define PIN_SERIAL_TX4 (5) //microcontroller's RX, xbee TX, PA15, 24
// #define PAD_SERIAL_RX4 (SERCOM_RX_PAD_3)
// #define PAD_SERIAL_TX4 (UART_TX_PAD_2)

//Serial1 already defined in arduino 
//pins 15 (PA10), pin 16 (PA11), SERCOM 0

// pins 31 and 32 are taken by i2c SERCOM 5 or 3

Uart Serial2 (&sercom4, PIN_SERIAL_RX2, PIN_SERIAL_TX2, PAD_SERIAL_RX2, PAD_SERIAL_TX2 );
void SERCOM4_Handler()
{
  Serial2.IrqHandler();
}

Uart Serial3 (&sercom1, PIN_SERIAL_RX3, PIN_SERIAL_TX3, PAD_SERIAL_RX3, PAD_SERIAL_TX3);
void SERCOM1_Handler()
{
  Serial3.IrqHandler();
}
