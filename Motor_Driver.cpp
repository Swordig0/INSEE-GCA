//MOTOR DRIVER
//Project INSEE Gantry Crane Automation
//DETE-SEUSL
//Author: MALINDA MADUMAL
//2024.04.04

#include <ArduinoJson.h>
#include <AFMotor.h>

AF_DCMotor motor_drive(1);

int driveM_dirct = 0;
int driveM_speed = 0;

void motorDrive() {
  motor_drive.setSpeed(driveM_speed);
  if (driveM_dirct == 70) {
    motor_drive.run(FORWARD);
  } else if (driveM_dirct == 66) {
    motor_drive.run(BACKWARD);
  } else {
    motor_drive.run(RELEASE);
  }
}

void receiveJson() {
  // Check if data is available to read from the ESP32
  if (Serial.available()) {
    // Read the incoming JSON message
    String jsonString = Serial.readStringUntil('\n');

    // Parse the JSON message
    StaticJsonDocument<100> doc;
    DeserializationError error = deserializeJson(doc, jsonString);

    if (!error) {
      // Extract values from JSON
      driveM_dirct = doc["driveM_dirct"];
      driveM_speed = doc["driveM_speed"];

      // Print the received values to the Serial Monitor

      Serial.print("Received from ESP32: ");
      Serial.println("driveM_dirct");
      Serial.println(driveM_dirct);
      Serial.println("driveM_speed");
      Serial.println(driveM_speed);
      Serial.println("....................................................");

      motorDrive();

    } else {
      Serial.println("Failed to parse JSON");
    }
  }
}

void setup() {
  // Start the hardware serial communication at 9600 baud rate
  Serial.begin(9600);
  motor_drive.run(RELEASE);
}

void loop() {
  receiveJson();
  delay(300);
}
