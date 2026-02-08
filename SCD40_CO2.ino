#include <Arduino.h>
#include <SensirionI2cScd4x.h>
#include <Wire.h>

SensirionI2cScd4x scd4x;
bool isLowPowerMode = false;

void printCurrentMode() {
    if (isLowPowerMode) {
        Serial.println(">>> CHE DO HIEN TAI: LOW POWER (30s/lan) <<<");
    } else {
        Serial.println(">>> CHE DO HIEN TAI: NORMAL (5s/lan) <<<");
    }
}

void setup() {
    Serial.begin(115200);
    Wire.begin();

    scd4x.begin(Wire, SCD41_I2C_ADDR_62);
    scd4x.stopPeriodicMeasurement();
    
    // Bắt đầu ở chế độ Normal mặc định
    scd4x.startPeriodicMeasurement();
    Serial.println("Cam bien SCD40 da san sang.");
    Serial.println("Nhap 'LOW' de tiet kiem dien (30s), 'NORMAL' de do nhanh (5s)");
    printCurrentMode();
}

void loop() {
    // Xử lý lệnh từ Serial Monitor
    if (Serial.available()) {
        String cmd = Serial.readStringUntil('\n');
        cmd.trim();

        if (cmd == "LOW" && !isLowPowerMode) {
            scd4x.stopPeriodicMeasurement();
            delay(500);
            scd4x.startLowPowerPeriodicMeasurement();
            isLowPowerMode = true;
            Serial.println("\n[He thong] Da chuyen sang LOW POWER MODE.");
            printCurrentMode();
        } 
        else if (cmd == "NORMAL" && isLowPowerMode) {
            scd4x.stopPeriodicMeasurement();
            delay(500);
            scd4x.startPeriodicMeasurement();
            isLowPowerMode = false;
            Serial.println("\n[He thong] Da chuyen sang NORMAL MODE.");
            printCurrentMode();
        }
    }

    // Đọc dữ liệu
    bool isDataReady = false;
    uint16_t error = scd4x.getDataReadyStatus(isDataReady);

    if (!error && isDataReady) {
        uint16_t co2 = 0;
        float temperature = 0.0f;
        float humidity = 0.0f;

        error = scd4x.readMeasurement(co2, temperature, humidity);

        if (!error && co2 != 0) {
            Serial.print("[Data] CO2: ");
            Serial.print(co2);
            Serial.print(" ppm | ");
            Serial.print("Temp: ");
            Serial.print(temperature);
            Serial.print(" C | ");
            Serial.print("Hum: ");
            Serial.print(humidity);
            Serial.println(" %");
        }
    }
    
    delay(100);
}