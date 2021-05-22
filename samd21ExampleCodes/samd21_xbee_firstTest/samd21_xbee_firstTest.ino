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

const byte numChars = 40;
char receivedChars[numChars];
boolean newData = false;

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
  digitalWrite(ledPin, LOW);
  
  Serial2.begin(9600); 
  while (!Serial2){ Serial1.print("xbee aint starting"); };
  readDelayStart = millis();
  printDelayStart = millis();

}

int currentTs;
void loop() {
  //Serial.println("test");
  currentTs = millis();
   if((currentTs - printDelayStart) > printDelayNum){
    Serial2.println("test");
    printDelayStart = millis();
  }
  
  if((currentTs - readDelayStart) > readDelayNum){
     recvWithStartEndMarkers();
     showNewData();
     readDelayStart = millis();
   }
  
}

void recvWithStartEndMarkers() {
    static boolean recvInProgress = false;
    static byte ndx = 0;
    char startMarker = '<';
    char endMarker = '>';
    char rc;
 
    while (Serial.available() > 0 && newData == false) {
        rc = Serial.read();

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
        String stringVersionReceivedChars;
        stringVersionReceivedChars = receivedChars;
        
        if(stringVersionReceivedChars == "CMD,2617,CX,PING"){
          Serial.println("CMD_2617_CX_PING");
        }
        
        newData = false;
    }
}
