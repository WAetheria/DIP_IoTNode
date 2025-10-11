#include "hx711.h"

HX711::HX711(uint8_t clockPin, uint8_t signalPin, uint8_t gain){
    this->clockPin = clockPin;
    this->signalPin = signalPin;
    setGain(gain);

    pinMode(clockPin, OUTPUT);
    pinMode(signalPin, INPUT);
}

HX711::~HX711(){}

bool HX711::isReady(){
    if (digitalRead(signalPin) == LOW){
        return true;
    } else {
        return false;
    }
}

void HX711::setGain(uint8_t gain){
    switch (gain)
    {
    case 128:
        this->gain = 1;   // Channel A (default)
        break;
    case 64:
        this->gain = 3;   // Channel A
        break;
    case 32:
        this->gain = 2;   // Channel B
        break;
    default:
        break;
    }
}

long HX711::getReading(){
    while (!isReady()){}

    long value = 0;

    noInterrupts();
    {
        for (int i = 0; i < 24; i++){
            digitalWrite(clockPin, HIGH);
            delayMicroseconds(1);

            value |= digitalRead(signalPin) << (23 - i);
            
            digitalWrite(clockPin, LOW);
            delayMicroseconds(1);
        }

        for (int i = 0; i < gain; i++){
            digitalWrite(clockPin, HIGH);
            delayMicroseconds(1);

            digitalWrite(clockPin, LOW);
            delayMicroseconds(1);
        }
    }
    interrupts();

    if (value & 0x00800000){
        for (int i = 0; i < 8; i++){
            value |= 1 << (31 - i);
        }
    }

    return static_cast<long>(value);
}

void HX711::setOffset(long offset){
    this->offset = offset;
}

void HX711::setScale(double scale)
{
    this->scale = scale;
}

long HX711::getOffset(){
    return offset;
}

float HX711::getScale(){
    return scale;
}

void HX711::powerUp(){
    digitalWrite(clockPin, LOW);
}

void HX711::powerDown(){
    digitalWrite(clockPin, HIGH);
    delayMicroseconds(60);
}