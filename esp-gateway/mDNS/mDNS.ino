#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>

#include "ConfigManager.h"

const char* ssid = "RT-GPON-A228";
const char* password = "Ki7f3h8eeb";

ConfigManager config;
WebServer server(80);
bool ledState = false;

void enableCORS() {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Access-Control-Allow-Methods", "GET,POST,OPTIONS");
  server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
}

void handleToggle() {
  enableCORS();
  ledState = !ledState;
  digitalWrite(13, ledState);
  
  String json = "{\"status\":\"ok\",\"led\":" + String(ledState ? "true" : "false") + "}";
  server.send(200, "application/json", json);
}

void handlePrint() {
  enableCORS();
  
  if (server.hasArg("plain")) {
    String text = server.arg("plain");
    Serial.println("Greenneeka received: " + text);
    server.send(200, "text/plain", "Text received: " + text);
  } else {
    server.send(400, "text/plain", "No text received");
  }
}

void handleStatus() {
  enableCORS();
  String json = "{\"device\":\"Greenneeka\",\"led\":" + String(ledState ? "true" : "false") + "}";
  server.send(200, "application/json", json);
}

void handleOptions() {
  enableCORS();
  server.send(200, "text/plain", "");
}

void setup() {
  Serial.begin(115200);
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  
  // Подключаемся к Wi-Fi сети
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  
  Serial.println("\nConnected to WiFi!");
  Serial.print("Greenneeka IP address: ");
  Serial.println(WiFi.localIP());

  // Запускаем mDNS с именем "greenneeka"
  if (!MDNS.begin("greenneeka")) {
    Serial.println("Error setting up MDNS responder!");
  } else {
    Serial.println("mDNS responder started");
    Serial.println("You can now access Greenneeka at: http://greenneeka.local");
  }

  server.on("/api/toggle", HTTP_POST, handleToggle);
  server.on("/api/toggle", HTTP_OPTIONS, handleOptions);
  server.on("/api/print", HTTP_POST, handlePrint);
  server.on("/api/print", HTTP_OPTIONS, handleOptions);
  server.on("/api/status", HTTP_GET, handleStatus);
  
  server.begin();
  Serial.println("Greenneeka Web Server Started!");
}

void loop() {
  server.handleClient();
}
