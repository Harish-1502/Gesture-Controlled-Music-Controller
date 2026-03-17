# Gesture-Controlled Music Controller

An end-to-end wearable gesture-controlled media controller built with an ESP32, flex sensors, and an MPU6050.

It allows hand gestures to trigger media control actions such as play, pause, next, and previous track by processing real-time sensor data directly on the embedded firmware.

---

## Features

- Wearable gesture control interface
- Real-time sensor processing on ESP32
- Flex sensor finger movement detection
- MPU6050 motion sensing for gesture context
- Calibration-based gesture detection
- Modular firmware architecture
- State machine for device states (active, calibration, sleep)
- Low-latency gesture recognition
- Serial communication for command execution

---

## Architecture Overview

### Input Layer
User performs a hand gesture using a wearable glove with flex sensors and an MPU6050.

### Sensor Layer
Flex sensors measure finger bending and send analog values to the ESP32.  
The MPU6050 provides motion and orientation data.

### Processing Layer
The ESP32 processes raw sensor data and applies calibration thresholds to detect valid gestures.

### Gesture Mapping Layer
Recognized gestures are mapped to media control commands such as play, pause, next, and previous.

### Execution Layer
The command is transmitted to the host system to perform the corresponding media action.

---

## Design Decisions

- Gesture detection runs directly on the ESP32 to minimize latency
- Threshold-based recognition was chosen for reliability and easier debugging
- Modular firmware architecture separates sensors, gesture detection, and system state
- A state machine manages active, calibration, and sleep states
- Calibration logic improves gesture recognition accuracy

---

## Tech Stack

### Firmware
- ESP32
- PlatformIO framework
- C++

### Sensors
- Flex sensors
- MPU6050 (accelerometer + gyroscope)

### Communication
- Serial

---

## Setup

### Requirements

- ESP32 development board
- Flex sensors
- MPU6050 module
- Breadboard and wiring
- Arduino IDE

---

### Steps

1. Download the repository
2. Build the hardware setup based on the wiring diagram in the `docs` folder
3. Open the folder in your IDE and make sure that it has platformIO installed (I installed it my VS Code)
4. Open the serial monitor and run the main.cpp file.
5. Perform calibration to configure gesture thresholds

---

## Demo

https://github.com/user-attachments/assets/34057b68-e849-4ee3-acbe-c08592f69379

---

## Future Improvements

- Improved gesture recognition algorithms
- Sensor filtering to reduce noise
- Desktop configuration interface
- Wireless communication instead of serial
- Gesture logging and debugging tools
- Advanced gesture recognition using machine learning
