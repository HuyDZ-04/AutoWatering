#include "display.h"

static const uint8_t PROGMEM temp_icon[] = {
    0x18, 0x24, 0x24, 0x24, 0x24, 0x24, 0x18, 0x00
};

static const uint8_t PROGMEM humidity_icon[] = {
    0x00, 0x42, 0x24, 0x18, 0x18, 0x24, 0x42, 0x00
};

static const uint8_t PROGMEM soil_icon[] = {
    0x00, 0x3C, 0x42, 0x42, 0x42, 0x3C, 0x00, 0x00
};

static const uint8_t PROGMEM pump_icon[] = {
    0x00, 0x3E, 0x22, 0x3E, 0x22, 0x3E, 0x00, 0x00
};

static const uint8_t PROGMEM wifi_icon[] = {
    0x00, 0x3E, 0x41, 0x41, 0x3E, 0x08, 0x08, 0x00
};

Display::Display() : display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET) {
    initialized = false;
}

//quét địa chỉ I2C 
bool scanI2CAddress(byte address) {
    for (int i = 0; i < 3; i++) {  // Thử 3 lần
        Wire.beginTransmission(address);
        byte error = Wire.endTransmission();
        if (error == 0) {
            return true;
        }
        delay(10);
    }
    return false;
}

//quét địa chỉ I2C
void scanI2C() {
    Serial.println("\nQuét thiết bị I2C...");
    int deviceCount = 0;
    
    for (int scan = 0; scan < 3; scan++) {
        Serial.print("Lần quét ");
        Serial.println(scan + 1);
        
        for(byte address = 1; address < 127; address++) {
            if (scanI2CAddress(address)) {
                Serial.print("Thiết bị I2C tìm thấy tại địa chỉ 0x");
                if (address < 16) {
                    Serial.print("0");
                }
                Serial.println(address, HEX);
                deviceCount++;
            }
        }
        
        if (deviceCount > 0) {
            break;  // Nếu tìm thấy thiết bị, dừng quét
        }
        delay(100);  // Đợi trước lần quét tiếp theo
    }
    
    if (deviceCount == 0) {
        Serial.println("Không tìm thấy thiết bị I2C nào!");
        Serial.println("Vui lòng kiểm tra:");
        Serial.println("1. Kết nối dây SDA và SCL");
        Serial.println("2. Nguồn cấp cho OLED");
        Serial.println("3. Điện trở kéo lên (pull-up resistors)");
    } else {
        Serial.print("Tìm thấy ");
        Serial.print(deviceCount);
        Serial.println(" thiết bị I2C");
    }
}

bool Display::begin() {
    Serial.println("\nKhởi tạo màn hình OLED...");
    
    // Khởi tạo I2C 
    Wire.begin(SDA_PIN, SCL_PIN);
    Wire.setClock(100000);
    delay(100);  // Đợi I2C ổn định
    
    // Quét thiết bị I2C
    scanI2C();
    
    // khởi tạo OLED với các địa chỉ phổ biến
    uint8_t addresses[] = {0x3C, 0x3D};  // Các địa chỉ I2C phổ biến của OLED
    bool success = false;
    
    for (uint8_t addr : addresses) {
        Serial.print("\nThử địa chỉ 0x");
        Serial.println(addr, HEX);
        
        for (int i = 0; i < 3; i++) {
            if(display.begin(SSD1306_SWITCHCAPVCC, addr)) {
                Serial.print("✅ OLED đã khởi tạo thành công tại địa chỉ 0x");
                Serial.println(addr, HEX);
                initialized = true;
                success = true;
                
                // Hiển thị màn hình test
                display.clearDisplay();
                display.setTextSize(1);
                display.setTextColor(SSD1306_WHITE);
                display.setCursor(0, 0);
                display.println("OLED Test");
                display.println("Dia chi: 0x");
                display.print(addr, HEX);
                display.display();
                delay(2000);
                
                break;
            }
            delay(100);
        }
        
        if (success) break;
    }
    
    if (!success) {
        Serial.println("\n Không thể khởi tạo OLED!");
        Serial.println("Vui lòng kiểm tra:");
        Serial.println("1. Kết nối dây SDA và SCL");
        Serial.println("2. Nguồn cấp cho OLED");
        Serial.println("3. Địa chỉ I2C của OLED");
    }
    
    return success;
}

void Display::drawIcon(int x, int y, const uint8_t* icon, int width, int height) {
    display.drawBitmap(x, y, icon, width, height, SSD1306_WHITE);
}

void Display::drawHeader() {
    display.setCursor(0, 0);
    display.setTextSize(1);
    display.println("He thong tuoi cay");
    display.drawLine(0, 10, SCREEN_WIDTH, 10, SSD1306_WHITE);
}

void Display::drawTemperature(float temp) {
    drawIcon(0, 16, temp_icon, 8, 8);
    display.setCursor(12, 16);
    display.print("Nhiet do: ");
    display.print(temp, 1);
    display.println(" *C");
}

void Display::drawHumidity(float humidity) {
    drawIcon(0, 28, humidity_icon, 8, 8);
    display.setCursor(12, 28);
    display.print("Do am KK: ");
    display.print(humidity, 1);
    display.println(" %");
}

void Display::drawSoilMoisture(int moisture) {
    drawIcon(0, 40, soil_icon, 8, 8);
    display.setCursor(12, 40);
    display.print("Do am dat: ");
    display.print(moisture);
    display.println(" %");
    
    // Draw moisture bar
    int barWidth = map(moisture, 0, 100, 0, SCREEN_WIDTH - 12);
    display.drawRect(12, 50, SCREEN_WIDTH - 12, 4, SSD1306_WHITE);
    display.fillRect(12, 50, barWidth, 4, SSD1306_WHITE);
}

void Display::drawPumpStatus(bool state) {
    drawIcon(0, 56, pump_icon, 8, 8);
    display.setCursor(12, 56);
    display.print("Bom: ");
    display.println(state ? "BAT" : "TAT");
}

void Display::drawWifiStatus(bool connected) {
    drawIcon(SCREEN_WIDTH - 8, 56, wifi_icon, 8, 8);
}

void Display::update(float temperature, float humidity, int soilMoisture, bool pumpState, bool wifiConnected) {
    if (!initialized) return;
    
    display.clearDisplay();
    
    drawHeader();
    drawTemperature(temperature);
    drawHumidity(humidity);
    drawSoilMoisture(soilMoisture);
    drawPumpStatus(pumpState);
    drawWifiStatus(wifiConnected);
    
    display.display();
}

void Display::showMessage(const char* line1, const char* line2) {
    if (!initialized) return;
    
    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextSize(1);
    display.println(line1);
    
    if (line2 != nullptr) {
        display.setCursor(0, 16);
        display.println(line2);
    }
    
    display.display();
}

void Display::showLoading(const char* message) {
    if (!initialized) return;
    
    static int dots = 0;
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print(message);
    
    // Animate loading dots
    for (int i = 0; i < dots; i++) {
        display.print(".");
    }
    dots = (dots + 1) % 4;
    
    display.display();
}

void Display::showError(const char* message) {
    if (!initialized) return;
    
    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextSize(1);
    display.println("LOI!");
    display.setCursor(0, 16);
    display.println(message);
    display.display();
} 