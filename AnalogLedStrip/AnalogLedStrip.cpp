#include "Arduino.h"
#include "AnalogLedStrip.h"
//
// Created by Beast on 10/1/2016.
//

//******************************************************************
//** Constructor #1.
//** param redPin - The pin which the red color controlling mosfet is connected to.
//** param greenPin - The pin which the green color controlling mosfet is connected to.
//** param bluePin - The pin which the blue color controlling mosfet is connected to.
//******************************************************************
AnalogLedStrip::AnalogLedStrip(int redPin, int greenPin, int bluePin) {

    this->redPin = redPin;
    this->greenPin = greenPin;
    this->bluePin = bluePin;
    this->effectSpeed = 4;
    SetColor(255,255,255);
    SetPower(false);
    SetBrightness(255);
}

//******************************************************************
//** Constructor #2.
//** param redPin - The pin which the red color controlling mosfet is connected to.
//** param greenPin - The pin which the green color controlling mosfet is connected to.
//** param bluePin - The pin which the blue color controlling mosfet is connected to.
//** param startAddress - The memory address of the first availible memory.
//** param endAddress - The last memory adress, needs to be .. before the start address.
//******************************************************************
AnalogLedStrip::AnalogLedStrip(int redPin, int greenPin, int bluePin, int startAddress, int endAddress) {
    this->redPin = redPin;
    this->greenPin = greenPin;
    this->bluePin = bluePin;
    //Trying to load settings:
    //LoadSettings();
    power = true;
}

//******************************************************************
//** TimerEvent.
//** Called from the main timer event.
//******************************************************************
void AnalogLedStrip::TimerEvent() {
    if(timerCounter % effectSpeed == 0 || effectSpeed == 0) {
        if (effectMode == 'j')
            Jump3Effect();
        if (effectMode == 'J')
            Jump7Effect();
        if (effectMode == 'f')
            Fade3Effect();
        if (effectMode == 'F')
            Fade7Effect();
        if (effectMode == 'v')
            FlashEffect();
        timerCounter = 0;
    }
    timerCounter++;
}

//******************************************************************
//** SerialEvent.
//** Called from the main serial event.
//******************************************************************
void AnalogLedStrip::SerialEvent(String serialCommand) {
    String command = serialCommand.substring(0, 3);
    if (command.equals("SCL")) { //Set the color of the led strip.
        red = serialCommand.substring(3, 6).toInt();
        green = serialCommand.substring(6, 9).toInt();
        blue = serialCommand.substring(9, 12).toInt();
        SetColor(red, green, blue);
        Serial.println("Ledstrip color set");
    }
    else if (command.equals("POW")) {
        if (!serialCommand.substring(3, 4).toInt()) {
            SetPower(false);
            Serial.println("Ledstrip turned off");
        }
        else {
            SetPower(true);
            Serial.println("Ledstrip turned on");
        }
    }

}

void AnalogLedStrip::SetColor(int red, int green, int blue) {
    if (effectMode != 'v')
        effectMode = ' ';
    analogWrite(redPin, red);
    analogWrite(greenPin, green);
    analogWrite(bluePin, blue);
    if (red != 0 || green != 0 || blue != 0)
        SetRecentColor(red, green, blue);
}

void AnalogLedStrip::SetColorEffect(int red, int green, int blue) {
    analogWrite(redPin, red);
    analogWrite(greenPin, green);
    analogWrite(bluePin, blue);
    //if (red != 0 || green != 0 || blue != 0)
        SetRecentColorEffect(red, green, blue);
}

void AnalogLedStrip::SetRecentColor(int red, int green, int blue) {
    this->red = red;
    this->green = green;
    this->blue = blue;
}

void AnalogLedStrip::SetRecentColorEffect(int red, int green, int blue) {
    effectRed = red;
    effectGreen = green;
    effectBlue = blue;
}

//Effects:

void AnalogLedStrip::Jump3() {
    effectMode = 'j';
    colorId = 0;
}

void AnalogLedStrip::Jump3Effect() {
    if (power) {
        switch (colorId % 36) {
            case 0:
                SetColorEffect(255, 0, 0); //Red
                break;
            case 12:
                SetColorEffect(0, 255, 0); //Green
                break;
            case 24:
                SetColorEffect(0, 0, 255); //Blue
                break;
        }
        colorId++;
    }
}

void AnalogLedStrip::Jump7() {
    effectMode = 'J';
    colorId = 0;
}

