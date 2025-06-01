#include <Arduino.h>
#include "config.h"
#include "sensor.h"
#include "display.h"
#include "firebase.h"
#include "pump.h"

Sensor sensor;
Display display;
FirebaseManager firebase;
Pump pump;

unsigned long lastSensorReadTime = 0;
unsigned long lastFirebaseUpdateTime = 0;

void setup() {
    Serial.begin(115200);
    Serial.println("Khởi động hệ thống tuoi cây tự động...");
    
    // Khởi tạo các thành phần
    sensor.begin();
    display.begin();
    firebase.begin();
    pump.begin();
    
    // Hiển thị màn hình khởi động
    display.showMessage("He thong tuoi cay", "Da san sang!");
    Serial.println(" Hệ thống đã sẵn sàng!");
    delay(1000);
}

void loop() {
    unsigned long currentMillis = millis();
    
    // Đọc cảm biến theo khoảng thời gian đã định
    if (currentMillis - lastSensorReadTime >= SENSOR_READ_INTERVAL) {
        lastSensorReadTime = currentMillis;
        if (sensor.readSensors()) {
            display.update(
                sensor.getTemperature(),
                sensor.getHumidity(),
                sensor.getSoilMoisture(),
                pump.getState(),
                firebase.isConnected()
            );
        }
    }
    
    // Kiểm tra điều khiển từ Firebase
    bool pumpRequest;
    if (firebase.getPumpRequest(pumpRequest)) {
        pump.setState(pumpRequest);
    }
    
    // Cập nhật Firebase định kỳ
    if (currentMillis - lastFirebaseUpdateTime >= FIREBASE_UPDATE_INTERVAL) {
        lastFirebaseUpdateTime = currentMillis;
        firebase.updateData(
            sensor.getTemperature(),
            sensor.getHumidity(),
            sensor.getSoilMoisture(),
            pump.getState()
        );
    }
    
    // Tự động điều khiển bơm dựa vào độ ẩm đất
    if (firebase.isConnected()) {
        pump.autoControl(sensor.getSoilMoisture());
    }
    
    // Delay nhỏ để tránh quá tải CPU
    delay(100);
}