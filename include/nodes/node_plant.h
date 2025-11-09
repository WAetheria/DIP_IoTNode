#ifndef PLANT_NODE
#define PLANT_NODE

#include <Arduino.h>
#include <ArduinoJson.h>

#include "network.h"
#include "device.h"
#include "clock.h"
#include "camera.h"

#include "env.h"     // NOTE: CREATE + CONFIGURE BEFORE USE

#define WATER_SENSOR    36 
#define WATER_PUMP      26

#define WATER_THRESHOLD 1000

Device water = Device(WATER_SENSOR, DeviceMode::ANALOG_INPUT);
Device pump  = Device(WATER_PUMP  , DeviceMode::DIGITAL_OUTPUT);

const char* serverURL = PLANT_ENDPOINT_URL;

void setup(){
    Serial.begin(115200);
    setupWiFi();
}

void loop(){
    // Sensor Handling
    int waterLevel = water.readInput();

    (waterLevel <= WATER_THRESHOLD) ? pump.turnOn() : pump.turnOff();
	
	Serial.printf("Water Level: %d\n", waterLevel);

    // JSON Handling
	JsonDocument doc;
	
	doc["Water Level"] = waterLevel;

	char payload[256];
	serializeJson(doc, payload);

	// HTTP Handling
    String response;
    static HTTPClient http;

    if (!http.connected()){
		http.begin(serverURL);
		http.setReuse(true);
	}
    
    postJSON(payload, http);

	#if DEBUG == true
	    delay(30000); // An added delay so the network doesn't get overloaded
	#endif
}

#endif