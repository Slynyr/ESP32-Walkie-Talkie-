#include "UpdateDisplay.h"
#include "ExternalIO.h"
#include "P2PConnect.h"
#include <vector>
#include <string>


unsigned long previousMillis = 0;

void setup() {
  Serial.begin(115200);

  //Initialize Display
  displayInitialize();

  //Get first battery voltage
  batteryIndicatorValues(pollBattery());

  //Initialize P2P
  P2PInitialize();
}

void loop() {
  //Update millis
  unsigned long currentMillis = millis();
  //displayGetMillis(currentMillis);

  //Update display
  displayUpdate();

  //Broadcast demo
  broadcast("AMONGUS");

  //Pass battery value to display every 10s
  if (currentMillis - previousMillis >= 100) {
    previousMillis = currentMillis;
    batteryIndicatorValues(pollBattery());
  }
}
