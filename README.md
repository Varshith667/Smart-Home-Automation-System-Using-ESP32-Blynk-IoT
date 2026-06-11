# 🏠 Smart Home Security System
### ESP32 + Blynk IoT | Wokwi Simulator

A real-time IoT-based home security system that monitors gas leakage, fire, motion, and temperature/humidity using an ESP32 microcontroller. Alerts and sensor data are sent to the **Blynk IoT app** on your phone.

---

## 📦 Components Used (Wokwi)

| Component       | Purpose                                      |
|----------------|----------------------------------------------|
| ESP32           | Main microcontroller + WiFi                  |
| PIR Sensor      | Motion detection                             |
| Flame Sensor    | Fire/flame detection                         |
| DHT22           | Temperature & humidity monitoring            |
| MQ-2 Gas Sensor | LPG / smoke / gas leakage detection          |
| Buzzer          | Audio alert for fire & gas                   |
| Relay Module    | Controls exhaust fan or external alarm       |

---

## 🔌 Wiring (Pin Connections)

### ESP32 GPIO Assignments

| Sensor / Component  | ESP32 Pin   | Notes                          |
|--------------------|-------------|--------------------------------|
| DHT22 (Data)        | GPIO 27     | 10kΩ pull-up to 3.3V           |
| PIR Sensor (OUT)    | GPIO 15     | Digital output                 |
| Flame Sensor (DO)   | GPIO 4      | Digital output (LOW = flame)   |
| MQ-2 Gas (AO)       | GPIO 34     | Analog input (ADC1)            |
| Relay (IN)          | GPIO 2      | Active HIGH                    |
| Buzzer (+)          | GPIO 14     | Active buzzer                  |
| All sensors VCC     | 3.3V / 5V   | Check your sensor datasheet    |
| All sensors GND     | GND         |                                |

### Wokwi Wiring Diagram (Text)

```
ESP32                 PIR Sensor
GPIO 15  ──────────►  OUT
3.3V     ──────────►  VCC
GND      ──────────►  GND

ESP32                 Flame Sensor
GPIO 4   ──────────►  DO
3.3V     ──────────►  VCC
GND      ──────────►  GND

ESP32                 DHT22
GPIO 27  ──────────►  DATA (+ 10kΩ to 3.3V)
3.3V     ──────────►  VCC
GND      ──────────►  GND

ESP32                 MQ-2 Gas Sensor
GPIO 34  ──────────►  AO (Analog Out)
5V       ──────────►  VCC
GND      ──────────►  GND

ESP32                 Relay Module
GPIO 2   ──────────►  IN
5V       ──────────►  VCC
GND      ──────────►  GND

ESP32                 Buzzer
GPIO 14  ──────────►  + (Positive)
GND      ──────────►  - (Negative)
```

> ⚠️ **Important:** GPIO 34 is input-only (no internal pull-up). Perfect for analog reads from MQ-2. Do NOT use GPIO 34 as output.

---

## 📱 Blynk IoT Setup (Step-by-Step)

