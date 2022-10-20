#include "UpdateDisplay.h"
#include "ExternalIO.h"
#include "P2PConnect.h"
#include "I2S-Audio.h"
#include <vector>
#include <string>

unsigned long previousBatteryMillis = 0;

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
  getP2PMillis(currentMillis);
  getDisplayMillis(currentMillis);

  //Update display
  displayUpdate();

  //Broadcast demo
  broadcast("AMONGUS");

  //Pass battery value to display every 10s
  if (currentMillis - previousBatteryMillis >= 100) {
    previousBatteryMillis = currentMillis;
    batteryIndicatorValues(pollBattery());
  }
}
