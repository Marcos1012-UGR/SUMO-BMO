//#define enableTest

// ========================
// BOTÓN START
// ========================
const int BOTON_START = 13;

// ========================
// DEMO
// ========================
bool DEMO = false;

// ========================
// IA
// ========================
bool ATTACK = false;
int maxI, maxD, maxT, minI, minD, minT;
int UMBRAL_SENSORES = 80;
unsigned long lastI = 0;
unsigned long lastD = 0;
unsigned long lastT = 0;
const unsigned long HOLD_TIME = 1000;

bool last_i = false;
bool last_d = false;
bool last_t = false;
bool last_attack = false;
bool last_corrigiendo = false;

enum Sensor {
  IZQUIERDO,
  DERECHO,
  TRASERO
};

// ========================
// ESTRUCTURAS I2C
// ========================
struct DatosEncoders {
  long e3;
  long e4;
};

struct DatosVelocidades {
  byte v3;
  byte v4;
  bool dir3;
  bool dir4;
};

struct DatosVelocidadesMaestro {
  byte v1;
  byte v2;
  bool dir1;
  bool dir2;
};

DatosVelocidades dv;
DatosEncoders de;
DatosVelocidadesMaestro dvm;

void setup() {
  Serial.begin(9600);

  maxI = maxD = maxT = 0;
  minT = minD = minI = 4000;

  #ifdef enableTest
  Serial.println(">>>>>>>>>>>>>>>>>>>>>>MAIN: Sensor");
  setupSensor(); // Solo imprime por Serie
  #endif

  #ifdef enableTest
  Serial.println(">>>>>>>>>>>>>>>>>>>>>>MAIN: SHARP");
  setupSHARP(); // Solo imprime por Serie
  #endif

  #ifdef enableTest
  Serial.println(">>>>>>>>>>>>>>>>>>>>>>MAIN: I2C");
  #endif
  setupI2C();

  #ifdef enableTest
  Serial.println(">>>>>>>>>>>>>>>>>>>>>>MAIN: Servo");
  #endif
  setupServo();

  #ifdef enableTest
  Serial.println(">>>>>>>>>>>>>>>>>>>>>>MAIN: Motor");
  #endif
  setupMotor();
}

void loop() {
  #ifdef enableTest
  test();
  #else
  loopMain();
  #endif
}

void test() {
  Serial.println(">>>>>>>>>>>>>>>>>>>>>>MAIN LOOP: Sensor");
  testSensor();

  Serial.println(">>>>>>>>>>>>>>>>>>>>>>MAIN LOOP: SHARP");
  loopSHARP();

  Serial.println(">>>>>>>>>>>>>>>>>>>>>>MAIN LOOP: I2C");
  testI2C();

  Serial.println(">>>>>>>>>>>>>>>>>>>>>>MAIN LOOP: Servo");
  testServo();

  Serial.println(">>>>>>>>>>>>>>>>>>>>>>MAIN LOOP: Motor");
  test1();
}

void loopMain() {
  if (DEMO)
    demo();
  else {
    //test2();
    definitive();
  }
}

int * getSensores() {
  static int res[3];
  res[IZQUIERDO] = getSensor(IZQUIERDO);
  res[DERECHO] = getSensor(DERECHO);
  res[TRASERO] = getSensor(TRASERO);
  return res;
}

void IA() {
  if (!ATTACK) {
    delay(50);

    float sharpSensor = getSHARP();

    if (sharpSensor >= 2.0) {
      ATTACK = true;
    }

    int * sens;
    sens = getSensores();

    unsigned long now = millis();

    // IZQUIERDO
    if (sens[IZQUIERDO] <= UMBRAL_SENSORES) {
      lastI = now;
    }
    bool i = (now - lastI <= HOLD_TIME);

    // DERECHO
    if (sens[DERECHO] <= UMBRAL_SENSORES) {
      lastD = now;
    }
    bool d = (now - lastD <= HOLD_TIME);

    // TRASERO
    if (sens[TRASERO] <= UMBRAL_SENSORES) {
      lastT = now;
    }
    bool t = (now - lastT <= HOLD_TIME);

    if (i || d || t) {
      String aux = "Corrigiendo: " + String(i) + " (" + String(sens[IZQUIERDO]) + ") ";
      aux +=                         String(d) + " (" + String(sens[DERECHO]) + ") ";
      aux +=                         String(t) + " (" + String(sens[TRASERO]) + ") ";
      Serial.println(aux);
      if (millis() % 300 <= 80)
        Serial.println(aux);
      MovCorregir(i, d, t);
    } else {
      //Avanzar (cuidado lineas)
      Serial.println("Explorando...");
      MovExploracion();
    }
  }

  else {
    Serial.println("AL ATAQUEEE!!!");
    MovSinPutoMiedo();
  }
}

void AuxSensoresTest() {
  int * sens;
  sens = getSensores();

  maxI = sens[IZQUIERDO] > maxI ? sens[IZQUIERDO] : maxI;
  maxD = sens[DERECHO] > maxD ? sens[DERECHO] : maxD;
  maxT = sens[TRASERO] > maxT ? sens[TRASERO] : maxT;

  minI = sens[IZQUIERDO] < minI ? sens[IZQUIERDO] : minI;
  minD = sens[DERECHO] < minD ? sens[DERECHO] : minD;
  minT = sens[TRASERO] < minT ? sens[TRASERO] : minT;

  Serial.println("SI: " + String(sens[IZQUIERDO]) + " min " + String(minI) + " max " + String(maxI));
  Serial.println("SD: " + String(sens[DERECHO]) + " min " + String(minD) + " max " + String(maxD));
  Serial.println("ST: " + String(sens[TRASERO]) + " min " + String(minT) + " max " + String(maxT));
  Serial.println("");

  delay(500);
}

// FACING UP
// 4  3
// 2  1