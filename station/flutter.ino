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
  ESP.restart();
  server.send(200, "text/plain", "reset success");
}
void setRestart() {
  state = 4;
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
