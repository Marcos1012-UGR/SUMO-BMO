//#define enableTest

// ========================
// BOTÓN START
// ========================
const int BOTON_START = 13;

// ========================
// DEMO
// ========================
bool DEMO = false;

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
  else
    test2();
}



// FACING UP
// 4  
// 2  1