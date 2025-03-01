#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Firebase_ESP_Client.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
#include <math.h>

// **Firebase Helpers**
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

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

// **Firebase Data Object**
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// **GPS Approximation (Modify with Real GPS if Available)**
float lat = 40.7128;  // Initial latitude (Example: New York City)
float lon = -74.0060; // Initial longitude

// **Speed & Location Variables**
float speedX = 0, speedY = 0, speedZ = 0, speedTotal = 0;
float locationX = 0, locationY = 0, locationZ = 0;
float distanceTravelled = 0; // For GPS approximation
unsigned long lastUpdate = 0;
const int VIBRATION_THRESHOLD = 500;

// **Setup**
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
  digitalWrite(BUZZER_PIN, LOW);
  pinMode(VIBRATION_SENSOR, INPUT);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println("\nConnected to Wi-Fi");

  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  
  if (Firebase.signUp(&config, &auth, "", "")) {
    Serial.println("Firebase connected!");
  } else {
    Serial.printf("Firebase Error: %s\n", config.signer.signupError.message.c_str());
  }

  config.token_status_callback = tokenStatusCallback;
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

// **Calculate Distance Using Ultrasonic Sensor**
float getDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 30000);
  if (duration == 0) return -1;  // No echo detected

  return duration * 0.034 / 2;  // Convert to cm
}

// **GPS Approximation Using ADXL345 Data**
void updateGPS(float dx, float dy) {
  float earthRadius = 6371000.0; // Earth radius in meters

  // Convert displacement (meters) into latitude/longitude change
  lat += (dy / earthRadius) * (180.0 / M_PI);
  lon += (dx / (earthRadius * cos(lat * M_PI / 180.0))) * (180.0 / M_PI);
}

// **Main Loop**
void loop() {
  if (Firebase.ready() && millis() - lastUpdate > 1000) {
    lastUpdate = millis();

    // **Read ADXL345 Data**
    sensors_event_t event;
    accel.getEvent(&event);
    float accelX = event.acceleration.x;
    float accelY = event.acceleration.y;
    float accelZ = event.acceleration.z;

    // **Calculate Speed & Location**
    float deltaTime = 1.0; // 1 second interval
    speedX += accelX * deltaTime;
    speedY += accelY * deltaTime;
    speedZ += accelZ * deltaTime;

    locationX += speedX * deltaTime;
    locationY += speedY * deltaTime;
    locationZ += speedZ * deltaTime;

    // **Compute Total Speed (m/s)**
    speedTotal = sqrt(speedX * speedX + speedY * speedY + speedZ * speedZ);

    // **Update Approximate GPS Coordinates**
    updateGPS(locationX, locationY);

    // **Read Ultrasonic Sensor**
    float distance = getDistance();

    // **Read Vibration Sensor**
    int vibrationValue = analogRead(VIBRATION_SENSOR);

    // **Accident Detection**
    if (distance < 5 && vibrationValue > VIBRATION_THRESHOLD) {
      digitalWrite(BUZZER_PIN, HIGH);
      Serial.println("🚨 ACCIDENT DETECTED!");
      Firebase.RTDB.setBool(&fbdo, "accident/detected", true);
      Firebase.RTDB.setString(&fbdo, "accident/status", "Accident");
    } else {
      digitalWrite(BUZZER_PIN, LOW);
      Firebase.RTDB.setBool(&fbdo, "accident/detected", false);
      Firebase.RTDB.setString(&fbdo, "accident/status", "Safe");
    }

    // **Send Data to Firebase**
    Firebase.RTDB.setFloat(&fbdo, "accident/distance", distance);
    Firebase.RTDB.setInt(&fbdo, "accident/vibration", vibrationValue);
    Firebase.RTDB.setFloat(&fbdo, "speed/total", speedTotal);
    Firebase.RTDB.setFloat(&fbdo, "location/latitude", lat);
    Firebase.RTDB.setFloat(&fbdo, "location/longitude", lon);

    // **Serial Monitor Debugging**
    Serial.print("GPS: "); Serial.print(lat); Serial.print(", "); Serial.println(lon);
    Serial.print("Speed: "); Serial.print(speedTotal); Serial.println(" m/s");
    Serial.print("Distance: "); Serial.print(distance); Serial.println(" cm");
    Serial.print("Vibration: "); Serial.println(vibrationValue);
  }
}