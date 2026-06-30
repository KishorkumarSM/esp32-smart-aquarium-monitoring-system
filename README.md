# ESP32 Smart Aquarium Monitoring and Feeding System

## Overview
This project is an ESP32-based smart aquarium prototype developed to monitor basic aquarium conditions and automate fish feeding.

The system uses an ESP32 microcontroller to read aquarium sensor values, control a servo-based fish feeder, provide manual feeding through a WiFi web page, and display system status through serial logs.

## Features
- Scheduled fish feeding using a servo motor
- Manual feed trigger through a local WiFi web page
- Temperature monitoring using an analogue temperature sensor concept
- pH monitoring using an analogue pH sensor concept
- Alert LED for abnormal temperature or pH values
- Serial monitor logs for testing and debugging
- Simple web dashboard showing sensor readings and feeder status

## Tools and Technologies
- ESP32
- Arduino IDE
- C/C++
- Servo motor
- Temperature sensor
- pH sensor
- WiFi web server
- Basic sensor calibration and troubleshooting

## Hardware Used
- ESP32 development board
- Servo motor for food dispensing
- Analogue temperature sensor
- Analogue pH sensor module
- LED indicator
- Jumper wires and breadboard/prototype setup

## What I Worked On
- Implemented ESP32-based sensor reading and actuator control
- Used a servo motor to automate fish feeding
- Added WiFi-based manual control through a local web page
- Monitored temperature and pH values using analogue sensor inputs
- Added basic alert logic for abnormal aquarium conditions
- Tested serial logs and sensor readings during development

## Current Status
This repository is a recreated and documented version of an older academic/personal project. The original project files are no longer available, so the code and documentation are being rebuilt as a clean public portfolio version.

## Future Improvements
- Add real calibrated pH sensor testing results
- Add waterproof temperature sensor support
- Add Blynk/ThingSpeak/MQTT cloud logging
- Add mobile notifications for abnormal water conditions
- Add enclosure and wiring photos
- Add feeding history storage
