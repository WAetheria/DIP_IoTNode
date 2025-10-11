#include "device.h"

int pwmChannels[16] = {0};

Device::Device(int gpioPin, DeviceMode mode){
    this->gpioPin = gpioPin;
    this->mode = mode;

    this->isOn = false;
    this->channel = -1;
    this->resolution = 5000;
    this->duty = -1.f;

    if (mode == DeviceMode::DIGITAL_INPUT || mode == DeviceMode::ANALOG_INPUT){
        pinMode(gpioPin, INPUT);
    } else {
        pinMode(gpioPin, OUTPUT);
    }
}

Device::~Device(){}

void Device::show(){
    Serial.print("GPIO Pin: ");
    Serial.println(gpioPin);
    Serial.print("Device Mode: ");
    Serial.println(static_cast<int>(mode));
    Serial.print("IsOn: ");
    Serial.println(isOn);
    Serial.print("Channel: ");
    Serial.println(channel);
    Serial.print("Resolution: ");
    Serial.println(resolution);
    Serial.print("Duty Cycle: ");
    Serial.println(duty);
}

int Device::getChannel(){
    return channel;
}

float Device::getDuty(){
    return duty;
}

void Device::update(){
    switch (mode){
        case DeviceMode::DIGITAL_OUTPUT:
            digitalUpdate();
            break;
        case DeviceMode::PWM_OUTPUT:
            pwmUpdate();
            break;
        default:
            Serial.printf("Error: Update failed due to invalid device mode at pin %d\n", gpioPin);
            break;
    }
}

void Device::digitalUpdate(){
    (isOn) ? digitalWrite(gpioPin, HIGH) : digitalWrite(gpioPin, LOW);
}

void Device::pwmUpdate(){
    if (duty < 0){
        Serial.printf("Error: PWM update failed due to invalid duty at pin %d", gpioPin);
        return;
    }

    if (isOn){
        ledcWrite(channel, int(duty * pow(2, resolution)));
    } else {
        ledcWrite(channel, 0);
    }
}

int Device::findEmptyChannel(){
    for (int i = 0; i < sizeof(pwmChannels)/sizeof(pwmChannels[0]); i++){
        if (pwmChannels[i] == 0){
            return i;
        }
    }

    return -1;
}

bool Device::setPWM(int freq, int resolution){
    if (mode != DeviceMode::PWM_OUTPUT){
        Serial.printf("Error: Cannot use setPWM() on non-PWM output mode at pin %d", gpioPin);
        return false;
    }

    this->channel = findEmptyChannel();
    this->resolution = resolution;

    if (channel < 0){
        Serial.printf("Error: No channels found for pin %d", gpioPin);
        return false;
    }

    ledcSetup(channel, freq, resolution);
    ledcAttachPin(gpioPin, channel);

    pwmChannels[channel] = 1;
    return true;
}

void Device::setDuty(float duty){
    this->duty = duty;
    update();
}

void Device::setChannel(int channel){
    this->channel = channel;
    update();
}

void Device::toggle(){
    isOn = !isOn;
    update();
}

void Device::turnOn(){
    isOn = true;
    update();
}

void Device::turnOff(){
    isOn = false;
    update();
}

int Device::readInput(){
    switch (mode){
        case DeviceMode::DIGITAL_INPUT:
            return digitalRead(gpioPin);
        case DeviceMode::ANALOG_INPUT:
            return analogRead(gpioPin);
        default:
            Serial.printf("Error: Attempting to read input from non-input device at pin %d\n", gpioPin);
            return -1;
    }
}