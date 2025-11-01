#include <ArduinoJson.h>

#include "network.h"
#include "env.h"

#define DEVICE_LIVROOM

#ifdef DEVICE_GAS
    const char* deviceID   = GAS_NODE_ID;
    const char* deviceName = GAS_NODE_NAME; 
#endif
#ifdef DEVICE_LIVROOM
    const char* deviceID   = LIVROOM_NODE_ID;
    const char* deviceName = LIVROOM_NODE_NAME; 
#endif
#ifdef DEVICE_PIR
    const char* deviceID   = PIR_NODE_ID;
    const char* deviceName = PIR_NODE_NAME; 
#endif
#ifdef DEVICE_PLANT
    const char* deviceID   = PLANT_NODE_ID;
    const char* deviceName = PLANT_NODE_NAME; 
#endif

const char* serverURL  = AUTHENTICATION_URL;

void setup(){
    // Setups all the necessary stuff
    Serial.begin(115200);
    setupWiFi();

    // Creates a JSON payload
    JsonDocument payloadDoc;
    payloadDoc["device_id"] = deviceID;
    payloadDoc["name"] = deviceName;
    char payload[256];
    serializeJson(payloadDoc, payload);

    Serial.printf("Payload: %s\n", payload);

    // Posts the JSON payload
    HTTPClient http;
    http.begin(serverURL);
    String response = postJSON(payload, http);

    Serial.print("HTTP Response: ");
    Serial.println(response);

    // Parses the JSON response for refresh token
    JsonDocument responseDoc;
    deserializeJson(responseDoc, response);
    String jwt = responseDoc["jwt"];
    String refreshToken = responseDoc["refresh_token"];

    Serial.print("JWT: ");
    Serial.println(jwt);

    Serial.print("Refresh Token: ");
    Serial.println(refreshToken);

    // Save the refresh token to permanent memory
    if (refreshToken != NULL){
        saveToken(refreshToken);
    }

    String savedToken = loadToken();
    Serial.print("Saved Token: ");
    Serial.println(savedToken);
}

void loop(){}