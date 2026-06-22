//#define enableTest

volatile long encoder3 = 0;
volatile long encoder4 = 0;

struct DatosVelocidades {
  byte v3;
  byte v4;
  bool dir3;
  bool dir4;
};

struct DatosEncoders {
  long e3;
  long e4;
};

DatosVelocidades dv;
DatosEncoders de;

// CONTROL INTERRUPTS EN ENVIO
bool ACCESS = false;

void setup() {
  setupI2C();
  setupMotor();
}

void loop() {
  if(ACCESS) {
    noInterrupts();
    resetE3();
    resetE4();
    interrupts();
    ACCESS = false;
  }
  setMotor(dv);
  //Serial.println(getE3());
  delay(10);
}
