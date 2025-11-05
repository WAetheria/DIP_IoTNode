#ifndef TEST_NODE
#define TEST_NODE

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

#include "camera.h"
#include "device.h"
#include "network.h"

camera_fb_t fb;

const char* serverURL = "http://10.139.217.226:8000/upload";

void setup(){
	Serial.begin(115200);
	setupWiFi();

	camera_init();

	if (WiFi.status() == WL_CONNECTED) {
		HTTPClient http;
		http.begin(serverURL
		);
		http.addHeader("Content-Type", "image/jpeg");

		// Capture a frame
		camera_fb_t *fb = esp_camera_fb_get();
		if (!fb) {
		Serial.println("Camera capture failed");
		delay(2000);
		return;
		}

		// Send the JPEG buffer
		int httpCode = http.POST(fb->buf, fb->len);
		Serial.printf("Upload result: %d\n", httpCode);
		Serial.println(http.errorToString(httpCode));

		http.end();
		esp_camera_fb_return(fb);
	} else {
		Serial.println("WiFi not connected");
	}
}

void loop(){
	
}

#endif