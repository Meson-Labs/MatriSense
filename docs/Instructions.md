Master Pinout 
TableA. Shared I2C Sensor BusAll three primary health sensors operate over a shared I2C bus powered by the MCU's regulated 3.3V rail.

(ACEBOTT Buzzer)The ACEBOTT passive buzzer module includes an onboard switching transistor and base resistor, allowing direct control from a low-power microcontroller pin using Pulse-Width Modulation (PWM).

Hardware Verification Test
Before flashing the full firmware/ project, upload the standalone diagnostic sketch to verify physical wiring integrity:
Bash# Flash the diagnostic scanner
pio run --target upload --environment nicenano --project-dir diagnostics/sensor_health_check

Expected serial output (115200 Baud):Plaintext=== MatriSense Hardware Health Diagnostics ===
Scanning I2C Bus for Sensors...
 [PASS] Found I2C Device at address: 0x48 (TMP117 Precision Temp)
 [PASS] Found I2C Device at address: 0x57 (MAX30100 PulseOximeter)
 [PASS] Found I2C Device at address: 0x68 (MPU-6050 Motion Sensor)
I2C Scan Complete.

Testing ACEBOTT Buzzer Circuit...
 [PASS] Audio test finished.
