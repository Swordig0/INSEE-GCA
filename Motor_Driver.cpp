#include <ArduinoJson.h>
#include <AFMotor.h>

void receiveJson() {
  // Check if data is available to read from the ESP32
  if (Serial.available()) {
    String jsonString = Serial.readStringUntil('\n');
    //Parse the json
    StaticJsonDocument<100> doc;
    DeserializationError error = deserializeJson(doc, jsonString);

    if (!error) {
      //Extract values from json
      const char* driveM_dirct = doc["driveM_dirct"];
      int driveM_speed = doc["driveM_speed"];
    }
  }
}

void setup() {
  Serial.begin(9600);
}

void loop() {
  void receiveJson();
  delay(300);
  
}
