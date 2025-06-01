#include "pump.h"

Pump::Pump() {
    pumpState = false;
}

void Pump::begin() {
    pinMode(RELAY_PIN, OUTPUT);
    digitalWrite(RELAY_PIN, HIGH);  // Relay ngược - HIGH là TẮT
}

void Pump::setState(bool state) {
    if (state != pumpState) {
        pumpState = state;
        updateRelay();
        
        if (state) {
            Serial.println("Bật bơm!");
        } else {
            Serial.println("Tắt bơm!");
        }
    }
}

void Pump::updateRelay() {
    digitalWrite(RELAY_PIN, pumpState ? LOW : HIGH);  // Relay ngược - LOW là BẬT, HIGH là TẮT
}

void Pump::autoControl(int soilMoisture) {
    if (soilMoisture < SOIL_MOISTURE_LOW && !pumpState) {
        setState(true);  // Bật bơm khi độ ẩm đất thấp
    } else if (soilMoisture > SOIL_MOISTURE_HIGH && pumpState) {
        setState(false); // Tắt bơm khi độ ẩm đất cao
    }
} 