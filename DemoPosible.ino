
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

void setup() {

  Serial.begin(115200);

  // Set WiFi to station mode and disconnect from an AP if it was Previously
  // connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  // Attempt to connect to Wifi network:
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  IPAddress ip = WiFi.localIP();
  Serial.println(ip);

  apifb = new FacebookApi(client, FACEBOOK_ACCESS_TOKEN, FACEBOOK_APP_ID, FACEBOOK_APP_SECRET);

}

void getInstagramStatsForUser() {
  InstagramUserStats response = instaStats.getUserStats(userName);
  Serial.print("Instagram: ");
  Serial.println(response.followedByCount);
}

void getYoutube() {
  if (api.getChannelStatistics(CHANNEL_ID)) {
    Serial.print("Youtube: ");
    Serial.println(api.channelStats.subscriberCount);
  }
}

void getFacebook() {
  int pageLikes = apifb->getPageFanCount("163069780414846");
    Serial.print("Facebook: ");
    Serial.println(pageLikes);
}

void loop() {
  unsigned long timeNow = millis();
  if ((timeNow > whenDueToCheck))  {
    Serial.println("---Datos----");
    getInstagramStatsForUser();
    getYoutube();
    getFacebook();
    whenDueToCheck = timeNow + delayBetweenChecks;
  }
}
