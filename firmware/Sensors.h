

# pragma once


void handleGesture()
{
  uint8_t gesture = apds.readGesture();
  switch (gesture)
  {
    case APDS9960_UP:
      if (mode == 0)
      {
        if (active_menu++ >= 12)active_menu = 1;
      }
      screen_update = 1;

      break;
    case APDS9960_DOWN:
      if (mode == 0)
      {
        if (active_menu-- <= 1)active_menu = 11;
      }
      screen_update = 1;

      break;
    case APDS9960_LEFT:
      if (mode == 0)mode = active_menu;
      screen_update = 1;
      break;

    case APDS9960_RIGHT:
      if (mode == 0)mode = 1;
      else mode = 0;
      screen_update = 1;
      break;
  }
}

void touch0_detected()
{
  //mode = 0;
}

void touch1_detected()
{
  if ((mode == 0) && (active_menu > 1) && (touch_timer1 > 300))
  {
    if (active_menu-- < 1)active_menu = 11;
    touch_timer1 = 0;
    screen_update = 1;
  }
  //Serial.println("touch1");
}

void touch2_detected()
{
  if ((mode == 0) && (touch_timer2 > 300))

  {
    touch_timer2 = 0;
    screen_update = 1;
    mode = active_menu;
  }

  else if ((mode != 0) && (touch_timer2 > 300))
  {

    touch_timer2 = 0;
    screen_update = 1;
    mode = 0;
  }
  //Serial.println("touch1");

}

void touch3_detected()
{
  if ((mode == 0) && /*(active_menu < 8) &&*/ (touch_timer3 > 300))
  {
    if (active_menu++ >= 12)active_menu = 1;
    touch_timer3 = 0;
    screen_update = 1;
  }
  //Serial.println("touch1");
}



void read_touch()
{
  if (touchRead(T7) < 25) touch1_detected();
  if (touchRead(T8) < 25) touch2_detected();
  if (touchRead(T9) < 25) touch3_detected();
}


void update_timers()
{
  touch_timer1 += millis() - Elapsed_time;
  touch_timer2 += millis() - Elapsed_time;
  touch_timer3 += millis() - Elapsed_time;
  targetTime += millis() - Elapsed_time;
  sensorTime += millis() - Elapsed_time;
  Elapsed_time = millis();
}


float Azimuth(float x, float y)
{
  return atan2(x, y) * 180 / PI;
}

void setMagMinMaxAndSetOffset(MPU9250_asukiaaa * sensor, int seconds) {
  unsigned long calibStartAt = millis();
  float magX, magXMin, magXMax, magY, magYMin, magYMax, magZ, magZMin, magZMax;
  float X_offset, Y_offset, Z_offset;

  sensor->magUpdate();
  magXMin = magXMax = sensor->magX();
  magYMin = magYMax = sensor->magY();
  magZMin = magZMax = sensor->magZ();

  while (millis() - calibStartAt < (unsigned long) seconds * 1000) {
    delay(100);
    sensor->magUpdate();
    magX = sensor->magX();
    magY = sensor->magY();
    magZ = sensor->magZ();
    if (magX > magXMax) magXMax = magX;
    if (magY > magYMax) magYMax = magY;
    if (magZ > magZMax) magZMax = magZ;
    if (magX < magXMin) magXMin = magX;
    if (magY < magYMin) magYMin = magY;
    if (magZ < magZMin) magZMin = magZ;
  }

  Serial.println("magXOffset");
  Serial.print(- (magXMax + magXMin) / 2);
  Serial.println("magYOffset");
  Serial.print(- (magYMax + magYMin) / 2);
  Serial.println("magZOffset");
  Serial.print(- (magZMax + magZMin) / 2);

  X_offset = - (magXMax + magXMin) / 2;
  Y_offset = - (magYMax + magYMin) / 2;
  Z_offset = - (magZMax + magZMin) / 2;

  EEPROM.put(0, X_offset);
  EEPROM.put(5, X_offset);
  EEPROM.put(10, X_offset);

  sensor->magXOffset = X_offset;
  sensor->magYOffset = Y_offset;
  sensor->magZOffset = Z_offset;
}



void update_sensors(uint32_t upd_rate)
{
  if (sensorTime > upd_rate)
  {
    uint8_t sensorId;
    if (mySensor.readId(&sensorId) == 0) {
    } else {
      Serial.println("Cannot read sensorId");
    }

    if (mySensor.accelUpdate() == 0) {
      aX = mySensor.accelX();
      aY = mySensor.accelY();
      aZ = mySensor.accelZ();
    } else {
      Serial.println("Cannod read accel values");
    }

    if (mySensor.gyroUpdate() == 0) {
      gX = mySensor.gyroX();
      gY = mySensor.gyroY();
      gZ = mySensor.gyroZ();
    } else {
      Serial.println("Cannot read gyro values");
    }

    if (mySensor.magUpdate() == 0) {
      mX = mySensor.magX();
      mY = mySensor.magY();
      mZ = mySensor.magZ();

    } else {
      Serial.println("Cannot read mag values");
    }
    temperature = bmp.readTemperature();
    //screen_update = 1;
    sensors_update = 1;
    sensorTime = 0;
  }
}


