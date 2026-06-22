//#define enableTest

// ========================
// BOTÓN START
// ========================
const int BOTON_START = 13;

enum Sensor {
  IZQUIERDO,
  DERECHO,
  TRASERO
};

struct DatosVelocidades {
  byte v3;
  byte v4;
  bool dir3;
  bool dir4;
};

DatosVelocidades dv;

void setup() {
  Serial.begin(9600);

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
  for(int i = 0; i < 256; i+=20) {
    dv.v3 = i;
    dv.v4 = 255 - i;
    if (i%40 == 0) {
      dv.dir3 = true;
      dv.dir4 = false;
    }
    else {
      dv.dir3 = false;
      dv.dir4 = true;
    }

    Serial.println("Velocidades------------");
    Serial.println("Vel3: " + String(dv.v3));
    Serial.println("Vel4: " + String(dv.v4));
    Serial.println("FW3: " + String(dv.dir3));
    Serial.println("FW4: " + String(dv.dir4));
    Serial.println("Velocidades------------\n");

    sendI2C(dv);

    delay(1000);
  }
}