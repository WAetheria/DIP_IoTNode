#ifndef TEST_NODE
#define TEST_NODE

#include <Arduino.h>

#include "camera.h"

void setup(){
    Serial.begin(115200);
    camera_init();
}

void loop(){
    camera_capture();
    delay(10000);
}

#endif