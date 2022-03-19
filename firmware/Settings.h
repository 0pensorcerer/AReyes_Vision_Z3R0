# pragma once

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `mahttps menuconfig` to and enable it
#endif

#define SDA_PIN 21
#define SCL_PIN 22
#define CALIB_SEC 20
#define touch_threshold 30

#define ADC_PIN 34
#define CONV_FACTOR 1.8
#define READS 20

const char* ssid     = "the_dark_lord";
const char* password = "13371488";

const char* ntpServer = "pool.ntp.org";
const int  gmtOffset_sec = 7200;
const int   daylightOffset_sec = 3600;

/////////////////////////////////////////////////////////////////////////
uint16_t TEXT_COLOR = 0x07E0;
uint16_t CLOCK_COLOR = 0xFDA0;
uint16_t ACTIVE_TEXT_COLOR = 0x07FF;
uint16_t ICON_COLOR = 0xC618;
uint16_t ACTIVE_ICON_COLOR = 0xF800;
////////////////////////////////////////////////////////////////////
