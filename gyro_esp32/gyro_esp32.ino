#include <WiFi.h>
#include <WebServer.h>

const char* ssid     = "MyESP32_AP";
const char* password = "esp32pass"; // at least 8 chars

WebServer server(80);

String page = "<!doctype html><html><head><title>ESP32 AP</title></head>"
              "<body><h1>Hello from ESP32 Access Point</h1>"
              "<p>Connect to WiFi SSID: MyESP32_AP</p></body></html>";

void handleRoot() {
  server.send(200, "text/html", page);
}

void setup() {
  Serial.begin(115200);
  delay(100);

  // Start WiFi in AP mode
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  server.on("/", handleRoot);
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}
