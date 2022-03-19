# pragma once




void screen_setup()
{
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TEXT_COLOR, TFT_BLACK);
  screen_update = 1;
}

void Draw_Battery(int x, int y, int charge)
{
  // tft.fillScreen(TFT_BLACK);
  //Serial.print(charge);
  tft.drawRect(x, y, 21, 8, TFT_SILVER);
  tft.fillRect(x - 4, y + 2, 4, 4, TFT_SILVER);
  if (charge <= 25)tft.fillRoundRect(x + 1, y + 1, 4, 6, 2, TFT_RED);
  if (charge > 25)
  {
    tft.fillRoundRect(x + 1, y + 1, 4, 6, 2, TFT_GREEN);
    tft.fillRoundRect(x + 6, y + 1, 4, 6, 2, TFT_GREEN);
  }

  if (charge > 50)tft.fillRoundRect(x + 11, y + 1, 4, 6, 2, TFT_GREEN);
  if (charge > 75)tft.fillRoundRect(x + 16, y + 1, 4, 6, 2, TFT_GREEN);
}



void draw_time(uint8_t x_pos, uint8_t y_pos)
{
  if ((screen_update) || (targetTime > 60000))
  {
    tft.setTextColor(CLOCK_COLOR, TFT_BLACK);
    tft.fillRect(x_pos, y_pos, 150, tft.fontHeight(7), TFT_BLACK);
    tft.drawString(rtc.getTime("%H:%M"), x_pos, y_pos, 7);
    tft.setTextColor(TEXT_COLOR, TFT_BLACK);
    //time_update = 0;
    targetTime = 0;
    data_bar_update = 1;
  }
}


void draw_time_full(uint8_t x_pos, uint8_t y_pos)
{
  if ((screen_update) || (targetTime > 1000))
  {
    tft.setTextColor(CLOCK_COLOR, TFT_BLACK);
    tft.fillRect(x_pos, y_pos, 200, tft.fontHeight(7), TFT_BLACK);
    tft.drawString(rtc.getTime("%H:%M:%S"), x_pos, y_pos, 7);
    tft.setTextColor(TEXT_COLOR, TFT_BLACK);
    //time_update = 0;
    targetTime = 0;
  }
}


void Display_data()
{
  if (screen_update || sensors_update)
  {
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TEXT_COLOR, TFT_BLACK);
    tft.setCursor (30, 15);
    tft.print(rtc.getTimeDate());

    tft.setCursor (30, 25);
    tft.print("Gyro: ");
    tft.setCursor (30, 35);
    tft.print("X: ");
    tft.print(gX);
    tft.print(" Y: ");
    tft.print(gY);
    tft.print(" Z: ");
    tft.print(gZ);

    tft.setCursor (30, 45);
    tft.print("Accel: ");
    tft.setCursor (30, 55);
    tft.print("X: ");
    tft.print(aX);
    tft.print(" Y: ");
    tft.print(aY);
    tft.print(" Z: ");
    tft.print(aZ);

    tft.setCursor (30, 65);
    tft.print("Compass: ");
    tft.setCursor (30, 75);
    tft.print("X: ");
    tft.print(mX);
    tft.print(" Y: ");
    tft.print(mY);
    tft.print(" Z: ");
    tft.print(mZ);

    tft.setCursor (30, 85);
    tft.print("Azimuth: ");
    tft.print(-1 * Azimuth(mZ, mY));

    tft.setCursor (30, 95);
    tft.print("Temperature: ");
    tft.print(temperature);

    tft.setCursor (30, 105);
    tft.print("Longitude: ");
    tft.print(Lng);
    tft.setCursor (30, 115);
    tft.print("Latitude: ");
    tft.print(Lat);
    tft.setCursor (30, 125);
    tft.print("Altitude: ");
    tft.print(Alt);
    tft.setCursor (30, 135);
    tft.print("Speed: ");
    tft.print(Spd);
    //screen_update = 0;
  }

}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void draw_compass_full(float Azm, int x, int y, float y_coef = 1)
{
  float len = 40;
  float wid = 10;
  float range = 3.5;
  float Nx, Ny, Sx, Sy, Ex, Ey, Wx, Wy;
  if (screen_update || sensors_update)
  {
    tft.fillEllipse(x, y, len+18, (len+18) * y_coef, TFT_BLACK);

    float Ang = (Azm * (PI / 180));

    Sx = x - len * sin(Ang);
    Sy = y + len * cos(Ang) * y_coef;
    Nx = x + len * sin(Ang);
    Ny = y - len * cos(Ang) * y_coef;

    Ex = x - wid * cos(Ang);
    Ey = y - wid * sin(Ang) * y_coef;
    Wx = x + wid * cos(Ang);
    Wy = y + wid * sin(Ang) * y_coef;

    // Draw_Battery(150, 5, BL.getBatteryChargeLevel());
    tft.drawEllipse(x, y, len, len * y_coef, TFT_SILVER);
    tft.fillTriangle(Nx, Ny, Ex, Ey, Wx, Wy, TFT_BLUE);
    tft.fillTriangle(Sx, Sy, Ex, Ey, Wx, Wy, TFT_RED);

    if (got_destination)
    {
      float Dx1, Dy1, Dx2, Dy2, Dx, Dy;
      float D_Ang = ((Azm-Tgt_Azm) * (PI / 180));
      float D_Ang1 = (((Azm-Tgt_Azm) + 10) * (PI / 180));
      float D_Ang2 = (((Azm-Tgt_Azm) - 10) * (PI / 180));

      Dx = x + (len+15) * sin(D_Ang);
      Dy = y - (len+15) * cos(D_Ang) * y_coef;
      Dx1 = x + len * sin(D_Ang1);
      Dy1 = y - len * cos(D_Ang1) * y_coef;
      Dx2 = x + len * sin(D_Ang2);
      Dy2 = y - len * cos(D_Ang2) * y_coef;
      tft.fillTriangle(Dx, Dy, Dx1, Dy1,Dx2, Dy2, TFT_GREEN);

    }
    //screen_update = 0;
  }

}



