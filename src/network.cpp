#include "network.h"

bool setupWiFi(){
	return setupWiFi("IoT Device Setup", false);
}

bool setupWiFi(char const *manager_ssid)
{
    return setupWiFi(manager_ssid, false);
}

bool setupWiFi(char const *manager_ssid, bool reset)
{
    WiFi.mode(WIFI_STA);                      // Default WiFi mode: Station
	WiFiManager wm;                           // WiFiManager object 

	if (reset){wm.resetSettings();}           // Reset credentials during testing
	wm.setSaveConnectTimeout(10);
	wm.setConnectTimeout(10);
	wm.setConnectRetries(3);

	bool res;
	res = wm.autoConnect(manager_ssid);

	if (!res){
		Serial.print("Failed to connect\n");
	} else{
		Serial.print("WiFi connected\n");
	}

	return res;                               // Returns whether the connection failed
}

void connectToWifi(const char* ssid, const char* password){
	WiFi.begin(ssid, password);               // Initiate the connection process

	Serial.print("Connecting to WiFi");       
	while (WiFi.status() != WL_CONNECTED){
	delay(250);
	Serial.print(".");
	}

    Serial.print("Done\n");                   // Show successful connection and local IP address
}

String postJSON(const String& payload, HTTPClient& http){
	http.addHeader("Content-Type", "application/json");
	
	int httpResponseCode = http.POST(payload);

	Serial.println(httpResponseCode);
	Serial.println(http.getString());

	return http.getString();
}

String postAutoJSON(const String &payload, const String serverURL)
{
    static HTTPClient http;

	if (!http.connected()){
		http.begin(serverURL);
		http.setReuse(true);
	}

	return postJSON(payload, http);
}

int postJPEG(camera_fb_t *payload, HTTPClient &http){
	http.addHeader("Content-Type", "image/jpeg");
	
	int httpResponseCode = http.POST(payload->buf, payload->len);

	return httpResponseCode;
}

String getJSON(HTTPClient &http){
	int httpResponseCode = http.GET();

	Serial.print("HTTP Response Code: ");
	Serial.println(httpResponseCode);

	String httpResponse = http.getString();
    Serial.print("HTTP Response: ");
	Serial.println(httpResponse);

	return httpResponse;
}

void writeStringToEEPROM(int addrOffset, const String &str) {
	byte len = str.length();
	EEPROM.write(addrOffset, len); // write string length first
	for (int i = 0; i < len; i++) {
		EEPROM.write(addrOffset + 1 + i, str[i]);
	}
}

String readStringFromEEPROM(int addrOffset) {
	int len = EEPROM.read(addrOffset);
	char data[len + 1];
	for (int i = 0; i < len; i++) {
		data[i] = EEPROM.read(addrOffset + 1 + i);
	}
	data[len] = '\0';
	return String(data);
}