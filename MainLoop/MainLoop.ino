#include "UpdateDisplay.h"
#include "ExternalIO.h"
#include "P2PConnect.h"
#include "I2S-Audio.h"
#include "configurator.h"

unsigned long previousBatteryMillis = 0;

void setup() {
  Serial.begin(115200);

  initializeConfigurator();
  //initializes configurator

  //Specific Init Order DO NOT CHANGE!
  //Initialize P2P
  P2PInitialize();

  //Initialize Display
  displayInitialize();

  //Get first battery voltage
  batteryIndicatorValues();

  //Initialize inputs
  inputsInitialize();

  //Initialize I2S
  i2s_init();

  //Start multicore I2S
  i2s_startTask();
}

void loop() {
  //Update millis
  unsigned long currentMillis = millis();

  getP2PMillis(currentMillis);

  getDisplayMillis(currentMillis);

  //Update display
  displayUpdate();

  //Update IO Values
  ioUpdate();

  //Broadcast demo
  //broadcast("AMONGUS");

  //Audio broadcast
  audioBroadcast();

  //Pass battery value to display every 10s
  if (currentMillis - previousBatteryMillis >= 10000) {
    previousBatteryMillis = currentMillis;
    batteryIndicatorValues();
  }

  compareActiveRollingArray();

  configurator();
}