#include <esp_now.h>
#include <WiFi.h>
#include <ESP32Ping.h>
#include "LedControl.h"
esp_now_peer_info_t peerInfo;
LedControl lc = LedControl(25, 27, 26, 1);
uint8_t stationMac[6];
bool check_coming = false, check_win = false, push = false;;
char station_number[] = "Station#";
unsigned int timer = 0;
typedef struct Data {
  String msg;
  float latency;
} MyData;
MyData send_data_latency;
byte loopsuccess = 4;
void setup() {
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  Serial.println(WiFi.macAddress());
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  esp_now_setup();
  pinMode(0, INPUT);
  pinMode(2, OUTPUT);
  digitalWrite(2, LOW);
  lc.shutdown(0, false);
  lc.setIntensity(0, 8);
  lc.clearDisplay(0);
  attachInterrupt(digitalPinToInterrupt(0), pushing, FALLING);
}

void loop() {
  if (check_coming) {
    show_station();
    digitalWrite(2, HIGH);
    if (push) {
      digitalWrite(2, LOW);
      add_peer(stationMac);
      send_data(stationMac, "ready", sending_data());
      delay(200);
      ESP.restart();
    }
    if (millis() - timer >= 10500) {
      ESP.restart();
    }
  }
  else if (!check_win) {
    waiting_show();
  }
  if (check_win) {
    while(loopsuccess > 0){
      show_win();
      delay(200);
      lc.clearDisplay(0);
      delay(200);
      loopsuccess -= 1;
    }
    show_win();
    if (push) {
      add_peer(stationMac);
      send_data(stationMac, "end", 0);
      ESP.restart();
    }
  }
}
float sending_data() {
  Serial.println("Connecting to WiFi");
  Serial.println(station_number);
  WiFi.begin(station_number);
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }
  float latency = 0;
  bool ret = Ping.ping(IPAddress(192, 168, 4, 1));
  if (ret) {
    latency = Ping.averageTime();
  }
  return latency;
}
void waiting_show() {
  lc.setRow(0, 7, B00000101);
  lc.setChar(0, 6, 'e', false);
  lc.setChar(0, 5, 'a', false);
  lc.setChar(0, 4, 'd', false);
  lc.setRow(0, 3, B00111011);
  lc.setRow(0, 2, B10000000);
  lc.setRow(0, 1, B10000000);
  lc.setRow(0, 0, B10000000);
  delay(100);
  lc.setRow(0, 0, B00000000);
  delay(100);
}
void show_station() {
  lc.setRow(0, 7, B01011011);
  lc.setRow(0, 6, B00001111);
  lc.setChar(0, 5, 'a', false);
  lc.setRow(0, 4, B00001111);
  lc.setRow(0, 3, B00110000);
  lc.setRow(0, 2, B01111110);
  lc.setRow(0, 1, B00010101);
  lc.setDigit(0, 0, station_number[7] - 48, false);
  delay(50);
}
void show_win() {
  lc.setRow(0, 7, B01011011);
  lc.setRow(0, 6, B00111110);
  lc.setRow(0, 5, B01001110);
  lc.setRow(0, 4, B01001110);
  lc.setChar(0, 3, 'e', false);
  lc.setRow(0, 2, B01011011);
  lc.setRow(0, 1, B01011011);
  lc.setRow(0, 0, B10000000);
  delay(50);
}
void pushing() {
  push = true;
}
