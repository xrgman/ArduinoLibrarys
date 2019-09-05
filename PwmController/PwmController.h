#ifndef PWMCONTROLLER_PWMCONTROLLER_H
#define PWMCONTROLLER_PWMCONTROLLER_H

#include "arduino.h";


#define MODE1 0x0
#define LED0_ON_L 0x6
#define PRE_SCALE 0xFE

class PwmController {

    public:
        PwmController(int address = 0x40);
        void Begin();
        void Reset();
        void SetPwmFreq(int);
        void SetPin(int, int);
        void setPwm(int, int, int);

    private:
        int address, sdaPin, sclPin;
        int readFromAddress(int);
        void writeToAddress(int, int);


};

#endif //PWMCONTROLLER_PWMCONTROLLER_H
