#include <Arduino.h>
#include <Wire.h>
#include "MAX30100_PulseOximeter.h"
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_TMP117.h>
#include <bluefruit.h>

#define BUZZER_PIN 17

PulseOximeter pox;
Adafruit_MPU6050 mpu;
Adafruit_TMP117 tmp117;

BLEService matriService = BLEService(0x180D);
BLECharacteristic vitalsChar = BLECharacteristic(0x2A37);

void setup() {
  Serial.begin(115200);
  Wire.begin();
  pinMode(BUZZER_PIN, OUTPUT);

  // Initialize Sensors
  if (!pox.begin()) Serial.println("MAX30100 Init Failed!");
  if (!mpu.begin(0x68)) Serial.println("MPU6050 Init Failed!");
  if (!tmp117.begin(0x48)) Serial.println("TMP117 Init Failed!");

  // Initialize Bluefruit BLE Stack
  Bluefruit.begin();
  Bluefruit.setName("MatriSense-Node");
  
  matriService.begin();
  vitalsChar.setProperties(CHR_PROPS_NOTIFY | CHR_PROPS_READ);
  vitalsChar.setPermission(SECMODE_OPEN, SECMODE_OPEN);
  vitalsChar.setFixedLen(8);
  vitalsChar.begin();

  // Advertising
  Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
  Bluefruit.Advertising.addService(matriService);
  Bluefruit.Advertising.start(0);
}

void loop() {
  pox.update();
  
  static uint32_t lastReport = 0;
  if (millis() - lastReport > 1000) {
    lastReport = millis();

    sensors_event_t a, g, tempEvt;
    mpu.getEvent(&a, &g, &tempEvt);
    sensors_event_t skinTemp;
    tmp117.getEvent(&skinTemp);

    uint8_t hr = pox.getHeartRate();
    uint8_t spo2 = pox.getSpO2();
    float tempC = skinTemp.temperature;

    // Trigger Buzzer Alert if vitals are dangerous
    if (spo2 < 90 && spo2 > 0) {
      tone(BUZZER_PIN, 2000, 500); // 2kHz warning siren
    }

    // Packet structure: [HR, SpO2, Temp x 100 (2 bytes), AccelX, AccelY, AccelZ]
    uint8_t payload[8];
    payload[0] = hr;
    payload[1] = spo2;
    int16_t tPacked = (int16_t)(tempC * 100);
    payload[2] = (tPacked >> 8) & 0xFF;
    payload[3] = tPacked & 0xFF;
    payload[4] = (int8_t)(a.acceleration.x * 10);
    payload[5] = (int8_t)(a.acceleration.y * 10);
    payload[6] = (int8_t)(a.acceleration.z * 10);
    payload[7] = 0x00; // Reserved flag

    vitalsChar.notify(payload, sizeof(payload));
  }
}
