#ifndef PLANT_LIVROOM_NODE
#define PLANT_LIVROOM_NODE

#include <Arduino.h>
#include <ArduinoJson.h>

#include "network.h"
#include "device.h"

#include "env.h"     // NOTE: CREATE + CONFIGURE BEFORE USE

// Living Room Definitions
#define LED1_PIN 16
#define LED2_PIN 17
#define LED3_PIN 25

#define TEMPERATURE_THRESHOLD  0
#define HUMIDITY_THRESHOLD     0
#define ILLUMINATION_THRESHOLD 0

Device led1 = Device(LED1_PIN, DeviceMode::DIGITAL_OUTPUT);
Device led2 = Device(LED2_PIN, DeviceMode::DIGITAL_OUTPUT);
Device led3 = Device(LED3_PIN, DeviceMode::DIGITAL_OUTPUT);

// Plant Definitions
#define WATER_SENSOR    36 
#define WATER_PUMP      26

#define WATER_THRESHOLD 1000

Device water = Device(WATER_SENSOR, DeviceMode::ANALOG_INPUT);
Device pump  = Device(WATER_PUMP  , DeviceMode::DIGITAL_OUTPUT);

// Endpoint URLs
const char* livroomURL = LIVROOM_ENDPOINT_URL;
const char* plantURL   = PLANT_ENDPOINT_URL;

// HTTPClient objects
HTTPClient http1;
HTTPClient http2;

void setup(){
    Serial.begin(115200);
    setupWiFi();
}

void loop(){
    // HTTP setup
    if(!http1.connected()){
        http1.begin(livroomURL);
        http1.setReuse(true);
    }

    if(!http2.connected()){
        http2.begin(plantURL);
        http2.setReuse(true);
    }

    // Living Room Node Portion
    String responseLivRoom;
    responseLivRoom = getJSON(http1);

    Serial.print("Response: ");
    Serial.println(responseLivRoom);

    JsonDocument docLivRoom;
    deserializeJson(docLivRoom, responseLivRoom);

    float temperature = docLivRoom["temperature"];
    float humidity    = docLivRoom["humidity"];
    int illumination  = docLivRoom["illumination"];

    Serial.printf("Temperature: %f\n", temperature);
    Serial.printf("Humidity: %f\n", humidity);
    Serial.printf("Illumination: %d\n", illumination);

    (temperature  >= TEMPERATURE_THRESHOLD)  ? led1.turnOn() : led1.turnOff();
    (humidity     >= HUMIDITY_THRESHOLD)     ? led2.turnOn() : led2.turnOff();
    (illumination >= ILLUMINATION_THRESHOLD) ? led3.turnOn() : led3.turnOff();

    // Plant Node Portion
    int waterLevel = water.readInput();
    (waterLevel <= WATER_THRESHOLD) ? pump.turnOn() : pump.turnOff();
	Serial.printf("Water Level: %d\n", waterLevel);

	JsonDocument docPlant;
	docPlant["Water Level"] = waterLevel;
	char payload[256];
	serializeJson(docPlant, payload);

    String responsePlant;
    postJSON(payload, http2);

    // For Debugging
	#if DEBUG == true
	    delay(30000); // An added delay so the network doesn't get overloaded
	#endif
}

#endif