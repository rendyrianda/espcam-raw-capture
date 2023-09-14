# ESP32-CAM Image Capture and Management

This is a simple Arduino program for the ESP32-CAM module that allows you to capture images, save them to the SPIFFS (SPI Flash File System), check the size of the saved image, delete the image, and display it over a serial connection. It also accepts commands via a serial interface.

## Requirements
- [ESP32-CAM](https://www.espressif.com/en/products/socs/esp32-cam/overview) module
- Arduino IDE with ESP32 board support
- [esp_camera library](https://github.com/espressif/esp32-camera) installed
- SPIFFS library

## Installation
1. Make sure you have the required libraries installed in your Arduino IDE.
2. Upload the sketch to your ESP32-CAM module.

## Usage
1. Connect a serial monitor to your ESP32-CAM to interact with the program.
2. Send the following commands to the ESP32-CAM:
   - `CAPTURE`: Capture an image and save it to SPIFFS as "capture.jpg".
   - `SIZE`: Check the size of the saved image.
   - `DELETE`: Delete the saved image.
   - `SHOW`: Display the saved image over the serial connection.

## Functions
- `captureImageAndSave()`: Captures an image using the ESP32-CAM and saves it as "capture.jpg" to SPIFFS.
- `printImageSize()`: Prints the size of the saved image.
- `deleteImage()`: Deletes the saved image from SPIFFS.
- `showImage()`: Displays the saved image over the serial connection in chunks.

## Configuration
The program initializes the ESP32-CAM module with the following camera configuration:
- Camera model: AI Thinker ESP32-CAM
- Image format: JPEG
- Frame size: QQVGA (if PSRAM is found), otherwise SVGA
- JPEG quality: 40 (if PSRAM is found), otherwise 12
- Other camera settings: Brightness, contrast, saturation, special effects, white balance, exposure control, auto gain control, and more.

You can adjust these camera settings in the `setup()` function to fit your requirements.

**Note:** Make sure you have set the correct GPIO pins in the `camera_pins.h` file for your ESP32-CAM module.

## Troubleshooting
- If you encounter issues with SPIFFS, ensure that you have formatted the SPIFFS partition properly and have sufficient space available.
- Check the serial monitor for error messages or debugging information.
