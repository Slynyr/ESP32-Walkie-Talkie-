#include "UpdateDisplay.h"
#include "ExternalIO.h"

unsigned long previousMillis = 0;

void setup() {
  Serial.begin(115200);

  //Initialize Display
  displayInitialize();
}

void loop() {
  //Update millis
  unsigned long currentMillis = millis();

  //Update display
  displayUpdate();

  //Poll battery every 10s
  if (currentMillis - previousMillis >= 10000) {
    previousMillis = currentMillis;
    
    //Pass battery value to display
    batteryIndicatorValues(pollBattery());
  }
}
