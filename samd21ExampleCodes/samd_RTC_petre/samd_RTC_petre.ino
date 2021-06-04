#include <Wire.h>
#include <ds3231.h>
#include <SerialDefinitions.h>

#define DS3231_CONTROL_INTCN    0x4
 
struct ts t; 
 
void setup() {
  Serial2.begin(9600);
  Wire.begin();
  DS3231_init(DS3231_CONTROL_INTCN);
  /*----------------------------------------------------------------------------
  In order to synchronise your clock module, insert timetable values below !
  ----------------------------------------------------------------------------*/
  t.hour = 6; 
  t.min = 59;
  t.sec = 0;
  t.mday = 3;
  t.mon = 6;
  t.year = 2021;
 
  //DS3231_set(t); 
}
 
void loop() {
  DS3231_get(&t);
  Serial2.print("Date : ");
  Serial2.print(t.mday);
  Serial2.print("/");
  Serial2.print(t.mon);
  Serial2.print("/");
  Serial2.print(t.year);
  Serial2.print("\t Hour : ");
  Serial2.print(t.hour);
  Serial2.print(":");
  Serial2.print(t.min);
  Serial2.print(".");
  Serial2.println(t.sec);
 
  delay(1000);
}
