#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
#include <ESP8266WiFi.h>
#include <Firebase_ESP_Client.h>


// **Network Credentials**
#define WIFI_SSID "Levenn"
#define WIFI_PASSWORD "11223344"

// **Firebase Credentials**
#define API_KEY "AIzaSyD4ZX5wKS94-DktiJBfZeziNdz447Tb7kw"
#define DATABASE_URL "https://l-guard-default-rtdb.europe-west1.firebasedatabase.app/"


// **Ultrasonic Sensor Pins**
#define TRIG_PIN D6
#define ECHO_PIN D5

// **Buzzer Pin**
#define BUZZER_PIN D3

// **Vibration Sensor**
#define VIBRATION_SENSOR A0  

// **ADXL345 Accelerometer**
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);

// **Firebase Objects**
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

void setup() {
  Serial.begin(115200);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println("\nConnected to Wi-Fi");

  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void loop() {
  Serial.println("WiFi & Firebase connected...");
  delay(2000);
}


void setup() {
  Serial.begin(115200);
  
  Wire.begin();
  if (!accel.begin()) {
    Serial.println("❌ ADXL345 NOT detected!");
    while (1);
  }
  accel.setRange(ADXL345_RANGE_16_G);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(VIBRATION_SENSOR, INPUT);
}

// **Calculate Distance Using Ultrasonic Sensor**
float getDistance() {
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);
  
    long duration = pulseIn(ECHO_PIN, HIGH, 30000);
    return duration * 0.034 / 2;  // Convert to cm
  }

void loop() {
  Serial.println("Base Project Setup Running...");
  delay(1000);
}
