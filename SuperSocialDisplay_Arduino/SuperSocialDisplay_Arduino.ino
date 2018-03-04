#include <ESP8266WiFi.h> //Libreria de ESP8266
#include <WiFiClientSecure.h> //Libreria de Consultas Escriptadas
#include <YoutubeApi.h> //Libreria de Youtube
#include "InstagramStats.h"//Libreria de Instagram
#include <FacebookApi.h>//Libreria de Facebook
#include <ArduinoJson.h>//Libreria de Decifrado Json
#include "JsonStreamingParser.h"///Libreria de Decifrado Json
#include "Contrasenna.h"//Archivo con info de contrasenas para hacer las consultas

//Configuraciones de RED
//char ssid[] = "TURBONETT_ALSW"; //Nombre de Red
//char password[] = "2526-4897";  //Contrasenna de Red
//char ssid[] = "ALSW2"; //Nombre de Red
//char password[] = "7210-3607";  //Contrasenna de Red
char ssid[] = "G_WIFI"; //Nombre de Red
char password[] = "Medicina09";  //Contrasenna de Red

//ID de Redes Sociales
#define userNameInstagram "alswnet"//usuario de Insgramam
#define fanpageID "163069780414846"//ID de fanpage de Facebook
#define CHANNEL_ID "UCS5yb75qx5GFOG-uV5JLYlQ" // ID de Canal de Youtube

WiFiClientSecure client;

InstagramStats instaStats(client);
YoutubeApi api(API_KEY, client);
FacebookApi *apifb;

unsigned long EsperaEstreConsulta = 60000;//cada 20 Segundos
unsigned long EsperaCambioDisplay = 10000;//cada1 Segundo
unsigned long SiquientePreguntaAPI = 0;
unsigned long SiquienteCambioDisplay = 0;
unsigned long TiempoActual = 0;
unsigned long ValocidadBarrido = 300;

byte segmentClock = 13;
byte segmentLatch = 12;
byte segmentData = 16;

#define Facebook 0
#define Youtube 1
#define Instagram 2

const int LedIndicador = 5;
const int PinLed[3] = {15, 4, 0};
const int Buzzer = 14;
const int CantidadDisplay = 4;
int Mostar = 0;
int Sub[3] = {0, 0, 0};

void setup() {

  Serial.begin(115200);
  pinMode(LedIndicador, OUTPUT);
  pinMode(Buzzer, OUTPUT);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  Serial.println("Iniciando Programa de SuperSocialDisplay 0.1");
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    digitalWrite(LedIndicador, 0);
    delay(500);
    digitalWrite(LedIndicador, 1);
    delay(250);
  }
  Melodia(3, false);//Tono de Activado
  MostarNumero( 0, CantidadDisplay);
  Serial.println("WiFi Conectada");
  Serial.println("Direcion IP: ");
  IPAddress ip = WiFi.localIP();
  Serial.println(ip);

  apifb = new FacebookApi(client, FACEBOOK_ACCESS_TOKEN, FACEBOOK_APP_ID, FACEBOOK_APP_SECRET);

  pinMode(segmentClock, OUTPUT);
  pinMode(segmentData, OUTPUT);
  pinMode(segmentLatch, OUTPUT);

  digitalWrite(segmentClock, LOW);
  digitalWrite(segmentData, LOW);
  digitalWrite(segmentLatch, LOW);

  for (int i = 0; i < 3 ; i++) {
    pinMode(PinLed[i], OUTPUT);
    digitalWrite(PinLed[i], 0);
  }

  //getYoutube();
  getInstagram();
  getFacebook();

  Melodia(4, false);//Tono de Empezar
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
    digitalWrite(PinLed[Mostar], 0);//Apagar el Led Anterior
    Mostar = ++Mostar % 3;//Cambia al siquiente Red Social
    digitalWrite(PinLed[Mostar], 1);//Encender el Led Actual
    MostarBarrido(Sub[Mostar], CantidadDisplay);//Muestra el numero de Segidores
    SiquienteCambioDisplay = TiempoActual + EsperaCambioDisplay;
  }
}

void getSegidores() {
  boolean NuevoSegidor = false;
  if (TiempoActual > SiquientePreguntaAPI)  {
    if (NuevoSegidor || getFacebook()) {

      Melodia(Facebook, true);
    } else if (NuevoSegidor || getInstagram()) {

      Melodia(Instagram, true);
    } else if (NuevoSegidor || getYoutube()) {

      Melodia(Youtube, true);
    }
    SiquientePreguntaAPI = TiempoActual + EsperaEstreConsulta;
  }
}

//Consulta para buscar cuando segirores en Instagram
boolean getInstagram() {
  InstagramUserStats response = instaStats.getUserStats(userNameInstagram);
  if (Sub[Instagram] < response.followedByCount) {
    Sub[Instagram] = response.followedByCount;
    Serial.print("Instagram: ");
    Serial.println(Sub[Instagram]);
    return true;
  }
  return false;
}

