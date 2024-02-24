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
  //initialize the scope
  scope.init();
  //set up the timer interrupt for the ADC
  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, onTimer, true);
  timerAlarmWrite(timer, 1000000 / SAMPLE_RATE, true);
  timerAlarmEnable(timer);


}

void loop() 
{
  scope.tick();
}

