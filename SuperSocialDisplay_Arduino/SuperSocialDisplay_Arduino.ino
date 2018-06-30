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
    Melodia(5, false);//Tono de Activado
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
  Melodia(3, false);//Tono de Activado
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
      Melodia(Facebook, true);
    }
#endif
#ifdef InstagramID
    if (NuevoSegidor || getInstagram()) {
      Melodia(Instagram, true);
    }
#endif
#ifdef YoutubeID
    if (NuevoSegidor || getYoutube()) {
      rainbow(20);
      Melodia(Youtube, true);
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

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for (j = 0; j < 256; j++) {
    for (i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
// Tonos y Fecuencias sacados de https://es.m.wikipedia.org/wiki/Frecuencias_de_afinación_del_piano
// c normal  y c# sostenida
//Nota    Frecuencia   Preriodo   TiempoEnAlto
//c       261 Hz       3830       1915
//c#      277 Hz       3610       1805
//d       294 Hz       3400       1700
//d#      311 Hz       3215       1607
//e       329 Hz       3038       1519
//f       349 Hz       2864       1432
//f#      370 Hz       2702       1351
//g       392 Hz       2550       1275
//g#      415 Hz       2409       1204
//a       440 Hz       2272       1136
//a#      466 Hz       2145       1072
//b       493 Hz       2028       1014
//C       523 Hz       1912        956
//C#      554 Hz       1805        902
//D       587 HZ       1703        851
//D#      622 Hz       1607        803
//E       659 Hz       1517        758
//F       698 Hz       1432        716
//F#      740 Hz       1351        675
//G       783 Hz       1277        638
//G#      830 Hz       1204        602
//A       880 Hz       1136        568
//A#      932 Hz       1072        536
//B       987 Hz       1013        506
//¢      1046 Hz       956         478

int Longitud[] = {15, 15, 15, 6, 6, 36 };
char notes[6][50] = {
  {"ccggaagffeeddc "},//Melodia 0 >> Facebook
  {"ee eceg"},//Melodia 1 >> Youtube
  {"ccaacbcbaaged C"},//Melodia 2 >> Instagram
  {"ababab"},
  {""},//Pokemon
  {"cc#dd#eff#gg#aa#bCC#DD#EFF#GG#AA#BĆ"}
};

int beats[6][50] = {
  { 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 2, 4 },
  { 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 2, 4 },
  { 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 2, 4 },
  { 1, 2, 3, 3, 2, 1},
  { 2, 1, 1, 2, 1, 1},
  { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }
};

int tempo = 300;
float TiempoPasado = 0;

void Melodia(int Melodia, boolean Random) {
  for (int i = 0; i < Longitud[Melodia]; i++) {
    if (Random) MostarRandom(CantidadDisplay);
    if (notes[Melodia][i] == ' ') {
      delay(beats[Melodia][i] * tempo);
    } else {
      if (notes[Melodia][i + 1] == '#') {
        playNote(notes[Melodia][i], beats[Melodia][i] * tempo, true);
        i++;
      }
      else {
        playNote(notes[Melodia][i], beats[Melodia][i] * tempo, false);
      }
    }
  }
}

void playNote(char note, int duration, boolean sostenido) {
  int CantidadNotas = 25;
  char names[] = { 'c', '#', 'd', '#', 'e', 'f', '#', 'g', '#',  'a', '#', 'b',
                   'C', '#',  'D', '#',  'E', 'F', '#',  'G', '#',  'A', '#',  'B',
                   'Ć'
                 };
  int tones[] = { 1915, 1805, 1700, 1607, 1519, 1432, 1351, 1275,  1204, 1136, 1072, 1014,
                  956, 902, 851, 803, 758, 716, 675, 638, 602, 568, 536, 506,
                  478
                };
  for (int i = 0; i < CantidadNotas; i++) {
    if (names[i] == note) {
      Serial.print(note);
      if (sostenido) {
        Serial.print('#');
        playTone(tones[i + 1], duration);
      }
      else {
        playTone(tones[i], duration);
      }
      Serial.println("");
      delay(50);
    }
  }
}

void playTone(int tone, int duration) {
  for (long i = 0; i < duration * 500L; i += tone * 2) {
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

