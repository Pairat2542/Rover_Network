#include <ArduinoJson.h>
#include "EEPROM.h"
#include <esp_now.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <WiFi.h>
#include <WiFiAP.h>
int station_number = 1;
WebServer server(8080);
int state = 1;
bool Status = false;
bool restart_esp = false;
bool checkSend = true;
bool checkAdd = true;
bool check_send_fail = true;
bool ticker = true;
esp_now_peer_info_t peerInfo;
uint8_t broadcastMac[] = {0xFF, 0xFF , 0xFF , 0xFF , 0xFF , 0xFF };
uint8_t registerMac[6];
unsigned long start_time = 0, check_car_time = 0;
typedef struct Data {
  String msg;
  float latency;
} MyData;
MyData receive_data, winner_data;
char ssid[] = "Station#";
void setup() {
  Serial.begin(9600);
  WiFi.mode(WIFI_AP_STA);
  ssid[7] = station_number + 48;
  Serial.println(ssid);
  WiFi.softAP(ssid);
  Serial.println(WiFi.softAPIP());
  esp_now_setup();
  pinMode(0, INPUT);
  EEPROM.begin(64);
  Status = EEPROM.read(0);
  server.on("/", handleRoot);
  server.on("/setRestart", setRestart);
  server.on("/setStatusTrue", setTrue);
  server.on("/refresh", reFresh);
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");
}
void loop() {
  server.handleClient();
  if (Status && checkSend) {
    checkSend = false;
    start_send();
    EEPROM.write(0, Status);
    EEPROM.commit();
  }
  if (start_time != 0 && millis() - start_time >= 10000) {
    if (winner_data.latency == 0) start_send();
    else if (checkAdd) {
      checkAdd = false;
      add_peer(registerMac);
      delay(100);
      int i = 0;
      for (i = 0 ; i < 3; i++) {
        send_data(registerMac, "Win");
        delay(1000);
        Serial.println(check_send_fail);
        if (check_send_fail) {
          state = 2;
          check_car_time = millis();
          ticker = true;
          break;
        }
      }
      if (i >= 3) {
        ESP.restart();
      }
    }
  }
  if (!check_send_fail) {
    checkSend = false;
    start_send();
    check_send_fail = true;
  }
  if (ticker && millis() - check_car_time >= 3000) {
    send_data(registerMac, "Alive");
    delay(500);
    if (!check_send_fail) {
      ESP.restart();
    }
    check_car_time = millis();
  }
}
void start_send() {
  start_time = millis();
  winner_data.latency = 0;
  send_data(broadcastMac, ssid);
}
void display_macaddress(const uint8_t* mac) {
  Serial.print("Mac Address: ");
  for (int i = 0 ; i < 6; i++) {
    Serial.print(mac[i], HEX);
    Serial.print(" ");
  }
  Serial.println();
}
