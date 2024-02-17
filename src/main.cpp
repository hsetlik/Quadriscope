#include <Arduino.h>
#include "Quadriscope.h"

Quadriscope scope;

void setup() 
{
  scope.init();
}

void loop() 
{
  scope.tick();
}

