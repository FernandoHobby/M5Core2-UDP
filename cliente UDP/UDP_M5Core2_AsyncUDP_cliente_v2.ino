// https://github.com/espressif/arduino-esp32/blob/master/libraries/AsyncUDP/src/AsyncUDP.h
// https://github.com/espressif/arduino-esp32/blob/master/libraries/AsyncUDP/src/AsyncUDP.cpp
//--------------
#include <M5Core2.h>
#include <WiFi.h>
#include "AsyncUDP.h"
#include "config.hpp"
RTC_TimeTypeDef TimeStruct;
RTC_DateTypeDef DateStruct;
#include "util.hpp"

struct trama {
  unsigned long orden;
  unsigned long millis;
  char data[20];
  byte rojo;
  byte verde;
  byte azul;
} mensajeClienteTx, mensajeClienteRx;

unsigned long contador = 0;
bool debug = false;
unsigned long tiempo = millis();
unsigned long periodo = 3000;
bool onOff = true;

AsyncUDP udp;

//==============
void setup() {

  M5.begin();
  delay(2000);

  Serial.println("M5Core2 en rol CLIENTE UDP");

  bool errorWiFi = iniciarWiFi(25);
  if (errorWiFi) {
    M5.Lcd.setTextColor(RED, BLACK);
    M5.Lcd.println("error -> sketch detenido");
    while (true) {}
  }

  iniciarRTC();

  if (udp.connect(ipServer, serverPort)) {
    Serial.println("\nINFORMACION CONEXION UDP -rol CLIENTE------------");
    Serial.printf("UDP conectado a server IP = %s  port = %d\n",ipServer.toString(),serverPort);

    udp.onPacket(recibidoPacket);
  }
  M5.Lcd.clear();

}

//==============
void loop() {

  M5.update();

  if (millis() > tiempo + periodo){

    tiempo = millis();

    if (onOff){
        
        // preparo mensajeClienteTx
      mensajeClienteTx.orden = contador++;
      mensajeClienteTx.millis = millis();
      char texto[] = "valores RGB led";
      memcpy(mensajeClienteTx.data, texto, sizeof(texto)); 
      mensajeClienteTx.rojo = random (1,255);
      mensajeClienteTx.verde = random (1,255);
      mensajeClienteTx.azul = random (1,255);

      uint8_t buffer[40];
      memcpy(&buffer, &mensajeClienteTx, sizeof(mensajeClienteTx));

      // Envio periodico por UDP unicast a server 
      // size_t writeTo(const uint8_t *data, size_t len, const IPAddress addr, uint16_t port, tcpip_adapter_if_t tcpip_if=TCPIP_ADAPTER_IF_MAX)
      udp.writeTo(buffer, sizeof(mensajeClienteTx), IPAddress(192,168,1,80), serverPort);
      
      uint16_t colorFore = M5.Lcd.color565(mensajeClienteTx.rojo, mensajeClienteTx.verde, mensajeClienteTx.azul);
      M5.Lcd.setTextColor(colorFore, BLACK);
      M5.Lcd.setCursor(97, 50);
      M5.Lcd.setTextSize(3);
      M5.Lcd.print("Cliente");
      M5.Lcd.setCursor(97, 80);
      M5.Lcd.setTextSize(7);
      M5.Lcd.print("UDP");

    }

    if (onOff) {
      M5.Lcd.setTextColor(GREEN, BLACK);
    } else {
      M5.Lcd.setTextColor(RED, BLACK);
    }
    M5.Lcd.setCursor(25,220);
    M5.Lcd.setTextSize(2);
    M5.Lcd.setTextPadding(300);
    M5.Lcd.print("On/Off   -Per.    +Per.");
  }
  
  if (M5.BtnA.wasPressed()) {
    onOff = (onOff) ? false : true ;
  }

  if (M5.BtnB.wasPressed()) { // -periodo
    if (periodo < 51) {
      periodo = 25;
    } else {
      periodo -= ((periodo <= 1000) ? 50 : 100);
    }
  }

  if (M5.BtnC.wasPressed()) { // +periodo
     if (periodo < 49) {
      periodo = 50;
    } else {
    periodo += ((periodo >= 1000) ? 100 : 50);
    }
  }
 
  M5.Lcd.setTextColor(WHITE, BLACK);
  M5.Lcd.setCursor(40,180);
  M5.Lcd.setTextSize(1);
  M5.Lcd.printf("(periodo entre mensajes UDP = %4u mseg)", periodo);
}

//==============
void recibidoPacket(AsyncUDPPacket packet) {

  if(debug){
    Serial.print("UDP Packet: ");
    Serial.print(packet.isBroadcast() ? "Broadcast" : packet.isMulticast() ? "Multicast" : "Unicast");
    Serial.printf(", de: %s",packet.remoteIP().toString());
    Serial.printf(":%d",packet.remotePort());
    Serial.printf(", a: %s",packet.localIP().toString());
    Serial.printf(":%d , ",packet.localPort());
  }

  Serial.printf("ECO server (%d bytes) ,",packet.length());
  // void * memcpy ( void * destination, const void * source, size_t num );
  memcpy(&mensajeClienteRx, packet.data(),packet.length());
  Serial.printf(" orden = %6lu ",mensajeClienteRx.orden);
  Serial.printf(" millis = %8lu ",mensajeClienteRx.millis);
  Serial.printf(" rojo= %3u ",mensajeClienteRx.rojo);
  Serial.printf(" verde= %3u ",mensajeClienteRx.verde);
  Serial.printf(" azul= %3u\n",mensajeClienteRx.azul);
  
}

//---------------