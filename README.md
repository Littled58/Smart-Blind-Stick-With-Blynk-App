---

# Smart Blind Stick with ESP8266 & Blynk IoT

## Overview

The Smart Blind Stick is an assistive IoT device designed to help visually impaired individuals navigate safely.
It detects nearby obstacles and identifies accidental falls using sensors. Alerts are sent through the Blynk app to notify family members or caregivers.

---

## Features

* Obstacle detection using ultrasonic sensor
* Fall detection using MPU6050 gyroscope/accelerometer
* Buzzer alert for nearby obstacles
* IoT alert notification via Blynk app
* Portable and low-power system
* ESP8266 microcontroller based design

---

## Hardware Components

* ESP8266 NodeMCU (microcontroller)
* Ultrasonic Sensor (HC-SR04)
* MPU6050 Gyroscope & Accelerometer
* Buzzer
* Connecting wires
* Power supply / Power bank

---

## Working Principle

1. ESP8266 powers up and connects to Wi-Fi.
2. Ultrasonic sensor continuously measures distance.
3. If an obstacle is detected within 50 cm, buzzer alerts the user.
4. MPU6050 monitors movement and tilt to identify falls.
5. If a fall is detected, ESP8266 triggers a notification through the Blynk IoT app.
6. System resumes monitoring automatically.

---

## Software Used

* Arduino IDE
* Blynk IoT Platform

---

## Libraries Required

* BlynkSimpleESP8266
* Wire.h (for I2C communication)
* MPU6050 library (optional − raw values used in code)

---

## Code

The code connects ESP8266 to Wi-Fi, reads sensor values, identifies obstacles/falls, and triggers buzzer and Blynk notifications.
*(Full code already provided in repository.)*

---

## How to Use

1. Upload code to ESP8266 using Arduino IDE
2. Configure Wi-Fi credentials and Blynk Auth Token in code
3. Connect components as per wiring diagram
4. Power the stick using a battery or power bank
5. Monitor alerts on Blynk mobile application

---

## Applications

* Assistive walking tool for visually impaired individuals
* Fall detection and safety alert system
* Can be extended for real-time GPS tracking and voice feedback

---

## Future Improvements

* GPS tracking for outdoor use
* Voice assistance feedback
* Vibration motor for silent alerts
* Waterproof casing and ergonomic stick handle

---

## License

This project is open-source for educational and research purposes.

---

Just tell me.
