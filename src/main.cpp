#include <Arduino.h>
#include "Quadriscope.h"

Quadriscope scope;

hw_timer_t* timer = NULL;
void IRAM_ATTR onTimer()
{
  scope.sampleInterrupt();
}

void setup() 
{
    Serial.begin(115200);
    Serial.println("Total PSRAM: " + String(ESP.getPsramSize()));
  scope.init();
    Serial.println("Free PSRAM: "+ String(ESP.getFreePsram()));
  //initialize the scope
  //set up the timer interrupt for the ADC
  // timer = timerBegin(0, 80, true);
  // timerAttachInterrupt(timer, onTimer, true);
  // timerAlarmWrite(timer, 1000000 / SAMPLE_RATE, true);
  // timerAlarmEnable(timer);


}

unsigned long lastUpdate = 0;
void loop() 
{    
  scope.tick();
}

