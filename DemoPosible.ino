#include <YoutubeApi.h>
#include "InstagramStats.h"
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>
#include <FacebookApi.h>

#include "JsonStreamingParser.h"
char ssid[] = "TURBONETT_ALSW";       // your network SSID (name)
char password[] = "2526-4897";  // your network key
#define API_KEY "AIzaSyChaTNrG24ikJ2D8hAtL2LUTPgk8U-MSyg"  // your google apps API Token
#define CHANNEL_ID "UCS5yb75qx5GFOG-uV5JLYlQ" // makes up the url of channel

WiFiClientSecure client;
InstagramStats instaStats(client);
YoutubeApi api(API_KEY, client);

String FACEBOOK_ACCESS_TOKEN = "EAAMzCQes4NkBAB9kXSrBoMXx4IvN6i4kTEteVcb1ecS5uISZAiUG8DH0oiHUo8VCZBYXBpUSEh4ORPpHSD1a6pDDnFlZA1HkCBoOMZAhcxAIq5fNCLjZBE7wDE4aMrZCoAqqiuUuj0bAXqZClHNBkYoWOc8ml3EFBLyPODMDu1xV8siAP85cNxjRCcZA4qAbZAVMZD";    // not needed for the page fan count
String FACEBOOK_APP_ID = "900538806624473";
String FACEBOOK_APP_SECRET = "aacbf4b652ecf4be2290d40430c9f4d5";

FacebookApi *apifb;

unsigned long delayBetweenChecks = 20000; //mean time between api requests
unsigned long whenDueToCheck = 0;

String userName = "alswnet";

byte segmentClock = 13;
byte segmentLatch = 12;
byte segmentData = 16;

#define Facebook 0
#define Youtube 1
#define Instagram 2

const int Led = 5;
const int Pin[3] = {15, 4, 0};
const int Buzzer = 14;

int Sub[3] = {0, 0, 0};
void setup() {

  Serial.begin(115200);
  pinMode(Led, OUTPUT);

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  Serial.println("Iniciando Programa");
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    digitalWrite(Led, 0);
    delay(250);
    digitalWrite(Led, 1);
    delay(250);
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  IPAddress ip = WiFi.localIP();
  Serial.println(ip);

  apifb = new FacebookApi(client, FACEBOOK_ACCESS_TOKEN, FACEBOOK_APP_ID, FACEBOOK_APP_SECRET);

  pinMode(segmentClock, OUTPUT);
  pinMode(segmentData, OUTPUT);
  pinMode(segmentLatch, OUTPUT);
  pinMode(Buzzer, OUTPUT);

  digitalWrite(segmentClock, LOW);
  digitalWrite(segmentData, LOW);
  digitalWrite(segmentLatch, LOW);
  for (int i = 0; i < 3 ; i++) {
    pinMode(Pin[i], OUTPUT);
  }
  Serial.println("---Datos----");
}

boolean getInstagramStatsForUser() {
  InstagramUserStats response = instaStats.getUserStats(userName);
  if (Sub[Instagram] < response.followedByCount) {
    Sub[Instagram] = response.followedByCount;
    Serial.print("Instagram: ");
    Serial.println(Sub[Instagram]);
    return true;
  }
  return false;
}

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

boolean getFacebook() {
  int pageLikes = apifb->getPageFanCount("163069780414846");
  if (Sub[Facebook] < pageLikes) {
    Sub[Facebook] = pageLikes;
    Serial.print("Facebook: ");
    Serial.println(Sub[Facebook]);
    return true;
  }
  return false;
}

//note    frequency    period    timeHigh
//c       261 Hz       3830       1915
//d       294 Hz       3400       1700
//e       329 Hz       3038       1519
//f       349 Hz       2864       1432
//g       392 Hz       2550       1275
//a       440 Hz       2272       1136
//b       493 Hz       2028       1014
//C       523 Hz       1912        956

int length = 15; // the number of notes
char notes[] = "ccggaagffeeddc "; // a space represents a rest
int beats[] = { 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 2, 4 };
int tempo = 300;

void Tono() {
  for (int i = 0; i < length; i++) {
    if (notes[i] == ' ') {
      delay(beats[i] * tempo); // rest
    } else {
      playNote(notes[i], beats[i] * tempo);
    }
  }
}

void playNote(char note, int duration) {
  char names[] = { 'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C' };
  int tones[] = { 1915, 1700, 1519, 1432, 1275, 1136, 1014, 956 };

  // play the tone corresponding to the note name
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

void loop() {
  unsigned long timeNow = millis();
  boolean SiTono = false;
  if ((timeNow > whenDueToCheck))  {
    SiTono = SiTono || getYoutube();
    SiTono = SiTono || getInstagramStatsForUser();
    SiTono = SiTono || getFacebook();
    showNumber(10);
    whenDueToCheck = timeNow + delayBetweenChecks;
  }
  digitalWrite(Led, 0);
  delay(100);
  digitalWrite(Led, 1);
  delay(100);
  if (SiTono) {
    Tono();
    SiTono = false;
  }
}

//Takes a number and displays 2 numbers. Displays absolute value (no negatives)
void showNumber(float value)
{
  int number = abs(value); //Remove negative signs and any decimals

  for (byte x = 0 ; x < 2 ; x++)
  {
    int remainder = number % 10;

    postNumber(remainder, false);

    number /= 10;
  }

  //Latch the current segment data
  digitalWrite(segmentLatch, LOW);
  digitalWrite(segmentLatch, HIGH); //Register moves storage register on the rising edge of RCK
}

//Given a number, or '-', shifts it out to the display
void postNumber(byte number, boolean decimal)
{
  //    -  A
  //   / / F/B
  //    -  G
  //   / / E/C
  //    -. D/DP

#define a  1<<0
#define b  1<<6
#define c  1<<5
#define d  1<<4
#define e  1<<3
#define f  1<<1
#define g  1<<2
#define dp 1<<7

  byte segments;

  switch (number)
  {
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

  //Clock these bits out to the drivers
  for (byte x = 0 ; x < 8 ; x++)
  {
    digitalWrite(segmentClock, LOW);
    digitalWrite(segmentData, segments & 1 << (7 - x));
    digitalWrite(segmentClock, HIGH); //Data transfers to the register on the rising edge of SRCK
  }
}
