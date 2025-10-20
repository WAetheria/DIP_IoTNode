#ifndef PIR_NODE
#define PIR_NODE

#include <Arduino.h>
#include <ArduinoJson.h>

#include "network.h"
#include "clock.h"
#include "device.h"

#include "env.h"     // NOTE: CREATE + CONFIGURE BEFORE USE

#define PIR_SENSOR    16
#define STOVE_SWITCH  -1
#define LED_OUTPUT    25 
#define BUZZER_OUTPUT -1

#define PIR_TIMEOUT   10000

Device pir     = Device(PIR_SENSOR   , DeviceMode::DIGITAL_INPUT);
Device stove   = Device(STOVE_SWITCH , DeviceMode::DIGITAL_INPUT);
Device led     = Device(LED_OUTPUT   , DeviceMode::DIGITAL_OUTPUT);
Device buzzer  = Device(BUZZER_OUTPUT, DeviceMode::DIGITAL_OUTPUT);

Timer pirTimer = Timer(PIR_TIMEOUT);

const char* serverURL = PIR_ENDPOINT_URL;

void setup(){
    Serial.begin(115200);

    const char* managerSSID = "IoT Device Setup";
    bool resetWiFiSettings  = false;

    setupWiFi(managerSSID, resetWiFiSettings);
}

void loop(){
    // Sensor Handling
	bool inKitchen = (bool)pir.readInput();
	bool stoveIsOn = (bool)stove.readInput();

	if (inKitchen){pirTimer.resetTimer();}

	(stoveIsOn && pirTimer.timedOut()) ? buzzer.turnOn() : buzzer.turnOff();
	(!pirTimer.timedOut()) ? led.turnOn() : led.turnOff();

	#if DEBUG == true
		Serial.print("inKitchen: ");
		Serial.println(inKitchen);
		Serial.print("stoveIsOn: ");
		Serial.println(stoveIsOn);
	#endif

	// JSON Handling
	JsonDocument doc;
	
	doc["in_kitchen"] = inKitchen;
	doc["stove_on"]   = stoveIsOn;

	char payload[256];
	serializeJson(doc, payload);

	// HTTP Handling
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