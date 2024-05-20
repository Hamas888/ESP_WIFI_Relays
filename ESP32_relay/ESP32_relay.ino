#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h> // Include the mDNS library

const char* ssid = "espRelay1";
const char* password = "12345678";
const char* hostname = "esprelay"; 
WebServer server(80);

const int ledPin = 2; 

void setup() {
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  Serial.begin(115200);
  
  // Connect to WiFi
  WiFi.softAP(ssid, password, true);
  IPAddress myIP = WiFi.softAPIP();
  Serial.println("AP IP address: ");
  Serial.print(myIP);
  Serial.println("");

  // Configure mDNS
  if (!MDNS.begin(hostname)) {
    Serial.println("Error setting up MDNS responder!");
  }
  Serial.println("mDNS responder started");

  // Configure web server routes
  server.on("/", HTTP_GET, [](){
    server.send(200, "text/plain", "espRelay1 check: ok");
  });

  // Endpoint to control the GPIO pin
  server.on("/control", HTTP_GET, [](){
      if (server.hasArg("state")) {
          String state = server.arg("state");
          if (state == "high") {
              digitalWrite(ledPin, HIGH);
          } else if (state == "low") {
              digitalWrite(ledPin, LOW);
          }
          server.send(200, "text/plain", "OK");
      } else {
          server.send(400, "text/plain", "Bad Request: 'state' parameter missing");
      }
  });

  // Start the web server
  server.begin();
}

void loop() {
  server.handleClient();
}
