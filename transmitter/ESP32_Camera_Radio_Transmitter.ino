// ----------------- User Parameters ----------------- //

#define SERIAL_BAUD_RATE 115200

#include "esp_camera.h"
#include "Arduino.h"
#include "soc/soc.h"           // Disable brownout problems
#include "soc/rtc_cntl_reg.h"  // Disable brownout problems
#include <stdint.h>
#include <stdbool.h>
#include <WiFi.h>
#include <BluetoothSerial.h>

#include <SPI.h>
#include "SX127X.h"

#include <math.h>

BluetoothSerial SerialBT;

#define TRASMISSION_TIMEOUT 10000

#define LED_FLASH 4
#define RADIO_NRST 16
#define RADIO_SS 12  // Pray that the sd card wont spit out anything into this pin
#define RADIO_MISO 2
#define RADIO_MOSI 15
#define RADIO_SCLK 14
#define SD_CARD_SS 13

// define the number of bytes you want to access
#define EEPROM_SIZE 1

// Pin definition for CAMERA_MODEL_AI_THINKER
#define PWDN_GPIO_NUM 32
#define RESET_GPIO_NUM -1
#define XCLK_GPIO_NUM 0
#define SIOD_GPIO_NUM 26
#define SIOC_GPIO_NUM 27

#define Y9_GPIO_NUM 35
#define Y8_GPIO_NUM 34
#define Y7_GPIO_NUM 39
#define Y6_GPIO_NUM 36
#define Y5_GPIO_NUM 21
#define Y4_GPIO_NUM 19
#define Y3_GPIO_NUM 18
#define Y2_GPIO_NUM 5
#define VSYNC_GPIO_NUM 25
#define HREF_GPIO_NUM 23
#define PCLK_GPIO_NUM 22

unsigned long transmissionBeginMillis;

// ----------------- SPI Objects ----------------- //
// Uninitalised pointer to SPI object
SPIClass *radio_spi = NULL;
SX127X_t SX127X;

void camera_turn_on(){
  digitalWrite(PWDN_GPIO_NUM, LOW);
}

void camera_turn_off(){
  digitalWrite(PWDN_GPIO_NUM, HIGH);
}

void camera_config() {

  // source file in: C:\Users\julio\AppData\Local\Arduino15\packages\esp32\hardware\esp32\2.0.6\tools\sdk\esp32\include\esp32-camera\driver\include
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  config.frame_size = FRAMESIZE_CIF;  // FRAMESIZE_ + QVGA|CIF|VGA|SVGA|XGA|SXGA|UXGA
  config.jpeg_quality = 40;
  config.fb_count = 2;
  config.grab_mode = CAMERA_GRAB_LATEST;

  // Init Camera
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }

  // If this is called after the radio has been configured it fucks things up, so do it before
  sensor_t *s = esp_camera_sensor_get();

  s->set_whitebal(s, 1);
  s->set_awb_gain(s, 1);
  s->set_wb_mode(s, 0);
  s->set_exposure_ctrl(s, 1);
  s->set_gain_ctrl(s, 1);
}


int camera_work() {

  camera_fb_t *fb = esp_camera_fb_get();

  if (!fb) {
    Serial.println("Camera capture failed");
    return 0;
  }

  Serial.println("Captured image: \n");
  for (int i = 0; i < fb->len; i++) Serial.printf("%.2X ", *(fb->buf + i));
  Serial.println("");

  // Serial.write(0xDE);
  // Serial.write(0xAD);
  // Serial.write(0xBE);
  // Serial.write(0xEF);

  // Serial.write(fb->buf, fb->len);
  // Serial.flush();

  // Serial.write(0xDE);
  // Serial.write(0xAD);
  // Serial.write(0xBE);
  // Serial.write(0xEF);

  transmit_image(fb->buf, fb->len);

  esp_camera_fb_return(fb);
  return 1;
}

