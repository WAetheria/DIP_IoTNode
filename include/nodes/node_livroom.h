#ifndef LIVROOM_NODE
#define LIVROOM_NODE

#include <Arduino.h>
#include <ArduinoJson.h>

#include "network.h"
#include "device.h"

#include "env.h"     // NOTE: CREATE + CONFIGURE BEFORE USE

#define LED1_PIN -1
#define LED2_PIN -1
#define LED3_PIN -1
#define FAN_PIN  -1

#define TEMPERATURE_THRESHOLD  0
#define HUMIDITY_THRESHOLD     0
#define ILLUMINATION_THRESHOLD 0

Device led1 = Device(LED1_PIN, DeviceMode::DIGITAL_OUTPUT);
Device led2 = Device(LED2_PIN, DeviceMode::DIGITAL_OUTPUT);
Device led3 = Device(LED3_PIN, DeviceMode::DIGITAL_OUTPUT);
Device fan  = Device(FAN_PIN , DeviceMode::DIGITAL_OUTPUT);

const char* serverURL = LIVROOM_ENDPOINT_URL;

void setup(){
    Serial.begin(115200);
    setupWiFi();
}

void loop(){
    static HTTPClient http;
    JsonDocument doc;

	if (!http.connected()){
		http.begin(serverURL);
		http.setReuse(true);
	}

    String response = getJSON(http);

    deserializeJson(doc, response);

    float temperature = doc["temperature"];
    float humidity    = doc["humidity"];
    int illumination  = doc["illumination"];

    (temperature  >= TEMPERATURE_THRESHOLD)  ? fan.turnOn() : fan.turnOff();
    (humidity     >= HUMIDITY_THRESHOLD)     ? led2.turnOn() : led2.turnOff();
    (illumination >= ILLUMINATION_THRESHOLD) ? led3.turnOn() : led3.turnOff();

	#if DEBUG == true
	    delay(30000); // An added delay so the network doesn't get overloaded
	#endif
}

#endif