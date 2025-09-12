#include <ESP8266WiFi.h>
#include <espnow.h>
#include <U8g2lib.h>
#include <SPI.h>
#include <MFRC522.h>

// OLED Display
U8G2_SH1106_128X64_NONAME_F_4W_HW_SPI u8g2(U8G2_R0, D8, D3, D4);

// RFID RC522 Pins
#define SS_PIN D1
#define RST_PIN D0
MFRC522 rfid(SS_PIN, RST_PIN);

// PIR Pin
#define PIR_PIN D2
bool pirState = false;

// ESP-NOW data struct
typedef struct struct_message {
  float ax;
  float ay;
  float az;
  char direction[10];
} struct_message;

struct_message accelData;

// Track display mode: false = normal, true = Gone
bool goneMode = false;

// PIR motion display tracking
bool displayOn = false;
unsigned long displayStartTime = 0;
const unsigned long displayDuration = 5000; // 5 seconds

// ESP-NOW callback
void OnDataRecv(uint8_t *mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&accelData, incomingData, sizeof(accelData));
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != 0) while(1);
  esp_now_set_self_role(ESP_NOW_ROLE_COMBO);
  esp_now_register_recv_cb(OnDataRecv);

  pinMode(PIR_PIN, INPUT);
  u8g2.begin();

  // RFID Init
  SPI.begin();
  rfid.PCD_Init();
  Serial.println("Place your RFID tag near the reader...");
}

void loop() {
  // Check RFID scan
  if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
    goneMode = !goneMode; // toggle mode
    rfid.PICC_HaltA();    // halt the card
  }

  // Check PIR motion
  pirState = digitalRead(PIR_PIN);
  if (pirState) {
    displayOn = true;
    displayStartTime = millis();
  }

  // Clear display buffer
  u8g2.clearBuffer();

  // Show display for 5 seconds after PIR triggers
  if (displayOn) {
    u8g2.setFont(u8g2_font_ncenB14_tr); // font for direction
    if (goneMode) {
      u8g2.drawStr(0, 40, "Gone");
    } else {
      u8g2.drawStr(0, 40, accelData.direction);
    }

    if (millis() - displayStartTime > displayDuration) {
      displayOn = false; // turn display off
    }
  }

  u8g2.sendBuffer();
  delay(100);
}
