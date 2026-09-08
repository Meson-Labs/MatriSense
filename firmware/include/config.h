#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// ==========================================
// Hardware Pin Definitions (nRF52840 / Pro Micro)
// ==========================================
#define PIN_I2C_SDA         6   // P0.06
#define PIN_I2C_SCL         8   // P0.08
#define PIN_BUZZER          17  // P0.17 (ACEBOTT PWM Pin)

// ==========================================
// I2C Sensor Addresses
// ==========================================
#define I2C_ADDR_MPU6050    0x68
#define I2C_ADDR_MAX30100   0x57
#define I2C_ADDR_TMP117     0x48

// ==========================================
// System & Sampling Configuration
// ==========================================
#define SERIAL_BAUD_RATE    115200
#define SAMPLING_PERIOD_MS  1000    // Sample vitals every 1000 ms
#define SENSOR_READ_TIMEOUT 500     // I2C read timeout in ms

// ==========================================
// Alarm & Safety Thresholds
// ==========================================
#define CRITICAL_SPO2_MIN    90     // SpO2 below 90% triggers Red Alert
#define CRITICAL_HR_MIN      50     // Bradycardia threshold (BPM)
#define CRITICAL_HR_MAX      120    // Tachycardia threshold (BPM)
#define CRITICAL_TEMP_HIGH   38.0   // High fever threshold (°C)
#define APNEA_NO_MOTION_SEC  15     // Apnea timeout (seconds without chest movement)

// ==========================================
// Audio Alert Frequencies
// ==========================================
#define TONE_WARN_FREQ_HZ   1500   // Yellow Alert pitch (1.5 kHz)
#define TONE_ALARM_FREQ_HZ  2500   // Red Alert pitch (2.5 kHz)

#endif // CONFIG_H
