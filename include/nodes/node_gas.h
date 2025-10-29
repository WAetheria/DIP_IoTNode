#ifndef GAS_NODE
#define GAS_NODE

#include <Arduino.h>
#include <ArduinoJson.h>

#include "network.h"
#include "clock.h"
#include "device.h"

#include "env.h"     // NOTE: CREATE + CONFIGURE BEFORE USE

#define GAS_SENSOR_MQ135 34
#define GAS_SENSOR_MQ2   36
#define LED_FOR_MQ135    22
#define LED_FOR_MQ2      23
#define FAN_MOTOR        21

#define GAS_THRESHOLD    25     // In percentage (0% - 100%)
#define FAN_TIMEOUT      15000

Device gas1 = Device(GAS_SENSOR_MQ135, DeviceMode::ANALOG_INPUT);
Device gas2 = Device(GAS_SENSOR_MQ2  , DeviceMode::ANALOG_INPUT);
Device led1 = Device(LED_FOR_MQ135   , DeviceMode::DIGITAL_OUTPUT);
Device led2 = Device(LED_FOR_MQ2     , DeviceMode::DIGITAL_OUTPUT);
Device fan  = Device(FAN_MOTOR       , DeviceMode::DIGITAL_OUTPUT);

Timer gasTimer = Timer(FAN_TIMEOUT);

const char* serverURL = GAS_ENDPOINT_URL;

void setup(){
    Serial.begin(115200);
    setupWiFi();
}

void loop(){
    // Sensor Handling
    int mq135Reading = gas1.readInput();
    int mq2Reading   = gas2.readInput();

    float gas1Perc = 100 * float(mq135Reading)/4095;
    float gas2Perc = 100 * float(mq2Reading)  /4095;

    (gas1Perc >= GAS_THRESHOLD) ? led1.turnOn() : led1.turnOff();
	(gas2Perc >= GAS_THRESHOLD) ? led2.turnOn() : led2.turnOff();

    if (gas1Perc >= GAS_THRESHOLD || gas2Perc >= GAS_THRESHOLD){gasTimer.resetTimer();}
    (!gasTimer.timedOut()) ? fan.turnOn() : fan.turnOff();

    Serial.print("Air Purity (Percentage): ");
    Serial.println(gas1Perc);
    Serial.print("Air Purity (Raw): ");
    Serial.println(mq135Reading);
    Serial.print("Volatile Concentration (Percentage): ");
    Serial.println(gas2Perc);
    Serial.print("Volatile Concentration (Raw): ");
    Serial.println(mq2Reading);

	// JSON Handling
	JsonDocument doc;
	
	doc["air_purity"]             = gas1Perc;
    doc["volatile_concentration"] = gas2Perc;

	char payload[256];
	serializeJson(doc, payload);

	// HTTP Handling
    Serial.println(postAutoJSON(payload, serverURL));

	#if DEBUG == true
	    delay(30000); // An added delay so the network doesn't get overloaded
	#endif
}

#endif