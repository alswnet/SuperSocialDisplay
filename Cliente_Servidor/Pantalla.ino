
int segmentClock = 13;//
int segmentLatch = 12;//
int segmentData = 16;//
int ValocidadBarrido = 500;

void InicializarPantallas() {
  pinMode(segmentClock, OUTPUT);
  pinMode(segmentData, OUTPUT);
  pinMode(segmentLatch, OUTPUT);

  digitalWrite(segmentClock, LOW);
  digitalWrite(segmentData, LOW);
  digitalWrite(segmentLatch, LOW);

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
