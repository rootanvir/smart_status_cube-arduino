#include <ESP8266WiFi.h>
#include <espnow.h>
#include <U8g2lib.h>
#include <SPI.h>

U8G2_SH1106_128X64_NONAME_F_4W_HW_SPI u8g2(U8G2_R0, D8, D3, D4);

typedef struct struct_message {
  float ax;
  float ay;
  float az;
} struct_message;

struct_message accelData;

// Callback: called when ESP-NOW data is received
void OnDataRecv(uint8_t *mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&accelData, incomingData, sizeof(accelData));
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != 0) {
    Serial.println("ESP-NOW init failed");
    while (1);
  }

  esp_now_set_self_role(ESP_NOW_ROLE_COMBO);
  esp_now_register_recv_cb(OnDataRecv);

  u8g2.begin();
}

void loop() {
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB10_tr);

  char buffer[25];

  snprintf(buffer, sizeof(buffer), "X: %.2f", accelData.ax);
  u8g2.drawStr(0, 20, buffer);

  snprintf(buffer, sizeof(buffer), "Y: %.2f", accelData.ay);
  u8g2.drawStr(0, 35, buffer);

  snprintf(buffer, sizeof(buffer), "Z: %.2f", accelData.az);
  u8g2.drawStr(0, 50, buffer);

  u8g2.sendBuffer();
  delay(100);
}
