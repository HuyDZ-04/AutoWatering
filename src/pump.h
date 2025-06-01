#ifndef PUMP_H
#define PUMP_H

#include <Arduino.h>
#include "config.h"

class Pump {
public:
    Pump();
    void begin();
    void setState(bool state);
    bool getState() { return pumpState; }
    void autoControl(int soilMoisture);

private:
    bool pumpState;
    void updateRelay();
};

#endif 