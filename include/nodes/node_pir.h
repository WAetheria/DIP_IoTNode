#ifndef PIR_NODE
#define PIR_NODE

#include <Arduino.h>
#include <ArduinoJson.h>

#include "network.h"
#include "clock.h"
#include "device.h"

#include "env.h"     // NOTE: CREATE + CONFIGURE BEFORE USE

#define PIR_SENSOR    16
#define STOVE_PIN     34
#define LED_OUTPUT    25 
#define BUZZER_OUTPUT -1

#define PIR_TIMEOUT   10000
#define STOVE_ON      500

Device pir     = Device(PIR_SENSOR   , DeviceMode::DIGITAL_INPUT);
Device stove   = Device(STOVE_PIN    , DeviceMode::ANALOG_INPUT);
Device led     = Device(LED_OUTPUT   , DeviceMode::DIGITAL_OUTPUT);
Device buzzer  = Device(BUZZER_OUTPUT, DeviceMode::DIGITAL_OUTPUT);

Timer pirTimer = Timer(PIR_TIMEOUT);

const char* serverURL = PIR_ENDPOINT_URL;

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
}

void loop(){
    // Sensor Handling
	bool inKitchen = (bool)pir.readInput();
	bool stoveIsOn = (stove.readInput() >= STOVE_ON);

	Serial.printf("inKitchen: %d\n", inKitchen);
	Serial.printf("stoveIsOn: %d\n", stoveIsOn);

	if (inKitchen){pirTimer.resetTimer();}

	(stoveIsOn && pirTimer.timedOut()) ? buzzer.turnOn() : buzzer.turnOff();
	(!pirTimer.timedOut()) ? led.turnOn() : led.turnOff();

	// JSON Handling
	JsonDocument doc;
	
	doc["in_kitchen"] = inKitchen;
	doc["stove_on"]   = stoveIsOn;

	char payload[256];
	serializeJson(doc, payload);

	// HTTP Handling
	if(postSecureAutoJSON(payload, serverURL, jwt, refreshToken)){
        saveToken(JWT_KEYNAME, jwt);
    }

	#if DEBUG == true
	    delay(30000); // An added delay so the network doesn't get overloaded
	#endif
}

#endif