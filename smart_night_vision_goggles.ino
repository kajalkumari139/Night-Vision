#include "esp_camera.h"
#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();

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

void setup()
{
    Serial.begin(115200);

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

    config.pixel_format = PIXFORMAT_RGB565;

    /*
     * SMALLER FRAME
     */
    config.frame_size = FRAMESIZE_QQVGA;
    config.fb_count = 1;

    if (esp_camera_init(&config) != ESP_OK)
    {
        Serial.println("CAM FAIL");
        while (true);
    }

    tft.init();
    tft.setRotation(0);
    tft.fillScreen(TFT_BLACK);

    Serial.println("READY");
}

void loop()
{
    camera_fb_t *fb = esp_camera_fb_get();

    if (!fb)
        return;

    uint16_t *img = (uint16_t *)fb->buf;

    /*
     * Scale vertically:
     * take 60 rows
     * display as 120 rows
     */

    for (int y = 0; y < 60; y++)
    {
        uint16_t *row = img + ((y * 2) * 160) + 16;

        /*
         * draw same row twice
         */
        tft.pushImage(0, 20 + (y * 2), 128, 1, row);
        tft.pushImage(0, 21 + (y * 2), 128, 1, row);
    }

    esp_camera_fb_return(fb);
    delay(20);
}
