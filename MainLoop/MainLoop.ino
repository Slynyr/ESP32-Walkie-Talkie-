#include "UpdateDisplay.h"
#include "ExternalIO.h"
#include "P2PConnect.h"

unsigned long previousMillis = 0;

void setup() {
  Serial.begin(115200);

  //Initialize Display
  displayInitialize();

  //Get first battery voltage
  batteryIndicatorValues(pollBattery());
}

void loop() {
  //Update millis
  unsigned long currentMillis = millis();
  //displayGetMillis(currentMillis);

  //Update display
  displayUpdate();

  //Pass battery value to display every 10s
  if (currentMillis - previousMillis >= 10000) {
    previousMillis = currentMillis;
    batteryIndicatorValues(pollBattery());
  }
}
