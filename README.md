## Code
- [esp8266 with display,rfid,pir](display_esp8266/display_esp8266.ino)
- [esp8266 with  MPU6050](acce_esp8266/acce_esp8266.ino)



## Libraries Required

1. **ESP8266WiFi.h**  
   - Provides Wi-Fi functionality for ESP8266 in Station/Access Point mode.

2. **espnow.h**  
   - Enables ESP-NOW communication protocol for direct peer-to-peer communication.

3. **U8g2lib.h**  
   - Used to control the SH1106 OLED display.

4. **SPI.h**  
   - Provides SPI communication required by the RFID module.

5. **MFRC522.h**  
   - Used to interface with the RC522 RFID reader.


## MAC Address (mine)
- EPS with gy - 34:5F:45:58:AB:62
- ESP with display - 34:5F:45:55:E4:34

## Component used
- [ESP8266](https://store.roboticsbd.com/development-boards/694-esp8266-ch340-nodemcu-wifi-module-lua-v3-robotics-bangladesh.html)
- [PIR sensor](https://store.roboticsbd.com/sensors/2339-am312-mini-pir-motion-sensor-robotics-bangladesh.html)
- [DC vibration motor](https://store.roboticsbd.com/vibration-sensor-robotics-bangladesh/1932-dc-motor-mobile-phone-vibrator-vibration-motor-alarm-module-black-robotics-bangladesh.html)
- [1.3' OLED display](https://store.roboticsbd.com/display/2050-13-inch-spi-oled-display-module-7pin-blue-robotics-bangladesh.html)
- [GY-87 10DOF](https://store.roboticsbd.com/sensors/382-gy-87-10dof-mpu6050-hmc5883l-bmp180-sensor-module-robotics-bangladesh.html)
- [RC522 RFID Module](https://store.roboticsbd.com/arduino-shield/313-rc522-rfid-card-reader-module-kit-android-nfc-supported-robotics-bangladesh.html)


## pir sensor pinout 
<img width="500" height="500" alt="Image" src="https://github.com/user-attachments/assets/a86779a7-fd3f-4f25-b7e7-55a97493e1fb" />

## pir to esp8266

| OLED Pin | ESP8266 (NodeMCU) |
| -------- | ----------------- |
| VCC      | 3.3V              |
| GND      | GND               |
| OUT      | D2                |


---

## 1.3'' spi oled display pinout
| OLED Pin | ESP8266 (NodeMCU) |
| -------- | ----------------- |
| VCC      | 3.3V              |
| GND      | GND               |
| SCL (D0) | D5 (GPIO14)       |
| SDA (D1) | D7 (GPIO13)       |
| RES      | D4 (GPIO2)        |
| DC       | D3 (GPIO0)        |
| CS       | D8 (GPIO15)       |


---
## MPU6050 sensor


| MPU6050 | ESP8266        |
|---------|----------------|
| VCC     | 3.3V           |
| GND     | GND            |
| SCL     | D1 (GPIO5)     |
| SDA     | D2 (GPIO4)     |


---
## ESP8266 D1 Mini Motor Control Wiring

| ESP8266             | Motor Pin       |
|---------------------|-----------------|
| D5                  | IN (Control)    | 
| 3.3V                | VCC             |
| GND                 | GND             | 

---
## RFID to ESP8266
| RC522 Pin | ESP8266 Pin |
| --------- | ----------- |
| SDA (CS)  | D1          |
| SCK       | D5          |
| MOSI      | D7          |
| MISO      | D6          |
| RST       | D0          |
| 3.3V      | 3.3V        |
| GND       | GND         |

