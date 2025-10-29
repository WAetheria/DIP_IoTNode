#ifndef MY_NETWORK_H
#define MY_NETWORK_H

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiManager.h>
#include <HTTPClient.h>
#include <EEPROM.h>

#include "camera.h"

#define REFRESH_TOKEN_SIZE 32

bool setupWiFi();
bool setupWiFi(char const* manager_ssid);
bool setupWiFi(char const* manager_ssid, bool reset);

void connectToWifi(const char* ssid, const char* password);

String postJSON(const String& payload, HTTPClient& http);
int postSecureJSON(const String& payload, HTTPClient& http, String token);

String postAutoJSON(const String& payload, const String serverURL);

int postJPEG(camera_fb_t* payload, HTTPClient& http);
int postSecureJPEG(camera_fb_t* payload, HTTPClient& http, String token);

String getJSON(HTTPClient& http);
String getSecureJSON(HTTPClient& http, String token);

void writeStringToEEPROM(int addrOffset, const String &str);
String readStringFromEEPROM(int addrOffset);

#endif