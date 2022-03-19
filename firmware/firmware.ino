#include "Settings.h"
#include "Sys_var.h"
#include "Sensors.h"
#include "mob_data.h"
#include "WiFi_ctrl.h"
#include "Bt_ctrl.h"
#include "Graphics.h"




void setup() {
  Serial.begin(115200);
  SerialBT.begin("AReyes Vision Z3R0"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");
  EEPROM.begin(EEPROM_SIZE);

  Wire.begin(SDA_PIN, SCL_PIN);
  mySensor.setWire(&Wire);

  mySensor.beginAccel();
  mySensor.beginGyro();
  mySensor.beginMag();

  bmp.begin(0x76);//specified adress for module
  apds.begin();
  apds.enableProximity(true);
  apds.enableGesture(true);
  pinMode(14, OUTPUT);
  digitalWrite(14, HIGH);


  float X_offset;
  float Y_offset;
  float Z_offset;

  EEPROM.get(0, X_offset);
  EEPROM.get(5, Y_offset);
  EEPROM.get(10, Z_offset);

  mySensor.magXOffset = X_offset;
  mySensor.magYOffset = Y_offset;
  mySensor.magZOffset = Z_offset;
  //SyncTime();
  //BT_time()
  //Advance_time(0);

  screen_setup();
  targetTime = millis() + 1000;

  touchAttachInterrupt(T5, touch0_detected, touch_threshold);
  touchAttachInterrupt(T7, touch1_detected, touch_threshold);
  touchAttachInterrupt(T8, touch2_detected, touch_threshold);
  touchAttachInterrupt(T9, touch3_detected, touch_threshold);
  esp_sleep_enable_touchpad_wakeup();
  //
  //  Serial.print("ESP Board MAC Address:  ");
  //  Serial.println(WiFi.macAddress());


  apds.setGestureGain(APDS9960_GGAIN_1);


}






void loop() {
  bt_read();
  update_timers();
  read_touch();
  update_sensors(500);
  handleGesture();
  
  if (screen_update)tft.fillScreen(TFT_BLACK );
  switch (mode)
  {

    case -3:
      Incoming_Call(call_info, call_status);
      manage_calls();
      break;

    case -2:
      Call_Answered(call_info);
      manage_calls();
      break;

    case -1:
      calibrate();
      mode = 0;
      break;

    case 0:
       Draw_menu(active_menu);
     

      break;

    case 1:
            draw_time(80, 40);
            draw_compass_full(Azimuth(mZ, mY), 145, 100, 0.3);
            Nav_data_short(100, 65);
            draw_compass_small(Azimuth(mZ, mY), 145, 120);
            msg_icon(35, 20, 35, 25, msg_counter);
            call_icon(35, 65, 30, 30, call_counter);
      
      break;

    case 2:
      //draw_time_full(20,65);
      Draw_clock(rtc.getSecond(), rtc.getMinute(), rtc.getHour());
      
      break;

    case 3:
      Nav_data(20, 20);
      draw_compass_full(Azimuth(mZ, mY), 150, 50, 0.3);
      draw_compass_small(Azimuth(mZ, mY), 150, 100);
      
      break;

    case 4:
      View_Notifications();
      
      break;

    case 5:
      View_Calls();
      
      break;

    case 6:
      View_Messages();
      
      break;

    case 7:
      Display_text(cast_text, 0, 15);
      
      break;

    case 8:
      bt_serial_monitor();
      
      break;

    case 9:
      usb_serial_monitor();
      
      break;

    case 10:
      Display_data();
      
      break;

    case 11:
      //wifi_scan();
      
      break;

    case 12:
      //wifi_hack();
     
      break;

  }
  draw_data_bar();
  screen_update = 0;
  sensors_update = 0;

  //  if(
  //    (touch_timer3>60000)&&
  //    (touch_timer2>60000)&&
  //    (touch_timer1>60000)
  //    )esp_deep_sleep_start();
}


void calibrate()
{
  //SyncTime();

  tft.fillScreen(TFT_BLACK);
  tft.drawRightString("Spin for 20 sec", 200, 50, 4);


  setMagMinMaxAndSetOffset(&mySensor, CALIB_SEC);

  screen_update = 1;
  mode = 0;
}

/*
  code  color
  0x0000  Black
  0xFFFF  White
  0xBDF7  Light Gray
  0x7BEF  Dark Gray
  0xF800  Red
  0xFFE0  Yellow
  0xFBE0  Orange
  0x79E0  Brown
  0x7E0 Green
  0x7FF Cyan
  0x1F  Blue
  0xF81F  Pink

*/