void draw_compass_small(float Azm, int x, int y, float range = 3.5)
{
  if (screen_update || sensors_update)
  {
    tft.fillRect(0, y - 5, 240, 20, TFT_BLACK);


    for (float i = Azm - 180; i <= Azm + 180; i += 10)
    {
      if (i == Azm)
      {
        tft.drawChar(x + i * range, y, 'N', TFT_BLUE, TFT_BLACK, 2);
      }
      else if ((i == (Azm - 180)) || (i == (Azm + 180)))
      {
        tft.drawChar(x + i * range, y, 'S', TFT_RED,TFT_BLACK, 2);
      }
      else if (i == (Azm - 90))
      {
        tft.drawChar(x + i * range, y, 'W', TFT_GOLD, TFT_BLACK, 2);
      }
      else if (i == (Azm + 90))
      {
        tft.drawChar(x + i * range, y, 'E', TFT_GOLD, TFT_BLACK, 2);
      }

      else if (i == (Azm - Tgt_Azm))
      {
        if (got_destination)tft.drawChar(x + i * range, y, 'T', TFT_GREEN, TFT_BLACK, 2);
      }

      else
      {
        tft.drawChar(x + i * range, y, '|', TFT_SILVER, TFT_BLACK, 1);
      }
    }

    tft.setTextColor(TEXT_COLOR);
  }
}



void Temp_data(int x, int y)
{
  if (screen_update || sensors_update)
  {
    tft.fillRect(x, y, 50, 20, TFT_BLACK);
    tft.setTextColor(TEXT_COLOR, TFT_BLACK);
    tft.setCursor (x, y);
    tft.print("Temperature: ");
    tft.print(temperature);
    tft.print("*C");
  }
}


void Nav_data(int x, int y)
{
  if (screen_update || sensors_update)
  {
    tft.fillRect(x, y, 40, 45, TFT_BLACK);
    tft.setTextColor(TEXT_COLOR, TFT_BLACK);

    tft.setCursor (x, y);
    tft.print("Azimuth: ");
    tft.print(-1 * Azimuth(mZ, mY));


    tft.setCursor (x, y + 10);
    tft.print("My Latitude: ");
    tft.print(Lat);
    tft.setCursor (x, y + 20);
    tft.print("My Longitude: ");
    tft.print(Lng);
    tft.setCursor (x, y + 30);
    tft.print("Altitude: ");
    tft.print(Alt);
    tft.setCursor (x, y + 40);
    tft.print("Speed: ");
    tft.print(Spd);
    tft.setCursor (x, y + 50);
    tft.print("Target Latitude: ");
    tft.print(Tgt_Lat);
    tft.setCursor (x, y + 60);
    tft.print("Target Longitude: ");
    tft.print(Tgt_Lng);
  }

}


