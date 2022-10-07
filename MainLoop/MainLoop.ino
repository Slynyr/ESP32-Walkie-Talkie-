#include "UpdateDisplay.h"

void setup() {
  Serial.begin(115200);

  //Initialize Display
  displayInitialize();
}

void loop() {
  displayUpdate();
}
