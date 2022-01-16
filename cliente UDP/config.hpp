//----------------

const char *ssid = "WLAN_FB2";
const char *password = "************";
const char *hostname = "M5Core2";

//----------------

IPAddress ip(192, 168, 1, 81);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress dns1(80, 58, 61, 250);
IPAddress dns2(80, 58, 61, 254);

//----------------

const char* ntpServer = "europe.pool.ntp.org";
const long  gmtOffset_sec = 3600;       // +1 hora en España
const int   daylightOffset_sec = 3600;  // 1 hora cbio horario de verano

const char* TZ_INFO = "CET-1CEST,M3.3.0,M10.5.0/3";

//----------------

IPAddress ipServer(192, 168, 1, 80);

//----------------
uint16_t serverPort = 8888;
unsigned int clientePort = 8889;

//----------------