#include "ble_service.h"

// BLE Service & Characteristic Instances
BLEService        matriService = BLEService(BLE_SERVICE_UUID);
BLECharacteristic vitalsChar   = BLECharacteristic(BLE_CHAR_VITALS_UUID);

// Initialize Bluetooth Low Energy Stack
void initBLE() {
    // 1. Initialize Adafruit Bluefruit BLE library
    Bluefruit.begin();
    Bluefruit.setTxPower(4); // Max power +4 dBm for strong signal in clinics
    Bluefruit.setName(BLE_DEVICE_NAME);

    // 2. Configure MatriSense GATT Service
    matriService.begin();

    // 3. Configure Vitals Characteristic (Read + Notify enabled)
    vitalsChar.setProperties(CHR_PROPS_READ | CHR_PROPS_NOTIFY);
    vitalsChar.setPermission(SECMODE_OPEN, SECMODE_OPEN);
    vitalsChar.setFixedLen(sizeof(VitalsPacket)); // Fixed 8-byte payload
    vitalsChar.begin();
}

// Start BLE Advertising so the mobile app can discover MatriSense
void startAdvertising() {
    // Advertising packet configuration
    Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
    Bluefruit.Advertising.addTxPower();
    
    // Include MatriSense Service UUID in the advertisement payload
    Bluefruit.Advertising.addService(matriService);
    
    // Secondary scan response packet with device name
    Bluefruit.ScanResponse.addName();

    /* 
     * Start Advertising:
     * - Fast advertising interval: 20 ms (first 30 seconds for quick pairing)
     * - Slow advertising interval: 152.5 ms (power-saving timeout fallback)
     * - 0 = Don't stop advertising automatically
     */
    Bluefruit.Advertising.restartOnDisconnect(true);
    Bluefruit.Advertising.setInterval(32, 244); // Units of 0.625 ms
    Bluefruit.Advertising.setFastTimeout(30);    // Fast mode for 30 seconds
    Bluefruit.Advertising.start(0);              // 0 = Continuous advertising
}

// Send non-blocking binary notification over BLE to connected app
void sendVitalsNotification(const VitalsPacket& packet) {
    // Only transmit if a central device (Flutter Mobile App) is connected
    if (Bluefruit.connected() && vitalsChar.notifyEnabled()) {
        vitalsChar.notify((const uint8_t*)&packet, sizeof(VitalsPacket));
    }
}
