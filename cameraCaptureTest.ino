#include "esp_camera.h"
#include "FS.h" 
#include "SPIFFS.h"

#define FLASH_LED_PIN 4
#define CAMERA_MODEL_AI_THINKER
#include "camera_pins.h"


void captureImageAndSave() {
  // Turn on the light
  digitalWrite(FLASH_LED_PIN, HIGH);

  camera_fb_t *fb = NULL;
  fb = esp_camera_fb_get();
  
  if (!fb) {
    Serial.println("Camera capture failed");
    return;
  }

  // Save the image to SPIFFS
  File file = SPIFFS.open("/capture.jpg", FILE_WRITE);
  
  if (!file) {
    Serial.println("Failed to open file for writing");
    return;
  }
  
  file.write(fb->buf, fb->len);
  file.close();
  esp_camera_fb_return(fb);

  // Turn off the light
  digitalWrite(FLASH_LED_PIN, LOW);
}

void printImageSize() {
  if (SPIFFS.exists("/capture.jpg")) {
    File file = SPIFFS.open("/capture.jpg");
    Serial.print("Size of capture.jpg: ");
    Serial.println(file.size());
    file.close();
  } else {
    Serial.println("File does not exist");
  }
}

void deleteImage() {
  if (SPIFFS.remove("/capture.jpg")) {
    Serial.println("File deleted successfully");
  } else {
    Serial.println("File deletion failed");
  }
}

void showImage() {
  File file = SPIFFS.open("/capture.jpg");
  if (!file) {
    Serial.println("Failed to open file for reading");
    return;
  }

  size_t chunkSize = 1024; // Change this to the size you want
  uint8_t* buffer = new uint8_t[chunkSize];
  size_t n;

  // Use file.read() to read the file in chunks
  while ((n = file.read(buffer, chunkSize)) > 0) {
    // Write the chunk binary data to Serial
    Serial.write(buffer, n);
  }

  delete[] buffer;
  file.close();
}

void setup() {
  Serial.begin(115200);
  pinMode(FLASH_LED_PIN, OUTPUT);
  // Initialize the SPIFFS
  if (!SPIFFS.begin(true)) {
    Serial.println("An error has occurred while mounting SPIFFS");
    return;
  }

  // Initialize the camera for AI Thinker ESP32-CAM module
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
  if (psramFound()) {
    config.frame_size = FRAMESIZE_QQVGA;
    config.jpeg_quality = 40;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }
  
  // Initialize the camera with the configuration
  esp_err_t err = esp_camera_init(&config);
  
  if (err != ESP_OK) {
    Serial.printf("Camera initialization failed with error 0x%x", err);
    return;
  }
  
  Serial.println("Camera initialized successfully!");
  sensor_t * s = esp_camera_sensor_get();
  s->set_brightness(s, 0);     // -2 to 2
  s->set_contrast(s, 0);       // -2 to 2
  s->set_saturation(s, 0);     // -2 to 2
  s->set_special_effect(s, 0); // 0 to 6 (0 - No Effect, 1 - Negative, 2 - Grayscale, 3 - Red Tint, 4 - Green Tint, 5 - Blue Tint, 6 - Sepia)
  s->set_whitebal(s, 1);       // 0 = disable , 1 = enable
  s->set_awb_gain(s, 1);       // 0 = disable , 1 = enable
  s->set_wb_mode(s, 0);        // 0 to 4 - if awb_gain enabled (0 - Auto, 1 - Sunny, 2 - Cloudy, 3 - Office, 4 - Home)
  s->set_exposure_ctrl(s, 1);  // 0 = disable , 1 = enable
  s->set_aec2(s, 0);           // 0 = disable , 1 = enable
  s->set_ae_level(s, 0);       // -2 to 2
  s->set_aec_value(s, 300);    // 0 to 1200
  s->set_gain_ctrl(s, 1);      // 0 = disable , 1 = enable
  s->set_agc_gain(s, 0);       // 0 to 30
  s->set_gainceiling(s, (gainceiling_t)0);  // 0 to 6
  s->set_bpc(s, 0);            // 0 = disable , 1 = enable
  s->set_wpc(s, 1);            // 0 = disable , 1 = enable
  s->set_raw_gma(s, 1);        // 0 = disable , 1 = enable
  s->set_lenc(s, 1);           // 0 = disable , 1 = enable
  s->set_hmirror(s, 0);        // 0 = disable , 1 = enable
  s->set_vflip(s, 0);          // 0 = disable , 1 = enable
  s->set_dcw(s, 1);            // 0 = disable , 1 = enable
  s->set_colorbar(s, 0);       // 0 = disable , 1 = enable
}

void loop() {
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    command.trim();

    if (command == "CAPTURE") {
      captureImageAndSave();
    } else if (command == "SIZE") {
      printImageSize();
    } else if (command == "DELETE") {
      deleteImage();
    } else if (command == "SHOW") {
      showImage();
    } else {
      Serial.println("Invalid command");
    }
  }
}

