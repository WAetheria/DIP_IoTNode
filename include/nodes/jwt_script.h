#include <ArduinoJson.h>

#include "network.h"
#include "env.h"

#define REFRESH_TOKEN_SIZE 128
#define REFRESH_TOKEN_ADDR 0

const char* serverURL  = "example.com";
const char* deviceID   = "id";
const char* deviceName = "name"; 

void setup(){
    // Setups all the necessary stuff
    Serial.begin(115200);
    setupWiFi();
    if (!EEPROM.begin(REFRESH_TOKEN_SIZE)){
        Serial.println("EEPROM.begin() failed!");
    }

    // Creates a JSON payload
    JsonDocument payloadDoc;
    payloadDoc["id"] = deviceID;
    payloadDoc["device_name"] = deviceName;
    String payload;
    serializeJson(payloadDoc, payload);

    Serial.printf("Payload: %s\n", &payload);

    // Posts the JSON payload
    HTTPClient http;
    http.begin(serverURL);
    String response = postJSON(payload, http);

    Serial.printf("Response: %s\n", &response);

    // Parses the JSON response for refresh token
    JsonDocument responseDoc;
    deserializeJson(responseDoc, response);
    String refreshToken = responseDoc["refresh_token"];

    Serial.printf("Refresh Token: %s\n", &refreshToken);

    // Save the refresh token to permanent memory
    saveString(REFRESH_TOKEN_ADDR, refreshToken);
    EEPROM.commit();

    String savedToken = loadString(REFRESH_TOKEN_ADDR);
    Serial.printf("Saved Token: %s\n", &savedToken);
}

void loop(){}