#ifndef MY_NETWORK_H
#define MY_NETWORK_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <WiFiManager.h>
#include <HTTPClient.h>
#include <Preferences.h>

#include "camera.h"
#include "env.h"

#define JWT_KEYNAME "jwt"
#define REFRESHTOKEN_KEYNAME "refresh_token"

bool setupWiFi();
bool setupWiFi(char const* manager_ssid);
bool setupWiFi(char const* manager_ssid, bool reset);

void connectToWifi(const char* ssid, const char* password);

String postJSON(const String& payload, HTTPClient& http);
int postSecureJSON(const String& payload, HTTPClient& http, const String& token);

String postJPEG(camera_fb_t* payload, HTTPClient& http);
String postSecureJPEG(camera_fb_t* payload, HTTPClient& http, const String& token);

String getJSON(HTTPClient& http);
String getSecureJSON(HTTPClient& http, const String& token);

// Returns a true when JWT is refreshed
bool postSecureAutoJSON(const String& payload, const String& serverURL, String& token, const String& refreshToken);
bool postSecureAutoJPEG(camera_fb_t* payload, const String& serverURL, String& token, const String& refreshToken);
bool getSecureAutoJSON(const String& serverURL, String& token, const String& refreshToken);

void saveToken(const char* keyName, const String& token);
String loadToken(const char* keyName);
String parseJWT(String& response);
String getNewJWT(const String& refreshToken);

#endif