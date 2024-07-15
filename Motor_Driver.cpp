//MOTOR DRIVER
//Project INSEE Gantry Crane Automation
//DETE-SEUSL
//Author: MALINDA MADUMAL
//2024.04.04

#include <ArduinoJson.h>
#include <AFMotor.h>

AF_DCMotor motor_drive(1);

const char* driveM_dirct;
int driveM_speed = 0;

void receiveJson() {
  // Check if data is available to read from the ESP32
  if (Serial.available()) {
    String jsonString = Serial.readStringUntil('\n');
    //Parse the json
    StaticJsonDocument<100> doc;
    DeserializationError error = deserializeJson(doc, jsonString);

    if (!error) {
      //Extract values from json
      driveM_dirct = doc["driveM_dirct"];
      driveM_speed = doc["driveM_speed"];
    }
  }
}

void setup() {
  Serial.begin(9600);
  motor_drive.run(RELEASE);
}

void loop() {
  void receiveJson();

  motor_drive.setSpeed(driveM_speed);

  if(driveM_dirct == 'F'){
    motor_drive.run(FORWARD);
  }
  else if(driveM_dirct == 'B'){
    motor_drive.run(BACKWARD);
  }
  else{
    motor_drive.run(RELEASE);
  }

  delay(100);
  
}
