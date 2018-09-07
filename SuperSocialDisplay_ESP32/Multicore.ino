void MultiCore( void * pvParameters ) {
  Serial.println("Procesos en Procesador 0 Iniciados");
  InicializarPantallas();
  strip.begin();
  MostarNumero(9999, CantidadDisplay);
  strip.show();

  while (true) {
    switch (Estado) {
      case 0:
        Serial.println("Esperando Wifi");
        colorWipe(strip.Color(255, 0, 0), 50);
        strip.show();
        break;
      case 1:
        Serial.println("Esperando RedesSociales");
        MostarNumero( 0, CantidadDisplay);
        colorWipe(strip.Color(0, 255, 0), 50);
        strip.show();
        break;
      case 2:
        Serial.println("---Datos----");
        colorWipe(strip.Color(0, 0, 0), 50); // Red
        strip.show();
        Estado = 3;
        break;
      case 3:
        CambiarDisplay();
        break;
      default:
        Serial.println("Habla con chepecarlos tenemos un problema");
        break;
    }
    digitalWrite(LedIndicador, 0);
    delay(500);
    digitalWrite(LedIndicador, 1);
    delay(500);

  }
}


void CambiarDisplay() {
  if (millis() > SiquienteCambioDisplay) {
    Mostar =  SiquienteRed(Mostar) ;
    MostarNumero(Sub[Mostar], CantidadDisplay);//Muestra el numero de Segidores
    SiquienteCambioDisplay = millis() + EsperaCambioDisplay;
  }
}


int SiquienteRed(int Actual) {
  Serial.println();
  while (true) {
    Actual++;
    if (Actual > 2 )
      Actual = 0;

#ifdef FacebookID
    if (Actual == Facebook) {
      Serial.print("Facebook ");
      return Actual;
    }
#endif

#ifdef InstagramID
    if (Actual == Instagram) {
      Serial.print("Instagram ");
      return Actual;
    }
#endif

#ifdef YoutubeID
    if (Actual == Youtube) {
      Serial.print("Youtube ");
      return Actual;
    }
#endif
  }
}


