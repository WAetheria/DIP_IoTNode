#include <Arduino.h>
#include <ArduinoJson.h>

#include "network.h"
#include "clock.h"
#include "device.h"
#include "hx711.h"

#include "env.h"     // NOTE: CREATE + CONFIGURE BEFORE USE

#define LIVROOM_BUILD    // VERY IMPORTANT: CHECK BEFORE BUILDING/UPLOADING
#define DEBUG false

// BUILD DEFINITIONS
#ifdef PIR_BUILD
    #define PIR_SENSOR    16
    #define STOVE_SWITCH  -1
    #define LED_OUTPUT    25 
    #define BUZZER_OUTPUT -1

    #define PIR_TIMEOUT   10000
#endif
#ifdef GAS_BUILD
    #define GAS_SENSOR_MQ135 34
    #define GAS_SENSOR_MQ2   36
    #define LED_FOR_MQ135    16
    #define LED_FOR_MQ2      17
    #define FAN_MOTOR        21

    #define GAS_THRESHOLD    1800
    #define FAN_TIMEOUT      15000
#endif
#ifdef LOAD_BUILD
    #define SCK_PIN  16
    #define DOUT_PIN 17
    #define AMP_GAIN 128
#endif
#ifdef PLANT_BUILD
    #define WATER_SENSOR    16 
    #define WATER_PUMP      26 

    #define WATER_THRESHOLD 0  
#endif
#ifdef LIVROOM_BUILD
    #define LED1_PIN              -1
    #define LED2_PIN              -1
    #define LED3_PIN              -1

    #define TEMPERATURE_THRESHOLD  0
    #define HUMIDITY_THRESHOLD     0
    #define ILLUMINATION_THRESHOLD 0
#endif

// BUILD-SPECIFIC OBJECTS
#ifdef PIR_BUILD
    Device pir     = Device(PIR_SENSOR   , DeviceMode::DIGITAL_INPUT);
    Device stove   = Device(STOVE_SWITCH , DeviceMode::DIGITAL_INPUT);
    Device led     = Device(LED_OUTPUT   , DeviceMode::DIGITAL_OUTPUT);
    Device buzzer  = Device(BUZZER_OUTPUT, DeviceMode::DIGITAL_OUTPUT);

    Timer pirTimer = Timer(PIR_TIMEOUT);
#endif
#ifdef GAS_BUILD
    Device gas1 = Device(GAS_SENSOR_MQ135, DeviceMode::ANALOG_INPUT);
    Device gas2 = Device(GAS_SENSOR_MQ2  , DeviceMode::ANALOG_INPUT);
    Device led1 = Device(LED_FOR_MQ135   , DeviceMode::DIGITAL_OUTPUT);
    Device led2 = Device(LED_FOR_MQ2     , DeviceMode::DIGITAL_OUTPUT);
    Device fan  = Device(FAN_MOTOR       , DeviceMode::DIGITAL_OUTPUT);

    Timer gasTimer = Timer(FAN_TIMEOUT);
#endif
#ifdef LOAD_BUILD
    HX711 load = HX711(SCK_PIN, DOUT_PIN, AMP_GAIN);
#endif
#ifdef PLANT_BUILD
    Device water = Device(WATER_SENSOR, DeviceMode::ANALOG_INPUT);
    Device pump  = Device(WATER_PUMP  , DeviceMode::DIGITAL_OUTPUT);
#endif
#ifdef LIVROOM_BUILD
    Device led1 = Device(LED1_PIN, DeviceMode::DIGITAL_OUTPUT);
    Device led2 = Device(LED2_PIN, DeviceMode::DIGITAL_OUTPUT);
    Device led3 = Device(LED3_PIN, DeviceMode::DIGITAL_OUTPUT);
#endif

// ENDPOINT URL
#ifdef PIR_BUILD
    const char* serverURL = PIR_ENDPOINT_URL;
#endif
#ifdef GAS_BUILD
    const char* serverURL = GAS_ENDPOINT_URL;
#endif
#ifdef LOAD_BUILD
    const char* serverURL = LOAD_ENDPOINT_URL;
#endif
#ifdef PLANT_BUILD
    const char* serverURL = PLANT_ENDPOINT_URL;
#endif
#ifdef LIVROOM_BUILD
    const char* serverURL = LIVROOM_ENDPOINT_URL;
#endif

#ifndef TEST_BUILD
void setup(){
    Serial.begin(115200);

    const char* managerSSID = "IoT Device Setup";
    bool resetWiFiSettings  = false;

    setupWiFi(managerSSID, resetWiFiSettings);
}
#endif

#ifdef PIR_BUILD
/*======================================================================================*/
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
/*======================================================================================*/
#endif

#ifdef GAS_BUILD
/*======================================================================================*/
void loop(){
    // Sensor Handling
    int mq135Reading;
    int mq2Reading;

    (gas1.readInput() >= GAS_THRESHOLD) ? led1.turnOn() : led1.turnOff();
	mq135Reading = gas1.readInput();

	(gas2.readInput() >= GAS_THRESHOLD) ? led2.turnOn() : led2.turnOff();
	mq2Reading = gas2.readInput();

    if (mq135Reading >= GAS_THRESHOLD || mq2Reading >= GAS_THRESHOLD){gasTimer.resetTimer();}
    (!gasTimer.timedOut()) ? fan.turnOn() : fan.turnOff();

	// JSON Handling
	JsonDocument doc;
	
	doc["air_purity"]             = 100 * float(mq135Reading/4095);
    doc["volatile_concentration"] = 100 * float(mq2Reading  /4095);

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
/*======================================================================================*/
#endif

#ifdef LOAD_BUILD
/*======================================================================================*/
void loop(){
    if(load.isReady()){
        Serial.print("Load Reading (Raw): ");
        Serial.println(load.getReading());
    } else {
        Serial.print("HX711 Not Found.\n");
    }
}
/*======================================================================================*/
#endif

#ifdef PLANT_BUILD
/*======================================================================================*/
void loop(){
    // Sensor Handling
    int waterLevel = water.readInput();

    (waterLevel <= WATER_THRESHOLD) ? pump.turnOn() : pump.turnOff();
	#if DEBUG == true
		Serial.print("Water Level: ");
		Serial.println(waterLevel);
	#endif

    // JSON Handling
	JsonDocument doc;
	
	doc["Water Level"] = waterLevel;

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
/*======================================================================================*/
#endif

#ifdef LIVROOM_BUILD
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

    (temperature >= TEMPERATURE_THRESHOLD)   ? led1.turnOn() : led1.turnOff();
    (humidity >= HUMIDITY_THRESHOLD)         ? led2.turnOn() : led2.turnOff();
    (illumination >= ILLUMINATION_THRESHOLD) ? led3.turnOn() : led3.turnOff();

	#if DEBUG == true
	delay(30000); // An added delay so the network doesn't get overloaded
	#endif
}
#endif

#ifdef TEST_BUILD
/*======================================================================================*/
void setup(){
	Serial.begin(115200);
}

void loop(){

}
/*======================================================================================*/
#endif