### Step 1 — Create a Blynk Account
1. Go to [https://blynk.cloud](https://blynk.cloud) and sign up for a free account.
2. Verify your email and log in.

### Step 2 — Create a New Template
1. In the Blynk console, click **"+ New Template"**.
2. Fill in:
   - **Name:** Smart Home Security
   - **Hardware:** ESP32
   - **Connection:** WiFi
3. Click **Done**.

### Step 3 — Set Up Datastreams (Virtual Pins)
Go to **Datastreams** tab inside your template and add these:

| Name                | Virtual Pin | Data Type | Min | Max |
|--------------------|-------------|-----------|-----|-----|
| Gas Sensor          | V1          | Integer   | 0   | 1   |
| Flame Sensor        | V3          | Integer   | 0   | 1   |
| Motion Sensor       | V4          | Integer   | 0   | 1   |
| Temperature         | V5          | Double    | -40 | 80  |
| Humidity            | V6          | Double    | 0   | 100 |
| Relay Status        | V7          | Integer   | 0   | 1   |

Click **Save** after adding all datastreams.

### Step 4 — Create a Web/Mobile Dashboard
1. Go to the **Web Dashboard** or **Mobile Dashboard** tab.
2. Add widgets and link them to virtual pins:
   - **LED / Status** widget → V1 (Gas), V3 (Flame), V4 (Motion), V7 (Relay)
   - **Gauge** widget → V5 (Temperature), V6 (Humidity)
   - **Label / Value Display** → any pin for live reading

### Step 5 — Set Up Event Alerts (Notifications)
1. Go to **Events** tab in your template.
2. Create an event:
   - **Event Name:** `warning_message`
   - **Type:** Warning
   - Enable **"Send Email"** and/or **"Push Notification"**
3. Save.

> This is the event name used in the code: `Blynk.logEvent("warning_message", "...")`

### Step 6 — Create a Device
1. Go to **Devices** → **+ New Device** → **From Template**.
2. Select your template → name it (e.g., "Home ESP32").
3. Copy the 3 credentials shown:
   - `BLYNK_TEMPLATE_ID`
   - `BLYNK_TEMPLATE_NAME`
   - `BLYNK_AUTH_TOKEN`

### Step 7 — Update the Code
Paste your credentials at the top of `smart_home_security.ino`:

```cpp
#define BLYNK_TEMPLATE_ID   "TMPLxxxxxxxx"
#define BLYNK_TEMPLATE_NAME "Smart Home Security"
#define BLYNK_AUTH_TOKEN    "your_long_token_here"
```

---

## 🧪 Running in Wokwi

1. Open [https://wokwi.com](https://wokwi.com)
2. Create a new **ESP32** project.
3. Paste the code from `smart_home_security.ino` into the editor.
4. Add sensors in the diagram using the **"+"** button:
   - DHT22, PIR, Flame Sensor, MQ-2 Gas Sensor, Buzzer, Relay
5. Wire them according to the pin table above.
6. In `diagram.json` or the wiring editor, set WiFi SSID to `"Wokwi-GUEST"` — it connects automatically, no password needed.
7. Click **▶ Run**.
8. Open your Blynk app — data should appear live!

---

## 🚨 How Each Alert Works

### 🔥 Flame Detected
- `digitalRead(flame_sensor_pin) == HIGH`
- Relay activates (sprinkler/fan)
- Buzzer beeps at 550Hz until flame is gone
- Blynk push notification sent

### 💨 Gas Leakage
- `gas_output_value > 165` (mapped from 0–4095 ADC → 0–255)
- Relay activates (exhaust fan)
- Buzzer beeps at 1000Hz until gas clears
- Blynk notification sent

### 🌡️ Over Temperature
- `temperature >= 50°C`
- Relay activates (fan/cooling)
- Blynk notification sent
- Deactivates once temp drops below 50°C

### 🚶 Motion Detected
- `digitalRead(PIR) == HIGH`
- Blynk notification sent: "Someone might be inside"
- State tracked to avoid repeated alerts

---

## 📚 Libraries Required

Install these in Arduino IDE via **Sketch → Include Library → Manage Libraries**:

| Library                  | Install Name               |
|--------------------------|----------------------------|
| Blynk for ESP32          | `Blynk`                    |
| DHT Sensor Library (ESP) | `DHTesp`                   |
| WiFi (built-in ESP32)    | Pre-installed with ESP32   |

---

## 📁 Project Structure

```
smart-home-security/
├── smart_home_security.ino    ← Main Arduino code
└── README.md                  ← This file
```

---

## 🔧 Troubleshooting

| Problem                        | Solution                                              |
|-------------------------------|-------------------------------------------------------|
| WiFi not connecting in Wokwi  | Use `ssid = "Wokwi-GUEST"` and `pass = ""`           |
| Blynk not connecting           | Double-check Auth Token, Template ID, Template Name   |
| Gas sensor always high         | Adjust threshold value (default: 165) in code         |
| DHT22 returning NaN            | Check wiring; add 10kΩ pull-up resistor on DATA pin   |
| Notifications not working      | Create `warning_message` event in Blynk console       |

---

## 👨‍💻 Author

Developed as a mini project for IoT / Embedded Systems coursework.
Built and tested on **Wokwi Simulator** with **Blynk IoT Cloud**.
