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
    Serial.println("error WiFi");
  } else {
    Serial.print("Ok WiFi");
    Serial.print("   IP = ");
    Serial.println(WiFi.localIP());
    
    Serial.println("INFORMACION CONEXION WIFI --------------------");
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
