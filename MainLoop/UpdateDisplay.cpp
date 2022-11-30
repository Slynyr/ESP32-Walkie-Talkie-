#include "UpdateDisplay.h"
#include "ExternalIO.h"
#include "Sprites.h"
#include "P2PConnect.h"

//Globals
float voltageLevel;
unsigned short int batteryLevel;
bool connectionStatus = false;
char *state = "splash";
int userCount;
int channelCount; 
int menuPosition;

//Battery blink vars
unsigned long previousDisplayMillis = 0;
unsigned long currentDisplayMillis;
float batteryLowThreshold = 3.50;  //in volts
short int batteryBlinkTime = 1;    //in seconds
bool showBattery = true;
bool isWarnDismissed = false;
bool isActiveNotification = false;  //used to turn off elements of the screen if a notification is present

//menu options
//char* settingsOptions[] = {"Power", "placeholder", "Placeholder"};
std::vector<std::string> settingsOptions = {"colinistheimposter", "Power", "Placeholder2"};

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
void drawText(bool isCenter, int cursX, int cursY, int textSize, char* message, char* color) {
  display.setTextSize(textSize);
  if (color == "WHITE"){
    display.setTextColor(WHITE);
  } else if (color == "BLACK"){
    display.setTextColor(BLACK);
  }
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
  Serial.printf("[BATTERY VOLTAGE] %d\n", voltageLevel);
}

void batteryWarnToggle() {
  if (state != "splash") {
    if (!isWarnDismissed) {
      drawText(true, 64, 32, 2, "LOW POWER", "WHITE");
      drawText(true, 64, 50, 1, "OK to dismiss", "WHITE");
      isActiveNotification = true;

      if (rawPushButton() == HIGH) {
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
    //drawText(false, 17, 4, 1, ("~%dV", voltageLevel), "WHITE");
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

      drawText(true, posX, posY, 3.8, msgBuffer, "WHITE");
      drawText(true, posX, posY + 23, 1.5, subHeadint, "WHITE");
    } else {
      drawText(true, posX, posY, 3.5, "NO CON", "WHITE");
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

void testPushButton(){
   Serial.printf("[UPDATE]\nButton Pin: %d\nPreviouse State: %d\nCurrent State: %d\n Time Pressed: %d\n", (
     buttons[0].buttonPin,
     //buttons[0].pressStatus, Press Status: %s\n
     buttons[0].previousState,
     buttons[0].currentState,
     buttons[0].timePressed
  ));
  Serial.println(buttons[0].samePress);
  //Serial.println(buttons[0].pressStatus);

   //buttonPin, pressStatus, samepress, previousestatem currentstate
 }

void Counter(int& counterInt, int min, int max){
  if (!isActiveNotification){
    if (buttons[0].pressStatus == "SHORT" || buttons[0].pressStatus == "LONG"){
        counterInt++;
    } else if (buttons[1].pressStatus == "SHORT" || buttons[1].pressStatus == "LONG") {
        counterInt--;
    }
  }

  if (counterInt > max) {
    counterInt = 0;
  } else if (counterInt < min) {
    counterInt = 14;
  }
}/*
bool settingWidget(auto param, int min=0, int max=0){
  if (type(param) == int){

  } else if (type(param) == bool){
    
  }
}
*/


void renderMenu(std::vector<std::string>& menuArray,int sizeofArray, int position, int vertStep, int textSize){
  Counter(position, 0, menuArray.size());
  int startX = 18; 
  int index = 0; //ill change later
  int vertPosOffset = 1;
  int horPosOffset = 0;

  for (int i = 0; i < menuArray.size(); i++){
    if (position != i){
      drawText(false, 10,  (startX + (vertStep * i)), textSize, (char*) menuArray[i].c_str(), "WHITE");
    } else {
      //display.drawRect(5, (startX + (vertStep * i)), 100, 8, WHITE); //Dumpster fire
      display.fillRect(0, 0, 128, 15, WHITE); // Menu bar
      drawText(true, 64, 4, 1, "Options Menus", "BLACK");
      //Options and select box
      display.fillRect(8 + horPosOffset,  (startX + (vertStep * i)) - 1 + vertPosOffset, 110, 9, WHITE);
      display.drawRect(6 + horPosOffset, (startX + (vertStep * i)) - 3 + vertPosOffset, 114, 13, WHITE);
      drawText(false, 10 + horPosOffset,  (startX + (vertStep * i)) + vertPosOffset, textSize, (char*) menuArray[i].c_str(), "BLACK");
      Serial.printf("[WARN] Printing black text %s\n", menuArray[i].c_str());
    }
  }
}

/*
constantUIElements(char* pageHeader, char* currentMode){
  batteryIndicatorDraw(WHITE);
  backdrop();
  modeConnectionStatus(currentMode, connectionStatus, 100, 1);

  if (pageHeader){
    //drawText(true, 64, 7, 1.5, pageHeader);
    ;
  }
}
*/

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
    lowerScreenMain(channelCount, userCountP2P);
    //testPushButton();
    Counter(channelCount, 0, 14);

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
    renderMenu(settingsOptions, 3, menuPosition, 12, 1.5);
    testPushButton();
  }

  display.display();
}

