#include <ESP8266WiFi.h>
#include <espnow.h>
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

Adafruit_MPU6050 mpu;

// Receiver ESP8266 MAC address
uint8_t receiverMac[] = {0x34, 0x5F, 0x45, 0x55, 0xE4, 0x34};

typedef struct struct_message {
  float ax;
  float ay;
  float az;
  char direction[10];
} struct_message;

struct_message accelData;

// Vibration motor pin
#define VIB_PIN D5
String lastDirection = ""; // to store previous direction

void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.print("Send Status: ");
  Serial.println(sendStatus == 0 ? "Success" : "Fail");
}

void setup() {
  Serial.begin(115200);
  Wire.begin();

  // Initialize MPU6050
  if (!mpu.begin()) {
    Serial.println("MPU6050 not found!");
    while (1);
  }
  Serial.println("MPU6050 ready");

  pinMode(VIB_PIN, OUTPUT);
  digitalWrite(VIB_PIN, LOW);

  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("ESP-NOW init failed");
    while (1);
  }

  esp_now_set_self_role(ESP_NOW_ROLE_COMBO);
  esp_now_register_send_cb(OnDataSent);
  esp_now_add_peer(receiverMac, ESP_NOW_ROLE_COMBO, 1, NULL, 0);
}

void loop() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  accelData.ax = a.acceleration.x;
  accelData.ay = a.acceleration.y;
  accelData.az = a.acceleration.z;

  // Detect orientation for 6 sides
  String currentDir;
  if (a.acceleration.x > 7) currentDir = "In Class";
  else if (a.acceleration.x < -7) currentDir = "In Meeting";
  else if (a.acceleration.y > 7) currentDir = "Exam Duty";
  else if (a.acceleration.y < -7) currentDir = "Lunch Time";
  else if (a.acceleration.z > 7) currentDir = "Available";
  else if (a.acceleration.z < -7) currentDir = "Busy !";
  else currentDir = "Loading.";

  strcpy(accelData.direction, currentDir.c_str());

  // Vibrate motor if direction changed
  if (currentDir != lastDirection) {
    digitalWrite(VIB_PIN, HIGH);
    delay(500);  // vibrate 200ms
    digitalWrite(VIB_PIN, LOW);
    lastDirection = currentDir;
  }

  // Send data
  esp_now_send(receiverMac, (uint8_t *) &accelData, sizeof(accelData));

  Serial.printf("X: %.2f  Y: %.2f  Z: %.2f  Dir: %s\n",
                accelData.ax, accelData.ay, accelData.az, accelData.direction);

  delay(500);
}
