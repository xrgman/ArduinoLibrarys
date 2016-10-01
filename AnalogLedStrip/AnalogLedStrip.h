//
// Created by Beast on 10/1/2016.
//

#ifndef ANALOGLEDSTRIP_ANALOGLEDSTRIP_H
#define ANALOGLEDSTRIP_ANALOGLEDSTRIP_H

class AnalogLedStrip {
    private:
        int redPin, greenPin, bluePin;
        int red, green, blue;
        bool power;
        int effectSpeed, effectCounter;
        char effectMode;
    public:
        AnalogLedStrip(int, int, int);
        AnalogLedStrip(int, int, int, int, int); // Constructor with saveAdress
        void SetColor(int, int, int);
};

#endif //ANALOGLEDSTRIP_ANALOGLEDSTRIP_H
