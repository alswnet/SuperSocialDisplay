#ifdef InstagramID
#include "InstagramStats.h"//Libreria de Instagram
InstagramStats instaStats(client);

//Consulta para buscar cuando segirores en Instagram
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
