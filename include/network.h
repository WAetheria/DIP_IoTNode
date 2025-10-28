#ifndef MY_NETWORK_H
#define MY_NETWORK_H

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiManager.h>
#include <HTTPClient.h>

#include "camera.h"

bool setupWiFi();
bool setupWiFi(char const* manager_ssid);
bool setupWiFi(char const* manager_ssid, bool reset);

void connectToWifi(const char* ssid, const char* password);

int postJSON(const String& payload, HTTPClient& http); 
int postAutoJSON(const String& payload, const String serverURL);

// Note: Untested
int postJPEG(camera_fb_t* payload, HTTPClient& http);

String getJSON(HTTPClient& http);

#endif