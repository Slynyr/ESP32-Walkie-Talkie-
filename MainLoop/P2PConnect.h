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

#define MAC_ADDRESS_STRING_SIZE 18

int countUsedSlots(char arrayIn[ESP_MAX_P2P][MAC_ADDRESS_STRING_SIZE]);

bool isSlotEmpty(char *slot);

void getP2PMillis(unsigned long masterMillis);
//Gets millis from main

bool isAddressInArray(char **arrayIn, char *strIn);
//Checks if incoming mac address is contained in incoming array

int findEmptySlot(char **arrayIn);
//Finds an empty slot in an incoming array

void clearArray(char **arrayIn);
//Clears the incoming array

void updateActiveRollingArray(char *macAddrIn);
//Updates the active and rolling arrays with the incoming mac

void compareActiveRollingArray();
//Compares the active and rolling arrays to drop macs if necessary

int countUsers();
//Counts the amount of users peered (exclusive)

void formatMacAddress(const uint8_t *macAddr, char *buffer, int maxLength);
//Formats the incoming mac address

void sentCallback(const uint8_t *macAddr, esp_now_send_status_t deliveryStatus);
//Called when data is sent

void receiveCallback(const uint8_t *macAddr, const uint8_t *data, int dataLen);
//Called when data is received

void P2PInitialize();
//Initialize the P2P functionality

void broadcast(const String &message);
//Broadcast the incoming string

#endif