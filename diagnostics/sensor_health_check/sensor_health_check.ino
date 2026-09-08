#include <Wire.h>

void setup() {
  Serial.begin(115200);
  while (!Serial);
  Wire.begin();
  
  Serial.println("\n=== MatriSense Hardware Health Diagnostics ===");
  scanI2CBus();
  testBuzzer();
}

void scanI2CBus() {
  byte error, address;
  int nDevices = 0;

  Serial.println("Scanning I2C Bus for Sensors...");
  for (address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0) {
      Serial.print(" [PASS] Found I2C Device at address: 0x");
      if (address < 16) Serial.print("0");
      Serial.print(address, HEX);

      if (address == 0x57) Serial.println(" (MAX30100 PulseOximeter)");
      else if (address == 0x68) Serial.println(" (MPU-6050 Motion Sensor)");
      else if (address == 0x48) Serial.println(" (TMP117 Precision Temp)");
      else Serial.println(" (Unknown Device)");

      nDevices++;
    }
  }
  if (nDevices == 0) {
    Serial.println(" [FAIL] No I2C devices detected! Check SDA/SCL pull-up resistors or voltage levels.");
  } else {
    Serial.println("I2C Scan Complete.\n");
  }
}

void testBuzzer() {
  Serial.println("Testing ACEBOTT Buzzer Circuit...");
  pinMode(17, OUTPUT);
  tone(17, 1000, 200);
  delay(300);
  tone(17, 2000, 200);
  Serial.println(" [PASS] Audio test finished.\n");
}

void loop() {
  // Idle after diagnostic sequence completes
}
