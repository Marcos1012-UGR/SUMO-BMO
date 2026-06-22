void setup() {
  Serial.begin(9600);

  Serial.println(">>>>>>>>>>>>>>>>>>>>>>MAIN: Sensor");
  setupSensor();

  Serial.println(">>>>>>>>>>>>>>>>>>>>>>MAIN: SHARP");
  setupSHARP();

  Serial.println(">>>>>>>>>>>>>>>>>>>>>>MAIN: I2C");
  setupI2C();

  Serial.println(">>>>>>>>>>>>>>>>>>>>>>MAIN: Servo");
  setupServo();

  Serial.println(">>>>>>>>>>>>>>>>>>>>>>MAIN: Motor");
  setupMotor();
}

void loop() {
  Serial.println(">>>>>>>>>>>>>>>>>>>>>>MAIN LOOP: Sensor");
  loopSensor();

  Serial.println(">>>>>>>>>>>>>>>>>>>>>>MAIN LOOP: SHARP");
  loopSHARP();

  Serial.println(">>>>>>>>>>>>>>>>>>>>>>MAIN LOOP: I2C");
  loopI2C();

  Serial.println(">>>>>>>>>>>>>>>>>>>>>>MAIN LOOP: Servo");
  loopServo();

  Serial.println(">>>>>>>>>>>>>>>>>>>>>>MAIN LOOP: Motor");
  test1();

  //delay(2000);
}