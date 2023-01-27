//This file is for ESP peer to peer communication using ESP-NOW
#include "P2PConnect.h"
#include "I2S-Audio.h"
#include "ExternalIO.h"
#include "AudioBuffer.h"
#include "Statistics.h"

#include "WiFi.h"
#include "esp_now.h"

//GLOBALS
bool bufferReceived;

const byte timeoutTime = 3; //seconds
unsigned long previousCompareMillis = 0;
unsigned long currentCompareMillis;
char activeMacAddressArray[ESP_MAX_P2P][MAC_ADDRESS_STRING_SIZE] = {0};
char rollingMacAddressArray[ESP_MAX_P2P][MAC_ADDRESS_STRING_SIZE] = {0};
int userCountP2P = 0;

static const uint8_t heartbeatMessage = 0xff;

void dumpArray(char arrayIn[ESP_MAX_P2P][MAC_ADDRESS_STRING_SIZE]){
  int numberOfSlotsUsed = countUsers();
  Serial.println("=========ARRAYDUMP=========");
  Serial.printf("Numbers of slots used %d\n", numberOfSlotsUsed);
  Serial.printf("Numbers of slots free %d\n", ESP_MAX_P2P - numberOfSlotsUsed);
  Serial.printf("Numbers of slots available %d\n", ESP_MAX_P2P);
  //Serial.println(arrayIn[0]);
  for (int i = 0; i < ESP_MAX_P2P_ARR; i++){
    //Serial.println(i);
    //Serial.println(arrayIn[i]);
    if(isSlotEmpty(arrayIn[i])){
      Serial.printf("Slot[%d]=Emtpy\n", i);    
    } else {
      Serial.printf("Slot[%d]=%s\n", i, arrayIn[i]);    
    }
  }
}

int countUsedSlots(char arrayIn[ESP_MAX_P2P][MAC_ADDRESS_STRING_SIZE]){
  int numberOfSlotsUsed = 0;
  for (int i = 0; i < ESP_MAX_P2P_ARR; i++) {
    if(!isSlotEmpty(arrayIn[i])){
      numberOfSlotsUsed++;
    }
  }
  return numberOfSlotsUsed;
}

void clearSlot(char* slot){
  memset(slot, 0, MAC_ADDRESS_STRING_SIZE);
}

bool isSlotEmpty(char *slot){
  if (slot[0] == 0){
    return true;
  }
  return false;
}

void getP2PMillis(unsigned long masterMillis) {
  currentCompareMillis = masterMillis;
}

bool isAddressInArray(char arrayIn[ESP_MAX_P2P][MAC_ADDRESS_STRING_SIZE], char *strIn) {
  bool isInArray = false;

  for (int i = 0; i < ESP_MAX_P2P_ARR; i++) {
    if (strcmp(arrayIn[i], strIn)==0) {
      isInArray = true;
      break;
    }
  }
  return isInArray;
}

int findEmptySlot(char arrayIn[ESP_MAX_P2P][MAC_ADDRESS_STRING_SIZE]) {
  //\dumpArray(arrayIn);
  for (int i = 0; i < ESP_MAX_P2P_ARR; i++) {
    if (arrayIn[i][0] == 0) {
      return i;
    }
  }
  return -1;
}

void clearArray(char arrayIn[ESP_MAX_P2P][MAC_ADDRESS_STRING_SIZE]) {
  for (int i = 0; i < ESP_MAX_P2P_ARR; i++) {
    clearSlot(arrayIn[i]);
  }
}

void updateActiveRollingArray(char *macAddrIn) {
  //Update rolling first, then active
  int rollingEmpty;
  int activeEmpty;

  if (isAddressInArray(rollingMacAddressArray, macAddrIn) == false) {
    rollingEmpty = findEmptySlot(rollingMacAddressArray);
    if (rollingEmpty != -1) {      
      strcpy(rollingMacAddressArray[rollingEmpty], macAddrIn);
    } else {
      Serial.println("ERROR: Didnt fine empty slot in rollingMacAddress array");
    }
  }

  if (isAddressInArray(activeMacAddressArray, macAddrIn) == false) {
    activeEmpty = findEmptySlot(activeMacAddressArray);
    if (activeEmpty != -1) {
      Serial.printf("Added peer (mac=%s) to active list\n", macAddrIn);
      strcpy(activeMacAddressArray[activeEmpty], macAddrIn);
    } else {
      Serial.println("ERROR: Didnt fine empty slot in activeMacAddress array");
    }
  }
}

