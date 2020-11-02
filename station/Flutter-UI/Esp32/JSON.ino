#include <ArduinoJson.h>

#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>

const char* ssid = "new";
const char* password = "12345678";
IPAddress ip(192, 168, 43, 2); 
IPAddress gateway(192, 168, 43, 1); 

IPAddress subnet(255, 255, 255, 0);
WebServer server(8080);
int state = 1;
bool Status = false;

void handleRoot() {
  const size_t capacity = JSON_OBJECT_SIZE(2);
  String replyMsg;
  DynamicJsonDocument doc(capacity);

  doc["state"] = state;
  doc["status"] = Status;

  serializeJson(doc, replyMsg);
  server.send(200, "application/json", replyMsg);

}

void setTrue() {
  Status = true;
  server.send(200, "text/plain", "set false success");
}

void setFalse() {
  Status = false;
  server.send(200, "text/plain", "set true success");
}

void reFresh() {
  Status = false;
  state = 1;
  server.send(200, "text/plain", "reset success");
}



void handleNotFound() {

  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);

}

void setup(void) {

  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.config(ip, gateway, subnet);

  WiFi.begin(ssid, password);
  Serial.println("");
  pinMode(0, INPUT_PULLUP);
  pinMode(15, INPUT_PULLUP);
  pinMode(2, OUTPUT);
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp32")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);
  server.on("/setStatusFalse", setFalse);
  server.on("/setStatusTrue", setTrue);
  server.on("/refresh", reFresh);

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
  if ((digitalRead(0) == 0) && Status && state == 1){
    state = 2;
  }

  if (digitalRead(15) == 0 && state == 2) {
    state = 3;
  }

  if (Status == true) {
    digitalWrite(2, HIGH);
  }
  else {
    digitalWrite(2, LOW);
  }
}
