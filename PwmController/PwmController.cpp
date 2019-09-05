#include <Wire.h>

#include "PwmController.h"

PwmController::PwmController(int address) {
    this->address = address;
}

void PwmController::Begin() {
    Wire.begin();
    Reset();
}

void PwmController::Reset() {
    Wire.beginTransmission(address);
    Wire.write(MODE1);
    Wire.write(0);
    Wire.endTransmission();
}

void PwmController::SetPwmFreq(int freq) {

    //Checking if freq is between boundries:
    freq = min(freq, 1600);

    //Caluclating prescale value;
    int prescale = floor(25000000/(4096*freq));

    int oldMode = readFromAddress(MODE1);
    writeToAddress(MODE1, oldMode | 0x10);
    writeToAddress(PRE_SCALE, prescale);
    writeToAddress(MODE1, oldMode);

    delay(5);
    writeToAddress(MODE1, oldMode | 0xa1);
}

//0 -> 4095
void PwmController::SetPin(int pinNumber, int value ) {

    //Making sure value is between 0 and 4096
    value = min(value, 4096);

    if(value == 0) {
        setPwm(pinNumber, 0, 4096);
    }
    else if(value == 4096) {
        setPwm(pinNumber, 4096, 0);
    }
    else {
        setPwm(pinNumber, 0, value);
    }
}

void PwmController::setPwm(int pinNumber, int on, int off) {

    Wire.beginTransmission(address);
    Wire.write(LED0_ON_L + (4*pinNumber));

    //Writing on and off pwm values:
    Wire.write(on);
    Wire.write(on >> 8);
    Wire.write(off);
    Wire.write(off >> 8);
    Wire.endTransmission();
}

void PwmController::writeToAddress(int add, int value) {
    Wire.beginTransmission(address);
    Wire.write(add);
    Wire.write(value);
    Wire.endTransmission();
}

int PwmController::readFromAddress(int add) {
    Wire.beginTransmission(address);
    Wire.write(add);
    Wire.endTransmission();

    Wire.requestFrom(address, 1);
    return Wire.read();
}