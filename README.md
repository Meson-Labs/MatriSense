# MatriSense On-Device AI Health Monitor

MatriSense is an Edge AI wearable for maternal and infant health monitoring in low-resource and offline environments.

## Repository Contents
- `/firmware`: Embedded PlatformIO C++ firmware for the nRF52840 board.
- `/mobile_app`: Flutter application for BLE streaming and CSV data exporting.
- `/diagnostics`: Standalone hardware test utilities for I2C and audio circuit validation.

## Quickstart Hardware Setup
1. Connect all I2C sensors (`MAX30100`, `MPU-6050`, `TMP117`) to `SDA (P0.06)` and `SCL (P0.08)`.
2. Connect the ACEBOTT Buzzer module to `GPIO P0.17`.
3. Flash `diagnostics/sensor_health_check/sensor_health_check.ino` to verify all components appear on the bus.
4. Deploy `/firmware` using PlatformIO or Arduino IDE.