//void Advance_time( uint32_t upd_rate)
//{
//  if (targetTime < millis())
//  {
//    targetTime = millis() + upd_rate;
//    time_update = 1;
//  }
//}



void BT_time()
{
  String buf = "";
  unsigned short int read_state = 0;
  char temp;
  unsigned short int new_sec, new_min, new_hour, new_day, new_month, new_year, new_week_day;

  while (SerialBT.available() > 0)
  {
    temp = SerialBT.read();
    if ((temp == '|') || (temp == '\0'))
    {
      switch (read_state)
      {
        case 0:

          new_sec = buf.toInt();
          break;

        case 1:

          new_min = buf.toInt();
          break;

        case 2:

          new_hour = buf.toInt();
          break;

        case 3:
          new_day = buf.toInt();
          break;

        case 4:
          new_month = buf.toInt();
          break;

        case 5:
          new_year = buf.toInt();
          break;
      }
      Serial.print(buf + temp);
      Serial.println();
      buf = "";
      read_state++;
    }
    else
    {
      buf += temp;
    }
  }
  Serial.println(String(new_sec) + ':' + String(new_min) + ':' + String(new_hour) + ' ' + String(new_day) + '/' + String(new_month) + '/' + String(new_year));
  rtc.setTime(new_sec, new_min, new_hour, new_day, new_month, new_year);
  //timeinfo = rtc.getTimeStruct();
  Serial.println(rtc.getTimeDate(true));  //  (String) 15:24:38 Sunday, January 17 2021
  targetTime = new_sec * 1000;
  screen_update = 1;
}




void get_location()
{
  String buf = "";
  unsigned short int read_state = 0;
  char temp;

  while (SerialBT.available() > 0)
  {
    temp = SerialBT.read();
    if ((temp == '|') || (temp == '\0'))
    {
      switch (read_state)
      {
        case 0:

          Lat = buf.toFloat();
          break;

        case 1:

          Lng = buf.toFloat();
          break;

        case 2:

          Alt = buf.toFloat();
          break;
        case 3:

          Spd = buf.toFloat();
          break;
      }
      Serial.print(buf + temp);
      Serial.println();
      buf = "";
      read_state++;
    }
    else
    {
      buf += temp;
    }
  }
  sensors_update = 1;
}


void get_destination()
{
  String buf = "";
  unsigned short int read_state = 0;
  char temp;

  while (SerialBT.available() > 0)
  {
    temp = SerialBT.read();
    if ((temp == '|') || (temp == '\0'))
    {
      switch (read_state)
      {
        case 0:

          Tgt_Lat = buf.toFloat();
          break;

        case 1:

          Tgt_Lng = buf.toFloat();
          break;
      }
      Serial.print(buf + temp);
      Serial.println();
      buf = "";
      read_state++;
    }
    else
    {
      buf += temp;
    }
  }
  sensors_update = 1;
}


float DegRad(float ang)
{
  return ang * PI / 180;
}

///////////////////////////////////////////////////////////BAKA!!!!/////////////////////////////////////////////////////////
//change this __*PI/180 shit to radians or DegRad/////////////////////////////////////////////////////////pidor tupoi

void navigate (float tgt_lat, float tgt_lng, float & tgt_azm, int & dist)
{
  const int globe_rad = 6372795;

  float rad_lat = Lat * PI / 180;
  float rad_lng = Lng * PI / 180;
  float rad_tgt_lat = tgt_lat * PI / 180;
  float rad_tgt_lng = tgt_lng * PI / 180;

  float cos_lat = cos(rad_lat);
  float cos_tgt_lat = cos(rad_tgt_lat);
  float sin_lat = sin(rad_lat);
  float sin_tgt_lat = sin(rad_tgt_lat);

  float delta = rad_tgt_lng - rad_lng;
  float sin_delta = sin(delta);
  float cos_delta = cos(delta);

  float y = sqrt(pow(cos_tgt_lat * sin_delta, 2) + pow(cos_lat * sin_tgt_lat - sin_lat * cos_tgt_lat * cos_delta, 2));
  float x = sin_lat * sin_tgt_lat + cos_lat * cos_tgt_lat * cos_delta;
  dist = globe_rad * atan2(y, x);

  x = (cos_lat * sin_tgt_lat) - (sin_lat * cos_tgt_lat * cos_delta);
  y = sin_delta * cos_tgt_lat;
  float z = degrees(atan(-y / x));

  if (x < 0) z += 180;

  float z2 = radians(fmod((z + 180.0), 360) - 180.0);
  float tgt_ang_rad = z2 - ((2 * PI) * floor((z2 / (2 * PI))));
  tgt_azm = (tgt_ang_rad * 180) / PI;

  // tgt_azm = degrees(z2 - ((2*PI)*floor((z2/(2*PI)))));

}
