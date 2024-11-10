#include <ESP8266WiFi.h>
#include <WebSocketsServer.h>
#include <ArduinoJson.h>
//#include "Secrets.hpp"
String ssid = "N3P7UN3";
String password = "1337m0nk3y";
String apSSID = "IOT_HUB_AP";
String apPassword = "RaspBerry";

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
        //webSocketData.sendTXT(1, payload, length);
        // printJSON(payload, length);
        webSocketData.sendTXT(num, payload, length);
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


void printJSON(uint8_t * payload, size_t length) {
  // Create a JSON document
  // Ensure the payload is null-terminated
  char jsonBuffer[length + 1];
  memcpy(jsonBuffer, payload, length);
  jsonBuffer[length] = '\0';

  // Create a JSON document
  StaticJsonDocument<192> doc;

  // Deserialize the JSON document
  DeserializationError error = deserializeJson(doc, jsonBuffer);

  // Error handling
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }

  Serial.println(jsonBuffer);

  // int serial = doc["serial"]; // 2
  // long time = doc["time"]; // 1351824120

  // JsonArray data = doc["data"];
  // float data_0 = data[0]; // 48.75
  // float data_1 = data[1]; // 2.3
  // float data_2 = data[2]; // 30.22
  // float data_3 = data[3]; // 10.1

// // Print JSON document values
//   Serial.println(doc["serial"]);
//   Serial.println(doc["time"]);
//   JsonArray data = doc["data"];

//   Serial.println(data[0]);
//   Serial.println(data[1]);
//   //Serial.println(data[2]);
//   // Modify JSON document values

  }