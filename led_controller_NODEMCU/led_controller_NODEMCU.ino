#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>

const char* ssid = "WLAN-NJYHLR";
const char* password = "7810079531736273";
const IPAddress staticIP(192, 168, 2, 199); //Freie IP Adresse Hinzufügen auf der der NodeMcu connecten kann
const IPAddress gateway(192, 168, 2, 1);
const IPAddress subnet(255, 255, 255, 0);

ESP8266WebServer server(80);

void handleGet() {
  StaticJsonDocument<200> jsonDoc;
  jsonDoc["status"] = "Online";
  String jsonResponse;
  serializeJson(jsonDoc, jsonResponse);
  server.send(200, "application/json", jsonResponse);
  Serial.println("GET request request!");
}

void handlePost() {
  String message = server.arg("message");
  Serial.println("POST-Nachricht: " + message);
  server.send(200, "text/plain", "Nachricht erhalten: " + message);
  Serial.println("POST request request!");
}

void setup() {
  Serial.begin(9600);
  
  WiFi.begin(ssid, password);
  WiFi.config(staticIP, gateway, subnet); // Statische IP-Konfiguration
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Verbindung zum WiFi-Netzwerk wird hergestellt...");
  }
  
  Serial.println("Verbunden mit dem WiFi-Netzwerk");
  Serial.print("NodeMCU IP-Adresse: ");
  Serial.println(WiFi.localIP());
  
  server.on("/get", handleGet);
  server.on("/post", HTTP_POST, handlePost);
  server.begin();
  Serial.println("Webserver gestartet");
}

void loop() {
  server.handleClient();
}
