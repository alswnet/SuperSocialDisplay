void ConectarWifi() {

  wifiMulti.addAP(ssid1, password1);
  wifiMulti.addAP(ssid2, password2);
  wifiMulti.addAP(ssid3, password3);

  Serial.println("Conectando con Wifi...");
  if (wifiMulti.run() == WL_CONNECTED) {
    Serial.println("");
    Serial.println("WiFi Conectado");
    Serial.println("Mi IP es: ");
    Serial.println(WiFi.localIP());
  }

  /*bool Red1 = true;
    int CantidadIntentos = 10;
    int Intentos = CantidadIntentos ;
    bool Conectado = false;
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(1000);
    Serial.println("Iniciando Programa de SuperSocialDisplay 0.1");
    Serial.print("Conectando Wifi: ");
    Serial.println(ssid1);
    WiFi.begin(ssid1, password1);
    while (!Conectado) {
    if (WiFi.status() != WL_CONNECTED) {
      Serial.print("..");
      Serial.print(Intentos);
      digitalWrite(LedIndicador, 0);
      delay(500);
      digitalWrite(LedIndicador, 1);
      delay(500);
      Intentos--;
      if (Intentos < 0) {
        Intentos = CantidadIntentos ;
        Serial.println();
        Red1 = !Red1;
        if (Red1) {
          Serial.print("Conectando Wifi: ");
          Serial.println(ssid1);
          WiFi.begin(ssid1, password1);
        } else {
          Serial.print("Conectando Wifi: ");
          Serial.println(ssid2);
          WiFi.begin(ssid2, password2);
        }
      }
    } else {
      Conectado = true;
      Serial.println();
    }
    }
    Serial.println("WiFi Conectada");
    Serial.print("Direcion IP: ");
    IPAddress ip = WiFi.localIP();
    Serial.println(ip);
  */
}


void WifiActiva() {
  if (wifiMulti.run() != WL_CONNECTED) {
    Serial.println("Wifi No Conectada!");
    delay(1000);
  }
}