void Nav_data_short(int x, int y)
{
  if (screen_update || sensors_update)
  {
    tft.fillRect(x, y, 50, 20, TFT_BLACK);
    tft.setTextColor(TEXT_COLOR, TFT_BLACK);

    tft.setCursor (x, y);
    tft.print("Longitude: ");
    tft.print(Lng);
    tft.setCursor (x, y + 10);
    tft.print("Latitude: ");
    tft.print(Lat);
  }

}


void Display_text(String txt, int pos_x, int pos_y)
{
  if (screen_update)
  {
    tft.setTextSize(1);
    tft.setTextColor(TEXT_COLOR);
    tft.setCursor(pos_x+10, pos_y);
    tft.print(txt);
  }
}


void Incoming_Call(String info, char stat)
{
  //if(screen_update)
  //{
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(ACTIVE_TEXT_COLOR);
  delay(200);

  if (stat == '1') tft.drawCentreString("Incoming call", 100, 40, 4);
  if (stat == '2') tft.drawCentreString("Now calling", 100, 40, 4);

  tft.drawCentreString(info, 100, 65, 4);
  delay(300);
  //}
}


void View_Calls()
{
  if (screen_update)
  {
    short unsigned int output_pointer = call_pointer;
    for (int i = 0; i <= call_counter; i++)
    {
      tft.setTextColor(ACTIVE_TEXT_COLOR);
      //tft.drawString(missed_calls[call_pointer].abonent_name, 20, 20 + i * 10);
      tft.drawString(missed_calls[output_pointer].abonent_num, 20, i * 30);
      tft.setTextColor(TEXT_COLOR);
      tft.drawString(missed_calls[output_pointer].timestamp, 20, 10 + i * 30);
      if (output_pointer-- < 0)output_pointer = call_counter;
    }
  }
}



void View_Messages()
{
  if (screen_update)
  {
    short unsigned int output_pointer = call_pointer;
    for (int i = 0; i <= msg_counter; i++)
    {
      tft.setTextColor(ACTIVE_TEXT_COLOR);
      //tft.drawString(messages[call_pointer].abonent_name, 20, 20 + i * 10);
      tft.drawString(messages[output_pointer].abonent_num, 20, 20 + i * 30);
      tft.setTextColor(TEXT_COLOR);
      tft.drawString(messages[output_pointer].timestamp, 20, 30 + i * 30);
      tft.drawString(String(messages[output_pointer].short_msg_text), 20, 40 + i * 30);
      if (output_pointer-- < 0)output_pointer = msg_counter;
    }
  }
}


void View_Notifications()
{
  if (screen_update)
  {
    short unsigned int output_pointer = call_pointer;
    for (int i = 0; i <= notif_counter; i++)
    {
      tft.setTextColor(TEXT_COLOR);
      tft.drawString(notifications[output_pointer].timestamp, 20, 20 + i * 30);
      tft.drawString(String(notifications[output_pointer].short_notif_text), 20, 30 + i * 30);
      if (output_pointer-- < 0)output_pointer = notif_counter;
    }
  }
}


void Call_Answered(String info)
{
  if (screen_update)
  {
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TEXT_COLOR);
    tft.drawCentreString("Now talking to", 100, 40, 4);
    tft.drawCentreString(info, 100, 65, 4);
    screen_update = 0;
  }
}






void usb_serial_monitor()
{
  String port_data = "";
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(1);
  tft.setTextColor(TEXT_COLOR);
  tft.setCursor(5, 5);

  while (true)
  {
    if (touchRead(T8) < 25)return;
    if (Serial.available() > 0)
    {
      port_data = Serial.readString();
      tft.println(port_data);
    }
    delay(100);
  }
}

void bt_serial_monitor()
{
  String port_data = "";
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(1);
  tft.setTextColor(TEXT_COLOR);
  tft.setCursor(5, 5);

  while (true)
  {
    if (touchRead(T8) < 25)return;
    if (SerialBT.available() > 0)
    {
      port_data = SerialBT.readString();
      tft.println(port_data);
    }
    delay(100);
  }
}


