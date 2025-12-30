#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include <Servo.h>

// Create the motor shield object
Adafruit_MotorShield AFMS = Adafruit_MotorShield();

// Create servo object
Servo gaugeServo;

// Pins
const int sensorPin = A0;

// Servo limits (MATLAB: gaugeMin = 1, gaugeMax = 0)
const int servoMin = 180;   // corresponds to position = 1
const int servoMax = 0;     // corresponds to position = 0

// Light calibration variables
int lightMin = 1023;
int lightMax = 0;

unsigned long startTime;

void setup() {
  Serial.begin(9600);

  // Start motor shield
  AFMS.begin();

  // Attach servo to SERVO 1 on motor shield
  gaugeServo.attach(10);  // Servo 1 uses pin 10

  // Initial sweep
  gaugeServo.write(servoMin);
  delay(1000);
  gaugeServo.write(servoMax);
  delay(1000);

  startTime = millis();
}

void loop() {
  // Run for 120 seconds (same as MATLAB tic/toc)
  if (millis() - startTime < 120000) {

    int lightLevel = analogRead(sensorPin);

    // Update min/max light values
    if (lightLevel > lightMax) {
      lightMax = lightLevel;
    }
    if (lightLevel < lightMin) {
      lightMin = lightLevel;
    }

    // Avoid division by zero
    if (lightMax != lightMin) {
      float lightPct = (float)(lightLevel - lightMin) / (lightMax - lightMin);
      lightPct = constrain(lightPct, 0.0, 1.0);

      int servoPos = servoMin + lightPct * (servoMax - servoMin);

      gaugeServo.write(servoPos);
    }

    delay(50);  // smooth movement
  }
}
