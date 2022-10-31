#include "UpdateDisplay.h"
#include "ExternalIO.h"
#include "Sprites.h"
#include "P2PConnect.h"

//Globals
float voltageLevel;
unsigned short int batteryLevel;
bool connectionStatus = false;
char *state = "main";
char *previousState = "main";
int userCount;

//Battery blink vars
unsigned long previousDisplayMillis = 0;
unsigned long currentDisplayMillis;
float batteryLowThreshold = 3.60;  //in volts
short int batteryBlinkTime = 1;    //in seconds
bool showBattery = true;
bool isWarnDismissed = false;
bool isActiveNotification = false;  //used to turn off elements of the screen if a notification is present

Adafruit_SSD1306 display(OLED_WIDTH, OLED_HEIGHT);

void displayInitialize() {
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  display.clearDisplay();
  display.display();
}

void getDisplayMillis(unsigned long masterMillis) {
  currentDisplayMillis = masterMillis;
}

//------------DRAW-TEXT
void drawText(bool isCenter, int cursX, int cursY, int textSize, char *message) {
  display.setTextSize(textSize);
  display.setTextColor(WHITE);
  display.setTextWrap(false);
  if (isCenter) {
    int16_t x1, y1;
    uint16_t w, h;
    display.getTextBounds(message, cursX, cursY, &x1, &y1, &w, &h);  //calc width of new string
    display.setCursor(cursX - w / 2, cursY);
    display.print(message);
  } else {
    display.setCursor(cursX, cursY);
    display.print(message);
  }
  display.setTextWrap(true);
}

void drawCenteredCircle(bool isFilled, int cursX, int cursY, int size) {
  if (isFilled) {
    display.fillCircle(cursX - (size / 2), cursY - (size / 2), size, WHITE);
  } else {
    display.drawCircle(cursX - (size / 2), cursY - (size / 2), size, WHITE);
  }
}

//--------------DRAW-TYPES
void backdrop(const int mode) {
  //Frame of screen boundaries
  if (mode == 1) {
    display.drawRect(0, 0, 128, 15, WHITE);
    display.drawRect(0, 16, 128, 48, WHITE);
  } else if (mode == 2) {
    display.drawLine(0, 15, 128, 15, WHITE);
  } else if (mode == 3) {
    display.drawLine(0, 15, 128, 15, WHITE);
    display.drawLine(0, 16, 128, 16, WHITE);
  }
}

void batteryIndicatorValues() {
  //Voltage and level math
  //For overall best accuracy, use the linear region of the ADC (2.00V to 0.3V)
  //1850-> ~3.5V, 2250-> ~4.2V Using R1 2K2 R2 2K
  //School PSU
  //1985-> ~3.5V, 2400-> ~4.2V Using R1 2K2 R2 1K+1K
  batteryLevel = map(pollBattery(), 1985, 2400, 0, 8);
  voltageLevel = ((pollBattery() * 4.2) / 2400);
}

void batteryWarnToggle() {
  if (state != "splash") {
    if (!isWarnDismissed) {
      drawText(true, 64, 32, 2, "LOW POWER");
      drawText(true, 64, 50, 1, "OK to dismiss");
      isActiveNotification = true;

      if (debugPushbutton() == HIGH) {
        isWarnDismissed = true;
      }
    }
  }
}

void batteryIndicatorDraw(const int colour) {

  if (voltageLevel <= batteryLowThreshold) {
    batteryWarnToggle();
    if ((currentDisplayMillis - previousDisplayMillis) >= (batteryBlinkTime * 1000)) {
      showBattery = !showBattery;
      previousDisplayMillis = currentDisplayMillis;
    }
  } else {
    showBattery = true;
    isWarnDismissed = false;  //resets warn once voltage goes above low level
  }

  if (showBattery) {
    //Battery internal size is 8x4
    display.drawBitmap(2, 1, batteryBitmap, 13, 13, colour);

    //Draw rectangle based on battery level
    display.fillRect(4, 4, batteryLevel, 6, colour);

    //Draw voltage text
    display.setTextSize(1);
    display.setTextColor(colour);
    display.setCursor(17, 4);
    display.print("~");
    display.print(voltageLevel);
    display.print("V");
  }
}

//--------Mode Connection Status Icon
void modeConnectionStatus(char *mode, bool isConnected, int posX, int posY) {
  if (mode == "NODE") {
    if (isConnected) {
      display.drawBitmap(posX, posY, nodeConnectionEstablished, 13, 13, WHITE);
    } else {
      display.drawBitmap(posX, posY, noNodeConnectionmap, 13, 13, WHITE);
    }
  } else if (mode == "LOCAL") {
    ;
    //render local mode sprites
  }
}


//--------Lower Screen Menus
void drawIntWithSubheading(bool isConnected, int posX, int posY, int value, char *subHeadint) {
  if (!isActiveNotification) {  //only print if the displayt is clear of notifications
    if (isConnected) {
      char msgBuffer[32] = { 0 };
      sprintf(msgBuffer, "%d", value);

      drawText(true, posX, posY, 3.8, msgBuffer);
      drawText(true, posX, posY + 23, 1.5, subHeadint);
    } else {
      drawText(true, posX, posY, 3.5, "NO CON");
    }
  }
}

void lowerScreenMain(int channelCountValue, int userCountValue) {
  drawIntWithSubheading(true, 26, 25, channelCountValue, "Channel");  //Channe counter
  drawIntWithSubheading(true, 102, 25, userCountValue, "Users");      //User counter
}

//-----------Dynamic Animations
void nodeAnimation(int clientsConnected, bool isMaster) {
  //Not important just ignore this
  if (clientsConnected >= 2) {
    //center
    display.drawCircle(20, 20, 10, WHITE);
  }
}

void testbuttons(){
  if (upButton() == "LP"){
    drawCenteredCircle(true, 64, 32, 5);
  } else if (upButton() == "SP"){
    Serial.print("testcase passed"); //doesnt make sense 
    drawCenteredCircle(true, 64, 32, 5);
  }
}

//-------------UPDATE
//Update Display
void displayUpdate() {
  display.clearDisplay();
  isActiveNotification = false;

  if (state == "main") {
    //Draw images
    batteryIndicatorDraw(WHITE);
    backdrop(2);
    modeConnectionStatus("NODE", connectionStatus, 100, 1);
    lowerScreenMain(24, userCountP2P);
    testbuttons();

    //Update display
  } else if (state == "splash") {
    delay(500);
    //Draw logo
    display.drawBitmap(0, 16, splashNodeLogo, 128, 48, WHITE);
    display.display();
    delay(1500);
    display.clearDisplay();
    delay(1000);
    display.drawBitmap(0, 16, splashNodeFont, 128, 48, WHITE);
    display.display();
    delay(750);

    //Loading bar
    for (int i = 0; i < 128; i++) {
      delay(random(30, 60));
      display.drawRect(0, 6, i, 6, WHITE);
      display.display();
    }

    delay(1000);
    state = "main";

  } else if (state == "menu") {
    ;  //menu selection screen here
  }

  display.display();
}