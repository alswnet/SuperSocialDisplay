//ID de Redes Sociales
#define InstagramID "alswnet"//usuario de Insgramam
//#define FacebookID "163069780414846"//ID de fanpage de Facebook
#define YoutubeID "UCS5yb75qx5GFOG-uV5JLYlQ" // ID de Canal de Youtube

#define Facebook 0
#define Youtube 1
#define Instagram 2

#include <WiFi.h>//Libreria de ESP8266
#include <WiFiMulti.h>
#include <WiFiClientSecure.h> //Libreria de Consultas Escriptadas
#include "Contrasenna.h"//Archivo con info de contrasenas para hacer las consultas

WiFiClientSecure client;

#include <ArduinoJson.h>//Libreria de Decifrado Json
#include "JsonStreamingParser.h"///Libreria de Decifrado Json
#include <Adafruit_NeoPixel.h>
Adafruit_NeoPixel strip = Adafruit_NeoPixel(13, 4, NEO_GRB + NEO_KHZ800);

//Configuraciones de RED
char ssid3[] = "ALSW2"; //Nombre de Red 1
char password3[] = "7210-3607";  //Contrasenna de Red 1
char ssid2[] = "ALSW"; //Nombre de Red 2
char password2[] = "2526-4897";  //Contrasenna de Red 2
char ssid1[] = "ALSW2"; //Nombre de Red 1
char password1[] = "7210-3607";  //Contrasenna de Red 1

unsigned long EsperaEstreConsulta = 1000;//cada n/1000 segundos
unsigned long EsperaCambioDisplay = 100000;//cada n/1000 Segundo
unsigned long SiquientePreguntaAPI = 0;
unsigned long SiquienteCambioDisplay = 0;
unsigned long TiempoActual = 0;
unsigned long ValocidadBarrido = 300;


const int LedIndicador = 5;
const int PinLed[3] = {15, 0, 0};
const int CantidadDisplay = 4;
int Mostar = 1;
int Sub[3] = {0, 0, 0};

void setup() {

  Serial.begin(115200);
  pinMode(LedIndicador, OUTPUT);
  InicializarPantallas();

  MostarNumero(9999, CantidadDisplay);
  strip.begin();
  strip.show();

  colorWipe(strip.Color(255, 0, 0), 50); // Red
  strip.show();
  ConectarWifi();
  colorWipe(strip.Color(0, 255, 0), 50); // Red
  strip.show();

  MostarNumero( 0, CantidadDisplay);


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
    Mostar =  SiquienteRed(Mostar) ;
    MostarNumero(Sub[Mostar], CantidadDisplay);//Muestra el numero de Segidores
    SiquienteCambioDisplay = TiempoActual + EsperaCambioDisplay;
  }
}

int  SiquienteRed(int Actual) {
  while (true) {
    Actual++;
    if (Actual > 2 )
      Actual = 0;

#ifdef FacebookID
    if (Actual == Facebook)
      return Actual;
#endif

#ifdef InstagramID
    if (Actual == Instagram)
      return Actual;
#endif

#ifdef YoutubeID
    if (Actual == Youtube)
      return Actual;
#endif
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
    Serial.print("Buscando.");
#ifdef FacebookID
    Serial.println(".F.");
    if (NuevoSegidor || getFacebook()) {
      // Melodia(Facebook, true);
    }
#endif
#ifdef InstagramID
    Serial.print(".I.");
    if (NuevoSegidor || getInstagram()) {
      // Melodia(Instagram, true);
    }
#endif
#ifdef YoutubeID
    Serial.print(".Y.");
    if (NuevoSegidor || getYoutube()) {
      rainbow(20);
      //  Melodia(Youtube, true);
      colorWipe(strip.Color(0, 0, 0), 50); // Red
      strip.show();
    }
#endif
    if (NuevoSegidor ) {
      Serial.println(".Si");
    }
    else {
      Serial.println(".NO");
    }
    SiquientePreguntaAPI = TiempoActual + EsperaEstreConsulta;
  }
}


