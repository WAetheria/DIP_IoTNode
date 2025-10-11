#ifndef MY_NETWORK_H
#define MY_NETWORK_H

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiManager.h>
#include <HTTPClient.h>

extern const char* serverURL;

bool setupWiFi(char const* manager_ssid);
bool setupWiFi(char const* manager_ssid, bool reset);

void connectToWifi(const char* ssid, const char* password);

void postJSON(const String& payload, HTTPClient& http);
String getJSON(HTTPClient& http);

#endif