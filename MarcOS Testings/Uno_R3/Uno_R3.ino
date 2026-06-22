//#define enableTest

enum Sensor {
  IZQUIERDO,
  DERECHO,
  TRASERO
};

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
  loopI2C();

  Serial.println(">>>>>>>>>>>>>>>>>>>>>>MAIN LOOP: Servo");
  testServo();

  Serial.println(">>>>>>>>>>>>>>>>>>>>>>MAIN LOOP: Motor");
  test1();
}

void loopMain() {

  

}