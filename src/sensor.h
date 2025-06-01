#ifndef SENSOR_H
#define SENSOR_H

#include <Arduino.h>
#include <DHT.h>
#include "config.h"

class Sensor {
public:
    Sensor();
    void begin();
    bool readSensors();
    float getTemperature() { return temperature; }
    float getHumidity() { return humidity; }
    int getSoilMoisture() { return soilMoisture; }

private:
    DHT dht;
    float temperature;
    float humidity;
    int soilMoisture;
};

#endif 