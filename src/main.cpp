#include <Arduino.h>
#include "esp_camera.h"
#include <WiFi.h>
#define Y2_GPIO_NUM 2 // GPIO2 is used for Y2
#define Y3_GPIO_NUM 4

void setup()
{
  Serial.begin(115200);
  WiFi.begin("ssid", "password");
  setupCamera();
}

void loop()
{
  camera_fb_t *img1 = esp_camera_fb_get();
  delay(5000); // capture every 5 seconds
  camera_fb_t *img2 = esp_camera_fb_get();

  if (compareImages(img1, img2))
  {
    Serial.println("Detected changes!");
    // Send image to the server
  }

  esp_camera_fb_return(img1);
  esp_camera_fb_return(img2);
}

bool compareImages(camera_fb_t *img1, camera_fb_t *img2)
{
  int pixel_diff = 0;
  for (int i = 0; i < img1->len; i++)
  {
    if (abs(img1->buf[i] - img2->buf[i]) > 20)
    { // Ngưỡng khác biệt pixel
      pixel_diff++;
    }
  }
  return (pixel_diff > 10000); // Threshold of diff pixel
}

void setupCamera()
{
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  esp_camera_init(&config);
}