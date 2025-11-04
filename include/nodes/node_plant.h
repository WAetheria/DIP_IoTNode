#ifndef PLANT_NODE
#define PLANT_NODE

#include <Arduino.h>
#include <ArduinoJson.h>

#include "network.h"
#include "device.h"
#include "clock.h"
#include "camera.h"

#include "env.h"     // NOTE: CREATE + CONFIGURE BEFORE USE

#define WATER_SENSOR    -1 
#define WATER_PUMP      -1

#define WATER_THRESHOLD 1000
#define CAMERA_TIMEOUT  300000

Device water = Device(WATER_SENSOR, DeviceMode::ANALOG_INPUT);
Device pump  = Device(WATER_PUMP  , DeviceMode::DIGITAL_OUTPUT);
Timer cameraTimer = Timer(CAMERA_TIMEOUT);

const char* serverURL = PLANT_ENDPOINT_URL;

String jwt;
String refreshToken;

void setup(){
    Serial.begin(115200);
    setupWiFi();

    jwt = loadToken(JWT_KEYNAME);
    refreshToken = loadToken(REFRESHTOKEN_KEYNAME);

    Serial.print("JWT: ");
    Serial.println(jwt);
    Serial.print("Refresh Token: ");
    Serial.println(refreshToken);

	camera_init();
}

void loop(){
    // Sensor Handling
    int waterLevel = water.readInput();

    (waterLevel <= WATER_THRESHOLD) ? pump.turnOn() : pump.turnOff();
	
	Serial.printf("Water Level: %d\n", waterLevel);

	// JPEG Handling
	camera_fb_t frameBuffer;
	bool shouldSendJPEG = false;

	if (cameraTimer.timedOut()){
		camera_capture(&frameBuffer);
		cameraTimer.resetTimer();
		shouldSendJPEG = true;
	}

    // JSON Handling
	JsonDocument doc;
	
	doc["Water Level"] = waterLevel;

	char payload[256];
	serializeJson(doc, payload);

	// HTTP Handling
    if(postSecureAutoJSON(payload, serverURL, jwt, refreshToken)){
        saveToken(JWT_KEYNAME, jwt);
    }

	if (shouldSendJPEG){
		HTTPClient httpJPEG;
		httpJPEG.begin(serverURL);
		postJPEG(&frameBuffer, httpJPEG);
		httpJPEG.end();
	}

	#if DEBUG == true
	    delay(30000); // An added delay so the network doesn't get overloaded
	#endif
}

#endif