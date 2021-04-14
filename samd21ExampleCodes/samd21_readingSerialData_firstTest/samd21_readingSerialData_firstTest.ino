
// Example 3 - Receive with start- and end-markers
//#include <init.h>
#include "wiring_private.h" // pinPeripheral() function

#define PIN_SERIAL_RX3 (24) //microcontrollers RX, Xbee TX
#define PIN_SERIAL_TX3 (23) //microcontroller's TX, xbee RX
#define PAD_SERIAL_RX3 (SERCOM_RX_PAD_3)
#define PAD_SERIAL_TX3 (UART_TX_PAD_2)

const byte numChars = 32;
char receivedChars[numChars];

boolean newData = false;
Uart Serial2 (&sercom4, PIN_SERIAL_RX3, PIN_SERIAL_TX3, PAD_SERIAL_RX3, PAD_SERIAL_TX3);
void SERCOM4_Handler()
{
  Serial2.IrqHandler();
}

void setup() {
    Serial1.begin(9600);

    //pinPeripheral(PIN_SERIAL_RX3, PIO_SERCOM_ALT);
    //pinPeripheral(PIN_SERIAL_TX3, PIO_SERCOM_ALT);
    Serial2.begin(9600); 
    while (!Serial2)
    {
      Serial2.print("xbee aint starting");
    };
    Serial2.println("<Arduino is ready>");
}

void loop() {
    recvWithStartEndMarkers();
    showNewData();
    //Serial2.println("test");
}

void recvWithStartEndMarkers() {
    static boolean recvInProgress = false;
    static byte ndx = 0;
    char startMarker = '<';
    char endMarker = '>';
    char rc;
 
    while (Serial2.available() > 0 && newData == false) {
        rc = Serial2.read();

        if (recvInProgress == true) {
            if (rc != endMarker) {
                receivedChars[ndx] = rc;
                ndx++;
                if (ndx >= numChars) {
                    ndx = numChars - 1;
                }
            }
            else {
                receivedChars[ndx] = '\0'; // terminate the string
                recvInProgress = false;
                ndx = 0;
                newData = true;
            }
        }

        else if (rc == startMarker) {
            recvInProgress = true;
        }
    }
}

void showNewData() {
    if (newData == true) {
        Serial2.print("This just in ... ");
        Serial2.println(receivedChars);
        newData = false;
    }
}
