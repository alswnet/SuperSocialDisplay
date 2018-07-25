
#ifdef YoutubeID

#include <YoutubeApi.h> //Libreria de Youtube
YoutubeApi api(API_KEY, client);


//Consulta para buscar cuantos subcriptores en Youtube
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

