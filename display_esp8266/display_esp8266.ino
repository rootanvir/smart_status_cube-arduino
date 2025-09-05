#include <U8g2lib.h>
#include <SPI.h>

// SH1106 SPI constructor (CS, DC, RST)
U8G2_SH1106_128X64_NONAME_F_4W_HW_SPI u8g2(U8G2_R0, D8, D3, D4);

void setup() {
  u8g2.begin();
}

void loop() {
  u8g2.clearBuffer();                 // Clear memory
  u8g2.setFont(u8g2_font_ncenB14_tr); // Select font
  u8g2.drawStr(10, 30, "Hello Boss"); // Draw text
  u8g2.sendBuffer();                  // Send to display
  delay(1000);
}
