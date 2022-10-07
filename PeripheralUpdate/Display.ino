#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

#define OLED_WIDTH 128
#define OLED_HEIGHT 64

#define OLED_ADDR 0x3C

Adafruit_SSD1306 display(OLED_WIDTH, OLED_HEIGHT);

// 'icons8-empty-battery-64', 13x13px
const unsigned char batteryBitmap [] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xe0, 0x40, 0x20, 0x40, 0x30, 0x40, 0x30, 0x40, 0x30, 
	0x40, 0x20, 0x7f, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  display.clearDisplay();
  display.display();
}
void loop() {
  // put your main code here, to run repeatedly:
  display.setTextSize(10);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);

  //Draw images
  emptyShell();
  batteryIndicator();

  //Update display
  display.display();
}

void emptyShell() {
  //Frame of screen boundaries
  display.drawRect(0, 0, 128, 15, WHITE);
  display.drawRect(0, 16, 128, 48, WHITE);
}

void batteryIndicator() {
  display.drawBitmap(2, 1, batteryBitmap, 13, 13, WHITE);
}