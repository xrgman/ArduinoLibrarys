#include "Arduino.h"
#include "Led.h"

//******************************************************************
//** Constructor.
//** param LedPin - The pin which the Led mosfet is connected to.
//******************************************************************
Led::Led(int LedPin) {
    this->LedPin = LedPin;
    this->usePwmController = false;
    this->maxBrightness = 255;
    this->fadeSpeed = 5;
}

//******************************************************************
//** Constructor.
//** param LedPin - The pin which the Led mosfet is connected to.
//******************************************************************
Led::Led(int LedPin, PwmController pwmController) {
    this->LedPin = LedPin;
    this->pwmController = pwmController;
    this->usePwmController = true;
    this->maxBrightness = 4096;
    this->fadeSpeed = 40;
}

//***********************************************
//** turnOn - Initializes fading on the Led.
//***********************************************
void Led::turnOn() {
    //LedValue = 0;
    LedFading = true;
    LedState = true;
}

//***********************************************
//** turnOff - Initializes fading off the Led.
//***********************************************
void Led::turnOff() {
    //LedValue = 255;
    LedFading = true;
    LedState = false;
}

//***********************************************
//** fadeLed - Fades the Led.
//***********************************************
void Led::fadeLed() {
    if(LedValue <= maxBrightness  && LedValue >= 0 && LedFading) {

        if(!usePwmController)
            analogWrite(LedPin, LedValue);
        else
            pwmController.SetPin(LedPin, LedValue);

        if(LedState)
            LedValue += fadeSpeed;
        else
            LedValue -= fadeSpeed;
    }
    else {
        LedFading = false;
        if(LedState)
            LedValue -= fadeSpeed;
        else
            LedValue += fadeSpeed;
    }
}

//************************************************
//** getLedFading.
//** Return whether or not the Led is fading.
//************************************************
bool Led::getLedFading() {
    return LedFading;
}

bool Led::getLedOn() {
    return getBrightness() > 0;
}

//************************************************
//** changeBrightness.
//** Param offset - The amount of brightness you want to add to the current brightness.
//************************************************
void Led::changeBrightness(int offset) {
    if((LedValue + offset) <= maxBrightness && (LedValue + offset) >= 0) {
        LedValue += offset;

        if(!usePwmController)
            analogWrite(LedPin, LedValue);
        else
            pwmController.SetPin(LedPin, LedValue);
    }
}

//************************************************
//** setBrightness.
//** Param value - The new brightness value of the Led.
//************************************************
void Led::setBrightness(int value) {
    if(value >= 0 && value <= maxBrightness) {
        LedValue = value;

        if(!usePwmController)
            analogWrite(LedPin, LedValue);
        else
            pwmController.SetPin(LedPin, LedValue);
    }
}

//************************************************
//** getBrightness.
//** Return the current brightness of the Led.
//************************************************
int Led::getBrightness() {
    return LedValue;
}