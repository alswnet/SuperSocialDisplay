//https://github.com/monstrenyatko/ArduinoMqtt
#ifdef ARDUINO_ARCH_ESP32
#include <WiFi.h>
#include <WiFiMulti.h>

WiFiMulti wifiMulti;
#else
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

ESP8266WiFiMulti wifiMulti;
#endif

#include <MQTT.h>
#include "Key.h"//Necesario cambiar Key_Ejemplo.h a Key.h y poner sus credenciales

char ssid1[] = "ALSW2";//Nombre de la red primaria
char password1[] = "7210-3607";//Contraseña de la red primaria
char ssid2[] = "ALSW";//Nombre de la red secundaria
char password2[] = "2526-4897";//Contraseña de la red secundaria
char ssid3[] = "Garcia Master WIFI";//Nombre de la red secundaria
char password3[] = "garciadiaz9";//Contraseña de la red secundaria
boolean WifiConectado = false;

unsigned long TiempoConsulta = 5000;//Tiempo entre consultas a la API de youtube
unsigned long UltimaConsulta;//Tiempo a la última consulta de la API de Youtube
int CantidadSub  = 0;//Cantidad de suscriptores de tu canal
unsigned long TiempoBajada = 0;//Tiempo para refrescar en caso de que alguien se desuscriba
unsigned long TiempoRefrescar = 0;//Tiempo para actualizar el número

const char* BrokerMQTT = "broker.shiftr.io";
int PuertoMQTT = 80;
const char* ClienteIDMQTT = "SSD";
const char* UsuarioMQTT = "chepecarlos";
const char* ContrasenaMQTT = "secretoespecial";

WiFiClient ESP_Cliente;
MQTTClient client;

long lastMsg = 0;
char msg[50];
int value = 0;

void setup() {

  Serial.begin(115200);//Activar comunicación serial
  InicializarPantallas();//Inicializar comunicación serial para pantallas
  MostarNumero(1234, 4);//Primero=número, Segundo=Cantidad de dígitos

  WiFi.mode(WIFI_STA);//Cambiar modo del Wi-Fi
  delay(100);

  Serial.println("Conectando con Wifi: ");

  wifiMulti.addAP(ssid1, password1);//Agregar a la lista red 1
  wifiMulti.addAP(ssid2, password2);//Agregar a la lista red 2
  wifiMulti.addAP(ssid3, password3);//Agregar a la lista red 3

  ActualizarRed();

  client.begin("broker.shiftr.io", ESP_Cliente);
  client.onMessage(messageReceived);
}

void connect() {
  Serial.print("checking wifi...");

  Serial.print("\nconnecting...");
  while (!client.connect(ClienteIDMQTT, UsuarioMQTT, ContrasenaMQTT)) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("\nconnected!");

  client.subscribe("/ALSW/Redes/Youtube");
  client.subscribe("/ALSW/Redes/Instagram");
  // client.unsubscribe("/hello");
}

void ActualizarRed() {//Función que busca la red
  while (wifiMulti.run() != WL_CONNECTED) {
    Serial.println("Sin Wifi!");
    MostarNumero(1234, 4);
    delay(500);
    MostarNumero(8888, 4);
    delay(500);
    MostarNumero(CantidadSub, 4);
    WifiConectado = false;
  }
  if (wifiMulti.run() == WL_CONNECTED && !WifiConectado ) {
    WifiConectado = true;
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
  }
}

void messageReceived(String &topic, String &payload) {
  Serial.println("Mensaje: " + topic + " - " + payload);
  int CantidadSubNueva = payload.toInt();
  if (CantidadSubNueva == CantidadSub) {//Si la cantidad no cambia refrescará la pantalla cada 5 minutos
    if (TiempoRefrescar == 0) {
      TiempoRefrescar = millis();
    } else if (TiempoRefrescar + 1000 * 60 * 5 < millis()) {
      MostarNumero(CantidadSub, 4);
      TiempoRefrescar = 0;
    }
  } else if (CantidadSubNueva > CantidadSub) {//Si la cantidad nueva es mayor hará una animación
    AnimarSub(CantidadSubNueva);
    CantidadSub = CantidadSubNueva;
    TiempoBajada = 0;
    TiempoRefrescar = 0;
  } else if (CantidadSubNueva < CantidadSub) {//Si la cantidad es menor lo actualizará después de unos minutos
    if (TiempoBajada == 0) {
      Serial.println("Bajando Contador");
      TiempoBajada = millis();
    } else if (TiempoBajada + 1000 * 60 * 2 < millis() ) {
      Serial.println("Bajando Listo");
      TiempoBajada = 0;
      MostarNumero(CantidadSubNueva, 4);
      CantidadSub =  CantidadSubNueva;
    }
  }
}


unsigned long lastMillis = 0;

void loop() {

  ActualizarRed();//mientras busca red muestra "1234-8888"

  if (!client.connected()) {
    connect();
  }

  client.loop();
  delay(10);
  if (CantidadSub == 0) {
    Serial.println("Buscando MQTT");
    MostarNumero(1234, 4);
    delay(500);
    MostarNumero(4321, 4);
    delay(500);

  }
}


void AnimarSub(int CantidadAnimar) {//Animación para nuevos suscriptores, empeiza a contar desde 0
  int tmp = 0;
  int Espera = 1000;
  int Minimo = 1;

  while (tmp < CantidadAnimar) {

    MostarNumero(tmp, 4);
    tmp++;
    delay(Espera);

    if (tmp + 5  > CantidadAnimar) {
      Espera += 100;
    }
    else {
      Espera -= 100;
    }
    if (Espera < Minimo) {
      Espera = Minimo;
    }
  }
  int R = random(10);
  Serial.println(R);
  if (R > 7) {
    MostarNumero(CantidadAnimar + 1, 4);
    delay(Espera);
    MostarNumero(CantidadAnimar + 2, 4);
    delay(Espera);
    MostarNumero(CantidadAnimar + 1, 4);
    delay(Espera);
  }
  MostarNumero(CantidadAnimar, 4);
}
