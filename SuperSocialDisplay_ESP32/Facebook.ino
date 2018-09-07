
#ifdef FacebookID
#include <FacebookApi.h>//Libreria de Facebook
FacebookApi *apifb;

void IniciarFacebook() {
  apifb = new FacebookApi(client, FACEBOOK_ACCESS_TOKEN, FACEBOOK_APP_ID, FACEBOOK_APP_SECRET);
}

//Consulta para buscar los seguidores una Fanpage de Facebook
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

