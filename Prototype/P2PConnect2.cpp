//This file is for ESP peer to peer communication using ESP-NOW
#include "P2PConnect2.h"

//GLOBALS

#define INVALID_PEER_SLOT -1

struct Peer
{
  bool SlotUsed;
  char MacAddress[MAC_ADDRESS_STRING_SIZE];
  unsigned long lastUpdateTimestamp;
};

void clearPeerSlot(Peer* peer);
bool isPeerSlotUsed(Peer* peer);
int findPeerSlot(char* macAddress);
int findEmptyPeerSlot();
void resetPeerTable();
int addPeer(char* macAddress);
void removePeer(char* macAddress);
int countPeers();

const byte timeoutTime2 = 1;
unsigned long previousCompareMillis2 = 0;
unsigned long currentCompareMillis2;
int userCountP2P2 = 0;

#define MAXIMUM_NUMBER_OF_PEERS ESP_MAX_P2P
Peer peerTable[ESP_MAX_P2P] = {0};

void dumpPeerTable(){
  // Dumps the peer table 
  int numberOfPeers = countPeers();
  Serial.println("=========ARRAYDUMP=========");
  Serial.printf("Numbers of slots used %d\n", numberOfPeers);
  Serial.printf("Numbers of slots free %d\n", MAXIMUM_NUMBER_OF_PEERS - numberOfPeers);
  Serial.printf("Numbers of slots available %d\n", MAXIMUM_NUMBER_OF_PEERS);
  //Serial.println(arrayIn[0]);
  for (int i = 0; i < MAXIMUM_NUMBER_OF_PEERS; i++){
    //Serial.println(i);
    //Serial.println(arrayIn[i]);
    if(!peerTable[i].SlotUsed){
      Serial.printf("Slot[%d]=Emtpy\n", i);    
    }else{
      Serial.printf("Slot[%d]= Mac=%s Timestamp=%d\n", i, peerTable[i].MacAddress, peerTable[i].lastUpdateTimestamp);    
    }
  }
}

int countPeers(){
  int numberOfPeers = 0;
  for (int i = 0; i < MAXIMUM_NUMBER_OF_PEERS; i++) {
    if(peerTable[i].SlotUsed){
      numberOfPeers++;
    }
  }
  return numberOfPeers;
}

void clearPeerSlot(Peer* peer){
  memset(peer, 0, sizeof(Peer));
}

bool isPeerSlotUsed(Peer* peer){
  return peer->SlotUsed;
}

void getP2PMillis2(unsigned long masterMillis) {
  currentCompareMillis2 = masterMillis;
}

int findPeerSlot(char* macAddress){
  int slotNumber = INVALID_PEER_SLOT;
  for(int i=0; i < MAXIMUM_NUMBER_OF_PEERS; i++){
    if(peerTable[i].SlotUsed && strcmp(peerTable[i].MacAddress, macAddress)==0){
      slotNumber = i;
      break;
    }
  }
  return slotNumber;
}

int findEmptyPeerSlot() {
  int emptySlot = INVALID_PEER_SLOT;
  for (int i = 0; i < MAXIMUM_NUMBER_OF_PEERS; i++) {
    if (!peerTable[i].SlotUsed){
      emptySlot = i;
      break;
    }
  }
  return emptySlot;
}

void resetPeerTable() {
  memset(peerTable, 0, sizeof(peerTable));
}

int addPeer(char* macAddress){
  int peerSlot = findPeerSlot(macAddress);
  if(peerSlot == INVALID_PEER_SLOT){
    peerSlot = findEmptyPeerSlot();
    if(peerSlot != -1){
      peerTable[peerSlot].SlotUsed = true;
      strcpy(peerTable[peerSlot].MacAddress, macAddress);
      peerTable[peerSlot].lastUpdateTimestamp = currentCompareMillis2;
    } else {
      Serial.printf("[ERROR] Failed to find emtpy slot\n");
    }
  } else {
      Serial.printf("[WARNING] Peer with mac=%s already in list at slot %d\n", macAddress, peerSlot);
  }
  return peerSlot;
}

void removePeer(char* macAddress){
  int peerSlot = findPeerSlot(macAddress);
  if(peerSlot != INVALID_PEER_SLOT){
      Serial.printf("[INFO] Peer with mac=%s removed from slot %d\n", macAddress, peerSlot);
      memset(&peerTable[peerSlot], 0, sizeof(Peer));    
  } else {
      Serial.printf("[INFO] Peer with mac=%s not found in peer list\n", macAddress);
  }
}

void formatMacAddress2(const uint8_t *macAddr, char *buffer, int maxLength)
//Formats MAC Address
{
  snprintf(buffer, maxLength, "%02x:%02x:%02x:%02x:%02x:%02x", macAddr[0], macAddr[1], macAddr[2], macAddr[3], macAddr[4], macAddr[5]);
}


void P2PInitialize2() {
  //Start ESP in station mode
  WiFi.mode(WIFI_STA);
  Serial.printf("MAC: %s\n", WiFi.macAddress());

  //Disconnect from wifi and initialize ESP-NOW
  WiFi.disconnect();
  esp_err_t initResult = esp_now_init();
  if (initResult == ESP_OK) {
    Serial.println("[INFO] ESP-NOW Initialized");

    //Define callback functions
    esp_now_register_recv_cb(receiveCallback2);
    esp_now_register_send_cb(sentCallback2);

  } else {
    Serial.printf("[ERROR] Initialize Failed: %d\n", initResult);
    //Try again if initialize failed
    ESP.restart();
  }
}

void updateP2P(){
   if ((currentCompareMillis2 - previousCompareMillis2) >= (timeoutTime2 * 1000)) {
    for(int i = 0; i< MAXIMUM_NUMBER_OF_PEERS; i++){
      if(peerTable->SlotUsed){
        if((currentCompareMillis2 - peerTable->lastUpdateTimestamp) >= (timeoutTime2 * 1000)){
          removePeer(peerTable[i].MacAddress);
        }
      }
    }
    userCountP2P2 = countPeers();   
   }
}
