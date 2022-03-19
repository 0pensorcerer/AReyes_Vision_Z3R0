# pragma once

#include <ESP32Time.h>
#include <WiFi.h>
#include "BluetoothSerial.h"
#include "time.h"
#include <Pangodream_18650_CL.h>
#include <MPU9250_asukiaaa.h>
#include <Adafruit_BMP280.h>
#include <Wire.h>
#include "Adafruit_APDS9960.h"

#include <SPI.h>
#include <TFT_eSPI.h>
#include "EEPROM.h"


#define EEPROM_SIZE 512
#define TFT_BL 4


BluetoothSerial SerialBT;
TFT_eSPI tft = TFT_eSPI();
MPU9250_asukiaaa mySensor;
Adafruit_APDS9960 apds;
Adafruit_BMP280 bmp;
Pangodream_18650_CL BL(ADC_PIN, CONV_FACTOR, READS);



bool screen_update = 0;
bool time_update = 0;
bool sensors_update = 0;
bool data_bar_update = 0;
bool viewport_update = 0;
bool notif_update = 0;

uint32_t targetTime = 0;
uint32_t sensorTime = 0;
uint32_t touch_timer1 = 0;
uint32_t touch_timer2 = 0;
uint32_t touch_timer3 = 0;
uint32_t Elapsed_time = 0;

short int mode = 0;
unsigned short int active_menu = 1;
bool got_location=0;
bool got_destination=0;
bool got_text =0;

//struct tm timeinfo;
RTC_DATA_ATTR ESP32Time rtc;
//
//char Year[5];
//char Month[20];
//char Day_of_week[10];
//char Day_of_month[3];
//char Hour[3];
//char Min[3];
//char Sec[3];
//char Date[12];//DD.MM.YYYY
//char Time[9];//hh:mm:ss
//char Short_time[6];

String bt_command;

float t_y;
String cast_text;
String notification;
String call_info;
char call_status;

const String Menu[] = 
{
  "Menu", 
  "Home", 
  "Time", 
  "Navigation",
  "Notifications", 
  "Missed calls",
  "SMS", 
  "Text projection",
  "BT_serial",
  "USB_serial" ,
  "Sensor Data",
  "WiFi Scan", 
  "Deauth" 
  };


float aX, aY, aZ, gX, gY, gZ, mX, mY, mZ,temperature;
float Lat, Lng, Alt, Spd;
float Tgt_Lat, Tgt_Lng, Tgt_Azm;
int Tgt_Dist;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
