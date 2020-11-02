void esp_now_setup() {
  while (esp_now_init() != ESP_OK) {
    Serial.println("Error to init esp_now");
  }
  esp_now_register_recv_cb(OnDataRecv);
  esp_now_register_send_cb(displaySendData);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
}
void add_peer(uint8_t macAdd[]) {
  memcpy(peerInfo.peer_addr, macAdd, 6);
  while (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
  }
}
void displaySendData(const uint8_t *mac_addr, esp_now_send_status_t status) {
  char macStr[18];
  Serial.print("Packet to: ");
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x", mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.print(macStr);
  Serial.print(" send status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}
void send_data(uint8_t macAdd[], String sending, float laten) {
  send_data_latency.msg = sending;
  send_data_latency.latency = laten;
  esp_err_t result = esp_now_send(macAdd, (uint8_t *) &send_data_latency, sizeof(send_data_latency));
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }
}
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  String check_word = "";
  memcpy(&check_word, incomingData, sizeof(check_word));
  Serial.println(check_word);
  if (findword(check_word, "Station") && !check_win) {
    memcpy(&stationMac, mac, sizeof(stationMac));
    get_station_number(check_word);
    check_coming = true;
    timer = millis();
  } else if (check_word == "Win") {
    memcpy(&stationMac, mac, sizeof(stationMac));
    check_win = true;
    check_coming = false;
  } else if (check_word != "Alive") {
    ESP.restart();
  }
}
void get_station_number(String sta) {
  for (int i = 0; i < sta.length(); i++) {
    station_number[i] = sta[i];
  }
}
bool findword(String str, String finding) {
  bool check = false;
  int j = 0;
  for (int i = 0; i < str.length(); i++) {
    if (check) {
      if (str[i] != finding[j++]) {
        check = false;
      }
      if (j == finding.length()) {
        break;
      }
    }
    if (finding[j] == str[i]) {
      check = true;
      j++;
    }
  }
  return check;
}
