#ifndef PLANT_NODE
#define PLANT_NODE

#include <Arduino.h>
#include <ArduinoJson.h>

#include "network.h"
#include "device.h"
#include "clock.h"
#include "camera.h"

#include "env.h"     // NOTE: CREATE + CONFIGURE BEFORE USE

#define WATER_SENSOR    16 
#define WATER_PUMP      26 

#define WATER_THRESHOLD 0
#define CAMERA_TIMEOUT  300000

Device water = Device(WATER_SENSOR, DeviceMode::ANALOG_INPUT);
Device pump  = Device(WATER_PUMP  , DeviceMode::DIGITAL_OUTPUT);
Timer cameraTimer = Timer(CAMERA_TIMEOUT);

const char* serverURL = PLANT_ENDPOINT_URL;

void setup(){
    Serial.begin(115200);
    setupWiFi();
}

void loop(){
    // Sensor Handling
    int waterLevel = water.readInput();

    (waterLevel <= WATER_THRESHOLD) ? pump.turnOn() : pump.turnOff();
	#if DEBUG == true
		Serial.print("Water Level: ");
		Serial.println(waterLevel);
	#endif

	// JPEG Handling
	camera_fb_t frameBuffer;

	if (cameraTimer.timedOut()){
		camera_capture(&frameBuffer);
		cameraTimer.resetTimer();
	}

    // JSON Handling
	JsonDocument doc;
	
	doc["Water Level"] = waterLevel;

	char payload[256];
	serializeJson(doc, payload);

	// HTTP Handling
    static HTTPClient http;

	postJSON(payload, http);
	postJPEG(&frameBuffer, http);


	#if DEBUG == true
	    delay(30000); // An added delay so the network doesn't get overloaded
	#endif
}

#endif