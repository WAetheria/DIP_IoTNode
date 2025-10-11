#include <Arduino.h>
#include <ArduinoJson.h>

#include "network.h"
#include "clock.h"
#include "device.h"
#include "hx711.h"

#include "env.h"   // NOTE: CREATE + CONFIGURE BEFORE USE

#define TEST_BUILD  // VERY IMPORTANT: CHECK BEFORE BUILDING/UPLOADING
#define DEBUG   true

#ifdef PIR_BUILD
/*======================================================================================*/
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

const char* serverName  = PIR_ENDPOINT_URL;
const char* managerSSID = "PIR Node";
bool resetWiFiSettings  = false;

HTTPClient http;

void setup(){
    Serial.begin(115200);

	setupWiFi(managerSSID, resetWiFiSettings);
    SetupTimeAttempts(3);
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
	if (!http.connected()){
		http.begin(serverName);
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
#define GAS_SENSOR_MQ135 34
#define GAS_SENSOR_MQ2   36
#define LED_FOR_MQ135    16
#define LED_FOR_MQ2      17
#define FAN_MOTOR        21

#define GAS_THRESHOLD    1800
#define GAS_TIMEOUT      15000

Device gas1 = Device(GAS_SENSOR_MQ135, DeviceMode::ANALOG_INPUT);
Device gas2 = Device(GAS_SENSOR_MQ2  , DeviceMode::ANALOG_INPUT);
Device led1 = Device(LED_FOR_MQ135   , DeviceMode::DIGITAL_OUTPUT);
Device led2 = Device(LED_FOR_MQ2     , DeviceMode::DIGITAL_OUTPUT);
Device fan  = Device(FAN_MOTOR       , DeviceMode::DIGITAL_OUTPUT);

Timer gasTimer = Timer(GAS_TIMEOUT);

const char* serverName  = GAS_ENDPOINT_URL;
const char* managerSSID = "Gas Node";
bool resetWiFiSettings  = false;

HTTPClient http;

void setup(){
    Serial.begin(115200);

    setupWiFi(managerSSID, resetWiFiSettings);
    SetupTimeAttempts(3);
}

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
	if (!http.connected()){
		http.begin(serverName);
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
#define SCK_PIN   16
#define DOUT_PIN  17
#define AMP_GAIN  128

HX711 load = HX711(SCK_PIN, DOUT_PIN, AMP_GAIN);

const char* serverName  = LOAD_ENDPOINT_URL;
const char* managerSSID = "Load Node";
bool resetWiFiSettings  = false;

void setup(){
    Serial.begin(115200);

	setupWiFi(managerSSID, resetWiFiSettings);
    SetupTimeAttempts(3);
}

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
#define WATER_SENSOR          16  // Change Later
#define WATER_PUMP            26  // Change Later

#define WATER_THRESHOLD       0  // Change Later

Device water = Device(WATER_SENSOR, DeviceMode::ANALOG_INPUT);
Device pump  = Device(WATER_PUMP  , DeviceMode::DIGITAL_OUTPUT);

const char* serverName  = PLANT_ENDPOINT_URL;
const char* managerSSID = "Plant Node";
bool resetWiFiSettings  = false;

HTTPClient http;

void setup(){
    Serial.begin(115200);

	setupWiFi(managerSSID, resetWiFiSettings);
    SetupTimeAttempts(3);
}

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
	if (!http.connected()){
		http.begin(serverName);
		http.setReuse(true);
	}

	postJSON(payload, http);

	#if DEBUG == true
	delay(30000); // An added delay so the network doesn't get overloaded
	#endif
}
/*======================================================================================*/
#endif

#ifdef TEST_BUILD
/*======================================================================================*/
void setup(){
	Serial.begin(115200);
	Serial.println("Hello World!");

	pinMode(25, OUTPUT);
}

void loop(){
	digitalWrite(25, HIGH);
}
/*======================================================================================*/
#endif