void transmit_image(uint8_t *pPhotoBytes, size_t inSize) {

  int bytesReadUntilNow = 0;
  int packetLength;
  int packetCounter = 0;
  uint8_t packet[255];
  uint16_t numberOfPackets = (uint16_t)ceil(inSize / 255.0);

  Serial.println("\nBeginning photo transmission.");
  Serial.println("Sending header info.");
  Serial.printf("inSize: %d    numberOfPackets: %d", inSize, numberOfPackets);

  packet[0] = 'Z';
  packet[1] = 'E';
  packet[2] = 'N';
  packet[3] = 'I';
  packet[4] = 'T';
  packet[5] = 'H';
  packet[6] = 0;
  packet[7] = (uint8_t)numberOfPackets;
  packet[8] = (uint8_t)numberOfPackets >> 8;

  Serial.println("\nPacket: \n");
  for (int i = 0; i < 9; i++) Serial.printf("%.2X ", packet[i]);
  Serial.println("");

  FSK_Transmit(&SX127X, packet, 9);
  while (!FSK_CheckFIFOEmpty(&SX127X)) delay(1);
  delay(30);  // Give time for the receiver to prepare

  //Serial.println("Sending Packets.");
  transmissionBeginMillis = millis();

  while (true) {
    if (bytesReadUntilNow + 255 < inSize) {
      memcpy(packet, pPhotoBytes + bytesReadUntilNow, 255);
      packetLength = 255;
      bytesReadUntilNow += 255;
    } else {
      memcpy(packet, pPhotoBytes + bytesReadUntilNow, (inSize - bytesReadUntilNow));
      packetLength = inSize - bytesReadUntilNow;
    }

    //Serial.printf("\nPacket: %d\n", packetCounter);
    //for (int i = 0; i < packetLength; i++) Serial.printf("%.2X ", packet[i]);
    //Serial.println("");

    FSK_BigTransmit(&SX127X, packet, packetLength);
    packetCounter++;
    delay(50);  // 30 was as quick as I could do it  // Give time for the receiver to think

    // Note: Ideally this would be a DIO0 hardware interrupt, but since the ESPCam is a piece of garbage
    // with a screaming abssence of pins, this had to be done via software, by checking the FIFO empty flag. - Julio
    //Serial.print("\nWaiting for packet transmission confirmation.");
    while (!FSK_CheckFIFOEmpty(&SX127X)) delay(1);

    // Last package has been sent or there was a timeout in transmission
    if (packetLength < 255 || (millis() - transmissionBeginMillis > TRASMISSION_TIMEOUT)) {
      break;
    } else Serial.print(".");
  }

  analogWrite(LED_FLASH, 2);
  delay(5);
  analogWrite(LED_FLASH, 0);

  Serial.println("\nDone");
}


void setup() {


  SerialBT.begin("ESP32test");
  delay(100);
  SerialBT.end();  //calls btStop();

  WiFi.mode(WIFI_OFF);

  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);  //disable brownout detector

  Serial.begin(115200);

  // Turns off the ESP32-CAM white on-board LED (flash) connected to GPIO 4
  pinMode(LED_FLASH, OUTPUT);
  digitalWrite(LED_FLASH, LOW);

  pinMode(PWDN_GPIO_NUM, OUTPUT);



  // source file in: C:\Users\julio\AppData\Local\Arduino15\packages\esp32\hardware\esp32\2.0.6\tools\sdk\esp32\include\esp32-camera\driver\include
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  config.frame_size = FRAMESIZE_CIF;  // FRAMESIZE_ + QVGA|CIF|VGA|SVGA|XGA|SXGA|UXGA
  config.jpeg_quality = 40;
  config.fb_count = 2;
  config.grab_mode = CAMERA_GRAB_LATEST;

  // Init Camera
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }

  // If this is called after the radio has been configured it fucks things up, so do it before
  sensor_t *s = esp_camera_sensor_get();

  s->set_whitebal(s, 1);
  s->set_awb_gain(s, 1);
  s->set_wb_mode(s, 0);
  s->set_exposure_ctrl(s, 1);
  s->set_gain_ctrl(s, 1);

  // Now the radio:

  // ----- SPI initialization ----- //
  radio_spi = new SPIClass(VSPI);

  radio_spi->begin(RADIO_SCLK, RADIO_MISO, RADIO_MOSI, RADIO_SS);  //SCLK, MISO, MOSI, SS
  // Set up slave select pins as outputs as the Arduino API doesn't handle
  // automatically pulling SS low, so each library (radio/flash) handles it idenpendetly
  pinMode(radio_spi->pinSS(), OUTPUT);  //VSPI SS
  pinMode(RADIO_NRST, OUTPUT);

  FSKConfig(&SX127X);

  // Sanity check:
  //uint8_t test[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
  //FSK_BigTransmit(&SX127X, test, 10);
  //delay(200);
}

void loop() {

  //camera_turn_on();
  //camera_config();
  // FSKConfig(&SX127X);
  camera_work();
  //camera_turn_off();  

  delay(10000);
}





// typedef enum {
//     FRAMESIZE_96X96,    // 96x96
//     FRAMESIZE_QQVGA,    // 160x120
//     FRAMESIZE_QCIF,     // 176x144
//     FRAMESIZE_HQVGA,    // 240x176
//     FRAMESIZE_240X240,  // 240x240
//     FRAMESIZE_QVGA,     // 320x240
//     FRAMESIZE_CIF,      // 400x296
//     FRAMESIZE_HVGA,     // 480x320
//     FRAMESIZE_VGA,      // 640x480
//     FRAMESIZE_SVGA,     // 800x600
//     FRAMESIZE_XGA,      // 1024x768
//     FRAMESIZE_HD,       // 1280x720
//     FRAMESIZE_SXGA,     // 1280x1024
//     FRAMESIZE_UXGA,     // 1600x1200
//     // 3MP Sensors
//     FRAMESIZE_FHD,      // 1920x1080
//     FRAMESIZE_P_HD,     //  720x1280
//     FRAMESIZE_P_3MP,    //  864x1536
//     FRAMESIZE_QXGA,     // 2048x1536
//     // 5MP Sensors
//     FRAMESIZE_QHD,      // 2560x1440
//     FRAMESIZE_WQXGA,    // 2560x1600
//     FRAMESIZE_P_FHD,    // 1080x1920
//     FRAMESIZE_QSXGA,    // 2560x1920
//     FRAMESIZE_INVALID
// } framesize_t;
