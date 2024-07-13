//TRANSMITTER module
//Project INSEE Gantry Crane Automation
//DETE-SEUSL
//Author: MALINDA MADUMAL
//2024.04.04

#include <WiFi.h>
#include <esp_now.h>

#define CHANNEL 0  //ESP-NOW communication channel
#define joy1X 25    //Joystick 1 - X axis
#define joy1Y 26    //Joystick 1 - Y axis
#define joy2X 18    //Joystick 2 - X axis
#define joy2Y 19    //Joystick 2 - Y axis
#define eStop 4    //Emergency stop button

esp_now_peer_info_t slave;

//Data to be sent
typedef struct message {
  bool ESTOP;
  int JOY1X;
  int JOY1Y;
  int JOY2X;
  int JOY2Y;
} message;
message mData;

//Connecting to slave
void findSlave() {
  bool slaveFound = 0;
  memset(&slave, 0, sizeof(slave));
  int mac[6] = { 0x08, 0xD1, 0xF9, 0xF6, 0x1D, 0xCD };
  for (int ii = 0; ii < 6; ++ii) {
    slave.peer_addr[ii] = (uint8_t)mac[ii];
  }
  slave.channel = CHANNEL;
  slave.encrypt = 0;  //No encryption
  slaveFound = 1;
  if (slaveFound) {
    Serial.println("Slave Found, Processing..");
  } else {
    Serial.println("Slave Not Found, Retrying...");
  }
}

//Callback when data is sent from Master to Slave
void sendCallback(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.println("Data Sent!");
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  esp_now_init();
  esp_now_register_send_cb(sendCallback);
  findSlave();
  esp_now_add_peer(&slave);

  pinMode(joy1X, INPUT);
  pinMode(joy1Y, INPUT);
  pinMode(joy2X, INPUT);
  pinMode(joy2Y, INPUT);
  pinMode(eStop, INPUT);
}

void loop() {

  mData.ESTOP = digitalRead(eStop);
  mData.JOY1X = analogRead(joy1X);
  mData.JOY1Y = analogRead(joy1Y);
  mData.JOY2X = analogRead(joy2X);
  mData.JOY2Y = analogRead(joy2Y);
  
  esp_now_send(slave.peer_addr, (uint8_t *) &mData, sizeof(mData));
  delay(300);
}
