#ifndef FIREBASE_H
#define FIREBASE_H

#include <Arduino.h>
#include <WiFi.h>
#include <FirebaseESP32.h>
#include "config.h"

class FirebaseManager {
public:
    FirebaseManager();
    void begin();
    bool updateData(float temperature, float humidity, int soilMoisture, bool pumpState);
    bool getPumpRequest(bool& requestState);
    bool isConnected() { return WiFi.status() == WL_CONNECTED; }

private:
    FirebaseData fbdo;
    FirebaseAuth auth;
    FirebaseConfig config;
};

#endif 