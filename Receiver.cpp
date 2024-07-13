//RECEIVER module
//Project INSEE Gantry Crane Automation
//DETE-SEUSL
//Author: MALINDA MADUMAL
//2024.04.04

#include <WiFi.h>
#include <esp_now.h>

#define CHANNEL 0     //ESP-NOW communication channel
#define lSwitch_GF 1  //Limit switch - Girder Front
#define lSwitch_GB 2  //Limit switch - Girder Back
#define lSwitch_HR 3  //Limit switch - Hoist Right
#define lSwitch_HL 4  //Limit switch - Hoist Left
#define eStop 5       //Emergency stop button

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

void setup() {
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
  int stop = digitalRead(eStop);

  

  delay(300);
}
