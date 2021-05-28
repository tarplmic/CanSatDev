#include <Arduino.h>   // required before wiring_private.h
#include "wiring_private.h" // pinPeripheral() function

/*#define PIN_SERIAL_RX4 (5) //microcontroller's RX, xbee TX, PA15, 24
#define PIN_SERIAL_TX4 (2) //microcontrollers TX, Xbee RX, PA14, 23
#define PAD_SERIAL_RX4 (SERCOM_RX_PAD_3)
#define PAD_SERIAL_TX4 (UART_TX_PAD_2)*/

/*#define PIN_SERIAL_RX4 (38) //microcontrollers RX, Xbee TX, PA13, 22
#define PIN_SERIAL_TX4 (22) //microcontroller's TX, xbee RX, PA12, 21
#define PAD_SERIAL_RX4 (SERCOM_RX_PAD_1)
#define PAD_SERIAL_TX4 (UART_TX_PAD_0)*/

/*#define PIN_SERIAL_RX2 (24) //microcontrollers RX, Xbee TX, PB11, 20
#define PIN_SERIAL_TX2 (23) //microcontroller's TX, xbee RX, PB10, 19
#define PAD_SERIAL_RX2 (SERCOM_RX_PAD_3)
#define PAD_SERIAL_TX2 (UART_TX_PAD_2)*/

//PA20 - 29 - micro TX
//PA21 - 30 - micro RX
/*#define PIN_SERIAL_RX4 (22) //microcontrollers TX, Xbee RX, PA12, 21
#define PIN_SERIAL_TX4 (38) //microcontroller's RX, xbee TX, PA13, 22
#define PAD_SERIAL_RX4 (SERCOM_RX_PAD_3)
#define PAD_SERIAL_TX4 (UART_TX_PAD_2)*/

//PA20 - 29
//PA21 - 30
/*#define PIN_SERIAL_RX4 (7) //microcontrollers RX, Xbee TX, PA21 - 30
#define PIN_SERIAL_TX4 (6) //microcontrollers TX, Xbee RX, PA20 - 29
#define PAD_SERIAL_RX4 (SERCOM_RX_PAD_3)
#define PAD_SERIAL_TX4 (UART_TX_PAD_2)*/

const int ledPin =  6;

/*Uart Serial4 (&sercom2, PIN_SERIAL_RX4, PIN_SERIAL_TX4, PAD_SERIAL_RX4, PAD_SERIAL_TX4 );
void SERCOM2_Handler()
{
  Serial4.IrqHandler();
}*/

/*Uart Serial2 (&sercom4, PIN_SERIAL_RX2, PIN_SERIAL_TX2, PAD_SERIAL_RX2, PAD_SERIAL_TX2 );
void SERCOM4_Handler()
{
  Serial2.IrqHandler();
}*/
#include <SerialDefinitions.h> //initialize Serial2 and Serial3

const byte numChars3 = 50;
char receivedChars3[numChars3];
boolean newData3 = false;

const int readDelayNum = 10;
int readDelayStart;
const int printDelayNum = 1000;
int printDelayStart;

void setup() {
  //Serial1.begin(115200);
  //Serial1.begin(9600);
 // while(!Serial1); 
  //Serial1.println("trying out xbee");
  
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);
  
  Serial.begin(9600); 
  while (!Serial){ Serial1.print("xbee aint starting"); };
  Serial.println("start tes");
  readDelayStart = millis();
  printDelayStart = millis();

}

int currentTs;
void loop() {
  //Serial.println("test");
  currentTs = millis();
   if((currentTs - printDelayStart) > printDelayNum){
    Serial.println("test");
    printDelayStart = millis();
  }
  
  if((currentTs - readDelayStart) > readDelayNum){
     recvWithStartEndMarkers3();
     showNewData3();
     readDelayStart = millis();
   }
  
}

void recvWithStartEndMarkers3() {
    static boolean recvInProgress3 = false;
    static byte ndx = 0;
    char startMarker = '<';
    char endMarker = '>';
    char rc;

    while (Serial.available() > 0 && newData3 == false) {
        rc = Serial.read();
        Serial.println("in serial avail");

        if (recvInProgress3 == true) {
            if (rc != endMarker) {
                receivedChars3[ndx] = rc;
                ndx++;
                if (ndx >= numChars3) {
                    ndx = numChars3 - 1;
                }
            }
            else {
                receivedChars3[ndx] = '\0'; // terminate the string
                recvInProgress3 = false;
                ndx = 0;
                newData3 = true;
            }
        }

        else if (rc == startMarker) {
            recvInProgress3 = true;
        }
    }
}

void showNewData3() {
  String firstPart;
  String secondPart;
  String middlePart;
  String sendToGnd;
  
  if (newData3 == true) {
        String stringVersionReceivedChars;
        stringVersionReceivedChars = receivedChars3;
        Serial.println(stringVersionReceivedChars);
        newData3 = false;
    }
}
