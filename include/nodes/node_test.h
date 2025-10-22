#ifndef TEST_NODE
#define TEST_NODE

#include <Arduino.h>

#include "camera.h"
#include "device.h"

Device resistor = Device(35, DeviceMode::ANALOG_INPUT);

void setup(){
    Serial.begin(115200);
}

void loop(){
    Serial.println(resistor.readInput());
}

#endif