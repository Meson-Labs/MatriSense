#ifndef BLE_SERVICE_H
#define BLE_SERVICE_H

#include <Arduino.h>
#include <bluefruit.h>
#include "config.h"

// ==========================================
// Custom GATT Service & Characteristic UUIDs
// ==========================================
// Health Thermometer / Custom Vitals Service UUID: 0x180D
#define BLE_SERVICE_UUID        0x180D  
#define BLE_CHAR_VITALS_UUID    0x2A37  

// Device Name advertised over Bluetooth
#define BLE_DEVICE_NAME         "MatriSense-Node"

// ==========================================
// Packed Data Structure for BLE Notifications
// ==========================================
// Size: 8 Bytes total (fits within default 20-byte BLE ATT MTU)
#pragma pack(push, 1)
struct VitalsPacket {
    uint8_t  heartRate;      // Pulse BPM (0 - 255)
    uint8_t  spO2;           // Blood Oxygen % (0 - 100)
    int16_t  temperature;    // Body Temp in °C scaled by 100 (e.g., 3685 = 36.85°C)
    int8_t   accelX;         // X-axis accel scaled by 10 (m/s²)
    int8_t   accelY;         // Y-axis accel scaled by 10 (m/s²)
    int8_t   accelZ;         // Z-axis accel scaled by 10 (m/s²)
    uint8_t  alertStatus;    // 0: Safe (Green), 1: Caution (Yellow), 2: Critical (Red)
};
#pragma pack(pop)

// ==========================================
// Function Prototypes
// ==========================================
void initBLE();
void startAdvertising();
void sendVitalsNotification(const VitalsPacket& packet);

#endif // BLE_SERVICE_H
