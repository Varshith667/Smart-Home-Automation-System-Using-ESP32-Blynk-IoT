<div align="center">

# 🏠 Smart Home Automation System
### ESP32 · Blynk IoT Cloud · Multi-Sensor Real-Time Monitoring

[![Platform](https://img.shields.io/badge/Platform-ESP32-blue?style=for-the-badge&logo=espressif)](https://www.espressif.com/)
[![Framework](https://img.shields.io/badge/Framework-Arduino-00979D?style=for-the-badge&logo=arduino)](https://www.arduino.cc/)
[![Cloud](https://img.shields.io/badge/Cloud-Blynk%20IoT-23C48E?style=for-the-badge)](https://blynk.io/)
[![Simulator](https://img.shields.io/badge/Simulator-Wokwi-FF5722?style=for-the-badge)](https://wokwi.com/)
[![Language](https://img.shields.io/badge/Language-C%2B%2B-00599C?style=for-the-badge&logo=cplusplus)](https://isocpp.org/)

> A fully cloud-connected IoT security and environment monitoring system that detects motion intrusion, gas leakage, fire, and hazardous temperature in real-time — triggering instant alerts, relay-controlled emergency responses, and live dashboards on both mobile and web.

</div>

---

## 📋 Table of Contents

- [Overview](#-overview)
- [System Architecture](#-system-architecture)
- [Features](#-features)
- [Hardware Components](#-hardware-components)
- [Pin Configuration](#-pin-configuration)
- [Circuit Diagram](#-circuit-diagram)
- [Software Stack](#-software-stack)
- [Blynk IoT Setup](#-blynk-iot-setup)
- [Virtual Pin Mapping](#-virtual-pin-mapping)
- [Alert Thresholds](#-alert-thresholds)
- [Project Structure](#-project-structure)
- [Getting Started](#-getting-started)
- [Dashboard Preview](#-dashboard-preview)
- [Results](#-results)
- [Author](#-author)

---

## 🔍 Overview

The **Smart Home Automation System** is an IoT-based real-time monitoring and automated protection platform built on the **ESP32 microcontroller** and **Blynk IoT Cloud**. The system continuously polls an array of environmental and security sensors, streams live telemetry to the Blynk cloud, and automatically triggers warning events — including email and push notifications and relay-controlled actuators — when hazardous conditions are detected.

Designed and simulated entirely in **Wokwi**, the project demonstrates a production-ready IoT pipeline covering sensor interfacing, Wi-Fi connectivity, cloud data streaming, event-driven alerting, and hardware actuation — all within a single embedded firmware.

---

## 🏗 System Architecture

```
┌─────────────────────────────────────────────────────────────────┐
│                        SENSOR LAYER                             │
│  PIR Motion │ Flame Sensor │ MQ2 Gas │ DHT22 Temp/Humidity      │
└────────────────────────┬────────────────────────────────────────┘
                         │ GPIO Reads (Digital / Analog)
                         ▼
┌─────────────────────────────────────────────────────────────────┐
│                     ESP32 FIRMWARE                              │
│   BlynkTimer (1000ms) → readSensors() → Threshold Check         │
│       │                                           │             │
│       ▼                                           ▼             │
│  Relay / Buzzer Control               Blynk.virtualWrite()      │
└─────────────────────────────────────────────────────────────────┘
                         │ Wi-Fi / TCP-IP / HTTP
                         ▼
┌─────────────────────────────────────────────────────────────────┐
│                    BLYNK IoT CLOUD (BLR1)                       │
│  Data Streams │ Event Logs │ Email / Push Notifications         │
└──────────────────────┬──────────────────────────────────────────┘
                       │
          ┌────────────┴────────────┐
          ▼                         ▼
  ┌───────────────┐       ┌──────────────────┐
  │  Mobile App   │       │   Web Dashboard  │
  │ (Android/iOS) │       │  (Blynk Console) │
  └───────────────┘       └──────────────────┘
```

---

## ✨ Features

| Feature | Description |
|---|---|
| 🚶 **Motion Detection** | PIR sensor detects intrusion and triggers instant cloud alert |
| 🔥 **Fire Detection** | Flame sensor monitors for fire; activates relay + buzzer alarm |
| 💨 **Gas Leakage Detection** | MQ2 analog sensor detects combustible gas above safety threshold |
| 🌡️ **Temperature Monitoring** | DHT22 tracks ambient temperature with over-temperature alerting |
| 💧 **Humidity Monitoring** | Real-time humidity readings streamed live to Blynk dashboard |
| 📲 **Mobile & Web Dashboard** | Live gauges, LED status indicators on Blynk app and web console |
| 📧 **Cloud Notifications** | Automated email and push alerts on any hazard event via Blynk |
| ⚡ **Relay Emergency Control** | Automatic relay activation on gas / fire / over-temperature events |
| 🔔 **Buzzer Alarm** | Audible alarm using `tone()` when any hazard condition is active |
| 🔄 **Auto-Reconnect Sync** | `BLYNK_CONNECTED()` re-syncs all virtual pins on cloud reconnect |

---

## 🔧 Hardware Components

| Component | Model | Quantity | Purpose |
|---|---|---|---|
| Microcontroller | ESP32 DevKit C V4 | 1 | Main processing unit + Wi-Fi |
| Temperature & Humidity | DHT22 (AM2302) | 1 | Ambient environment sensing |
| Gas Sensor | MQ2 | 1 | Combustible gas detection (analog) |
| Motion Sensor | PIR (HC-SR501) | 1 | Intrusion / presence detection |
| Flame Sensor | IR Flame Module | 1 | Fire / open-flame detection |
| Relay Module | 5V Single Channel | 1 | Emergency load switching |
| Active Buzzer | — | 1 | Audible hazard alarm |

---

## 📌 Pin Configuration

| Sensor / Actuator | ESP32 GPIO | Signal Type |
|---|---|---|
| PIR Motion Sensor | GPIO 15 | Digital INPUT |
| Flame Sensor | GPIO 4 | Digital INPUT |
| MQ2 Gas Sensor | GPIO 34 | Analog INPUT (ADC1) |
| DHT22 Data | GPIO 27 | Single-Wire (DHTesp) |
| Relay Module | GPIO 2 | Digital OUTPUT |
| Active Buzzer | GPIO 14 | PWM OUTPUT (`tone()`) |

> **Note:** GPIO 34 is input-only on the ESP32 — no internal pull-up/down. Ideal for analog reads from MQ2. The MQ2 and Relay module both require **5V** power, not 3.3V.

---

## 🔌 Circuit Diagram

The complete circuit was designed and validated in **Wokwi Simulator**.

![Circuit Diagram](Pin diagram.png)

The `Diagram.json` file in `Circuit_Diagram/` can be directly imported into [wokwi.com](https://wokwi.com) to reproduce the full interactive simulation.

🔗 **Live Simulation:** [https://wokwi.com/projects/466557671389500417](https://wokwi.com/projects/466557671389500417)

---

## 🖥 Software Stack

| Layer | Technology |
|---|---|
| Firmware Language | C++ (Arduino Framework) |
| IDE | Arduino IDE |
| Simulation | Wokwi Online Simulator |
| IoT Cloud | Blynk IoT (Template-based, Region: BLR1) |
| Communication | Wi-Fi → TCP/IP → HTTP (Blynk protocol) |
| Version Control | Git / GitHub |

### Arduino Libraries

```cpp
#include <WiFi.h>              // ESP32 Wi-Fi driver
#include <WiFiClient.h>        // TCP client for Blynk
#include <BlynkSimpleEsp32.h>  // Blynk IoT cloud library
#include "DHTesp.h"            // DHT22 temperature/humidity driver
```

---

## ☁️ Blynk IoT Setup

### Step 1 — Create Account & Template
1. Sign up at [blynk.cloud](https://blynk.cloud)
2. Go to **Developer Zone → "+ New Template"**
3. Name: `Smart Home Automation` | Hardware: `ESP32` | Connection: `WiFi`
4. Save and note your `BLYNK_TEMPLATE_ID` and `BLYNK_TEMPLATE_NAME`

### Step 2 — Add Data Streams
In the **Datastreams** tab, create 6 virtual pins (see table in next section).

### Step 3 — Create the Warning Event
1. Go to **Events & Notifications → "+ Add New Event"**
2. Set **Event Code** to exactly: `warning_message`
3. Enable ✅ **Send Email Notification** and ✅ **Push Notification**
4. Save — this is what fires `Blynk.logEvent("warning_message", "...")` in the firmware

### Step 4 — Build Dashboards
Add widgets to the **Web Dashboard** and **Mobile App Dashboard**:
- **LED widgets** → V1 (Gas), V3 (Flame), V4 (Motion), V7 (Relay)
- **Gauge widgets** → V5 (Temperature, −40 to 80), V6 (Humidity, 0 to 100)
- **Gauge widget** → V1 (Gas Status, 0 to 1)
- **Value Display** → V5 (Temperature label)

### Step 5 — Create a Device
Go to **Devices → "+ New Device" → From Template** → copy your `BLYNK_AUTH_TOKEN`

---

## 🗂 Virtual Pin Mapping

| Virtual Pin | Sensor / Actuator | Data Type | Range | Widget |
|---|---|---|---|---|
| V1 | MQ2 Gas Sensor | Integer | 0 – 1 | Gauge / LED |
| V3 | Flame Sensor | Integer | 0 – 1 | LED |
| V4 | PIR Motion Sensor | Integer | 0 – 1 | LED |
| V5 | DHT22 Temperature | Double | −40 – 80 | Gauge |
| V6 | DHT22 Humidity | Double | 0 – 100 | Gauge |
| V7 | Relay Status | Integer | 0 – 1 | LED / Button |

---

## ⚠️ Alert Thresholds

| Hazard | Condition | Action |
|---|---|---|
| Gas Leakage | Mapped ADC value > **165** (raw ~3628) | `logEvent` + Relay ON + Buzzer 1000 Hz |
| Over-Temperature | Temperature ≥ **50 °C** | `logEvent` + Relay ON |
| Motion Intrusion | PIR = **HIGH** | `logEvent` — alert sent on state change only |
| Fire Detected | Flame Sensor = **HIGH** | `logEvent` + Relay ON + Buzzer 550 Hz |

State-change tracking (`pirState`) prevents repeated cloud event spam on continuous PIR HIGH readings. Gas and flame loops hold the relay/buzzer active until the hazard condition clears.

---

## 📁 Project Structure

```
Smart_Home_Automation/
│
├── 01_Code/
│   └── Smart_Home_Automation/
│       └── Smart_Home_Automation.ino        # Main firmware
│
├── Circuit_Diagram/
│   ├── Circuit_Diagram.png                  # Visual schematic (Wokwi)
│   └── Diagram.json                         # Wokwi-importable circuit file
│
├── Cloud_Blynk_IoT/
│   ├── Mobile_App_Dashboard.jpeg            # Mobile app screenshot
│   ├── Web_Dashboard.png                    # Web console screenshot
│   ├── Datastreams_Setup.png                # Virtual pin data stream config
│   └── Event_Setup.png                      # Blynk event configuration
│
├── Results/
│   ├── Serial_Monitor_Output.png            # Live serial monitor output
│   └── Email_Alert_Notification.png         # Email alert sample
│
└── README.md
```

---

## 🚀 Getting Started

### Prerequisites

- [Arduino IDE](https://www.arduino.cc/en/software) (v2.x recommended)
- ESP32 board package via Boards Manager:
  ```
  https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
  ```
- Blynk account at [blynk.cloud](https://blynk.cloud)

### Library Installation

Open Arduino IDE → **Tools → Manage Libraries** and install:

| Library | Author |
|---|---|
| `Blynk` | Volodymyr Shymanskyy |
| `DHT sensor library for ESPx` | beegee-tokyo |

### Configuration

Open `Smart_Home_Automation.ino` and update your credentials:

```cpp
#define BLYNK_TEMPLATE_ID   "YOUR_TEMPLATE_ID"
#define BLYNK_TEMPLATE_NAME "Smart Home Automation"
#define BLYNK_AUTH_TOKEN    "YOUR_AUTH_TOKEN"

char ssid[] = "YOUR_WIFI_SSID";    // Use "Wokwi-GUEST" for Wokwi simulator
char pass[] = "YOUR_WIFI_PASSWORD"; // Leave "" for Wokwi
```

> ⚠️ **Security:** Never commit real credentials to a public repository. Use `secrets.h` or environment variables for production deployments.

### Upload & Run

1. Connect ESP32 via USB
2. Select **Board:** `ESP32 Dev Module` and the correct **Port**
3. Click **Upload** (`Ctrl + U`)
4. Open **Serial Monitor** at `115200 baud` to observe live sensor readings
5. Open Blynk mobile app or web console to view the live dashboard

### Simulate in Wokwi

1. Open the live project: [wokwi.com/projects/466557671389500417](https://wokwi.com/projects/466557671389500417)  
   — OR — go to [wokwi.com](https://wokwi.com) → **New Project → ESP32** → import `Diagram.json`
2. Paste firmware into `sketch.ino` and update Blynk credentials
3. Click ▶ **Start Simulation**
4. Interact with sensors:
   - **DHT22** — click sensor → adjust temperature/humidity slider
   - **PIR** — click sensor dome to toggle motion HIGH/LOW
   - **MQ2 Gas** — click sensor → drag the gas concentration slider up
   - **Flame Sensor** — click sensor → toggle flame state

---

## 📊 Dashboard Preview

| Mobile App Dashboard | Web Dashboard |
|:---:|:---:|
| ![Mobile Dashboard](Mobile app dashboard.jpeg) | ![Web Dashboard](Web dashboard.png) |

| Datastreams Configuration | Blynk Event Setup |
|:---:|:---:|
| ![Datastreams](Datastreams.png) | ![Event Setup](Cloud_Blynk_IoT/Event_Setup.png) |

---

## 📈 Results

| Serial Monitor Output | Email Alert Notification |
|:---:|:---:|
| ![Serial Monitor](Serial monitor.png) | ![Email Notification](Email alert message.png) |

The system successfully:
- Streams all sensor data (gas, temperature, humidity, flame, motion) to Blynk cloud in real-time
- Triggers instant cloud events on threshold violations with one-shot state-change logic
- Delivers automated email warnings from `robot@blynk.cloud` for gas, fire, motion, and temperature hazards
- Activates relay and buzzer in real-time, holding them active until the hazard condition clears

---

## 👤 Author

**Padam Shankara Bala Sai Varshith**  
Smart Home Automation — IoT Mini Project  
Blynk Organization: My organization - 4511RH

---

<div align="center">

⭐ **If this project was helpful, consider giving it a star!** ⭐

*Built with ESP32 + Blynk IoT | Simulated on Wokwi*

</div>
