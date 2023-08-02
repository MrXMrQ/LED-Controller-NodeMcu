#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>
#include <FastLED.h>

//Webserver
const char* ssid = "WLAN-NJYHLR";
const char* password = "7810079531736273";
const IPAddress staticIP(192, 168, 2, 199); //Freie IP Adresse Hinzufügen auf der der NodeMcu connecten kann
const IPAddress gateway(192, 168, 2, 1);
const IPAddress subnet(255, 255, 255, 0);
ESP8266WebServer server(80);

//LED-Strip
#define LED_PIN D2
#define NUM_LEDS 150

CRGB leds[NUM_LEDS];

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
  server.on("/post", handlePost);
  server.begin();
  Serial.println("Webserver gestartet");

  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS); // Art des LED-Streifens anpassen, falls notwendig
  Serial.println("LED-Konfig ready");
  
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS); // Art des LED-Streifens anpassen, falls notwendig
  FastLED.setBrightness(20); // Helligkeit einstellen
}

void loop() {
  server.handleClient();
}

void handleGet() {
  StaticJsonDocument<200> jsonDoc;
  jsonDoc["status"] = "Online";
  String jsonResponse;
  serializeJson(jsonDoc, jsonResponse);
  server.send(200, "application/json", jsonResponse);
  Serial.println("GET request request!");
}

void handlePost() {
  // Stelle sicher, dass die empfangene JSON nicht größer ist als 200 Bytes
  StaticJsonDocument<200> jsonDoc;

  // Überprüfe, ob es eine gültige JSON-POST-Anfrage gibt
  if (server.hasArg("plain")) {
    // Deserialisiere die empfangene JSON
    DeserializationError error = deserializeJson(jsonDoc, server.arg("plain"));

    // Überprüfe, ob das Deserialisieren erfolgreich war
    if (error == DeserializationError::Ok) {
      // Überprüfe, ob alle erforderlichen Felder vorhanden sind
      if (jsonDoc.containsKey("selectedIP") && jsonDoc.containsKey("selectedBrightness") &&
          jsonDoc.containsKey("keyword") && jsonDoc.containsKey("red") && 
          jsonDoc.containsKey("green") && jsonDoc.containsKey("blue")) {

        // Hole die Werte aus dem JSON-Dokument
        String IP = jsonDoc["selectedIP"].as<String>();
        int brightness = jsonDoc["selectedBrightness"].as<int>();
        String keyword = jsonDoc["keyword"].as<String>();
        int red = jsonDoc["red"].as<int>();
        int green = jsonDoc["green"].as<int>();
        int blue = jsonDoc["blue"].as<int>();

        // Antworte mit einer Erfolgsmeldung
        server.send(200, "text/plain", "Data received successfully!");
        controllLED(brightness, keyword, red, green, blue);
      } else {
        // Wenn nicht alle erforderlichen Felder vorhanden sind, antworte mit "400 Bad Request"
        server.send(400, "text/plain", "Bad Request - Missing fields");
      }
    } else {
      // Bei einem Fehler beim Deserialisieren, antworte mit "400 Bad Request"
      server.send(400, "text/plain", "Bad Request - Invalid JSON");
    }
  } else {
    // Wenn keine JSON-POST-Anfrage empfangen wurde, antworte mit "400 Bad Request"
    server.send(400, "text/plain", "Bad Request");
  }
}

void controllLED(int brightness, String keyword, int red, int green, int blue) {
  if (keyword == "normal") {
    Serial.println("change");
    fill_solid(leds, NUM_LEDS, CRGB(red, green, blue));
    FastLED.setBrightness(brightness);
    FastLED.show();
  } 
}
