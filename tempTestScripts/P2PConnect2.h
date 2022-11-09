#ifndef _P2P_CONNECT_H_
#define _P2P_CONNECT_H_
#define ESP_MAX_P2P 20
// 20 - 1 user for array usage
#define ESP_MAX_P2P_ARR 19
#include <Arduino.h>

#include "WiFi.h"
#include "esp_now.h"

//External global
extern int userCountP2P2;

#define MAC_ADDRESS_STRING_SIZE 18


void getP2PMillis2(unsigned long masterMillis);
//Gets millis from main


void sentCallback2(const uint8_t *macAddr, esp_now_send_status_t deliveryStatus);
//Called when data is sent

void receiveCallback2(const uint8_t *macAddr, const uint8_t *data, int dataLen);
//Called when data is received

void P2PInitialize2();
//Initialize the P2P functionality

void broadcast2(const String &message);
//Broadcast the incoming string

void updateP2P();


#endif