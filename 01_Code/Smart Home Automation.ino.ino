// ============================================================
// Smart Home Security System — ESP32 + Blynk IoT
// Sensors: PIR | Flame | DHT22 | MQ2 Gas | Relay | Buzzer
// ============================================================

// ---------- Blynk Credentials ----------
#define BLYNK_TEMPLATE_ID "TMPL36LrGS5-h"
#define BLYNK_TEMPLATE_NAME "Smart Home Automation"
#define BLYNK_AUTH_TOKEN "-3zyxrS6rH8aQFogzFHuYodW-1nr4Zeh"

#define BLYNK_PRINT Serial

// ---------- Libraries ----------
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <DHTesp.h>

// ---------- WiFi ----------
char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "Wokwi-GUEST";
char pass[] = "";

// ---------- Blynk Timer ----------
BlynkTimer timer;

// ---------- Pin Definitions ----------
#define DHTPIN 27

const int motion_sensor_pin = 15;
const int flame_sensor_pin  = 4;
const int gas_sensor_pin    = 34;
const int relay_pin         = 2;
const int buzzer_pin        = 14;

// ---------- Virtual Pins ----------
#define gas_sensor_vpin         V1
#define flame_sensor_vpin       V3
#define motion_sensor_vpin      V4
#define temperature_sensor_vpin V5
#define humidity_sensor_vpin    V6
#define relay_vpin              V7

// ---------- DHT ----------
DHTesp dht;

// ---------- Alert Flags ----------
bool gasAlertSent = false;
bool tempAlertSent = false;
bool motionAlertSent = false;
bool flameAlertSent = false;

// ============================================================
// BLYNK CONNECTED
// ============================================================
BLYNK_CONNECTED()
{
  Blynk.syncAll();
}

// ============================================================
// READ ALL SENSORS
// ============================================================
void readSensors()
{
  bool alarm = false;

  // ========================================================
  // GAS SENSOR
  // ========================================================
  int gasValue = analogRead(gas_sensor_pin);

  Blynk.virtualWrite(gas_sensor_vpin, gasValue);

  Serial.print("Gas Value: ");
  Serial.println(gasValue);

  if (gasValue > 3800)
  {
    alarm = true;

    if (!gasAlertSent)
    {
      Serial.println("Gas Leakage Detected!");

      Blynk.logEvent(
        "warning_message",
        "Gas Leakage Detected!"
      );

      gasAlertSent = true;
    }
  }
  else
  {
    gasAlertSent = false;
  }
  delay(1000);
  // ========================================================
  // TEMPERATURE & HUMIDITY
  // ========================================================
  TempAndHumidity data = dht.getTempAndHumidity();

  float temperature = data.temperature;
  float humidity = data.humidity;

  Blynk.virtualWrite(temperature_sensor_vpin, temperature);
  Blynk.virtualWrite(humidity_sensor_vpin, humidity);

  Serial.print(" Temperature: ");
  Serial.print(temperature);

  Serial.print(" °C  Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");

  if (temperature > 50)
  {
    alarm = true;

    if (!tempAlertSent)
    {
      Serial.println("Over Temperature Detected!");

      Blynk.logEvent(
        "warning_message",
        "Over Temperature Detected!"
      );

      tempAlertSent = true;
    }
  }
  else
  {
    tempAlertSent = false;
  }
  delay(1000);
  // ========================================================
  // PIR MOTION SENSOR
  // ========================================================
int motion = digitalRead(motion_sensor_pin);

Blynk.virtualWrite(motion_sensor_vpin, motion);

if (motion == HIGH)
{
    alarm = true;   // <-- ADD THIS

    if (!motionAlertSent)
    {
        Serial.println("Motion Detected!");

        Blynk.logEvent(
            "warning_message",
            "Motion Detected!"
        );

        motionAlertSent = true;
    }
}
else
{
    motionAlertSent = false;
}
delay(1000);
  // ========================================================
  // FLAME SENSOR
  // ========================================================

  int flameState = digitalRead(flame_sensor_pin);

  bool flameDetected = (flameState == LOW);

  Blynk.virtualWrite(flame_sensor_vpin, flameDetected);

  if (flameDetected)
  {
    alarm = true;

    digitalWrite(relay_pin, HIGH);
    Blynk.virtualWrite(relay_vpin, 1);

    if (!flameAlertSent)
    {
      Serial.println("Fire Detected!");

      Blynk.logEvent(
        "warning_message",
        "Fire Detected!"
      );

      flameAlertSent = true;
    }
  }
  else
  {
    digitalWrite(relay_pin, LOW);
    Blynk.virtualWrite(relay_vpin, 0);

    flameAlertSent = false;
  }
delay(1000);
  // ========================================================
  // CONTINUOUS BUZZER LOGIC
  // ========================================================

  if (alarm)
  {
    tone(buzzer_pin, 1000);   // Continuous sound
  }
  else
  {
    noTone(buzzer_pin);       // Stop sound
  }

  Serial.println("--------------------------------");
}

// ============================================================
// SETUP
// ============================================================
void setup()
{
  Serial.begin(115200);

  pinMode(motion_sensor_pin, INPUT);
  pinMode(flame_sensor_pin, INPUT);

  pinMode(relay_pin, OUTPUT);
  pinMode(buzzer_pin, OUTPUT);

  digitalWrite(relay_pin, LOW);
  digitalWrite(buzzer_pin, LOW);

  dht.setup(DHTPIN, DHTesp::DHT22);

  Blynk.begin(auth, ssid, pass);

  // Read sensors every second
  timer.setInterval(200L, readSensors);
}

// ============================================================
// LOOP
// ============================================================
void loop()
{
  Blynk.run();
  timer.run();
}