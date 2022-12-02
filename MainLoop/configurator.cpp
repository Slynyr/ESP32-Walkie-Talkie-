/*
#include "UpdateDisplay.h"
#include "configurator.h"

//predefined memory address for battery offset and configurator status 
int isConfigCompleteAddr = 1000;
int batteryOffsetAddr = 1001; //tbd


bool ignoreConfig = false; //debug
bool isConfigComplete = EEPROM.read(isConfigCompleteAddr); 
int actualBatteryVoltage; 
int batteryOffset = EEPROM.read(batteryOffsetAddr); 

void configureBatteryOffset(){
    drawText(true, 64, 32, 3, "Configure Battery");
    serialManagerBatteryConfig();
    if (actualBatteryVoltage){
        EEPROM.write(isConfigCompleteAddr, true);
        EEPROM.write(batteryOffsetAddr, actualBatteryVoltage);
    }
}

void serialManagerBatteryConfig(){
    Serial.println("[CONFIG] Input current battery voltage into the serial monitor in order to complete setup process");
    if (Serial.readInt()){
        actualBatteryVoltage = Serial.readInt();
    }
}

void configurator(){
    if (!ignoreConfig || !isConfigComplete){
        configureBatteryOffset();
    }
}
*/