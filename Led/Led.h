#ifndef LED_H
#define LED_H

#include "PwmController.h"

class Led {
    public:
        Led(int);
        Led(int, PwmController);
        void turnOn();
        void turnOff();
        void fadeLed();
        void changeBrightness(int);
        void setBrightness(int);
        int getBrightness();
        bool getLedFading();
        bool getLedOn();
    private:
        int LedPin, LedValue, maxBrightness, fadeSpeed;
        bool LedState, LedFading, usePwmController;
        PwmController pwmController;
};
#endif //LED_H
