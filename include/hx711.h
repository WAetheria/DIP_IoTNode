#ifndef MY_HX711_H
#define MY_HX711_H

#include <Arduino.h>

class HX711
{
private:
    uint8_t clockPin;
    uint8_t signalPin;
    uint8_t gain;            // gain here determines how many extra pulses to send after the read operation
    long offset = 0;
    double  scale  = 1;
public:
    HX711(uint8_t clockPin, uint8_t signalPin, uint8_t gain);
    virtual ~HX711();

    bool isReady();
    void setGain(uint8_t gain);
    void calibrate(uint8_t count);

    long getReading();
    long getAverageReading();
    long getCalibratedReading();
    long getScaledReading();

    void setOffset(long offset);
    void setScale(double scale);
    long getOffset();
    float getScale();

    void powerUp();
    void powerDown();
};

#endif