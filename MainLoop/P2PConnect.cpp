//This file is for ESP peer to peer communication using ESP-NOW
#include "P2PConnect.h"

//GLOBALS
const byte maxUsers = 16;
const byte timeoutTime = 1;
unsigned long previousCompareMillis = 0;
unsigned long currentCompareMillis;
char* activeMacAddressArray[16];
char* rollingMacAddressArray[16];
int userCountP2P;

void getP2PMillis(unsigned long masterMillis) {
  currentCompareMillis = masterMillis;
}

bool isAddressInArray(char** arrayIn, char* strIn) {
  bool isInArray = false;

  for (int i = 0; i <= sizeof(arrayIn); i++) {
    if (arrayIn[i] == strIn) {
        isInArray == true;
      }
  }
  return isInArray;
}

char findEmptySlot(char** arrayIn) {
  for (int i = 0; i <= sizeof(arrayIn); i++) {
    if (arrayIn[i] == 0) {
      return i;
    }
  }
  return -1;
}

void clearArray(char** arrayIn) {
  for (int i = 0; i <= sizeof(arrayIn); i++) {
    arrayIn[i] = NULL;
  }
}

void updateActiveRollingArray(char* macAddrIn) {
  if (!isAddressInArray(activeMacAddressArray, macAddrIn)) {
    int activeEmpty = findEmptySlot(activeMacAddressArray);
    if (activeEmpty != -1) {
      activeMacAddressArray[activeEmpty] = macAddrIn;
    }

    if (!isAddressInArray(rollingMacAddressArray, macAddrIn)) {
      int rollingEmpty = findEmptySlot(rollingMacAddressArray);
      if (rollingEmpty != -1) {
        rollingMacAddressArray[rollingEmpty] = macAddrIn;
      }
    }
  }
}

void compareActiveRollingArray() {
  bool inRolling = false;
  if ((currentCompareMillis - previousCompareMillis) <= (timeoutTime * 100)) {
    for (int i = 0; i < sizeof(activeMacAddressArray); i++) {
      for (int j = 0; j < sizeof(rollingMacAddressArray); j++) {
        if ((strncmp(activeMacAddressArray[i], rollingMacAddressArray[j], 17) == 0)) {
          inRolling = true;
        }
      }
      if (inRolling == false) {
        activeMacAddressArray[i] = NULL;
      }
    }
    userCountP2P = sizeof(activeMacAddressArray);
    previousCompareMillis = currentCompareMillis;
    clearArray(rollingMacAddressArray);
  }
}


void formatMacAddress(const uint8_t *macAddr, char *buffer, int maxLength)
//Formats MAC Address
{
  snprintf(buffer, maxLength, "%02x:%02x:%02x:%02x:%02x:%02x", macAddr[0], macAddr[1], macAddr[2], macAddr[3], macAddr[4], macAddr[5]);
}

void receiveCallback(const uint8_t *macAddr, const uint8_t *data, int dataLen)
//Rewrite so incoming data is stored in a circular buffer
//Called when data is received
{
  //Only allow a maximum of 250 characters in the message + a null terminating byte
  char buffer[ESP_NOW_MAX_DATA_LEN + 1];
  //Compare the length of max allowed data and actual length of incoming data
  int msgLen = min(ESP_NOW_MAX_DATA_LEN, dataLen);
  //Copy the incoming data to the buffer, limiting it to msgLen length
  strncpy(buffer, (const char *)data, msgLen);

  //Make sure we are null terminated (string thing) (remove)
  buffer[msgLen] = 0;

  //Format the MAC address
  char macStr[18];
  formatMacAddress(macAddr, macStr, 18);

  //Send Debug log message to the serial port
  Serial.printf("Received message from: %s - %s\n", macStr, buffer);

  //Update known macs
  updateActiveRollingArray(macStr);
}


void sentCallback(const uint8_t *macAddr, esp_now_send_status_t deliveryStatus)
//Called when data is sent
{
  char macStr[18];
  formatMacAddress(macAddr, macStr, 18);
  Serial.print("Last Packet Sent to: ");
  Serial.println(macStr);
  Serial.print("Last Packet Send Status: ");
  Serial.println(deliveryStatus == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void P2PInitialize() {
  //Start ESP in station mode
  WiFi.mode(WIFI_STA);
  Serial.print("MAC: ");
  Serial.print(WiFi.macAddress());

  //Disconnect from wifi and initialize ESP-NOW
  WiFi.disconnect();
  esp_err_t initResult = esp_now_init();
  if (initResult == ESP_OK) {
    Serial.println("ESP-NOW Initialized");

    //Define callback functions
    esp_now_register_recv_cb(receiveCallback);
    esp_now_register_send_cb(sentCallback);

  } else {
    Serial.print("Initialize Failed: ");
    Serial.print(initResult);
    //Try again if initialize failed
    ESP.restart();
  }
}

void broadcast(const String &message) {
  //Need to add a way to break down message into ESP_NOW_MAX_DATA_LEN chunks (circular buffer)
  //Broadcast to every device in range (reserved multicast)
  uint8_t broadcastAddress[] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
  //Setup peer info struct
  esp_now_peer_info peerInfo = {};
  //The address of peerInfo.peer_addr is set to the value of broadcastAddress, which has 6 bits
  memcpy(&peerInfo.peer_addr, broadcastAddress, 6);

  //Add every MAC as a peer, if it hasn't been done already
  if (!esp_now_is_peer_exist(broadcastAddress)) {
    esp_now_add_peer(&peerInfo);
  }

  //Send data to every connected peer (FF:FF:FF:FF:FF:FF)
  esp_err_t sendResult = esp_now_send(broadcastAddress, (const uint8_t *)message.c_str(), message.length());
  Serial.println(sendResult);
}