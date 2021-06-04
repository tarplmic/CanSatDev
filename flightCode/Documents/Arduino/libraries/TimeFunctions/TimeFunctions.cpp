#include "Arduino.h"
#include "TimeFunctions.h"

TimeFunctions::TimeFunctions(void)
{
}

String TimeFunctions::getTime()
{
    unsigned long runMillis= millis();
    unsigned long allSeconds=millis()/1000;
    int runHours= allSeconds/3600;
    int secsRemaining=allSeconds%3600;
    int runMinutes=secsRemaining/60;
    int runSeconds=secsRemaining%60;

    return String(runHours) + ":" + String(runMinutes) + ":" + String(runSeconds);
}