//Consulta para buscar cuantos subcriptores en Youtube
boolean getYoutube() {
  if (api.getChannelStatistics(CHANNEL_ID)) {
    if (Sub[Youtube] < api.channelStats.subscriberCount) {
      Sub[Youtube] = api.channelStats.subscriberCount;
      Serial.print("Youtube: ");
      Serial.println(Sub[Youtube]);
      return true;
    }
  }
  return false;
}

//Consulta para buscar los seguidores una Fanpage de Facebook
boolean getFacebook() {
  int pageLikes = apifb->getPageFanCount(fanpageID);
  if (Sub[Facebook] < pageLikes) {
    Sub[Facebook] = pageLikes;
    Serial.print("Facebook: ");
    Serial.println(Sub[Facebook]);
    return true;
  }
  return false;
}

//Nota    Frecuencia   Preriodo   TiempoEnAlto
//c       261 Hz       3830       1915
//d       294 Hz       3400       1700
//e       329 Hz       3038       1519
//f       349 Hz       2864       1432
//g       392 Hz       2550       1275
//a       440 Hz       2272       1136
//b       493 Hz       2028       1014
//C       523 Hz       1912        956

int Longitud[] = {15, 15, 15, 6, 6 };
char notes[5][16] = {
  {"ccggaagffeeddc "},//Melodia 0 >> Facebook
  {"aabbaabbbbcc CC"},//Melodia 1 >> Youtube
  {"ccaacbcbaaged C"},//Melodia 2 >> Instagram
  {"ababab"},
  {"aCaCac"}
};
int beats[5][16] = {
  { 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 2, 4 },
  { 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 2, 4 },
  { 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 2, 4 },
  { 1, 2, 3, 3, 2, 1},
  { 2, 1, 1, 2, 1, 1}
};

int tempo = 300;
float TiempoPasado = 0;

void Melodia(int Melodia, boolean Random) {
  for (int i = 0; i < Longitud[Melodia]; i++) {
    if (Random) MostarRandom(CantidadDisplay);
    if (notes[Melodia][i] == ' ') {
      delay(beats[Melodia][i] * tempo);
    } else {
      playNote(notes[Melodia][i], beats[Melodia][i] * tempo);
    }
  }
}

void playNote(char note, int duration) {
  char names[] = { 'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C' };
  int tones[] = { 1915, 1700, 1519, 1432, 1275, 1136, 1014, 956 };
  for (int i = 0; i < 8; i++) {
    if (names[i] == note) {
      playTone(tones[i], duration);
      delay(50);
    }
  }
}

void playTone(int tone, int duration) {
  for (long i = 0; i < duration * 1000L; i += tone * 2) {
    digitalWrite(Buzzer, HIGH);
    delayMicroseconds(tone);
    digitalWrite(Buzzer, LOW);
    delayMicroseconds(tone);
  }
}

void MostarRandom(int Digitos) {
  int Numero = random(pow(10, Digitos));
  MostarNumero(Numero, Digitos);
}

void MostarBarrido(float Valor, int Digitos) {
  MostarNumero( 0, Digitos);
  int Divisor = pow(10, Digitos);
  for (int i = 0; i <= Digitos; i++) {
    delay(ValocidadBarrido);
    MostarNumero(Valor / Divisor, Digitos);
    Divisor /= 10;
  }
}

void MostarNumero(float Valor, int Digitos) {
  int number = abs(Valor);
  for (byte x = 0 ; x < Digitos ; x++)  {
    int remainder = number % 10;
    if (number == 0) {
      postNumber(' ', false);
    }
    else {
      postNumber(remainder, false);
    }
    number /= 10;
  }

  digitalWrite(segmentLatch, LOW);
  digitalWrite(segmentLatch, HIGH);
}

void postNumber(byte number, boolean decimal) {

  //     --     A
  //   /    /   F/B
  //     --     G
  //   /    /   E/C
  //     --     D/DP

#define a  1<<0
#define b  1<<6
#define c  1<<5
#define d  1<<4
#define e  1<<3
#define f  1<<1
#define g  1<<2
#define dp 1<<7

  byte segments;
  switch (number)  {
    case 1: segments = b | c; break;
    case 2: segments = a | b | d | e | g; break;
    case 3: segments = a | b | c | d | g; break;
    case 4: segments = f | g | b | c; break;
    case 5: segments = a | f | g | c | d; break;
    case 6: segments = a | f | g | e | c | d; break;
    case 7: segments = a | b | c; break;
    case 8: segments = a | b | c | d | e | f | g; break;
    case 9: segments = a | b | c | d | f | g; break;
    case 0: segments = a | b | c | d | e | f; break;
    case ' ': segments = 0; break;
    case 'c': segments = g | e | d; break;
    case '-': segments = g; break;
  }
  if (decimal) segments |= dp;
  for (byte x = 0 ; x < 8 ; x++) {
    digitalWrite(segmentClock, LOW);
    digitalWrite(segmentData, segments & 1 << (7 - x));
    digitalWrite(segmentClock, HIGH);
  }
}
