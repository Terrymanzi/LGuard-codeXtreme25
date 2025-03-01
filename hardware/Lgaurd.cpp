#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>

// **Ultrasonic Sensor Pins**
#define TRIG_PIN D6
#define ECHO_PIN D5

// **Buzzer Pin**
#define BUZZER_PIN D3

// **Vibration Sensor**
#define VIBRATION_SENSOR A0  

// **ADXL345 Accelerometer**
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);

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

void loop() {
  Serial.println("Base Project Setup Running...");
  delay(1000);
}