void draw_data_bar()
{
  if (data_bar_update || screen_update)
  {
    tft.setTextColor(TEXT_COLOR);
    tft.fillRect(0, 0, 240, 10, TFT_BLACK);
    tft.drawString(rtc.getTime("%A, %d/%m/%Y"), 30, 7, 1);
    Draw_Battery(180, 5, BL.getBatteryChargeLevel());
    data_bar_update = 0;
  }
}

void msg_icon(int pos_x, int pos_y, int w = 15, int h = 10, int msg_num = 0)
{
  if (notif_update || screen_update)
  {
    tft.fillRect(pos_x - 5, pos_y - 5, w + 10, h + 10, TFT_BLACK);
    tft.fillRoundRect(pos_x, pos_y, w, h, 5, ICON_COLOR);
    tft.drawTriangle(pos_x, pos_y, pos_x + w, pos_y, pos_x + w / 2, pos_y + h / 2, TFT_BLACK);
    if (msg_num > 0)
    {
      tft.setTextColor(ICON_COLOR);
      tft.fillCircle(pos_x + w - 1, pos_y + 1, 7, ACTIVE_ICON_COLOR);
      tft.drawCentreString(String(msg_num), pos_x + w, pos_y - 1, 1);
      tft.setTextColor(TEXT_COLOR);
    }
  }
}


void notif_icon(int pos_x, int pos_y, int w = 15, int h = 10, int notif_num = 0)
{
  if (notif_update || screen_update)
  {
    tft.fillRect(pos_x - 5, pos_y - 5, w + 10, h + 10, TFT_BLACK);
    tft.fillRoundRect(pos_x + w / 2 - w / 10, pos_y, w / 5, h / 6 * 4, 5, ICON_COLOR);
    tft.fillCircle(pos_x + w / 2, pos_y + h / 6 * 5, w / 10, ICON_COLOR);
    if (notif_num > 0)
    {
      tft.setTextColor(ICON_COLOR);
      tft.fillCircle(pos_x + w - 1, pos_y + 1, 7, ACTIVE_ICON_COLOR);
      tft.drawCentreString(String(notif_num), pos_x + w, pos_y - 1, 1);
      tft.setTextColor(TEXT_COLOR);
    }
  }
}


void call_icon(int pos_x, int pos_y, int w = 15, int h = 10, int call_num = 0)
{
  if (notif_update || screen_update)
  {
    tft.fillRect(pos_x - 5, pos_y - 5, w + 10, h + 10, TFT_BLACK);
    tft.fillCircle(pos_x + w / 2, pos_y + h / 2, h / 2, ICON_COLOR);
    tft.fillRoundRect(pos_x + w / 4, pos_y + h / 4, w / 2, h / 2 + 2, 5, TFT_BLACK);
    tft.fillRect(pos_x, pos_y, w / 2, h + 1, TFT_BLACK);
    tft.fillRect(pos_x + w - 3, pos_y, w / 4, h, TFT_BLACK);
    if (call_num > 0)
    {
      tft.setTextColor(ICON_COLOR);
      tft.fillCircle(pos_x + w - 1, pos_y + 1, 7, ACTIVE_ICON_COLOR);
      tft.drawCentreString(String(call_num), pos_x + w, pos_y - 1, 1);
      tft.setTextColor(TEXT_COLOR);
    }
  }
}


void nav_icon(int pos_x, int pos_y, int w = 15, int h = 15, bool nav_active = 0)
{
  if (notif_update || screen_update)
  {
    tft.fillRect(pos_x - 5, pos_y - 5, w + 10, h + 10, TFT_BLACK);
    tft.fillCircle(pos_x + w / 2, pos_y + h / 2, h / 2, ICON_COLOR);
    tft.drawEllipse(pos_x + w / 2, pos_y + h / 2,  h / 3, h / 2, TFT_BLACK);
    tft.drawEllipse(pos_x + w / 2, pos_y + h / 2,  h / 7, h / 2, TFT_BLACK);
    //tft.drawFastVLine(int32_t x, int32_t y, int32_t h,TFT_BLACK);
    for (int i = pos_y + h / 5; i < pos_y + h; i += h / 5)
    {
      tft.drawFastHLine(pos_x, i, w, TFT_BLACK);
    }

    if (!nav_active)
    {
      tft.fillCircle(pos_x + w - 1, pos_y + 1, 7, ACTIVE_ICON_COLOR);

    }
  }
}

