#ifndef CONFIG_H
#define CONFIG_H

// WiFi và Firebase 
#define WIFI_SSID ""
#define WIFI_PASSWORD ""
#define FIREBASE_HOST ""
#define FIREBASE_AUTH ""

// GPIO 
#define RELAY_PIN 5   
#define DHTPIN 32     
#define DHTTYPE DHT11 

// Soil Moisture Sensor
#define SOIL_MOISTURE_AO 34  // Analog output
#define SOIL_MOISTURE_DO 35  // Digital output

// I2C OLED
#define SDA_PIN 21  // SDA pin for I2C
#define SCL_PIN 22  // SCL pin for I2C

// OLED 
#define SCREEN_WIDTH 128    // OLED width
#define SCREEN_HEIGHT 64    // OLED height
#define OLED_RESET -1       // Reset pin (not used)
#define SCREEN_ADDRESS 0x3C // I2C address of OLED

const unsigned long SENSOR_READ_INTERVAL = 2000;    // 2 seconds
const unsigned long FIREBASE_UPDATE_INTERVAL = 10000; // 10 seconds

// Pump Control 
#define SOIL_MOISTURE_LOW 30   // Turn pump on 
#define SOIL_MOISTURE_HIGH 40  // Turn pump off 

#endif 