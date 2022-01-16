//--------------
bool iniciarWiFi(int intentos) {

  bool error = false;
  
  WiFi.mode(WIFI_STA);
  WiFi.setHostname(hostname);
  WiFi.config(ip, gateway, subnet, dns1, dns2);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    intentos--;
    if (intentos == 0) break;
    delay(500);
  }

  if (intentos == 0) {
    M5.Lcd.setTextColor(RED, BLACK);
    M5.Lcd.println("error WiFi");
  } else {
    M5.Lcd.setTextColor(GREEN, BLACK);
    M5.Lcd.print("Ok WiFi");
    M5.Lcd.print("   IP = ");
    M5.Lcd.println(WiFi.localIP());
    
    Serial.println("\nINFORMACION CONEXION WIFI --------------------");
    Serial.printf("WiFi.SSID()        = %s\n",WiFi.SSID());
    Serial.print("WiFi.psk()         = ");Serial.println(WiFi.psk());
    Serial.printf("WiFi.localIP()     = %s\n",WiFi.localIP().toString());
    Serial.printf("WiFi.gatewayIP()   = %s\n",WiFi.gatewayIP().toString());
    Serial.printf("WiFi.subnetMask()  = %s\n",WiFi.subnetMask().toString());
    Serial.printf("WiFi.dnsIP(0)      = %s\n",WiFi.dnsIP(0).toString());
    Serial.printf("WiFi.dnsIP(1)      = %s\n",WiFi.dnsIP(1).toString());
    Serial.printf("WiFi.BSSID()       = %u\n",WiFi.BSSID());
    Serial.printf("WiFi.RSSI()        = %d\n",WiFi.RSSI());
    Serial.print("WiFi.macAddress()  = "); Serial.println(WiFi.macAddress());
    error = false;
  }

  return error;
}

//--------------
bool iniciarRTC() {

  struct tm timeinfo;
  bool error = false;

  if (!WiFi.isConnected()) {
    Serial.println("!WiFi.isConnected() = true");
    error = true;
  } else {
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

    if (!getLocalTime(&timeinfo))  {
      error = true;
    } else {
      DateStruct.WeekDay  = timeinfo.tm_wday;         // de 0 a 6, 0 = domingo
      DateStruct.Month    = timeinfo.tm_mon + 1;      // de 0 a 11
      DateStruct.Date     = timeinfo.tm_mday;         // de 1 a 31
      DateStruct.Year     = timeinfo.tm_year + 1900;  // de 0 a XXX, años desde 1900
      M5.Rtc.SetDate(&DateStruct);

      TimeStruct.Hours    = timeinfo.tm_hour;         // de 0 a 23
      TimeStruct.Minutes  = timeinfo.tm_min;          // de 0 a 59
      TimeStruct.Seconds  = timeinfo.tm_sec;          // de 0 a 59
      M5.Rtc.SetTime(&TimeStruct);
    }
  }

  if (error) {
    M5.Lcd.setTextColor(RED, BLACK);
    M5.Lcd.println("error NTP->RTC");
  } else {
    M5.Lcd.setTextColor(GREEN, BLACK);
    M5.Lcd.println("Ok NTP->RTC");
  }

  return error;
}

//--------------
String getFecha() {
  M5.Rtc.GetDate(&DateStruct);
  return String(DateStruct.Date) + "/" + String(DateStruct.Month) + "/" + String(DateStruct.Year);
}

//--------------
String getHora() {
  M5.Rtc.GetTime(&TimeStruct);
  return String(TimeStruct.Hours) + ":" + String(TimeStruct.Minutes) + ":" + String(TimeStruct.Seconds);
}
//--------------
time_t getEpoch() {

  M5.Rtc.GetTime(&TimeStruct);
  M5.Rtc.GetDate(&DateStruct);

  struct tm t = {0};  // Initalize to all 0's
  t.tm_year   = DateStruct.Year - 1900;  // This is year-1900, so 112 = 2012
  t.tm_mon    = DateStruct.Month - 1;
  t.tm_mday   = DateStruct.Date;
  t.tm_hour   = TimeStruct.Hours;
  t.tm_min    = TimeStruct.Minutes;
  t.tm_sec    = TimeStruct.Seconds;
  time_t timeSinceEpoch = mktime(&t);
  return timeSinceEpoch;
}

//---------------