#ifndef MY_NETWORK_H
#define MY_NETWORK_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <WiFiManager.h>
#include <HTTPClient.h>
#include <Preferences.h>

#include "camera.h"

#define JWT_KEYNAME "jwt"
#define REFRESHTOKEN_KEYNAME "refresh_token"

bool setupWiFi();
bool setupWiFi(char const* manager_ssid);
bool setupWiFi(char const* manager_ssid, bool reset);

void connectToWifi(const char* ssid, const char* password);

String postJSON(const String& payload, HTTPClient& http);
int postSecureJSON(const String& payload, HTTPClient& http, const String& token);
void postSecureAutoJSON(const String& payload, const String& serverURL, String& token, const String& refreshToken);

String postJPEG(camera_fb_t* payload, HTTPClient& http);
String postSecureJPEG(camera_fb_t* payload, HTTPClient& http, const String& token);

String getJSON(HTTPClient& http);
String getSecureJSON(HTTPClient& http, const String& token);

void saveToken(const char* keyName, const String& token);
String loadToken(const char* keyName);
String parseJWT(String& response);

#endif