void compareActiveRollingArray() {
  if ((currentCompareMillis - previousCompareMillis) >= (timeoutTime * 1000)) {
    //\\dumpArray(activeMacAddressArray);
    //\\dumpArray(rollingMacAddressArray);
    previousCompareMillis = currentCompareMillis;
    for (int i = 0; i < ESP_MAX_P2P_ARR; i++) {
      bool inRolling = false;
      for (int j = 0; j < ESP_MAX_P2P_ARR; j++) {
        if (strcmp(activeMacAddressArray[i], rollingMacAddressArray[j])==0) {
          inRolling = true;
          break;
        }
      }

      if (inRolling == false) {
        Serial.printf("Removed peer (mac=%s) from active list\n", activeMacAddressArray[i]);
        clearSlot(activeMacAddressArray[i]);
      }
    }
    //dumpArray(rollingMacAddressArray);
    clearArray(rollingMacAddressArray);
    userCountP2P = countUsers();
  }
}

int countUsers() {
  int count = 0;
  
  for (int i = 0; i < ESP_MAX_P2P_ARR; i++) {
    if (!isSlotEmpty(activeMacAddressArray[i])) {
      count++;
    }
  }
  return count;
}

void formatMacAddress(const uint8_t *macAddr, char *buffer, int maxLength)
//Formats MAC Address into string
{
  snprintf(buffer, maxLength, "%02x:%02x:%02x:%02x:%02x:%02x", macAddr[0], macAddr[1], macAddr[2], macAddr[3], macAddr[4], macAddr[5]);
}

void receiveCallback(const uint8_t *macAddr, const uint8_t *incomingBufferData, int bufferLength)
//Called when data is received
{
  //Format the MAC address
  // Check if the Talk button is pressed. If so, discard the data (don't write it to the Audio Buffer)
    // Write the data to the Audio Buffer
  bool isHeartbeatMessage = false;
  if (bufferLength == sizeof(heartbeatMessage))
  {
    if(memcmp(incomingBufferData, &heartbeatMessage, bufferLength)==0)
    {
      isHeartbeatMessage = true;
    }
  }
  else
  {
    audioBuffer->write((int16_t *)incomingBufferData, bufferLength / sizeof(int16_t));
    statistics.NumberOfSamplesReceived += bufferLength / sizeof(int16_t);
    statistics.BytesReceived += bufferLength;
  }
  
  
  // Do this last for the moment 
  
  char macStr[18] = { 0 };
  formatMacAddress(macAddr, macStr, 18);

   //Update known macs
  updateActiveRollingArray(macStr);

  if(isHeartbeatMessage)
  {
    Serial.printf("Received heartbeat from MAC=%s\n", macStr);
  }
 
}

// void sentCallback(const uint8_t *macAddr, esp_now_send_status_t deliveryStatus)
// //Called when data is sent
// {
//   char macStr[18];
//   formatMacAddress(macAddr, macStr, 18);
//   /*
//   Serial.print("Last Packet Sent to: ");
//   Serial.println(macStr);
//   Serial.print("Last Packet Send Status: ");
//   Serial.println(deliveryStatus == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
//   */
// }

void network_task(void *pvParameters)
{
  Serial.printf("Network task started on core %d\n", xPortGetCoreID());
  const uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

  while(true)
  {
    esp_now_send(broadcastAddress, (const uint8_t *)&heartbeatMessage, sizeof(heartbeatMessage));
    Serial.println("Heartbeat message sent");
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
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
    //esp_now_register_send_cb(sentCallback);

  } else {
    Serial.print("Initialize Failed: ");
    Serial.print(initResult);
    //Try again if initialize failed
    ESP.restart();
  }
  //Add the broadcast address
  const uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
  esp_now_peer_info_t peerInfo = {};
  memcpy(&peerInfo.peer_addr, broadcastAddress, 6);
  if (!esp_now_is_peer_exist(broadcastAddress))
  {
    esp_err_t espResult = esp_now_add_peer(&peerInfo);
    if (espResult != ESP_OK)
    {
      Serial.printf("Failed to add broadcast peer: %s\n", esp_err_to_name(espResult));
      ESP.restart();
    }
  }  
  xTaskCreate(network_task, "network_task", 2048, 0, tskIDLE_PRIORITY, nullptr);
}


// void audioBroadcast() {
//   //Broadcast to every device in range (reserved multicast)
//   uint8_t broadcastAddress[] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
//   //Setup peer info struct
//   esp_now_peer_info peerInfo = {};
//   //The address of peerInfo.peer_addr is set to the value of broadcastAddress, which has 6 bits
//   memcpy(&peerInfo.peer_addr, broadcastAddress, 6);

//   //Add every MAC as a peer, if it hasn't been done already
//   if (!esp_now_is_peer_exist(broadcastAddress)) {
//     esp_now_add_peer(&peerInfo);
//   }

//   // Only send if talk button is pressed
//   if (buttons[3].currentState == HIGH) {
//     //Send data to every connected peer (FF:FF:FF:FF:FF:FF)
//     esp_err_t sendResult = esp_now_send(broadcastAddress, (const uint8_t *)outgoingBuffer, sizeof(outgoingBuffer));
//   } 
// }