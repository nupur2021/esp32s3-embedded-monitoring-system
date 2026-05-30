# Smart Inverter Monitoring HMI using ESP32-S3

## Overview

This project implements a Human Machine Interface (HMI) using an ESP32-S3 display module to monitor and visualize real-time operating parameters of a smart energy management system.

The system receives data from various subsystems through CAN Bus and RS485 communication interfaces and displays the information on an 800×480 touch screen using LVGL.

### Monitored Systems
- Smart Inverter
- AC Controller
- DC Controller
- CCU (Central Control Unit)
- Battery System
- EV Charging Parameters

---

## Features

- Real-time parameter monitoring
- 800×480 TFT touch display
- LVGL-based graphical user interface
- CAN Bus communication (ESP32 TWAI)
- RS485 communication support
- Multi-screen dashboard navigation
- Battery status monitoring
- Inverter performance monitoring
- AC/DC controller monitoring
- EV system monitoring
- Touch-enabled interface

---

## Hardware Used

- ESP32-S3 Display Module
- 800×480 TFT Display
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
- ESP32-S3
- LVGL
- ESP32 TWAI CAN
- SPI
- I2C (Wire)
- SquareLine Studio UI

---

## Communication Architecture

Smart Inverter / Controllers → CAN Bus / RS485 → ESP32-S3 → LVGL Display

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
- System Overview
- Communication Status
- Controller Status
- Fault Information

---

## Working Principle

1. ESP32-S3 initializes the display and touch interface.
2. LVGL loads the graphical user interface.
3. CAN Bus receives real-time data from inverter and controllers.
4. RS485 communication exchanges system information.
5. Data is processed by the ESP32-S3.
6. Display values are updated periodically.
7. Users navigate between screens using touch controls.

---

## Future Improvements

- MQTT cloud connectivity
- Data logging to SD card
- OTA firmware updates
- Remote diagnostics
- Alarm and fault history
- Modbus RTU support

---

## Applications

- Solar Power Plants
- Smart Inverters
- EV Charging Stations
- Battery Energy Storage Systems (BESS)
- Industrial Automation
- Energy Monitoring Systems

---

## Author

**Nupur Acharya**

Embedded Systems | ESP32 | CAN | RS485 | LVGL | Linux | C Programming

---

## License

MIT License
