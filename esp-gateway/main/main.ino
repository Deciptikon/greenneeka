#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "Greenneeka WiFi";
const char* password = "12345678";

WebServer server(80);
bool ledState = false;

void enableCORS() {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Access-Control-Allow-Methods", "GET,POST,OPTIONS");
  server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
}

// Переключить LED
void handleToggle() {
  enableCORS();
  ledState = !ledState;
  digitalWrite(13, ledState);
  
  String json = "{\"status\":\"ok\",\"led\":" + String(ledState ? "true" : "false") + "}";
  server.send(200, "application/json", json);
}

// Получить текст и вывести в Serial
void handlePrint() {
  enableCORS();
  
  if (server.hasArg("plain")) {
    String text = server.arg("plain");
    Serial.println("Received: " + text);
    server.send(200, "text/plain", "Text received: " + text);
  } else {
    server.send(400, "text/plain", "No text received");
  }
}

void handleOptions() {
  enableCORS();
  server.send(200, "text/plain", "");
}

void setup() {
  Serial.begin(115200);
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  
  WiFi.softAP(ssid, password);
  
  Serial.println("AP Started!");
  Serial.print("IP: ");
  Serial.println(WiFi.softAPIP());

  server.on("/api/toggle", HTTP_POST, handleToggle);
  server.on("/api/toggle", HTTP_OPTIONS, handleOptions);
  server.on("/api/print", HTTP_POST, handlePrint);
  server.on("/api/print", HTTP_OPTIONS, handleOptions);
  
  server.begin();
  Serial.println("Web Server Started!");
}

void loop() {
  server.handleClient();
}
