//
// Created by Beast on 10/1/2016.
//

#ifndef ANALOGLEDSTRIP_ANALOGLEDSTRIP_H
#define ANALOGLEDSTRIP_ANALOGLEDSTRIP_H

#include "PwmController.h"

class AnalogLedStrip {

    private:
    //Fields:
        int redPin, greenPin, bluePin;
        int red, green, blue;
        int effectRed, effectGreen, effectBlue;
        int brightness, timerCounter;
        int effectSpeed, effectCounter, colorId;
        bool usePwmController, power;
        char effectMode, prevEffectMode;
        PwmController pwmController;
    //Methods:
        void SetRecentColor(int, int, int);
        void SetRecentColorEffect(int, int, int);
        void SetColorEffect(int, int, int);
    //Effects:
        void Jump3Effect();
        void Jump7Effect();
        void Fade3Effect();
        void Fade7Effect();
        void FlashEffect();
        void PowerOnEffect();

    public:
        enum Color { Red=0, Green=1, Blue=2 };
    //Constructors:
        AnalogLedStrip(int, int, int);
        AnalogLedStrip(int, int, int, PwmController); //
        AnalogLedStrip(int, int, int, int, int); // Constructor with saveAdress
    //Events:
        void TimerEvent();
        void SerialEvent(String);
        void IREvent(long irCode);
    //Basic functionality:
        void SetColor(int, int, int);
        void SetBrightness(int);
        void SetEffectSpeed(int);
    //Effects:
        void Jump3();
        void Jump7();
        void Fade3();
        void Fade7();
        void Flash();
        void AlterColor(Color, int);
    //Getter and setters:
        void SetPower(bool);
        void SetPowerFade(bool);
        bool GetPower();
        int GetRed();
        int GetGreen();
        int GetBlue();
};

#endif //ANALOGLEDSTRIP_ANALOGLEDSTRIP_H
