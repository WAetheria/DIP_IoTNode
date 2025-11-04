#ifndef TEST_NODE
#define TEST_NODE

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

#include "camera.h"
#include "device.h"
#include "network.h"

camera_fb_t fb;

const char* serverURL = "example.com";

void setup(){
	Serial.begin(115200);
	setupWiFi();

	camera_init();

	camera_capture(&fb);

	HTTPClient http;
	postJPEG(&fb, http);
}

void loop(){
	
}

#endif