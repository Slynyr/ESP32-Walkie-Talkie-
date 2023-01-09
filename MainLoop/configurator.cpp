#include "UpdateDisplay.h"
#include "configurator.h"
#include "ExternalIO.h"

//predefined memory address for battery offset and configurator status 
const int isConfigCompleteAddr = 10;
const int batteryOffsetAddr = 100; //tbd

bool ignoreConfig = false; //debug
bool forceConfig = false; //debug
bool isConfigComplete = false;
float actualBatteryVoltage = 0.f; 
float batteryOffset = 0.f; 
float serialRead = 0.f;

void configureBatteryOffset(){
    Serial.printf("Wrote %.2f to memory\n", actualBatteryVoltage);
    EEPROM.put(isConfigCompleteAddr, true);
    EEPROM.put(batteryOffsetAddr, calculateTolerance());
    EEPROM.commit();
    EEPROM.get(isConfigCompleteAddr, isConfigComplete); 
    EEPROM.get(batteryOffsetAddr , batteryOffset); 
    Serial.printf("Current isConfigComplete value in memory: %d\n", isConfigComplete);
    Serial.printf("Current batteryOffset value in memory: %.2f\n", batteryOffset);
}

void serialManagerBatteryConfig(){
    Serial.println("[CONFIG] Input current battery voltage into the serial monitor in order to complete setup process");
    drawText(true, 64, 32, 3, "Configure Battery", "WHITE");

    serialRead = Serial.parseFloat();
    //Serial.printf("serialRead=%.2f\n", serialRead);
    if (serialRead != 0){
      actualBatteryVoltage = serialRead;
      configureBatteryOffset();
    }
}

float calculateTolerance(){
  float readVoltageLevel = ((pollBattery() * 4.2) / 2205);
  float toleranceModifier = 0;
  if (readVoltageLevel != 0){
    toleranceModifier = (actualBatteryVoltage / readVoltageLevel);
  } else {
    Serial.println("[WARN] Battery voltage returned zero. Defaulted battery offset to 0");
  }

  //Serial.printf("ReadVoltage: %.2f ActualBatteryVoltage: %.2f BatteryOffset: %.2f", readVoltageLevel, actualBatteryVoltage, toleranceModifier); //BROKEN | ARDUINO pritf does not properly support %f. 
  Serial.printf("actualBatterVoltage: %.2f\n", actualBatteryVoltage);
  Serial.printf("toleranceModifier: %.2f\n", toleranceModifier);
  Serial.printf("readVoltageLevel: %.2f\n", readVoltageLevel);
  return toleranceModifier;
}

void initializeConfigurator(){
  EEPROM.begin(255);

  EEPROM.get(batteryOffsetAddr, batteryOffset);
  EEPROM.get(isConfigCompleteAddr, isConfigComplete);

  Serial.printf("BatteryOffset: %.2f\n", batteryOffset);
}

void configurator(){
  if (!ignoreConfig && !isConfigComplete || forceConfig){
      serialManagerBatteryConfig();
  }
}
