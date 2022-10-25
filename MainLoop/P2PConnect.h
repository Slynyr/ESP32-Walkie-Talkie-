#ifndef _P2P_CONNECT_H_
#define _P2P_CONNECT_H_
#define ESP_MAX_P2P 20
// 20 - 1 user for array usage
#define ESP_MAX_P2P_ARR 19
#include <Arduino.h>

#include "WiFi.h"
#include "esp_now.h"

//External global
extern int userCountP2P;

void getP2PMillis(unsigned long masterMillis);
//Get millis from main for P2P files

bool isAddressInArray(char** arrayIn, char* strIn);
//Checks if macAddrIn is in the list listIn

char findEmptySlot(char** arrayIn);
//Find empty array slot

void updateActiveRollingArray(char* macAddrIn);
//Updates the current MACs connected

void clearArray(char** arrayIn);
//Clear the list arrayIn

void compareActiveRollingArray();
//Iterates through each active list element and checks if it is present in rolling list. If not present, it is popped.

int countUsers();
//Count the amount of users connected

void formatMacAddress(const uint8_t *macAddr, char *buffer, int maxLength);
//Formats MAC Address from int to char array

void receiveCallback(const uint8_t *macAddr, const uint8_t *data, int dataLen);
//Called by the API every time data is received

void sentCallback(const uint8_t *macAddr, esp_now_send_status_t deliveryStatus);
//Called by the API every time data is sent

void P2PInitialize();
//Initialize variables and library

void broadcast(const String &message);
//Broadcast message

#endif