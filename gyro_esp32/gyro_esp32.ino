#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <WiFi.h>
#include <esp_now.h>

Adafruit_MPU6050 mpu;

typedef struct struct_message {
  float ax;
  float ay;
  float az;
} struct_message;

struct_message accelData;

// Replace with ESP8266 MAC address of your receiver
uint8_t receiverMAC[] = {0x34, 0x5F, 0x45, 0x55, 0xE4, 0x34};

void setup() {
  Serial.begin(115200);

  // Initialize MPU6050
  if (!mpu.begin()) {
    Serial.println("MPU6050 not found!");
    while (1);
  }

  // Initialize ESP-NOW
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW init failed");
    while (1);
  }

  // Add peer (ESP8266 receiver)
  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, receiverMAC, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    while (1);
  }
}

void loop() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  accelData.ax = a.acceleration.x;
  accelData.ay = a.acceleration.y;
  accelData.az = a.acceleration.z;

  // Send data to ESP8266
  esp_err_t result = esp_now_send(receiverMAC, (uint8_t *)&accelData, sizeof(accelData));

  if (result == ESP_OK) {
    Serial.println("Sent successfully");
  } else {
    Serial.println("Error sending data");
  }

  delay(100); // Send every 100ms
}
