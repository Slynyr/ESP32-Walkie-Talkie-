#include "UpdateDisplay.h"
#include "ExternalIO.h"
#include "P2PConnect.h"
#include "I2S-Audio.h"
#include <vector>
#include <string>

unsigned long previousBatteryMillis = 0;

void setup() {
  Serial.begin(115200);

  //Specific Init Order DO NOT CHANGE!
  //Initialize P2P
  P2PInitialize();

  //Initialize Display
  displayInitialize();

  //Get first battery voltage
  batteryIndicatorValues();
}

void loop() {
  //Update millis
  unsigned long currentMillis = millis();
  getP2PMillis(currentMillis);
  getDisplayMillis(currentMillis);

  //Update display
  displayUpdate();

  //Broadcast demo
  broadcast("AMONGUS");

  //Pass battery value to display every 10s
  if (currentMillis - previousBatteryMillis >= 10000) {
    previousBatteryMillis = currentMillis;
    batteryIndicatorValues();
  }

  compareActiveRollingArray();
}