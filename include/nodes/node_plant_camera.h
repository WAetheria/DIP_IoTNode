#ifndef PLANT_CAMERA_NODE
#define PLANT_CAMERA_NODE

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

#include "camera.h"
#include "device.h"
#include "network.h"

camera_fb_t frameBuffer;

const char* serverURL = CAMERA_UPLOAD_URL;

void setup(){
	Serial.begin(115200);
	setupWiFi();

	camera_init();

    HTTPClient http;
    http.begin(serverURL);
    http.addHeader("Content-Type", "image/jpeg");

    camera_fb_t* frameBuffer = esp_camera_fb_get();

    int httpCode = http.POST(frameBuffer->buf, frameBuffer->len);
    Serial.printf("Response Code: %d\n", httpCode);
    Serial.println(http.errorToString(httpCode));

    http.end();
    esp_camera_fb_return(frameBuffer);
}

#endif