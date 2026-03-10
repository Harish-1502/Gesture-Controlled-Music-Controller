# Gesture-Controlled-Music-Controller

This is a gesture-controlled music controller built using an ESP32, flex sensors, and a breadboard, with a desktop script that translates gestures into media control actions.

- Demo coming soon

# Why This Was Made

The goal of this project was to explore human–computer interaction using gestures and to build a hands-free way to control music playback. This project focuses on embedded systems, sensor input processing, and real-time interaction between hardware and software.

# Tech Stack

- Language: Arduino (C++)
- Microcontroller: ESP32
- Sensors: Flex sensors and MPU6050
- Communication: Serial 

# How It Was Made

- Sensors are connected to the ESP32 on a wearable glove to detect finger and hand movement
- Detected gestures are mapped to control commands such as play, pause, next, or previous

# Architecture Overview

**Input Layer**
- User performs a hand gesture
- Flex sensors detect changes in resistance and send analog values to the ESP32

**Processing Layer**
- The ESP32 processes raw sensor data
- Calibration and threshold logic is applied to identify valid gestures

**Command Layer**
- Each recognized gesture is mapped to a media control command

**Execution Layer**
- The code executes the corresponding media control action on the system

# Design Decisions

- Gesture detection is handled on the ESP32 to reduce latency
- Simple threshold-based gesture recognition was chosen for reliability and ease of debugging
  
# Limitations

- Gesture recognition is basic and depends on calibration accuracy
- Sensor readings can be affected by noise and hand positioning
- Limited to predefined gestures and media controls
- No graphical interface for configuration

# Future Improvements

- Add calibration for better accuracy
- Improve gesture recognition using more advanced filtering or machine learning
- Log sensor data for debugging and analysis