void home_icon(int pos_x, int pos_y, int w = 15, int h = 15)
{
  if (notif_update || screen_update)
  {
    tft.fillRect(pos_x - 5, pos_y - 5, w + 10, h + 10, TFT_BLACK);
    tft.fillRect(pos_x + w / 6, pos_y + h / 3, w / 6 * 5, h - h / 3, ICON_COLOR);
    tft.fillTriangle(pos_x, pos_y + h / 3, pos_x + w / 2, pos_y, pos_x + w,  pos_y + h / 3, ICON_COLOR);
    tft.fillRect(pos_x + w / 4 * 3, pos_y, w / 6, h / 3, ICON_COLOR);
    tft.fillRect(pos_x + w / 5 * 2, pos_y + h / 2, w / 5, h / 3, TFT_BLACK);
  }
}

void time_icon(int pos_x, int pos_y, int w = 15, int h = 15)
{
  if (notif_update || screen_update)
  {
    tft.fillRect(pos_x - 5, pos_y - 5, w + 10, h + 10, TFT_BLACK);
    tft.fillCircle(pos_x + w / 2, pos_y + h / 2, h / 2, ICON_COLOR);
    tft.fillCircle(pos_x + w / 2, pos_y + h / 2, h / 10, TFT_BLACK);
    tft.drawFastHLine(pos_x, pos_y + h / 2, w / 8, TFT_BLACK);
    tft.drawFastHLine(pos_x + w - w / 8, pos_y + h / 2, w / 8, TFT_BLACK);
    tft.drawFastVLine(pos_x + w / 2, pos_y, h / 8, TFT_BLACK);
    tft.drawFastVLine(pos_x + w / 2, pos_y + h - h / 8, h / 8, TFT_BLACK);
    tft.drawLine(pos_x + w / 2, pos_y + h / 2, pos_x + w / 2 - w / 3, pos_y + h / 2 - h / 3, TFT_BLACK);
    tft.drawLine(pos_x + w / 2, pos_y + h / 2, pos_x + w / 2 + w / 4, pos_y + h / 2 - h / 4, TFT_BLACK);
  }
}

void text_icon(int pos_x, int pos_y, int w = 15, int h = 15, bool text_active = 0)
{
  if (notif_update || screen_update)
  {
    tft.fillRect(pos_x - 5, pos_y - 5, w + 10, h + 10, TFT_BLACK);
    tft.fillRoundRect(pos_x + w / 6, pos_y, w / 6 * 5, h, w / 8, ICON_COLOR);

    for (int i = pos_y + h / 7; i < pos_y + h / 8 * 7; i += h / 7)
    {
      tft.drawFastHLine(pos_x + w / 4, i, w / 3 * 2, TFT_BLACK);
    }

    if (text_active)
    {
      tft.fillCircle(pos_x + w - 1, pos_y + 1, 7, ACTIVE_ICON_COLOR);

    }
  }
}



void Draw_menu(int active_menu)
{
  if (screen_update)
  {

    for (int i = 0; i < 13; i++)
    {
      if (i == active_menu)
      {
        tft.setTextDatum(3);
        tft.setTextColor(ACTIVE_TEXT_COLOR);
        tft.setTextFont(2);
      }
      tft.drawString(Menu[i], 110, 65 + (i - active_menu) * 15);
      tft.setTextColor(TEXT_COLOR);
      tft.setTextFont(1);
    }
  }

  switch (active_menu)
  {
    case 0:
      active_menu = 1;
      break;
    case 1:
      home_icon(60, 50, 35, 30);
      break;
    case 2:
      time_icon(60, 50, 35, 35);
      break;
    case 3:
      nav_icon(60, 50, 30, 30, got_location);
      break;
    case 4:
      notif_icon(60, 50, 30, 30, call_counter);
      break;
    case 5:
      call_icon(60, 50, 30, 30, call_counter);
      break;
    case 6:
      msg_icon(60, 50, 35, 25, msg_counter);
      break;
    case 7:
      text_icon(60, 50, 30, 30, got_text);
      break;
  }

  //msg_icon(35,20,35,25,msg_counter);
  //call_icon(35,65,30,30,call_counter);
  //nav_icon(35,65,30,30,1);
  //home_icon(35,65,35,30);
  //time_icon(35,65,35,35);
  //text_icon(35,65,30,30,1);
}

