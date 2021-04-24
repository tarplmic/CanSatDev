#include "SAMDTimerInterrupt.h"
#include "SAMD_ISR_Timer.h"
#include <init.h> //includes necessary libraries and initializes data vars
#include <Sensors.h> //initialize and get data from sensors
#include <SerialDefinitions.h> //initialize Serial2 and Serial3
#include <TimeFunctions.h> //function to grab time (in terms of arduino millis)

SAMDTimer ITimer0(TIMER_TC3);
SAMD_ISR_Timer ISR_Timer;

Sensors sensors(6);
TimeFunctions timeFunctions;

#define TIMER0_INTERVAL_MS        1000      // 1s = 1000ms
#define HW_TIMER_INTERVAL_MS          50L

#define TIMER_INTERVAL_2S             1000L
#define TIMER_INTERVAL_5S             100L

void setup()
{
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);
  
  Serial2.begin(9600); 
  while (!Serial2)
  {
    Serial2.print("xbee aint starting");
  };
  Serial2.println("test");

  sensors.init();
  
  // Interval in microsecs
  /*if (ITimer0.attachInterruptInterval(TIMER0_INTERVAL_MS * 1000, TimerHandler0))
    Serial.println("Starting  ITimer0 OK, millis() = " + String(millis()));
  else
    Serial.println("Can't set ITimer0. Select another freq. or timer");*/

  if (ITimer0.attachInterruptInterval(HW_TIMER_INTERVAL_MS * 1000, TimerHandler)){
    int lastMillis = millis();
    Serial.println("Starting  ITimer OK, millis() = " + String(lastMillis));
  }
  else{
    Serial.println("Can't set ITimer correctly. Select another freq. or interval");
  }
  ISR_Timer.setInterval(TIMER_INTERVAL_2S, doingSomething2s);
  ISR_Timer.setInterval(TIMER_INTERVAL_5S, doingSomething5s);
}  

void loop() {
  // put your main code here, to run repeatedly:

}

void TimerHandler(void)
{
  ISR_Timer.run();
}

void doingSomething2s()
{
  Serial2.println("dosomething 1s");
  // Doing something here inside ISR
}
  
void doingSomething5s()
{
  Serial2.println("dosomething 100ms");
  // Doing something here inside ISR
}

void TimerHandler0(void)
{
  Serial2.println("test1");
  // Doing something here inside ISR
}
