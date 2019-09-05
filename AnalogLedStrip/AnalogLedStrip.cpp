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
    this->usePwmController = false;
    this->effectSpeed = 4;
    SetColor(255,255,255);
    SetPower(false);
    SetBrightness(255);
}

AnalogLedStrip::AnalogLedStrip(int redPin, int greenPin, int bluePin, PwmController pwnController) {
    this->redPin = redPin;
    this->greenPin = greenPin;
    this->bluePin = bluePin;
    this->pwmController = pwmController;
    this->usePwmController = true;
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
        if (effectMode == 'P')
            PowerOnEffect();

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

    if(usePwmController)
    {
        pwmController.SetPin(redPin, red);
        pwmController.SetPin(greenPin, green);
        pwmController.SetPin(bluePin, blue);
    }
    else
    {
        analogWrite(redPin, red);
        analogWrite(greenPin, green);
        analogWrite(bluePin, blue);
    }

    if (red != 0 || green != 0 || blue != 0)
        SetRecentColor(red, green, blue);
}

void AnalogLedStrip::SetColorEffect(int red, int green, int blue) {
    if(usePwmController)
    {
        pwmController.SetPin(redPin, red);
        pwmController.SetPin(greenPin, green);
        pwmController.SetPin(bluePin, blue);
    }
    else
    {
        analogWrite(redPin, red);
        analogWrite(greenPin, green);
        analogWrite(bluePin, blue);
    }
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
    int maxValue = usePwmController ? 4096 : 255;

    if (power) {
        switch (colorId % 36) {
            case 0:
                SetColorEffect(maxValue, 0, 0); //Red
                break;
            case 12:
                SetColorEffect(0, maxValue, 0); //Green
                break;
            case 24:
                SetColorEffect(0, 0, maxValue); //Blue
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
    int maxValue = usePwmController ? 4096 : 255;

    if (power) {
        switch (colorId % 84) {
            case 0:
                SetColorEffect(maxValue, 0, 0); //Red
                break;
            case 12:
                SetColorEffect(0, maxValue, 0); //Green
                break;
            case 24:
                SetColorEffect(0, 0, maxValue); //Blue
                break;
            case 36:
                if(usePwmController) {
                    SetColorEffect(4096, 642, 0); //Orange
                }
                else {
                    SetColorEffect(255, 40, 0); //Orange
                }
                break;
            case 48:
                if(usePwmController) {
                    SetColorEffect(0, 3068, 4096); //Deep sky blue
                }
                else {
                    SetColorEffect(0, 191, 255); //Deep sky blue
                }
                break;
            case 60:
                SetColorEffect(maxValue, maxValue, 0); //Yellow
                break;
            case 72:
                if(usePwmController) {
                    SetColorEffect(1205, 0, 2088); //Indigo
                }
                else {
                    SetColorEffect(75, 0, 130); //Indigo
                }
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

        int maxValue = usePwmController ? 4096 : 255;

        if (effectCounter < maxValue) {
            if(colorId%3 == 0)
                SetColorEffect(maxValue-effectCounter,effectCounter,0);
            else if(colorId%3 == 1)
                SetColorEffect(0,maxValue-effectCounter,effectCounter);
            else if(colorId%3 == 2)
                SetColorEffect(effectCounter,0,maxValue-effectCounter);
            effectCounter += usePwmController ? 16 : 1;
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

void AnalogLedStrip::SetPowerFade(bool state) {
    //Saving the current effectMode
    prevEffectMode = effectMode;
    effectCounter = 0;

    //Setting effect mode:
    !state ? effectMode = 'p' : effectMode = 'P';

    //Setting power status:
    power = state;
}

void AnalogLedStrip::PowerOnEffect() {

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

void AnalogLedStrip::IREvent(long irCode) {
    switch(irCode) {
        case 0xFF02FD: //power
            SetPower(!GetPower());
            break;

        //All the colors:
        case 0xFF1AE5: //red
            if(GetPower())
                if(usePwmController)
                    SetColor(4096,0,0);
                else
                    SetColor(255,0,0);
            break;
        case 0xFF9A65: //green
            if(GetPower())
                if(usePwmController)
                    SetColor(0,4096,0);
                else
                    SetColor(0,255,0);
            break;
        case 0xFFA25D: //blue
            if(GetPower())
                if(usePwmController)
                    SetColor(0,0,4096);
                else
                    SetColor(0,0,255);
            break;
        case 0xFF22DD: //white
            if(GetPower())
                if(usePwmController)
                    SetColor(4096, 4096, 4096);
                else
                    SetColor(255,255,255);
            break;
        case 0xFF2AD5: //red1
            if(GetPower())
                if(usePwmController)
                    SetColor(4096, 2249, 0);
                else
                    SetColor(255,140,0);
            break;
        case 0xFF0AF5: //red2
            if(GetPower())
                if(usePwmController)
                    SetColor(4096, 643, 0);
                else
                    SetColor(255,40,0);
            break;
        case 0xFF38C7: //red3
            if(GetPower())
                if(usePwmController)
                    SetColor(4096, 2040, 0);
                else
                    SetColor(255,127,0);
            break;
        case 0xFF18E7: //red4
            if(GetPower())
                if(usePwmController)
                    SetColor(4096, 4096, 0);
                else
                    SetColor(255,255,0);
            break;
        case 0xFFAA55: //green1
            if(GetPower())
                if(usePwmController)
                    SetColor(819, 4096, 819);
                else
                    SetColor(51,255,51);
            break;
        case 0xFF8A75: //green2
            if(GetPower())
                if(usePwmController)
                    SetColor(4096, 4096, 4096);
                else
                    SetColor(819,255,255);
            break;
        case 0xFFB847: //green3
            if(GetPower())
                if(usePwmController)
                    SetColor(0, 2458, 2458);
                else
                    SetColor(0,153,153);
            break;
        case 0xFF9867: //green4
            if(GetPower())
                if(usePwmController)
                    SetColor(0, 1638, 1638);
                else
                    SetColor(0,102,102);
            break;
        case 0xFF926D: //blue1
            if(GetPower())
                if(usePwmController)
                    SetColor(819, 819, 4096);
                else
                    SetColor(51,51,255);
            break;
        case 0xFFB24D: //blue2
            if(GetPower())
                if(usePwmController)
                    SetColor(819, 0, 1638);
                else
                    SetColor(51,0,102);
            break;
        case 0xFF7887: //blue3
            if(GetPower())
                if(usePwmController)
                    SetColor(1638, 0, 1638);
                else
                    SetColor(102,0,102);
            break;
        case 0xFF58A7: //blue4
            if(GetPower())
                if(usePwmController)
                    SetColor(4096, 0, 4096);
                else
                    SetColor(255,0,255);
            break;
        case 0xFF12ED: //white1
            if(GetPower())
                if(usePwmController)
                    SetColor(4096, 3277, 3277);
                else
                    SetColor(255,204,204);
            break;
        case 0xFF32CD: //white2
            if(GetPower())
                if(usePwmController)
                    SetColor(4096, 3678, 3277);
                else
                    SetColor(255,229,204);
            break;
        case 0xFFF807: //white3
            if(GetPower())
                if(usePwmController)
                    SetColor(2458, 4096, 4096);
                else
                    SetColor(153,255,255);
            break;
        case 0xFFD827: //white4
            if(GetPower())
                if(usePwmController)
                    SetColor(3277, 4096, 4096);
                else
                    SetColor(204,255,255);
            break;

            //Color buttons:
        case 0xFF28D7: //red up
            if(GetPower())
                AlterColor(AnalogLedStrip::Red, 10);
            break;
        case 0xFF08F7: //red down
            if(GetPower())
                AlterColor(AnalogLedStrip::Red, -10);
            break;
        case 0xFFA857: //green up
            if(GetPower())
                AlterColor(AnalogLedStrip::Green, 10);
            break;
        case 0xFF8877: //green down
            if(GetPower())
                AlterColor(AnalogLedStrip::Green, -10);
            break;
        case 0xFF6897: //blue up
            if(GetPower())
                AlterColor(AnalogLedStrip::Blue, 10);
            break;
        case 0xFF48B7: //blue down
            if(GetPower())
                AlterColor(AnalogLedStrip::Blue, -10);
            break;

            //Speed:
        case 0xFFE817: //Quick
            if(GetPower())
                SetEffectSpeed(1);
            break;
        case 0xFFC837: //Slow
            if(GetPower())
                SetEffectSpeed(-1);
            break;

            //Effects:
        case 0xFF20DF: //jump3
            if(GetPower())
                Jump3();
            break;
        case 0xFFA05F: //jump7
            if(GetPower())
                Jump7();
            break;
        case 0xFF609F: //fade3
            if(GetPower())
                Fade3();
            break;
        case 0xFFE01F: //fade7
            if(GetPower())
                Fade7();
            break;
        case 0xFFD02F: //flash
            if(GetPower())
                Flash();
            break;
    }
}