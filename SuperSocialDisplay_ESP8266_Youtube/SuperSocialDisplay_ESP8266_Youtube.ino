#include <YoutubeApi.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WiFiClientSecure.h>

#include <ArduinoJson.h> // This Sketch doesn't technically need this, but the library does so it must be installed.

ESP8266WiFiMulti wifiMulti;

char ssid1[] = "ALSW2";       // your network SSID (name)
char password1[] = "7210-3607";  // your network key
char ssid2[] = "Garcia Wifi";
char password2[] = "cirugia93";
char ssid3[] = "ALSW";
char password3[] = "2526-4897";
#define API_KEY "AIzaSyChaTNrG24ikJ2D8hAtL2LUTPgk8U-MSyg"  // your google apps API Token
#define CHANNEL_ID "UCS5yb75qx5GFOG-uV5JLYlQ" // makes up the url of channel


WiFiClientSecure client;
YoutubeApi api(API_KEY, client);

unsigned long api_mtbs = 5000; //mean time between api requests
unsigned long api_lasttime;   //last time api request has been done
int CantidadSub  = 0;
unsigned long TiempoBajada = 0;
long subs = 0;

void setup() {

  Serial.begin(115200);
  InicializarPantallas();
  MostarNumero(1234, 4);

  WiFi.mode(WIFI_STA);
  //WiFi.disconnect();
  delay(100);

  Serial.println("Conectando con Wifi: ");

  wifiMulti.addAP(ssid1, password1);
  wifiMulti.addAP(ssid2, password2);
  wifiMulti.addAP(ssid3, password3);


  if (wifiMulti.run() == WL_CONNECTED) {
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
  }

}

void loop() {

  while (wifiMulti.run() != WL_CONNECTED) {
    Serial.println("Sin Wifi!");
    MostarNumero(1234, 4);
    delay(500);
    MostarNumero(8888, 4);
    delay(500);
    MostarNumero(CantidadSub, 4);
  }

  if (millis() - api_lasttime > api_mtbs)  {
    if (api.getChannelStatistics(CHANNEL_ID))    {
      int CantidadSubNueva = api.channelStats.subscriberCount;

      if (CantidadSubNueva > CantidadSub) {
        AnimarSub(CantidadSubNueva);
        CantidadSub = CantidadSubNueva;
        TiempoBajada = 0;
      } else if (CantidadSubNueva < CantidadSub) {
        if (TiempoBajada == 0) {
          Serial.println("Bajando Contador");
          TiempoBajada = millis();
        } else if (TiempoBajada + 1000 * 90 < millis() ) {
          Serial.println("Bajando Listo");
          TiempoBajada = 0;
          MostarNumero(CantidadSubNueva, 4);
          CantidadSub =  CantidadSubNueva;
        }
      }
      Serial.println("---------Stats---------");
      Serial.print("Subscriber Count: ");
      Serial.println(api.channelStats.subscriberCount);
      Serial.print("View Count: ");
      Serial.println(api.channelStats.viewCount);
      //Serial.print("Comment Count: ");
      //Serial.println(api.channelStats.commentCount);
      Serial.print("Video Count: ");
      Serial.println(api.channelStats.videoCount);
      // Probably not needed :)
      //Serial.print("hiddenSubscriberCount: ");
      //Serial.println(api.channelStats.hiddenSubscriberCount);
      Serial.println("------------------------");

    }
    api_lasttime = millis();
  }
}

void AnimarSub(int CantidadAnimar) {
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
