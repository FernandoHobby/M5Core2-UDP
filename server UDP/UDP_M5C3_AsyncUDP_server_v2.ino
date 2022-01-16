// https://github.com/espressif/arduino-esp32/blob/master/libraries/AsyncUDP/src/AsyncUDP.h
// https://github.com/espressif/arduino-esp32/blob/master/libraries/AsyncUDP/src/AsyncUDP.cpp
//--------------

#include <WiFi.h>
#include "AsyncUDP.h"
#include "config.hpp"
#include "util.hpp"
#include <Adafruit_NeoPixel.h>

#define PIXEL_PIN 2
#define NUMPIXELS 1
#define BTN_PIN 3

struct trama {
  unsigned long orden;
  unsigned long millis;
  char data[20];
  byte rojo;
  byte verde;
  byte azul;
} mensajeServerRx,mensajeServerTx;

bool debug = false;

AsyncUDP udp;

Adafruit_NeoPixel pixel(NUMPIXELS, PIXEL_PIN, NEO_GRBW + NEO_KHZ400);

//==============
void setup() {

  pixel.begin(); // Inicializa el objeto NeoPixel
  pixel.clear(); 
  pixel.show();

  Serial.begin(115200);
  delay(2000);

  Serial.println("M5StampC3 en rol SERVER UDP");

  bool errorWiFi = iniciarWiFi(25);
  if (errorWiFi ) {
    Serial.println("error -> sketch detenido");
    while (true) {}
  }
  
  if (udp.listen(serverPort)) {
    Serial.println("\nINFORMACION CONEXION UDP -rol SERVER------------");
    Serial.printf("UDP escuchando en IP = %s  port = %u\n",WiFi.localIP().toString(), serverPort);

    udp.onPacket(recibidoPacket);
  }
}

//==============
void loop() {

  //udp.broadcast("udp.broadcast");
  //udp.writeTo(texto, sizeof(texto), IPAddress(192,168,1,81), serverPort);

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

  Serial.printf("Cliente envia (%d bytes) ,",packet.length());

  // void * memcpy ( void * destination, const void * source, size_t num );
  memcpy(&mensajeServerRx, packet.data(),packet.length());

  Serial.printf(" orden = %6lu ",mensajeServerRx.orden);
  Serial.printf(" millis = %8lu ",mensajeServerRx.millis);
  Serial.printf(" rojo= %3u ",mensajeServerRx.rojo);
  Serial.printf(" verde= %3u ",mensajeServerRx.verde);
  Serial.printf(" azul= %3u \n",mensajeServerRx.azul);

  pixel.setPixelColor(0, pixel.Color(mensajeServerRx.rojo,mensajeServerRx.verde,mensajeServerRx.azul));
  pixel.show();

  mensajeServerTx.orden = mensajeServerRx.orden;
  mensajeServerTx.millis = mensajeServerRx.millis;
  char texto[] = "ECO valores RGB led";
  memcpy(mensajeServerTx.data, texto, sizeof(texto)); 
  mensajeServerTx.rojo = mensajeServerRx.rojo;
  mensajeServerTx.verde = mensajeServerRx.verde;
  mensajeServerTx.azul = mensajeServerRx.azul;

  uint8_t buffer[40];
  // void * memcpy ( void * destination, const void * source, size_t num );
  memcpy(&buffer, &mensajeServerTx, sizeof(mensajeServerTx));

  //respuesta a cliente
  //size_t write(const uint8_t *data, size_t len);
  packet.write(buffer,sizeof(mensajeServerTx));
  
}

//--------------