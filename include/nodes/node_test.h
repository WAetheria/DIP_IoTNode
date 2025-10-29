#ifndef TEST_NODE
#define TEST_NODE


/*
#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

#include "camera.h"
#include "device.h"

const char* ssid = "ssid";
const char* password = "password";

WebServer server(80);

void handleCapture() {
  camera_fb_t *fb = esp_camera_fb_get();
  if (!fb) {
    server.send(500, "text/plain", "Camera capture failed");
    return;
  }

  server.sendHeader("Content-Type", "image/jpeg");
  server.send_P(200, "image/jpeg", (const char *)fb->buf, fb->len);
  esp_camera_fb_return(fb);
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500); Serial.print(".");
  }
  Serial.println(WiFi.localIP());

  server.on("/capture", handleCapture);
  server.begin();
}

void loop() {
  server.handleClient();
}
*/

#include <Arduino.h>
#include <ArduinoJson.h>
#include <EEPROM.h>

#include "network.h"
#include "env.h"

#define TOKEN_SIZE    64
#define TOKEN_ADDRESS 0

void setup(){
	Serial.begin(115200);
	EEPROM.begin(TOKEN_SIZE);
// 	setupWiFi();

// 	JsonDocument setupDoc;
// 	const String id = LIVROOM_ID;
// 	const String deviceName = LIVROOM_NAME;

// 	setupDoc["device_id"] = id;
// 	setupDoc["name"] = deviceName;

// 	char payload[256];
// 	serializeJson(setupDoc, payload);

// 	HTTPClient http;
// 	http.begin("https://piot-183946185085.asia-southeast1.run.app/device");
// 	String response = postJSON(payload, http);

// 	JsonDocument doc;
// 	deserializeJson(doc, response);

// 	String jwt = doc["jwt"];
// 	String refreshToken = doc["refresh_token"];

// 	Serial.println(jwt);
// 	Serial.println(refreshToken);

// 	writeStringToEEPROM(TOKEN_ADDRESS, refreshToken);
// 	EEPROM.commit();
}

void loop(){
	delay(1000);
}

#endif