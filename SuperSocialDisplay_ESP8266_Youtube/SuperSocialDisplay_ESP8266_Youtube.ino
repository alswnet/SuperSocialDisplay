#include <YoutubeApi.h>//Librería para comunicarse con la API de Youtube, es necesario instalarla
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>//Librería para hacer consulta Json, es necesario instalarla
#include "Key.h"//Necesario cambiar Key_Ejemplo.h a Key.h y poner sus credenciales

ESP8266WiFiMulti wifiMulti;

char ssid1[] = "ALSW2";//Nombre de la red primaria
char password1[] = "7210-3607";//Contraseña de la red primaria
char ssid2[] = "ALSW";//Nombre de la red secundaria
char password2[] = "2526-4897";//Contraseña de la red secundaria

WiFiClientSecure client;
YoutubeApi api(API_KEY, client);

unsigned long TiempoConsulta = 5000;//Tiempo entre consultas a la API de youtube
unsigned long UltimaConsulta;//Tiempo a la última consulta de la API de Youtube
int CantidadSub  = 0;//Cantidad de suscriptores de tu canal
unsigned long TiempoBajada = 0;//Tiempo para refrescar en caso de que alguien se desuscriba
unsigned long TiempoRefrescar = 0;//Tiempo para actualizar el número

void setup() {

  Serial.begin(115200);//Activar comunicación serial
  InicializarPantallas();//Inicializar comunicación serial para pantallas
  MostarNumero(1234, 4);//Primero=número, Segundo=Cantidad de dígitos

  WiFi.mode(WIFI_STA);//Cambiar modo del Wi-Fi
  delay(100);

  Serial.println("Conectando con Wifi: ");

  wifiMulti.addAP(ssid1, password1);//Agregar a la lista red 1
  wifiMulti.addAP(ssid2, password2);//Agregar a la lista red 2

  ActualizarRed();

}

void loop() {

  ActualizarRed();//mientras busca red muestra "1234-8888"

  if (millis() - UltimaConsulta > TiempoConsulta)  {//Verifica si es tiempo de hacer la siguiente consulta
    if (api.getChannelStatistics(CHANNEL_ID))    {//hace la consulta a la API de Youtube
      int CantidadSubNueva = api.channelStats.subscriberCount;//Obtiene la cantidad de suscriptores actuales

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
      Serial.println("---------Data---------");
      Serial.print("Cantidad Subcriptores: ");
      Serial.println(api.channelStats.subscriberCount);
      Serial.print("Cantidad Vistas: ");
      Serial.println(api.channelStats.viewCount);
      //Serial.print("Comment Count: ");
      //Serial.println(api.channelStats.commentCount);
      Serial.print("Cantidad Video: ");
      Serial.println(api.channelStats.videoCount);
      // Probably not needed :)
      //Serial.print("hiddenSubscriberCount: ");
      //Serial.println(api.channelStats.hiddenSubscriberCount);
      Serial.println("------------------------");

    }
    UltimaConsulta = millis();//Guarda cuando fue la última consulta
  }
}

void ActualizarRed() {//Función que busca la red 
  while (wifiMulti.run() != WL_CONNECTED) {
    Serial.println("Sin Wifi!");
    MostarNumero(1234, 4);
    delay(500);
    MostarNumero(8888, 4);
    delay(500);
    MostarNumero(CantidadSub, 4);
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
