//
// Created by Beast on 10/1/2016.
//
#include "AnalogLedStrip.h"

void AnalogLedStrip::SetColor(int redPin, int greenPin, int bluePin) {
    this->red = redPin;
    this->greenPin = greenPin;
    this->bluePin = bluePin;
}

AnalogLedStrip::AnalogLedStrip(int redPin, int greenPin, int redPin , int startAddress, int endAddress) {
    this->red = redPin;
    this->greenPin = greenPin;
    this->bluePin = bluePin;
}

void AnalogLedStrip::SetColor(int red, int green, int blue) { }