
#include <YoutubeApi.h>
#include "InstagramStats.h"
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>


#include "JsonStreamingParser.h"
char ssid[] = "TURBONETT_ALSW";       // your network SSID (name)
char password[] = "2526-4897";  // your network key
#define API_KEY "AIzaSyChaTNrG24ikJ2D8hAtL2LUTPgk8U-MSyg"  // your google apps API Token
#define CHANNEL_ID "UCS5yb75qx5GFOG-uV5JLYlQ" // makes up the url of channel


WiFiClientSecure client;
InstagramStats instaStats(client);
YoutubeApi api(API_KEY, client);


unsigned long delayBetweenChecks = 10000; //mean time between api requests
unsigned long whenDueToCheck = 0;

//Inputs
String userName = "alswnet"; // from their instagram url https://www.instagram.com/brian_lough/


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
}

void getInstagramStatsForUser() {
  Serial.println("Getting instagram user stats for " + userName );
  InstagramUserStats response = instaStats.getUserStats(userName);
  Serial.println("Response:");
  Serial.print("Number of followers: ");
  Serial.println(response.followedByCount);
}

void getYoutube() {
  if (api.getChannelStatistics(CHANNEL_ID))
  {
    Serial.println("---------Stats---------");
    Serial.print("Subscriber Count: ");
    Serial.println(api.channelStats.subscriberCount);
  }
}

void loop() {
  unsigned long timeNow = millis();
  if ((timeNow > whenDueToCheck))  {
    getInstagramStatsForUser();
    getYoutube();
    whenDueToCheck = timeNow + delayBetweenChecks;
  }
}
