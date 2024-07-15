//MOTOR DRIVER
//Project INSEE Gantry Crane Automation
//DETE-SEUSL
//Author: MALINDA MADUMAL
//2024.04.04

#include <ArduinoJson.h>
#include <AFMotor.h>

AF_DCMotor motor_drive(1);

String jsonString;

int driveM_dirct = 0;
int driveM_speed = 0;

void receiveJson() {
  // Check if data is available to read from the ESP32
  if (Serial.available()) {
    jsonString = Serial.readStringUntil('\n');
    //Parse the json
    StaticJsonDocument<200> doc;
    DeserializationError error = deserializeJson(doc, jsonString);

    if (!error) {
      //Extract values from json
      driveM_dirct = doc["driveM_dirct"];
      driveM_speed = doc["driveM_speed"];

      Serial.println("driveM_dirct");
      Serial.println(driveM_dirct);
      Serial.println("driveM_speed");
      Serial.println(driveM_speed);
      Serial.println();

      motor_drive.setSpeed(driveM_speed);

      if (driveM_dirct == 70) {
        motor_drive.run(FORWARD);
      } else if (driveM_dirct == 66) {
        motor_drive.run(BACKWARD);
      } else {
        motor_drive.run(RELEASE);
      }
    } else {
      Serial.println("Failed to parse JSON");
    }
  }
}

void setup() {
  Serial.begin(9600);
  motor_drive.run(RELEASE);
}

void loop() {
  void receiveJson();

  //For debugging only
  //**************************
  //Serial.println(jsonString);
  //Serial.println();


  //**************************



  delay(300);
}
