#ifndef VOLTAGESENSOR_H
#define VOLTAGESENSOR_H

#include "arduino.h";

class VoltageSensor {
    public:
        VoltageSensor(int);
        VoltageSensor(int, float, float, float);
        float readSensor();
    private:
        float R1, R2;
        int pinNumber, internalVoltage;
};
#endif //VOLTAGESENSOR_H
