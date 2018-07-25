//ID de Redes Sociales
//#define InstagramID "alswnet"//usuario de Insgramam
//#define FacebookID "163069780414846"//ID de fanpage de Facebook
#define YoutubeID "UCS5yb75qx5GFOG-uV5JLYlQ" // ID de Canal de Youtube

#include <ESP8266WiFi.h> //Libreria de ESP8266
#include <WiFiClientSecure.h> //Libreria de Consultas Escriptadas
#include "Contrasenna.h"//Archivo con info de contrasenas para hacer las consultas

WiFiClientSecure client;

#ifdef YoutubeID
#include <YoutubeApi.h> //Libreria de Youtube
YoutubeApi api(API_KEY, client);
#endif

#ifdef InstagramID
#include "InstagramStats.h"//Libreria de Instagram
InstagramStats instaStats(client);
#endif

#ifdef FacebookID
#include <FacebookApi.h>//Libreria de Facebook
FacebookApi *apifb;
#endif

#include <ArduinoJson.h>//Libreria de Decifrado Json
#include "JsonStreamingParser.h"///Libreria de Decifrado Json
#include <Adafruit_NeoPixel.h>
Adafruit_NeoPixel strip = Adafruit_NeoPixel(13, 4, NEO_GRB + NEO_KHZ800);

//Configuraciones de RED
//char ssid[] = "TURBONETT_ALSW"; //Nombre de Red
//char password[] = "2526-4897";  //Contrasenna de Red
char ssid[] = "ALSW2"; //Nombre de Red
char password[] = "7210-3607";  //Contrasenna de Red
//char ssid[] = "TURBONETT_22C4DF"; //Nombre de Red
//char password[] = "zQLfWT66";  //Contrasenna de Red
//char ssid[] = "Garcia WIFI"; //Nombre de Red
//char password[] = "cirugia93";  //Contrasenna de Red


unsigned long EsperaEstreConsulta = 60000;//cada 20 Segundos
unsigned long EsperaCambioDisplay = 10000;//cada1 Segundo
unsigned long SiquientePreguntaAPI = 0;
unsigned long SiquienteCambioDisplay = 0;
unsigned long TiempoActual = 0;
unsigned long ValocidadBarrido = 300;

byte segmentClock = 13;//
byte segmentLatch = 12;//
byte segmentData = 16;

#define Facebook 0
#define Youtube 1
#define Instagram 2

const int LedIndicador = 14;
const int PinLed[3] = {15, 0, 0};
const int Buzzer = 5;
const int CantidadDisplay = 4;
int Mostar = 1;
int Sub[3] = {0, 0, 0};

void setup() {

  Serial.begin(115200);
  pinMode(LedIndicador, OUTPUT);
  pinMode(Buzzer, OUTPUT);

  pinMode(segmentClock, OUTPUT);
  pinMode(segmentData, OUTPUT);
  pinMode(segmentLatch, OUTPUT);

  digitalWrite(segmentClock, LOW);
  digitalWrite(segmentData, LOW);
  digitalWrite(segmentLatch, LOW);

  while (true) {
  //  Melodia(4, false);//Tono de Activado
  }

  MostarNumero(9999, CantidadDisplay);
  strip.begin();
  strip.show();

  colorWipe(strip.Color(255, 0, 0), 50); // Red
  strip.show();

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  Serial.println("Iniciando Programa de SuperSocialDisplay 0.1");
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    digitalWrite(LedIndicador, 0);
    delay(500);
    digitalWrite(LedIndicador, 1);
    delay(250);
  }
  colorWipe(strip.Color(0, 255, 0), 50); // Red
  strip.show();

  MostarNumero( 0, CantidadDisplay);
  Serial.println("WiFi Conectada");
  Serial.println("Direcion IP: ");
  IPAddress ip = WiFi.localIP();
  Serial.println(ip);

  //apifb = new FacebookApi(client, FACEBOOK_ACCESS_TOKEN, FACEBOOK_APP_ID, FACEBOOK_APP_SECRET);


  for (int i = 0; i < 3 ; i++) {
    pinMode(PinLed[i], OUTPUT);
    digitalWrite(PinLed[i], 0);
  }

#ifdef InstagramID
  getInstagram();
#endif
  delay(10);
#ifdef YoutubeID
  getYoutube();
#endif
  delay(10);
#ifdef FacebookID
  getFacebook();
#endif
  delay(10);
  colorWipe(strip.Color(0, 0, 0), 50); // Red
  strip.show();
  //Melodia(4, false);//Tono de Empezar
  Serial.println("---Datos----");
}

void loop() {
  TiempoActual = millis();

  CambiarDisplay();

  getSegidores();

  digitalWrite(LedIndicador, 0);
  delay(100);
  digitalWrite(LedIndicador, 1);
  delay(100);
}

void CambiarDisplay() {
  if (TiempoActual > SiquienteCambioDisplay) {
    //digitalWrite(PinLed[Mostar], 0);//Apagar el Led Anterior
    //Mostar = ++Mostar % 3;//Cambia al siquiente Red Social
    //digitalWrite(PinLed[Mostar], 1);//Encender el Led Actual
    MostarNumero(Sub[Mostar], CantidadDisplay);//Muestra el numero de Segidores
    SiquienteCambioDisplay = TiempoActual + EsperaCambioDisplay;
  }
}

// Fill the dots one after the other with a color
void colorWipe(int c, int esperar) {
  for (int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(esperar);
  }
}

void getSegidores() {
  boolean NuevoSegidor = false;
  if (TiempoActual > SiquientePreguntaAPI)  {
#ifdef FacebookID
    if (NuevoSegidor || getFacebook()) {
     // Melodia(Facebook, true);
    }
#endif
#ifdef InstagramID
    if (NuevoSegidor || getInstagram()) {
     // Melodia(Instagram, true);
    }
#endif
#ifdef YoutubeID
    if (NuevoSegidor || getYoutube()) {
      rainbow(20);
    //  Melodia(Youtube, true);
      colorWipe(strip.Color(0, 0, 0), 50); // Red
      strip.show();
    }
#endif
    SiquientePreguntaAPI = TiempoActual + EsperaEstreConsulta;
  }
}

//Consulta para buscar cuando segirores en Instagram
#ifdef InstagramID
boolean getInstagram() {
  InstagramUserStats response = instaStats.getUserStats(InstagramID);
  if (Sub[Instagram] < response.followedByCount) {
    Sub[Instagram] = response.followedByCount;
    Serial.print("Instagram: ");
    Serial.println(Sub[Instagram]);
    return true;
  }
  return false;
}
#endif

//Consulta para buscar cuantos subcriptores en Youtube
#ifdef YoutubeID
boolean getYoutube() {
  if (api.getChannelStatistics(YoutubeID)) {
    if (Sub[Youtube] < api.channelStats.subscriberCount) {
      Sub[Youtube] = api.channelStats.subscriberCount;
      Serial.print("Youtube: ");
      Serial.println(Sub[Youtube]);
      return true;
    }
  }
  return false;
}
#endif

//Consulta para buscar los seguidores una Fanpage de Facebook
#ifdef FacebookID
boolean getFacebook() {
  int pageLikes = apifb->getPageFanCount(FacebookID);
  if (Sub[Facebook] < pageLikes) {
    Sub[Facebook] = pageLikes;
    Serial.print("Facebook: ");
    Serial.println(Sub[Facebook]);
    return true;
  }
  return false;
}
#endif

