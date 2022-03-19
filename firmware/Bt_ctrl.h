# pragma once


void Command_execute()
{
  if (bt_command.equals("DATE_TIME"))
  {
    BT_time();
    Serial.println("Date_time");
  }
  else if (bt_command.equals("NOTIFICATION"))
  {

    Notification_recived();

  }

  else if (bt_command.equals("CALL"))
  {
    Call_detected();
  }

  else if (bt_command.equals("CALL_ANSWERED"))
  {
    screen_update = 1;
    mode = -2;
  }

  else if (bt_command.equals("CALL_ENDED"))
  {
    call_info = "";
    screen_update = 1;
    mode = 0;
  }

  else if (bt_command.equals("MISSED_CALL"))
  {
    Call_missed();
  }

  else if (bt_command.equals("SMS"))
  {
    Recived_SMS();
  }

  else if (bt_command.equals("OPTIONS"))
  {

    Serial.println("options");
  }

  else if (bt_command.equals("TEXT_CAST"))
  {
    while (SerialBT.available() > 0)
    {
      char temp = SerialBT.read();
      cast_text += temp;
    }
    got_text = 1;
    mode = 7;
    screen_update = 1;
  }

  else if (bt_command.equals("CLEAR"))
  {
    cast_text = "";
    Serial.println("text_cleared");
    Serial.println(cast_text);
    got_text = 0;
    mode = 0;
    screen_update = 1;
  }

  else if (bt_command.equals("LOCATION"))
  {
    get_location();
    got_location = 1;
    if(got_destination)navigate(Tgt_Lat,Tgt_Lng,Tgt_Azm, Tgt_Dist);
    Serial.println("Location");
  }

  else if (bt_command.equals("NAVIGATION"))
  {

  }

  else if (bt_command.equals("DESTINATION"))
  {
    get_destination();
    got_destination = 1;
    if(got_location)navigate(Tgt_Lat,Tgt_Lng,Tgt_Azm, Tgt_Dist);
    Serial.println("Destination");
  }

  else if (bt_command.equals("NO_DESTINATION"))
  {
    got_destination = 0;
    Serial.println("Destination");
  }

  else if (bt_command.equals("CALIBRATE"))
  {
    mode = -1;
    //calibrate();
  }

  else if (bt_command.equals("SET_TEXT_COLOR"))
  {
    TEXT_COLOR = tft.color24to16(SerialBT.parseInt());
    screen_update = 1;
  }

  else if (bt_command.equals("SET_HIGHLIGHTED_TEXT_COLOR"))
  {
    ACTIVE_TEXT_COLOR = tft.color24to16(SerialBT.parseInt());
    screen_update = 1;
  }
  else if (bt_command.equals("SET_ICONS_COLOR"))
  {
    ICON_COLOR = tft.color24to16(SerialBT.parseInt());
    screen_update = 1;
  }
  else if (bt_command.equals("SET_HIGHLIGHTED_ICONS_COLOR"))
  {
    ACTIVE_ICON_COLOR = tft.color24to16(SerialBT.parseInt());
    screen_update = 1;
  }
  else if (bt_command.equals("SET_CLOCK_COLOR"))
  {
    CLOCK_COLOR = tft.color24to16(SerialBT.parseInt());
    screen_update = 1;
  }
}


void manage_calls()
{
  if (touchRead(T7) < 25)
  {
    SerialBT.println("ACCEPT");
  }

  if (touchRead(T9) < 25)
  {
    SerialBT.println("DECLINE");
  }
}



void bt_read()
{
  while (SerialBT.available() > 0)
  {
    char mark = SerialBT.read();

    //if ((mark == 0)||(mark == '\0'))
    if (mark == '#')
    {
      Command_execute();
      Serial.println(" executed");
      bt_command = "";
      while (SerialBT.available() > 0) SerialBT.read();
    } else
    {
      bt_command += mark;
    }
  }

}
