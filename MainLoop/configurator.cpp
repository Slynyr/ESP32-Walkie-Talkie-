#include "UpdateDisplay.h"
#include "Configurator.h"

float actualBatteryVoltage; 
float serialRead;
bool isConfigComplete;
bool warningPrinted= false;

// Start preferences
Preferences preferences;

bool configureBatteryOffset() {
    preferences.begin("batterySetting", false);

    // Temp erase config 
    preferences.clear();

    bool isConfigComplete = preferences.getBool("configStatus", false);
    float batteryOffset = preferences.getFloat("batteryOffset", 0);

    if (batteryOffset == 0) {
        preferences.putBool("configStatus", false);
    }

    if (actualBatteryVoltage != 0) {
        // Update values if they have changed
        isConfigComplete = true;
        preferences.putBool("configStatus", true);
        preferences.putFloat("batteryOffset", actualBatteryVoltage);

        Serial.print("Wrote, current battery offset value in memory: ");
        Serial.println(preferences.getFloat("batteryOffset", 0));

        if (preferences.getBool("configStatus", false)) {
            Serial.println("Setup Complete.");
        }
    }

    else {
        // Nothing updated, use current values
        preferences.putBool("configStatus", isConfigComplete);
        preferences.putFloat("batteryOffset", batteryOffset);
    }

    return isConfigComplete;
}

void serialManagerBatteryConfig() {
    Serial.println("[CONFIG] Warning: Input current battery voltage into the serial monitor in order to complete setup process");

    drawText(true, 64, 32, 3, "Configure Battery", "WHITE");

    if (Serial.available()) {
        serialRead = Serial.parseFloat();
    }

    if (serialRead != 0) {
        actualBatteryVoltage = serialRead;
        Serial.print("Updated battery offset to: ");
        Serial.println(actualBatteryVoltage);
        isConfigComplete = configureBatteryOffset();
    }
}

void configurator() {
    if (!isConfigComplete) {
        serialManagerBatteryConfig();
    }

    else {
        preferences.end();
    }
}
