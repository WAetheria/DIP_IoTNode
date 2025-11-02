#ifndef TEST_NODE
#define TEST_NODE

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

#endif