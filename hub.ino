#include <ESP8266WiFi.h>
#include <WebSocketsServer.h>
#include "Secrets.hpp"

WebSocketsServer webSocketData = WebSocketsServer(81);
WebSocketsServer webSocketCommand = WebSocketsServer(82);

void webSocketDataEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
  switch(type) {
    case WStype_DISCONNECTED:
      Serial.printf("[%u] WebSocket disconnected", num);
      Serial.println();
      break;
    case WStype_CONNECTED:
      {
        IPAddress ip = webSocketData.remoteIP(num);
        Serial.printf("[%u] WebSocket connected from %d.%d.%d.%d", num, ip[0], ip[1], ip[2], ip[3]);
        Serial.println();
      }
      break;
    case WStype_TEXT:
      {
        IPAddress ip = webSocketData.remoteIP(num);
        Serial.printf("[%u] Received data from %d.%d.%d.%d: %s", num, ip[0], ip[1], ip[2], ip[3], payload);
        Serial.println();
        webSocketData.sendTXT(1, payload, length);
      }
      break;
  }
}

void webSocketCommandEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
  switch(type) {
    case WStype_DISCONNECTED:
      Serial.printf("[%u] WebSocket disconnected", num);
      Serial.println();
      break;
    case WStype_CONNECTED:
      {
        IPAddress ip = webSocketCommand.remoteIP(num);
        Serial.printf("[%u] WebSocket connected from %d.%d.%d.%d", num, ip[0], ip[1], ip[2], ip[3]);
        Serial.println();
      }
      break;
    case WStype_TEXT:
      {
        IPAddress ip = webSocketCommand.remoteIP(num);
        Serial.printf("[%u] Received command from %d.%d.%d.%d: %s", num, ip[0], ip[1], ip[2], ip[3], payload);
        Serial.println();
        webSocketCommand.broadcastTXT(payload);
      }
      break;
  }
}

void setup() {
  Serial.begin(115200);
  setupAP();
  setupWiFi();
  webSocketData.begin();
  webSocketCommand.begin();
  webSocketData.onEvent(webSocketDataEvent);
  webSocketCommand.onEvent(webSocketCommandEvent);
}

void loop() {
  webSocketData.loop();
  webSocketCommand.loop();
  
  // Send random data to connected clients every 5 seconds
  static unsigned long lastSendTime = 0;
  if (millis() - lastSendTime > 5000) {
    String command = "Random Data: " + String(random(0, 1024));
    webSocketCommand.broadcastTXT(command);
    lastSendTime = millis();
  }
}

void setupWiFi() {
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void setupAP() {
  WiFi.softAP(apSSID, apPassword);

  Serial.println("Access Point started");
  Serial.print("IP Address: ");
  Serial.println(WiFi.softAPIP());
}
