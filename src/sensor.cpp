#include "sensor.h"

Sensor::Sensor() : dht(DHTPIN, DHTTYPE) {
    temperature = 0;
    humidity = 0;
    soilMoisture = 0;
}

void Sensor::begin() {
    dht.begin();
    pinMode(SOIL_MOISTURE_AO, INPUT);
    pinMode(SOIL_MOISTURE_DO, INPUT);
    Serial.println(" Cảm biến đã khởi tạo");
}

bool Sensor::readSensors() {
    float newHumidity = dht.readHumidity();
    float newTemperature = dht.readTemperature();
    int newSoilMoisture = analogRead(SOIL_MOISTURE_AO);

    if (isnan(newHumidity) || isnan(newTemperature)) {
        Serial.println(" Lỗi đọc cảm biến DHT!");
        return false;
    }
    
    humidity = newHumidity;
    temperature = newTemperature;
    // Đảo ngược giá trị độ ẩm đất: 4095 (không có nước) -> 0%, 0 (nhiều nước) -> 100%
    soilMoisture = map(newSoilMoisture, 4095, 0, 0, 100);
    
    Serial.print(" Nhiệt độ: ");
    Serial.print(temperature);
    Serial.print("°C |  Độ ẩm không khí: ");
    Serial.print(humidity);
    Serial.print("% |  Độ ẩm đất: ");
    Serial.print(soilMoisture);
    Serial.println("%");
    return true;
} 