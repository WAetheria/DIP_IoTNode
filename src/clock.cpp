#include "clock.h"

TicToc& tt_timer = TicToc::createTicTocTimer();

Timer::Timer(){
    resetTimer();
}

Timer::Timer(unsigned long timeoutInMs){
    setTimeout(timeoutInMs);
    resetTimer();
}

Timer::~Timer(){}

void Timer::updateTimer(){
    endTime = millis();
}

void Timer::resetTimer(){
    startTime = millis();
    endTime = startTime;
}

void Timer::setTimeout(unsigned long timeoutInMs){
    timeout = timeoutInMs;
}

bool Timer::timedOut(){
    updateTimer();

    if (getDuration() >= timeout){
        return true;
    } else {
        return false;
    }
}

unsigned long Timer::getDuration(){
    return endTime - startTime;
}

void tic(){
    tt_timer.ticStartTime = millis();
    Serial.print("Tic!\n");
}

void toc(){
    tt_timer.tocEndTime = millis();
    Serial.printf("Toc! Time elapsed: %lu ms\n", tt_timer.tocEndTime - tt_timer.ticStartTime);
}

bool setupTime()
{
    const char* ntpServer = "pool.ntp.org";
    const long  gmtOffset_sec = 0;           // adjust for your timezone
    const int   daylightOffset_sec = 0;      // adjust for DST

    struct tm timeinfo;
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    
    Serial.print("Configuring internal clock");
    for (int i = 0; i < 3; i++){
        delay(250);
        Serial.print(".");
    }

    if (getLocalTime(&timeinfo)){
        Serial.print("Done\n");
        return true;
    } else {
        Serial.print("Failed!\n");
        return false;
    }

    return true;
}

bool SetupTimeAttempts(int count){
    for (int i = 0; i < count; i++){
        if (setupTime){
            return true;
        }
    }

    return false;

}

String getTime(){
    struct tm timeinfo;
    char timestamp[22];

    if (getLocalTime(&timeinfo)) {
        strftime(timestamp, sizeof(timestamp), "%Y-%m-%dT%H:%M:%SZ", &timeinfo);
    }

    return timestamp;
}