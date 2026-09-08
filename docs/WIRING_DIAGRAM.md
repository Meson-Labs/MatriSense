# MatriSense Wearable Hardware Wiring Diagram & Pinout Guide

This document details the physical pin connections, power routing, and signal bus architecture for the **MatriSense** Edge AI health monitoring prototype built around the **Nordic Semiconductor nRF52840** (Pro Micro / nice!nano v2 form factor).

---

## 1. System Schematic Overview

```text
                               +-------------------------------------+
                               |     LiPo Battery (3.7V 200mAh)      |
                               +------------------+------------------+
                                                  |
                                                  | (BAT+ / GND)
                                                  v
                               +-------------------------------------+
                               |   Pro Micro nRF52840 (nice!nano v2)  |
                               +--+------------+------------+-----+--+
                                  | 3.3V       | GND        |     |
                                  |            |            |     |
               +------------------+------------+            |     |
               |                  |                         |     |
               v                  v                         v     v
       +---------------+  +---------------+  +---------------+   +-------------------+
       |   MPU-6050    |  |   MAX30100    |  |    TMP117     |   |  ACEBOTT Buzzer   |
       | Motion Sensor |  | Pulse Oximeter|  |  Temp Sensor  |   |    Alert Module   |
       +-------+-------+  +-------+-------+  +-------+-------+   +---------+---------+
               |                  |                  |                     |
               +------------------+------------------+                     |
               |                                     |                     |
               | I2C Bus (SDA / SCL)                 |                     | Signal / PWM
               v                                     v                     v
   [ GPIO P0.06 (SDA) ]                   [ GPIO P0.08 (SCL) ]      [ GPIO P0.17 (PWM) ]
