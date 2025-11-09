#ifndef LIVROOM_NODE
#define LIVROOM_NODE

#include <Arduino.h>
#include <ArduinoJson.h>

#include "network.h"
#include "device.h"

#include "env.h"     // NOTE: CREATE + CONFIGURE BEFORE USE

#define LED1_PIN 16
#define LED2_PIN 17
#define LED3_PIN 25

#define TEMPERATURE_THRESHOLD  0
#define HUMIDITY_THRESHOLD     0
#define ILLUMINATION_THRESHOLD 0

Device led1 = Device(LED1_PIN, DeviceMode::DIGITAL_OUTPUT);
Device led2 = Device(LED2_PIN, DeviceMode::DIGITAL_OUTPUT);
Device led3 = Device(LED3_PIN, DeviceMode::DIGITAL_OUTPUT);

const char* serverURL = LIVROOM_ENDPOINT_URL;

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
    // HTTP Handling
    String response;
    if(getSecureAutoJSON(response, serverURL, jwt, refreshToken)){
        saveToken(JWT_KEYNAME, jwt);
    }
    Serial.print("Actual Response: ");
    Serial.println(response);

    JsonDocument doc;
    deserializeJson(doc, response);

    float temperature = doc["temperature"];
    float humidity    = doc["humidity"];
    int illumination  = doc["illumination"];

    Serial.printf("Temperature: %f\n", temperature);
    Serial.printf("Humidity: %f\n", humidity);
    Serial.printf("Illumination: %d\n", illumination);

    (temperature  >= TEMPERATURE_THRESHOLD)  ? led1.turnOn() : led1.turnOff();
    (humidity     >= HUMIDITY_THRESHOLD)     ? led2.turnOn() : led2.turnOff();
    (illumination >= ILLUMINATION_THRESHOLD) ? led3.turnOn() : led3.turnOff();

	#if DEBUG == true
	    delay(30000); // An added delay so the network doesn't get overloaded
	#endif
}

#endif