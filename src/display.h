#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include "config.h"

class Display {
public:
    Display();
    bool begin();
    void update(float temperature, float humidity, int soilMoisture, bool pumpState, bool wifiConnected);
    void showMessage(const char* line1, const char* line2 = nullptr);
    void showLoading(const char* message);
    void showError(const char* message);

private:
    Adafruit_SSD1306 display;
    bool initialized;
    void drawHeader();
    void drawTemperature(float temp);
    void drawHumidity(float humidity);
    void drawSoilMoisture(int moisture);
    void drawPumpStatus(bool state);
    void drawWifiStatus(bool connected);
    void drawIcon(int x, int y, const uint8_t* icon, int width, int height);
};

#endif 