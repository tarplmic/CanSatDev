
// Example 3 - Receive with start- and end-markers
#include "wiring_private.h" // pinPeripheral() function

#define ledPin 6

#define PIN_SERIAL_RX3 (38) //microcontrollers RX, Xbee TX
#define PIN_SERIAL_TX3 (22) //microcontroller's TX, xbee RX
#define PAD_SERIAL_RX3 (SERCOM_RX_PAD_1)
#define PAD_SERIAL_TX3 (UART_TX_PAD_0)

int rc;

Uart Serial2 (&sercom4, PIN_SERIAL_RX3, PIN_SERIAL_TX3, PAD_SERIAL_RX3, PAD_SERIAL_TX3);
void SERCOM4_Handler()
{
  Serial2.IrqHandler();
}
    
void setup() {
    
    Serial1.begin(9600);
    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, LOW);

    pinPeripheral(PIN_SERIAL_RX3, PIO_SERCOM_ALT);
    pinPeripheral(PIN_SERIAL_TX3, PIO_SERCOM_ALT);
    Serial2.begin(9600); 
    while (!Serial2)
    {
      Serial2.print("xbee aint starting");
    };
    Serial2.println("<Arduino is ready>");
}

void loop() {
  while (Serial2.available() > 0){
    digitalWrite(ledPin, HIGH);
    rc = Serial2.read();
  }
  Serial2.println("test");
  //digitalWrite(ledPin, LOW);
}
