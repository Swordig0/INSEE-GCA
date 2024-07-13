//RECEIVER module
//Project INSEE Gantry Crane Automation
//DETE-SEUSL
//Author: MALINDA MADUMAL
//2024.04.04

#include <WiFi.h>
#include <esp_now.h>
#include <ArduinoJson.h>      //For communicating with the UNO
#include <HardwareSerial.h>   //For communicating with the UNO

#define CHANNEL 0     //ESP-NOW communication channel
#define lSwitch_GF 1  //Limit switch - Girder Front
#define lSwitch_GB 2  //Limit switch - Girder Back
#define lSwitch_HR 3  //Limit switch - Hoist Right
#define lSwitch_HL 4  //Limit switch - Hoist Left
#define eStop 5       //Emergency stop button

HardwareSerial UNOserial(2); //UART2 for serial communication with UNO

String jsonString;

char driveM_dirct;
int driveM_speed = 0;

//Data to be received
typedef struct message {
  bool ESTOP;
  int JOY1X;
  int JOY1Y;
  int JOY2X;
  int JOY2Y;
} message;
message mData;

//Callback when data is received
void receiveCallback(const esp_now_recv_info_t *mac_addr, const uint8_t *data, int data_len) {
  Serial.print("Data Received!");
  memcpy(&mData, data, sizeof(mData));
}

//Maps joystick values to motors
void motorData(){
  int j1x = mData.JOY1X;
  if (j1x < 2048){
    driveM_dirct = 'B';
    driveM_speed = map(j1x, 0, 2047, 255, 0);
  }
  else{
    driveM_dirct = 'F';
    driveM_speed = map(j1x, 2048, 4096, 0, 255);
  }

  
}

//Creates a json message to send to the UNO
void createJson(){
  StaticJsonDocument<100> doc;
  doc["driveM_dirct"] = driveM_dirct;
  doc["driveM_speed"] = driveM_speed;

  serializeJson(doc, jsonString);
}

void sendJson(){
  createJson();
  UNOserial.println(jsonString);
}

//Checks whether the emergency stop is pressed
int checkEstop() {
  int state = digitalRead(eStop);
  return (state == LOW) ? 1 : 0;  // Return 1 if button is pressed (LOW state), otherwise return 0
}

void setup() {
  UNOserial.begin(9600, SERIAL_8N1, 16, 17); // RX2=16, TX2=17
  Serial.begin(115200);
  WiFi.mode(WIFI_AP);
  WiFi.softAP("GCA", "23175", CHANNEL, 0);
  esp_now_init();
  esp_now_register_recv_cb(receiveCallback);

  pinMode(lSwitch_GF, INPUT);
  pinMode(lSwitch_GB, INPUT);
  pinMode(lSwitch_HR, INPUT);
  pinMode(lSwitch_HL, INPUT);
  pinMode(eStop, INPUT);
}

void loop() {
  if(!checkEstop()){
    motorData();
    sendJson();

  }
  delay(300);
}
