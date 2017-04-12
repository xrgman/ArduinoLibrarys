#include "VoltageSensor.h"

/*!
 * Constructor.
 * @param pinNumber - The number of the pin that the sensor is connected to.
 */
VoltageSensor::VoltageSensor(int pinNumber) {
    this->pinNumber = pinNumber;

    //Setting default resistor values for module from ebay:
    this->R1 = 30000.0;
    this->R2 = 7500.0;
    this->internalVoltage = 5.00;
}

/*!
 * Constructor, with ability to set different resistor values. Used for own made sensor.
 * @param pinNumber- The number of the pin that the sensor is connected to.
 * @param R1 - Value of the first resistor.
 * @param R2 - Value of the first resistor.
 */
VoltageSensor::VoltageSensor(int pinNumber, float R1, float R2, float internalVoltgae) {
    this->pinNumber = pinNumber;
    this->R1 = R1;
    this->R2 = R2;
    this->internalVoltage = internalVoltgae;
}

/*!
 * ReadSensor
 *
 * Fetch the current voltage from the connected powersource.
 * @return Voltage in (V) for the power source.
 */
float VoltageSensor::readSensor() {
    float value = analogRead(pinNumber);
    float vOut = (value * internalVoltage) / 1024.0;
    float vIn = vOut / (R2/(R1+R2));
    return vIn;
}
