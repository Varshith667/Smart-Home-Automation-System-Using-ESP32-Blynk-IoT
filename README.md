# 🏠 Smart Home Automation System

> **Real-time IoT home security using ESP32 + Blynk Cloud**  
> Monitors gas leakage, fire, motion, and temperature/humidity — sends instant alerts to your phone and email.

[![Wokwi Simulation](https://img.shields.io/badge/Wokwi-Simulation-green?logo=arduino)](https://wokwi.com/projects/466557671389500417)
[![Blynk IoT](https://img.shields.io/badge/Blynk-IoT%20Cloud-blue)](https://blynk.cloud)
[![Platform](https://img.shields.io/badge/Platform-ESP32-red)](https://www.espressif.com/)
[![License](https://img.shields.io/badge/License-MIT-yellow)](LICENSE)

---

## 📸 Project Screenshots

| Wokwi Circuit | Blynk Web Dashboard |
|:---:|:---:|
| ![Pin Diagram](screenshots/Pin_diagram.png) | ![Web Dashboard](screenshots/Web_dashboard.png) |

| Mobile App Dashboard | Email Alert |
|:---:|:---:|
| ![Mobile App](screenshots/Mobile_app_dashboard.jpeg) | ![Email Alert](screenshots/Email_alert_message.png) |

| Serial Monitor | Blynk Datastreams |
|:---:|:---:|
| ![Serial Monitor](screenshots/Serial_monitor.png) | ![Datastreams](screenshots/Datastreams.png) |

> 📁 Place all screenshots inside a `screenshots/` folder in the repo root.

---

## 📋 Table of Contents

- [Overview](#-overview)
- [Features](#-features)
- [Components](#-components)
- [Circuit Wiring](#-circuit-wiring)
- [Blynk IoT Setup](#-blynk-iot-setup)
- [Code Setup](#-code-setup)
- [Running on Wokwi](#-running-on-wokwi)
- [How It Works](#-how-it-works)
- [Libraries](#-libraries)
- [Troubleshooting](#-troubleshooting)

---

## 🔍 Overview

This project is a smart home security system built on an **ESP32** microcontroller that continuously monitors the environment for hazards and intruders. All sensor data is streamed live to the **Blynk IoT cloud**, viewable from both the **web dashboard** and **mobile app**. When a threat is detected, the system:

- Activates the **relay** (for exhaust fan, sprinkler, or alarm)
- Sounds the **buzzer**
- Sends a **push notification + email** via Blynk

---

## ✨ Features

- 🔴 **Gas Leakage Detection** — triggers relay + buzzer when MQ-2 threshold is exceeded
- 🔥 **Fire/Flame Detection** — activates relay + buzzer on flame detection
- 🌡️ **Temperature & Humidity Monitoring** — live readings via DHT22 on Blynk gauges
- 🚨 **Over-Temperature Alert** — relay activates if temperature exceeds 50°C
- 🚶 **Motion Detection** — PIR sensor sends intrusion alert to Blynk
- 📧 **Email + Push Notifications** — instant alerts via Blynk event system
- 📊 **Live Dashboard** — web + mobile dashboard with gauges, LEDs, and value display
- 🔄 **Auto-reconnect** — syncs all virtual pins on Blynk reconnect

---

## 📦 Components

| Component | Quantity | Purpose |
|---|---|---|
| ESP32 DevKit C v4 | 1 | Main controller + WiFi |
| DHT22 Sensor | 1 | Temperature & humidity |
| PIR Motion Sensor | 1 | Intruder / motion detection |
| MQ-2 Gas Sensor | 1 | LPG / smoke / gas detection |
| Flame Sensor (IR) | 1 | Fire detection |
| Relay Module | 1 | Controls external device (fan/alarm) |
| Buzzer | 1 | Audio alert |

---

## 🔌 Circuit Wiring

### GPIO Pin Assignments

| Component | ESP32 Pin | Wire Color | Signal Type |
|---|---|---|---|
| DHT22 — DATA | GPIO 27 | Green | Digital |
| DHT22 — VCC | 3.3V | Red | Power |
| DHT22 — GND | GND | Black | Ground |
| PIR — OUT | GPIO 15 | Green | Digital |
| PIR — VCC | 3.3V | Red | Power |
| PIR — GND | GND | Black | Ground |
| Flame Sensor — DAT | GPIO 4 | Green | Digital |
| Flame Sensor — VCC | 3.3V | Red | Power |
| Flame Sensor — GND | GND | Black | Ground |
| MQ-2 Gas — AOUT | GPIO 34 | Green | Analog |
| MQ-2 Gas — VCC | 5V | Red | Power |
| MQ-2 Gas — GND | GND | Black | Ground |
| Relay — IN | GPIO 2 | Green | Digital |
| Relay — VCC | 5V | Red | Power |
| Relay — GND | GND | Black | Ground |
| Buzzer — (+) | GPIO 14 | Green | PWM |
| Buzzer — (−) | GND | Black | Ground |

> ⚠️ **Note:** GPIO 34 is input-only on ESP32 — ideal for analog reads. MQ-2 and Relay require **5V**, not 3.3V.

### Wokwi `diagram.json`

The `diagram.json` file in this repo contains the complete Wokwi circuit. Open the project directly at:  
👉 **https://wokwi.com/projects/466557671389500417**

---

## 📱 Blynk IoT Setup

### Step 1 — Sign Up
Create a free account at [https://blynk.cloud](https://blynk.cloud).

### Step 2 — Create a Template
1. Click **"Developer Zone" → "+ New Template"**
2. Name: `Smart Home Automation`
3. Hardware: `ESP32`, Connection: `WiFi`
4. Click **Save**

### Step 3 — Add Datastreams
Go to the **Datastreams** tab and add these 6 virtual pins exactly:

| Virtual Pin | Name | Data Type | Min | Max | Default |
|---|---|---|---|---|---|
| V1 | Gas Sensor | Integer | 0 | 1 | 0 |
| V3 | Flame Sensor | Integer | 0 | 1 | 0 |
| V4 | Motion Sensor | Integer | 0 | 1 | 0 |
| V5 | Temperature | Double | -40 | 80 | 0 |
| V6 | Humidity | Double | 0 | 100 | 0 |
| V7 | Relay Status | Integer | 0 | 1 | 0 |

### Step 4 — Create the Event (for alerts)
1. Go to **Events & Notifications** tab
2. Click **"+ Add New Event"**
3. Fill in:
   - **Event Code:** `warning_message` ← must be exactly this
   - **Event Name:** Warning Message
   - **Type:** Warning
4. Enable **"Send Email Notification"** ✅
5. Enable **"Push Notification"** ✅ (for mobile app alerts)
6. Click **Save**

> This is the event fired in code as: `Blynk.logEvent("warning_message", "Gas Leakage Detected!")`

### Step 5 — Build the Dashboard

**Web Dashboard** — add these widgets:
- 4× **LED widgets** → V1 (Gas), V3 (Flame), V4 (Motion), V7 (Relay)
- 2× **Gauge widgets** → V5 (Temperature, range -40 to 80), V6 (Humidity, range 0 to 100)
- 1× **Gauge widget** → V1 (Gas status, range 0 to 1)
- 1× **Value Display** → V5 (Temperature label)

**Mobile App Dashboard** — same layout, works identically from the Blynk app.

### Step 6 — Create a Device
1. Go to **Devices → "+ New Device" → "From Template"**
2. Select your template
3. Copy all 3 credentials:

```cpp
#define BLYNK_TEMPLATE_ID   "TMPLxxxxxxxx"
#define BLYNK_TEMPLATE_NAME "Smart Home Automation"
#define BLYNK_AUTH_TOKEN    "xxxxxxxxxxxxxxxxxxxxxx"
```

---

## 💻 Code Setup

### 1. Install Arduino Libraries

In Arduino IDE go to **Sketch → Include Library → Manage Libraries** and install:

| Library | Search Name |
|---|---|
| Blynk | `Blynk` |
| DHT sensor for ESPx | `DHT sensor library for ESPx` |

Or if using Wokwi, add this to `libraries.txt`:
```
DHT sensor library for ESPx
Blynk
```

### 2. Update Credentials in Code

Open `smart_home_security.ino` and replace the placeholders at the top:

```cpp
#define BLYNK_TEMPLATE_ID   "YOUR_TEMPLATE_ID"
#define BLYNK_TEMPLATE_NAME "Smart Home Automation"
#define BLYNK_AUTH_TOKEN    "YOUR_AUTH_TOKEN"

char ssid[] = "Wokwi-GUEST";  // For Wokwi simulator
char pass[] = "";              // No password needed in Wokwi
```

For a real ESP32 on your home WiFi, replace `ssid` and `pass` with your network credentials.

---

## 🧪 Running on Wokwi

1. Open the project: **https://wokwi.com/projects/466557671389500417**
2. Click the code editor and paste your updated `smart_home_security.ino`
3. Replace the Blynk credentials with your own
4. Click **▶ Play** to start simulation
5. Open your Blynk dashboard — data streams in live

**To trigger sensor events in Wokwi:**
- **DHT22** — click the sensor → adjust the temperature/humidity slider
- **PIR** — click the PIR sensor dome to toggle motion detection
- **Gas Sensor** — click the sensor → drag the "Gas" slider up
- **Flame Sensor** — click the sensor → toggle the flame state

---

## ⚙️ How It Works

### Alert Logic Summary

| Trigger | Condition | Action |
|---|---|---|
| Gas Leakage | `gas_output_value > 165` | Relay ON + Buzzer 1000Hz + Blynk alert |
| Flame Detected | `digitalRead(flame_pin) == HIGH` | Relay ON + Buzzer 550Hz + Blynk alert |
| Over Temperature | `temperature >= 50°C` | Relay ON + Blynk alert |
| Motion Detected | `digitalRead(PIR) == HIGH` | Blynk notification only |

All alerts use `Blynk.logEvent("warning_message", "...")` which triggers an **email** (from robot@blynk.cloud) and a **push notification** on the mobile app.

### Virtual Pin Flow

```
Sensor Reading  →  ESP32 GPIO  →  Blynk.virtualWrite(Vx)  →  Dashboard Widget
                                       ↓
                              Blynk.logEvent()  →  Email + Push Notification
```

---

## 📚 Libraries

```
DHT sensor library for ESPx  →  https://github.com/beegee-tokyo/DHTesp
Blynk                         →  https://github.com/blynkkk/blynk-library
WiFi, WiFiClient               →  Built into ESP32 Arduino core
```

---

## 🐛 Troubleshooting

| Problem | Fix |
|---|---|
| Blynk not connecting | Verify all 3 credentials: Template ID, Template Name, Auth Token |
| WiFi not connecting in Wokwi | Use `"Wokwi-GUEST"` with empty password — do not change |
| No email alerts | Ensure the event code is exactly `warning_message` (case-sensitive) |
| DHT22 shows `nan` | Check GPIO 27 wiring; DHTesp library must be used (not standard DHT) |
| Gas always triggering | Lower the threshold from `165` in code, or calibrate MQ-2 warmup |
| Relay not switching | Confirm relay is on 5V, not 3.3V |
| Push notifications missing | Enable notifications in Blynk app settings for your device |

---

## 📁 Repository Structure

```
smart-home-automation/
├── smart_home_security.ino    ← Main Arduino source code
├── diagram.json               ← Wokwi circuit diagram
├── libraries.txt              ← Wokwi library list
├── wokwi-project.txt          ← Wokwi project link
├── README.md                  ← This file
└── screenshots/
    ├── Pin_diagram.png
    ├── Web_dashboard.png
    ├── Mobile_app_dashboard.jpeg
    ├── Email_alert_message.png
    ├── Serial_monitor.png
    └── Datastreams.png
```

---

## 👤 Author

**Padam Shankara Bala Sai Varshith** 
B.Tech - Electronics and Communication Engineering
Project: Smart Home Automation  
Blynk Organization: My organization - 4511RH  
Simulated & tested on [Wokwi](https://wokwi.com) with [Blynk IoT Cloud](https://blynk.cloud)

## 📄 License

This project is open source under the [MIT License](LICENSE).