void AnalogLedStrip::Jump7Effect() {
    if (power) {
        switch (colorId % 84) {
            case 0:
                SetColorEffect(255, 0, 0); //Red
                break;
            case 12:
                SetColorEffect(0, 255, 0); //Green
                break;
            case 24:
                SetColorEffect(0, 0, 255); //Blue
                break;
            case 36:
                SetColorEffect(255, 40, 0); //Orange
                break;
            case 48:
                SetColorEffect(0, 191, 255); //Deep sky blue
                break;
            case 60:
                SetColorEffect(255, 255, 0); //Yellow
                break;
            case 72:
                SetColorEffect(75, 0, 130); //Indigo
                break;
        }
        colorId++;
    }
}

void AnalogLedStrip::Fade3() {
    effectMode = 'f';
    effectCounter = 0;
    colorId = 0;
}

void AnalogLedStrip::Fade3Effect() {
    if (power) {
        if (effectCounter < 255) {
            if(colorId%3 == 0)
                SetColorEffect(255-effectCounter,effectCounter,0);
            else if(colorId%3 == 1)
                SetColorEffect(0,255-effectCounter,effectCounter);
            else if(colorId%3 == 2)
                SetColorEffect(effectCounter,0,255-effectCounter);
            effectCounter++;
        }
        else {
            effectCounter = 0;
            colorId++;
        }
    }
}

void AnalogLedStrip::Fade7() {
    effectMode = 'F';
    effectCounter = 0;
    colorId = 0;
}

void AnalogLedStrip::Fade7Effect() {
    if (power) {
        if (effectCounter < 255) {
            if(colorId%3 == 0)
                SetColorEffect(255-effectCounter,effectCounter,effectCounter);
            else if(colorId%3 == 1)
                SetColorEffect(0,255,255-effectCounter);
            else if(colorId%3 == 2)
                SetColorEffect(effectCounter,255-effectCounter,0);
            effectCounter++;
        }
        else {
            effectCounter = 0;
            colorId++;
            if(colorId % 3 == 0)
                colorId = 0;
        }
    }
}

//******************************************************************
//** Flash effect, makes the ledstrip flash in the current color.
//** This method can be used as a switch, meaning that it can turn the flashing on and off.
//******************************************************************
void AnalogLedStrip::Flash() {
    if(effectMode != 'v') {
        effectMode = 'v';
    }
    else {
        effectMode = ' ';
        SetColor(red, green, blue);
    }
}

//******************************************************************
//** Private method that gets called from the timerevent method.
//** This method makes the flash effect.
//******************************************************************
void AnalogLedStrip::FlashEffect() {
    if(power) {
        if (effectRed == 0 && effectGreen == 0 && effectBlue == 0) {
            SetColorEffect(red, green, blue);
        }
        else {
            SetColorEffect(0, 0, 0);
        }
    }
}

void AnalogLedStrip::AlterColor(Color color, int dif) {
    switch (color) {
        case Color::Red:
            if (red + dif <= 255 && red + dif >= 0)
                red += dif;
            else if(dif < 0)
                red = 0;
            else
                red = 255;
            break;
        case Color::Green:
            if (green + dif <= 255 && green + dif >= 0)
                green += dif;
            else if(dif < 0)
                green = 0;
            else
                green = 255;
            break;
        case Color::Blue:
            if (blue + dif < 255 && blue + dif > 0)
                blue += dif;
            else if(dif < 0)
                blue = 0;
            else
                blue = 255;
            break;
    }
    SetColor(red, green, blue);
}


//Getters and setter:
void AnalogLedStrip::SetPower(bool state) {
    if (!state)
        SetColorEffect(0, 0, 0);
    else {
        if (effectMode == ' ')
            SetColorEffect(red, green, blue);
        else
            SetColorEffect(effectRed, effectGreen, effectBlue);
    }
    power = state;
}

void AnalogLedStrip::SetEffectSpeed(int speed) {
    //Flipping the number to let it make some sense
    if(speed > 0)
        speed *= -1;
    else
        speed -= speed*2;

    if(effectSpeed + speed >= 7)
        this->effectSpeed = 7;
    else if(effectSpeed + speed <= 0)
        this->effectSpeed = 0;
    else
        this->effectSpeed += speed;
}

void AnalogLedStrip::SetBrightness(int brightness) {

}

bool AnalogLedStrip::GetPower() {
    return power;
}

int AnalogLedStrip::GetRed() {
    return red;
}

int AnalogLedStrip::GetGreen() {
    return green;
}

int AnalogLedStrip::GetBlue() {
    return blue;
}