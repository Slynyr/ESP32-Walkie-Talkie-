#ifndef _P2P_CONNECT_H_
#define _P2P_CONNECT_H_
#include <Arduino.h>

#include "WiFi.h"
#include "esp_now.h"

#include <vector>
#include <string>
#include <iterator>

//External global
extern int userCountP2P;

void getP2PMillis(unsigned long masterMillis);
//Get millis from main for P2P files

bool isAddressInList(std::vector<std::string> listIn, std::string macAddrIn);
//Checks if macAddrIn is in the list listIn

void updateActiveList(std::string macAddrIn);
//Updates the current MACs connected

void clearList(std::vector<std::string> listIn);
//Clear the list listIn

void compareActiveRollingLists();
//Iterates through each active list element and checks if it is present in rolling list. If not present, it is popped.

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