#include <EEPROM.h>
#define EEPROM_SIZE 512 

//Alternatively you can use the website listed below
//https://adafruit.github.io/Adafruit_WebSerial_ESPTool/

void setup() {
  Serial.begin(115200);

  //Wiping EEPROM
  Serial.println("Wiping EEPROM");
  EEPROM.begin(EEPROM_SIZE);
  for (int i = 0; i < EEPROM_SIZE; i++) {
  EEPROM.write(i, 0);
  }
  EEPROM.commit();
  delay(500);
  Serial.println("Wiping complete");

  //Veryfing wipe
  Serial.println("Verifying");
  for (int i = 0; i < EEPROM_SIZE; i++) {
    int value;
    EEPROM.get(i, value);
    if(value != 0){
      Serial.printf("[WARN] Memory address %d has not been reset (%d)\n", (i, value));
    }
  }
  Serial.println("Verifying complete");
}

void loop() {

}
