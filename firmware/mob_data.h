RTC_DATA_ATTR short unsigned int call_counter = 0;

RTC_DATA_ATTR short unsigned int msg_counter = 0;

RTC_DATA_ATTR short unsigned int notif_counter = 0;

RTC_DATA_ATTR short unsigned int call_pointer = 0;

RTC_DATA_ATTR short unsigned int msg_pointer = 0;

RTC_DATA_ATTR short unsigned int notif_pointer = 0;




struct Phone_call
{
  //String abonent_name;
  String abonent_num;
//  char call_date[12];
//  char call_time[6];
   String timestamp;
};


struct Text_msg
{
  //String abonent_name;
  String abonent_num;
  String msg_text;
  char short_msg_text[40];
//  char msg_date[12];
//  char msg_time[6];
  String timestamp;
};

struct Notification
{
  String notif_source;
  String notif_text;
  char short_notif_text[40];
//  char notif_date[12];
//  char notif_time[6];
  String timestamp;
};



RTC_DATA_ATTR Phone_call missed_calls[20];
RTC_DATA_ATTR Text_msg messages[20];
RTC_DATA_ATTR Notification notifications[20];




void Recived_SMS()
{
  unsigned short int read_state = 0;
  unsigned short int read_char = 0;
  while (SerialBT.available() > 0)
  {
    char temp = SerialBT.read();
    if (temp == '|')
    {
      read_state++;
      continue;
    }

    switch (read_state)
    {
      case 0:
        messages[msg_pointer].msg_text += temp;
        if (read_char < 40)
        {
          messages[msg_pointer].short_msg_text[read_char] = temp;
          read_char++;
        }
        break;

      case 1:
        messages[msg_pointer].abonent_num += temp;
        break;
    }

  }

  //strftime(messages[msg_pointer].msg_date, 12, "%d.%m.%Y", &timeinfo);
  //strftime(messages[msg_pointer].msg_time, 6, "%R", &timeinfo);
  messages[msg_pointer].timestamp=rtc.getTimeDate();

  if (msg_pointer++ >= 19)msg_pointer = 0;
  if (msg_counter < 19)msg_counter ++;
}



void Notification_recived()
{
  unsigned short int read_char;
  while (SerialBT.available() > 0)
  {
    char temp = SerialBT.read();
    notifications[notif_pointer].notif_text += temp;
    if (read_char < 40)
    {
      notifications[notif_pointer].short_notif_text[read_char] = temp;
      read_char++;
    }
  }

  //strftime(notifications[notif_pointer].notif_date, 12, "%d.%m.%Y", &timeinfo);
  //strftime(notifications[notif_pointer].notif_time, 6, "%R", &timeinfo);
   notifications[notif_pointer].timestamp=rtc.getTimeDate();

  if (notif_pointer++ >= 19)call_pointer = 0;
  if (notif_counter < 19)notif_counter++;
  Serial.println("Notification recived");
}


void Call_missed()
{
  while (SerialBT.available() > 0)
  {
    char temp = SerialBT.read();
    missed_calls[call_pointer].abonent_num += temp;
  }

  //strftime(missed_calls[call_pointer].call_date, 12, "%d.%m.%Y", &timeinfo);
  //strftime(missed_calls[call_pointer].call_time, 6, "%R", &timeinfo);
   missed_calls[call_pointer].timestamp=rtc.getTimeDate();
   
  if (call_pointer++ >= 19)call_pointer = 0;
  if (call_counter < 19)call_counter ++;
 
  screen_update = 1;
  mode = 0;
}

void Call_detected()
{
  while (SerialBT.available() > 0)
  {
    char temp = SerialBT.read();
    if (temp == '|')break;
    call_info += temp;
  }
  call_status = SerialBT.read();
  mode = -3;
  screen_update = 1;

}
