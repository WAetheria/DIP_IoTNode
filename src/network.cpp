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

String postJSON(const String &payload, HTTPClient &http){
	http.addHeader("Content-Type", "application/json");
	
	int httpResponseCode = http.POST(payload);
	String httpResponse  = http.getString();

	return httpResponse;
}

String postSecureJSON(const String& payload, HTTPClient& http, const String& token){
	String authKey = "Bearer " + token;

	http.addHeader("Authorization", authKey);
	http.addHeader("Content-Type", "application/json");

	int httpResponseCode = http.POST(payload);
	String httpResponse  = http.getString();

	return httpResponse;	
}

String postSecureAutoJSON(const String& payload, const String& serverURL, const String& token){
	static HTTPClient http;

	if (!http.connected()){
		http.begin(serverURL);
		http.setReuse(true);
	}

	// TODO: Add JWT Handling here
	return postSecureJSON(payload, http, token);
}

String postJPEG(camera_fb_t* payload, HTTPClient& http){
	http.addHeader("Content-Type", "image/jpeg");
	
	int httpResponseCode = http.POST(payload->buf, payload->len);
	String httpResponse  = http.getString();

	return httpResponse;
}

String postSecureJPEG(camera_fb_t* payload, HTTPClient& http, const String& token){
	String authKey = "Bearer " + token;

	http.addHeader("Authorization", authKey);
	http.addHeader("Content-Type", "image/jpeg");
	
	int httpResponseCode = http.POST(payload->buf, payload->len);
	String httpResponse  = http.getString();

	return httpResponse;
}

String getJSON(HTTPClient &http){
	int httpResponseCode = http.GET();
	String httpResponse  = http.getString();

	return httpResponse;

}

String getSecureJSON(HTTPClient &http, const String& token){
	String authKey = "Bearer " + token;

	http.addHeader("Authorization", authKey);

	int httpResponseCode = http.GET();
	String httpResponse  = http.getString();

	return httpResponse;
}

void saveString(int address, const String& str){
	byte len = str.length();

	EEPROM.write(address, len); // write string length first
	for (int i = 0; i < len; i++) {
		EEPROM.write(address + 1 + i, str[i]);
	}
}

String loadString(int address)
{
    int len = EEPROM.read(address);
	char data[len + 1];

	for (int i = 0; i < len; i++) {
		data[i] = EEPROM.read(address + 1 + i);
	}

	data[len] = '\0';

	return String(data);
}