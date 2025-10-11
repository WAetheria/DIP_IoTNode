#ifndef MY_DEVICE_H
#define MY_DEVICE_H

#include <Arduino.h>

enum class DeviceMode{
    INACTIVE,
    DIGITAL_OUTPUT,
    DIGITAL_INPUT,
    ANALOG_INPUT,
    PWM_OUTPUT
};

class Device
{
private:
    int gpioPin;
    DeviceMode mode;

    bool isOn;
    int channel;
    int resolution;
    float duty;
    
    int findEmptyChannel();
public:
    Device(int gpioPin, DeviceMode mode);
    virtual ~Device();

    void show();
    int getChannel();
    float getDuty();

    void update();
    void digitalUpdate();
    void pwmUpdate();

    bool setPWM(int freq, int resolution);
    void setDuty(float duty);
    void setChannel(int channel);

    void toggle();
    void turnOn();
    void turnOff();

    int readInput();
};

#endif