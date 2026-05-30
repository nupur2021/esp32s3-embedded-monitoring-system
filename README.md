# Embedded HMI for Smart Energy Management using ESP32-S3

## Overview

This project implements a Human Machine Interface (HMI) using an ESP32-S3 display module to monitor and visualize real-time operating parameters of a smart energy management system.

The system receives data from various subsystems through CAN Bus and RS485 communication interfaces and displays the information on an 800×480 touch screen using LVGL.

### Monitored Systems

- Smart Inverter
- AC Controller
- DC Controller
- CCU (Central Control Unit)
- Battery System
- EV Charging System

---

## Features

- Real-time parameter monitoring
- 800×480 TFT touch display
- LVGL-based graphical user interface
- CAN Bus communication using ESP32 TWAI
- RS485 communication support
- Multi-screen dashboard navigation
- Battery monitoring
- Inverter monitoring
- AC/DC controller monitoring
- EV charging system monitoring
- Touch-enabled interface

---

## Hardware Used

- ESP32-S3 Display Module
- 800×480 TFT Touch Display
- Capacitive Touch Panel
- CAN Transceiver
- RS485 Transceiver
- Smart Inverter
- AC Controller
- DC Controller
- CCU

---

## Software Stack

- Arduino Framework
- C/C++
- LVGL
- ESP32 TWAI (CAN)
- SPI
- I2C (Wire)
- SquareLine Studio

---

## Display Screens

### Inverter Screen
- Output Voltage
- Output Current
- Power
- Frequency
- Operating Status

### AC Controller Screen
- AC Voltage
- AC Current
- Load Information
- Fault Status

### DC Controller Screen
- DC Voltage
- DC Current
- Power Status
- System Health

### Battery Screen
- Battery Voltage
- Charging Current
- State of Charge (SOC)
- Battery Health

### EV Screen
- Charging Status
- Charging Voltage
- Charging Current
- Energy Delivered

### CCU Screen
- Communication Status
- Controller Status
- System Overview
- Fault Monitoring

---

## Working Principle

1. ESP32-S3 initializes the display and touch interface.
2. LVGL loads the graphical user interface.
3. CAN Bus receives real-time data from the inverter and controllers.
4. RS485 communication exchanges system information.
5. Data is processed by the ESP32-S3.
6. The display is updated with live system parameters.
7. Users navigate between screens using the touch interface.

---

## Applications

- Smart Energy Management Systems
- Solar Power Plants
- EV Charging Stations
- Battery Energy Storage Systems (BESS)
- Industrial Automation
- Real-Time Monitoring Systems

---

## Future Improvements

- MQTT Cloud Connectivity
- Data Logging to SD Card
- OTA Firmware Updates
- Remote Diagnostics
- Alarm and Fault History
- Modbus RTU Support
