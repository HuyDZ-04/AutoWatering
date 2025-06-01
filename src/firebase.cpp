#include "firebase.h"

FirebaseManager::FirebaseManager() {
}

void FirebaseManager::begin() {
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("Đang kết nối WiFi");
    
    int wifiAttempts = 0;
    while (WiFi.status() != WL_CONNECTED && wifiAttempts < 20) {
        delay(500);
        Serial.print(".");
        wifiAttempts++;
    }
    
    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\n✅ WiFi đã kết nối!");
        Serial.print("IP: ");
        Serial.println(WiFi.localIP());
        
        // Configure Firebase
        Firebase.reconnectWiFi(true);
        config.api_key = FIREBASE_AUTH;
        config.database_url = FIREBASE_HOST;
        config.signer.tokens.legacy_token = FIREBASE_AUTH;
        
        Firebase.begin(&config, &auth);
        Serial.println("Firebase đã kết nối!");
    } else {
        Serial.println("\n Không thể kết nối WiFi");
    }
}

bool FirebaseManager::updateData(float temperature, float humidity, int soilMoisture, bool pumpState) {
    if (!isConnected()) {
        Serial.println(" Không có kết nối WiFi - bỏ qua cập nhật Firebase");
        return false;
    }
    
    bool success = false;
    
    // Try to send data 3 times if failed
    for (int i = 0; i < 3; i++) {
        if (Firebase.setFloat(fbdo, "/cambien/nhietdo", temperature) &&
            Firebase.setFloat(fbdo, "/cambien/doam", humidity) &&
            Firebase.setInt(fbdo, "/cambien/doamdat", soilMoisture) &&
            Firebase.setBool(fbdo, "/maybom/trangthai", pumpState)) {
            success = true;
            break;
        }
        delay(300);
    }
    
    if (success) {
        Serial.println("Dữ liệu đã gửi lên Firebase!");
    } else {
        Serial.print(" Lỗi gửi Firebase: ");
        Serial.println(fbdo.errorReason());
    }
    
    return success;
}

bool FirebaseManager::getPumpRequest(bool& requestState) {
    if (!isConnected()) return false;
    
    if (Firebase.getBool(fbdo, "/maybom/yeucaubat")) {
        requestState = fbdo.boolData();
        return true;
    }
    return false;
} 