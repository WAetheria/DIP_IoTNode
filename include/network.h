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
String postSecureJSON(const String& payload, HTTPClient& http, const String& token);
String postSecureAutoJSON(const String& payload, const String& serverURL, const String& token);

String postJPEG(camera_fb_t* payload, HTTPClient& http);
String postSecureJPEG(camera_fb_t* payload, HTTPClient& http, const String& token);

String getJSON(HTTPClient& http);
String getSecureJSON(HTTPClient& http, const String& token);

void saveString(int address, const String& str);
String loadString(int address);

#endif