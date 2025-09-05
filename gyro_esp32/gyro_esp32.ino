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
  char direction[15];
} struct_message;

struct_message accelData;

// EMA smoothing
float alpha = 0.1;
float axSmooth = 0, aySmooth = 0, azSmooth = 0;

// Vibration motor pin
#define MOTOR_PIN 5  // GPIO5

// ESP8266 MAC address
uint8_t receiverMAC[] = {0x34, 0x5F, 0x45, 0x55, 0xE4, 0x34};

void setup() {
  Serial.begin(115200);

  if (!mpu.begin()) {
    Serial.println("MPU6050 not found!");
    while(1);
  }

  pinMode(MOTOR_PIN, OUTPUT);
  digitalWrite(MOTOR_PIN, LOW);

  // Initialize ESP-NOW
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW init failed");
    while(1);
  }

  // Add ESP8266 receiver as peer
  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, receiverMAC, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
  esp_now_add_peer(&peerInfo);
}

void loop() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  // EMA smoothing
  axSmooth = alpha * a.acceleration.x + (1 - alpha) * axSmooth;
  aySmooth = alpha * a.acceleration.y + (1 - alpha) * aySmooth;
  azSmooth = alpha * a.acceleration.z + (1 - alpha) * azSmooth;

  accelData.ax = axSmooth;
  accelData.ay = aySmooth;
  accelData.az = azSmooth;

  // Predict direction
  if (azSmooth > 7) strcpy(accelData.direction, "Up");
  else if (azSmooth < -7) strcpy(accelData.direction, "Down");
  else if (axSmooth > 7) strcpy(accelData.direction, "Right");
  else if (axSmooth < -7) strcpy(accelData.direction, "Left");
  else if (aySmooth > 7) strcpy(accelData.direction, "Forward");
  else if (aySmooth < -7) strcpy(accelData.direction, "Backward");
  else strcpy(accelData.direction, "Loading...");

  // Vibrate motor if flat
  if (strcmp(accelData.direction, "Loading...") == 0) {
    digitalWrite(MOTOR_PIN, HIGH);
  } else {
    digitalWrite(MOTOR_PIN, LOW);
  }

  // Send data to ESP8266
  esp_now_send(receiverMAC, (uint8_t *)&accelData, sizeof(accelData));

  Serial.print("X="); Serial.print(axSmooth);
  Serial.print(" Y="); Serial.print(aySmooth);
  Serial.print(" Z="); Serial.print(azSmooth);
  Serial.print(" Dir="); Serial.println(accelData.direction);

  delay(100);
}