void Draw_clock(int ss, int mm, int hh, int cx = 120, int cy = 65, int full_rad = 40)
{
  if ((screen_update) || (targetTime > 1000))
  {
    int mark_x1, mark_y1, mark_x0, mark_y0, mark_len = 5;
    int h_x, h_y, m_x, m_y, s_x, s_y;
    int h_x1, h_y1, m_x1, m_y1, s_x1, s_y1;
    int h_x2, h_y2, m_x2, m_y2;
    int h_len = full_rad / 2, m_len = full_rad / 8 * 6, s_len = full_rad / 8 * 6, h_wid, m_wid, s_wid;
    float h_ang, m_ang, s_ang, h_ang1, m_ang1, h_ang2, m_ang2;

    tft.fillCircle(cx, cy, full_rad + 15, TFT_BLACK);
    tft.drawCircle(cx, cy, full_rad, CLOCK_COLOR);

    tft.drawCentreString("IX", cx - full_rad - 8, cy - 8, 2);
    tft.drawCentreString("III", cx + full_rad + 8, cy - 8, 2);
    tft.drawCentreString("XII", cx , cy - full_rad - 15, 2);
    tft.drawCentreString("VI", cx, cy + full_rad, 2);
    tft.drawCentreString(rtc.getTime(), cx, cy + full_rad / 5, 2);

    for (int i = 0; i < 360; i += 6)
    {
      float rad =  i * PI / 180;
      mark_x1 = cx - full_rad * sin(rad);
      mark_y1 = cy + full_rad * cos(rad);
      if (i % 5 == 0)
      {
        mark_x0 = cx - (full_rad - mark_len * 2) * sin(rad);
        mark_y0 = cy + (full_rad - mark_len * 2) * cos(rad);
      }
      else
      {
        mark_x0 = cx - (full_rad - mark_len) * sin(rad);
        mark_y0 = cy + (full_rad - mark_len) * cos(rad);
      }

      tft.drawLine(mark_x1, mark_y1, mark_x0, mark_y0, CLOCK_COLOR);
    }

    s_ang = (ss * 6 - 180) * PI / 180;          // 0-59 -> 0-354
    m_ang = (mm * 6 - 180) * PI / 180; // 0-59 -> 0-360 - includes seconds
    h_ang = (hh * 30 - 180) * PI / 180; // 0-11 -> 0-360 - includes minutes and seconds

    m_ang1 = (mm * 6 - 165) * PI / 180;
    m_ang2 = (mm * 6 - 195) * PI / 180;

    h_ang1 = (hh * 30 - 160) * PI / 180;
    h_ang2 = (hh * 30 - 200) * PI / 180;


    h_x = cx - h_len * sin(h_ang);
    h_y = cy + h_len * cos(h_ang);

    m_x = cx - m_len * sin(m_ang);
    m_y = cy + m_len * cos(m_ang);

    s_x = cx - s_len * sin(s_ang);
    s_y = cy + s_len * cos(s_ang);



    h_x1 = cx - h_len / 3 * sin(h_ang1);
    h_y1 = cy + h_len / 3 * cos(h_ang1);

    m_x1 = cx - m_len / 4 * sin(m_ang1);
    m_y1 = cy + m_len / 4 * cos(m_ang1);

    h_x2 = cx - h_len / 3 * sin(h_ang2);
    h_y2 = cy + h_len / 3 * cos(h_ang2);

    m_x2 = cx - m_len / 4 * sin(m_ang2);
    m_y2 = cy + m_len / 4 * cos(m_ang2);


    tft.drawLine(cx, cy, s_x, s_y, CLOCK_COLOR);


    tft.drawTriangle(cx, cy, m_x, m_y, m_x1,  m_y1, CLOCK_COLOR);
    tft.drawTriangle(cx, cy, m_x, m_y, m_x2,  m_y2, CLOCK_COLOR);


    tft.fillTriangle(cx, cy, h_x, h_y, h_x1,  h_y1, CLOCK_COLOR);
    tft.fillTriangle(cx, cy, h_x, h_y, h_x2,  h_y2, CLOCK_COLOR);

    tft.fillCircle(cx, cy, full_rad / 20, CLOCK_COLOR);

    targetTime = 0;
  }



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
